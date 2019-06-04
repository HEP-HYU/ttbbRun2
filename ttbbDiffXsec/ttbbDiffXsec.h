#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TMath.h>

#include "../include/histBook.h"
#include "../include/tdrstyle.C"

const int _DELTAR = 0;
const int _INVMASS = 1;

string output_dir = "../output/unfold/";
TGraphErrors *buildGraphFromHist(TH1* h){
  TGraphErrors *grp = new TGraphErrors();

  for ( int i=1; i<=h->GetNbinsX(); ++i ) {
    const double w = h->GetXaxis()->GetBinWidth(i);
    const double y = h->GetBinContent(i);
    const double err = h->GetBinError(i);
    grp->SetPoint(i-1, h->GetBinCenter(i), y);
    //grp->SetPointError(i-1, w/2, err);
    grp->SetPointError(i-1, 0, err);
  }
  grp->SetEditable(false);

  return grp;
}

TH1 *calculateDiffXsec(TH1 *h_in, TH1 *h_acceptance, int variable, bool gen = false){
  const int dR = 0, invMass = 1;
  string var;
  if(variable == 0) var = "deltaR";
  else if(variable == 1) var = "invMass";

  TH1 *h_out = (TH1 *)h_in->Clone(); 
  double incXsec = 0.0; double incXsec_err = 0.0;
  cout << "--------------------------------------------------------"<< endl;
  if(gen){
    std::cout << h_in->GetName() << std::endl;

    for(int ibin=1; ibin<=h_in->GetNbinsX(); ++ibin){
      const double bin_width = h_in->GetXaxis()->GetBinWidth(ibin);
      const double diffXsec = h_in->GetBinContent(ibin)/(bin_width*LUMINOSITY_*BRANCHINGRATIO_);
      const double diffXsec_err = h_in->GetBinError(ibin)/(bin_width*LUMINOSITY_*BRANCHINGRATIO_); 
      
      h_out->SetBinContent(ibin, diffXsec);
      h_out->SetBinError(ibin, diffXsec_err);
      incXsec += diffXsec*bin_width;
      incXsec_err += pow(diffXsec_err*bin_width,2);
      cout << ibin << "th diffXsec : " << diffXsec <<
	" || err : " << diffXsec_err << " || bin width : " << bin_width << endl;
    }
    cout << "inclusive Xsection : " << incXsec << " || error : " << sqrt(incXsec_err) << endl;
    cout << "--------------------------------------------------------"<< endl;
  }
  else{
    std::cout << h_in->GetName() << endl;

    for(int iBin=1; iBin<=h_in->GetNbinsX(); ++iBin){
      // diffXsec = (1/bin width)*(nevt/accp*lumi)
      double bin_width = h_in->GetXaxis()->GetBinWidth(iBin);
      double diffXsec = h_in->GetBinContent(iBin)/
        (h_acceptance->GetBinContent(iBin)*bin_width*LUMINOSITY_*BRANCHINGRATIO_);
      double diffXsec_err = h_in->GetBinError(iBin)/
	(h_acceptance->GetBinContent(iBin)*bin_width*LUMINOSITY_*BRANCHINGRATIO_);
      if(h_acceptance->GetBinContent(iBin) == 0.0){
        diffXsec = 0.0;
	diffXsec_err = 0.0;
      }
      h_out->SetBinContent(iBin, diffXsec);
      h_out->SetBinError(iBin, diffXsec_err);
      incXsec += diffXsec*bin_width;
      incXsec_err += pow(diffXsec_err*bin_width,2);
      cout << iBin << "th diffXsec : " << diffXsec <<
	" || err : " << diffXsec_err << " || bin width : " << bin_width << endl;
    }
    cout << "inclusive Xsection : " << incXsec << " || error : " << sqrt(incXsec_err) << endl; 
    cout << "--------------------------------------------------------"<< endl;
  }
  
  return h_out;
}

