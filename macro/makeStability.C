#include "../include/histBook.h"
#include "../include/tdrstyle.C"

void makeStability(){
  string input = "../output/root/";
  string output = "../output/root/";

  cout << "FIJ" << endl;
  //FILES
  TFile *f_ttbb = TFile::Open(Form("%shist_ttbb.root",input.c_str()));
  TFile *f_matrix = TFile::Open(Form("%shist_respMatrix_ttbb_test.root",input.c_str()));
  TFile *f_outFile = TFile::Open(Form("%shist_stability_ttbb.root",output.c_str()),"recreate");
  
  //NORMALIZATION
  TH1 *EventInfo = (TH1 *)f_ttbb->Get("EventInfo");
  const double scale = LUMINOSITY_*XSEC_[TTBB_]/EventInfo->GetBinContent(2);

  //HISTOGRAMS
  HistoBook *h_stb = new HistoBook(3,NAME_[TTBB_].c_str());

  TH2D *h_respMatrix_deltaR[nChannel][nStep];
  TH2D *h_respMatrix_invMass[nChannel][nStep];

  TH1D *h_gen_addbjets_deltaR[nChannel][nStep];
  TH1D *h_gen_addbjets_invMass[nChannel][nStep];

  TH1D *h_reco_addjets_deltaR[nChannel][nStep];
  TH1D *h_reco_addjets_invMass[nChannel][nStep];

  TH1D *h_gen_addbjets_nosel_deltaR[nChannel];
  TH1D *h_gen_addbjets_nosel_invMass[nChannel];
  TH1D *h_gen_addbjets_afterSel_deltaR[nChannel];
  TH1D *h_gen_addbjets_afterSel_invMass[nChannel];

  for(int iChannel=0; iChannel<nChannel; ++iChannel){    
    h_gen_addbjets_nosel_deltaR[iChannel] = (TH1D *)f_matrix->Get(Form("h_%s_Ch%d_nosel_%s",GEN_ADDBJETS_DELTAR_,iChannel,NAME_[TTBB_].c_str()));
    h_gen_addbjets_nosel_deltaR[iChannel]->Scale(scale);
    h_gen_addbjets_nosel_invMass[iChannel] = (TH1D *)f_matrix->Get(Form("h_%s_Ch%d_nosel_%s",GEN_ADDBJETS_INVARIANT_MASS_,iChannel,NAME_[TTBB_].c_str()));
    h_gen_addbjets_nosel_invMass[iChannel]->Scale(scale);
    h_gen_addbjets_afterSel_deltaR[iChannel] = (TH1D *)f_matrix->Get(Form("h_%s_Ch%d_S3_%s",GEN_ADDBJETS_DELTAR_,iChannel,NAME_[TTBB_].c_str()));
    h_gen_addbjets_afterSel_deltaR[iChannel]->Scale(scale);
    h_gen_addbjets_afterSel_invMass[iChannel] = (TH1D *)f_matrix->Get(Form("h_%s_Ch%d_S3_%s",GEN_ADDBJETS_INVARIANT_MASS_,iChannel,NAME_[TTBB_].c_str()));
    h_gen_addbjets_afterSel_invMass[iChannel]->Scale(scale);

    for(int iStep=0; iStep<nStep; ++iStep){
      h_respMatrix_deltaR[iChannel][iStep] = (TH2D *)f_matrix->Get(Form("h_%s_Ch%d_S%d_%s",RESPONSE_MATRIX_DELTAR_,iChannel,iStep,NAME_[TTBB_].c_str()));
      h_respMatrix_deltaR[iChannel][iStep]->Scale(scale);
      h_respMatrix_invMass[iChannel][iStep] = (TH2D *)f_matrix->Get(Form("h_%s_Ch%d_S%d_%s",RESPONSE_MATRIX_INVARIANT_MASS_,iChannel,iStep,NAME_[TTBB_].c_str()));
      h_respMatrix_invMass[iChannel][iStep]->Scale(scale);

      h_gen_addbjets_deltaR[iChannel][iStep] = h_respMatrix_deltaR[iChannel][iStep]->ProjectionY();
      h_gen_addbjets_invMass[iChannel][iStep] = h_respMatrix_invMass[iChannel][iStep]->ProjectionY();

      h_reco_addjets_deltaR[iChannel][iStep] = h_respMatrix_deltaR[iChannel][iStep]->ProjectionX();
      h_reco_addjets_invMass[iChannel][iStep] = h_respMatrix_invMass[iChannel][iStep]->ProjectionX();
    }
  }
  
  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    for(int iStep=0; iStep < nStep; ++iStep){
	double acceptance_dR = 0.0;
      for(int iBin=1; iBin<h_stb->xNbins_reco_addjets_dR+1; ++iBin){
	//PURITY
	double purity_dR = 0.0;
	if(h_reco_addjets_deltaR[iChannel][iStep]->GetBinContent(iBin) != 0.0){
	  auto xlow = h_respMatrix_deltaR[iChannel][iStep]->GetXaxis()->GetBinLowEdge(iBin);
	  auto ybin = h_respMatrix_deltaR[iChannel][iStep]->GetYaxis()->FindBin(xlow);
	  purity_dR = h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(iBin,ybin)/h_reco_addjets_deltaR[iChannel][iStep]->GetBinContent(iBin);
	}
	h_stb->h_purity_deltaR[iChannel][iStep]->SetBinContent(iBin, purity_dR);
      }
      for(int iBin=1; iBin<h_stb->xNbins_gen_addbjets_dR+1; ++iBin){
	//STABILITY
	double stability_dR = 0.0;
	if(h_gen_addbjets_deltaR[iChannel][iStep]->GetBinContent(iBin) != 0.0){
	  auto ylow = h_respMatrix_deltaR[iChannel][iStep]->GetYaxis()->GetBinLowEdge(iBin);
	  auto ymax = h_respMatrix_deltaR[iChannel][iStep]->GetYaxis()->GetBinLowEdge(iBin+1);
	  auto xlow = h_respMatrix_deltaR[iChannel][iStep]->GetXaxis()->FindBin(ylow);
	  auto xmax = h_respMatrix_deltaR[iChannel][iStep]->GetXaxis()->FindBin(ymax);
	  double xbincontent = 0.;
	  for(int ixbin = xlow; ixbin < xmax; ++ixbin)
	    xbincontent += h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(ixbin,iBin);
	  stability_dR = xbincontent/h_gen_addbjets_deltaR[iChannel][iStep]->GetBinContent(iBin);
	}
	h_stb->h_stability_deltaR[iChannel][iStep]->SetBinContent(iBin, stability_dR);
      }

      for(int iBin=1; iBin<h_stb->xNbins_reco_addjets_M+1; ++iBin){
	//PURITY
	double purity_M = 0.0;
	if(h_reco_addjets_invMass[iChannel][iStep]->GetBinContent(iBin) != 0.0){
	  auto xlow = h_respMatrix_invMass[iChannel][iStep]->GetXaxis()->GetBinLowEdge(iBin);
	  auto ybin = h_respMatrix_invMass[iChannel][iStep]->GetYaxis()->FindBin(xlow);
	  purity_M = h_respMatrix_invMass[iChannel][iStep]->GetBinContent(iBin,ybin)/h_reco_addjets_invMass[iChannel][iStep]->GetBinContent(iBin);
	}
	h_stb->h_purity_invMass[iChannel][iStep]->SetBinContent(iBin, purity_M);
      }
      for(int iBin=1; iBin<h_stb->xNbins_gen_addbjets_M+1; ++iBin){
	//STABILITY
	double stability_M = 0.0;
	if(h_gen_addbjets_invMass[iChannel][iStep]->GetBinContent(iBin) != 0.0){
	  auto ylow = h_respMatrix_invMass[iChannel][iStep]->GetYaxis()->GetBinLowEdge(iBin);
	  auto ymax = h_respMatrix_invMass[iChannel][iStep]->GetYaxis()->GetBinLowEdge(iBin+1);
	  auto xlow = h_respMatrix_invMass[iChannel][iStep]->GetXaxis()->FindBin(ylow);
	  auto xmax = h_respMatrix_invMass[iChannel][iStep]->GetXaxis()->FindBin(ymax);
	  double xbincontent = 0.;
	  for(int ixbin = xlow; ixbin < xmax; ++ixbin)
	    xbincontent += h_respMatrix_invMass[iChannel][iStep]->GetBinContent(ixbin,iBin);
	  stability_M = xbincontent/h_gen_addbjets_invMass[iChannel][iStep]->GetBinContent(iBin);
	}
	h_stb->h_stability_invMass[iChannel][iStep]->SetBinContent(iBin, stability_M);
      }
    }//Step

    //ACCEPTANCE
    for(int iBin=1; iBin<h_stb->xNbins_gen_addbjets_dR+1;++iBin){
      double acceptance_dR = 0.0;
      double dR_err = 0.0;
      cout << h_gen_addbjets_nosel_deltaR[iChannel]->GetBinContent(iBin) << endl;
      if( h_gen_addbjets_nosel_deltaR[iChannel]->GetBinContent(iBin) != 0.0 ){
        acceptance_dR = h_gen_addbjets_afterSel_deltaR[iChannel]->GetBinContent(iBin)/(h_gen_addbjets_nosel_deltaR[0]->GetBinContent(iBin)+h_gen_addbjets_nosel_deltaR[1]->GetBinContent(iBin));
	dR_err = abs(acceptance_dR)*sqrt(pow(h_gen_addbjets_afterSel_deltaR[iChannel]->GetBinError(iBin)/h_gen_addbjets_afterSel_deltaR[iChannel]->GetBinContent(iBin),2)
	    +pow(h_gen_addbjets_nosel_deltaR[0]->GetBinError(iBin)/h_gen_addbjets_nosel_deltaR[0]->GetBinContent(iBin),2)+pow(h_gen_addbjets_nosel_deltaR[0]->GetBinError(iBin)/h_gen_addbjets_nosel_deltaR[0]->GetBinContent(iBin),2));
      }
      cout << "DeltaR Acceptance " << iBin << "th bin :: " << acceptance_dR << endl;
      cout << "Error : " << dR_err << endl;
      h_stb->h_acceptance_deltaR[iChannel]->SetBinContent(iBin, acceptance_dR);
      h_stb->h_acceptance_deltaR[iChannel]->SetBinError(iBin, dR_err);
    }
    cout << "Inclusive Acceptance : " << h_gen_addbjets_afterSel_deltaR[iChannel]->Integral()/(h_gen_addbjets_nosel_deltaR[0]->Integral()+h_gen_addbjets_nosel_deltaR[1]->Integral()) << endl;
    
    for(int iBin=1; iBin<h_stb->xNbins_gen_addbjets_M+1;++iBin){
      double acceptance_M = 0.0;
      double M_err = 0.0;
      if( h_gen_addbjets_nosel_invMass[iChannel]->GetBinContent(iBin) != 0.0 ){
        acceptance_M = h_gen_addbjets_afterSel_invMass[iChannel]->GetBinContent(iBin)/(h_gen_addbjets_nosel_invMass[0]->GetBinContent(iBin)+h_gen_addbjets_nosel_invMass[1]->GetBinContent(iBin));
	M_err = abs(acceptance_M)*sqrt(pow(h_gen_addbjets_afterSel_invMass[iChannel]->GetBinError(iBin)/h_gen_addbjets_afterSel_invMass[iChannel]->GetBinContent(iBin),2)
	    +pow(h_gen_addbjets_nosel_invMass[0]->GetBinError(iBin)/h_gen_addbjets_nosel_invMass[0]->GetBinContent(iBin),2)+pow(h_gen_addbjets_nosel_invMass[1]->GetBinError(iBin)/h_gen_addbjets_nosel_invMass[1]->GetBinContent(iBin),2));
      }
      cout << "InvMass Acceptance " << iBin << "th bin :: " << acceptance_M << endl;
      cout << "Error : " << M_err << endl;
      h_stb->h_acceptance_invMass[iChannel]->SetBinContent(iBin, acceptance_M);
      h_stb->h_acceptance_invMass[iChannel]->SetBinError(iBin, M_err);
    }
    cout << "Inclusive Acceptance : " << h_gen_addbjets_afterSel_invMass[iChannel]->Integral()/(h_gen_addbjets_nosel_invMass[0]->Integral()+h_gen_addbjets_nosel_invMass[1]->Integral()) << endl;
  }//Channel
  
  f_outFile->Write();
  
  gROOT->ProcessLine("setTDRStyle();");

  gStyle->SetPaintTextFormat("4.1f");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TGaxis::SetMaxDigits(3);

  TPaveText *label_cms = tdrCMSlabel();
  TCanvas *c = new TCanvas("c","c",800,800);
  
  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    auto h_tmp = (TH1D *)h_stb->h_acceptance_deltaR[iChannel]->Clone();
    h_tmp->Scale(100);
    auto h_tmp2 = (TH1D *)h_tmp->Clone();

    h_tmp->SetMaximum(h_tmp->GetMaximum()*1.05);
    h_tmp->SetMinimum(h_tmp->GetMinimum()*0.95);
    h_tmp->GetYaxis()->SetTitle("Acceptance(%)");
    h_tmp->GetYaxis()->SetTitleOffset(1.5);
    h_tmp->GetXaxis()->SetTitleOffset(1.2);
    h_tmp->SetLineWidth(2);
    h_tmp->SetLineColor(kBlue);
    h_tmp2->SetFillStyle(3013);
    h_tmp2->SetFillColor(kBlue);
    h_tmp->Draw("hist");
    h_tmp2->Draw("e2 same");
    label_cms->Draw("same");
    c->Print(Form("../output/pdf/%s.pdf", h_tmp->GetName()),"pdf");
    c->Clear();
    h_tmp->~TH1D();
    h_tmp2->~TH1D();

    h_tmp = (TH1D *)h_stb->h_acceptance_invMass[iChannel]->Clone();
    h_tmp->Scale(100);
    h_tmp2 = (TH1D *)h_tmp->Clone();

    h_tmp->SetMaximum(h_tmp->GetMaximum()*1.05);
    h_tmp->SetMinimum(h_tmp->GetMinimum()*0.95);
    h_tmp->GetYaxis()->SetTitle("Acceptance(%)");
    h_tmp->GetYaxis()->SetTitleOffset(1.5);
    h_tmp->GetXaxis()->SetTitleOffset(1.2);
    h_tmp->SetLineWidth(2);
    h_tmp->SetLineColor(kBlue);
    h_tmp2->SetFillStyle(3013);
    h_tmp2->SetFillColor(kBlue);
    h_tmp->Draw("hist");
    h_tmp2->Draw("e2 same");
    label_cms->Draw("same");
    c->Print(Form("../output/pdf/%s.pdf", h_tmp->GetName()),"pdf");
    c->Clear();
    h_tmp->~TH1D();
    h_tmp2->~TH1D();
      
    h_tmp = (TH1D *)h_stb->h_purity_deltaR[iChannel][3]->Clone();
    h_tmp->Scale(100);

    h_tmp->SetMaximum(h_tmp->GetMaximum()*1.05);
    h_tmp->SetMinimum(h_tmp->GetMinimum()*0.95);
    h_tmp->GetYaxis()->SetTitle("Purity(%)");
    h_tmp->GetYaxis()->SetTitleOffset(1.5);
    h_tmp->GetXaxis()->SetTitleOffset(1.2);
    h_tmp->SetLineWidth(2);
    h_tmp->SetLineColor(kBlue);
    h_tmp->Draw("hist");
    label_cms->Draw("same");
    c->Print(Form("../output/pdf/%s.pdf", h_tmp->GetName()),"pdf");
    c->Clear();   
    h_tmp->~TH1D();
    
    h_tmp = (TH1D *)h_stb->h_purity_invMass[iChannel][3]->Clone();
    h_tmp->Scale(100);

    h_tmp->SetMaximum(h_tmp->GetMaximum()*1.05);
    h_tmp->SetMinimum(h_tmp->GetMinimum()*0.95);
    h_tmp->GetYaxis()->SetTitle("Purity(%)");
    h_tmp->GetYaxis()->SetTitleOffset(1.5);
    h_tmp->GetXaxis()->SetTitleOffset(1.2);
    h_tmp->SetLineWidth(2);
    h_tmp->SetLineColor(kBlue);
    h_tmp->Draw("hist");
    label_cms->Draw("same");
    c->Print(Form("../output/pdf/%s.pdf", h_tmp->GetName()),"pdf");
    c->Clear();
    h_tmp->~TH1D();

    h_tmp = (TH1D *)h_stb->h_stability_deltaR[iChannel][3]->Clone();
    h_tmp->Scale(100);

    h_tmp->SetMaximum(h_tmp->GetMaximum()*1.05);
    h_tmp->SetMinimum(h_tmp->GetMinimum()*0.95);
    h_tmp->GetYaxis()->SetTitle("Stability(%)");
    h_tmp->GetYaxis()->SetTitleOffset(1.5);
    h_tmp->GetXaxis()->SetTitleOffset(1.2);
    h_tmp->SetLineWidth(2);
    h_tmp->SetLineColor(kBlue);
    h_tmp->Draw("hist");
    label_cms->Draw("same");
    c->Print(Form("../output/pdf/%s.pdf", h_tmp->GetName()),"pdf");
    c->Clear();   
    h_tmp->~TH1D();
    
    h_tmp = (TH1D *)h_stb->h_stability_invMass[iChannel][3]->Clone();
    h_tmp->Scale(100);

    h_tmp->SetMaximum(h_tmp->GetMaximum()*1.05);
    h_tmp->SetMinimum(h_tmp->GetMinimum()*0.95);
    h_tmp->GetYaxis()->SetTitle("Stability(%)");
    h_tmp->GetYaxis()->SetTitleOffset(1.5);
    h_tmp->GetXaxis()->SetTitleOffset(1.2);
    h_tmp->SetLineWidth(2);
    h_tmp->SetLineColor(kBlue);
    h_tmp->Draw("hist");
    label_cms->Draw("same");
    c->Print(Form("../output/pdf/%s.pdf", h_tmp->GetName()),"pdf");
    c->Clear();
    h_tmp->~TH1D();
  }
  f_outFile->Close();
}
