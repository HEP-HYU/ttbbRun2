#include "../include/histBook.h"
#include "../include/tdrstyle.C"

TFile *MakeRoot(string name);
void DrawHist(TH1 *h_in, std::string name, bool drawError);

string input = "../output/root/";
string output = "../output/root/";
string output_pdf = "../output/pdf/";

void makeCriteria(){
  std::vector<std::string> v_name = {
    "TTLJ_PowhegPythia", "TT_PowhegHerwig", "TT_PowhegPythiaEvtgen", "TT_aMCatNLOPythia",
    "TT_PowhegPythia_SYS_FSRup", "TT_PowhegPythia_SYS_FSRdown", "TT_PowhegPythia_SYS_ISRup", "TT_PowhegPythia_SYS_ISRdown"
  };

  for(int i=0; i < v_name.size(); ++i){
    auto f_tmp = MakeRoot(v_name.at(i));
    TIter next(f_tmp->GetListOfKeys());
    TKey *key;
    TObject *obj;
    while((key = (TKey *)next())){
      obj = key->ReadObj();
      string tmp = obj->GetName();
      ssize_t pos;
      if((pos = tmp.find("Bin",0,3)) == std::string::npos)
	  continue;
      auto h_tmp = (TH1 *)obj;
      if((pos = tmp.find("Acc",0,3)) != std::string::npos)
	DrawHist(h_tmp, v_name.at(i), true);
      else
        DrawHist(h_tmp, v_name.at(i), false);
    }
  }
}

TFile *MakeRoot(string name_){
  auto name = name_;
  cout << "SAMPLE: " << name << endl;

  TFile *f_in = TFile::Open(Form("%s/hist_%s_ttbb.root", input.c_str(), name.c_str()));
  TFile *f_out = TFile::Open(Form("%s/hist_criteria_%s_ttbb.root",output.c_str(),name.c_str()),"recreate");

  //NORMALIZATION
  TH1 *EventInfo = (TH1 *)f_in->Get("EventInfo");
  double scale;
  ssize_t pos;
  if((pos = name.find("TTLJ",0,4)) != std::string::npos)
    scale = LUMINOSITY_*XSEC_[TTBB_]/EventInfo->GetBinContent(2);
  else 
    scale = LUMINOSITY_*831.76/EventInfo->GetBinContent(2);

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
    h_gen_addbjets_nosel_deltaR[iChannel] = (TH1D *)f_in->Get(Form("h_%s_Ch%d_nosel_ttbb",
	  GEN_ADDBJETS_DELTAR_, iChannel));
    h_gen_addbjets_nosel_deltaR[iChannel]->Scale(scale);
    h_gen_addbjets_nosel_invMass[iChannel] = (TH1D *)f_in->Get(Form("h_%s_Ch%d_nosel_ttbb",
	  GEN_ADDBJETS_INVARIANT_MASS_, iChannel));
    h_gen_addbjets_nosel_invMass[iChannel]->Scale(scale);
    h_gen_addbjets_afterSel_deltaR[iChannel] = (TH1D *)f_in->Get(Form("h_%s_Ch%d_S3_ttbb",
	  GEN_ADDBJETS_DELTAR_, iChannel));
    h_gen_addbjets_afterSel_deltaR[iChannel]->Scale(scale);
    h_gen_addbjets_afterSel_invMass[iChannel] = (TH1D *)f_in->Get(Form("h_%s_Ch%d_S3_ttbb",
	  GEN_ADDBJETS_INVARIANT_MASS_, iChannel));
    h_gen_addbjets_afterSel_invMass[iChannel]->Scale(scale);
    
    h_respMatrix_deltaR[iChannel] = (TH2D *)f_in->Get(Form("h_%s_Ch%d_S3_ttbb",
	  RESPONSE_MATRIX_DELTAR_,iChannel));
    h_respMatrix_deltaR[iChannel]->Scale(scale);
    h_respMatrix_invMass[iChannel] = (TH2D *)f_in->Get(Form("h_%s_Ch%d_S3_ttbb",
	  RESPONSE_MATRIX_INVARIANT_MASS_,iChannel));
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
  f_out->Write();

  return f_out;
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
  c->Print(Form("%s/%s_%s.pdf", output_pdf.c_str(), h_in->GetName(), name.c_str()),"pdf");

  c->~TCanvas();
  h_in->~TH1();
}