void findBestk(vector<TH1 *>h_in_, int data, int variable, int channel, TH1 *h_truth_){
  gROOT->ProcessLine("setTDRStyle();"); 
  
  vector<string> var = {"deltaR", "invMass"};
  vector<string> dat = {"Data", "MC"};
  vector<int> markerStyle = { kFullCircle, kFullSquare, kFullTriangleUp, kFullTriangleDown,
  kFullStar, kFullDiamond, kFullCross, kFullCrossX, kFullFourTrianglesPlus, kOpenStar};
  vector<int> colorStyle = { kGreen, kYellow, kOrange, kRed, kPink, kViolet, kBlue, kTeal,
    kSpring+4, kPink-5};

  auto h_in = h_in_;
  auto h_truth = h_truth_;

  TPaveText *label_cms = tdrCMSlabel();
  TCanvas *c = new TCanvas("","",800,800);
  TLegend *leg = new TLegend(0.75,0.7,0.88,0.88);

  vector<TGraph *> v_grp;
  vector<TGraph *> v_grp2;
  for(auto v_itr = h_in.begin(); v_itr != h_in.end(); ++v_itr){
    TH1 *h_tmp = *v_itr;
    TGraph *grp = new TGraph();
    TGraph *grp2 = new TGraph();

    for(int j=1; j<=h_tmp->GetNbinsX(); ++j){
      const double y = (h_tmp->GetBinError(j)/h_tmp->GetBinContent(j))*100;
      grp->SetPoint(j-1, h_tmp->GetBinCenter(j), y);

      const double yy = (h_truth->GetBinContent(j)-h_tmp->GetBinContent(j))/h_truth->GetBinContent(j);
      grp2->SetPoint(j-1, h_tmp->GetBinCenter(j), yy);
    }
    v_grp.push_back(grp);
    v_grp2.push_back(grp2);
  }

  TGraph *grp_truth = new TGraph();
  for(int i=1; i<=h_truth->GetNbinsX(); ++i){
    const double y = (sqrt(h_truth->GetBinContent(i))/h_truth->GetBinContent(i))*100;
    grp_truth->SetPoint(i-1, h_truth->GetBinCenter(i), y);
  }
  grp_truth->SetLineStyle(8);
  grp_truth->SetLineColor(kSpring+4);
  grp_truth->SetMarkerColor(kSpring+4);
  grp_truth->SetMarkerStyle(kOpenStar);

  c->cd();
  TH1 *h_tmp = (TH1 *)(h_in.at(0))->Clone();
  h_tmp->SetMaximum(80);
  h_tmp->SetMinimum(-0.01);
  h_tmp->GetYaxis()->SetTitleOffset(1.6);
  h_tmp->GetYaxis()->SetTitleSize(0.04);
  h_tmp->GetYaxis()->SetTitle("Statistical Uncertainty(%)");
  h_tmp->GetXaxis()->SetTitleOffset(1.2);
  if(var.at(variable) == "deltaR") h_tmp->GetXaxis()->SetTitle("#DeltaR_{b#bar{b}}");
  else h_tmp->GetXaxis()->SetTitle("m_{b#bar{b}}");
  h_tmp->Draw("axis");
  for(int i=0; i<v_grp.size(); ++i){
    v_grp.at(i)->SetMarkerSize(1.5);
    v_grp.at(i)->SetMarkerColor(colorStyle.at(i));
    v_grp.at(i)->SetMarkerStyle(markerStyle.at(i));
    leg->AddEntry(v_grp.at(i), Form("k = %d",i+2), "p");
    
    v_grp.at(i)->Draw("C p same");
  }
  leg->AddEntry(grp_truth, "MC input", "lp");
  grp_truth->Draw("C p same");
  
  leg->Draw("same");
  label_cms->Draw("same");
  c->Print(Form("%s/SVD_statErr%s_Ch%d_%s.pdf",
	output_dir.c_str(), (dat.at(data)).c_str(), channel, (var.at(variable)).c_str()),"pdf");
  leg->Clear();
  c->Clear();

  c->cd();
  TH1 *h_tmp2 = (TH1 *)(h_in.at(0))->Clone();
  h_tmp2->SetMaximum(0.3);
  h_tmp2->SetMinimum(-0.5);
  h_tmp2->GetYaxis()->SetTitleOffset(1.6);
  h_tmp2->GetYaxis()->SetTitleSize(0.04);
  h_tmp2->GetYaxis()->SetTitle("(N_{truth}-N_{unfolded})/N_{truth}");
  h_tmp2->GetXaxis()->SetTitleOffset(1.2);
  if(var.at(variable) == "deltaR") h_tmp2->GetXaxis()->SetTitle("#DeltaR_{b#bar{b}}");
  else h_tmp2->GetXaxis()->SetTitle("m_{b#bar{b}}");
  h_tmp2->Draw("axis");
  for(int i=0; i<v_grp2.size(); ++i){
    v_grp2.at(i)->SetMarkerSize(1.5);
    v_grp2.at(i)->SetMarkerColor(colorStyle.at(i));
    v_grp2.at(i)->SetMarkerStyle(markerStyle.at(i));
    leg->AddEntry(v_grp2.at(i), Form("k = %d", i+2), "p");

    v_grp2.at(i)->Draw("C p same");
  }
  leg->Draw("same");
  label_cms->Draw("same");
  c->Print(Form("%s/SVD_gap%s_Ch%d_%s.pdf",
	output_dir.c_str(), (dat.at(data)).c_str(), channel, (var.at(variable)).c_str()), "pdf");
  leg->~TLegend();
  c->~TCanvas();
}

