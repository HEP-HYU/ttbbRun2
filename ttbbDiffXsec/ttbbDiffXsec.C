#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

#include <TLegend.h>
#include <TAttFill.h>

#include "ttbbDiffXsec.h"
#include "runTUnfold.C"
//#include "runSVDUnfold.C"

void ttbbDiffXsec(string input_dir, string output_dir, string input){
  ssize_t pos;
  gErrorIgnoreLevel = kFatal; // kWarning
  gROOT->ProcessLine("setTDRStyle();");

  auto syst_total = syst_list;
  syst_total.insert(syst_total.end(), syst_ttbar.begin(), syst_ttbar.end());
  std::cout << "Load Files..." << std::endl;
  TFile *f_out = TFile::Open(Form("%s/hist_unfolded_%s.root", output_dir.c_str(), input.c_str()),"recreate");

  TFile *f_matrix = TFile::Open(Form("../output/post/hist_ttbbFilterttbb.root"));
  TFile *f_criteria = TFile::Open(Form("../output/post/hist_criteria_mindR.root"));
  TFile *f_input = TFile::Open(Form("%s/%s.root", input_dir.c_str(), input.c_str()));
  TFile *f_ttbb = TFile::Open(Form("%s/hist_ttbb.root", input_dir.c_str()));
  TH2 *h_resp_dR[nChannel], *h_resp_M[nChannel];
  TH1 *h_data_dR[nChannel], *h_data_M[nChannel];
  TH1 *h_gen_dR[nChannel],  *h_gen_M[nChannel];
  TH1 *h_gen_nosel_dR[nChannel],   *h_gen_nosel_M[nChannel]; 

  for(int ich=0; ich<nChannel; ++ich){
    h_resp_dR[ich]   = (TH2 *)f_matrix->Get(Form("h_%s_%s_Ch%d_S3", genMode, MATRIX_DR_,   ich));
    h_resp_M[ich]    = (TH2 *)f_matrix->Get(Form("h_%s_%s_Ch%d_S3", genMode, MATRIX_M_,    ich));

    h_data_dR[ich]   = (TH1 *)f_input->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_DR_,   ich));
    h_data_M[ich]    = (TH1 *)f_input->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_M_,    ich)); 
    
    h_gen_dR[ich]   = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_S3", genMode, GEN_ADD_DR_,   ich));
    h_gen_M[ich]    = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_S3", genMode, GEN_ADD_M_,    ich));

    h_gen_nosel_dR[ich]   = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_nosel", genMode, GEN_ADD_DR_,   ich ));
    h_gen_nosel_M[ich]    = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_nosel", genMode, GEN_ADD_M_,    ich ));
  }

  std::map<const char *, TH2 *> m_sys_dR[nChannel], m_sys_M[nChannel];
  for(int ich=0; ich<nChannel; ++ich){
    TH1 *EventInfo = (TH1 *)f_matrix->Get("EventInfo");
    double scale = LUMINOSITY_*31.06/EventInfo->GetBinContent(2);
    //double scale = SF_ttbb*LUMINOSITY_*XSEC_[TTBB_]/EventInfo->GetBinContent(2);
    h_resp_dR[ich]->Scale(scale);   h_resp_M[ich]->Scale(scale);
    
    if(runSystematics){
      for(auto v_itr = syst_list.begin(); v_itr != syst_list.end(); ++v_itr){
	TH2 *tmp_dR, *tmp_M;
	tmp_dR   = (TH2 *)f_matrix->Get(Form("h_%s_%s_Ch%d_S3%s", genMode, MATRIX_DR_,   ich, (*v_itr).c_str()));
	tmp_M    = (TH2 *)f_matrix->Get(Form("h_%s_%s_Ch%d_S3%s", genMode, MATRIX_M_,    ich, (*v_itr).c_str()));
	tmp_dR->Scale(scale);   tmp_M->Scale(scale);
	m_sys_dR[ich].insert(pair<const char *, TH2 *>((*v_itr).c_str(), tmp_dR));
	m_sys_M[ich].insert(pair<const char *, TH2 *>((*v_itr).c_str(), tmp_M));
      }
    }
  }
  std::map<const char *, TH1 *> m_bkg_dR[nChannel], m_bkg_M[nChannel];
  std::map<const char *, double> m_scale;
  std::map<const char *, std::map<const char *, TH1 *>> m_bkgsys_dR[nChannel], m_bkgsys_M[nChannel];
  std::vector<TFile *>v_samples;
  if( (pos = input.find("Data")) != std::string::npos ){
    for(int i=0; i<static_cast<int>(Sample_List_::LAST); ++i){
      TFile *f_sample = TFile::Open(Form("%s/hist_%s.root", input_dir.c_str(), NAME_[i].c_str()));
      TH1 *EventInfo = (TH1 *)f_sample->Get("EventInfo");
      double scale = LUMINOSITY_*XSEC_[i]/EventInfo->GetBinContent(2);
      if     (i == TTBB_) scale *= SF16_ttbb;
      else if(i == TTBJ_) scale *= SF16_ttbj;
      else if(i == TTCC_) scale *= SF16_ttcc;
      else if(i == TTLF_) scale *= SF16_ttLF;
      else                scale *= 1;
      m_scale.insert(pair<const char *, double>(NAME_[i].c_str(), scale));
      v_samples.push_back(f_sample);
    }
    for(int i=0; i<static_cast<int>(Sample_List_::LAST); ++i){
      for(int ich=0; ich<nChannel; ++ich){
	if(i == TTBB_) continue;
	TH1 *h_bkg_dR, *h_bkg_M;
	h_bkg_dR   = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_DR_,   ich));
        h_bkg_M    = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_M_,    ich));
	m_bkg_dR[ich].insert(pair<const char *, TH1 *>(NAME_[i].c_str(), h_bkg_dR));
	m_bkg_M[ich].insert(pair<const char *, TH1 *>(NAME_[i].c_str(), h_bkg_M));
      }
    }
    if(runSystematics){
      std::map<const char *, TH1 *> m_tmp_dR, m_tmp_M;
      for(auto v_itr = syst_list.begin(); v_itr != syst_list.end(); ++v_itr){
	for(int ich=0; ich<nChannel; ++ich){
	  for(int i=0; i<static_cast<int>(Sample_List_::LAST); ++i){
	    if(i == TTBB_) continue;

	    TH1 *tmp_dR, *tmp_M, *tmp_dEta, *tmp_dPhi;
	    tmp_dR   = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3%s", genMode, RECO_ADD_DR_,   ich, (*v_itr).c_str()));
	    tmp_M    = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3%s", genMode, RECO_ADD_M_,    ich, (*v_itr).c_str()));

	    m_tmp_dR.insert(std::pair<const char *, TH1 *>(NAME_[i].c_str(), tmp_dR));
	    m_tmp_M.insert(std::pair<const char *, TH1 *>(NAME_[i].c_str(), tmp_M));
	  }
	  m_bkgsys_dR[ich].insert(pair<const char *,   std::map<const char *, TH1 *>>((*v_itr).c_str(), m_tmp_dR));
	  m_bkgsys_M[ich].insert(pair<const char *,    std::map<const char *, TH1 *>>((*v_itr).c_str(), m_tmp_M));

	  m_tmp_dR.clear();
	  m_tmp_M.clear();
	}	
      }
      for(auto v_itr = syst_ttbar.begin(); v_itr != syst_ttbar.end(); ++v_itr){
        for(int ich=0; ich<nChannel; ++ich){
	  for(int i=0; i<static_cast<int>(Sample_List_::LAST); ++i){
	    if(i == TTBB_) continue;
	    
	    TH1 *tmp_dR, *tmp_M, *tmp_dEta, *tmp_dPhi;
	    
	    if(i <= TTOTHER_){
	      tmp_dR   = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3%s", genMode, RECO_ADD_DR_,   ich, (*v_itr).c_str()));
	      tmp_M    = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3%s", genMode, RECO_ADD_M_,    ich, (*v_itr).c_str()));
	    }
	    else{
	      tmp_dR   = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_DR_,   ich));
	      tmp_M    = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_M_,    ich));
	    }
	    m_tmp_dR.insert(std::pair<const char *, TH1 *>(NAME_[i].c_str(), tmp_dR));
	    m_tmp_M.insert(std::pair<const char *, TH1 *>(NAME_[i].c_str(), tmp_M));
	  }
	  m_bkgsys_dR[ich].insert(pair<const char *,   std::map<const char *, TH1 *>>((*v_itr).c_str(), m_tmp_dR));
	  m_bkgsys_M[ich].insert(pair<const char *,    std::map<const char *, TH1 *>>((*v_itr).c_str(), m_tmp_M));

	  m_tmp_dR.clear();
	  m_tmp_M.clear();
	}
      }
    }
  }
  else{
    TH1 *EventInfo = (TH1 *)f_ttbb->Get("EventInfo");
    double scale = SF16_ttbb*LUMINOSITY_*XSEC_[TTBB_]/EventInfo->GetBinContent(2);
    for(int ich=0; ich < nChannel; ++ich){
      h_data_dR[ich]->Scale(scale);   h_data_M[ich]->Scale(scale);
    }
  }

  TH1 *EventInfo = (TH1 *)f_ttbb->Get("EventInfo");
  double scale = SF16_ttbb*LUMINOSITY_*XSEC_[TTBB_]/EventInfo->GetBinContent(2);
  EventInfo->~TH1();
  for(int ich=0; ich < nChannel; ++ich){
    h_gen_dR[ich]->Scale(scale);         h_gen_M[ich]->Scale(scale);
    h_gen_nosel_dR[ich]->Scale(scale);   h_gen_nosel_M[ich]->Scale(scale);
  }
 
  f_out->cd();
  cout << "Start unfolding..." << endl;

  TH1 *h_unfolded_dR_lep, *h_unfolded_M_lep;

  for(int ich=0; ich<nChannel; ++ich){
    cout << "Channel: " << ich << endl;
    std::vector<TH1 *> v_unfolded_dR, v_unfolded_M;
    std::map<const char *, TH1 *> m_unbkgsys_dR, m_unbkgsys_M;
    if(useTUnfold){
      v_unfolded_dR = runTUnfold(
          input.c_str(), h_data_dR[ich], h_resp_dR[ich], 
          m_bkg_dR[ich], m_scale, m_sys_dR[ich],
          scanLcurve, taumin_dR, taumax_dR, fixtau_dR, fixedtau_dR);
      v_unfolded_M = runTUnfold(
          input.c_str(), h_data_M[ich], h_resp_M[ich],
          m_bkg_M[ich], m_scale, m_sys_M[ich],
          scanLcurve, taumin_M, taumax_M, fixtau_M, fixedtau_M);

      if(runSystematics){
        std::vector<TH1 *> v_tmp_dR, v_tmp_M;
        for(auto m_itr = m_bkgsys_dR[ich].begin(); m_itr != m_bkgsys_dR[ich].end(); ++m_itr){
	  v_tmp_dR = runTUnfold(
              input.c_str(), h_data_dR[ich], h_resp_dR[ich], 
              m_itr->second, m_scale, m_sys_dR[ich],
              scanLcurve, taumin_dR, taumax_dR, fixtau_dR, fixedtau_dR);
	  m_unbkgsys_dR.insert(std::pair<const char *, TH1 *>(m_itr->first, v_tmp_dR[0]));
	}
        for(auto m_itr = m_bkgsys_M[ich].begin(); m_itr != m_bkgsys_M[ich].end(); ++m_itr){
	  v_tmp_M = runTUnfold(
              input.c_str(), h_data_M[ich], h_resp_M[ich],
              m_itr->second, m_scale, m_sys_M[ich],
              scanLcurve, taumin_M, taumax_M, fixtau_M, fixedtau_M);
	  m_unbkgsys_M.insert(std::pair<const char *, TH1 *>(m_itr->first ,v_tmp_M[0]));
	}
      }
    }
    /*else{
      if(findBestK){
	cout << "Find Best k" << endl;
	vector<TH1 *> v_bestk_dR, v_bestk_M, v_bestk_dEta, v_bestk_dPhi;
	for(int i=2; i < nbins_gen_addbjets_dR; ++i){
	  vector<TH1 *> v_tmp_dR = runSVDUnfold(
	      h_data_dR[ich], h_resp_dR[ich],
	      std::map<const char *, TH1 *>(), std::map<const char *, double>(), 
	      std::map<const char *, TH2 *>(), i
	      );
	  v_bestk_dR.push_back(v_tmp_dR[0]);
	}
	for(int i=2; i < nbins_gen_addbjets_M; ++i){
	  vector<TH1 *> v_tmp_M = runSVDUnfold(
	      h_data_M[ich], h_resp_M[ich],
	      std::map<const char *, TH1 *>(), std::map<const char *, double>(),
	      std::map<const char *, TH2 *>(), i
	      );
	  v_bestk_M.push_back(v_tmp_M[0]);
	}	
	for(int i=2; i < nbins_gen_addbjets_dEta; ++i){
	  vector<TH1 *> v_tmp_dEta = runSVDUnfold(
	      h_data_dEta[ich], h_resp_dEta[ich],
	      std::map<const char *, TH1 *>(), std::map<const char *, double>(), 
	      std::map<const char *, TH2 *>(), i
	      );
	  v_bestk_dEta.push_back(v_tmp_dEta[0]);
	}
	for(int i=2; i < nbins_gen_addbjets_dPhi; ++i){
	  vector<TH1 *> v_tmp_dPhi = runSVDUnfold(
	      h_data_dPhi[ich], h_resp_dPhi[ich],
	      std::map<const char *, TH1 *>(), std::map<const char *, double>(), 
	      std::map<const char *, TH2 *>(), i
	      );
	  v_bestk_dPhi.push_back(v_tmp_dPhi[0]);
	}
	findBestk(input.c_str(), v_unfolded_dR,   h_gen_dR[ich]);
	findBestk(input.c_str(), v_unfolded_M,    h_gen_M[ich]);
	findBestk(input.c_str(), v_unfolded_dEta, h_gen_dEta[ich]);
	findBestk(input.c_str(), v_unfolded_dPhi, h_gen_dPhi[ich]);
      }
      v_unfolded_dR = runSVDUnfold(
          h_data_dR[ich], h_resp_dR[ich],
          m_bkg_dR[ich], m_scale, m_sys_dR[ich], reg_dR);
      v_unfolded_M = runSVDUnfold(
          h_data_M[ich], h_resp_M[ich],
          m_bkg_M[ich], m_scale, m_sys_M[ich], reg_M);
      v_unfolded_dEta = runSVDUnfold(
          h_data_dEta[ich], h_resp_dEta[ich],
          m_bkg_dEta[ich], m_scale, m_sys_dEta[ich], reg_dEta);
      v_unfolded_dPhi = runSVDUnfold(
          h_data_dPhi[ich], h_resp_dPhi[ich],
          m_bkg_dPhi[ich], m_scale, m_sys_dPhi[ich], reg_dPhi);
    }*/

    TH1 *h_unfolded_dR, *h_unfolded_M;
    h_unfolded_dR   = v_unfolded_dR[0];
    h_unfolded_M    = v_unfolded_M[0];
    
    if(ich == 0){
      h_unfolded_dR_lep = (TH1 *)h_unfolded_dR->Clone();
      h_unfolded_M_lep  = (TH1 *)h_unfolded_M->Clone();
    }
    else if(ich == 1){
      h_unfolded_dR_lep->Add(h_unfolded_dR);
      h_unfolded_M_lep->Add(h_unfolded_M);
    }

    TH1 *h_acc_dR   = (TH1 *)f_criteria->Get(Form("h_%s_Ch%d", ACC_DR_,   ich));
    TH1 *h_acc_M    = (TH1 *)f_criteria->Get(Form("h_%s_Ch%d", ACC_M_,    ich));

    TH1 *h_MC_diffXsec_nosel_dR   = calculateDiffXsec(h_gen_nosel_dR[2],   NULL, true);
    TH1 *h_MC_diffXsec_nosel_M    = calculateDiffXsec(h_gen_nosel_M[2],    NULL, true);
    TH1 *h_diffXsec_dR    = calculateDiffXsec(h_unfolded_dR,    h_acc_dR);
    TH1 *h_diffXsec_M     = calculateDiffXsec(h_unfolded_M,     h_acc_M);

    std::vector<TH1 *>v_dR_sys, v_M_sys;
    if( runSystematics ){
      // Acceptance uncertainties
      for(auto v_itr=syst_total.begin(); v_itr != syst_total.end(); ++v_itr){
	std::cout << *v_itr << std::endl;
        TH1 *h_dR1 = (TH1 *)f_criteria->Get(Form("h_%s_Ch%d%s", ACC_DR_, ich, (*v_itr).c_str()));
	TH1 *h_dR2 = (TH1 *)calculateDiffXsec(h_unfolded_dR, h_dR1);
	h_dR2->SetName(Form("%s_Acceptance%s", h_dR2->GetName(), (*v_itr).c_str()));
        v_dR_sys.push_back(h_dR2);

        TH1 *h_M1 = (TH1 *)f_criteria->Get(Form("h_%s_Ch%d%s", ACC_M_, ich, (*v_itr).c_str()));
	TH1 *h_M2 = (TH1 *)calculateDiffXsec(h_unfolded_M, h_M1);
	h_M2->SetName(Form("%s_Acceptance%s", h_M2->GetName(), (*v_itr).c_str()));
        v_M_sys.push_back(h_M2);
      }

      // Matrix uncertainties
      for(int i=4; i<v_unfolded_dR.size(); ++i){
        TH1 *h_dR   = calculateDiffXsec(v_unfolded_dR.at(i),   h_acc_dR);
        TH1 *h_M    = calculateDiffXsec(v_unfolded_M.at(i),    h_acc_M);
	h_dR->SetTitle("");
	h_M->SetTitle("");
	v_dR_sys.push_back(h_dR);
	v_M_sys.push_back(h_M);
      }

      // Background uncertainties
      for(auto m_itr = m_unbkgsys_dR.begin(); m_itr != m_unbkgsys_dR.end(); ++m_itr){
        TH1 *h_dR   = (TH1 *)calculateDiffXsec(m_unbkgsys_dR[m_itr->first], h_acc_dR);
	h_dR->SetName(Form("%s_Background%s", h_dR->GetName(), m_itr->first)); 
        TH1 *h_M    = (TH1 *)calculateDiffXsec(m_unbkgsys_M[m_itr->first], h_acc_M); 
	h_M->SetName(Form("%s_Background%s", h_M->GetName(), m_itr->first)); 
	v_dR_sys.push_back(h_dR);
	v_M_sys.push_back(h_M);
      }
    }

    //Save histograms
    std::cout << "SAVE THE HISTOGRAMS" << std::endl;
    h_diffXsec_dR->Write();   h_diffXsec_M->Write();
    for(auto v_itr=v_dR_sys.begin();  v_itr != v_dR_sys.end();  ++v_itr) (*v_itr)->Write();
    for(auto v_itr=v_M_sys.begin();   v_itr != v_M_sys.end();   ++v_itr) (*v_itr)->Write();

    //Draw histogram to validation
    //1. Unfolded reco distribution vs. Gen distribution
    //2. Data diffXsec vs. Gen diffXsec
    drawHist(input, h_unfolded_dR,   h_gen_dR[ich]);
    drawHist(input, h_unfolded_M,    h_gen_M[ich]);
    drawHist(input, h_diffXsec_dR,   h_MC_diffXsec_nosel_dR);
    drawHist(input, h_diffXsec_M,    h_MC_diffXsec_nosel_M);
  }
  std::cout << "Finish unfolding" << endl;
  
  TCanvas *c = new TCanvas("","",800,800);
  TLegend *leg = new TLegend(0.6,0.7,0.88,0.88);
  leg->SetTextSize(0.02);

  TPaveText *label_cms = tdrCMSlabel();
  TPaveText *label_sim = tdrCMSSimlabel();

  TH1 *h_gen_dR_lep, *h_gen_M_lep;
  h_gen_dR_lep = (TH1 *)h_gen_dR[0]->Clone();
  h_gen_dR_lep->Add(h_gen_dR[1]);
  h_gen_M_lep = (TH1 *)h_gen_M[0]->Clone();
  h_gen_M_lep->Add(h_gen_M[1]);

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
  h_unfolded_dR_lep->Draw("p e same");
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
  h_unfolded_M_lep->Draw("p e same");
  label_sim->Draw("same");
  leg->Draw("same");
   
  if(useTUnfold) 
    c->Print(Form("%s/%s_h_output_gen_M_lep_tau%.3fto%.3f.pdf", output_dir.c_str(), input.c_str(), taumin_M, taumax_M), "pdf");
  else
    c->Print(Form("%s/%s_h_output_gen_M_lep_k%d.pdf", output_dir.c_str(), input.c_str(), reg_M), "pdf");
  leg->Clear();
  c->Clear();
 
  //f_out->Write();
  f_out->Close();
}
