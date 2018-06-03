#include <iostream>

#include "TGraph.h"
#include "TSpline.h"
#include "TUnfold.h"
#include "TUnfoldDensity.h"

void drawLCurve(const char *h_name_, TGraph *grp_Lcurve_, TGraph *grp_BestLcurve_,  TSpline *spl_logTauscanX_, TGraph *grp_BestLogTauLogChi2_){
  auto h_name = h_name_;
  auto grp_Lcurve = grp_Lcurve_;
  auto spl_logTauscanX = spl_logTauscanX_;
  auto grp_BestLcurve = grp_BestLcurve_;
  auto grp_BestLogTauLogChi2 = grp_BestLogTauLogChi2_;

  TCanvas *c = new TCanvas("","",800,800);
  
  spl_logTauscanX->SetTitle("log(chi^2)log(tau)");
  spl_logTauscanX->Draw();
  grp_BestLogTauLogChi2->SetMarkerSize(2.3);
  grp_BestLogTauLogChi2->SetMarkerColor(kBlue);
  grp_BestLogTauLogChi2->SetMarkerStyle(kFullStar);
  grp_BestLogTauLogChi2->Draw("p");
  grp_BestLogTauLogChi2->SetEditable(false);

  c->Print(Form("result/TUnfold_regular_logTaulogChi2_%s.pdf",h_name),"pdf");
  c->Clear();
  c->cd();

  grp_Lcurve->Draw();
  grp_Lcurve->GetXaxis()->SetTitle("log(#tau)");
  grp_BestLcurve->SetMarkerSize(2.3);
  grp_BestLcurve->SetMarkerColor(kBlue);
  grp_BestLcurve->SetMarkerStyle(kFullStar);
  grp_BestLcurve->Draw("p");
  grp_BestLcurve->SetEditable(false);

  c->Print(Form("result/TUnfold_regular_lCurve_%s.pdf",h_name),"pdf");
}

void drawTauRho(const char *h_name_, TSpline *spl_Tauscan_, TGraph *grp_BestRho_){
  auto h_name = h_name_;
  auto spl_Tauscan = spl_Tauscan_;
  auto grp_BestRho = grp_BestRho_;

  TCanvas *c = new TCanvas("","",800,800);
  c->cd();
 
  spl_Tauscan->SetTitle("Tau scan; log_{10}(#tau); avg #rho");
  spl_Tauscan->SetMarkerSize(0.7);
  spl_Tauscan->Draw("lp");
  grp_BestRho->SetMarkerSize(2.3);
  grp_BestRho->SetMarkerColor(kBlue);
  grp_BestRho->SetMarkerStyle(kFullStar);
  grp_BestRho->Draw("p");
  grp_BestRho->SetEditable(false);

  c->Print(Form("result/TUnfold_regular_%s.pdf",h_name),"pdf");
}


TH1 *runTUnfold(TH1 *h_in_, TH2 *h_resp_, bool fixTau_, double tauMin_ = 0., double tauMax_ = 0., double fixedTau_ = 0.){
  auto h_in = h_in_;
  auto h_resp = h_resp_;
  TUnfoldDensity *unfold = new TUnfoldDensity(h_resp, TUnfold::kHistMapOutputVert,
      TUnfold::kRegModeCurvature, TUnfold::kEConstraintArea, TUnfoldDensity::kDensityModeBinWidth,
      0, 0, 0, "*[UO]");
  unfold->SetInput(h_in);

  cout << "----------------------------------------" << endl;
  cout << "TUnfold version : " << TUnfold::GetTUnfoldVersion() << endl;
 
  double bestTau = fixedTau_;
  if(!fixTau_){
    const int nScan = 100;
    int iBest = 0;
    double tauMax = tauMax_, tauMin = tauMin_;
    double bestRho;

    TSpline *spl_Tauscan, *spl_TauscanX, *spl_TauscanY;
    TGraph *grp_TauscanL;

    iBest = unfold->ScanTau(nScan, tauMin, tauMax,
	&spl_Tauscan, TUnfoldDensity::kEScanTauRhoAvg, 0, "*[UO]",
	&grp_TauscanL, &spl_TauscanX, &spl_TauscanY);

    spl_Tauscan->GetKnot(iBest, bestTau, bestRho);
    TGraph *grp_BestRho = new TGraph(1, &bestTau, &bestRho);
    drawTauRho(h_in->GetName(), spl_Tauscan, grp_BestRho);
    
    cout << "Best Tau : " << bestTau << endl;
    cout << "Best Rho : " << bestRho << endl;
  }
  else cout << "Fixed Tau : " << bestTau << endl;
  cout << "----------------------------------------" << endl;
  
  const double unfoldResult = unfold->DoUnfold(bestTau, h_in);
  auto h_output = unfold->GetOutput(Form("Unfolded_%s",h_in->GetName()),"",0,"*[UO]",true);

  return h_output;
}

TH1 *runTUnfold(TH1 *h_in_, TH2 *h_resp_, double tauMin_ = 0., double tauMax_ = 0.){
  auto h_in = h_in_;
  auto h_resp = h_resp_;
  auto tauMin = tauMin_, tauMax = tauMax_;

  TUnfoldDensity *unfold = new TUnfoldDensity(h_resp, TUnfold::kHistMapOutputVert,
      TUnfold::kRegModeSize, TUnfold::kEConstraintArea, TUnfoldDensity::kDensityModeBinWidth,
      0, 0, 0, "*[UO]");
  unfold->SetInput(h_in);

  cout << "----------------------------------------" << endl;
  cout << "TUnfold version : " << TUnfold::GetTUnfoldVersion() << endl;

  int nScan = 50;
  double bestTau, bestTauX, bestTauY;
  int iBest = 0;

  TSpline *spl_Tauscan, *spl_logTauscanX, *spl_logTauscanY;
  TGraph *grp_Lcurve;

  iBest = unfold->ScanLcurve(nScan, tauMin, tauMax, &grp_Lcurve, &spl_logTauscanX, &spl_logTauscanY);

  cout << "chi^2 = " << unfold->GetChi2A() << " + " << unfold->GetChi2L() << "/" << unfold->GetNdf() << endl;

  spl_logTauscanX->GetKnot(iBest, bestTau, bestTauX);
  spl_logTauscanY->GetKnot(iBest, bestTau, bestTauY);
  TGraph *grp_BestLcurve = new TGraph(1, &bestTauX, &bestTauY);
  TGraph *grp_BestLogTauLogChi2 = new TGraph(1, &bestTau, &bestTauX);

  const double unfoldResult = unfold->DoUnfold(bestTau, h_in);
  auto h_output = unfold->GetOutput(Form("Unfolded_%s", h_in->GetName()),"",0,"*[UO]",true);

  drawLCurve(h_in->GetName(), grp_Lcurve, grp_BestLcurve, spl_logTauscanX, grp_BestLogTauLogChi2);
  
  return h_output;
}
