#include <iostream>
#include <fstream>

#include "../include/histBook.h"
#include "../include/tdrstyle.C"

const bool runSystematics = false;
const char *genMode = "mindR";

HistoBook *MakeHist(const char *genmode, TFile *f_in, std::string syst);
void DrawHist(TH1 *h_in, std::string name, bool drawError);

void makeCriteria(){
  string input_dir = "../output/post/";
  TFile *f_in = TFile::Open(Form("%s/hist_ttbb.root", input_dir.c_str()));
  TFile *f_out = TFile::Open(Form("%s/hist_criteria_%s.root", input_dir.c_str(), genMode),"recreate");

  std::vector<std::string> v_syst_list = {
    "", "__jerup", "__jerdown", "__jecup", "__jecdown",
    "__puup", "__pudown",
    "__musfup", "__musfdown", "__mutrgup", "__mutrgdown",
    "__elsfup", "__elsfdown", "__eltrgup", "__eltrgdown",
    "__lfup", "__lfdown", "__hfup", "__hfdown",
    "__hfstat1up", "__hfstat1down", "__hfstat2up", "__hfstat2down",
    "__lfstat1up", "__lfstat1down", "__lfstat2up", "__lfstat2down",
    "__cferr1up", "__cferr1down", "__cferr2up", "__cferr2down",
    "__psup", "__psdown", "__swup", "__swdown",
    "__hdampup", "__hdampdown", "__tuneup", "__tunedown"
  };

  f_out->cd();
  for(auto v_itr = v_syst_list.begin(); v_itr != v_syst_list.end(); ++v_itr){
    if(!(runSystematics || *v_itr == "")) continue;
    auto h_tmp = MakeHist(genMode, f_in, *v_itr);
    
    for(int ich=0; ich<nChannel; ++ich){
      h_tmp->h_stability_deltaR[ich]->Write();
      h_tmp->h_stability_invMass[ich]->Write();
      h_tmp->h_stability_deltaEta[ich]->Write();
      h_tmp->h_stability_deltaPhi[ich]->Write();
      h_tmp->h_purity_deltaR[ich]->Write();
      h_tmp->h_purity_invMass[ich]->Write();
      h_tmp->h_purity_deltaEta[ich]->Write();
      h_tmp->h_purity_deltaPhi[ich]->Write();
      h_tmp->h_acceptance_deltaR[ich]->Write();
      h_tmp->h_acceptance_invMass[ich]->Write();
      h_tmp->h_acceptance_deltaEta[ich]->Write();
      h_tmp->h_acceptance_deltaPhi[ich]->Write();
    }
    h_tmp->~HistoBook();
  }
  f_out->Write();
  f_out->Close();
}

