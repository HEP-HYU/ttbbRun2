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

#include <TLegend.h>
#include <TAttFill.h>

#include "ttbbDiffXsec.h"
#include "runTUnfold.C"
#include "runSVDUnfold.C"

//Unfolding options
const bool useTUnfold = false;
const bool scanLcurve = true;
const bool fixtau_dR = false;
const float fixedtau_dR = 0.0001;//log_{10}(#tau)
const double taumin_dR = 0., taumax_dR = 0.;
const bool fixtau_M = false;
const float fixedtau_M = 0.0;
const double taumin_M = 0.8, taumax_M = 1.3;
const bool findBestK = false;
const int reg_dR = 3, reg_M = 3;

const double SF_ttbb = 0.992;
const double SF_ttbj = 0.997;
const double SF_ttcc = 0.871;
const double SF_ttLF = 0.890;

void ttbbDiffXsec(string input_dir, string output_dir, string input, string syst = ""){
  gErrorIgnoreLevel = kFatal; // kWarning
  TFile *f_respMatrix = TFile::Open(Form("../output/post/hist_ttbbFilter_ttbb.root"));
  TFile *f_criteria = TFile::Open(Form("../output/post/hist_criteria.root"));
  TFile *f_input = TFile::Open(Form("%s/%s.root", input_dir.c_str(), input.c_str()));
  TFile *f_ttbb = TFile::Open(Form("%s/hist_ttbb.root", input_dir.c_str()));
  TH2 *h_resp_dR[nChannel], *h_resp_M[nChannel];
  TH1 *h_data_dR[nChannel], *h_data_M[nChannel];
  TH1 *h_gen_dR[nChannel], *h_gen_M[nChannel];
  TH1 *h_gen_nosel_dR[nChannel], *h_gen_nosel_M[nChannel];

  for(int ich=0; ich<nChannel; ++ich){
    h_resp_dR[ich] = (TH2 *)f_respMatrix->Get(Form("h_ResponseMatrixDeltaR_Ch%d_S3%s", ich, syst.c_str()));
    h_resp_M[ich] = (TH2 *)f_respMatrix->Get(Form("h_ResponseMatrixInvMass_Ch%d_S3%s", ich, syst.c_str()));
    if(input.find("Data")){
      h_data_dR[ich] = (TH1 *)f_input->Get(Form("h_%s_Ch%d_S3",RECO_ADDJETS_DELTAR_, ich));
      h_data_M[ich] = (TH1 *)f_input->Get(Form("h_%s_Ch%d_S3",RECO_ADDJETS_INVARIANT_MASS_, ich));
    }   
    else{
      h_data_dR[ich] = (TH1 *)f_input->Get(Form("h_%s_Ch%d_S3%s",RECO_ADDJETS_DELTAR_, ich, syst.c_str()));
      h_data_M[ich] = (TH1 *)f_input->Get(Form("h_%s_Ch%d_S3%s",RECO_ADDJETS_INVARIANT_MASS_, ich, syst.c_str()));
    }
    h_gen_dR[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_S3%s",GEN_ADDBJETS_DELTAR_, ich, syst.c_str()));
    h_gen_M[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_S3%s",GEN_ADDBJETS_INVARIANT_MASS_, ich, syst.c_str()));
    h_gen_nosel_dR[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_nosel",GEN_ADDBJETS_DELTAR_, ich ));
    h_gen_nosel_M[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_nosel",GEN_ADDBJETS_INVARIANT_MASS_, ich ));
  }

  for(int ich=0; ich<nChannel; ++ich){
//TH1 *EventInfo = (TH1 *)f_respMatrix->Get("EventInfo");
//    double scale = LUMINOSITY_*31.06/EventInfo->GetBinContent(2);
    double scale = 1.0;
    h_resp_dR[ich]->Scale(scale); h_resp_M[ich]->Scale(scale);
  }

  ssize_t pos;
  bool ps = false;
  bool sw = false;
  if((pos = syst.find("ps")) != std::string::npos) ps = true;
  if((pos = syst.find("sw")) != std::string::npos) sw = true;

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

      for(int ich=0; ich<nChannel; ++ich){
	if(i == TTBB_) continue;
	TH1 *h_bkg_dR, *h_bkg_M;
	h_bkg_dR = (TH1 *)f_sample->Get(Form("h_%s_Ch%d_S3",RECO_ADDJETS_DELTAR_,ich));
        h_bkg_M = (TH1 *)f_sample->Get(Form("h_%s_Ch%d_S3",RECO_ADDJETS_INVARIANT_MASS_,ich));
	h_bkg_dR->Scale(scale); h_bkg_M->Scale(scale);
	h_data_dR[ich]->Add(h_bkg_dR, -1); h_data_M[ich]->Add(h_bkg_M, -1);
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

  std::string method = "UUU";
  //if(useTUnfold){
  //  if(scanLcurve) method = "TUnfold_curve";
  //  else method = "TUnfold_mgc";
  //}
  //else{
    //method = "SVDUnfold_dR"+std::to_string(reg_dR)+"_M"+std::to_string(reg_M);
  //}
  TFile *f_out = TFile::Open(Form("%s/hist_unfolded_%s%s.root", output_dir.c_str(), input.c_str(), syst.c_str()),"recreate");
  f_out->cd();
  cout << "Start unfolding..." << endl;
  for(int ich=0; ich<nChannel; ++ich){
    TH1 *h_unfolded_dR, *h_unfolded_M;
    if(!useTUnfold){
      //runSVDUnfold(TH1 *h_in, TH2 *h_response_matrix, int reg);
      if(findBestK){
	cout << "Find Best k" << endl;
	vector<TH1 *> v_unfolded_dR, v_unfolded_M;
	for(int i=2; i<10; ++i){
	  TH1 *h_tmp_dR = runSVDUnfold(h_data_dR[ich], h_resp_dR[ich], i);
	  //h_tmpMC_dR = calculateDiffXsec(h_tmpMC_dR, 0, ich, false);
	  //h_tmpData_dR = calculateDiffXsec(h_tmpData_dR, 0, ich, true);
	  v_unfolded_dR.push_back(h_tmp_dR);
	}
	for(int i=2; i<10; ++i){
	  TH1 *h_tmp_M = runSVDUnfold(h_data_M[ich], h_resp_M[ich], i);
	  //h_tmpMC_M = calculateDiffXsec(h_tmpMC_M, 1, ich, false);
	  //h_tmpData_M = calculateDiffXsec(h_tmpData_M, 1, ich, true);
	  v_unfolded_M.push_back(h_tmp_M);
	}
        TH1 *h_truth_dR = (TH1 *)h_gen_dR[ich]->Clone();//calculateDiffXsec(h_gen_nosel_dR_lep);
        TH1 *h_truth_M = (TH1 *)h_gen_M[ich]->Clone();//calculateDiffXsec(h_gen_nosel_M_lep);
	findBestk(v_unfolded_dR, 1, 0, ich, h_truth_dR);
	findBestk(v_unfolded_M, 1, 1, ich, h_truth_M);
      }
      h_unfolded_dR = runSVDUnfold(h_data_dR[ich], h_resp_dR[ich], reg_dR);
      h_unfolded_M = runSVDUnfold(h_data_M[ich], h_resp_M[ich], reg_M);
    }
    else{
      h_unfolded_dR = runTUnfold(h_data_dR[ich], h_resp_dR[ich], scanLcurve, taumin_dR, taumax_dR, fixtau_dR, fixedtau_dR);
      h_unfolded_M = runTUnfold(h_data_M[ich], h_resp_M[ich], scanLcurve, taumin_M, taumax_M, fixtau_M, fixedtau_M);
    }  
 
    TH1 *h_acc_dR = (TH1 *)f_criteria->Get(Form("h_BinAcceptanceDeltaR_Ch%d", ich));
    TH1 *h_acc_M  = (TH1 *)f_criteria->Get(Form("h_BinAcceptanceInvMass_Ch%d", ich));
    
    //Closure Test & Data
    TH1 *h_diffXsec_dR = calculateDiffXsec(h_unfolded_dR, h_acc_dR, 0, false);
    TH1 *h_diffXsec_M = calculateDiffXsec(h_unfolded_M, h_acc_M, 1, false);
    h_unfolded_dR->SetName(Form("h_unfolded_dR_Ch%d_S3%s", ich, syst.c_str()));
    h_unfolded_M->SetName(Form("h_unfolded_M_Ch%d_S3%s", ich, syst.c_str()));
    h_diffXsec_dR->SetName(Form("h_diffXsec_dR_Ch%d%s", ich, syst.c_str()));
    h_diffXsec_M->SetName(Form("h_diffXsec_M_Ch%d%s", ich, syst.c_str()));
    h_gen_dR[ich]->Write();
    h_gen_M[ich]->Write();
    h_data_dR[ich]->Write();
    h_data_M[ich]->Write();
    h_unfolded_dR->Write();
    h_unfolded_M->Write();
    h_diffXsec_dR->Write();
    h_diffXsec_M->Write();

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
