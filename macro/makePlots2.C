/*****************************************************************************************************************************
 * last update : 2017-08-02                                                                                                  *
 * Developer : San                                                                                                           *
 * Convert histograms as pdf file											     *
 * use example : root -l -b -q simplePlotmaker.C'("~/work/ttbb/170802/hist_resp_noselection_ttbb.root","~/work/ttbb/170802")'*
 *****************************************************************************************************************************/

#include <TFile.h>
#include <TObject.h>
#include <TKey.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TCanvas.h>

#include "../include/tdrstyle.C"

void makePlots2(string inputFile, string outputLocation){
  //gROOT->ProcessLine(".L ../include/tdrstyle.C");
  gROOT->ProcessLine("setTDRStyle();");
  
  gStyle->SetPaintTextFormat("4.1f");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetErrorX(00);
  TGaxis::SetMaxDigits(3);

  TPaveText *label_cms = tdrCMSlabel();
  
  TFile *file = TFile::Open(inputFile.c_str());

  TCanvas *c = new TCanvas("c","c",800,800);

  TIter next(file->GetListOfKeys());
  TKey *key;
  TObject *obj;
  while((key = (TKey*)next())){
    obj = key->ReadObj();
    if( obj->InheritsFrom(TH1D::Class())){
      auto h = (TH1D *)obj;
      auto e = (TH1D *)h->Clone();
      h->SetLineWidth(2);
      h->SetLineColor(kBlue);
      e->SetFillStyle(3013);
      e->SetFillColor(kBlue);
      h->Draw("hist");
      e->Draw("e2 same");
      h->GetYaxis()->SetTitleOffset(1.5);
      h->GetXaxis()->SetTitleOffset(1.2);
      label_cms->Draw("same");
      c->Print(Form("%s/%s.pdf",outputLocation.c_str(), obj->GetName()),"pdf");
    }
    else if( obj->InheritsFrom(TH2D::Class())){
      auto h = (TH2D *) obj;
      h->Draw("box");
      c->Print(Form("%s/%s.pdf",outputLocation.c_str(), obj->GetName()),"pdf");
    }
    else{
      auto h = (TH3D *) obj;
      h->Draw();
    }
  }
}
