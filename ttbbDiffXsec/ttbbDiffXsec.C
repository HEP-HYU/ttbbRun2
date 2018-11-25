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

//Running mode
const bool closuretest = false;
const bool rundata = true;
//const bool systematic = true;
//Unfolding options
const bool useTUnfold = false;
const bool scanbyLcurve = false;
const double taumax = 0.0, taumin = 0.0;
const bool fixtau = false;
const float fixedtau = 0.0;
const int reg_dR = 4, reg_M = 4;

void ttbbDiffXsec(){
  gErrorIgnoreLevel = kFatal; // kWarning

  TFile *f_respMatrix = TFile::Open(Form("%s/hist_respMatrix_ttbb.root", input_dir.c_str()));
  TFile *f_ttbb = TFile::Open(Form("%s/hist_TTLJ_PowhegPythia_ttbb.root", input_dir.c_str()));

  TFile *f_data[2] = {
    TFile::Open(Form("%s/hist_%s.root", input_dir.c_str(), DATA_[MUON_].c_str())),
    TFile::Open(Form("%s/hist_%s.root", input_dir.c_str(), DATA_[ELECTRON_].c_str()))
  };

  TFile *f_sample[static_cast<int>(Sample_List_::LAST)];
  for(int i=0; i<static_cast<int>(Sample_List_::LAST); ++i){
    f_sample[i] = TFile::Open(Form("%s/hist_%s.root", input_dir.c_str(), NAME_[i].c_str()));
  }

  std::vector<std::string> v_generators = {"PowhegHerwig","PowhegPythiaEvtgen"};//,"aMCatNLOPythia"};
  std::vector<std::string> v_sysname = {"FSR","ISR"};
  std::vector<std::string> v_sysvar = {"up","down"};
  const int ngen = v_generators.size();
  const int nsys = v_sysname.size();
  const int nvar = v_sysvar.size();
 
  TFile *f_generators[ngen], *f_sys[nsys][nvar];
  for(auto i = 0; i < ngen; ++i)
    f_generators[i] = TFile::Open(Form("%s/hist_TT_%s_ttbb.root",input_dir.c_str(),v_generators[i].c_str()));
  for(auto i = 0; i < nsys; ++i){
    for(auto j = 0; j < nvar; ++j){
      f_sys[i][j] = TFile::Open(Form("%s/hist_TT_PowhegPythia_SYS_%s%s_ttbb.root",input_dir.c_str(),v_sysname[i].c_str(),v_sysvar[j].c_str()));
    }
  }

  TH2 *h_resp_dR[nChannel], *h_resp_M[nChannel];
  TH1 *h_data_dR[nChannel], *h_data_M[nChannel];
  TH1 *h_reco_dR[nChannel], *h_reco_M[nChannel];
  TH1 *h_gen_dR[nChannel], *h_gen_M[nChannel];
  TH1 *h_gen_nosel_dR[nChannel], *h_gen_nosel_M[nChannel];
  TH1 *h_generators_reco_dR[ngen][nChannel], *h_generators_reco_M[ngen][nChannel];
  TH1 *h_generators_gen_dR[ngen][nChannel], *h_generators_gen_M[ngen][nChannel];
  TH1 *h_generators_gen_nosel_dR[ngen][nChannel], *h_generators_gen_nosel_M[ngen][nChannel];
  TH1 *h_sys_reco_dR[nsys][nvar][nChannel], *h_sys_reco_M[nsys][nvar][nChannel];
  TH1 *h_sys_gen_dR[nsys][nvar][nChannel], *h_sys_gen_M[nsys][nvar][nChannel];
  TH1 *h_sys_gen_nosel_dR[nsys][nvar][nChannel], *h_sys_gen_nosel_M[nsys][nvar][nChannel];

  for(int ich=0; ich<nChannel; ++ich){
    h_resp_dR[ich] = (TH2 *)f_respMatrix->Get(Form(
	  "h_ResponseMatrixDeltaR_Ch%d_S3_TTLJ_PowhegPythia_ttbbFilter_ttbb", ich));
    h_resp_M[ich] = (TH2 *)f_respMatrix->Get(Form(
	  "h_ResponseMatrixInvMass_Ch%d_S3_TTLJ_PowhegPythia_ttbbFilter_ttbb", ich));
    h_data_dR[ich] = (TH1 *)f_data[ich]->Get(Form("h_%s_Ch%d_S3_%s",
	  RECO_ADDJETS_DELTAR_, ich, DATA_[ich].c_str()));
    h_data_M[ich] = (TH1 *)f_data[ich]->Get(Form("h_%s_Ch%d_S3_%s",
	  RECO_ADDJETS_INVARIANT_MASS_, ich, DATA_[ich].c_str()));
    h_reco_dR[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_S3_%s",
	  RECO_ADDJETS_DELTAR_, ich, NAME_[TTBB_].c_str()));
    h_reco_M[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_S3_%s",
	  RECO_ADDJETS_INVARIANT_MASS_, ich, NAME_[TTBB_].c_str()));
    h_gen_dR[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_S3_%s",
	  GEN_ADDBJETS_DELTAR_, ich, NAME_[TTBB_].c_str()));
    h_gen_M[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_S3_%s",
	  GEN_ADDBJETS_INVARIANT_MASS_, ich, NAME_[TTBB_].c_str()));
    h_gen_nosel_dR[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_nosel_%s",
	  GEN_ADDBJETS_DELTAR_, ich, NAME_[TTBB_].c_str()));
    h_gen_nosel_M[ich] = (TH1 *)f_ttbb->Get(Form("h_%s_Ch%d_nosel_%s",
	  GEN_ADDBJETS_INVARIANT_MASS_, ich, NAME_[TTBB_].c_str()));

    for(int igen=0; igen<ngen; ++igen){
      h_generators_reco_dR[igen][ich] = (TH1 *)f_generators[igen]->Get(Form("h_%s_Ch%d_S3_ttbb",
        RECO_ADDJETS_DELTAR_, ich));
      h_generators_reco_M[igen][ich] = (TH1 *)f_generators[igen]->Get(Form("h_%s_Ch%d_S3_ttbb",
	RECO_ADDJETS_INVARIANT_MASS_, ich));
      h_generators_gen_dR[igen][ich] = (TH1 *)f_generators[igen]->Get(Form("h_%s_Ch%d_S3_ttbb",
	GEN_ADDBJETS_DELTAR_, ich));
      h_generators_gen_M[igen][ich] = (TH1 *)f_generators[igen]->Get(Form("h_%s_Ch%d_S3_ttbb",
	GEN_ADDBJETS_INVARIANT_MASS_, ich));
      h_generators_gen_nosel_dR[igen][ich] = (TH1 *)f_generators[igen]->Get(Form(
	"h_%s_Ch%d_nosel_ttbb",GEN_ADDBJETS_DELTAR_, ich));
      h_generators_gen_nosel_M[igen][ich] = (TH1 *)f_generators[igen]->Get(Form(
	"h_%s_Ch%d_nosel_ttbb", GEN_ADDBJETS_INVARIANT_MASS_, ich));
    }
    for(int isys=0; isys<nsys; ++isys){
      for(int ivar=0; ivar<nvar; ++ivar){
        h_sys_reco_dR[isys][ivar][ich] = (TH1 *)f_sys[isys][ivar]->Get(Form(
	  "h_%s_Ch%d_S3_ttbb", RECO_ADDJETS_DELTAR_, ich));
	h_sys_reco_M[isys][ivar][ich] = (TH1 *)f_sys[isys][ivar]->Get(Form(
	  "h_%s_Ch%d_S3_ttbb", RECO_ADDJETS_INVARIANT_MASS_, ich));
        h_sys_gen_dR[isys][ivar][ich] = (TH1 *)f_sys[isys][ivar]->Get(Form(
	  "h_%s_Ch%d_S3_ttbb", GEN_ADDBJETS_DELTAR_, ich));
	h_sys_gen_M[isys][ivar][ich] = (TH1 *)f_sys[isys][ivar]->Get(Form(
	  "h_%s_Ch%d_S3_ttbb", GEN_ADDBJETS_INVARIANT_MASS_, ich));
        h_sys_gen_nosel_dR[isys][ivar][ich] = (TH1 *)f_sys[isys][ivar]->Get(Form(
	  "h_%s_Ch%d_nosel_ttbb", GEN_ADDBJETS_DELTAR_, ich));
	h_sys_gen_nosel_M[isys][ivar][ich] = (TH1 *)f_sys[isys][ivar]->Get(Form(
	  "h_%s_Ch%d_nosel_ttbb", GEN_ADDBJETS_INVARIANT_MASS_, ich));
      }
    }
  }
  for(int ich=0; ich<nChannel; ++ich){
    TH1 *EventInfo = (TH1 *)f_respMatrix->Get("EventInfo");
    double scale = LUMINOSITY_*31.06/EventInfo->GetBinContent(2);
    h_resp_dR[ich]->Scale(scale); h_resp_M[ich]->Scale(scale);
  }

  for(int i=0; i<static_cast<int>(Sample_List_::LAST); ++i){
    for(int ich=0; ich<nChannel; ++ich){
      TH1 *EventInfo = (TH1 *)f_sample[i]->Get("EventInfo");
      double scale = LUMINOSITY_*XSEC_[i]/EventInfo->GetBinContent(2);
      if(i == TTBB_){
	h_reco_dR[ich]->Scale(scale); h_reco_M[ich]->Scale(scale);
	h_gen_nosel_dR[ich]->Scale(scale); h_gen_nosel_M[ich]->Scale(scale);
	h_gen_dR[ich]->Scale(scale); h_gen_M[ich]->Scale(scale);
      }
      else{
	TH1 *h_bkg_dR = (TH1 *)f_sample[i]->Get(Form(
	  "h_%s_Ch%d_S3_%s",RECO_ADDJETS_DELTAR_,ich,NAME_[i].c_str()));
	TH1 *h_bkg_M = (TH1 *)f_sample[i]->Get(Form(
	  "h_%s_Ch%d_S3_%s",RECO_ADDJETS_INVARIANT_MASS_,ich,NAME_[i].c_str()));
	h_bkg_dR->Scale(scale); h_bkg_M->Scale(scale);
	h_data_dR[ich]->Add(h_bkg_dR,-1);h_data_M[ich]->Add(h_bkg_M,-1);
      }
    }
  }

  TH1 *h_gen_nosel_dR_lep = (TH1 *)h_gen_nosel_dR[0]->Clone();
  h_gen_nosel_dR_lep->Add(h_gen_nosel_dR[1]);
  h_gen_nosel_dR_lep->SetName("h_GenbJetDeltaR_lep_nosel_ttbb");
  TH1 *h_gen_nosel_M_lep = (TH1 *)h_gen_nosel_M[0]->Clone();
  h_gen_nosel_M_lep->Add(h_gen_nosel_M[1]);
  h_gen_nosel_M_lep->SetName("h_GenbJetInvMass_lep_nosel_ttbb");

  TH1 *h_generators_gen_nosel_dR_lep[ngen], *h_generators_gen_nosel_M_lep[ngen];
  for(int igen=0; igen<ngen; ++igen){
    TH1 *EventInfo = (TH1 *)f_generators[igen]->Get("EventInfo");
    double scale = LUMINOSITY_*831.76/EventInfo->GetBinContent(2);
    for(int ich=0; ich<nChannel; ++ich){
      h_generators_reco_dR[igen][ich]->Scale(scale);
      h_generators_reco_M[igen][ich]->Scale(scale);
      h_generators_gen_dR[igen][ich]->Scale(scale);
      h_generators_gen_M[igen][ich]->Scale(scale);
      h_generators_gen_nosel_dR[igen][ich]->Scale(scale);
      h_generators_gen_nosel_M[igen][ich]->Scale(scale);
    }
    h_generators_gen_nosel_dR_lep[igen] = (TH1 *)h_generators_gen_nosel_dR[igen][0]->Clone();
    h_generators_gen_nosel_dR_lep[igen]->Add(h_generators_gen_nosel_dR[igen][1]);
    h_generators_gen_nosel_dR_lep[igen]->SetName(Form(
      "h_%s_lep_nosel_%s_ttbb", GEN_ADDBJETS_DELTAR_, v_generators[igen].c_str()));
    h_generators_gen_nosel_M_lep[igen] = (TH1 *)h_generators_gen_nosel_M[igen][0]->Clone();
    h_generators_gen_nosel_M_lep[igen]->Add(h_generators_gen_nosel_dR[igen][1]);
    h_generators_gen_nosel_M_lep[igen]->SetName(Form(
      "h_%s_lep_nosel_%s_ttbb",GEN_ADDBJETS_INVARIANT_MASS_, v_generators[igen].c_str()));
  }

  TH1 *h_sys_gen_nosel_dR_lep[nsys][nvar], *h_sys_gen_nosel_M_lep[nsys][nvar];
  for(int isys = 0; isys < nsys; ++isys){
    for(int ivar = 0; ivar < nvar; ++ivar){
      TH1 *EventInfo = (TH1 *)f_sys[isys][ivar]->Get("EventInfo");
      double scale = LUMINOSITY_*831.76/EventInfo->GetBinContent(2);
      for(int ich = 0; ich < nChannel; ++ich){ 
	h_sys_reco_dR[isys][ivar][ich]->Scale(scale);
	h_sys_reco_M[isys][ivar][ich]->Scale(scale);
	h_sys_gen_dR[isys][ivar][ich]->Scale(scale);
	h_sys_gen_M[isys][ivar][ich]->Scale(scale);
	h_sys_gen_nosel_dR[isys][ivar][ich]->Scale(scale);
	h_sys_gen_nosel_M[isys][ivar][ich]->Scale(scale);
      }
      h_sys_gen_nosel_dR_lep[isys][ivar] = (TH1 *)h_sys_gen_nosel_dR[isys][ivar][0]->Clone();
      h_sys_gen_nosel_dR_lep[isys][ivar]->Add(h_sys_gen_nosel_dR[isys][ivar][1]);
      h_sys_gen_nosel_dR_lep[isys][ivar]->SetName(Form(
        "h_%s_lep_nosel_%s_%s_ttbb", GEN_ADDBJETS_DELTAR_,
	v_sysname[isys].c_str(), v_sysvar[ivar].c_str()));
      h_sys_gen_nosel_M_lep[isys][ivar] = (TH1 *)h_sys_gen_nosel_M[isys][ivar][0]->Clone();
      h_sys_gen_nosel_M_lep[isys][ivar]->Add(h_sys_gen_nosel_M[isys][ivar][1]);
      h_sys_gen_nosel_M_lep[isys][ivar]->SetName(Form(
        "h_%s_lep_nosel_%s_%s_ttbb", GEN_ADDBJETS_INVARIANT_MASS_,
	v_sysname[isys].c_str(), v_sysvar[ivar].c_str()));
    }
  }
  cout << "Start unfolding..." << endl;
  std::string method;
  for(int ich=0; ich<nChannel; ++ich){
    DrawHist(Form("MC_compRecoRespX_Ch%d_dR", ich),
	h_reco_dR[ich], h_resp_dR[ich]->ProjectionX(), kReco, kRespX, true);
    DrawHist(Form("MC_compRecoRespX_Ch%d_M", ich),
	h_reco_M[ich], h_resp_M[ich]->ProjectionX(), kReco, kRespX, false);
    DrawHist(Form("MC_compGenRespY_Ch%d_dR", ich),
	h_gen_dR[ich], h_resp_dR[ich]->ProjectionY(), kPowhegPythia, kRespY, true);
    DrawHist(Form("MC_compGenRespY_Ch%d_M", ich),
	h_gen_M[ich], h_resp_M[ich]->ProjectionY(), kPowhegPythia, kRespY, false);
    DrawHist(Form("Data_compDataRespX_Ch%d_dR", ich),
	h_data_dR[ich], h_resp_dR[ich]->ProjectionX(), kSubDataBkg, kRespX, true);
    DrawHist(Form("Data_compDataRespX_Ch%d_M", ich),
	h_data_M[ich], h_resp_M[ich]->ProjectionX(), kSubDataBkg, kRespX, false);
    DrawHist(Form("Data_compDataMC_Ch%d_dR", ich),
	h_data_dR[ich], h_reco_dR[ich], kSubDataBkg, kReco, true);
    DrawHist(Form("Data_compDataMC_Ch%d_M", ich),
	h_data_M[ich], h_reco_M[ich], kSubDataBkg, kReco, false);

    TH1 *h_unfoldedMC_dR, *h_unfoldedMC_M;
    TH1 *h_unfoldedData_dR, *h_unfoldedData_M;
    TH1 *h_unfoldedMC_gens_dR[ngen], *h_unfoldedMC_gens_M[ngen];
    TH1 *h_unfoldedMC_sys_dR[nsys][nvar], *h_unfoldedMC_sys_M[nsys][nvar];
    if(!useTUnfold){
      //runSVDUnfold(TH1 *h_in, TH2 *h_response_matrix, int reg);
      method = "SVD";
      const bool findBestK = true;
      if(findBestK){
	cout << "Find Best k" << endl;
	vector<TH1 *> v_unfoldedMC_dR, v_unfoldedMC_M, v_unfoldedData_dR, v_unfoldedData_M;
	for(int i=2; i<10; ++i){
	  TH1 *h_tmpMC_dR = runSVDUnfold(h_reco_dR[ich], h_resp_dR[ich], i);
	  //h_tmpMC_dR = calculateDiffXsec(h_tmpMC_dR, 0, ich, false);
	  //h_tmpData_dR = calculateDiffXsec(h_tmpData_dR, 0, ich, true);
	  v_unfoldedMC_dR.push_back(h_tmpMC_dR);
	}
	for(int i=2; i<10; ++i){
	  TH1 *h_tmpMC_M = runSVDUnfold(h_reco_M[ich], h_resp_M[ich], i);
	  //h_tmpMC_M = calculateDiffXsec(h_tmpMC_M, 1, ich, false);
	  //h_tmpData_M = calculateDiffXsec(h_tmpData_M, 1, ich, true);
	  v_unfoldedMC_M.push_back(h_tmpMC_M);
	}
        TH1 *h_truth_dR = (TH1 *)h_gen_dR[ich]->Clone();//calculateDiffXsec(h_gen_nosel_dR_lep);
        TH1 *h_truth_M = (TH1 *)h_gen_M[ich]->Clone();//calculateDiffXsec(h_gen_nosel_M_lep);
	findBestk(v_unfoldedMC_dR, 1, 0, ich, h_truth_dR);
	findBestk(v_unfoldedMC_M, 1, 1, ich, h_truth_M);
      }
    
      h_unfoldedMC_dR = runSVDUnfold(h_reco_dR[ich], h_resp_dR[ich], reg_dR);
      h_unfoldedMC_M = runSVDUnfold(h_reco_M[ich], h_resp_M[ich], reg_M);
      h_unfoldedData_dR = runSVDUnfold(h_data_dR[ich], h_resp_dR[ich], reg_dR);
      h_unfoldedData_M = runSVDUnfold(h_data_M[ich], h_resp_M[ich], reg_M);
      /*
      for(int igen = 0; igen < ngen; ++igen){
	h_unfoldedMC_gens_dR[igen] = runSVDUnfold(
	    h_generators_reco_dR[igen][ich], h_resp_dR[ich], reg_dR);
	h_unfoldedMC_gens_M[igen] = runSVDUnfold(
	    h_generators_reco_M[igen][ich], h_resp_M[ich], reg_M);
      }
      for(int isys = 0; isys < nsys; ++isys){
        for(int ivar = 0; ivar < nvar; ++ivar){
	  h_unfoldedMC_sys_dR[isys][ivar] = runSVDUnfold(
	      h_sys_reco_dR[isys][ivar][ich], h_resp_dR[ich], reg_dR);
	  h_unfoldedMC_sys_M[isys][ivar] = runSVDUnfold(
	      h_sys_reco_M[isys][ivar][ich], h_resp_M[ich], reg_M);
	}
      }
      */
    }
    else{
      //scanbyRho
      //   runTUnfold(TH1 *h_in, TH2 *h_response_matrix, bool fixtau = false, double taumin = 0., double taumax = 0., double fixedtau_);
      //scanbyLcurve
      //   runTUnfold(TH1 *h_in, TH2 *h_response_matrix, double taumin = 0., double taumax = 0.);
      if(!scanbyLcurve){
	method = "TUnfold_scanRho";
	h_unfoldedMC_dR = runTUnfold(h_reco_dR[ich], h_resp_dR[ich],
	    fixtau, taumin, taumax, fixedtau);
	h_unfoldedMC_M = runTUnfold(h_reco_M[ich], h_resp_M[ich],
	    fixtau, taumin, taumax, fixedtau);
      }
      else{
	method = "TUnfold_scanLcurve";
	h_unfoldedMC_dR = runTUnfold(h_reco_dR[ich], h_resp_dR[ich], taumin, taumax);
	h_unfoldedMC_M = runTUnfold(h_reco_M[ich], h_resp_M[ich], taumin, taumax);
      }
    }  
  
    //Closure Test & Data
    DrawHist(Form("Test_%s_unfoldedMC_Ch%d_dR", method.c_str(), ich),
	h_unfoldedMC_dR, h_gen_dR[ich], kReco, kPowhegPythia,
	true, useTUnfold, false, reg_dR, true);
    DrawHist(Form("Test_%s_unfoldedMC_Ch%d_M", method.c_str(), ich),
	h_unfoldedMC_M, h_gen_M[ich], kReco, kPowhegPythia,
	false, useTUnfold, false, reg_M, true);
    DrawHist(Form("%s_unfoldedData_Ch%d_dR", method.c_str(), ich),
	h_unfoldedData_dR, h_gen_dR[ich], kUnfoldedData, kPowhegPythia,
	true, useTUnfold, false, reg_dR, true);
    DrawHist(Form("%s_unfoldedData_Ch%d_M", method.c_str(), ich),
	h_unfoldedData_M, h_gen_M[ich], kUnfoldedData, kPowhegPythia,
	false, useTUnfold, false, reg_M, true);
    
    TH1 *h_MC_diffXsec_dR = calculateDiffXsec(
	h_unfoldedMC_dR, 0, ich, false, false);
    TH1 *h_MC_diffXsec_M = calculateDiffXsec(
	h_unfoldedMC_M, 1, ich, false, false);
    TH1 *h_data_diffXsec_dR = calculateDiffXsec(h_unfoldedData_dR, 0, ich, true);
    TH1 *h_data_diffXsec_M = calculateDiffXsec(h_unfoldedData_M, 1, ich, true);
 
    TH1 *h_MC_diffXsec_nosel_dR = calculateDiffXsec(
	h_gen_nosel_dR_lep, 0, 2, false, true);
    TH1 *h_MC_diffXsec_nosel_M = calculateDiffXsec(
	h_gen_nosel_M_lep, 1, 2, false, true);

    DrawHist(Form("Test_%s_MC_diffXsec_Ch%d_dR", method.c_str(),ich),
	h_MC_diffXsec_dR, h_MC_diffXsec_nosel_dR, kUnfoldedMC, kPowhegPythia,
	true, useTUnfold, true, reg_dR, true);
    DrawHist(Form("Test_%s_MC_diffXsec_Ch%d_M", method.c_str(),ich),
	h_MC_diffXsec_M, h_MC_diffXsec_nosel_M, kUnfoldedMC, kPowhegPythia,
	false, useTUnfold, true, reg_M, true);
    DrawHist(Form("%s_data_diffXsec_Ch%d_dR", method.c_str(), ich),
	h_data_diffXsec_dR, h_MC_diffXsec_nosel_dR, kUnfoldedData, kPowhegPythia,
	true, useTUnfold, true, reg_dR, true);
    DrawHist(Form("%s_data_diffXsec_Ch%d_M", method.c_str(), ich),
	h_data_diffXsec_M, h_MC_diffXsec_nosel_M, kUnfoldedData, kPowhegPythia,
	false, useTUnfold, true, reg_M, true);
   
    /*
    //Systematics
    TH1 *h_MC_diffXsec_gens_dR[ngen], *h_MC_diffXsec_gens_M[ngen];
    TH1 *h_MC_diffXsec_gens_nosel_dR[ngen], *h_MC_diffXsec_gens_nosel_M[ngen];
    for(int igen = 0; igen < ngen; ++igen){
      int tmp;
      if(igen == 0) tmp = kPowhegHerwig;
      else if(igen == 1) tmp = kPowhegPythiaEvtgen;
      else tmp = kaMCatNLOPythia;

      DrawHist(Form("%s_unfoldedMC_Ch%d_dR_%s", method.c_str(), ich, v_generators[igen].c_str()),
	  h_unfoldedMC_gens_dR[igen], h_generators_gen_dR[igen][ich],
	  kUnfoldedMC, tmp,
	  true, useTUnfold, false, reg_dR, true);
      DrawHist(Form("%s_unfoldedMC_Ch%d_M_%s", method.c_str(), ich, v_generators[igen].c_str()),
	  h_unfoldedMC_gens_M[igen], h_generators_gen_M[igen][ich],
	  kUnfoldedMC, tmp,
	  false, useTUnfold, false, reg_M, true);

      h_MC_diffXsec_gens_dR[igen] = calculateDiffXsec(
	  h_unfoldedMC_gens_dR[igen], 0, ich, false, false, false, igen);
      h_MC_diffXsec_gens_M[igen] = calculateDiffXsec(
	  h_unfoldedMC_gens_M[igen], 1, ich, false, false, false, igen);
      h_MC_diffXsec_gens_nosel_dR[igen] = calculateDiffXsec(
	  h_generators_gen_nosel_dR_lep[igen], 0, 2, false, true, false, igen);
      h_MC_diffXsec_gens_nosel_M[igen] = calculateDiffXsec(
	  h_generators_gen_nosel_M_lep[igen], 1, 2, false, true, false, igen);

      DrawHist(Form("%s_MC_diffXsec_Ch%d_dR_%s", method.c_str(), ich, v_generators[igen].c_str()),
	  h_MC_diffXsec_gens_dR[igen], h_MC_diffXsec_gens_nosel_dR[igen],
	  kUnfoldedMC, tmp,
	  true, useTUnfold, true, reg_dR, true);
      DrawHist(Form("%s_MC_diffXsec_Ch%d_M_%s", method.c_str(), ich, v_generators[igen].c_str()),
	  h_MC_diffXsec_gens_M[igen], h_MC_diffXsec_gens_nosel_M[igen],
	  kUnfoldedMC, tmp,
	  false, useTUnfold, true, reg_M, true);
    }

    TH1 *h_MC_diffXsec_sys_dR[nsys][nvar], *h_MC_diffXsec_sys_M[nsys][nvar];
    TH1 *h_MC_diffXsec_sys_nosel_dR[nsys][nvar], *h_MC_diffXsec_sys_nosel_M[nsys][nvar];
    for(int isys = 0; isys < nsys; ++isys){
      for(int ivar = 0; ivar < nvar; ++ivar){
	DrawHist(Form("%s_unfoldedMC_Ch%d_dR_%s_%s", method.c_str(), ich,
	      v_sysname[isys].c_str(), v_sysvar[ivar].c_str()),
	    h_unfoldedMC_sys_dR[isys][ivar], h_sys_gen_dR[isys][ivar][ich],
	    kUnfoldedMC, kPowhegPythia,
	    true, useTUnfold, false, reg_dR, true);
	DrawHist(Form("%s_unfoldedMC_Ch%d_M_%s_%s", method.c_str(), ich,
	      v_sysname[isys].c_str(), v_sysvar[ivar].c_str()),
	    h_unfoldedMC_sys_M[isys][ivar], h_sys_gen_M[isys][ivar][ich],
	    kUnfoldedMC, kPowhegPythia,
	    false, useTUnfold, false, reg_M, true);

	h_MC_diffXsec_sys_dR[isys][ivar] = calculateDiffXsec(
	    h_unfoldedMC_sys_dR[isys][ivar], 0, ich, false, false, false, -1, isys, ivar);
	h_MC_diffXsec_sys_M[isys][ivar] = calculateDiffXsec(
	    h_unfoldedMC_sys_M[isys][ivar], 1, ich, false, false, false, -1, isys, ivar);
	h_MC_diffXsec_sys_nosel_dR[isys][ivar] = calculateDiffXsec(
	    h_sys_gen_nosel_dR_lep[isys][ivar], 0, 2, false, true, false, -1, isys, ivar);
	h_MC_diffXsec_sys_nosel_M[isys][ivar] = calculateDiffXsec(
	    h_sys_gen_nosel_M_lep[isys][ivar], 1, 2, false, true, false, -1, isys, ivar);

	DrawHist(Form("%s_MC_diffXsec_Ch%d_dR_%s_%s", method.c_str(), ich,
	      v_sysname[isys].c_str(), v_sysvar[ivar].c_str()),
	    h_MC_diffXsec_sys_dR[isys][ivar], h_MC_diffXsec_sys_nosel_dR[isys][ivar],
	    kUnfoldedMC, kPowhegPythia,
	    true, useTUnfold, true, reg_dR, true);
	DrawHist(Form("%s_MC_diffXsec_Ch%d_M_%s_%s", method.c_str(), ich,
	      v_sysname[isys].c_str(), v_sysvar[ivar].c_str()),
	    h_MC_diffXsec_sys_M[isys][ivar], h_MC_diffXsec_sys_nosel_M[isys][ivar],
	    kUnfoldedMC, kPowhegPythia,
	    false, useTUnfold, true, reg_M, true);
      }
    }
    
    //Calculate Systematic unceretainties
    std::vector<std::vector<double>> v_differ_generators_dR, v_differ_generators_M;
    std::vector<std::vector<double>> v_differ_sys_dR, v_differ_sys_M;
    for(int igen = 0; igen < ngen; ++igen){
      v_differ_generators_dR.push_back(
	  calculateDifference(h_MC_diffXsec_gens_dR[igen], h_MC_diffXsec_gens_nosel_dR[igen]));
      v_differ_generators_M.push_back(
	  calculateDifference(h_MC_diffXsec_gens_M[igen], h_MC_diffXsec_gens_nosel_M[igen]));
    }
    for(int isys = 0; isys < nsys; ++isys){
      for(int ivar =0; ivar < nvar; ++ivar){
        v_differ_sys_dR.push_back(
	    calculateDifference(h_MC_diffXsec_sys_dR[isys][ivar], h_MC_diffXsec_sys_nosel_dR[isys][ivar]));
	v_differ_sys_M.push_back(
	    calculateDifference(h_MC_diffXsec_sys_M[isys][ivar], h_MC_diffXsec_sys_nosel_M[isys][ivar]));
      }
    }
    
    auto grp_error_dR = CalculateUncertainty(
	v_differ_sys_dR, v_differ_generators_dR, h_data_diffXsec_dR);
    auto grp_error_M = CalculateUncertainty(
	v_differ_sys_M, v_differ_generators_M, h_data_diffXsec_M);

    //Draw all systematic sample to the one canvas and draw diffXsec with error
    auto grp_data_diffXsec_dR = buildGraphFromHist(h_data_diffXsec_dR);
    TCanvas *c = new TCanvas("","",800,800);
    TPaveText *label_cms = tdrCMSlabel();
    TLegend *leg = new TLegend(0.6,0.7,0.88,0.88);
    if(!useTUnfold) leg->SetHeader(Form("k = %d", reg_dR), "C");
    leg->AddEntry(grp_data_diffXsec_dR, "Unfolded data", "lp");
    h_data_diffXsec_dR->Draw("axis");
    //grp_data_diffXsec_dR->Draw("p same");
    for(int isys = 0; isys < nsys; ++isys){
      for(int ivar = 0; ivar < nvar; ++ivar){
	leg->AddEntry(h_MC_diffXsec_sys_dR[isys][ivar], 
	    Form("%s_%s",v_sysname[isys].c_str(),v_sysvar[ivar].c_str()),"l");
	h_MC_diffXsec_sys_dR[isys][ivar]->SetLineColor(kRed+(isys+1)*(ivar+1));
	h_MC_diffXsec_sys_dR[isys][ivar]->Draw("hist same");
      }
    }
    grp_data_diffXsec_dR->Draw("p same");
    label_cms->Draw("same");
    leg->Draw("same");
    c->Print(Form("result/%s_Ch%d_unfoldedSysMCtotal_dR.pdf",method.c_str(), ich),"pdf");
    c->Clear();
    leg->Clear();

    c->cd();
    auto grp_data_diffXsec_M = buildGraphFromHist(h_data_diffXsec_M);
    if(!useTUnfold) leg->SetHeader(Form("k = %d", reg_M), "C");
    leg->AddEntry(grp_data_diffXsec_M, "Unfolded data", "lp");
    h_data_diffXsec_M->Draw("axis");
    //grp_data_diffXsec_dR->Draw("p same");
    for(int isys = 0; isys < nsys; ++isys){
      for(int ivar = 0; ivar < nvar; ++ivar){
	leg->AddEntry(h_MC_diffXsec_sys_M[isys][ivar], 
	    Form("%s_%s",v_sysname[isys].c_str(),v_sysvar[ivar].c_str()),"l");
	h_MC_diffXsec_sys_M[isys][ivar]->SetLineColor(kRed+(isys+1)*(ivar+1));
	h_MC_diffXsec_sys_M[isys][ivar]->Draw("hist same");
      }
    }
    grp_data_diffXsec_M->Draw("p same");
    label_cms->Draw("same");
    leg->Draw("same");
    c->Print(Form("result/%s_Ch%d_unfoldedSysMCtotal_M.pdf",method.c_str(), ich),"pdf");
    c->Clear();
    leg->Clear();

    gStyle->SetHatchesSpacing(1);
    gStyle->SetHatchesLineWidth(1);
    c->cd();
    leg->AddEntry(grp_data_diffXsec_dR, "Unfolded data", "lp");
    leg->AddEntry(h_MC_diffXsec_nosel_dR, "Powheg + Pythia", "F");
    leg->AddEntry(grp_error_dR, "Uncertainties", "F");
    grp_error_dR->SetFillStyle(3154);
    grp_data_diffXsec_dR->SetMarkerSize(2);
    h_data_diffXsec_dR->Draw("axis");
    h_MC_diffXsec_nosel_dR->Draw("hist same"); 
    grp_error_dR->Draw("2 same");
    grp_data_diffXsec_dR->Draw("px same");
    leg->Draw("same");
    label_cms->Draw("same");
    c->Print(Form("result/%s_unfoldedDataWithErr_Ch%d_dR.pdf", method.c_str(), ich),"pdf");
    c->Clear();
    leg->Clear();

    c->cd();
    leg->AddEntry(grp_data_diffXsec_M, "Unfolded data", "lp");
    leg->AddEntry(h_MC_diffXsec_nosel_M, "Powheg + Pythia", "F");
    leg->AddEntry(grp_error_M, "Uncertainties", "F");
    grp_error_M->SetFillStyle(3154);
    grp_data_diffXsec_M->SetMarkerSize(2);
    h_data_diffXsec_M->Draw("axis");
    h_MC_diffXsec_nosel_M->Draw("hist same"); 
    grp_error_M->Draw("2 same");
    grp_data_diffXsec_M->Draw("px same");
    leg->Draw("same");
    label_cms->Draw("same");
    c->Print(Form("result/%s_unfoldedDataWithErr_Ch%d_M.pdf", method.c_str(), ich),"pdf");
    */
  }
}

