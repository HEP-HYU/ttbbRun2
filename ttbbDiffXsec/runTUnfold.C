#include <iostream>

#include "TGraph.h"
#include "TSpline.h"
#include "TUnfold.h"
#include "TUnfoldDensity.h"

void drawRegHist(const char *h_name_,
    TSpline *spl_logTauscanX_, TGraph *grp_BestLcurveX_, TGraph *grp_BestTau_, TGraph *grp_Lcurve_,
    bool scanLcurve_=true, TSpline *spl_Tauscan_=NULL){
  auto h_name = h_name_;
  auto grp_Lcurve = grp_Lcurve_;
  auto spl_logTauscanX = spl_logTauscanX_;
  auto grp_BestTau = grp_BestTau_;
  auto grp_BestLcurveX = grp_BestLcurveX_;
  auto spl_Tauscan = spl_Tauscan_;

  TCanvas *c = new TCanvas("","",800,800);
  spl_logTauscanX->SetTitle(/*log(chi^2)log(tau)*/"; log_{10}(#tau); L^{curve}_{x}");
  spl_logTauscanX->Draw();
  //spl_logTauscanX->GetXaxis()->SetLabelSize(0.1);
  //spl_logTauscanX->GetYaxis()->SetLabelSize(0.1);
  grp_BestLcurveX->SetMarkerSize(2.3);
  grp_BestLcurveX->SetMarkerColor(kBlue);
  grp_BestLcurveX->SetMarkerStyle(kFullStar);
  grp_BestLcurveX->Draw("p");
  //grp_BestLcurveX->SetEditable(false);

  c->Print(Form("../output/pdf/TUnfold_regular_LcurveX_%s.pdf",h_name),"pdf");
  c->Clear();
  c->cd();
  if(scanLcurve_){
    grp_Lcurve->SetTitle("; L^{curve}_{x}; L^{curve}");
    //grp_Lcurve->GetXaxis()->SetTitle("log(#tau)");
    grp_Lcurve->GetXaxis()->SetLabelSize(0.025);
    grp_Lcurve->GetYaxis()->SetLabelSize(0.03);
    grp_BestTau->SetMarkerSize(2.3);
    grp_BestTau->SetMarkerColor(kBlue);
    grp_BestTau->SetMarkerStyle(kFullStar);
    grp_Lcurve->Draw();
    grp_BestTau->Draw("p");
    //grp_BestTau->SetEditable(false);
    c->Print(Form("../output/pdf/TUnfold_regular_Lcurve_%s.pdf", h_name), "pdf");
  }
  else{
    spl_Tauscan->SetTitle(/*Tau scan*/"; log_{10}(#tau); avg #rho");
    spl_Tauscan->SetMarkerSize(0.7);
    grp_BestTau->SetMarkerSize(2.3);
    grp_BestTau->SetMarkerColor(kBlue);
    grp_BestTau->SetMarkerStyle(kFullStar);
    spl_Tauscan->Draw("lp");
    grp_BestTau->Draw("p");
    //grp_BestTau->SetEditable(false);
    c->Print(Form("../output/pdf/TUnfold_regular_minRho_%s.pdf", h_name), "pdf");
  } 
}

TH1 *runTUnfold(TH1 *h_in_, TH2 *h_resp_, bool scanLcurve_ = false,
    double tauMin_ = 0., double tauMax_ = 0.,
    bool fixTau_ = false, double fixedTau_ = 0.){
  auto h_in = h_in_;
  auto h_resp = h_resp_;
  auto scanLcurve = scanLcurve_;
  string regMethod;
  if(scanLcurve_){
//regMode = TUnfold::kRegModeCurvature;
    regMethod = "Scan L-Curve";
  }
  else{
//    regMode = TUnfold::kRegModeCurvature;
    regMethod = "Find minimum global correlation coefficient";
  }

  TUnfoldDensity *unfold = new TUnfoldDensity(h_resp, TUnfold::kHistMapOutputVert,
      TUnfold::kRegModeCurvature, TUnfold::kEConstraintArea, TUnfoldDensity::kDensityModeBinWidth,
      0, 0, 0, "*[UO]");

  unfold->SetInput(h_in);

  cout << "----------------------------------------" << endl;
  cout << "TUnfold version: " << TUnfold::GetTUnfoldVersion() << endl;
  cout << "Reg. Parameter optimization: " << regMethod << endl;
  cout << "----------------------------------------" << endl;


  double bestTau = fixedTau_;
  if(!fixTau_){
    const int nScan = 50;
    int iBest = 0;
    double tauMax = tauMax_, tauMin = tauMin_;
    double bestRho, bestTauX, bestTauY;

    TSpline *spl_Tauscan, *spl_logTauscanX, *spl_logTauscanY;
    TGraph *grp_Lcurve;
    TGraph *grp_BestTau, *grp_BestRho;

    if(scanLcurve){
      //ScanLcurve (Int_t nPoint, Double_t tauMin, Double_t tauMax, TGraph **lCurve, TSpline **logTauX=0, TSpline **logTauY=0, TSpline **logTauCurvature=0)
      iBest = unfold->ScanLcurve(nScan, tauMin, tauMax, &grp_Lcurve, &spl_logTauscanX, &spl_logTauscanY); 
      spl_logTauscanX->GetKnot(iBest, bestTau, bestTauX);
      spl_logTauscanY->GetKnot(iBest, bestTau, bestTauY);
      grp_BestTau = new TGraph(1, &bestTauX, &bestTauY);
    }
    else{
      //ScanTau (Int_t nPoint, Double_t tauMin, Double_t tauMax,
      //	TSpline **scanResult, Int_t mode=kEScanTauRhoAvg, const char *distribution=0, const char *projectionMode=0,
      //	TGraph **lCurvePlot=0, TSpline **logTauXPlot=0, TSpline **logTauYPlot=0)
      iBest = unfold->ScanTau(nScan, tauMin, tauMax,
	&spl_Tauscan, TUnfoldDensity::kEScanTauRhoAvg, 0, "*[UO]",
	&grp_Lcurve, &spl_logTauscanX, &spl_logTauscanY);
      spl_logTauscanX->GetKnot(iBest, bestTau, bestTauX);
      spl_logTauscanY->GetKnot(iBest, bestTau, bestTauY);
      spl_Tauscan->GetKnot(iBest, bestTau, bestRho);
      grp_BestTau = new TGraph(1, &bestTau, &bestRho);
    }
    //drawRegHist(const char *h_name_,
    //TSpline *spl_logTauscanX_, TGraph *grp_BestLcurveX_, TGraph *grp_BestTau_, TGraph *grp_Lcurve_,
    //bool scanLcurve_=false, TSpline *spl_Tauscan_=NULL
    TGraph *grp_BestLcurveX = new TGraph(1, &bestTau, &bestTauX);
    if(scanLcurve)
      drawRegHist(h_in->GetName(), spl_logTauscanX, grp_BestLcurveX, grp_BestTau, grp_Lcurve);
    else
      drawRegHist(h_in->GetName(), spl_logTauscanX, grp_BestLcurveX, grp_BestTau, grp_Lcurve,
	  false, spl_Tauscan);
  }

  if(fixTau_)
    cout << "Fixed Tau : " << bestTau << endl;
  else{
    cout << "Best log_{10}(#tau) : " << bestTau << endl;
  }
  cout << "----------------------------------------" << endl;
  
  const double unfoldResult = unfold->DoUnfold(bestTau, h_in);
  auto h_output = unfold->GetOutput(Form("Unfolded_%s",h_in->GetName()),"",0,"*[UO]",true);

  return h_output;
}
