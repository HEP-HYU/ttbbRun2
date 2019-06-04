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
const bool runSystematics = false;

const bool   useTUnfold =  true;
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
const double taumin_Phi = 0.8, taumax_dPhi = 0.9;

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
  "__cferr1up", "__cferr1down",  "__cferr2up",  "__cferr2down",
};

void ttbbDiffXsec(string input_dir, string output_dir, string input){
  ssize_t pos;
  gErrorIgnoreLevel = kFatal; // kWarning
  gROOT->ProcessLine("setTDRStyle();");

  TFile *f_respMatrix = TFile::Open(Form("../output/post/hist_ttbbFilterttbb.root"));
  TFile *f_criteria = TFile::Open(Form("../output/post/hist_criteria_mindR.root"));
  TFile *f_input = TFile::Open(Form("%s/%s.root", input_dir.c_str(), input.c_str()));
  TFile *f_ttbb = TFile::Open(Form("%s/hist_ttbb.root", input_dir.c_str()));
  TH2 *h_resp_dR[nChannel], *h_resp_M[nChannel];
  TH1 *h_data_dR[nChannel], *h_data_M[nChannel];
  TH1 *h_gen_dR[nChannel], *h_gen_M[nChannel];
  TH1 *h_gen_nosel_dR[nChannel], *h_gen_nosel_M[nChannel];

  for(int ich=0; ich<nChannel; ++ich){
    h_resp_dR[ich] = (TH2 *)f_respMatrix->Get(Form("h_mindR_ResponseMatrixDeltaR_Ch%d_S3", ich));
    h_resp_M[ich] = (TH2 *)f_respMatrix->Get(Form("h_mindR_ResponseMatrixInvMass_Ch%d_S3", ich));
    if( (pos = input.find("Data")) != std::string::npos ){
      h_data_dR[ich] = (TH1 *)f_input->Get(Form("h_mindR_%s_Ch%d_S3",RECO_ADDJETS_DELTAR_, ich));
      h_data_M[ich] = (TH1 *)f_input->Get(Form("h_mindR_%s_Ch%d_S3",RECO_ADDJETS_INVARIANT_MASS_, ich)); 
    }
    else{
      h_data_dR[ich] = (TH1 *)f_input->Get(Form("h_mindR_%s_Ch%d_S3",RECO_ADDJETS_DELTAR_, ich));
      h_data_M[ich] = (TH1 *)f_input->Get(Form("h_mindR_%s_Ch%d_S3",RECO_ADDJETS_INVARIANT_MASS_, ich));
    }
    h_gen_dR[ich] = (TH1 *)f_ttbb->Get(Form("h_mindR_%s_Ch%d_S3",GEN_ADDBJETS_DELTAR_, ich));
    h_gen_M[ich] = (TH1 *)f_ttbb->Get(Form("h_mindR_%s_Ch%d_S3",GEN_ADDBJETS_INVARIANT_MASS_, ich));
    h_gen_nosel_dR[ich] = (TH1 *)f_ttbb->Get(Form("h_GenbJetDeltaR_Ch%d_nosel", ich ));
    h_gen_nosel_M[ich] = (TH1 *)f_ttbb->Get(Form("h_GenbJetInvMass_Ch%d_nosel", ich ));
  }

  std::map<const char *, TH2 *> m_sys_dR[2], m_sys_M[2];
  for(int ich=0; ich<nChannel; ++ich){
    //TFile *tmp = TFile::Open("/data/users/seohyun/ntuple/hep2017/v808/nosplit/TTLJ_PowhegPythia_ttbbFilter_ttbb.root");
    TH1 *EventInfo = (TH1 *)f_respMatrix->Get("EventInfo");
    double scale = LUMINOSITY_*31.06/EventInfo->GetBinContent(2);
    //double scale = SF_ttbb*LUMINOSITY_*XSEC_[TTBB_]/EventInfo->GetBinContent(2);
    h_resp_dR[ich]->Scale(scale); h_resp_M[ich]->Scale(scale);
    
    if(runSystematics){
      for(auto v_itr = syst_list.begin(); v_itr != syst_list.end(); ++v_itr){
	if((pos = (*v_itr).find("ps")) != std::string::npos) continue;
	if((pos = (*v_itr).find("sw")) != std::string::npos) continue;
	if((pos = (*v_itr).find("jer")) != std::string::npos) continue;
	if((pos = (*v_itr).find("jec")) != std::string::npos) continue;
	TH2 *tmp_dR = (TH2 *)f_respMatrix->Get(Form("h_mindR_ResponseMatrixDeltaR_Ch%d_S3%s",
	  ich, (*v_itr).c_str()));
	TH2 *tmp_M = (TH2 *)f_respMatrix->Get(Form("h_mindR_ResponseMatrixInvMass_Ch%d_S3%s",
	  ich, (*v_itr).c_str()));
	tmp_dR->Scale(scale); tmp_dR->Scale(scale);
	m_sys_dR[ich].insert(pair<const char *, TH2 *>((*v_itr).c_str(), tmp_dR));
	m_sys_M[ich].insert(pair<const char *, TH2 *>((*v_itr).c_str(), tmp_M));
      }
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
	h_bkg_dR = (TH1 *)f_sample->Get(Form("h_mindR_%s_Ch%d_S3",RECO_ADDJETS_DELTAR_,ich));
        h_bkg_M = (TH1 *)f_sample->Get(Form("h_mindR_%s_Ch%d_S3",RECO_ADDJETS_INVARIANT_MASS_,ich));
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
  TH1 *h_unfolded_dR_lep, *h_unfolded_M_lep;
  TH1 *h_gen_dR_lep, *h_gen_M_lep;
  h_gen_dR_lep = (TH1 *)h_gen_dR[0]->Clone();
  h_gen_dR_lep->Add(h_gen_dR[1]);
  h_gen_M_lep = (TH1 *)h_gen_M[0]->Clone();
  h_gen_M_lep->Add(h_gen_M[1]);

  for(int ich=0; ich<nChannel; ++ich){
    cout << "Channel: " << ich << endl;
    vector<TH1 *> v_unfolded_dR, v_unfolded_M;
    if(!useTUnfold){
      if(findBestK){
	cout << "Find Best k" << endl;
	vector<TH1 *> v_unfolded_dR, v_unfolded_M;
	for(int i=2; i<10; ++i){
	  vector<TH1 *> v_tmp_dR = runSVDUnfold(h_data_dR[ich], h_resp_dR[ich],
	      std::map<const char *, TH1 *>(), std::map<const char *, double>(), 
	      std::map<const char *, TH2 *>(),i);
	  v_unfolded_dR.push_back(v_tmp_dR[0]);
	}
	for(int i=2; i<10; ++i){
	  vector<TH1 *> v_tmp_M = runSVDUnfold(h_data_M[ich], h_resp_M[ich],
	      std::map<const char *, TH1 *>(), std::map<const char *, double>(),
	      std::map<const char *, TH2 *>(), i);
	  v_unfolded_M.push_back(v_tmp_M[0]);
	}
        TH1 *h_truth_dR = (TH1 *)h_gen_dR[ich]->Clone();
        TH1 *h_truth_M = (TH1 *)h_gen_M[ich]->Clone();
	findBestk(v_unfolded_dR, 1, 0, ich, h_truth_dR);
	findBestk(v_unfolded_M, 1, 1, ich, h_truth_M);
      }
      v_unfolded_dR = runSVDUnfold(h_data_dR[ich], h_resp_dR[ich],
          m_bkg_dR[ich], m_scale, m_sys_dR[ich], reg_dR);
      v_unfolded_M = runSVDUnfold(h_data_M[ich], h_resp_M[ich],
          m_bkg_M[ich], m_scale, m_sys_M[ich], reg_M);
    }
    else{
      v_unfolded_dR = runTUnfold(input.c_str(), h_data_dR[ich], h_resp_dR[ich], 
          m_bkg_dR[ich], m_scale, m_sys_dR[ich],
          scanLcurve, taumin_dR, taumax_dR, fixtau_dR, fixedtau_dR);
      v_unfolded_M = runTUnfold(input.c_str(), h_data_M[ich], h_resp_M[ich],
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

    TH1 *h_unfolded_dR, *h_unfolded_M;
    h_unfolded_dR = v_unfolded_dR[0];
    h_unfolded_M = v_unfolded_M[0];
    
    if(ich == 0){
      cout << "wow" << endl;
      h_unfolded_dR_lep = (TH1 *)h_unfolded_dR->Clone();
      h_unfolded_M_lep  = (TH1 *)h_unfolded_M->Clone();
    }
    else{
      cout << "wow" << endl;
      h_unfolded_dR_lep->Add(h_unfolded_dR);
      h_unfolded_M_lep->Add(h_unfolded_M);
    }

    //Draw histogram to validation
    //1. Reco distribution vs. X-axis projection of response matrix
    //2. Gen distribution vs. Y-axis projection of response matrix
    //3. Unfolded reco distribution vs. Gen distribution
    //4. Data - background vs. reco distribution

    TCanvas *c = new TCanvas("","",800,800);
    TLegend *leg = new TLegend(0.6,0.7,0.88,0.88);
    leg->SetTextSize(0.02);

    TPaveText *label_cms = tdrCMSlabel();
    TPaveText *label_sim = tdrCMSSimlabel();

    h_gen_dR[ich]->Write();
    h_gen_M[ich]->Write();
    h_unfolded_dR->Write();
    h_unfolded_M->Write();

    TH1 *h_reco_dR_draw = (TH1 *)h_data_dR[ich]->Clone();
    TH1 *h_respX_dR_draw = (TH1 *)(h_resp_dR[ich]->ProjectionX());
    TH1 *h_reco_M_draw = (TH1 *)h_data_M[ich]->Clone();
    TH1 *h_respX_M_draw = (TH1 *)(h_resp_M[ich]->ProjectionX());
    h_reco_dR_draw->Scale(1/h_reco_dR_draw->Integral());
    h_respX_dR_draw->Scale(1/h_respX_dR_draw->Integral());
    h_reco_M_draw->Scale(1/h_reco_M_draw->Integral());
    h_respX_M_draw->Scale(1/h_respX_M_draw->Integral());

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
    c->Print(Form("%s/%s_h_reco_respX_dR_%d.pdf", output_dir.c_str(), input.c_str(), ich), "pdf");
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
    c->Print(Form("%s/%s_h_reco_respX_M_%d.pdf", output_dir.c_str(), input.c_str(), ich), "pdf");
    leg->Clear();
    c->Clear();
   
    TH1 *h_gen_dR_draw = (TH1 *)h_gen_dR[ich]->Clone();
    TH1 *h_respY_dR_draw = (TH1 *)(h_resp_dR[ich]->ProjectionY());
    TH1 *h_gen_M_draw = (TH1 *)h_gen_M[ich]->Clone();
    TH1 *h_respY_M_draw = (TH1 *)(h_resp_M[ich]->ProjectionY());
    h_gen_dR_draw->Scale(1/h_gen_dR_draw->Integral());
    h_respY_dR_draw->Scale(1/h_respY_dR_draw->Integral());
    h_gen_M_draw->Scale(1/h_gen_M_draw->Integral());
    h_respY_M_draw->Scale(1/h_respY_M_draw->Integral());

    c->cd();
    h_gen_dR_draw->SetMarkerSize(2);
    h_gen_dR_draw->SetMarkerStyle(20);
    h_gen_dR_draw->SetMarkerColor(kBlack);
    h_gen_dR_draw->SetLineColor(kBlack);
    h_respY_dR_draw->SetLineColor(kRed);
    h_respY_dR_draw->SetLineWidth(2);
    h_respY_dR_draw->SetMaximum(h_respY_dR_draw->GetMaximum()*2.0);
    h_respY_dR_draw->SetMinimum(0);
    h_respY_dR_draw->GetXaxis()->SetTitleOffset(1.2);

    leg->AddEntry(h_gen_dR_draw, "gen distribution", "p");
    leg->AddEntry(h_respY_dR_draw, "matrix y-axis projection", "l");

    h_respY_dR_draw->Draw("hist");
    h_gen_dR_draw->Draw("p same");
    leg->Draw("same");
    c->Print(Form("%s/%s_h_gen_respY_dR_%d.pdf", output_dir.c_str(), input.c_str(), ich), "pdf");
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
    h_respY_M_draw->GetXaxis()->SetTitleOffset(1.2);
    
    leg->AddEntry(h_gen_M_draw, "gen distribution", "p");
    leg->AddEntry(h_respY_M_draw, "matrix y-axis projection", "l");

    h_respY_M_draw->Draw("hist");
    h_gen_M_draw->Draw("p same");
    leg->Draw("same");
    c->Print(Form("%s/%s_h_gen_respY_M_%d.pdf", output_dir.c_str(), input.c_str(), ich), "pdf");
    leg->Clear();
    c->Clear();
    
    TH1 *h_unfolded_dR_draw = (TH1 *)h_unfolded_dR->Clone();
    TH1 *h_unfolded_M_draw = (TH1 *)h_unfolded_M->Clone();
    h_gen_dR_draw = (TH1 *)h_gen_dR[ich]->Clone();
    h_gen_M_draw = (TH1 *)h_gen_M[ich]->Clone();
    
    c->cd();
    h_unfolded_dR_draw->SetMarkerSize(2);
    h_unfolded_dR_draw->SetMarkerStyle(20);
    h_unfolded_dR_draw->SetMarkerColor(kBlack);
    h_unfolded_dR_draw->SetLineColor(kBlack);
    h_gen_dR_draw->SetLineColor(kRed);
    h_gen_dR_draw->SetLineWidth(2);
    h_gen_dR_draw->SetMaximum(h_unfolded_dR_draw->GetMaximum()*1.5);
    h_gen_dR_draw->SetMinimum(0);

    if(useTUnfold) leg->SetHeader(Form("tau scanning range: %.3f to %.3f", taumin_dR, taumax_dR));
    else leg->SetHeader(Form("Regularization k = %d", reg_dR));
    leg->AddEntry(h_unfolded_dR_draw, "unfold output", "p");
    leg->AddEntry(h_gen_dR_draw, "Step 3 gen distribution", "l");

    h_gen_dR_draw->Draw("hist");
    h_unfolded_dR_draw->Draw("e p same");
    leg->Draw("same");
    if(useTUnfold)
      c->Print(Form("%s/%s_h_output_gen_dR_%d_tau%.3fto%.3f.pdf", output_dir.c_str(), input.c_str(), ich, taumin_dR, taumax_dR), "pdf");
    else
      c->Print(Form("%s/%s_h_output_gen_dR_%d_k%d.pdf", output_dir.c_str(), input.c_str(), ich, reg_dR), "pdf");
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

    if(useTUnfold) leg->SetHeader(Form("tau scanning range: %.3f to %.3f", taumin_M, taumax_M));
    else leg->SetHeader(Form("Regularization k = %d", reg_M));
    leg->AddEntry(h_unfolded_M_draw, "unfold output", "p");
    leg->AddEntry(h_gen_M_draw, "Step 3 gen distribution", "l");

    h_gen_M_draw->Draw("hist");
    h_unfolded_M_draw->Draw("e p same");
    leg->Draw("same");
    if(useTUnfold) 
      //c->Print(Form("%s/%s_h_output_gen_M_%d_tau%.3fto%.3f.pdf", output_dir.c_str(), input.c_str(), ich, taumin_M, taumax_M), "pdf");
      c->Print(Form("%s/%s_h_output_gen_M_%d.pdf", output_dir.c_str(), input.c_str(), ich), "pdf");
    else
      c->Print(Form("%s/%s_h_output_gen_M_%d_k%d.pdf", output_dir.c_str(), input.c_str(), ich, reg_M), "pdf");
    leg->Clear();
    c->Clear();

    TH1 *h_accept_dR = (TH1 *)f_criteria->Get(Form("h_BinAcceptanceDeltaR_Ch%d",  ich));
    TH1 *h_accept_M  = (TH1 *)f_criteria->Get(Form("h_BinAcceptanceInvMass_Ch%d", ich));
    std::cout << h_accept_dR << std::endl;
    std::cout << h_accept_M << std::endl;

    TH1 *h_MC_diffXsec_nosel_dR = calculateDiffXsec(h_gen_nosel_dR_lep, NULL, 0, true);
    TH1 *h_MC_diffXsec_nosel_M  = calculateDiffXsec(h_gen_nosel_M_lep,  NULL, 1, true);
    TH1 *h_diffXsec_dR = calculateDiffXsec(h_unfolded_dR, h_accept_dR, 0);
    TH1 *h_diffXsec_M  = calculateDiffXsec(h_unfolded_M,  h_accept_M , 0);
    
    c->cd();
    h_MC_diffXsec_nosel_dR->SetFillColor(kRed);
    h_MC_diffXsec_nosel_dR->SetMaximum(h_diffXsec_dR->GetMaximum()*1.5);
    h_MC_diffXsec_nosel_dR->SetMinimum(0);
    h_diffXsec_dR->SetMarkerColor(kBlack);
    h_diffXsec_dR->SetMarkerSize(2);
    h_diffXsec_dR->SetMarkerStyle(20)
    ;
    if(useTUnfold) leg->SetHeader(Form("tau scanning range: %.3f to %.3f", taumin_dR, taumax_dR));
    else leg->SetHeader(Form("Regularization k = %d", reg_dR));
    leg->AddEntry(h_diffXsec_dR, "unfold output", "p");
    leg->AddEntry(h_MC_diffXsec_nosel_dR, "Powheg + Pythia", "F");

    h_MC_diffXsec_nosel_dR->Draw("hist");
    h_diffXsec_dR->Draw("p same");
    label_cms->Draw("same");
    leg->Draw("same");
   
    if(useTUnfold) 
      c->Print(Form("%s/%s_h_diffXsec_dR_%d_tau%.3fto%.3f.pdf", output_dir.c_str(), input.c_str(), ich, taumin_dR, taumax_dR), "pdf");
    else
      c->Print(Form("%s/%s_h_diffXsec_dR_%d_k%d.pdf", output_dir.c_str(), input.c_str(), ich, reg_dR), "pdf");
    leg->Clear();
    c->Clear();

    c->cd();
    h_MC_diffXsec_nosel_M->SetFillColor(kRed);
    h_MC_diffXsec_nosel_M->SetMaximum(h_diffXsec_M->GetMaximum()*1.5);
    h_MC_diffXsec_nosel_M->SetMinimum(0);
    h_diffXsec_M->SetMarkerColor(kBlack);
    h_diffXsec_M->SetMarkerSize(2);
    h_diffXsec_M->SetMarkerStyle(20);

    if(useTUnfold) leg->SetHeader(Form("tau scanning range: %.3f to %.3f", taumin_M, taumax_M));
    else leg->SetHeader(Form("Regularization k = %d", reg_M));
    leg->AddEntry(h_diffXsec_M, "unfold output", "p");
    leg->AddEntry(h_MC_diffXsec_nosel_M, "Powheg + Pythia", "F");

    h_MC_diffXsec_nosel_M->Draw("hist");
    h_diffXsec_M->Draw("p same");
    label_cms->Draw("same");
    leg->Draw("same");
   
    if(useTUnfold) 
      c->Print(Form("%s/%s_h_diffXsec_M_%d_tau%.3fto%.3f.pdf", output_dir.c_str(), input.c_str(), ich, taumin_M, taumax_M), "pdf");
    else
      c->Print(Form("%s/%s_h_diffXsec_M_%d_k%d.pdf", output_dir.c_str(), input.c_str(), ich, reg_M), "pdf");
    leg->Clear();
    c->Clear();

  }
  std::cout << "Finish unfolding" << endl;
  
  TCanvas *c = new TCanvas("","",800,800);
  TLegend *leg = new TLegend(0.6,0.7,0.88,0.88);
  leg->SetTextSize(0.02);

  TPaveText *label_cms = tdrCMSlabel();
  TPaveText *label_sim = tdrCMSSimlabel();


  c->cd();
  h_gen_dR_lep->SetLineColor(kRed);
  h_gen_dR_lep->SetLineWidth(2);
  h_gen_dR_lep->SetMaximum(h_unfolded_dR_lep->GetMaximum()*1.5);
  h_gen_dR_lep->SetMinimum(0);
  h_unfolded_dR_lep->SetMarkerColor(kBlack);
  h_unfolded_dR_lep->SetMarkerSize(2);
  h_unfolded_dR_lep->SetMarkerStyle(20);
     
  if(useTUnfold) leg->SetHeader(Form("tau scanning range: %.3f to %.3f", taumin_dR, taumax_dR));
  else leg->SetHeader(Form("Regularization k = %d", reg_dR));
  leg->AddEntry(h_unfolded_dR_lep, "unfold output", "p");
  leg->AddEntry(h_gen_dR_lep, "Powheg + Pythia", "l");

  h_gen_dR_lep->Draw("hist");
  h_unfolded_dR_lep->Draw("p same");
  label_sim->Draw("same");
  leg->Draw("same");
   
  if(useTUnfold) 
    c->Print(Form("%s/%s_h_output_gen_dR_lep_tau%.3fto%.3f.pdf", output_dir.c_str(), input.c_str(), taumin_dR, taumax_dR), "pdf");
  else
    c->Print(Form("%s/%s_h_output_gen_dR_lep_k%d.pdf", output_dir.c_str(), input.c_str(), reg_dR), "pdf");
  leg->Clear();
  c->Clear();
  
  c->cd();
  h_gen_M_lep->SetLineColor(kRed);
  h_gen_M_lep->SetLineWidth(2);
  h_gen_M_lep->SetMaximum(h_unfolded_M_lep->GetMaximum()*1.5);
  h_gen_M_lep->SetMinimum(0);
  h_unfolded_M_lep->SetMarkerColor(kBlack);
  h_unfolded_M_lep->SetMarkerSize(2);
  h_unfolded_M_lep->SetMarkerStyle(20);
     
  if(useTUnfold) leg->SetHeader(Form("tau scanning range: %.3f to %.3f", taumin_M, taumax_M));
  else leg->SetHeader(Form("Regularization k = %d", reg_M));
  leg->AddEntry(h_unfolded_M_lep, "unfold output", "p");
  leg->AddEntry(h_gen_M_lep, "Powheg + Pythia", "l");

  h_gen_M_lep->Draw("hist");
  h_unfolded_M_lep->Draw("p same");
  label_sim->Draw("same");
  leg->Draw("same");
   
  if(useTUnfold) 
    c->Print(Form("%s/%s_h_output_gen_M_lep_tau%.3fto%.3f.pdf", output_dir.c_str(), input.c_str(), taumin_M, taumax_M), "pdf");
  else
    c->Print(Form("%s/%s_h_output_gen_M_lep_k%d.pdf", output_dir.c_str(), input.c_str(), reg_M), "pdf");
  leg->Clear();
  c->Clear();
  
  f_out->Write();
  f_out->Close();
}
