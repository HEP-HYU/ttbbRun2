#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <TLegend.h>
#include <TAttFill.h>
#include <TPaveText.h>
#include <TROOT.h>

#include "ttbbDiffXsec.h"
#include "runTUnfold.C"
//#include "runSVDUnfold.C"

void ttbbDiffXsec(std::string year, const char * matrix_name, bool isData, bool runSystematics, bool useTUnfold
    bool fixtau=false){
  ssize_t pos;
  //gErrorIgnoreLevel = kFatal; // kWarning
  gROOT->ProcessLine("setTDRStyle();");

  double lumi = 1.0;
  lumi = m_lumi[year];
  std::string ttbb = "TTLJ_PowhegPythia_ttbb";

  std::string test = "";
  if(year == "16") test = "TTLJ_PowhegPythia_ttbb";
  else test = "ttbbClosureTest";

  const char *outname;
  if(isData) outname = "data";
  else outname = "ttbb";
  std::string input_dir  = "../output/root"   + year;
  std::string output_dir = "../output/unfold/" + year;
 
  std::string tmp_str;
  std::string tmp_name; double tmp_value;
  
  ifstream sample(Form("../samples/sample%s.txt", year.c_str()));
  std::vector<std::string> v_sample_name;
  while(getline(sample, tmp_str)){
    if( (pos = tmp_str.find("ntuple",0)) != std::string::npos ) continue;
    v_sample_name.push_back(tmp_str);
  }
  sample.close();
  
  ifstream xsec(Form("../samples/xsec%s.txt", year.c_str()));
  std::map<std::string, double> m_xsec;
  while(getline(xsec, tmp_str)){
    if( (pos = tmp_str.find("Sample",0)) != std::string::npos ) continue;
    std::stringstream ss;
    ss.str(tmp_str);
    ss >> tmp_name;
    ss >> tmp_value;
    m_xsec.insert(pair<std::string, double>(tmp_name, tmp_value));
  }
  xsec.close();

  ifstream genevt(Form("../samples/genevt%s.txt", year.c_str()));
  std::map<std::string, double> m_genevt;
  while(getline(genevt, tmp_str)){
    std::stringstream ss;
    ss.str(tmp_str);
    ss >> tmp_name;
    ss >> tmp_value;
    m_genevt.insert(pair<std::string, double>(tmp_name, tmp_value));
  }
  genevt.close();

  ifstream sf(Form("../samples/scale%s.txt", year.c_str()));
  std::map<std::string, double> m_sf;
  while(getline(sf, tmp_str)){
    std::stringstream ss;
    ss.str(tmp_str);
    ss >> tmp_name;
    ss >> tmp_value;
    if(year == "16") tmp_value = 1;
    m_sf.insert(pair<std::string, double>(tmp_name, tmp_value));
  }
  sf.close();

  ifstream tau(Form("tau/tau%s.txt", year.c_str()));
  std::map<std::string, double> v_tau;
  while(getline(tau, tmp_str)){
    std::stringstream ss;
    ss.str(tmp_str);
    ss >> tmp_name;
    ss >> tmp_value;
    v_tau.insert(pair<std::string, double>((tmp_name, tmp_value));
  }
  double fixedtau_dR = v_tau["fixedtau_dR"];
  double taumin_dR = v_tau["taumin_dR"];
  double taumax_dR = v_tau["taumax_dR"];

  double fixedtau_M = v_tau["fixedtau_M"];
  double taumin_M = v_tau["taumin_M"];
  double taumax_dR = v_tau["taumax_M"];

  std::vector<std::string> syst_total = syst_list;
  syst_total.insert(syst_total.end(), syst_ttbar.begin(), syst_ttbar.end());
  std::cout << "Load Files..." << std::endl;
  TFile *f_out = TFile::Open(Form("%s/hist_unfolded_%s.root", output_dir.c_str(), outname),"recreate");

  TFile *f_input[2];
  TFile *f_matrix = TFile::Open(Form("%s/hist_%s.root", input_dir.c_str(), matrix_name));
  TFile *f_accept = TFile::Open(Form("%s/hist_accept_mindR.root", input_dir.c_str()));
  if(isData){
    f_input[0] = TFile::Open(Form("%s/hist_DataSingleMu.root", input_dir.c_str()));
    f_input[1] = TFile::Open(Form("%s/hist_DataSingleEG.root", input_dir.c_str()));
  }
  else{
    f_input[0] = TFile::Open(Form("%s/hist_%s.root", input_dir.c_str(), test.c_str()));
    f_input[1] = TFile::Open(Form("%s/hist_%s.root", input_dir.c_str(), test.c_str()));
  }
  TFile *f_ttbb = TFile::Open(Form("%s/hist_%s.root", input_dir.c_str(), ttbb.c_str()));

  TH2 *h_resp_dR[nChannel], *h_resp_M[nChannel];
  TH1 *h_data_dR[nChannel], *h_data_M[nChannel];
  TH1 *h_gen_dR[nChannel],  *h_gen_M[nChannel];
  TH1 *h_gen_nosel_dR[nChannel],   *h_gen_nosel_M[nChannel]; 

  for(int ich=0; ich<nChannel; ++ich){
    h_resp_dR[ich]   = (TH2 *)f_matrix->Get(Form("h_%s_%s_Ch%d_S3", genMode, MATRIX_DR_,   ich));
    h_resp_M[ich]    = (TH2 *)f_matrix->Get(Form("h_%s_%s_Ch%d_S3", genMode, MATRIX_M_,    ich));
   
    h_gen_dR[ich]   = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_S3", genMode, GEN_ADD_DR_,   ich));
    h_gen_M[ich]    = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_S3", genMode, GEN_ADD_M_,    ich));

    h_gen_nosel_dR[ich]   = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_nosel", genMode, GEN_ADD_DR_,   ich ));
    h_gen_nosel_M[ich]    = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_nosel", genMode, GEN_ADD_M_,    ich ));
  }
    
  h_data_dR[0]   = (TH1 *)f_input[0]->Get(Form("h_%s_%s_Ch0_S3", genMode, RECO_ADD_DR_));
  h_data_M[0]    = (TH1 *)f_input[0]->Get(Form("h_%s_%s_Ch0_S3", genMode, RECO_ADD_M_ )); 
  h_data_dR[1]   = (TH1 *)f_input[1]->Get(Form("h_%s_%s_Ch1_S3", genMode, RECO_ADD_DR_));
  h_data_M[1]    = (TH1 *)f_input[1]->Get(Form("h_%s_%s_Ch1_S3", genMode, RECO_ADD_M_ )); 
  if(isData){
    TH1 *tmp_hist = (TH1 *)f_input[1]->Get(Form("h_%s_%s_Ch2_S3", genMode, RECO_ADD_DR_));
    h_data_dR[2] = (TH1 *)f_input[0]->Get(Form("h_%s_%s_Ch2_S3", genMode, RECO_ADD_DR_));
    h_data_dR[2]->Add(tmp_hist);
    tmp_hist = (TH1 *)f_input[1]->Get(Form("h_%s_%s_Ch2_S3", genMode, RECO_ADD_M_));
    h_data_M[2] = (TH1 *)f_input[0]->Get(Form("h_%s_%s_Ch2_S3", genMode, RECO_ADD_M_));
    h_data_M[2]->Add(tmp_hist);
  }
  else{
    h_data_dR[2] = (TH1 *)f_input[0]->Get(Form("h_%s_%s_Ch2_S3", genMode, RECO_ADD_DR_));
    h_data_M[2] = (TH1 *)f_input[0]->Get(Form("h_%s_%s_Ch2_S3", genMode, RECO_ADD_M_));
  }

  std::cout << "Load Matrix..." << std::endl;
  std::map<const char *, TH2 *> m_sys_dR[nChannel], m_sys_M[nChannel];
  for(int ich=0; ich<nChannel; ++ich){
    TH1 *EventInfo = (TH1 *)f_matrix->Get("EventInfo");
    double scale = m_sf[matrix_name]*lumi*m_xsec[matrix_name]/EventInfo->GetBinContent(2);
   // h_resp_dR[ich]->Scale(scale);   h_resp_M[ich]->Scale(scale);
    
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
  std::cout << "Save Backgrounds..." << std::endl;
  std::map<const char *, TH1 *> m_bkg_dR[nChannel], m_bkg_M[nChannel];
  std::map<const char *, double> m_scale;
  std::map<const char *, std::map<const char *, TH1 *>> m_bkgsys_dR[nChannel], m_bkgsys_M[nChannel];
  std::vector<TFile *>v_samples;
  if(isData){
    for(int i=0; i < v_sample_name.size(); ++i){
      std::string sample_name = v_sample_name.at(i);
      TFile *f_sample = TFile::Open(Form("%s/hist_%s.root", input_dir.c_str(), sample_name.c_str()));
      double scale = m_sf[sample_name]*lumi*m_xsec[sample_name]/m_genevt[sample_name];
      m_scale.insert(pair<const char *, double>(sample_name.c_str(), scale));
      v_samples.push_back(f_sample);
    }
    for(int i=0; i<v_sample_name.size(); ++i){
      for(int ich=0; ich<nChannel; ++ich){
        std::string sample_name = v_sample_name.at(i);
	if((pos = sample_name.find("ttbb")) != std::string::npos) continue;
	TH1 *h_bkg_dR, *h_bkg_M;
	h_bkg_dR   = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_DR_,   ich));
        h_bkg_M    = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_M_,    ich));
	m_bkg_dR[ich].insert(pair<const char *, TH1 *>(sample_name.c_str(), h_bkg_dR));
	m_bkg_M[ich].insert(pair<const char *, TH1 *>(sample_name.c_str(), h_bkg_M));
      }
    }
    std::cout << "Saved background samples" << std::endl;
    for(auto m_itr = m_bkg_dR[0].begin(); m_itr != m_bkg_dR[0].end(); ++m_itr){
      std::cout << m_itr->first << ": " << m_itr->second << ": " << m_scale[m_itr->first] << std::endl;
    }
    std::cout << "Save Systematic samples" << std::endl;
    if(runSystematics){
      std::map<const char *, TH1 *> m_tmp_dR, m_tmp_M;
      for(auto v_itr = syst_list.begin(); v_itr != syst_list.end(); ++v_itr){
	for(int ich=0; ich<nChannel; ++ich){
	  for(int i=0; i<v_sample_name.size(); ++i){
            std::string sample_name = v_sample_name.at(i);
	    if((pos = sample_name.find("ttbb")) != std::string::npos) continue;

	    TH1 *tmp_dR, *tmp_M, *tmp_dEta, *tmp_dPhi;
	    tmp_dR   = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3%s", genMode, RECO_ADD_DR_,   ich, (*v_itr).c_str()));
	    tmp_M    = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3%s", genMode, RECO_ADD_M_,    ich, (*v_itr).c_str()));

	    m_tmp_dR.insert(std::pair<const char *, TH1 *>(sample_name.c_str(), tmp_dR));
	    m_tmp_M.insert(std::pair<const char *, TH1 *>(sample_name.c_str(), tmp_M));
	  }
	  m_bkgsys_dR[ich].insert(pair<const char *,   std::map<const char *, TH1 *>>((*v_itr).c_str(), m_tmp_dR));
	  m_bkgsys_M[ich].insert(pair<const char *,    std::map<const char *, TH1 *>>((*v_itr).c_str(), m_tmp_M));

	  m_tmp_dR.clear();
	  m_tmp_M.clear();
	}	
      }
      for(auto v_itr = syst_ttbar.begin(); v_itr != syst_ttbar.end(); ++v_itr){
        for(int ich=0; ich<nChannel; ++ich){
	  for(int i=0; i<v_sample_name.size(); ++i){
            std::string sample_name = v_sample_name.at(i);
	    if((pos = sample_name.find("ttbb")) != std::string::npos) continue;
	    
	    TH1 *tmp_dR, *tmp_M, *tmp_dEta, *tmp_dPhi;
	    
	    if((pos = sample_name.find("PowhegPythia_tt")) != std::string::npos){
	      tmp_dR   = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3%s", genMode, RECO_ADD_DR_,   ich, (*v_itr).c_str()));
	      tmp_M    = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3%s", genMode, RECO_ADD_M_,    ich, (*v_itr).c_str()));
	    }
	    else{
	      tmp_dR   = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_DR_,   ich));
	      tmp_M    = (TH1 *)v_samples[i]->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_M_,    ich));
	    }
	    m_tmp_dR.insert(std::pair<const char *, TH1 *>(sample_name.c_str(), tmp_dR));
	    m_tmp_M.insert(std::pair<const char *, TH1 *>(sample_name.c_str(), tmp_M));
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
    std::cout << "Normalize ttbb MC" << std::endl;
    double scale = m_sf[ttbb]*lumi*m_xsec[ttbb]/m_genevt[ttbb];
    if( year != 16 ) scale *= 2;
    for(int ich=0; ich < nChannel; ++ich){
      h_data_dR[ich]->Scale(scale);   h_data_M[ich]->Scale(scale);
    }
  }

  std::cout << "Normalize gen distributions" << std::endl;
  double scale = m_sf[ttbb]*lumi*m_xsec[ttbb]/m_genevt[ttbb];
  for(int ich=0; ich < nChannel; ++ich){
    h_gen_dR[ich]->Scale(scale);         h_gen_M[ich]->Scale(scale);
    h_gen_nosel_dR[ich]->Scale(scale);   h_gen_nosel_M[ich]->Scale(scale);
  }
 
  f_out->cd();
  cout << "Start unfolding..." << endl;

  for(int ich=0; ich<nChannel; ++ich){
    cout << "Channel: " << ich << endl;
    std::string input;
    if(isData) input = "Data";
    else input = "ttbb";
    std::vector<TH1 *> v_unfolded_dR, v_unfolded_M;
    std::map<const char *, TH1 *> m_unbkgsys_dR, m_unbkgsys_M;
    if(useTUnfold){
      v_unfolded_dR = runTUnfold(
          input.c_str(), h_data_dR[ich], h_resp_dR[ich], 
          m_bkg_dR[ich], m_scale, m_sys_dR[ich],
          scanLcurve, taumin_dR, taumax_dR, fixtau, fixedtau_dR);
      v_unfolded_M = runTUnfold(
          input.c_str(), h_data_M[ich], h_resp_M[ich],
          m_bkg_M[ich], m_scale, m_sys_M[ich],
          scanLcurve, taumin_M, taumax_M, fixtau, fixedtau_M);

      if(runSystematics){
        std::vector<TH1 *> v_tmp_dR, v_tmp_M;
        for(auto m_itr = m_bkgsys_dR[ich].begin(); m_itr != m_bkgsys_dR[ich].end(); ++m_itr){
	  v_tmp_dR = runTUnfold(
              input.c_str(), h_data_dR[ich], h_resp_dR[ich], 
              m_itr->second, m_scale, m_sys_dR[ich],
              scanLcurve, taumin_dR, taumax_dR, fixtau, fixedtau_dR);
	  m_unbkgsys_dR.insert(std::pair<const char *, TH1 *>(m_itr->first, v_tmp_dR[0]));
	}
        for(auto m_itr = m_bkgsys_M[ich].begin(); m_itr != m_bkgsys_M[ich].end(); ++m_itr){
	  v_tmp_M = runTUnfold(
              input.c_str(), h_data_M[ich], h_resp_M[ich],
              m_itr->second, m_scale, m_sys_M[ich],
              scanLcurve, taumin_M, taumax_M, fixtau, fixedtau_M);
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
    TH1 *h_input_dR, *h_input_M;
    h_input_dR = v_unfolded_dR[1];
    h_input_M  = v_unfolded_M[1];

    TH1 *h_acc_dR   = (TH1 *)f_accept->Get(Form("h_%s_Ch%d", ACC_DR_,   ich));
    TH1 *h_acc_M    = (TH1 *)f_accept->Get(Form("h_%s_Ch%d", ACC_M_,    ich));

    TH1 *h_MC_diffXsec_nosel_dR   = calculateDiffXsec(year, h_gen_nosel_dR[2],   NULL, true);
    TH1 *h_MC_diffXsec_nosel_M    = calculateDiffXsec(year ,h_gen_nosel_M[2],    NULL, true);
    TH1 *h_diffXsec_dR    = calculateDiffXsec(year, h_unfolded_dR,    h_acc_dR);
    TH1 *h_diffXsec_M     = calculateDiffXsec(year, h_unfolded_M,     h_acc_M);

    std::vector<TH1 *>v_dR_sys, v_M_sys;
    if( runSystematics ){
      // Acceptance uncertainties
      for(auto v_itr=syst_total.begin(); v_itr != syst_total.end(); ++v_itr){
	std::cout << *v_itr << std::endl;
        TH1 *h_dR1 = (TH1 *)f_accept->Get(Form("h_%s_Ch%d%s", ACC_DR_, ich, (*v_itr).c_str()));
	TH1 *h_dR2 = (TH1 *)calculateDiffXsec(year, h_unfolded_dR, h_dR1);
	h_dR2->SetName(Form("%s_Acceptance%s", h_dR2->GetName(), (*v_itr).c_str()));
        v_dR_sys.push_back(h_dR2);

        TH1 *h_M1 = (TH1 *)f_accept->Get(Form("h_%s_Ch%d%s", ACC_M_, ich, (*v_itr).c_str()));
	TH1 *h_M2 = (TH1 *)calculateDiffXsec(year, h_unfolded_M, h_M1);
	h_M2->SetName(Form("%s_Acceptance%s", h_M2->GetName(), (*v_itr).c_str()));
        v_M_sys.push_back(h_M2);
      }

      // Matrix uncertainties
      for(int i=5; i<v_unfolded_dR.size(); ++i){
        TH1 *h_dR   = calculateDiffXsec(year, v_unfolded_dR.at(i),   h_acc_dR);
        TH1 *h_M    = calculateDiffXsec(year, v_unfolded_M.at(i),    h_acc_M);
	h_dR->SetTitle("");
	h_M->SetTitle("");
	v_dR_sys.push_back(h_dR);
	v_M_sys.push_back(h_M);
      }

      // Background uncertainties
      for(auto m_itr = m_unbkgsys_dR.begin(); m_itr != m_unbkgsys_dR.end(); ++m_itr){
        TH1 *h_dR   = (TH1 *)calculateDiffXsec(year, m_unbkgsys_dR[m_itr->first], h_acc_dR);
	h_dR->SetName(Form("%s_Background%s", h_dR->GetName(), m_itr->first)); 
        TH1 *h_M    = (TH1 *)calculateDiffXsec(year, m_unbkgsys_M[m_itr->first], h_acc_M); 
	h_M->SetName(Form("%s_Background%s", h_M->GetName(), m_itr->first)); 
	v_dR_sys.push_back(h_dR);
	v_M_sys.push_back(h_M);
      }
    }

    //Save histograms
    std::cout << "SAVE THE HISTOGRAMS" << std::endl;
    h_unfolded_dR->Write(); h_unfolded_M->Write();
    h_input_dR->Write(); h_input_M->Write();
    h_resp_dR[ich]->Write(); h_resp_M[ich]->Write();
    h_gen_dR[ich]->Write(); h_gen_M[ich]->Write();
    h_diffXsec_dR->Write(); h_diffXsec_M->Write();
    h_MC_diffXsec_nosel_dR->Write(); h_MC_diffXsec_nosel_M->Write();
    for(auto v_itr=v_dR_sys.begin();  v_itr != v_dR_sys.end();  ++v_itr) (*v_itr)->Write();
    for(auto v_itr=v_M_sys.begin();   v_itr != v_M_sys.end();   ++v_itr) (*v_itr)->Write();
  }
  
  std::cout << "Finish unfolding" << endl;
  f_out->Close();
}
