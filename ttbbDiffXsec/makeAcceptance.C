#include <iostream>
#include <fstream>
#include <vector>

#include "ttbbDiffXsec.h"

HistoBook *MakeHist(const char *genmode, TFile *f_in, std::string syst);
void DrawHist(std::string year, TH1 *h_in, bool drawError = false);

void makeAcceptance(std::string year, std::string ttbb, bool runSystematics){
  string input_dir = "../output/root" + year;
  TFile *f_in = TFile::Open(Form("%s/hist_%s.root", input_dir.c_str(), ttbb.c_str()));
  TFile *f_out = TFile::Open(Form("%s/hist_accept_%s.root", input_dir.c_str(), genMode),"recreate");

  std::vector<std::string> v_syst_list = syst_total;
  std::vector<std::string> v_syst_lep;
  if( year == "16" ){
    v_syst_lep = {
      "__musfup", "__musfdown", "__elsfup", "__elsfdown"
    };
  }
  else{
    v_syst_lep = {
    "__muidup",   "__muiddown", "__muisoup",  "__muisodown",
    "__elidup",   "__eliddown", "__elrecoup", "__elrecodown",
    "__elzvtxup", "__elzvtxdown",
    };
  }
  
  auto h_tmp = MakeHist(genMode, f_in, "");
  
  f_out->cd();
  for(int ich=0; ich<nChannel; ++ich){
    h_tmp->h_stability_deltaR[ich]->Write();
    h_tmp->h_stability_invMass[ich]->Write();
    h_tmp->h_purity_deltaR[ich]->Write();
    h_tmp->h_purity_invMass[ich]->Write();
    h_tmp->h_acceptance_deltaR[ich]->Write();
    h_tmp->h_acceptance_invMass[ich]->Write();

    DrawHist(year, h_tmp->h_stability_deltaR[ich]);
    DrawHist(year, h_tmp->h_stability_invMass[ich]);
    DrawHist(year, h_tmp->h_purity_deltaR[ich]);
    DrawHist(year, h_tmp->h_purity_invMass[ich]);
    DrawHist(year, h_tmp->h_acceptance_deltaR[ich], true);
    DrawHist(year, h_tmp->h_acceptance_invMass[ich], true);
  }
  h_tmp->~HistoBook();

  for(auto v_itr = v_syst_list.begin(); v_itr != v_syst_list.end(); ++v_itr){
    std::cout << "systematics:" << *v_itr << std::endl;
    auto h_tmp = MakeHist(genMode, f_in, *v_itr);
    
    for(int ich=0; ich<nChannel; ++ich){
      h_tmp->h_stability_deltaR[ich]->Write();
      h_tmp->h_stability_invMass[ich]->Write();
      h_tmp->h_purity_deltaR[ich]->Write();
      h_tmp->h_purity_invMass[ich]->Write();
      h_tmp->h_acceptance_deltaR[ich]->Write();
      h_tmp->h_acceptance_invMass[ich]->Write();
    }
    h_tmp->~HistoBook();
  }

  for(auto v_itr = v_syst_lep.begin(); v_itr != v_syst_lep.end(); ++v_itr){
    std::cout << "systematics:" << *v_itr << std::endl;
    auto h_tmp = MakeHist(genMode, f_in, *v_itr);
    
    for(int ich=0; ich<nChannel; ++ich){
      h_tmp->h_stability_deltaR[ich]->Write();
      h_tmp->h_stability_invMass[ich]->Write();
      h_tmp->h_purity_deltaR[ich]->Write();
      h_tmp->h_purity_invMass[ich]->Write();
      h_tmp->h_acceptance_deltaR[ich]->Write();
      h_tmp->h_acceptance_invMass[ich]->Write();
    }
    h_tmp->~HistoBook();
  }
  //f_out->Write();
  f_out->Close();
}

HistoBook *MakeHist(const char *genmode, TFile *f_in, std::string syst){
  std::ofstream fout(Form("BinCriteria%s.txt", syst.c_str()));
  fout << "Systematic sample: " << syst << std::endl;
  //NORMALIZATION
  //double xsec = 0.0;
  //if( syst.find("sw",0,2) != std::string::npos ) xsec = 831.76;
  //else if( syst.find("tune",0,4) != std::string::npos ) xsec = 831.76;
  //else xsec = XSEC_[TTBB_];
  //TH1 *EventInfo = (TH1 *)f_in->Get("EventInfo");
  //const double scale = SF_ttbb*LUMINOSITY_*xsec/EventInfo->GetBinContent(2);

  //HISTOGRAMS
  HistoBook *h_stb = new HistoBook(3,syst.c_str());

  TH2D *h_matrix_deltaR[nChannel];
  TH2D *h_matrix_invMass[nChannel];

  TH1D *h_gen_deltaR[nChannel];
  TH1D *h_gen_invMass[nChannel];

  TH1D *h_reco_deltaR[nChannel];
  TH1D *h_reco_invMass[nChannel];

  TH1D *h_gen_deltaR_nosel[nChannel];
  TH1D *h_gen_invMass_nosel[nChannel];

  for(int ich=0; ich < nChannel; ++ich){    
    h_gen_deltaR_nosel[ich]    = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_nosel", genmode, GEN_ADD_DR_,   ich));
    h_gen_invMass_nosel[ich]   = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_nosel", genmode, GEN_ADD_M_,    ich));
    
    h_matrix_deltaR[ich]   = (TH2D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, MATRIX_DR_,   ich, syst.c_str()));
    h_matrix_invMass[ich]  = (TH2D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, MATRIX_M_,    ich, syst.c_str()));

    h_gen_deltaR[ich]   = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, GEN_ADD_DR_,   ich, syst.c_str()));
    h_gen_invMass[ich]  = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, GEN_ADD_M_,    ich, syst.c_str()));

    h_reco_deltaR[ich]   = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, RECO_ADD_DR_,   ich, syst.c_str())); 
    h_reco_invMass[ich]  = (TH1D *)f_in->Get(Form("h_%s_%s_Ch%d_S3%s", genmode, RECO_ADD_M_,   ich, syst.c_str())); 
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
  }//Channel

  return h_stb;
}

void DrawHist(std::string year, TH1* h_in_, bool drawError = false){    
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
    h_in->SetMaximum(4.0);
    h_in->SetMinimum(0.0);
  }
  if((pos = histname.find("AcceptanceInvMass")) != string::npos){
    h_in->GetYaxis()->SetTitle("Acceptance(%)");
    h_in->SetMaximum(4.5);
    h_in->SetMinimum(0.0);
  }
  if((pos = histname.find("Purity")) != string::npos){
    h_in->GetYaxis()->SetTitle("Purity(%)");
    h_in->SetMaximum(100);
    h_in->SetMinimum(0);
  }
  if((pos = histname.find("Stability")) != string::npos){
    h_in->GetYaxis()->SetTitle("Stability(%)");
    h_in->SetMaximum(100);
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

  //label_work->Draw("same");
  label_sim->Draw("same");
  c->Print(Form("../output/unfold/%s/%s.pdf", year.c_str(), h_in->GetName()),"pdf");

  c->~TCanvas();
  h_in->~TH1();
}
