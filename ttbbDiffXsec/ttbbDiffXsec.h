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

const bool   scanLcurve = true;

const bool findBestK = false;
const int reg_dR = 3, reg_M = 3, reg_dEta = 3, reg_dPhi = 3;

std::map<std::string, int> m_lumi = {{"16",35922}, {"17",41529}, {"18",59693}};
double bratio = 0.436572; // 2*3*0.1086*0.67

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

TH1 *calculateDiffXsec(std::string year, TH1 *h_in, TH1 *h_acceptance, bool gen = false){
  TH1 *h_out = (TH1 *)h_in->Clone(); 
  double incXsec = 0.0; double incXsec_err = 0.0;
  
  std::cout << "--------------------------------------------------------"<< std::endl;
  std::cout << h_in->GetName() << endl;
  std::string name = h_in->GetName();
  ssize_t pos;
  
  for(int ibin=1; ibin <= h_in->GetNbinsX(); ++ibin){
    double bin_width, diffXsec, diffXsec_err;
    // diffXsec = (1/bin width)*(nevt/accp*lumi)
    bin_width = h_in->GetXaxis()->GetBinWidth(ibin);
    if(gen){
      diffXsec     = h_in->GetBinContent(ibin)/(bin_width*m_lumi[year]*bratio);
      diffXsec_err = h_in->GetBinError(ibin)/(bin_width*m_lumi[year]*bratio); 
    }
    else{
      diffXsec     = h_in->GetBinContent(ibin)
                   / (h_acceptance->GetBinContent(ibin)*bin_width*m_lumi[year]*bratio);
      diffXsec_err = h_in->GetBinError(ibin) 
                   / (h_acceptance->GetBinContent(ibin)*bin_width*m_lumi[year]*bratio);
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

