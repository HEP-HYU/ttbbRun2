#include "../include/histBook.h"
#include "../include/tdrstyle.C"

HistoBook *MakeHist(TFile *f_in, std::string syst);
void DrawHist(TH1 *h_in, std::string name, bool drawError);

void makeCriteria(){
  string input_dir = "../output/post/";
  TFile *f_in = TFile::Open(Form("%s/hist_ttbb.root", input_dir.c_str()));
  TFile *f_out = TFile::Open(Form("%s/hist_criteria.root", input_dir.c_str()),"recreate");

  std::vector<std::string> v_syst_list = {
    "", "__jerup", "__jerdown", "__jecup", "__jecdown",
    "__puup", "__pudown",
    "__musfup", "__musfdown", "__mutrgup", "__mutrgdown",
    "__elsfup", "__elsfdown", "__eltrgup", "__eltrgdown",
    "__lfup", "__lfdown", "__hfup", "__hfdown",
    "__hfstat1up", "__hfstat1down", "__hfstat2up", "__hfstat2down",
    "__lfstat1up", "__lfstat1down", "__lfstat2up", "__lfstat2down",
    "__cferr1up", "__cferr1down", "__cferr2up", "__cferr2down",
    "__psup", "__psdown", "__swup", "__swdown",
    "__hdampup", "__hdampdown", "__tuneup", "__tunedown"
  };

  f_out->cd();
  for(auto v_itr = v_syst_list.begin(); v_itr != v_syst_list.end(); ++v_itr){
    auto h_tmp = MakeHist(f_in, *v_itr);
    
    for(int ich=0; ich<2; ++ich){
      h_tmp->h_stability_deltaR[ich]->Write();
      h_tmp->h_stability_invMass[ich]->Write();
      h_tmp->h_purity_deltaR[ich]->Write();
      h_tmp->h_purity_invMass[ich]->Write();
      h_tmp->h_acceptance_deltaR[ich]->Write();
      h_tmp->h_acceptance_invMass[ich]->Write();

    }
    h_tmp->~HistoBook();
  }
  f_out->Write();
  f_out->Close();
}

HistoBook *MakeHist(TFile *f_in, string syst){
  cout << "syst: " << syst << endl;
  double xsec = 0.0;
  if( syst.find("sw",0,2) != std::string::npos ) xsec = 831.76;
  else if( syst.find("tune",0,4) != std::string::npos ) xsec = 831.76;
  else xsec = XSEC_[TTBB_];

  //NORMALIZATION
  TH1 *EventInfo = (TH1 *)f_in->Get("EventInfo");
  const double SF_ttbb = 0.992;
  const double scale = SF_ttbb*LUMINOSITY_*xsec/EventInfo->GetBinContent(2);

  //HISTOGRAMS
  HistoBook *h_stb = new HistoBook(3,syst.c_str());

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
    h_gen_addbjets_nosel_deltaR[iChannel] = (TH1D *)f_in->Get(Form("h_%s_Ch%d_nosel%s",
	  GEN_ADDBJETS_DELTAR_, iChannel, syst.c_str()));
    h_gen_addbjets_nosel_deltaR[iChannel]->Scale(scale);
    h_gen_addbjets_nosel_invMass[iChannel] = (TH1D *)f_in->Get(Form("h_%s_Ch%d_nosel%s",
	  GEN_ADDBJETS_INVARIANT_MASS_, iChannel, syst.c_str()));
    h_gen_addbjets_nosel_invMass[iChannel]->Scale(scale);
    h_gen_addbjets_afterSel_deltaR[iChannel] = (TH1D *)f_in->Get(Form("h_%s_Ch%d_S3%s",
	  GEN_ADDBJETS_DELTAR_, iChannel, syst.c_str()));
    h_gen_addbjets_afterSel_deltaR[iChannel]->Scale(scale);
    h_gen_addbjets_afterSel_invMass[iChannel] = (TH1D *)f_in->Get(Form("h_%s_Ch%d_S3%s",
	  GEN_ADDBJETS_INVARIANT_MASS_, iChannel, syst.c_str()));
    h_gen_addbjets_afterSel_invMass[iChannel]->Scale(scale);
    
    h_respMatrix_deltaR[iChannel] = (TH2D *)f_in->Get(Form("h_%s_Ch%d_S3%s",
	  RESPONSE_MATRIX_DELTAR_,iChannel, syst.c_str()));
    h_respMatrix_deltaR[iChannel]->Scale(scale);
    h_respMatrix_invMass[iChannel] = (TH2D *)f_in->Get(Form("h_%s_Ch%d_S3%s",
	  RESPONSE_MATRIX_INVARIANT_MASS_,iChannel, syst.c_str()));
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
    cout << "----------------------------------------" << endl;
    cout << "********** Acceptance  **********" << endl;
    cout << "Variable : delta R" << endl;
    for(int iBin=1; iBin<h_stb->xNbins_gen_addbjets_dR+1;++iBin){
      cout << iBin << "th Bin" << endl;
      double acceptance_dR = 0.0;
      double dR_err = 0.0;
      if( h_gen_addbjets_nosel_deltaR[iChannel]->GetBinContent(iBin) != 0.0 ){
	acceptance_dR = h_gen_addbjets_afterSel_deltaR[iChannel]->GetBinContent(iBin)/(h_gen_addbjets_nosel_deltaR[0]->GetBinContent(iBin)+h_gen_addbjets_nosel_deltaR[1]->GetBinContent(iBin));
	dR_err = abs(acceptance_dR)*sqrt(pow(h_gen_addbjets_afterSel_deltaR[iChannel]->GetBinError(iBin)/h_gen_addbjets_afterSel_deltaR[iChannel]->GetBinContent(iBin),2)
	    +pow(h_gen_addbjets_nosel_deltaR[0]->GetBinError(iBin)/h_gen_addbjets_nosel_deltaR[0]->GetBinContent(iBin),2)+pow(h_gen_addbjets_nosel_deltaR[0]->GetBinError(iBin)/h_gen_addbjets_nosel_deltaR[0]->GetBinContent(iBin),2));
      }
      h_stb->h_acceptance_deltaR[iChannel]->SetBinContent(iBin, acceptance_dR);
      h_stb->h_acceptance_deltaR[iChannel]->SetBinError(iBin, dR_err);
      cout << "value :" << acceptance_dR << endl;
      cout << "err :" << dR_err << endl;
    }
    
    cout << "Variable : invariant mass" << endl;
    for(int iBin=1; iBin<h_stb->xNbins_gen_addbjets_M+1;++iBin){
      cout << iBin << "th Bin" << endl;
      double acceptance_M = 0.0;
      double M_err = 0.0;
      if( h_gen_addbjets_nosel_invMass[iChannel]->GetBinContent(iBin) != 0.0 ){
	acceptance_M = h_gen_addbjets_afterSel_invMass[iChannel]->GetBinContent(iBin)/(h_gen_addbjets_nosel_invMass[0]->GetBinContent(iBin)+h_gen_addbjets_nosel_invMass[1]->GetBinContent(iBin));
	M_err = abs(acceptance_M)*sqrt(pow(h_gen_addbjets_afterSel_invMass[iChannel]->GetBinError(iBin)/h_gen_addbjets_afterSel_invMass[iChannel]->GetBinContent(iBin),2)
	    +pow(h_gen_addbjets_nosel_invMass[0]->GetBinError(iBin)/h_gen_addbjets_nosel_invMass[0]->GetBinContent(iBin),2)+pow(h_gen_addbjets_nosel_invMass[1]->GetBinError(iBin)/h_gen_addbjets_nosel_invMass[1]->GetBinContent(iBin),2));
      }
      h_stb->h_acceptance_invMass[iChannel]->SetBinContent(iBin, acceptance_M);
      h_stb->h_acceptance_invMass[iChannel]->SetBinError(iBin, M_err);
      cout << "value :" << acceptance_M << endl;
      cout << "err :" << M_err << endl;

    }
  }//Channel

  return h_stb;
}

