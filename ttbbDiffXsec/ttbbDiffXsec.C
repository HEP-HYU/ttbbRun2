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
#include "preProcess.C"
//#include "runSVDUnfold.C"

void ttbbDiffXsec(std::string year, const char * matrix_name, bool isData, bool runSystematics, bool useTUnfold, bool fixtau){
  std::cout << "Start ttbbDiffXsec" << std::endl;
  size_t pos;
  //gErrorIgnoreLevel = kFatal; // kWarning
  gROOT->ProcessLine("setTDRStyle();");

  std::string input_mc_dir    = "../output/root"   + year;
  std::string input_data_dir  = "../output/root"   + year;
  std::string output_dir      = "../output/unfold/" + year;
  const char *outname;
  if(isData){
    input_data_dir = "../output/postfit/" + year;
    outname = "data";
    preProcess(year);
  }
  else outname = "ttbb";
 
  if(year == "19") year = "18";

  double lumi = 1.0;
  lumi = m_lumi[year];
  std::string ttbb = "TTLJ_PowhegPythia_ttbb";

  std::string test = "";
  if(year == "16") test = ttbb;
  else test = "ResponseMatrix_ttbb";

  std::cout << "Load sample informations..." << std::endl;

  std::vector<std::string> syst_matrix = syst_ttbar;
  std::vector<std::string> syst_bkg = syst_basic;
  std::vector<std::string> syst_accpt = syst_total;
  //syst_accpt.insert(syst_accpt.end(), syst_basic.begin(), syst_basic.end());
  if( year == "16" || year == "2016" ){
    syst_accpt.push_back("__musfup"); syst_accpt.push_back("__musfdown");
    syst_accpt.push_back("__elsfup"); syst_accpt.push_back("__elsfdown");
  }
  else{
    syst_accpt.push_back("__muidup");   syst_accpt.push_back("__muiddown");
    syst_accpt.push_back("__muisoup");  syst_accpt.push_back("__muisodown");
    syst_accpt.push_back("__elidup");   syst_accpt.push_back("__eliddown");
    syst_accpt.push_back("__elrecoup"); syst_accpt.push_back("__elrecodown");
    syst_accpt.push_back("__elzvtxup"); syst_accpt.push_back("__elzvtxdown");
  }

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
  std::map<std::string, double> m_tau;
  while(getline(tau, tmp_str)){
    std::stringstream ss;
    ss.str(tmp_str);
    ss >> tmp_name;
    ss >> tmp_value;
    m_tau.insert(pair<std::string, double>(tmp_name, tmp_value));
  }
  
  double fixedtau_dR = m_tau["fixedtau_dR"];
  double taumin_dR = m_tau["taumin_dR"];
  double taumax_dR = m_tau["taumax_dR"];

  double fixedtau_M = m_tau["fixedtau_M"];
  double taumin_M = m_tau["taumin_M"];
  double taumax_M = m_tau["taumax_M"];

  std::cout << "Load Files..." << std::endl;

  TFile *f_out = TFile::Open(Form("%s/hist_unfolded_%s.root", output_dir.c_str(), outname),"recreate");
  TFile *f_input;
  TFile *f_matrix = TFile::Open(Form("%s/hist_%s.root", input_mc_dir.c_str(), matrix_name));
  TFile *f_accept = TFile::Open(Form("%s/hist_accept_%s.root", input_mc_dir.c_str(), genMode));
  TFile *f_ratio = TFile::Open(Form("../output/postfit/%s/hist_postfit.root", year.c_str()));
  if(isData){
    f_input = TFile::Open(Form("%s/hist_postfit.root", input_data_dir.c_str()));
  }
  else{
    f_input = TFile::Open(Form("%s/hist_%s.root", input_mc_dir.c_str(), test.c_str()));
  }
  TFile *f_ttbb = TFile::Open(Form("%s/hist_%s.root", input_mc_dir.c_str(), ttbb.c_str()));
  
  std::cout << "Input file: " << f_input << " " << f_input->GetName() << std::endl;
  std::cout << "Acceptance file: " << f_accept << " " << f_accept->GetName() << std::endl;
  std::cout << "Matrix file: " << f_matrix << " " << f_matrix->GetName() << std::endl;
  std::cout << "ttbb file: " << f_ttbb << " " << f_ttbb->GetName() << std::endl;
  std::cout << "Load histograms..." << std::endl;
  
  TH2 *h_resp_dR[nChannel], *h_resp_M[nChannel];
  TH1 *h_data_dR[nChannel], *h_data_M[nChannel];
  TH1 *h_reco_dR[nChannel], *h_reco_M[nChannel];
  TH1 *h_gen_dR[nChannel],  *h_gen_M[nChannel];
  TH1 *h_gen_nosel_dR[nChannel], *h_gen_nosel_M[nChannel];
  
  TH2 *h_resp_dR_copy[nChannel], *h_resp_M_copy[nChannel];
  TH1 *h_reco_ratio_dR[nChannel], *h_reco_ratio_M[nChannel];
  TH1 *h_gen_ratio_dR[nChannel], *h_gen_ratio_M[nChannel];

  for(int ich=0; ich<nChannel; ++ich){

    h_resp_dR[ich]  = (TH2 *)f_matrix->Get(Form("h_%s_%s_Ch%d_S3", genMode, MATRIX_DR_, ich));
    h_resp_M[ich]   = (TH2 *)f_matrix->Get(Form("h_%s_%s_Ch%d_S3", genMode, MATRIX_M_,  ich));
    h_resp_dR_copy[ich] = (TH2 *)h_resp_dR[ich]->Clone();
    h_resp_dR_copy[ich]->SetName(Form("h_%s_%s_Ch%d_S3_copy", genMode, MATRIX_DR_, ich));
    h_resp_M_copy[ich]  = (TH2 *)h_resp_M[ich]->Clone();
    h_resp_M_copy[ich]->SetName(Form("h_%s_%s_Ch%d_S3_copy", genMode, MATRIX_M_, ich));
   
    h_reco_dR[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_DR_, ich));
    h_reco_M[ich]  = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_M_,  ich));
    if(isData){
      h_data_dR[ich] = (TH1 *)f_input->Get(Form("h_%s_%s_Ch%d_S3_data_obs", genMode, RECO_ADD_DR_, ich));
      h_data_M[ich]  = (TH1 *)f_input->Get(Form("h_%s_%s_Ch%d_S3_data_obs", genMode, RECO_ADD_M_,  ich)); 

      h_gen_dR[ich]   = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_S3", genMode, GEN_ADD_DR_, ich));
      h_gen_M[ich]    = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_S3", genMode, GEN_ADD_M_,  ich));

      h_reco_ratio_dR[ich] = (TH1 *)f_input->Get(Form("h_%s_%s_Ch%d_S3_ratio", genMode, RECO_ADD_DR_, ich));
      h_reco_ratio_M[ich]  = (TH1 *)f_input->Get(Form("h_%s_%s_Ch%d_S3_ratio", genMode, RECO_ADD_M_,  ich));
    }
    else{
      h_data_dR[ich] = (TH1 *)f_input->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_DR_, ich));
      h_data_M[ich]  = (TH1 *)f_input->Get(Form("h_%s_%s_Ch%d_S3", genMode, RECO_ADD_M_,  ich)); 

      h_reco_ratio_dR[ich] = (TH1 *)f_ratio->Get(Form("h_%s_%s_Ch%d_S3_ratio", genMode, RECO_ADD_DR_, ich));
      h_reco_ratio_M[ich]  = (TH1 *)f_ratio->Get(Form("h_%s_%s_Ch%d_S3_ratio", genMode, RECO_ADD_M_,  ich));

      if( year == "16" ){
        h_gen_dR[ich]   = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_S3", genMode, GEN_ADD_DR_,   ich));
        h_gen_M[ich]    = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_S3", genMode, GEN_ADD_M_,    ich));
      }
      else{
        h_gen_dR[ich]   = (TH1 *)f_matrix->Get(Form("h_%s_%s_Ch%d_S3", genMode, GEN_ADD_DR_,   ich));
        h_gen_M[ich]    = (TH1 *)f_matrix->Get(Form("h_%s_%s_Ch%d_S3", genMode, GEN_ADD_M_,    ich));
      }
    }
    h_gen_nosel_dR[ich]   = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_nosel", genMode, GEN_ADD_DR_,   ich ));
    h_gen_nosel_M[ich]    = (TH1 *)f_ttbb->Get(Form("h_%s_%s_Ch%d_nosel", genMode, GEN_ADD_M_,    ich ));
  }

  std::cout << "Save histograms in std::map..." << std::endl;
  
  std::map<const char *, TH2 *> m_sys_dR[nChannel], m_sys_M[nChannel];
  for(int ich=0; ich<nChannel; ++ich){
    if( isData and ich != 2) continue;
    TH1 *EventInfo = (TH1 *)f_matrix->Get("EventInfo");
    //double scale = m_sf[matrix_name]*lumi*m_xsec[matrix_name]/EventInfo->GetBinContent(2);
    //h_resp_dR[ich]->Scale(scale);   h_resp_M[ich]->Scale(scale);
    
    // Apply fitting results
    //if( isData ){
    if( ich == 2){
      for( int iybin = 1; iybin <= h_resp_dR[ich]->GetNbinsY(); ++iybin ){
        for( int ixbin = 1; ixbin <= h_resp_dR[ich]->GetNbinsX(); ++ixbin ){
          auto tmp = h_resp_dR[ich]->GetBinContent(ixbin, iybin);
          tmp *= h_reco_ratio_dR[ich]->GetBinContent(ixbin);
          h_resp_dR[ich]->SetBinContent(ixbin, iybin, tmp);
        }
      }
      for( int iybin = 1; iybin <= h_resp_M[ich]->GetNbinsY(); ++iybin ){
        for( int ixbin = 1; ixbin <= h_resp_M[ich]->GetNbinsX(); ++ixbin ){
          auto tmp = h_resp_M[ich]->GetBinContent(ixbin, iybin);
          tmp *= h_reco_ratio_M[ich]->GetBinContent(ixbin);
          h_resp_M[ich]->SetBinContent(ixbin, iybin, tmp);
        }
      }
    }
    //}
    
    f_out->cd();
    TH1 *tmp_projY_dR  = (TH1 *)h_resp_dR_copy[ich]->ProjectionY();
    TH1 *tmp_projY_M   = (TH1 *)h_resp_M_copy[ich]->ProjectionY();
    TH1 *tmp_projY_dR2 = (TH1 *)h_resp_dR[ich]->ProjectionY();
    TH1 *tmp_projY_M2  = (TH1 *)h_resp_M[ich]->ProjectionY();
    h_gen_ratio_dR[ich] = (TH1 *)tmp_projY_dR2->Clone();
    h_gen_ratio_dR[ich]->Divide(tmp_projY_dR);
    h_gen_ratio_dR[ich]->SetName(Form("h_%s_%s_Ch%d_S3_ratio", genMode, GEN_ADD_DR_, ich));
    h_gen_ratio_M[ich] = (TH1 *)tmp_projY_M2->Clone();
    h_gen_ratio_M[ich]->Divide(tmp_projY_M);
    h_gen_ratio_M[ich]->SetName(Form("h_%s_%s_Ch%d_S3_ratio", genMode, GEN_ADD_M_, ich));
    
    if( !isData and runSystematics ){
      for(auto v_itr = syst_matrix.begin(); v_itr != syst_matrix.end(); ++v_itr){
        TH2 *tmp_dR, *tmp_M;
        tmp_dR   = (TH2 *)f_matrix->Get(Form("h_%s_%s_Ch%d_S3%s", genMode, MATRIX_DR_, ich, (*v_itr).c_str()));
        tmp_M    = (TH2 *)f_matrix->Get(Form("h_%s_%s_Ch%d_S3%s", genMode, MATRIX_M_,  ich, (*v_itr).c_str()));
        //tmp_dR->Scale(scale);   tmp_M->Scale(scale);
        m_sys_dR[ich].insert(pair<const char *, TH2 *>((*v_itr).c_str(), tmp_dR));
        m_sys_M[ich].insert(pair<const char *, TH2 *>((*v_itr).c_str(), tmp_M));
      }
    }
  }

  std::map<const char *, TH1 *> m_bkg_dR[nChannel], m_bkg_M[nChannel];
  std::map<const char *, double> m_scale;
  std::vector<const char *> v_bkg = { "ttbj", "ttcc", "ttLF", "ttbkg", "other" };
  if( isData ){ 
    for( auto v_itr = v_bkg.begin(); v_itr != v_bkg.end(); ++v_itr ){
      m_scale.insert(pair<const char *, double>(*v_itr, 1));
      for( int ich=0; ich < nChannel; ++ich){
        if( ich != 2 ) continue;
        auto tmp_dR = (TH1 *)f_input->Get(Form("h_%s_%s_Ch%d_S3_%s", genMode, RECO_ADD_DR_, ich, *v_itr));
        m_bkg_dR[ich].insert(pair<const char *, TH1 *>(*v_itr, tmp_dR));
        auto tmp_M = (TH1 *)f_input->Get(Form("h_%s_%s_Ch%d_S3_%s", genMode, RECO_ADD_M_, ich, *v_itr));
        m_bkg_M[ich].insert(pair<const char *, TH1 *>(*v_itr, tmp_M));
      }
    }
  }
  else{
    std::cout << "Normalize input MC" << std::endl;
    double scale = lumi*m_xsec[ttbb]/m_genevt[ttbb];
    for(int ich=0; ich < nChannel; ++ich){
      h_data_dR[ich]->Scale(scale);   h_data_M[ich]->Scale(scale);
      if( ich == 2){
        h_data_dR[ich]->Multiply(h_reco_ratio_dR[ich]);
        h_data_M[ich]->Multiply(h_reco_ratio_M[ich]);
      }
    }
  }
  // Background systematic unceratinty will be updated...
  std::cout << "Normalize ttbb distributions" << std::endl;
  double scale = lumi*m_xsec[ttbb]/m_genevt[ttbb];
  for(int ich=0; ich < nChannel; ++ich){
    h_reco_dR[ich]->Scale(scale);      h_reco_M[ich]->Scale(scale);
    h_gen_dR[ich]->Scale(scale);       h_gen_M[ich]->Scale(scale);
    h_gen_nosel_dR[ich]->Scale(scale); h_gen_nosel_M[ich]->Scale(scale);
  }
 
  f_out->cd();
  cout << "Start unfolding..." << endl;

  for(int ich=0; ich<nChannel; ++ich){
    if( isData and ich != 2) continue;
    cout << "Channel: " << ich << endl;
    
    std::string input;
    if(isData) input = "Data";
    else input = "ttbb";
    std::vector<TH1 *> v_unfolded_dR, v_unfolded_M;
    std::vector<TH1 *> v_test_dR, v_test_M;
    std::map<const char *, TH1 *> m_unbkgsys_dR, m_unbkgsys_M;
    if( useTUnfold ){
      v_unfolded_dR = runTUnfold(
          output_dir, input.c_str(), h_data_dR[ich], h_resp_dR[ich], 
          m_bkg_dR[ich], m_scale, m_sys_dR[ich],
          scanLcurve, taumin_dR, taumax_dR, fixtau, fixedtau_dR);
      v_unfolded_M = runTUnfold(
          output_dir, input.c_str(), h_data_M[ich], h_resp_M[ich],
          m_bkg_M[ich], m_scale, m_sys_M[ich],
          scanLcurve, taumin_M, taumax_M, fixtau, fixedtau_M);
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

    cout << "Save unfolded histograms" << endl;
    
    TH1 *h_unfolded_dR, *h_unfolded_M;
    h_unfolded_dR = v_unfolded_dR[0];
    h_unfolded_M  = v_unfolded_M[0];
    h_unfolded_dR->Divide(h_gen_ratio_dR[ich]);
    h_unfolded_M->Divide(h_gen_ratio_M[ich]);
    
    TH1 *h_input_dR, *h_input_M;
    h_input_dR = v_unfolded_dR[1];
    h_input_M  = v_unfolded_M[1];

    TH1 *h_acc_dR = (TH1 *)f_accept->Get(Form("h_%s_Ch%d", ACC_DR_, ich));
    TH1 *h_acc_M  = (TH1 *)f_accept->Get(Form("h_%s_Ch%d", ACC_M_,  ich));

    TH1 *h_MC_diffXsec_nosel_dR = calculateDiffXsec(year, h_gen_nosel_dR[2], NULL, true);
    TH1 *h_MC_diffXsec_nosel_M  = calculateDiffXsec(year ,h_gen_nosel_M[2],  NULL, true);
    TH1 *h_diffXsec_dR = calculateDiffXsec(year, h_unfolded_dR, h_acc_dR);
    TH1 *h_diffXsec_M  = calculateDiffXsec(year, h_unfolded_M,  h_acc_M);
    TH1 *h_diffXsec_from_gen_dR = calculateDiffXsec(year, h_gen_dR[ich], h_acc_dR);
    TH1 *h_diffXsec_from_gen_M  = calculateDiffXsec(year, h_gen_M[ich],  h_acc_M);

    std::vector<TH1 *>v_dR_sys, v_M_sys;
    if( runSystematics ){
      // Acceptance uncertainties
      for(auto v_itr=syst_accpt.begin(); v_itr != syst_accpt.end(); ++v_itr){
        if ( *v_itr == "" ) continue;
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
      for( unsigned int i=5; i < v_unfolded_dR.size(); ++i ){
        TH1 *h_dR = calculateDiffXsec(year, v_unfolded_dR.at(i), h_acc_dR);
        TH1 *h_M  = calculateDiffXsec(year, v_unfolded_M.at(i),  h_acc_M);
        h_dR->SetTitle("");
        h_M->SetTitle("");
        v_dR_sys.push_back(h_dR);
        v_M_sys.push_back(h_M);
      }
    }

    //Save histograms
    std::cout << "SAVE THE HISTOGRAMS" << std::endl;
    if( isData ){
      h_gen_ratio_dR[ich]->Write(); h_gen_ratio_M[ich]->Write();
      h_reco_ratio_dR[ich]->Write(); h_reco_ratio_M[ich]->Write();
    }
    h_acc_dR->Write(); h_acc_M->Write();
    h_unfolded_dR->Write(); h_unfolded_M->Write();
    h_input_dR->Write(); h_input_M->Write();
    h_resp_dR[ich]->Write(); h_resp_M[ich]->Write();
    h_resp_dR_copy[ich]->Write(); h_resp_M_copy[ich]->Write();
    h_reco_dR[ich]->Write(); h_reco_M[ich]->Write();
    h_gen_dR[ich]->Write(); h_gen_M[ich]->Write();
    h_diffXsec_dR->Write(); h_diffXsec_M->Write();
    h_diffXsec_from_gen_dR->Write(); h_diffXsec_from_gen_M->Write();
    h_MC_diffXsec_nosel_dR->Write(); h_MC_diffXsec_nosel_M->Write();
    v_unfolded_dR[2]->Write(); v_unfolded_M[2]->Write();
    v_unfolded_dR[3]->Write(); v_unfolded_M[3]->Write();
    v_unfolded_dR[4]->Write(); v_unfolded_M[4]->Write();
    //v_unfolded_dR[5]->Write(); v_unfolded_M[5]->Write();
    for(auto v_itr=v_dR_sys.begin();  v_itr != v_dR_sys.end();  ++v_itr) (*v_itr)->Write();
    for(auto v_itr=v_M_sys.begin();   v_itr != v_M_sys.end();   ++v_itr) (*v_itr)->Write();
  }
  
  std::cout << "Finish unfolding" << endl;
  f_out->Close();
}
