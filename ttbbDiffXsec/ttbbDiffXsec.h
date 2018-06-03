#include <TFile.h>
#include <TGraphErrors.h>
#include <TCanvas.h>

#include "../include/HistoBook.h"
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
  TLegend *leg = new TLegend(0.6,0.7,0.88,0.88);
  c->Print(Form("%d_%d_truth_%d.pdf",data,variable,channel),"pdf");
  c->Clear();

  vector<TGraph *> v_grp;
  vector<TGraph *> v_grp2;
  for(int i=0; i<h_in.size(); ++i){
    TH1 *h_tmp = h_in.at(i);
    TGraph *grp = new TGraph();
    TGraph *grp2 = new TGraph();

    for(int j=1; j<=h_tmp->GetNbinsX(); ++j){
      const double y = h_tmp->GetBinError(j);
      grp->SetPoint(j-1, h_tmp->GetBinCenter(j), y);

      const double yy = (h_truth->GetBinContent(j)-h_tmp->GetBinContent(j))/h_truth->GetBinContent(j);
      grp2->SetPoint(j-1, h_tmp->GetBinCenter(j), yy);
    }
    v_grp.push_back(grp);
    v_grp2.push_back(grp2);
  }

  TGraph *grp_truth = new TGraph();
  for(int i=1; i<=h_truth->GetNbinsX(); ++i){
    const double y = sqrt(h_truth->GetBinContent(i));
    grp_truth->SetPoint(i-1, h_truth->GetBinCenter(i), y);
  }
  grp_truth->SetLineStyle(8);
  grp_truth->SetLineColor(kSpring+4);
  grp_truth->SetMarkerColor(kSpring+4);
  grp_truth->SetMarkerStyle(kOpenStar);

  c->cd();
  TH1 *h_tmp = (TH1 *)(h_in.at(0))->Clone();
  h_tmp->SetMinimum(-0.01);
  h_tmp->GetYaxis()->SetTitle("Statistical Uncertainty");
  h_tmp->GetXaxis()->SetTitleOffset(1.2);
  h_tmp->Draw("axis");
  for(int i=0; i<v_grp.size(); ++i){
    v_grp.at(i)->SetMarkerSize(1.5);
    v_grp.at(i)->SetMarkerColor(colorStyle.at(i));
    v_grp.at(i)->SetMarkerStyle(markerStyle.at(i));
    leg->AddEntry(v_grp.at(i), Form("k = %d",i+2), "p");
    
    v_grp.at(i)->Draw("C p same");
  }
  leg->AddEntry(grp_truth, "MC truth", "lp");
  grp_truth->Draw("C p same");
  
  leg->Draw("same");
  label_cms->Draw("same");
  c->Print(Form("result/SVD_statErr%s_Ch%d_%s.pdf",(dat.at(data)).c_str(), channel, (var.at(variable)).c_str()),"pdf");
  leg->Clear();
  c->Clear();

  c->cd();
  TH1 *h_tmp2 = (TH1 *)(h_in.at(0))->Clone();
  h_tmp2->SetMaximum(0.5);
  h_tmp2->SetMinimum(-0.2);
  h_tmp2->GetYaxis()->SetTitle("(N_{truth}-N_{unfolded})/N_{truth}");
  h_tmp2->GetXaxis()->SetTitleOffset(1.2);
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

