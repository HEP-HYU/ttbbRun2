#include "../include/histBook.h"
#include "../include/tdrstyle.C"

void drawHist(TH1 *h_in, bool drawError);

string input = "../output/root/";
string output = "../output/root/";
string output_pdf = "../output/pdf/";

void makeStability(){
  //FILES
  TFile *f_ttbb = TFile::Open(Form("%shist_ttbb.root",input.c_str()));
  TFile *f_matrix = TFile::Open(Form("%shist_respMatrix_ttbb.root",input.c_str()));
  TFile *f_outFile = TFile::Open(Form("%shist_stability_ttbb.root",output.c_str()),"recreate");
  
  //NORMALIZATION
  TH1 *EventInfo = (TH1 *)f_ttbb->Get("EventInfo");
  const double scale = LUMINOSITY_*XSEC_[TTBB_]/EventInfo->GetBinContent(2);
  //const double scale = LUMINOSITY_*831.76/EventInfo->GetBinContent(2);

  //HISTOGRAMS
  HistoBook *h_stb = new HistoBook(3,NAME_[TTBB_].c_str());

  TH2D *h_respMatrix_deltaR[nChannel];
  TH2D *h_respMatrix_invMass[nChannel];

  TH1D *h_gen_addbjets_deltaR[nChannel];
  TH1D *h_gen_addbjets_invMass[nChannel];

  TH1D *h_reco_addjets_deltaR[nChannel];
  TH1D *h_reco_addjets_invMass[nChannel];

  TH1D *h_gen_addbjets_nosel_deltaR[nChannel];
  TH1D *h_gen_addbjets_nosel_invMass[nChannel];
  TH1D *h_gen_addbjets_afterSel_deltaR[nChannel];
  TH1D *h_gen_addbjets_afterSel_invMass[nChannel];

  for(int iChannel=0; iChannel<nChannel; ++iChannel){    
    h_gen_addbjets_nosel_deltaR[iChannel] = (TH1D *)f_matrix->Get(Form("h_GenbJetDeltaR_Ch%d_nosel_ttbb",iChannel));
    h_gen_addbjets_nosel_deltaR[iChannel]->Scale(scale);
    h_gen_addbjets_nosel_invMass[iChannel] = (TH1D *)f_matrix->Get(Form("h_GenbJetInvMass_Ch%d_nosel_%s",iChannel,NAME_[TTBB_].c_str()));
    h_gen_addbjets_nosel_invMass[iChannel]->Scale(scale);
    h_gen_addbjets_afterSel_deltaR[iChannel] = (TH1D *)f_matrix->Get(Form("h_GenbJetDeltaR_Ch%d_S3_%s",iChannel,NAME_[TTBB_].c_str()));
    h_gen_addbjets_afterSel_deltaR[iChannel]->Scale(scale);
    h_gen_addbjets_afterSel_invMass[iChannel] = (TH1D *)f_matrix->Get(Form("h_GenbJetInvMass_Ch%d_S3_%s",iChannel,NAME_[TTBB_].c_str()));
    h_gen_addbjets_afterSel_invMass[iChannel]->Scale(scale);

    h_respMatrix_deltaR[iChannel] = (TH2D *)f_matrix->Get(Form("h_%s_Ch%d_S3_%s",RESPONSE_MATRIX_DELTAR_,iChannel,NAME_[TTBB_].c_str()));
    h_respMatrix_deltaR[iChannel]->Scale(scale);
    h_respMatrix_invMass[iChannel] = (TH2D *)f_matrix->Get(Form("h_%s_Ch%d_S3_%s",RESPONSE_MATRIX_INVARIANT_MASS_,iChannel,NAME_[TTBB_].c_str()));
    h_respMatrix_invMass[iChannel]->Scale(scale);

    h_gen_addbjets_deltaR[iChannel] = h_respMatrix_deltaR[iChannel]->ProjectionY();
    h_gen_addbjets_invMass[iChannel] = h_respMatrix_invMass[iChannel]->ProjectionY();

    h_reco_addjets_deltaR[iChannel] = h_respMatrix_deltaR[iChannel]->ProjectionX();
    h_reco_addjets_invMass[iChannel] = h_respMatrix_invMass[iChannel]->ProjectionX();
  }
 
  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    cout << "=======================================" << endl;
    cout << "Channel : " << iChannel << endl;
    cout << "----------------------------------------" << endl;
    cout << "Variable : delta R" << endl;
    cout << "********** Purity **********" << endl;
    for(int iBin=1; iBin<h_stb->xNbins_reco_addjets_dR+1; ++iBin){
      //PURITY
      cout << iBin << "th Bin" << endl;
      double purity_dR = 0.0;
      if(h_reco_addjets_deltaR[iChannel]->GetBinContent(iBin) != 0.0){
	auto xlow = h_respMatrix_deltaR[iChannel]->GetXaxis()->GetBinLowEdge(iBin);
	auto ybin = h_respMatrix_deltaR[iChannel]->GetYaxis()->FindBin(xlow);
	purity_dR = h_respMatrix_deltaR[iChannel]->GetBinContent(iBin,ybin)/h_reco_addjets_deltaR[iChannel]->GetBinContent(iBin);
	 
	cout << "x low : " << xlow << endl;
	cout << "y bin number : " << ybin << endl;
	cout << "xBinContent : " << h_reco_addjets_deltaR[iChannel]->GetBinContent(iBin) << endl;
	cout << "yBinContent : " << h_respMatrix_deltaR[iChannel]->GetBinContent(iBin,ybin) << endl;
	cout << "value : " << purity_dR << endl;
      }
      h_stb->h_purity_deltaR[iChannel]->SetBinContent(iBin, purity_dR);
    }

    cout << "********** Stability  **********" << endl;
    for(int iBin=1; iBin<h_stb->xNbins_gen_addbjets_dR+1; ++iBin){
      //STABILITY
      cout << iBin << "th Bin" << endl;
      double stability_dR = 0.0;
      if(h_gen_addbjets_deltaR[iChannel]->GetBinContent(iBin) != 0.0){
        auto ylow = h_respMatrix_deltaR[iChannel]->GetYaxis()->GetBinLowEdge(iBin);
        auto ymax = h_respMatrix_deltaR[iChannel]->GetYaxis()->GetBinLowEdge(iBin+1);
        auto xlow = h_respMatrix_deltaR[iChannel]->GetXaxis()->FindBin(ylow);
        auto xmax = h_respMatrix_deltaR[iChannel]->GetXaxis()->FindBin(ymax);
        if(xlow == xmax && xmax == h_respMatrix_deltaR[iChannel]->GetNbinsX()) ++xmax;
        double xbincontent = 0.;
        for(int ixbin = xlow; ixbin < xmax; ++ixbin)
	  xbincontent += h_respMatrix_deltaR[iChannel]->GetBinContent(ixbin,iBin);
        stability_dR = xbincontent/h_gen_addbjets_deltaR[iChannel]->GetBinContent(iBin);
        cout << "y low : " << ylow << ", max : " << ymax << endl;
        cout << "x low : " << xlow << ", max : " << xmax << endl;
        cout << "xBinContent : " << xbincontent << endl;
        cout << "yBinContent : " << h_gen_addbjets_deltaR[iChannel]->GetBinContent(iBin) << endl;
        cout << "value :" << stability_dR << endl;
      }
      h_stb->h_stability_deltaR[iChannel]->SetBinContent(iBin, stability_dR);
    }
    cout << "----------------------------------------" << endl;
    cout << "Variable : invariant mass" << endl;
    cout << "********** Purity **********" << endl;

    for(int iBin=1; iBin<h_stb->xNbins_reco_addjets_M+1; ++iBin){
      //PURITY
      cout << iBin << "th Bin" << endl;
      double purity_M = 0.0;
      if(h_reco_addjets_invMass[iChannel]->GetBinContent(iBin) != 0.0){
        auto xlow = h_respMatrix_invMass[iChannel]->GetXaxis()->GetBinLowEdge(iBin);
        auto ybin = h_respMatrix_invMass[iChannel]->GetYaxis()->FindBin(xlow);
        purity_M = h_respMatrix_invMass[iChannel]->GetBinContent(iBin,ybin)/h_reco_addjets_invMass[iChannel]->GetBinContent(iBin);
      
        cout << "x low : " << xlow << endl;
        cout << "y bin number : " << ybin << endl;
        cout << "xBinContent : " << h_reco_addjets_invMass[iChannel]->GetBinContent(iBin) << endl;
        cout << "yBinContent : " << h_respMatrix_invMass[iChannel]->GetBinContent(iBin,ybin) << endl;
        cout << "value : " << purity_M << endl;
      }
      h_stb->h_purity_invMass[iChannel]->SetBinContent(iBin, purity_M);
    }

    cout << "********** Stability  **********" << endl;
    for(int iBin=1; iBin<h_stb->xNbins_gen_addbjets_M+1; ++iBin){
      //STABILITY
      cout << iBin << "th Bin" << endl;
      double stability_M = 0.0;
      if(h_gen_addbjets_invMass[iChannel]->GetBinContent(iBin) != 0.0){
        auto ylow = h_respMatrix_invMass[iChannel]->GetYaxis()->GetBinLowEdge(iBin);
        auto ymax = h_respMatrix_invMass[iChannel]->GetYaxis()->GetBinLowEdge(iBin+1);
        auto xlow = h_respMatrix_invMass[iChannel]->GetXaxis()->FindBin(ylow);
        auto xmax = h_respMatrix_invMass[iChannel]->GetXaxis()->FindBin(ymax);
        if(xlow == xmax && xmax == h_respMatrix_invMass[iChannel]->GetNbinsX()) ++xmax;
        double xbincontent = 0.;
        for(int ixbin = xlow; ixbin < xmax; ++ixbin)
	  xbincontent += h_respMatrix_invMass[iChannel]->GetBinContent(ixbin,iBin);
        stability_M = xbincontent/h_gen_addbjets_invMass[iChannel]->GetBinContent(iBin);
      
        cout << "y low : " << ylow << ", max : " << ymax << endl;
        cout << "x low : " << xlow << ", max : " << xmax << endl;
        cout << "xBinContent : " << xbincontent << endl;
        cout << "yBinContent : " << h_gen_addbjets_invMass[iChannel]->GetBinContent(iBin) << endl;
        cout << "value :" << stability_M << endl;
      }
      h_stb->h_stability_invMass[iChannel]->SetBinContent(iBin, stability_M);
    }
    cout << "----------------------------------------" << endl;

    //ACCEPTANCE
    for(int iBin=1; iBin<h_stb->xNbins_gen_addbjets_dR+1;++iBin){
      double acceptance_dR = 0.0;
      double dR_err = 0.0;
      if( h_gen_addbjets_nosel_deltaR[iChannel]->GetBinContent(iBin) != 0.0 ){
        acceptance_dR = h_gen_addbjets_afterSel_deltaR[iChannel]->GetBinContent(iBin)/(h_gen_addbjets_nosel_deltaR[0]->GetBinContent(iBin)+h_gen_addbjets_nosel_deltaR[1]->GetBinContent(iBin));
	dR_err = abs(acceptance_dR)*sqrt(pow(h_gen_addbjets_afterSel_deltaR[iChannel]->GetBinError(iBin)/h_gen_addbjets_afterSel_deltaR[iChannel]->GetBinContent(iBin),2)
	    +pow(h_gen_addbjets_nosel_deltaR[0]->GetBinError(iBin)/h_gen_addbjets_nosel_deltaR[0]->GetBinContent(iBin),2)+pow(h_gen_addbjets_nosel_deltaR[0]->GetBinError(iBin)/h_gen_addbjets_nosel_deltaR[0]->GetBinContent(iBin),2));
      }
      h_stb->h_acceptance_deltaR[iChannel]->SetBinContent(iBin, acceptance_dR);
      h_stb->h_acceptance_deltaR[iChannel]->SetBinError(iBin, dR_err);
    }
    
    for(int iBin=1; iBin<h_stb->xNbins_gen_addbjets_M+1;++iBin){
      double acceptance_M = 0.0;
      double M_err = 0.0;
      if( h_gen_addbjets_nosel_invMass[iChannel]->GetBinContent(iBin) != 0.0 ){
        acceptance_M = h_gen_addbjets_afterSel_invMass[iChannel]->GetBinContent(iBin)/(h_gen_addbjets_nosel_invMass[0]->GetBinContent(iBin)+h_gen_addbjets_nosel_invMass[1]->GetBinContent(iBin));
	M_err = abs(acceptance_M)*sqrt(pow(h_gen_addbjets_afterSel_invMass[iChannel]->GetBinError(iBin)/h_gen_addbjets_afterSel_invMass[iChannel]->GetBinContent(iBin),2)
	    +pow(h_gen_addbjets_nosel_invMass[0]->GetBinError(iBin)/h_gen_addbjets_nosel_invMass[0]->GetBinContent(iBin),2)+pow(h_gen_addbjets_nosel_invMass[1]->GetBinError(iBin)/h_gen_addbjets_nosel_invMass[1]->GetBinContent(iBin),2));
      }
      h_stb->h_acceptance_invMass[iChannel]->SetBinContent(iBin, acceptance_M);
      h_stb->h_acceptance_invMass[iChannel]->SetBinError(iBin, M_err);
    }
  }//Channel
  f_outFile->Write();
  
  gROOT->ProcessLine("setTDRStyle();");

  gStyle->SetPaintTextFormat("4.1f");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TGaxis::SetMaxDigits(3);

  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    auto h_tmp = (TH1D *)h_stb->h_acceptance_deltaR[iChannel]->Clone();
    h_tmp->Scale(100);
    h_tmp->GetYaxis()->SetTitle("Acceptance(%)");
    drawHist(h_tmp, true);
    h_tmp->~TH1D();

    h_tmp = (TH1D *)h_stb->h_acceptance_invMass[iChannel]->Clone();
    h_tmp->Scale(100);
    h_tmp->GetYaxis()->SetTitle("Acceptance(%)");
    drawHist(h_tmp, true);
    h_tmp->~TH1D();
      
    h_tmp = (TH1D *)h_stb->h_purity_deltaR[iChannel]->Clone();
    h_tmp->Scale(100);
    h_tmp->GetYaxis()->SetTitle("Purity(%)");
    drawHist(h_tmp, false);
    h_tmp->~TH1D();
    
    h_tmp = (TH1D *)h_stb->h_purity_invMass[iChannel]->Clone();
    h_tmp->Scale(100);
    h_tmp->GetYaxis()->SetTitle("Purity(%)");
    drawHist(h_tmp, false);
    h_tmp->~TH1D();

    h_tmp = (TH1D *)h_stb->h_stability_deltaR[iChannel]->Clone();
    h_tmp->Scale(100);
    h_tmp->GetYaxis()->SetTitle("Stability(%)");
    drawHist(h_tmp, false);
    h_tmp->~TH1D();
    
    h_tmp = (TH1D *)h_stb->h_stability_invMass[iChannel]->Clone();
    h_tmp->Scale(100);
    h_tmp->GetYaxis()->SetTitle("Stability(%)");
    drawHist(h_tmp, false);
    h_tmp->~TH1D();
  }
  f_outFile->Close();
}

void drawHist(TH1* h_in_, bool drawError){
  auto h_in = (TH1 *)h_in_->Clone();
  auto h_err = (TH1 *)h_in->Clone();

  TCanvas *c = new TCanvas("","",800,800);
  TPaveText *label_cms = tdrCMSlabel();

  h_in->SetMaximum(h_in->GetMaximum()*1.05);
  h_in->GetYaxis()->SetTitleOffset(1.5);
  h_in->GetXaxis()->SetTitleOffset(1.2);
  h_in->SetLineWidth(2);
  h_in->SetLineColor(kBlue);
  h_in->Draw("hist");
  if(drawError){
    h_err->SetFillStyle(3013);
    h_err->SetFillColor(kBlue);
    h_err->Draw("e2 same");
  }

  label_cms->Draw("same");
  c->Print(Form("%s/%s.pdf", output_pdf.c_str(), h_in->GetName()),"pdf");

  c->~TCanvas();
  h_in->~TH1();
}