HistoBook *MakeHist(const char *genmode, TFile *f_in, std::string syst){
  std::ofstream fout(Form("BinCriteria_%s.txt", syst.c_str()));
  fout << "Systematic sample: " << syst << std::endl;

  double xsec = 0.0;
  if( syst.find("sw",0,2) != std::string::npos ) xsec = 831.76;
  else if( syst.find("tune",0,4) != std::string::npos ) xsec = 831.76;
  else xsec = XSEC_[TTBB_];

  //NORMALIZATION
  //TH1 *EventInfo = (TH1 *)f_in->Get("EventInfo");
  //const double SF_ttbb = 0.992;
  //const double scale = SF_ttbb*LUMINOSITY_*xsec/EventInfo->GetBinContent(2);

  //HISTOGRAMS
  HistoBook *h_stb = new HistoBook(3,syst.c_str());

  TH2D *h_matrix_deltaR[nChannel];
  TH2D *h_matrix_invMass[nChannel];
  TH2D *h_matrix_deltaEta[nChannel];
  TH2D *h_matrix_deltaPhi[nChannel];

  TH1D *h_gen_deltaR[nChannel];
  TH1D *h_gen_invMass[nChannel];
  TH1D *h_gen_deltaEta[nChannel];
  TH1D *h_gen_deltaPhi[nChannel];

  TH1D *h_reco_deltaR[nChannel];
  TH1D *h_reco_invMass[nChannel];
  TH1D *h_reco_deltaEta[nChannel];
  TH1D *h_reco_deltaPhi[nChannel];

  TH1D *h_gen_deltaR_nosel[nChannel];
  TH1D *h_gen_invMass_nosel[nChannel];
  TH1D *h_gen_deltaEta_nosel[nChannel];
  TH1D *h_gen_deltaPhi_nosel[nChannel];

  for(int ich=0; ich < nChannel; ++ich){    
    h_gen_deltaR_nosel[ich]    = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_nosel", genmode, GEN_ADD_DR_,   ich));
    h_gen_invMass_nosel[ich]   = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_nosel", genmode, GEN_ADD_M_,    ich));
    h_gen_deltaEta_nosel[ich]  = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_nosel", genmode, GEN_ADD_DETA_, ich));
    h_gen_deltaPhi_nosel[ich]  = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_nosel", genmode, GEN_ADD_DPHI_, ich));
    
    h_matrix_deltaR[ich]   = (TH2D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, MATRIX_DR_,   ich, syst.c_str()));
    h_matrix_invMass[ich]  = (TH2D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, MATRIX_M_,    ich, syst.c_str()));
    h_matrix_deltaEta[ich] = (TH2D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, MATRIX_DETA_, ich, syst.c_str()));
    h_matrix_deltaPhi[ich] = (TH2D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, MATRIX_DPHI_, ich, syst.c_str()));

    h_gen_deltaR[ich]   = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, GEN_ADD_DR_,   ich, syst.c_str()));
    h_gen_invMass[ich]  = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, GEN_ADD_M_,    ich, syst.c_str()));
    h_gen_deltaEta[ich] = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, GEN_ADD_DETA_, ich, syst.c_str()));
    h_gen_deltaPhi[ich] = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, GEN_ADD_DPHI_, ich, syst.c_str()));

    h_reco_deltaR[ich]   = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, RECO_ADD_DR_,   ich, syst.c_str())); 
    h_reco_invMass[ich]  = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, RECO_ADD_M_,   ich, syst.c_str())); 
    h_reco_deltaEta[ich] = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, RECO_ADD_DETA_, ich, syst.c_str())); 
    h_reco_deltaPhi[ich] = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, RECO_ADD_DPHI_, ich, syst.c_str())); 
  }

  for(int ich=0; ich<nChannel; ++ich){
    fout << "=======================================" << std::endl;
    fout << "Channel: " << ich << std::endl;
    
    fout << "----------------------------------------" << std::endl;
    fout << "----------------------------------------" << std::endl;
    fout << "**************** Purity ****************" << std::endl;
    fout << "Variable: delta R" << endl;

    for(int ibin=1; ibin < nbins_reco_addbjets_dR+1; ++ibin){
      fout << "------" << ibin << "th Bin ------" << std::endl;
      double purity_dR = 0.0;
      if(h_reco_deltaR[ich]->GetBinContent(ibin) != 0.0){
	auto xlow = h_matrix_deltaR[ich]->GetXaxis()->GetBinLowEdge(ibin);
	auto ybin = h_matrix_deltaR[ich]->GetYaxis()->FindBin(xlow);
	purity_dR = h_matrix_deltaR[ich]->GetBinContent(ibin,ybin)/h_reco_deltaR[ich]->GetBinContent(ibin);
	 
	fout << "x low : " << xlow << std::endl;
	fout << "y bin number : " << ybin << std::endl;
	fout << "xBinContent : " << h_reco_deltaR[ich]->GetBinContent(ibin) << std::endl;
	fout << "yBinContent : " << h_matrix_deltaR[ich]->GetBinContent(ibin,ybin) << std::endl;
	fout << "value : " << purity_dR << std::endl;
      }
      h_stb->h_purity_deltaR[ich]->SetBinContent(ibin, purity_dR);
      fout << "----------------------------------------" << std::endl;
    }
    
    fout << "----------------------------------------" << std::endl;
    fout << "Variable: inv Mass" << endl;

    for(int ibin=1; ibin < nbins_reco_addbjets_M+1; ++ibin){
      fout << "------" << ibin << "th Bin ------" << std::endl;
      double purity_M = 0.0;
      if(h_reco_invMass[ich]->GetBinContent(ibin) != 0.0){
	auto xlow = h_matrix_invMass[ich]->GetXaxis()->GetBinLowEdge(ibin);
	auto ybin = h_matrix_invMass[ich]->GetYaxis()->FindBin(xlow);
	purity_M = h_matrix_invMass[ich]->GetBinContent(ibin,ybin)/h_reco_invMass[ich]->GetBinContent(ibin);
	 
	fout << "x low : " << xlow << std::endl;
	fout << "y bin number : " << ybin << std::endl;
	fout << "xBinContent : " << h_reco_invMass[ich]->GetBinContent(ibin) << std::endl;
	fout << "yBinContent : " << h_matrix_invMass[ich]->GetBinContent(ibin,ybin) << std::endl;
	fout << "value : " << purity_M << std::endl;
      }
      h_stb->h_purity_invMass[ich]->SetBinContent(ibin, purity_M);
      fout << "----------------------------------------" << std::endl;
    }

    fout << "----------------------------------------" << std::endl;
    fout << "Variable: delta Eta" << endl;

    for(int ibin=1; ibin < nbins_reco_addbjets_dEta+1; ++ibin){
      fout << "------" << ibin << "th Bin ------" << std::endl;
      double purity_dEta = 0.0;
      if(h_reco_deltaEta[ich]->GetBinContent(ibin) != 0.0){
	auto xlow = h_matrix_deltaEta[ich]->GetXaxis()->GetBinLowEdge(ibin);
	auto ybin = h_matrix_deltaEta[ich]->GetYaxis()->FindBin(xlow);
	purity_dEta = h_matrix_deltaEta[ich]->GetBinContent(ibin,ybin)/h_reco_deltaEta[ich]->GetBinContent(ibin);
	 
	fout << "x low : " << xlow << std::endl;
	fout << "y bin number : " << ybin << std::endl;
	fout << "xBinContent : " << h_reco_deltaEta[ich]->GetBinContent(ibin) << std::endl;
	fout << "yBinContent : " << h_matrix_deltaEta[ich]->GetBinContent(ibin,ybin) << std::endl;
	fout << "value : " << purity_dEta << std::endl;
      }
      h_stb->h_purity_deltaEta[ich]->SetBinContent(ibin, purity_dEta);
      fout << "----------------------------------------" << std::endl;
    }

    fout << "----------------------------------------" << std::endl;
    fout << "Variable: delta Eta" << endl;

    for(int ibin=1; ibin < nbins_reco_addbjets_dPhi+1; ++ibin){
      fout << "------" << ibin << "th Bin ------" << std::endl;
      double purity_dPhi = 0.0;
      if(h_reco_deltaPhi[ich]->GetBinContent(ibin) != 0.0){
	auto xlow = h_matrix_deltaPhi[ich]->GetXaxis()->GetBinLowEdge(ibin);
	auto ybin = h_matrix_deltaPhi[ich]->GetYaxis()->FindBin(xlow);
	purity_dPhi = h_matrix_deltaPhi[ich]->GetBinContent(ibin,ybin)/h_reco_deltaPhi[ich]->GetBinContent(ibin);
	 
	fout << "x low : " << xlow << std::endl;
	fout << "y bin number : " << ybin << std::endl;
	fout << "xBinContent : " << h_reco_deltaPhi[ich]->GetBinContent(ibin) << std::endl;
	fout << "yBinContent : " << h_matrix_deltaPhi[ich]->GetBinContent(ibin,ybin) << std::endl;
	fout << "value : " << purity_dPhi << std::endl;
      }
      h_stb->h_purity_deltaPhi[ich]->SetBinContent(ibin, purity_dPhi);
      fout << "----------------------------------------" << std::endl;
    }

    fout << "----------------------------------------" << std::endl;
    fout << "************** Stability  **************" << std::endl;
    fout << "Variable: delta R" << endl;
    for(int ibin=1; ibin<nbins_gen_addbjets_dR+1; ++ibin){
      fout << "------" << ibin << "th Bin ------" << std::endl;
      double stability_dR = 0.0;
      if(h_gen_deltaR[ich]->GetBinContent(ibin) != 0.0){
	auto ylow = h_matrix_deltaR[ich]->GetYaxis()->GetBinLowEdge(ibin);
	auto ymax = h_matrix_deltaR[ich]->GetYaxis()->GetBinLowEdge(ibin+1);
	auto xlow = h_matrix_deltaR[ich]->GetXaxis()->FindBin(ylow);
	auto xmax = h_matrix_deltaR[ich]->GetXaxis()->FindBin(ymax);
	if(xlow == xmax && xmax == h_matrix_deltaR[ich]->GetNbinsX()) ++xmax;
	double xbincontent = 0.;
	for(int ixbin = xlow; ixbin < xmax; ++ixbin)
	  xbincontent += h_matrix_deltaR[ich]->GetBinContent(ixbin,ibin);
	stability_dR = xbincontent/h_gen_deltaR[ich]->GetBinContent(ibin);
	fout << "y low : " << ylow << ", max : " << ymax << std::endl;
	fout << "x low : " << xlow << ", max : " << xmax << std::endl;
	fout << "xBinContent : " << xbincontent << std::endl;
	fout << "yBinContent : " << h_gen_deltaR[ich]->GetBinContent(ibin) << std::endl;
	fout << "value :" << stability_dR << std::endl;
      }
      h_stb->h_stability_deltaR[ich]->SetBinContent(ibin, stability_dR);
      fout << "----------------------------------------" << std::endl;
    }

    fout << "----------------------------------------" << std::endl;
    fout << "Variable: inv Mass" << endl;
    for(int ibin=1; ibin<nbins_gen_addbjets_M+1; ++ibin){
      fout << "------" << ibin << "th Bin ------" << std::endl;
      double stability_M = 0.0;
      if(h_gen_invMass[ich]->GetBinContent(ibin) != 0.0){
	auto ylow = h_matrix_invMass[ich]->GetYaxis()->GetBinLowEdge(ibin);
	auto ymax = h_matrix_invMass[ich]->GetYaxis()->GetBinLowEdge(ibin+1);
	auto xlow = h_matrix_invMass[ich]->GetXaxis()->FindBin(ylow);
	auto xmax = h_matrix_invMass[ich]->GetXaxis()->FindBin(ymax);
	if(xlow == xmax && xmax == h_matrix_invMass[ich]->GetNbinsX()) ++xmax;
	double xbincontent = 0.;
	for(int ixbin = xlow; ixbin < xmax; ++ixbin)
	  xbincontent += h_matrix_invMass[ich]->GetBinContent(ixbin,ibin);
	stability_M = xbincontent/h_gen_invMass[ich]->GetBinContent(ibin);
	fout << "y low : " << ylow << ", max : " << ymax << std::endl;
	fout << "x low : " << xlow << ", max : " << xmax << std::endl;
	fout << "xBinContent : " << xbincontent << std::endl;
	fout << "yBinContent : " << h_gen_invMass[ich]->GetBinContent(ibin) << std::endl;
	fout << "value :" << stability_M << std::endl;
      }
      h_stb->h_stability_invMass[ich]->SetBinContent(ibin, stability_M);
      fout << "----------------------------------------" << std::endl;
    }

    fout << "----------------------------------------" << std::endl;
    fout << "Variable: delta Eta" << endl;
    for(int ibin=1; ibin<nbins_gen_addbjets_dEta+1; ++ibin){
      fout << "------" << ibin << "th Bin ------" << std::endl;
      double stability_dEta = 0.0;
      if(h_gen_deltaEta[ich]->GetBinContent(ibin) != 0.0){
	auto ylow = h_matrix_deltaEta[ich]->GetYaxis()->GetBinLowEdge(ibin);
	auto ymax = h_matrix_deltaEta[ich]->GetYaxis()->GetBinLowEdge(ibin+1);
	auto xlow = h_matrix_deltaEta[ich]->GetXaxis()->FindBin(ylow);
	auto xmax = h_matrix_deltaEta[ich]->GetXaxis()->FindBin(ymax);
	if(xlow == xmax && xmax == h_matrix_deltaEta[ich]->GetNbinsX()) ++xmax;
	double xbincontent = 0.;
	for(int ixbin = xlow; ixbin < xmax; ++ixbin)
	  xbincontent += h_matrix_deltaEta[ich]->GetBinContent(ixbin,ibin);
	stability_dEta = xbincontent/h_gen_deltaEta[ich]->GetBinContent(ibin);
	fout << "y low : " << ylow << ", max : " << ymax << std::endl;
	fout << "x low : " << xlow << ", max : " << xmax << std::endl;
	fout << "xBinContent : " << xbincontent << std::endl;
	fout << "yBinContent : " << h_gen_deltaEta[ich]->GetBinContent(ibin) << std::endl;
	fout << "value :" << stability_dEta << std::endl;
      }
      h_stb->h_stability_deltaEta[ich]->SetBinContent(ibin, stability_dEta);
      fout << "----------------------------------------" << std::endl;
    }

    fout << "----------------------------------------" << std::endl;
    fout << "Variable: delta Phi" << endl;
    for(int ibin=1; ibin<nbins_gen_addbjets_dPhi+1; ++ibin){
      fout << "------" << ibin << "th Bin ------" << std::endl;
      double stability_dPhi = 0.0;
      if(h_gen_deltaPhi[ich]->GetBinContent(ibin) != 0.0){
	auto ylow = h_matrix_deltaPhi[ich]->GetYaxis()->GetBinLowEdge(ibin);
	auto ymax = h_matrix_deltaPhi[ich]->GetYaxis()->GetBinLowEdge(ibin+1);
	auto xlow = h_matrix_deltaPhi[ich]->GetXaxis()->FindBin(ylow);
	auto xmax = h_matrix_deltaPhi[ich]->GetXaxis()->FindBin(ymax);
	if(xlow == xmax && xmax == h_matrix_deltaPhi[ich]->GetNbinsX()) ++xmax;
	double xbincontent = 0.;
	for(int ixbin = xlow; ixbin < xmax; ++ixbin)
	  xbincontent += h_matrix_deltaPhi[ich]->GetBinContent(ixbin,ibin);
	stability_dPhi = xbincontent/h_gen_deltaPhi[ich]->GetBinContent(ibin);
	fout << "y low : " << ylow << ", max : " << ymax << std::endl;
	fout << "x low : " << xlow << ", max : " << xmax << std::endl;
	fout << "xBinContent : " << xbincontent << std::endl;
	fout << "yBinContent : " << h_gen_deltaPhi[ich]->GetBinContent(ibin) << std::endl;
	fout << "value :" << stability_dPhi << std::endl;
      }
      h_stb->h_stability_deltaPhi[ich]->SetBinContent(ibin, stability_dPhi);
      fout << "----------------------------------------" << std::endl;
    }

    fout << "----------------------------------------" << std::endl;
    fout << "************** Acceptance  *************" << std::endl;
    fout << "Variable : delta R" << std::endl;
    for(int ibin=1; ibin<nbins_gen_addbjets_dR+1;++ibin){
      fout << "------" << ibin << "th Bin ------" << std::endl;
      double acceptance_dR = 0.0;
      double dR_err = 0.0;
      if( h_gen_deltaR_nosel[0]->GetBinContent(ibin) > 0.0 && h_gen_deltaR_nosel[1]->GetBinContent(ibin) > 0.0 ){
	acceptance_dR = h_gen_deltaR[ich]->GetBinContent(ibin)
	              / (h_gen_deltaR_nosel[0]->GetBinContent(ibin) + h_gen_deltaR_nosel[1]->GetBinContent(ibin));
	if( h_gen_deltaR[ich]->GetBinContent(ibin) > 0.0 ){
	  dR_err = abs(acceptance_dR)
	         * sqrt(pow(h_gen_deltaR[ich]->GetBinError(ibin)/h_gen_deltaR[ich]->GetBinContent(ibin),2) 
	              + pow(h_gen_deltaR_nosel[0]->GetBinError(ibin)/h_gen_deltaR_nosel[0]->GetBinContent(ibin),2)
		      + pow(h_gen_deltaR_nosel[1]->GetBinError(ibin)/h_gen_deltaR_nosel[1]->GetBinContent(ibin),2));
	}
      }
      h_stb->h_acceptance_deltaR[ich]->SetBinContent(ibin, acceptance_dR);
      h_stb->h_acceptance_deltaR[ich]->SetBinError(ibin, dR_err);
      fout << "value :" << acceptance_dR << std::endl;
      fout << "err :" << dR_err << std::endl;
      fout << "----------------------------------------" << std::endl;
    }
    
    fout << "----------------------------------------" << std::endl;
    fout << "Variable : inv Mass" << std::endl;
    for(int ibin=1; ibin<nbins_gen_addbjets_M+1;++ibin){
      fout << "------" << ibin << "th Bin ------" << std::endl;
      double acceptance_M = 0.0;
      double M_err = 0.0;
      if( h_gen_invMass_nosel[0]->GetBinContent(ibin) > 0.0 && h_gen_invMass_nosel[1]->GetBinContent(ibin) > 0.0 ){
	acceptance_M = h_gen_invMass[ich]->GetBinContent(ibin)
	              / (h_gen_invMass_nosel[0]->GetBinContent(ibin) + h_gen_invMass_nosel[1]->GetBinContent(ibin));
	if( h_gen_invMass[ich]->GetBinContent(ibin) > 0.0 ){
	  M_err = abs(acceptance_M)
	         * sqrt(pow(h_gen_invMass[ich]->GetBinError(ibin)/h_gen_invMass[ich]->GetBinContent(ibin),2) 
	              + pow(h_gen_invMass_nosel[0]->GetBinError(ibin)/h_gen_invMass_nosel[0]->GetBinContent(ibin),2)
		      + pow(h_gen_invMass_nosel[1]->GetBinError(ibin)/h_gen_invMass_nosel[1]->GetBinContent(ibin),2));
	}
      }
      h_stb->h_acceptance_invMass[ich]->SetBinContent(ibin, acceptance_M);
      h_stb->h_acceptance_invMass[ich]->SetBinError(ibin, M_err);
      fout << "value :" << acceptance_M << std::endl;
      fout << "err :" << M_err << std::endl;
      fout << "----------------------------------------" << std::endl;
    }

    fout << "----------------------------------------" << std::endl;
    fout << "Variable : delta Eta" << std::endl;
    for(int ibin=1; ibin<nbins_gen_addbjets_dEta+1;++ibin){
      fout << "------" << ibin << "th Bin ------" << std::endl;
      double acceptance_dEta = 0.0;
      double dEta_err = 0.0;
      if( h_gen_deltaEta_nosel[0]->GetBinContent(ibin) > 0.0 && h_gen_deltaEta_nosel[1]->GetBinContent(ibin) > 0.0 ){
	acceptance_dEta = h_gen_deltaEta[ich]->GetBinContent(ibin)
	              / (h_gen_deltaEta_nosel[0]->GetBinContent(ibin) + h_gen_deltaEta_nosel[1]->GetBinContent(ibin));
	if( h_gen_deltaEta[ich]->GetBinContent(ibin) > 0.0 ){
	  dEta_err = abs(acceptance_dEta)
	         * sqrt(pow(h_gen_deltaEta[ich]->GetBinError(ibin)/h_gen_deltaEta[ich]->GetBinContent(ibin),2) 
	              + pow(h_gen_deltaEta_nosel[0]->GetBinError(ibin)/h_gen_deltaEta_nosel[0]->GetBinContent(ibin),2)
		      + pow(h_gen_deltaEta_nosel[1]->GetBinError(ibin)/h_gen_deltaEta_nosel[1]->GetBinContent(ibin),2));
	}
      }
      h_stb->h_acceptance_deltaEta[ich]->SetBinContent(ibin, acceptance_dEta);
      h_stb->h_acceptance_deltaEta[ich]->SetBinError(ibin, dEta_err);
      fout << "value :" << acceptance_dEta << std::endl;
      fout << "err :" << dEta_err << std::endl;
      fout << "----------------------------------------" << std::endl;
    }
 
    fout << "----------------------------------------" << std::endl;
    fout << "Variable : delta Phi" << std::endl;
    for(int ibin=1; ibin<nbins_gen_addbjets_dPhi+1;++ibin){
      fout << "------" << ibin << "th Bin ------" << std::endl;
      double acceptance_dPhi = 0.0;
      double dPhi_err = 0.0;
      if( h_gen_deltaPhi_nosel[0]->GetBinContent(ibin) > 0.0 && h_gen_deltaPhi_nosel[1]->GetBinContent(ibin) > 0.0 ){
	acceptance_dPhi = h_gen_deltaPhi[ich]->GetBinContent(ibin)
	              / (h_gen_deltaPhi_nosel[0]->GetBinContent(ibin) + h_gen_deltaPhi_nosel[1]->GetBinContent(ibin));
	if( h_gen_deltaPhi[ich]->GetBinContent(ibin) > 0.0 ){
	  dPhi_err = abs(acceptance_dPhi)
	         * sqrt(pow(h_gen_deltaPhi[ich]->GetBinError(ibin)/h_gen_deltaPhi[ich]->GetBinContent(ibin),2) 
	              + pow(h_gen_deltaPhi_nosel[0]->GetBinError(ibin)/h_gen_deltaPhi_nosel[0]->GetBinContent(ibin),2)
		      + pow(h_gen_deltaPhi_nosel[1]->GetBinError(ibin)/h_gen_deltaPhi_nosel[1]->GetBinContent(ibin),2));
	}
      }
      h_stb->h_acceptance_deltaPhi[ich]->SetBinContent(ibin, acceptance_dPhi);
      h_stb->h_acceptance_deltaPhi[ich]->SetBinError(ibin, dPhi_err);
      fout << "value :" << acceptance_dPhi << std::endl;
      fout << "err :" << dPhi_err << std::endl;
      fout << "----------------------------------------" << std::endl;
    }
  }//Channel

  return h_stb;
}

