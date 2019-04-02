/********************************************************************************
 * run by SVDUnfold :root -l -b -q ttbbDiffXsec.C'(false)'			*
 * run by TUnfold without tau scanning range constraint : 			*
 * 	root -l -b -q ttbbDiffXsec.C'(true)' 					*
 * run by TUnfold with tau scanning range constraint :				*
 * 	root -l -b -q ttbbDiffXec.C'(true, taumin, taumax)'			*
 * run by TUnfold with fixed tau :						*
 * 	root -l -b -q ttbbDiffXsec.C'(true, 0., 0., true, fixedtau)'		*
 ********************************************************************************/

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

#include <TLegend.h>
#include <TAttFill.h>

#include "ttbbDiffXsec.h"
#include "runTUnfold.C"
#include "runSVDUnfold.C"

//Unfolding options
const bool useTUnfold = true;
const bool scanLcurve = true;
const bool fixtau_dR = false;
const float fixedtau_dR = 0.01;//log_{10}(#tau)
const double taumin_dR = 0.1, taumax_dR = 0.5;
const bool fixtau_M = false;
const float fixedtau_M = 0.0;
const double taumin_M = 0.8, taumax_M = 1.3;
const bool findBestK = false;
const int reg_dR = 3, reg_M = 3;

const double SF_ttbb = 0.992;
const double SF_ttbj = 0.997;
const double SF_ttcc = 0.871;
const double SF_ttLF = 0.890;

std::vector<std::string> syst_list = {
  "__psup", "__psdown", "__swup", "__swdown",
  "__jerup", "__jerdown", "__jecup", "__jecdown",
  "__musfup", "__musfdown", "__mutrgup", "__mutrgdown",
  "__elsfup", "__elsfdown", "__eltrgup", "__eltrgdown",
  "__lfup", "__lfdown", "__hfup", "__hfdown",
  "__hfstat1up","__hfstat1down", "__hfstat2up", "__hfstat2down",
  "__lfstat1up","__lfstat1down", "__lfstat2up", "__lfstat2down",
  "__cferr1up", "__cferr1down",  "__cferr2up",  "__cferr2down"
};

