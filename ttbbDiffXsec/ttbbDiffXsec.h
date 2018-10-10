#include <TFile.h>
#include <TGraphErrors.h>
#include <TCanvas.h>

#include "../include/histBook.h"
#include "../include/tdrstyle.C"

const int _DELTAR = 0;
const int _INVMASS = 1;

using namespace std;
string input = "../output/root/";
string input_sys = "../output/root/";

/*
void calculateSysError(TH1 *h_in, int variable){
  for(int iBin=1; iBin<h_in->GetNbinsX()+1; ++iBin){
    if(variable == 0){
      auto tmp = h_in->GetBinError(iBin);
      h_in->SetBinError(iBin, sqrt(pow(tmp,2)+pow(errME_dR[iBin-1],2)+pow(errPS_dR[iBin-1],2)));
      cout << iBin << "th error" << endl;
      cout << tmp << "(stat.) + " << sqrt(pow(errME_dR[iBin-1],2)+pow(errPS_dR[iBin-1],2)) << "(sys.) = " << h_in->GetBinError(iBin) << endl;
    }
    else if(variable == 1){
      auto tmp = h_in->GetBinError(iBin);
      h_in->SetBinError(iBin, sqrt(pow(tmp,2)+pow(errME_M[iBin-1],2)+pow(errPS_M[iBin-1],2)));
      cout << iBin << "th error" << endl;
      cout << tmp << "(stat.) + " << sqrt(pow(errME_M[iBin-1],2)+pow(errPS_M[iBin-1],2)) << "(sys.) = " << h_in->GetBinError(iBin) << endl;

    }
  }
}
*/

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

TH1 *calculateDiffXsec(TH1 *h_in,int variable, int channel, bool data){
  const int dR = 0, invMass = 1;
  string var;
  if(variable == 0) var = "deltaR";
  else if(variable == 1) var = "invMass";
  
  TH1 *h_out = (TH1 *)h_in->Clone();
  TFile *f_stability = TFile::Open(Form("%shist_stability_ttbb.root",input.c_str()));
  TH1 *h_acceptance;
  if(variable == dR)
    h_acceptance = (TH1 *)f_stability->Get(Form("h_%s_Ch%d_%s",BIN_ACCEPTANCE_DELTAR_,channel,NAME_[TTBB_].c_str()));
  else if(variable == invMass)
    h_acceptance = (TH1 *)f_stability->Get(Form("h_%s_Ch%d_%s",BIN_ACCEPTANCE_INVARIANT_MASS_,channel,NAME_[TTBB_].c_str()));

  double incXsec = 0.0; double incXsec_err = 0.0;
  cout << "--------------------------------------------------------"<< endl;
  if(data) cout << "##### Unfolded Data #####" << endl;
  else cout << "##### Unfolded MC #####" << endl;
  cout << var << " Channel : " << channel << endl;
  for(int iBin=1; iBin<=h_in->GetNbinsX(); ++iBin){
    // diffXsec = (1/bin width)*(nevt/accp*lumi)
    const double bin_width = h_in->GetXaxis()->GetBinWidth(iBin);
    const double diffXsec = h_in->GetBinContent(iBin)/(h_acceptance->GetBinContent(iBin)*bin_width*LUMINOSITY_*BRANCHINGRATIO_);
    const double diffXsec_err = h_in->GetBinError(iBin)/(h_acceptance->GetBinContent(iBin)*bin_width*LUMINOSITY_*BRANCHINGRATIO_);
    h_out->SetBinContent(iBin,diffXsec);
    h_out->SetBinError(iBin, diffXsec_err);
    incXsec += diffXsec*bin_width;
    incXsec_err += pow(diffXsec_err*bin_width,2);
    cout << iBin << "th diffXsec : " << diffXsec << " || err : " << diffXsec_err << " || bin width : " << bin_width << endl;
  }
  cout << "inclusive Xsection : " << incXsec << " || error : " << sqrt(incXsec_err) << endl; 
  cout << "--------------------------------------------------------"<< endl;
  return h_out;
}