void DrawHist(TH1* h_in_, std::string name, bool drawError){    
  gROOT->ProcessLine("setTDRStyle();");

  gStyle->SetHatchesSpacing(1);
  gStyle->SetHatchesLineWidth(1);

  gStyle->SetPaintTextFormat("4.1f");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TGaxis::SetMaxDigits(3);

  gStyle->SetHatchesSpacing(1);
  gStyle->SetHatchesLineWidth(1);

  auto h_in = (TH1 *)h_in_->Clone();
  h_in->Scale(100);
  auto h_err = (TH1 *)h_in->Clone();
  string histname = h_in->GetName();

  TCanvas *c = new TCanvas("","",800,800);
  TPaveText *label_sim = tdrCMSSimlabel();
  TPaveText *label_work = tdrWorkProgress();

  ssize_t pos;
  h_in->SetMaximum(h_in->GetMaximum()*1.05);
  if((pos = histname.find("AcceptanceDeltaR")) != string::npos){
    h_in->GetYaxis()->SetTitle("Acceptance(%)");
    h_in->SetMaximum(1.2);
    h_in->SetMinimum(0.6);
  }
  if((pos = histname.find("AcceptanceInvMass")) != string::npos){
    h_in->GetYaxis()->SetTitle("Acceptance(%)");
    h_in->SetMaximum(2.0);
    h_in->SetMinimum(0.0);
  }
  if((pos = histname.find("Purity")) != string::npos){
    h_in->GetYaxis()->SetTitle("Purity(%)");
    h_in->SetMaximum(80);
    h_in->SetMinimum(30);
  }
  if((pos = histname.find("Stability")) != string::npos){
    h_in->GetYaxis()->SetTitle("Stability(%)");
    h_in->SetMaximum(80);
    h_in->SetMinimum(0);
  }
  
  h_in->GetYaxis()->SetTitleOffset(1.3);
  h_in->GetXaxis()->SetTitleOffset(1.2);
  h_in->SetLineWidth(2);
  h_in->SetLineColor(kBlue);
  h_in->Draw("hist");
  if(drawError){
    h_err->SetFillStyle(3154);
    h_err->SetFillColor(kBlack);
    h_err->Draw("e2 same");
  }

  label_work->Draw("same");
  label_sim->Draw("same");
  c->Print(Form("../output/pdf/%s_%s.pdf", h_in->GetName(), name.c_str()),"pdf");

  c->~TCanvas();
  h_in->~TH1();
}
