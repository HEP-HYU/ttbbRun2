/********************************************************************************
 * run by SVDUnfold :root -l -b -q ttbbDiffXsec.C'(false)'			*
 * run by TUnfold without tau scanning range constraint : 			*
 * 	root -l -b -q ttbbDiffXsec.C'(true)' 					*
 * run by TUnfold with tau scanning range constraint :				*
 * 	root -l -b -q ttbbDiffXec.C'(true, tauMin, tauMax)'			*
 * run by TUnfold with fixed tau :						*
 * 	root -l -b -q ttbbDiffXsec.C'(true, 0., 0., true, fixedTau)'		*
 ********************************************************************************/

#include <vector>
#include <iostream>
#include <string>

#include <TLegend.h>

#include "ttbbDiffXsec.h"
#include "runTUnfold.C"
#include "runSVDUnfold.C"
#include "scanHist.C"

void ttbbDiffXsec(bool useTUnfold_ = false, bool scanbyLcurve_ = true, double tauMin_ = 0., double tauMax_ = 0., bool fixTau_ = false, float fixedTau_ = 0.){
  const bool useTUnfold = useTUnfold_;
  const bool scanbyLcurve = scanbyLcurve_;
  const double tauMax = tauMax_, tauMin = tauMin_;
  const bool fixTau = fixTau_;
  const float fixedTau = fixedTau_;
 
  gErrorIgnoreLevel = kFatal; //kWarning;

  TFile *f_data[2] = {
    TFile::Open(Form("%shist_%s.root", input.c_str(), DATA_[MUON_].c_str())),
    TFile::Open(Form("%shist_%s.root", input.c_str(), DATA_[ELECTRON_].c_str()))
  };
  TFile *f_sample[static_cast<int>(Sample_List_::LAST)];
  for(int i=0; i<static_cast<int>(Sample_List_::LAST); ++i){
    f_sample[i] = TFile::Open(Form("%shist_%s.root", input.c_str(), NAME_[i].c_str()));
  }
  TFile *f_respMatrix = TFile::Open(Form("%shist_respMatrix_ttbb.root",input.c_str()));

  TH1 *h_data_dR[nChannel], *h_data_M[nChannel];
  TH2 *h_resp_dR[nChannel], *h_resp_M[nChannel];
  TH1 *h_reco_dR[nChannel], *h_reco_M[nChannel];
  TH1 *h_gen_dR[nChannel], *h_gen_M[nChannel];
  TH1 *h_gen_nosel_dR[nChannel], *h_gen_nosel_M[nChannel];
  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    h_data_dR[iChannel] = (TH1 *)f_data[iChannel]->Get(Form("h_%s_Ch%d_S3_%s",
	  RECO_ADDJETS_DELTAR_, iChannel, DATA_[iChannel].c_str()));
    h_data_M[iChannel] = (TH1 *)f_data[iChannel]->Get(Form("h_%s_Ch%d_S3_%s",
	  RECO_ADDJETS_INVARIANT_MASS_, iChannel, DATA_[iChannel].c_str()));
    h_resp_dR[iChannel] = (TH2 *)f_respMatrix->Get(Form("h_%s_Ch%d_S3_TTLJ_PowhegPythia_%s",
	  RESPONSE_MATRIX_DELTAR_, iChannel, NAME_[TTBB_].c_str()));
    h_resp_M[iChannel] = (TH2 *)f_respMatrix->Get(Form("h_%s_Ch%d_S3_TTLJ_PowhegPythia_%s",
	  RESPONSE_MATRIX_INVARIANT_MASS_, iChannel, NAME_[TTBB_].c_str()));
    h_reco_dR[iChannel] = (TH1 *)f_sample[TTBB_]->Get(Form("h_%s_Ch%d_S3_%s",
	  RECO_ADDJETS_DELTAR_, iChannel, NAME_[TTBB_].c_str()));
    h_reco_M[iChannel] = (TH1 *)f_sample[TTBB_]->Get(Form("h_%s_Ch%d_S3_%s",
	  RECO_ADDJETS_INVARIANT_MASS_, iChannel, NAME_[TTBB_].c_str()));
    h_gen_nosel_dR[iChannel] = (TH1 *)f_respMatrix->Get(Form("h_GenbJetDeltaR_Ch%d_nosel_TTLJ_PowhegPythia_%s",
	   iChannel, NAME_[TTBB_].c_str()));
    h_gen_nosel_M[iChannel] = (TH1 *)f_respMatrix->Get(Form("h_GenbJetInvMass_Ch%d_nosel_TTLJ_PowhegPythia_%s",
	   iChannel, NAME_[TTBB_].c_str()));
    h_gen_dR[iChannel] = (TH1 *)f_respMatrix->Get(Form("h_GenbJetDeltaR_Ch%d_S3_TTLJ_PowhegPythia_%s",
	   iChannel, NAME_[TTBB_].c_str()));
    h_gen_M[iChannel] = (TH1 *)f_respMatrix->Get(Form("h_GenbJetInvMass_Ch%d_S3_TTLJ_PowhegPythia_%s",
	   iChannel, NAME_[TTBB_].c_str()));
  }
 
  for(int i=0; i<static_cast<int>(Sample_List_::LAST); ++i){
    for(int iChannel=0; iChannel<nChannel; ++iChannel){
      TH1 *EventInfo = (TH1 *)f_sample[i]->Get("EventInfo");
      double scale = LUMINOSITY_*XSEC_[i]/EventInfo->GetBinContent(2);
      if(i == TTBB_){
        h_resp_dR[iChannel]->Scale(scale); h_resp_M[iChannel]->Scale(scale);
	h_reco_dR[iChannel]->Scale(scale); h_reco_M[iChannel]->Scale(scale);
	h_gen_nosel_dR[iChannel]->Scale(scale); h_gen_nosel_M[iChannel]->Scale(scale);
	h_gen_dR[iChannel]->Scale(scale); h_gen_M[iChannel]->Scale(scale);
      }
      else{
	TH1 *h_bkg_dR = (TH1 *)f_sample[i]->Get(Form("h_%s_Ch%d_S3_%s",RECO_ADDJETS_DELTAR_,iChannel,NAME_[i].c_str()));
	TH1 *h_bkg_M = (TH1 *)f_sample[i]->Get(Form("h_%s_Ch%d_S3_%s",RECO_ADDJETS_INVARIANT_MASS_,iChannel,NAME_[i].c_str()));
	h_bkg_dR->Scale(scale); h_bkg_M->Scale(scale);
	h_data_dR[iChannel]->Add(h_bkg_dR,-1);h_data_M[iChannel]->Add(h_bkg_M,-1);
      }
    }
  }

  cout << "Done" << endl;

  TH1 *h_gen_nosel_dR_lep = (TH1 *)h_gen_nosel_dR[0]->Clone();
  h_gen_nosel_dR_lep->Add(h_gen_nosel_dR[1]);
  TH1 *h_gen_nosel_M_lep = (TH1 *)h_gen_nosel_M[0]->Clone();
  h_gen_nosel_M_lep->Add(h_gen_nosel_M[1]);
   
  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    DrawHist(Form("subDataMC_Ch%d_dR.pdf",iChannel),
	h_data_dR[iChannel], h_reco_dR[iChannel], kDataBkgSub, kReco, kBlack, kBlue, true);
    DrawHist(Form("subDataMC_Ch%d_M.pdf",iChannel),
	h_data_M[iChannel], h_reco_M[iChannel], kDataBkgSub, kReco, kBlack, kBlue, false);
    DrawHist(Form("compRecoResp_Ch%d_dR.pdf",iChannel),
	h_reco_dR[iChannel], h_resp_dR[iChannel]->ProjectionX(), kReco, kRespX, kBlue, kRed, true);
    DrawHist(Form("compGenResp_Ch%d_dR.pdf",iChannel),
	h_gen_dR[iChannel], h_resp_dR[iChannel]->ProjectionY(), kGen, kRespY, kBlue, kRed, true);
    DrawHist(Form("compRecoResp_Ch%d_M.pdf",iChannel),
	h_reco_M[iChannel], h_resp_M[iChannel]->ProjectionX(), kReco, kRespX, kBlue, kRed, false);
    DrawHist(Form("compGenResp_Ch%d_M.pdf",iChannel),
	h_gen_M[iChannel], h_resp_M[iChannel]->ProjectionY(), kGen, kRespY, kBlue, kRed, false);
  }

  cout << "Start Unfolding..." << endl;
  std::string method;
  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    TH1 *h_unfoldedMC_dR, *h_unfoldedData_dR, *h_unfoldedMC_M, *h_unfoldedData_M;
    
    int reg_dR = 3, reg_M = 3;
    if(!useTUnfold){
      //runSVDUnfold(TH1 *h_in, TH2 *h_response_matrix, int reg);
      method = Form("SVD");

      const bool findBestK = true;
      if(findBestK){
	vector<TH1 *> v_unfoldedMC_dR, v_unfoldedMC_M, v_unfoldedData_dR, v_unfoldedData_M;
	for(int i=2; i<=h_reco_dR[iChannel]->GetNbinsX(); ++i){
	  TH1 *h_tmpMC_dR = runSVDUnfold(h_reco_dR[iChannel], h_resp_dR[iChannel], i);
	  TH1 *h_tmpData_dR = runSVDUnfold(h_reco_dR[iChannel], h_resp_dR[iChannel], i);
	  //h_tmpMC_dR = calculateDiffXsec(h_tmpMC_dR, 0, iChannel, false);
	  //h_tmpData_dR = calculateDiffXsec(h_tmpData_dR, 0, iChannel, true);
	  v_unfoldedMC_dR.push_back(h_tmpMC_dR);
	  v_unfoldedData_dR.push_back(h_tmpData_dR);
	}
	for(int i=2; i<=h_reco_M[iChannel]->GetNbinsX(); ++i){
	  TH1 *h_tmpMC_M = runSVDUnfold(h_reco_M[iChannel], h_resp_M[iChannel], i);
	  TH1 *h_tmpData_M = runSVDUnfold(h_reco_M[iChannel], h_resp_M[iChannel], i);
	  //h_tmpMC_M = calculateDiffXsec(h_tmpMC_M, 1, iChannel, false);
	  //h_tmpData_M = calculateDiffXsec(h_tmpData_M, 1, iChannel, true);
	  v_unfoldedMC_M.push_back(h_tmpMC_M);
	  v_unfoldedData_M.push_back(h_tmpData_M);
	}
        TH1 *h_truth_dR = (TH1 *)h_gen_dR[iChannel]->Clone();//calculateDiffXsec(h_gen_nosel_dR_lep);
        TH1 *h_truth_M = (TH1 *)h_gen_M[iChannel]->Clone();//calculateDiffXsec(h_gen_nosel_M_lep);
	findBestk(v_unfoldedMC_dR, 1, 0, iChannel, h_truth_dR);
	findBestk(v_unfoldedMC_M, 1, 1, iChannel, h_truth_M);
	findBestk(v_unfoldedData_dR, 0, 0, iChannel, h_truth_dR);
	findBestk(v_unfoldedData_M, 0, 1, iChannel, h_truth_M);
      }

      h_unfoldedMC_dR = runSVDUnfold(h_reco_dR[iChannel], h_resp_dR[iChannel], reg_dR);
      h_unfoldedData_dR = runSVDUnfold(h_data_dR[iChannel], h_resp_dR[iChannel], reg_dR);
      h_unfoldedMC_M = runSVDUnfold(h_reco_M[iChannel], h_resp_M[iChannel], reg_M);
      h_unfoldedData_M = runSVDUnfold(h_data_M[iChannel], h_resp_M[iChannel], reg_M);
    }
    else{
      //scanbyRho
      //   runTUnfold(TH1 *h_in, TH2 *h_response_matrix, bool fixTau = false, double tauMin = 0., double tauMax = 0., double fixedTau_);
      //scanbyLcurve
      //   runTUnfold(TH1 *h_in, TH2 *h_response_matrix, double tauMin = 0., double tauMax = 0.);
      if(!scanbyLcurve){
	method = "TUnfold_scanRho";
	h_unfoldedMC_dR = runTUnfold(h_reco_dR[iChannel], h_resp_dR[iChannel], fixTau, tauMin, tauMax, fixedTau);
	h_unfoldedData_dR = runTUnfold(h_data_dR[iChannel], h_resp_dR[iChannel], fixTau, tauMin, tauMax, fixedTau);
	h_unfoldedMC_M = runTUnfold(h_reco_M[iChannel], h_resp_M[iChannel], fixTau, tauMin, tauMax, fixedTau);
	h_unfoldedData_M = runTUnfold(h_data_M[iChannel], h_resp_M[iChannel], fixTau, tauMin, tauMax, fixedTau);
      }
      else{
	method = "TUnfold_scanLcurve";
	h_unfoldedMC_dR = runTUnfold(h_reco_dR[iChannel], h_resp_dR[iChannel], tauMin, tauMax);
	h_unfoldedData_dR = runTUnfold(h_data_dR[iChannel], h_resp_dR[iChannel], tauMin, tauMax);
	h_unfoldedMC_M = runTUnfold(h_reco_M[iChannel], h_resp_M[iChannel], tauMin, tauMax);
	h_unfoldedData_M = runTUnfold(h_data_M[iChannel], h_resp_M[iChannel], tauMin, tauMax);
      }
    }
  
    DrawHist(Form("%s_unfoldedMC_Ch%d_dR.pdf",method.c_str(),iChannel),
	h_unfoldedMC_dR, h_gen_dR[iChannel], kReco, kGen, kBlue, kRed,
	true, false, true, useTUnfold, reg_dR, true);
    DrawHist(Form("%s_unfoldedMC_Ch%d_M.pdf",method.c_str(),iChannel),
	h_unfoldedMC_M, h_gen_M[iChannel], kReco, kGen, kBlue, kRed,
	false, false, true, useTUnfold, reg_M, true);
    
    TH1 *h_MC_diffXsec_dR = calculateDiffXsec(h_unfoldedMC_dR,0,iChannel,false);
    TH1 *h_MC_diffXsec_nosel_dR = calculateDiffXsec(h_gen_nosel_dR_lep);
    TH1 *h_data_diffXsec_dR = calculateDiffXsec(h_unfoldedData_dR,0,iChannel,true);
    DrawHist(Form("%s_MC_diffXsec_Ch%d_dR.pdf",method.c_str(),iChannel),
	h_MC_diffXsec_dR, h_MC_diffXsec_nosel_dR, kReco, kGen, kBlue, kRed,
	true, true, true, useTUnfold, reg_dR, true);
    DrawHist(Form("%s_data_diffXsec_Ch%d_dR.pdf",method.c_str(),iChannel),
	h_data_diffXsec_dR, h_MC_diffXsec_nosel_dR, kUnfoldedData, kGen, kBlack, kRed,
	true, true, true, useTUnfold, reg_dR, true);
    
    TH1 *h_MC_diffXsec_M = calculateDiffXsec(h_unfoldedMC_M,1,iChannel,false);
    TH1 *h_MC_diffXsec_nosel_M = calculateDiffXsec(h_gen_nosel_M_lep);
    TH1 *h_data_diffXsec_M = calculateDiffXsec(h_unfoldedData_M,1,iChannel,true);
    DrawHist(Form("%s_MC_diffXsec_Ch%d_M.pdf",method.c_str(),iChannel),
	h_MC_diffXsec_M, h_MC_diffXsec_nosel_M, kReco, kGen, kBlue, kRed,
	false, true, true, useTUnfold, reg_M, true);
    DrawHist(Form("%s_data_diffXsec_Ch%d_M.pdf",method.c_str(),iChannel),
	h_data_diffXsec_M, h_MC_diffXsec_nosel_M, kUnfoldedData, kGen, kBlack, kRed,
	false, true, true, useTUnfold, reg_M, true);
  }
}
