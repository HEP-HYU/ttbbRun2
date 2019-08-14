#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TMath.h>

#include "../include/histBook.h"
#include "../include/tdrstyle.C"

string outdir = "../output/unfold/";

//Unfolding options
const char *genMode = "mindR";
const bool runSystematics = false;

const bool   useTUnfold = true; 
const bool   scanLcurve = true;

const bool   fixtau_dR = false; 
const float  fixedtau_dR = 0.01;
const double taumin_dR = 0.1, taumax_dR = 1.0;

const bool   fixtau_M = false; 
const float  fixedtau_M = 0.0;
const double taumin_M = 0.8, taumax_M = 0.9;

const bool   fixtau_dEta = false;
const float  fixedtau_dEta = 0.0;
const double taumin_dEta = 0.8, taumax_dEta = 0.9;

const bool   fixtau_dPhi = false;
const float  fixedtau_dPhi = 0.0;
const double taumin_dPhi = 0.8, taumax_dPhi = 0.9;

const bool findBestK = false;
const int reg_dR = 3, reg_M = 3, reg_dEta = 3, reg_dPhi = 3;

const double SF16_ttbb = 0.992;
const double SF16_ttbj = 0.997;
const double SF16_ttcc = 0.871;
const double SF16_ttLF = 0.890;

std::vector<std::string> syst_list = {
  "__jerup", "__jerdown", "__jecup", "__jecdown",
  "__musfup", "__musfdown", "__mutrgup", "__mutrgdown",
  "__elsfup", "__elsfdown", "__eltrgup", "__eltrgdown",
  "__lfup", "__lfdown", "__hfup", "__hfdown",
  "__hfstat1up","__hfstat1down", "__hfstat2up", "__hfstat2down",
  "__lfstat1up","__lfstat1down", "__lfstat2up", "__lfstat2down",
  "__cferr1up", "__cferr1down",  "__cferr2up",  "__cferr2down",
  "__puup", "__pudown"
};

std::vector<std::string> syst_ttbar = {
  "__psup", "__psdown", "__swup", "__swdown",
  "__hdampup", "__hdampdown", "__tuneup", "__tunedown"
};

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

TH1 *calculateDiffXsec(TH1 *h_in, TH1 *h_acceptance, bool gen = false){
  TH1 *h_out = (TH1 *)h_in->Clone(); 
  double incXsec = 0.0; double incXsec_err = 0.0;
  
  std::cout << "--------------------------------------------------------"<< std::endl;
  std::cout << h_in->GetName() << endl;
  
  for(int ibin=1; ibin <= h_in->GetNbinsX(); ++ibin){
    double bin_width, diffXsec, diffXsec_err;
    // diffXsec = (1/bin width)*(nevt/accp*lumi)
    bin_width = h_in->GetXaxis()->GetBinWidth(ibin);
    if(gen){
      diffXsec     = h_in->GetBinContent(ibin)/(bin_width*LUMINOSITY_*BRANCHINGRATIO_);
      diffXsec_err = h_in->GetBinError(ibin)/(bin_width*LUMINOSITY_*BRANCHINGRATIO_); 
    }
    else{
      diffXsec     = h_in->GetBinContent(ibin)
                   / (h_acceptance->GetBinContent(ibin)*bin_width*LUMINOSITY_*BRANCHINGRATIO_);
      diffXsec_err = h_in->GetBinError(ibin) 
                   / (h_acceptance->GetBinContent(ibin)*bin_width*LUMINOSITY_*BRANCHINGRATIO_);
      if(h_acceptance->GetBinContent(ibin) == 0.0){
	diffXsec = 0.0;
	diffXsec_err = 0.0;
      }
    }

    h_out->SetBinContent(ibin, diffXsec);
    h_out->SetBinError(ibin, diffXsec_err);

    incXsec += diffXsec*bin_width;
    incXsec_err += pow(diffXsec_err*bin_width,2);
    
    std::cout << ibin << "th diffXsec: " << diffXsec << " || err: " << diffXsec_err << " || bin width: " << bin_width << std::endl;
  }
  
  std::cout << "inclusive Xsection : " << incXsec << " || error : " << sqrt(incXsec_err) << std::endl; 
  std::cout << "--------------------------------------------------------"<< std::endl;
 
  h_out->SetName(Form("diffXsec_%s",h_out->GetName()));
  return h_out;
}

void drawHist(std::string input, TH1 *h_in1, TH1 *h_in2){
  gROOT->ProcessLine("setTDRStyle();");
  
  int reg;
  double taumin, taumax;
  const char *legentry, *legopt;

  std::string hName  = h_in1->GetName();
  std::string hName2 = h_in2->GetName();
  ssize_t pos;
  if((pos = hName.find("DeltaR")) != std::string::npos){
    taumin = taumin_dR; taumax = taumax_dR;
    reg = reg_dR;
  }
  else if((pos = hName.find("InvMass")) != std::string::npos){
    taumin = taumin_M; taumax = taumax_M;
    reg = reg_M;
  }
  else if((pos = hName.find("DeltaEta")) != std::string::npos){
    taumin = taumin_dEta; taumax = taumax_dEta;
    reg = reg_dEta;
  }
  else if((pos = hName.find("DeltaPhi")) != std::string::npos){
    taumin = taumin_dPhi; taumax = taumax_dPhi;
    reg = reg_dPhi;
  }
  else{
    taumin = 0.0, taumax = 0.0;
    reg = 0;
  }

  if((pos = hName2.find("nosel")) != std::string::npos){
    legentry = "Powehg + Pythia";
    legopt = "F";
  }
  else{
    legentry = "Step3 gen distribution";
    legopt = "l";
  }

  TCanvas *c = new TCanvas("","",800,800);
  TLegend *leg = new TLegend(0.5, 0.7, 0.88, 0.91);
  leg->SetTextFont(42);
  leg->SetTextSize(0.035);

  TPaveText *label = tdrCMSlabel();
  if((pos = input.find("Data")) != std::string::npos) label = tdrCMSlabel();
  else                                                label = tdrCMSSimlabel();

  TGraphErrors *grp_draw1 = buildGraphFromHist(h_in1);
  TH1 *h_draw2 = (TH1 *)h_in2->Clone();

  grp_draw1->SetMarkerSize(2);
  grp_draw1->SetMarkerStyle(20);
  grp_draw1->SetMarkerColor(kBlack);
  grp_draw1->SetLineColor(kBlack);

  if((pos = hName.find("diff")) != std::string::npos){
    h_draw2->SetLineColor(kRed);
    h_draw2->SetFillColor(kRed);
  }
  else{
    h_draw2->SetLineColor(kBlue);
    h_draw2->SetLineWidth(2);
  }
  h_draw2->SetMaximum(h_in1->GetMaximum()*1.5);
  h_draw2->SetMinimum(0);

  if(useTUnfold) leg->SetHeader(Form("tau range: %.3f to %.3f", taumin, taumax));
  else leg->SetHeader(Form("Regularization k = %d", reg));
  leg->AddEntry(grp_draw1, "Unfold output", "p");
  leg->AddEntry(h_draw2, legentry, legopt);

  h_draw2->Draw("hist");
  grp_draw1->Draw("p same");
  label->Draw("same");
  leg->Draw("same");

  if(useTUnfold)
    c->Print(Form("%s/TUnfold_%s_%s.pdf", outdir.c_str(), input.c_str(), h_in1->GetName()),"pdf");
  else
    c->Print(Form("%s/SVDUnfold_%s_%s.pdf", outdir.c_str(), input.c_str(), h_in1->GetName()), "pdf");
}
