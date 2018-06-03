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
 
  gErrorIgnoreLevel = kWarning;
  gROOT->ProcessLine("setTDRStyle();");

  TPaveText *label_cms = tdrCMSlabel();

  TFile *f_data[2] = {
    TFile::Open(Form("%shist_%s.root", input.c_str(), DATA_[MUON_].c_str())),
    TFile::Open(Form("%shist_%s.root", input.c_str(), DATA_[ELECTRON_].c_str()))
  };
  TFile *f_sample[static_cast<int>(Sample_List_::LAST)];
  for(int i=0; i<static_cast<int>(Sample_List_::LAST); ++i){
    f_sample[i] = TFile::Open(Form("%shist_%s.root", input.c_str(), NAME_[i].c_str()));
  }
  TFile *f_respMatrix = TFile::Open(Form("%shist_respMatrix_ttbb_test.root",input.c_str()));

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
    h_resp_dR[iChannel] = (TH2 *)f_respMatrix->Get(Form("h_%s_Ch%d_S3_%s",
	  RESPONSE_MATRIX_DELTAR_, iChannel, NAME_[TTBB_].c_str()));
    h_resp_M[iChannel] = (TH2 *)f_respMatrix->Get(Form("h_%s_Ch%d_S3_%s",
	  RESPONSE_MATRIX_INVARIANT_MASS_, iChannel, NAME_[TTBB_].c_str()));
    h_reco_dR[iChannel] = (TH1 *)f_sample[TTBB_]->Get(Form("h_%s_Ch%d_S3_%s",
	  RECO_ADDJETS_DELTAR_, iChannel, NAME_[TTBB_].c_str()));
    h_reco_M[iChannel] = (TH1 *)f_sample[TTBB_]->Get(Form("h_%s_Ch%d_S3_%s",
	  RECO_ADDJETS_INVARIANT_MASS_, iChannel, NAME_[TTBB_].c_str()));
    h_gen_nosel_dR[iChannel] = (TH1 *)f_respMatrix->Get(Form("h_%s_Ch%d_nosel_%s",
	  GEN_ADDBJETS_DELTAR_, iChannel, NAME_[TTBB_].c_str()));
    h_gen_nosel_M[iChannel] = (TH1 *)f_respMatrix->Get(Form("h_%s_Ch%d_nosel_%s",
	  GEN_ADDBJETS_INVARIANT_MASS_, iChannel, NAME_[TTBB_].c_str()));
    h_gen_dR[iChannel] = (TH1 *)f_respMatrix->Get(Form("h_%s_Ch%d_S3_%s",
	  GEN_ADDBJETS_DELTAR_, iChannel, NAME_[TTBB_].c_str()));
    h_gen_M[iChannel] = (TH1 *)f_respMatrix->Get(Form("h_%s_Ch%d_S3_%s",
	  GEN_ADDBJETS_INVARIANT_MASS_, iChannel, NAME_[TTBB_].c_str()));
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

  TH1 *h_gen_nosel_dR_lep = (TH1 *)h_gen_nosel_dR[0]->Clone();
  h_gen_nosel_dR_lep->Add(h_gen_nosel_dR[1]);
  TH1 *h_gen_nosel_M_lep = (TH1 *)h_gen_nosel_M[0]->Clone();
  h_gen_nosel_M_lep->Add(h_gen_nosel_M[1]);
   
  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    TCanvas *c = new TCanvas("","",800,800);
    TLegend *leg = new TLegend(0.6,0.7,0.88,0.88);
   
    TGraphErrors *grp_data_dR = buildGraphFromHist(h_data_dR[iChannel]);
    TH1 *h_reco_dR_copy = (TH1 *)h_reco_dR[iChannel]->Clone();
    //TGraphErrors *grp_reco_dR = buildGraphFromHist(h_reco_dR[iChannel]);

    h_data_dR[iChannel]->GetYaxis()->SetTitle("Events");
    h_data_dR[iChannel]->GetYaxis()->SetTitleOffset(1.3);
    h_data_dR[iChannel]->SetMaximum(200);
    grp_data_dR->SetMarkerColor(kBlack);
    grp_data_dR->SetLineColor(kBlack);
    h_reco_dR_copy->SetMarkerColor(kBlue);
    h_reco_dR_copy->SetLineColor(kBlue);

    leg->AddEntry(grp_data_dR, "data - background", "lp");
    leg->AddEntry(h_reco_dR_copy, "t#bar{t}b#bar{b} MC", "l");

    h_data_dR[iChannel]->Draw("axis");
    grp_data_dR->Draw("p same");
    h_reco_dR_copy->Draw("hist same");
    label_cms->Draw("same");
    leg->Draw("same");

    c->Print(Form("result/subDataMC_Ch%d_dR.pdf",iChannel),"pdf");
    c->Clear();
    leg->Clear();
   
    c->cd();
    TGraphErrors *grp_data_M = buildGraphFromHist(h_data_M[iChannel]);
    TH1 *h_reco_M_copy = (TH1 *)h_reco_M[iChannel]->Clone();
    //TGraphErrors *grp_reco_M = buildGraphFromHist(h_reco_M[iChannel]);

    h_data_M[iChannel]->GetYaxis()->SetTitle("Events");
    h_data_M[iChannel]->GetYaxis()->SetTitleOffset(1.3);
    h_data_M[iChannel]->SetMaximum(200);
    grp_data_M->SetMarkerColor(kBlack);
    grp_data_M->SetLineColor(kBlack);
    h_reco_M_copy->SetMarkerColor(kBlue);
    h_reco_M_copy->SetLineColor(kBlue);

    leg->AddEntry(grp_data_M, "data - background", "lp");
    leg->AddEntry(h_reco_M_copy, "t#bar{t}b#bar{b} MC", "l");

    h_data_M[iChannel]->Draw("axis");
    grp_data_M->Draw("p same");
    h_reco_M_copy->Draw("hist same");
    label_cms->Draw("same");
    leg->Draw("same");

    c->Print(Form("result/subDataMC_Ch%d_M.pdf",iChannel),"pdf");
    c->Clear();
    leg->Clear();

    c->cd();
    auto h_dR_projectionX = h_resp_dR[iChannel]->ProjectionX();
    h_dR_projectionX->SetLineColor(kRed);
  
    leg->AddEntry(h_reco_dR_copy, "t#bar{t}b#bar{b} MC", "lp");
    leg->AddEntry(h_dR_projectionX, "X-axis projection", "l");

    h_dR_projectionX->GetYaxis()->SetTitle("Events");
    h_dR_projectionX->GetYaxis()->SetTitleOffset(1.3);
    h_dR_projectionX->SetMaximum(150);
    h_dR_projectionX->Draw("hist");
    h_reco_dR_copy->Draw("hist same");
    label_cms->Draw("same");
    leg->Draw("same");
    
    c->Print(Form("result/compRecoResp_Ch%d_dR.pdf",iChannel),"pdf");
    leg->Clear();
    c->Clear();

    c->cd();
    TH1 *h_gen_dR_copy = (TH1 *)h_gen_dR[iChannel]->Clone();
    auto h_dR_projectionY = h_resp_dR[iChannel]->ProjectionY();
    h_gen_dR_copy->SetLineColor(kBlue);
    h_dR_projectionY->SetLineColor(kRed);
  
    leg->AddEntry(h_gen_dR_copy, "Powheg + Pythia", "lp");
    leg->AddEntry(h_dR_projectionY, "Y-axis projection", "l");

    h_dR_projectionY->GetYaxis()->SetTitle("Events");
    h_dR_projectionY->GetYaxis()->SetTitleOffset(1.3);
    h_dR_projectionY->SetMaximum(150);
    h_dR_projectionY->GetXaxis()->SetTitleOffset(1.2);
    h_dR_projectionY->Draw("hist");
    h_gen_dR_copy->Draw("hist same");
    label_cms->Draw("same");
    leg->Draw("same");
    
    c->Print(Form("result/compGenResp_Ch%d_dR.pdf",iChannel),"pdf");
    leg->Clear();
    c->Clear();

    c->cd();
    auto h_M_projectionX = h_resp_M[iChannel]->ProjectionX();
    h_M_projectionX->SetLineColor(kRed);
  
    leg->AddEntry(h_reco_M_copy, "t#bar{t}b#bar{b} MC", "lp");
    leg->AddEntry(h_M_projectionX, "X-axis projection", "l");

    h_M_projectionX->GetYaxis()->SetTitle("Events");
    h_M_projectionX->GetYaxis()->SetTitleOffset(1.3);
    h_M_projectionX->SetMaximum(150);
    h_M_projectionX->Draw("hist");
    h_reco_M_copy->Draw("hist same");
    label_cms->Draw("same");
    leg->Draw("same");
    
    c->Print(Form("result/compRecoResp_Ch%d_M.pdf",iChannel),"pdf");
    leg->Clear();
    c->Clear();

    c->cd();
    TH1 *h_gen_M_copy = (TH1 *)h_gen_M[iChannel]->Clone();
    auto h_M_projectionY = h_resp_M[iChannel]->ProjectionY();
    h_gen_M_copy->SetLineColor(kBlue);
    h_M_projectionY->SetLineColor(kRed);
  
    leg->AddEntry(h_gen_M_copy, "Powheg + Pythia", "lp");
    leg->AddEntry(h_M_projectionY, "Y-axis projection", "l");

    h_M_projectionY->GetYaxis()->SetTitle("Events");
    h_M_projectionY->GetYaxis()->SetTitleOffset(1.3);
    h_M_projectionY->SetMaximum(150);
    h_M_projectionY->GetXaxis()->SetTitleOffset(1.2);
    h_M_projectionY->Draw("hist");
    h_gen_M_copy->Draw("hist same");
    label_cms->Draw("same");
    leg->Draw("same");
    
    c->Print(Form("result/compGenResp_Ch%d_M.pdf",iChannel),"pdf");
    c->Clear();
    leg->Clear();

    c->cd();
    TH1 *h_gen_nosel_dR_copy = (TH1 *)h_gen_nosel_dR_lep->Clone();
    TH1 *h_gen_avoidVoid_dR = avoidVoid(h_gen_dR[iChannel],0,iChannel);
    TGraphErrors *grp_gen_avoidVoid_dR = buildGraphFromHist(h_gen_avoidVoid_dR);
    h_gen_nosel_dR_copy->SetLineColor(kRed);
    h_gen_nosel_dR_copy->SetFillColor(kRed);
    grp_gen_avoidVoid_dR->SetLineColor(kBlue);
    grp_gen_avoidVoid_dR->SetMarkerColor(kBlue);

    leg->AddEntry(grp_gen_avoidVoid_dR, "Step3xAcceptance", "lp");
    leg->AddEntry(h_gen_nosel_dR_copy, "Powheg + Pythia", "f");

    h_gen_nosel_dR_copy->GetYaxis()->SetTitle("Events");
    h_gen_nosel_dR_copy->GetYaxis()->SetTitleOffset(1.3);
    h_gen_nosel_dR_copy->GetXaxis()->SetTitleOffset(1.2);
    h_gen_nosel_dR_copy->SetMaximum(18000);
    h_gen_nosel_dR_copy->Draw("hist");
    grp_gen_avoidVoid_dR->Draw("p same");
    label_cms->Draw("same");
    leg->Draw("same");

    c->Print(Form("result/compS3xAccpNosel_Ch%d_dR.pdf",iChannel),"pdf");

    c->~TCanvas();
    leg->~TLegend();
  }
  
  std::string method;
  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    TH1 *h_unfoldedMC_dR, *h_unfoldedData_dR, *h_unfoldedMC_M, *h_unfoldedData_M;
    
    int reg = 3;
    if(!useTUnfold){
      //runSVDUnfold(TH1 *h_in, TH2 *h_response_matrix, int reg);
      method = Form("SVD_k%d",reg);
      h_unfoldedMC_dR = runSVDUnfold(h_reco_dR[iChannel], h_resp_dR[iChannel], reg);
      h_unfoldedData_dR = runSVDUnfold(h_data_dR[iChannel], h_resp_dR[iChannel], reg);
      h_unfoldedMC_M = runSVDUnfold(h_reco_M[iChannel], h_resp_M[iChannel], reg);
      h_unfoldedData_M = runSVDUnfold(h_data_M[iChannel], h_resp_M[iChannel], reg);

      const bool findBestK = true;
      if(findBestK){
	vector<TH1 *> v_unfoldedMC_dR, v_unfoldedMC_M, v_unfoldedData_dR, v_unfoldedData_M;
	for(int i=2; i<=h_unfoldedMC_dR->GetNbinsX(); ++i){
	  TH1 *h_tmpMC_dR = runSVDUnfold(h_reco_dR[iChannel], h_resp_dR[iChannel], i);
	  TH1 *h_tmpData_dR = runSVDUnfold(h_reco_dR[iChannel], h_resp_dR[iChannel], i);
	  //h_tmpMC_dR = calculateDiffXsec(h_tmpMC_dR, 0, iChannel, false);
	  //h_tmpData_dR = calculateDiffXsec(h_tmpData_dR, 0, iChannel, true);
	  v_unfoldedMC_dR.push_back(h_tmpMC_dR);
	  v_unfoldedData_dR.push_back(h_tmpData_dR);
	}
	for(int i=2; i<=h_unfoldedMC_M->GetNbinsX(); ++i){
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
    
    TCanvas *c = new TCanvas("","",800,800);
    TLegend *leg = new TLegend(0.6,0.7,0.88,0.88);

    c->cd();
    TGraphErrors *grp_unfoldedMC_dR = buildGraphFromHist(h_unfoldedMC_dR);
    TH1 *h_gen_dR_copy = (TH1 *)h_gen_dR[iChannel]->Clone();
    grp_unfoldedMC_dR->SetLineColor(kBlue);
    grp_unfoldedMC_dR->SetMarkerColor(kBlue);
    h_gen_dR_copy->SetLineColor(kRed);
    h_gen_dR_copy->SetMarkerColor(kRed);
   
    leg->AddEntry(grp_unfoldedMC_dR, "unfolded MC", "lp");
    leg->AddEntry(h_gen_dR_copy, "Powheg + Pythia", "l");
  
    h_gen_dR_copy->SetMaximum(200);
    h_gen_dR_copy->GetYaxis()->SetTitle("Events");
    h_gen_dR_copy->GetYaxis()->SetTitleOffset(1.9);
    h_gen_dR_copy->GetXaxis()->SetTitleOffset(1.2);
    h_gen_dR_copy->Draw("hist");
    grp_unfoldedMC_dR->Draw("p same");
    leg->Draw("same");
    label_cms->Draw("same");

    c->Print(Form("result/%s_unfoldedMC_Ch%d_dR.pdf",method.c_str(),iChannel),"pdf");
    leg->Clear();
    c->Clear();

    c->cd();
    TGraphErrors *grp_unfoldedMC_M = buildGraphFromHist(h_unfoldedMC_M);
    TH1 *h_gen_M_copy = (TH1 *)h_gen_M[iChannel]->Clone();
    grp_unfoldedMC_M->SetLineColor(kBlue);
    grp_unfoldedMC_M->SetMarkerColor(kBlue);
    h_gen_M_copy->SetLineColor(kRed);
    h_gen_M_copy->SetMarkerColor(kRed);
   
    leg->AddEntry(grp_unfoldedMC_M, "unfolded MC", "lp");
    leg->AddEntry(h_gen_M_copy, "Powheg + Pythia", "l");
   
    h_gen_M_copy->SetMaximum(200);
    h_gen_M_copy->GetYaxis()->SetTitle("Events");
    h_gen_M_copy->GetYaxis()->SetTitleOffset(1.9);
    h_gen_M_copy->GetXaxis()->SetTitleOffset(1.2);
    h_gen_M_copy->Draw("hist");
    grp_unfoldedMC_M->Draw("p same");
    leg->Draw("same");
    label_cms->Draw("same");

    c->Print(Form("result/%s_unfoldedMC_Ch%d_M.pdf",method.c_str(),iChannel),"pdf");
    leg->Clear();
    c->Clear();
    
    c->cd();
    //TH1 *calculateDiffXsec(TH1 *h_in, int variable, int channel, int process){
    TH1 *h_MC_diffXsec_dR = calculateDiffXsec(h_unfoldedMC_dR,0,iChannel,false);
    //h_MC_diffXsec_dR->Scale(2);
    TH1 *h_MC_diffXsec_nosel_dR = calculateDiffXsec(h_gen_nosel_dR_lep);
    TGraphErrors *grp_MC_diffXsec_dR = buildGraphFromHist(h_MC_diffXsec_dR);
    //TGraphErrors *grpMC_diffXsec_nosel_dR = buildGraphFromHist(h_MC_diffXsec_nosel_dR);
    grp_MC_diffXsec_dR->SetLineColor(kBlue);
    grp_MC_diffXsec_dR->SetMarkerColor(kBlue);
    h_MC_diffXsec_nosel_dR->SetLineColor(kRed);
    h_MC_diffXsec_nosel_dR->SetFillColor(kRed);
    h_MC_diffXsec_nosel_dR->SetMarkerSize(0);

    leg->AddEntry(grp_MC_diffXsec_dR, "Unfolded t#bar{t}b#bar{b} MC", "lp");
    leg->AddEntry(h_MC_diffXsec_nosel_dR, "Powheg + Pythia", "F");

    h_MC_diffXsec_nosel_dR->GetYaxis()->SetTitle("#frac{d#sigma^{full}}{d#DeltaR}[pb]");
    h_MC_diffXsec_nosel_dR->GetYaxis()->SetTitleOffset(1.9);
    h_MC_diffXsec_nosel_dR->SetMaximum(3.0);
    h_MC_diffXsec_nosel_dR->SetMinimum(0.0);
    h_MC_diffXsec_nosel_dR->GetXaxis()->SetTitleOffset(1.2);
    
    h_MC_diffXsec_nosel_dR->Draw("axis");
    h_MC_diffXsec_nosel_dR->Draw("hist");
    grp_MC_diffXsec_dR->Draw("p e same");
    label_cms->Draw("same");
    leg->Draw("same");
    
    c->Print(Form("result/%s_vali_MCToGen_diffXsec_Ch%d_dR.pdf",method.c_str(),iChannel),"pdf");
    c->Clear();
    leg->Clear();

    c->cd();
    TH1 *h_data_diffXsec_dR = calculateDiffXsec(h_unfoldedData_dR,0,iChannel,true);
    TGraphErrors *grp_data_diffXsec_dR = buildGraphFromHist(h_data_diffXsec_dR);
    grp_data_diffXsec_dR->SetLineColor(kBlack);
    grp_data_diffXsec_dR->SetMarkerColor(kBlack);

    leg->AddEntry(grp_data_diffXsec_dR, "Unfolded Data", "lp");
    leg->AddEntry(h_MC_diffXsec_nosel_dR, "Powheg + Pythia", "F");

    h_MC_diffXsec_nosel_dR->Draw("hist");
    grp_data_diffXsec_dR->Draw("p e same");
    label_cms->Draw("same");
    leg->Draw("same");

    c->Print(Form("result/%s_result_diffXsec_Ch%d_dR.pdf",method.c_str(),iChannel),"pdf");
    c->Clear();
    leg->Clear();

    c->cd();
    TH1 *h_MC_diffXsec_M = calculateDiffXsec(h_unfoldedMC_M,1,iChannel,false);
    TH1 *h_MC_diffXsec_nosel_M = calculateDiffXsec(h_gen_nosel_M_lep);
    TGraphErrors *grp_MC_diffXsec_M = buildGraphFromHist(h_MC_diffXsec_M);
    grp_MC_diffXsec_M->SetLineColor(kBlue);
    grp_MC_diffXsec_M->SetMarkerColor(kBlue);
    h_MC_diffXsec_nosel_M->SetLineColor(kRed);
    h_MC_diffXsec_nosel_M->SetFillColor(kRed);
    h_MC_diffXsec_nosel_M->SetMarkerSize(0);

    leg->AddEntry(grp_MC_diffXsec_M, "Unfolded t#bar{t}b#bar{b} MC", "lp");
    leg->AddEntry(h_MC_diffXsec_nosel_M, "Powheg + Pythia", "F");

    h_MC_diffXsec_nosel_M->Draw("axis");
    h_MC_diffXsec_nosel_M->GetYaxis()->SetTitle("#frac{d#sigma^{full}}{dm_{b#bar{b}}}[pb/GeV]");
    h_MC_diffXsec_nosel_M->GetYaxis()->SetTitleOffset(1.9);
    //h_MC_diffXsec_nosel_M->GetYaxis()->SetLabelSize(0.02);
    h_MC_diffXsec_nosel_M->SetMaximum(0.04);
    h_MC_diffXsec_nosel_M->SetMinimum(0.);
    h_MC_diffXsec_nosel_M->GetXaxis()->SetTitle("m_{b#bar{b}}[GeV]");
    h_MC_diffXsec_nosel_M->GetXaxis()->SetTitleOffset(1.2);
    h_MC_diffXsec_nosel_M->Draw("hist");
    grp_MC_diffXsec_M->Draw("p e same");
    label_cms->Draw("same");
    leg->Draw("same");

    c->Print(Form("result/%s_vali_MCToGen_diffXsec_Ch%d_M.pdf",method.c_str(),iChannel),"pdf");
    c->Clear();
    leg->Clear();
    
    c->cd();
    TH1 *h_data_diffXsec_M = calculateDiffXsec(h_unfoldedData_M,1,iChannel,true);
    TGraphErrors *grp_data_diffXsec_M = buildGraphFromHist(h_data_diffXsec_M);
    grp_data_diffXsec_M->SetLineColor(kBlack);
    grp_data_diffXsec_M->SetMarkerColor(kBlack);

    leg->AddEntry(grp_data_diffXsec_M, "Unfolded Data", "lp");
    leg->AddEntry(h_MC_diffXsec_nosel_M, "Powheg + Pythia", "F");

    h_MC_diffXsec_nosel_M->Draw("hist");
    grp_data_diffXsec_M->Draw("p e same");
    label_cms->Draw("same");
    leg->Draw("same");

    c->Print(Form("result/%s_result_diffXsec_Ch%d_M.pdf",method.c_str(),iChannel),"pdf");
    c->~TCanvas();
    leg->~TLegend();
  }
}