enum opt_legend {
  kUnfoldedMC = 0, kReco, kUnfoldedData, kSubDataBkg,
  kRespX, kRespY, kPowhegPythia, kPowhegHerwig, kPowhegPythiaEvtgen, kaMCatNLOPythia};

void DrawHist(string outname, TH1 *h_in1, TH1 *h_in2,
    int opt1, int opt2, bool dR = false, 
    bool useTUnfold = false, bool diffXsec = false, int reg = 0,
    bool h1togrp = false, bool h2togrp = false){
  
  gROOT->ProcessLine("setTDRStyle();"); 
  vector<string> v_legname = {
    "t#bar{t}b#bar{b} MC", "Unfolded data", "Data - Background",
    "Powheg+Pythia", "Powheg+Herwig", "Powheg+Pythia+Evtgen", "aMCatNLO+Pythia",
    "X-axis projection", "Y-axis projection"
  };

  TCanvas *c = new TCanvas("","",800,800);
  TLegend *leg = new TLegend(0.6,0.7,0.88,0.88);
  TPaveText *label_cms;
  
  if(opt1 == kUnfoldedData || opt1 == kSubDataBkg) label_cms = tdrCMSlabel();
  else label_cms = tdrCMSSimlabel();

  if(opt1 == kUnfoldedMC || opt1 == kReco){
    h_in1->SetLineColor(kBlue);
    h_in1->SetLineWidth(2);
  }
  else{
    h_in1->SetLineColor(kBlack);
    h_in1->SetLineWidth(2);
  }
  
  h_in2->SetLineColor(kRed);
 if(diffXsec)  h_in2->SetFillColor(kRed);
 else h_in2->SetLineWidth(2);

  TGraphErrors *grp1;
  if(h1togrp){
    grp1 = buildGraphFromHist(h_in1);
    if(opt1 == kUnfoldedMC || opt1 == kReco){
      grp1->SetLineColor(kBlue);
      grp1->SetMarkerColor(kBlue);
    }
    else{
      grp1->SetLineColor(kBlack);
      grp1->SetMarkerColor(kBlack);
    }
  }

  if(reg > 0 && !(useTUnfold))
    leg->SetHeader(Form("k = %d",reg),"C");

  if(h1togrp){
    switch(opt1){
      case kUnfoldedMC:
	leg->AddEntry(grp1, v_legname[0].c_str(), "lp");
	break;
      case kReco:
	leg->AddEntry(grp1, v_legname[0].c_str(), "lp");
	break;
      case kUnfoldedData:
	leg->AddEntry(grp1, v_legname[1].c_str(), "lp");
	break;
      case kSubDataBkg:
	leg->AddEntry(grp1, v_legname[2].c_str(), "lp");
	break;
    }
  }
  else{
    switch(opt1){
      case kUnfoldedMC:
	leg->AddEntry(h_in1, v_legname[0].c_str(), "l");
	break;
      case kReco:
	leg->AddEntry(h_in1, v_legname[0].c_str(), "l");
	break;
      case kUnfoldedData:
	leg->AddEntry(h_in1, v_legname[1].c_str(), "l");
	break;
      case kSubDataBkg:
	leg->AddEntry(h_in1, v_legname[2].c_str(), "l");
	break;
    }
  }

  std::string draw;
  if(diffXsec) draw = "F";
  else draw = "l";

  switch(opt2){
    case kReco:
      leg->AddEntry(h_in2, v_legname[0].c_str(), draw.c_str());
      break;
    case kRespX:
      leg->AddEntry(h_in2, v_legname[7].c_str(), draw.c_str());
      break;
    case kRespY:
      leg->AddEntry(h_in2, v_legname[8].c_str(), draw.c_str());
      break;
    case kPowhegPythia:
      leg->AddEntry(h_in2, v_legname[3].c_str(), draw.c_str());
      break;
    case kPowhegHerwig:
      leg->AddEntry(h_in2, v_legname[4].c_str(), draw.c_str());
      break;
    case kPowhegPythiaEvtgen:
      leg->AddEntry(h_in2, v_legname[5].c_str(), draw.c_str());
      break;
    case kaMCatNLOPythia:
      leg->AddEntry(h_in2, v_legname[6].c_str(), draw.c_str());
  }

  h_in1->GetYaxis()->SetTitleOffset(1.6);
  h_in1->GetXaxis()->SetTitleOffset(1.2);
  h_in1->SetMinimum(0.0);
  if(diffXsec && dR){
    h_in1->GetYaxis()->SetTitleSize(0.04);
    h_in1->GetYaxis()->SetTitle("#frac{d#sigma^{full}}{d#DeltaR}[pb]");
    h_in1->SetMaximum(4.0);
  }
  else if(diffXsec && !(dR)){
    h_in1->GetYaxis()->SetTitleSize(0.04);
    h_in1->GetYaxis()->SetLabelSize(0.025);
    h_in1->GetYaxis()->SetTitle("#frac{d#sigma^{full}}{dm_{b#bar{b}}}[pb/GeV]");
    h_in1->SetMaximum(0.05);
  }
  else{
    h_in1->GetYaxis()->SetTitle("Events");
    h_in1->GetYaxis()->SetTitleSize(0.04);
    h_in1->SetMaximum(h_in1->GetMaximum()*2.0);
  }

  if(dR) h_in1->GetXaxis()->SetTitle("#DeltaR_{b#bar{b}}");
  else h_in1->GetXaxis()->SetTitle("m_{b#bar{b}}[GeV]");

  h_in1->Draw("axis");
  
  if(h1togrp){
    h_in2->Draw("hist same");
    grp1->Draw("p e same");
  }
  else{
    h_in2->Draw("hist same");
    h_in1->Draw("hist same");
  }
  leg->Draw("same");
  label_cms->Draw("same");

  c->Print(Form("%s/%s.pdf", output_dir.c_str(), outname.c_str()),"pdf");
}