void ttbbDiffXsec(string input_dir, string output_dir, string input){
  gErrorIgnoreLevel = kFatal; // kWarning

  TFile *f_respMatrix = TFile::Open(Form("../output/post/hist_ttbbFilter_ttbb.root"));
  //TFile *f_respMatrix = TFile::Open(Form("../output/post/hist_ttbb.root"));
  TFile *f_criteria = TFile::Open(Form("../output/post/hist_criteria.root"));
  TFile *f_input = TFile::Open(Form("%s/%s.root", input_dir.c_str(), input.c_str()));
  TFile *f_ttbb = TFile::Open(Form("%s/hist_ttbb.root", input_dir.c_str()));
  TH2 *h_resp_dR[nChannel], *h_resp_M[nChannel];
  TH1 *h_data_dR[nChannel], *h_data_M[nChannel];
  TH1 *h_gen_dR[nChannel], *h_gen_M[nChannel];
  TH1 *h_gen_nosel_dR[nChannel], *h_gen_nosel_M[nChannel];

  for(int ich=0; ich<nChannel; ++ich){
    h_resp_dR[ich] = (TH2 *)f_respMatrix->Get(Form("h_ResponseMatrixDeltaR_Ch%d_S3", ich));
    h_resp_M[ich] = (TH2 *)f_respMatrix->Get(Form("h_ResponseMatrixInvMass_Ch%d_S3", ich));
    if(input.find("Data")){
      h_data_dR[ich] = (TH1 *)f_input->Get(Form("h_%s_Ch%d_S3",RECO_ADDJETS_DELTAR_, ich));
      h_data_M[ich] = (TH1 *)f_input->Get(Form("h_%s_Ch%d_S3",RECO_ADDJETS_INVARIANT_MASS_, ich));
    }   
    else{
      h_data_dR[ich] = (TH1 *)f_input->Get(Form("h_%s_Ch%d_S3",RECO_ADDJETS_DELTAR_, ich));
      h_data_M[ich] = (TH1 *)f_input->Get(Form("h_%s_Ch%d_S3",RECO_ADDJETS_INVARIANT_MASS_, ich));
    }
    h_gen_dR[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_S3",GEN_ADDBJETS_DELTAR_, ich));
    h_gen_M[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_S3",GEN_ADDBJETS_INVARIANT_MASS_, ich));
    h_gen_nosel_dR[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_nosel",GEN_ADDBJETS_DELTAR_, ich ));
    h_gen_nosel_M[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_nosel",GEN_ADDBJETS_INVARIANT_MASS_, ich ));
  }

  ssize_t pos;
  std::map<const char *, TH2 *> m_sys_dR[2], m_sys_M[2];
  for(int ich=0; ich<nChannel; ++ich){
    TFile *tmp = TFile::Open("/data/users/seohyun/ntuple/hep2017/v808/nosplit/TTLJ_PowhegPythia_ttbbFilter_ttbb.root");
    TH1 *EventInfo = (TH1 *)tmp->Get("ttbbLepJets/EventInfo");
    double scale = LUMINOSITY_*31.06/EventInfo->GetBinContent(2);
    //double scale = SF_ttbb*LUMINOSITY_*XSEC_[TTBB_]/EventInfo->GetBinContent(2);
    h_resp_dR[ich]->Scale(scale); h_resp_M[ich]->Scale(scale);
    for(auto v_itr = syst_list.begin(); v_itr != syst_list.end(); ++v_itr){
      if((pos = (*v_itr).find("ps")) != std::string::npos) continue;
      if((pos = (*v_itr).find("sw")) != std::string::npos) continue;
      if((pos = (*v_itr).find("jer")) != std::string::npos) continue;
      if((pos = (*v_itr).find("jec")) != std::string::npos) continue;
      TH2 *tmp_dR = (TH2 *)f_respMatrix->Get(Form("h_ResponseMatrixDeltaR_Ch%d_S3%s",
        ich, (*v_itr).c_str()));
      TH2 *tmp_M = (TH2 *)f_respMatrix->Get(Form("h_ResponseMatrixInvMass_Ch%d_S3%s",
        ich, (*v_itr).c_str()));
      tmp_dR->Scale(scale); tmp_dR->Scale(scale);
      m_sys_dR[ich].insert(pair<const char *, TH2 *>((*v_itr).c_str(), tmp_dR));
      m_sys_M[ich].insert(pair<const char *, TH2 *>((*v_itr).c_str(), tmp_M));
    }
  }

  std::map<const char *, TH1 *> m_bkg_dR[2], m_bkg_M[2];
  std::map<const char *, double> m_scale;
  if( (pos = input.find("Data")) != std::string::npos ){
    for(int i=0; i<static_cast<int>(Sample_List_::LAST); ++i){
      TFile *f_sample = TFile::Open(Form("%s/hist_%s.root", input_dir.c_str(), NAME_[i].c_str()));
      TH1 *EventInfo = (TH1 *)f_sample->Get("EventInfo");
      double scale = LUMINOSITY_*XSEC_[i]/EventInfo->GetBinContent(2);
      if     (i == TTBB_) scale *= SF_ttbb;
      else if(i == TTBJ_) scale *= SF_ttbj;
      else if(i == TTCC_) scale *= SF_ttcc;
      else if(i == TTLF_) scale *= SF_ttLF;
      else                scale *= 1;
      m_scale.insert(pair<const char *, double>(NAME_[i].c_str(), scale));
      
      for(int ich=0; ich<nChannel; ++ich){
	if(i == TTBB_) continue;
	TH1 *h_bkg_dR, *h_bkg_M;
	h_bkg_dR = (TH1 *)f_sample->Get(Form("h_%s_Ch%d_S3",RECO_ADDJETS_DELTAR_,ich));
        h_bkg_M = (TH1 *)f_sample->Get(Form("h_%s_Ch%d_S3",RECO_ADDJETS_INVARIANT_MASS_,ich));
	m_bkg_dR[ich].insert(pair<const char *, TH1 *>(NAME_[i].c_str(), h_bkg_dR));
	m_bkg_M[ich].insert(pair<const char *, TH1 *>(NAME_[i].c_str(), h_bkg_M));
      }
    }
  }
  else{
    TH1 *EventInfo = (TH1 *)f_ttbb->Get("EventInfo");
    double scale = SF_ttbb*LUMINOSITY_*XSEC_[TTBB_]/EventInfo->GetBinContent(2);
    for(int ich=0; ich < nChannel; ++ich){
      h_data_dR[ich]->Scale(scale); h_data_M[ich]->Scale(scale);
    }
  }

  TH1 *EventInfo = (TH1 *)f_ttbb->Get("EventInfo");
  double scale = SF_ttbb*LUMINOSITY_*XSEC_[TTBB_]/EventInfo->GetBinContent(2);
  EventInfo->~TH1();
  for(int ich=0; ich < nChannel; ++ich){
    h_gen_dR[ich]->Scale(scale);
    h_gen_M[ich]->Scale(scale);
    h_gen_nosel_dR[ich]->Scale(scale);
    h_gen_nosel_M[ich]->Scale(scale);
  }

  TH1 *h_gen_nosel_dR_lep = (TH1 *)h_gen_nosel_dR[0]->Clone();
  h_gen_nosel_dR_lep->Add(h_gen_nosel_dR[1]);
  h_gen_nosel_dR_lep->SetName("h_GenbJetDeltaR_lep_nosel_ttbb");
  TH1 *h_gen_nosel_M_lep = (TH1 *)h_gen_nosel_M[0]->Clone();
  h_gen_nosel_M_lep->Add(h_gen_nosel_M[1]);
  h_gen_nosel_M_lep->SetName("h_GenbJetInvMass_lep_nosel_ttbb");

  TFile *f_out = TFile::Open(Form("%s/hist_unfolded_%s.root", output_dir.c_str(), input.c_str()),"recreate");
  f_out->cd();
  cout << "Start unfolding..." << endl;
  for(int ich=0; ich<nChannel; ++ich){
    vector<TH1 *> v_unfolded_dR, v_unfolded_M;
    if(!useTUnfold){
      if(findBestK){
	cout << "Find Best k" << endl;
	vector<TH1 *> v_unfolded_dR, v_unfolded_M;
	for(int i=2; i<10; ++i){
	  vector<TH1 *> v_tmp_dR = runSVDUnfold(h_data_dR[ich], h_resp_dR[ich],
	      std::map<const char *, TH1 *>(), std::map<const char *, double>(), 
	      std::map<const char *, TH2 *>(),i);
	  //h_tmpMC_dR = calculateDiffXsec(h_tmpMC_dR, 0, ich, false);
	  //h_tmpData_dR = calculateDiffXsec(h_tmpData_dR, 0, ich, true);
	  v_unfolded_dR.push_back(v_tmp_dR[0]);
	}
	for(int i=2; i<10; ++i){
	  vector<TH1 *> v_tmp_M = runSVDUnfold(h_data_M[ich], h_resp_M[ich],
	      std::map<const char *, TH1 *>(), std::map<const char *, double>(),
	      std::map<const char *, TH2 *>(), i);
	  //h_tmpMC_M = calculateDiffXsec(h_tmpMC_M, 1, ich, false);
	  //h_tmpData_M = calculateDiffXsec(h_tmpData_M, 1, ich, true);
	  v_unfolded_M.push_back(v_tmp_M[0]);
	}
        TH1 *h_truth_dR = (TH1 *)h_gen_dR[ich]->Clone();//calculateDiffXsec(h_gen_nosel_dR_lep);
        TH1 *h_truth_M = (TH1 *)h_gen_M[ich]->Clone();//calculateDiffXsec(h_gen_nosel_M_lep);
	findBestk(v_unfolded_dR, 1, 0, ich, h_truth_dR);
	findBestk(v_unfolded_M, 1, 1, ich, h_truth_M);
      }
      v_unfolded_dR = runSVDUnfold(h_data_dR[ich], h_resp_dR[ich],
          m_bkg_dR[ich], m_scale, m_sys_dR[ich], reg_dR);
      v_unfolded_M = runSVDUnfold(h_data_M[ich], h_resp_M[ich],
          m_bkg_M[ich], m_scale, m_sys_M[ich], reg_M);
    }
    else{
      v_unfolded_dR = runTUnfold(h_data_dR[ich], h_resp_dR[ich], 
          m_bkg_dR[ich], m_scale, m_sys_dR[ich],
          scanLcurve, taumin_dR, taumax_dR, fixtau_dR, fixedtau_dR);
      v_unfolded_M = runTUnfold(h_data_M[ich], h_resp_M[ich],
          m_bkg_M[ich], m_scale, m_sys_M[ich],
          scanLcurve, taumin_M, taumax_M, fixtau_M, fixedtau_M);
    }  

    std::cout << v_unfolded_dR.size() << std::endl;
    for(auto v_itr=v_unfolded_dR.begin(); v_itr != v_unfolded_dR.begin(); ++v_itr){
      (*v_itr)->Write();
    }

    for(auto v_itr=v_unfolded_M.begin(); v_itr != v_unfolded_M.begin(); ++v_itr){
      (*v_itr)->Write();
    }

    TH1 *h_acc_dR = (TH1 *)f_criteria->Get(Form("h_BinAcceptanceDeltaR_Ch%d", ich));
    TH1 *h_acc_M  = (TH1 *)f_criteria->Get(Form("h_BinAcceptanceInvMass_Ch%d", ich));
    
    TH1 *h_unfolded_dR, *h_unfolded_M;
    h_unfolded_dR = v_unfolded_dR[0];
    h_unfolded_M = v_unfolded_M[0];
    //Closure Test & Data
    TH1 *h_diffXsec_dR = calculateDiffXsec(h_unfolded_dR, h_acc_dR, 0, false);
    TH1 *h_diffXsec_M = calculateDiffXsec(h_unfolded_M, h_acc_M, 1, false);
    h_unfolded_dR->SetName(Form("h_unfolded_dR_Ch%d_S3", ich));
    h_unfolded_M->SetName(Form("h_unfolded_M_Ch%d_S3", ich));
    h_diffXsec_dR->SetName(Form("h_diffXsec_dR_Ch%d", ich));
    h_diffXsec_M->SetName(Form("h_diffXsec_M_Ch%d", ich));
    h_gen_dR[ich]->Write();
    h_gen_M[ich]->Write();
    h_data_dR[ich]->Write();
    h_data_M[ich]->Write();
    h_unfolded_dR->Write();
    h_unfolded_M->Write();
    h_diffXsec_dR->Write();
    h_diffXsec_M->Write();

    for(auto v_itr=syst_list.begin(); v_itr != syst_list.end(); ++v_itr){
      TH1 *h_acc_dR_sys = (TH1 *)f_criteria->Get(
        Form("h_BinAcceptanceDeltaR_Ch%d%s", ich, (*v_itr).c_str()));
      TH1 *h_acc_M_sys = (TH1 *)f_criteria->Get(
        Form("h_BinAcceptanceInvMass_Ch%d%s", ich, (*v_itr).c_str()));
      TH1 *h_diffXsec_dR_sys = calculateDiffXsec(h_unfolded_dR, h_acc_dR_sys, 0, false);
      TH1 *h_diffXsec_M_sys = calculateDiffXsec(h_unfolded_M, h_acc_M_sys, 0, false);
      h_diffXsec_dR_sys->SetName(Form("h_diffXsec_dR_Ch%d%s", ich, (*v_itr).c_str()));
      h_diffXsec_M_sys->SetName(Form("h_diffXsec_M_Ch%d%s", ich, (*v_itr).c_str()));
      h_diffXsec_dR_sys->Write();
      h_diffXsec_M_sys->Write();
    }


    //Draw histogram to validation
    //1. Reco distribution vs. X-axis projection of response matrix
    //2. Gen distribution vs. Y-axis projection of response matrix
    //3. Unfolded reco distribution vs. Gen distribution
    //4. Data - background vs. reco distribution
    gROOT->ProcessLine("setTDRStyle();");
    TCanvas *c = new TCanvas("","",800,800);
    TLegend *leg = new TLegend(0.6,0.7,0.88,0.88);

    TPaveText *label_cms = tdrCMSlabel();

    TH1 *h_reco_dR_draw = (TH1 *)h_data_dR[ich]->Clone();
    TH1 *h_respX_dR_draw = (TH1 *)(h_resp_dR[ich]->ProjectionX());
    TH1 *h_reco_M_draw = (TH1 *)h_data_M[ich]->Clone();
    TH1 *h_respX_M_draw = (TH1 *)(h_resp_M[ich]->ProjectionX());

    c->cd();
    h_reco_dR_draw->SetMarkerSize(2);
    h_reco_dR_draw->SetMarkerStyle(20);
    h_reco_dR_draw->SetMarkerColor(kBlack);
    h_reco_dR_draw->SetLineColor(kBlack);
    h_respX_dR_draw->SetLineColor(kBlue);
    h_respX_dR_draw->SetLineWidth(2);
    h_respX_dR_draw->SetMaximum(h_reco_dR_draw->GetMaximum()*2.0);
    h_respX_dR_draw->SetMinimum(0);

    leg->AddEntry(h_reco_dR_draw, "reco distribution", "p");
    leg->AddEntry(h_respX_dR_draw, "matrix x-axis projection", "l");

    h_respX_dR_draw->Draw("hist");
    h_reco_dR_draw->Draw("p same");
    leg->Draw("same");
    c->Print(Form("%s/h_reco_respX_dR_%d.pdf", output_dir.c_str(), ich), "pdf");
    leg->Clear();
    c->Clear();

    c->cd();
    h_reco_M_draw->SetMarkerSize(2);
    h_reco_M_draw->SetMarkerStyle(20);
    h_reco_M_draw->SetMarkerColor(kBlack);
    h_reco_M_draw->SetLineColor(kBlack);
    h_respX_M_draw->SetLineColor(kBlue);
    h_respX_M_draw->SetLineWidth(2);
    h_respX_M_draw->SetMaximum(h_reco_M_draw->GetMaximum()*2.0);
    h_respX_M_draw->SetMinimum(0);

    leg->AddEntry(h_reco_M_draw, "reco distribution", "p");
    leg->AddEntry(h_respX_M_draw, "matrix x-axis projection", "l");

    h_respX_M_draw->Draw("hist");
    h_reco_M_draw->Draw("p same");
    leg->Draw("same");
    c->Print(Form("%s/h_reco_respX_M_%d.pdf", output_dir.c_str(), ich), "pdf");
    leg->Clear();
    c->Clear();
   
    TH1 *h_gen_dR_draw = (TH1 *)h_gen_dR[ich]->Clone();
    TH1 *h_respY_dR_draw = (TH1 *)(h_resp_dR[ich]->ProjectionY());
    TH1 *h_gen_M_draw = (TH1 *)h_gen_M[ich]->Clone();
    TH1 *h_respY_M_draw = (TH1 *)(h_resp_M[ich]->ProjectionY());
     
    c->cd();
    h_gen_dR_draw->SetMarkerSize(2);
    h_gen_dR_draw->SetMarkerStyle(20);
    h_gen_dR_draw->SetMarkerColor(kBlack);
    h_gen_dR_draw->SetLineColor(kBlack);
    h_respY_dR_draw->SetLineColor(kRed);
    h_respY_dR_draw->SetLineWidth(2);
    h_respY_dR_draw->SetMaximum(h_respY_dR_draw->GetMaximum()*2.0);
    h_respY_dR_draw->SetMinimum(0);

    leg->AddEntry(h_gen_dR_draw, "gen distribution", "p");
    leg->AddEntry(h_respY_dR_draw, "matrix y-axis projection", "l");

    h_respY_dR_draw->Draw("hist");
    h_gen_dR_draw->Draw("p same");
    leg->Draw("same");
    c->Print(Form("%s/h_gen_respY_dR_%d.pdf", output_dir.c_str(), ich), "pdf");
    leg->Clear();
    c->Clear();

    c->cd();
    h_gen_M_draw->SetMarkerSize(2);
    h_gen_M_draw->SetMarkerStyle(20);
    h_gen_M_draw->SetMarkerColor(kBlack);
    h_gen_M_draw->SetLineColor(kBlack);
    h_respY_M_draw->SetLineColor(kRed);
    h_respY_M_draw->SetLineWidth(2);
    h_respY_M_draw->SetMaximum(h_respY_M_draw->GetMaximum()*2.0);
    h_respY_M_draw->SetMinimum(0);

    leg->AddEntry(h_gen_M_draw, "gen distribution", "p");
    leg->AddEntry(h_respY_M_draw, "matrix y-axis projection", "l");

    h_respY_M_draw->Draw("hist");
    h_gen_M_draw->Draw("p same");
    leg->Draw("same");
    c->Print(Form("%s/h_gen_respY_M_%d.pdf", output_dir.c_str(), ich), "pdf");
    leg->Clear();
    c->Clear();
    //3. Unfolded reco distribution vs. Gen distribution
    //4. Data - background vs. reco distribution

    TH1 *h_unfolded_dR_draw = (TH1 *)h_unfolded_dR->Clone();
    TH1 *h_unfolded_M_draw = (TH1 *)h_unfolded_M->Clone();

    c->cd();
    h_unfolded_dR_draw->SetMarkerSize(2);
    h_unfolded_dR_draw->SetMarkerStyle(20);
    h_unfolded_dR_draw->SetMarkerColor(kBlack);
    h_unfolded_dR_draw->SetLineColor(kBlack);
    h_gen_dR_draw->SetLineColor(kRed);
    h_gen_dR_draw->SetLineWidth(2);
    h_gen_dR_draw->SetMaximum(h_unfolded_dR_draw->GetMaximum()*1.5);
    h_gen_dR_draw->SetMinimum(0);

    leg->AddEntry(h_unfolded_dR_draw, "unfold output", "p");
    leg->AddEntry(h_gen_dR_draw, "Step 3 gen distribution", "l");

    h_gen_dR_draw->Draw("hist");
    h_unfolded_dR_draw->Draw("e p same");
    leg->Draw("same");
    c->Print(Form("%s/h_output_gen_dR_%d.pdf", output_dir.c_str(), ich), "pdf");
    leg->Clear();
    c->Clear();

    c->cd();
    h_unfolded_M_draw->SetMarkerSize(2);
    h_unfolded_M_draw->SetMarkerStyle(20);
    h_unfolded_M_draw->SetMarkerColor(kBlack);
    h_unfolded_M_draw->SetLineColor(kBlack);
    h_gen_M_draw->SetLineColor(kRed);
    h_gen_M_draw->SetLineWidth(2);
    h_gen_M_draw->SetMaximum(h_unfolded_M_draw->GetMaximum()*1.5);
    h_gen_M_draw->SetMinimum(0);

    leg->AddEntry(h_unfolded_M_draw, "unfold output", "p");
    leg->AddEntry(h_gen_M_draw, "Step 3 gen distribution", "l");

    h_gen_M_draw->Draw("hist");
    h_unfolded_M_draw->Draw("e p same");
    leg->Draw("same");
    c->Print(Form("%s/h_output_gen_M_%d.pdf", output_dir.c_str(), ich), "pdf");
    leg->Clear();
    c->Clear();

    //TH1 *h_input_dR = (TH1 *)v_unfolded_dR[1]->Clone();
    //TH1 *h_input_M = (TH1 *)v_unfolded_M[1]->Clone();
  }

  TH1 *h_MC_diffXsec_nosel_dR = calculateDiffXsec(h_gen_nosel_dR_lep, NULL, 0, true);
  TH1 *h_MC_diffXsec_nosel_M = calculateDiffXsec(h_gen_nosel_M_lep, NULL, 1, true);
  h_gen_nosel_dR_lep->Write();
  h_gen_nosel_M_lep->Write();
  h_MC_diffXsec_nosel_dR->SetName("h_diffXsec_dR_lep");
  h_MC_diffXsec_nosel_M->SetName("h_diffXsec_M_lep");
  h_MC_diffXsec_nosel_dR->Write();
  h_MC_diffXsec_nosel_M->Write();

  f_out->Write();
  f_out->Close();
}