TH1 *calculateDiffXsec(TH1 *h_in, int variable, bool data){
  const int dR = 0, invMass = 1;
  string var;
  TH1 *h_out = (TH1 *)h_in->Clone();

  TFile *f_stability = TFile::Open(Form("%shist_stability_ttbb.root",input.c_str()));
  
  TH1 *h_acceptance;
  TH1 *h_tmp[2];
  if(variable == dR){
    var = "deltaR";
    h_tmp[0] = (TH1 *)f_stability->Get(Form("h_%s_Ch%d_%s",BIN_ACCEPTANCE_DELTAR_,0,NAME_[TTBB_].c_str()));
    h_tmp[1] = (TH1 *)f_stability->Get(Form("h_%s_Ch%d_%s",BIN_ACCEPTANCE_DELTAR_,1,NAME_[TTBB_].c_str()));
    h_tmp[0]->Add(h_tmp[1]);
    h_acceptance = h_tmp[0];
  }
  else if(variable == invMass){
    var = "invMass";
    h_tmp[0] = (TH1 *)f_stability->Get(Form("h_%s_Ch%d_%s",BIN_ACCEPTANCE_INVARIANT_MASS_,0,NAME_[TTBB_].c_str()));
    h_tmp[1] = (TH1 *)f_stability->Get(Form("h_%s_Ch%d_%s",BIN_ACCEPTANCE_INVARIANT_MASS_,0,NAME_[TTBB_].c_str()));
    h_tmp[0]->Add(h_tmp[1]);
    h_acceptance = h_tmp[0];
  }
  else{
    cout << "There is no such channel" << endl;
    return h_out;
  }

  double incXsec = 0.0; double incXsec_err = 0.0;
  cout << "--------------------------------------------------------"<< endl;
  if(data) cout << "##### Unfolded Data #####" << endl;
  else cout << "##### Unfolded MC #####" << endl;
  cout << var << endl;
  for(int iBin=1; iBin<=h_in->GetNbinsX(); ++iBin){
    // diffXsec = (1/bin width)*(nevt/accp*lumi)
    const double bin_width = h_in->GetXaxis()->GetBinWidth(iBin);
    const double diffXsec = h_in->GetBinContent(iBin)/(h_acceptance->GetBinContent(iBin)*bin_width*LUMINOSITY_*BRANCHINGRATIO_);
    const double diffXsec_err = h_in->GetBinError(iBin)/(h_acceptance->GetBinContent(iBin)*bin_width*LUMINOSITY_*BRANCHINGRATIO_);
    h_out->SetBinContent(iBin,diffXsec);
    h_out->SetBinError(iBin, diffXsec_err);
    incXsec += diffXsec*bin_width;
    incXsec_err += pow(diffXsec_err*bin_width,2);
    cout << iBin << "th diffXsec : " << diffXsec << " || err : " << diffXsec_err << " || bin width : " << bin_width << endl;
  }
  cout << "inclusive Xsection : " << incXsec << " || error : " << sqrt(incXsec_err) << endl; 
  cout << "--------------------------------------------------------"<< endl;
  return h_out;
}

TH1 *calculateDiffXsec(TH1 *h_in){
  TH1 *h_out = (TH1 *)h_in->Clone();
  
  double incXsec = 0.0; double incXsec_err = 0.0;
  cout << "--------------------------------------------------------"<< endl;
  cout << "##### Powheg + Pythia #####" << endl;
  for(int iBin=1; iBin<=h_in->GetNbinsX(); ++iBin){
    const double bin_width = h_in->GetXaxis()->GetBinWidth(iBin);
    const double diffXsec = h_in->GetBinContent(iBin)/(bin_width*LUMINOSITY_*BRANCHINGRATIO_);
    const double diffXsec_err = h_in->GetBinError(iBin)/(bin_width*LUMINOSITY_*BRANCHINGRATIO_); 
    h_out->SetBinContent(iBin, diffXsec);
    h_out->SetBinError(iBin, diffXsec_err);
    incXsec += diffXsec*bin_width;
    incXsec_err += pow(diffXsec_err*bin_width,2);
    cout << iBin << "th diffXsec : " << diffXsec << " || err : " << diffXsec_err << " || bin width : " << bin_width << endl;
  }
  cout << "inclusive Xsection : " << incXsec << " || error : " << sqrt(incXsec_err) << endl;
  cout << "--------------------------------------------------------"<< endl;

  return h_out;
}

TH1 *avoidVoid(TH1 *h_in,int variable, int channel){
  const int dR = 0, invMass = 1;
  string var;
  if(variable == 0) var = "deltaR";
  else if(variable == 1) var = "invMass";
  
  TH1 *h_out = (TH1 *)h_in->Clone();
  TFile *f_stability = TFile::Open(Form("%shist_stability_ttbb.root",input.c_str()));
  TH1 *h_acceptance;
  if(variable == dR)
    h_acceptance = (TH1 *)f_stability->Get(Form("h_%s_Ch%d_%s",BIN_ACCEPTANCE_DELTAR_,channel,NAME_[TTBB_].c_str()));
  else if(variable == invMass)
    h_acceptance = (TH1 *)f_stability->Get(Form("h_%s_Ch%d_%s",BIN_ACCEPTANCE_INVARIANT_MASS_,channel,NAME_[TTBB_].c_str()));
 
  h_out->Divide(h_acceptance);
  
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
  c->Print(Form("result/SVD_statErr%s_Ch%d_%s.pdf",(dat.at(data)).c_str(), channel, (var.at(variable)).c_str()),"pdf");
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
  c->Print(Form("result/SVD_gap%s_Ch%d_%s.pdf",(dat.at(data)).c_str(), channel, (var.at(variable)).c_str()), "pdf");
  leg->~TLegend();
  c->~TCanvas();
}