TGraphAsymmErrors * CalculateUncertainty(
    std::vector<std::vector<double>> v_diff_sys, std::vector<std::vector<double>> v_diff_gen,
    TH1 *h_data){
  const bool addsys = true;
  const bool addgens = false;
  TGraphAsymmErrors *grp = new TGraphAsymmErrors();
  
  ofstream writeFile(Form("%s/errors.txt",output_dir.c_str()),ios::app);
 
  writeFile << h_data->GetName() << endl;

  std::vector<std::string> v_sys = {"FSRup", "FSRdown", "ISRup", "ISRdown"};
  std::vector<std::string> v_gen = {"PowhegHerwig", "PowhegPythiaEvtgen", "aMCatNLOPythia"};

  for(int ibin = 1; ibin <= h_data->GetNbinsX(); ++ibin){
    writeFile << ibin << "th bin" << endl;
    
    const double w = h_data->GetXaxis()->GetBinWidth(ibin);
    double stat_error_sq = pow(h_data->GetBinError(ibin)/h_data->GetBinContent(ibin),2);
    double sys_error_up_sq = 0.0;
    double sys_error_down_sq = 0.0;
    double gen_error_up_sq = 0.0;
    double gen_error_down_sq = 0.0;
    
    int tmp = 0;
    if(addsys){
      for(auto v_itr = v_diff_sys.begin(); v_itr != v_diff_sys.end(); ++v_itr){
	if((*v_itr)[ibin] >= 0){
	  sys_error_up_sq += TMath::Power(TMath::Abs((*v_itr)[ibin]),2);
	  writeFile << v_sys[tmp] << " up : " << (*v_itr)[ibin] 
	    <<" (" <<(*v_itr)[ibin]*100 << "%)" << endl;
	  writeFile << "sum : " << sqrt(sys_error_up_sq) << endl;
	}
	else{
	  sys_error_down_sq += TMath::Power(TMath::Abs((*v_itr)[ibin]),2);
	  writeFile << v_sys[tmp] << " down : " << (*v_itr)[ibin]
	    <<" (" <<(*v_itr)[ibin]*100 << "%)" << endl;
	  writeFile << "sum : " << sqrt(sys_error_down_sq) << endl;
	}
	++tmp;
      }
    }
    tmp = 0;
    if(addgens){
      for(auto v_itr = v_diff_gen.begin(); v_itr != v_diff_gen.end(); ++v_itr){
	if((*v_itr)[ibin] >= 0){
	  gen_error_up_sq += TMath::Power(TMath::Abs((*v_itr)[ibin]),2);
	  writeFile << v_gen[tmp] << " up : " << (*v_itr)[ibin]
	    <<" (" <<(*v_itr)[ibin]*100 << "%)" << endl;
	  writeFile << "sum : " << sqrt(gen_error_up_sq) << endl;
	}
	else{
	  gen_error_down_sq += TMath::Power(TMath::Abs((*v_itr)[ibin]),2);
	  writeFile << v_gen[tmp] << " down : " << (*v_itr)[ibin]
	    <<" (" <<(*v_itr)[ibin]*100 << "%)" << endl;
	  writeFile << "sum : " << sqrt(gen_error_down_sq) << endl;
	}
	++tmp;
      }
    }
    double error_up = 
      sqrt(stat_error_sq + sys_error_up_sq + gen_error_up_sq)*h_data->GetBinContent(ibin);
    double error_down = 
      sqrt(stat_error_sq + sys_error_down_sq + gen_error_down_sq)*h_data->GetBinContent(ibin);

    writeFile << "square error up = stat : " << stat_error_sq 
      << " sys : " << sys_error_up_sq << " gen : " << gen_error_up_sq << endl;
    writeFile << "square error down = stat : " << stat_error_sq 
      << " sys : "<< sys_error_down_sq << " gen : "<< gen_error_down_sq << endl;

    grp->SetPoint(ibin-1, h_data->GetBinCenter(ibin), h_data->GetBinContent(ibin));
    grp->SetPointEXhigh(ibin-1, w/2);
    grp->SetPointEXlow(ibin-1, w/2);
    grp->SetPointEYhigh(ibin-1, error_up);
    grp->SetPointEYlow(ibin-1, error_down);
  }
  writeFile.close();
  return grp;
}