void DrawHist(TH1* h_in_, std::string name, bool drawError){    
  gROOT->ProcessLine("setTDRStyle();");

  gStyle->SetHatchesSpacing(1);
  gStyle->SetHatchesLineWidth(1);

  gStyle->SetPaintTextFormat("4.1f");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TGaxis::SetMaxDigits(3);

  gStyle->SetHatchesSpacing(1);
  gStyle->SetHatchesLineWidth(1);

  auto h_in = (TH1 *)h_in_->Clone();
  h_in->Scale(100);
  auto h_err = (TH1 *)h_in->Clone();
  string histname = h_in->GetName();

  TCanvas *c = new TCanvas("","",800,800);
  TPaveText *label_sim = tdrCMSSimlabel();
  TPaveText *label_work = tdrWorkProgress();

  ssize_t pos;
  h_in->SetMaximum(h_in->GetMaximum()*1.05);
  if((pos = histname.find("AcceptanceDeltaR")) != string::npos){
    h_in->GetYaxis()->SetTitle("Acceptance(%)");
    h_in->SetMaximum(1.2);
    h_in->SetMinimum(0.6);
  }
  if((pos = histname.find("AcceptanceInvMass")) != string::npos){
    h_in->GetYaxis()->SetTitle("Acceptance(%)");
    h_in->SetMaximum(2.0);
    h_in->SetMinimum(0.0);
  }
  if((pos = histname.find("Purity")) != string::npos){
    h_in->GetYaxis()->SetTitle("Purity(%)");
    h_in->SetMaximum(80);
    h_in->SetMinimum(30);
  }
  if((pos = histname.find("Stability")) != string::npos){
    h_in->GetYaxis()->SetTitle("Stability(%)");
    h_in->SetMaximum(80);
    h_in->SetMinimum(0);
  }
  
  h_in->GetYaxis()->SetTitleOffset(1.3);
  h_in->GetXaxis()->SetTitleOffset(1.2);
  h_in->SetLineWidth(2);
  h_in->SetLineColor(kBlue);
  h_in->Draw("hist");
  if(drawError){
    h_err->SetFillStyle(3154);
    h_err->SetFillColor(kBlack);
    h_err->Draw("e2 same");
  }

  label_work->Draw("same");
  label_sim->Draw("same");
  c->Print(Form("../output/pdf/%s_%s.pdf", h_in->GetName(), name.c_str()),"pdf");

  c->~TCanvas();
  h_in->~TH1();
}