enum opt_legend {kReco = 0, kGen, kRespX, kRespY, kUnfoldedData, kDataBkgSub};

void DrawHist(string outname, TH1 *h_in1, TH1 *h_in2, int opt1, int opt2, int cor1, int cor2,
  bool dR = false, bool diffXsec = false, bool unfold = false ,bool useTUnfold = false, int reg = 0, 
  bool h1togrp = false, bool h2togrp = false){
  gROOT->ProcessLine("setTDRStyle();"); 
  vector<string> v_legname = {
    "t#bar{t}b#bar{b} MC", "Powheg+Pythia", "X-axis projection", "Y-axis projection",
    "Unfolded data", "Data - Background" 
  };

  TCanvas *c = new TCanvas("","",800,800);
  TLegend *leg = new TLegend(0.6,0.7,0.88,0.88);
  TPaveText *label_cms;
  if(opt1 >= kUnfoldedData || opt2 >= kUnfoldedData) label_cms = tdrCMSlabel();
  else label_cms = tdrCMSSimlabel();

  TGraphErrors *grp1, *grp2;
  if(h1togrp){
    grp1 = buildGraphFromHist(h_in1);
    grp1->SetLineColor(cor1);
    grp1->SetMarkerColor(cor1);
  }
  if(h2togrp){
    grp2 = buildGraphFromHist(h_in2);
    grp2->SetLineColor(cor2);
    grp2->SetMarkerColor(cor2);
  }

  if(unfold && !(useTUnfold))
    leg->SetHeader(Form("k = %d",reg),"C");

  if(opt1 >= kUnfoldedData)
    leg->AddEntry(h_in1, v_legname[opt1].c_str(), "lp");
  else if(opt1 == kGen){
    leg->AddEntry(h_in1, v_legname[opt1].c_str(), "F");
    h_in1->SetFillColor(cor1);
  }
  else
    leg->AddEntry(h_in1, v_legname[opt1].c_str(), "l");
  if(opt2 >= kUnfoldedData)
    leg->AddEntry(h_in2, v_legname[opt2].c_str(), "lp");
  else if(opt2 == kGen){
    leg->AddEntry(h_in2, v_legname[opt2].c_str(), "F");
    h_in2->SetFillColor(cor2);
  }
  else
    leg->AddEntry(h_in2, v_legname[opt2].c_str(), "l");


  h_in1->SetLineColor(cor1);
  h_in2->SetLineColor(cor2);
  h_in1->GetYaxis()->SetTitleOffset(1.6);
  h_in1->GetXaxis()->SetTitleOffset(1.2);
  h_in1->SetMinimum(0.0);
  if(diffXsec && dR){
    h_in1->GetYaxis()->SetTitleSize(0.04);
    h_in1->GetYaxis()->SetTitle("#frac{d#sigma^{full}}{d#DeltaR}[pb]");
    h_in1->SetMaximum(3.0);
  }
  else if(diffXsec && !(dR)){
    h_in1->GetYaxis()->SetTitleSize(0.04);
    h_in1->GetYaxis()->SetLabelSize(0.025);
    h_in1->GetYaxis()->SetTitle("#frac{d#sigma^{full}}{dm_{b#bar{b}}}[pb/GeV]");
    h_in1->SetMaximum(0.04);
  }
  else{
    h_in1->GetYaxis()->SetTitle("Events");
    h_in1->SetMaximum(h_in1->GetMaximum()*1.5);
  }

  if(dR) h_in1->GetXaxis()->SetTitle("#DeltaR_{b#bar{b}}");
  else h_in1->GetXaxis()->SetTitle("m_{b#bar{b}}");

  h_in1->Draw("axis");
  
  if(h1togrp){
    h_in2->Draw("hist same");
    grp1->Draw("p e same");
  }
  else if(h2togrp){
    h_in1->Draw("hist same");
    grp2->Draw("p e same");
  }
  else{
    h_in2->Draw("hist same");
    h_in1->Draw("hist same");
  }
  leg->Draw("same");
  label_cms->Draw("same");

  c->Print(Form("result/%s.pdf",outname.c_str()),"pdf");
}
