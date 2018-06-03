#include <fstream>
#include <iostream>
#include <map>
#include <limits>

#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TLegend.h>
#include <TPaveText.h>


#include "../include/makePlots.h"

string INPUT = "../output/root/";
string OUTPUT = "../output/pdf/";

//const double SF_ttbb = 0.992;
//const double SF_ttbj = 0.997;
//const double SF_ttcc = 0.871;
//const double SF_ttLF = 0.890;

DataFile::DataFile(string input_data){
  string fileName = INPUT + "hist_" + input_data + ".root";
  this->file = TFile::Open(fileName.c_str());
  this->data = input_data;

  int tmp = 0;
  TIter next(file->GetListOfKeys());
  TKey *key;
  TObject *obj;
  while((key = (TKey*)next())){
    obj = key->ReadObj();
    ++tmp;
    string tmp = obj->GetName();
    ReplaceAll(tmp, this->data, "");
    this->v_histName.push_back(tmp);
  }
  this->nHist = tmp;
}

MonteFile::MonteFile(string input_sample, string input_category, int input_color, double xsec){
  string fileName = INPUT + "hist_" + input_sample + ".root";
  this->file = TFile::Open(fileName.c_str());
  this->color = input_color;
  this->sample = input_sample;
  this->category = input_category;

  int tmp = 0;
  TIter next(file->GetListOfKeys());
  TKey *key;
  TObject *obj;
  while((key = (TKey*)next())){
    obj = key->ReadObj();
    ++tmp;
    string tmp = obj->GetName();
    ReplaceAll(tmp, this->sample, "");
    this->v_histName.push_back(tmp);
  }
  this->nHist = tmp;
  TH1D *EventInfo = (TH1D *)this->file->Get("EventInfo");

  this->scale_Mu = (LUMINOSITY_*xsec)/EventInfo->GetBinContent(2);
  this->scale_El = (LUMINOSITY_*xsec)/EventInfo->GetBinContent(2);

  if(this->sample == "ttbb"){
    this->scale_Mu *= SF_ttbb;
    this->scale_El *= SF_ttbb;
  }
  if(this->sample == "ttbj"){
    this->scale_Mu *= SF_ttbj;
    this->scale_El *= SF_ttbj;
  }
  if(this->sample == "ttcc"){
    this->scale_Mu *= SF_ttcc;
    this->scale_El *= SF_ttcc;
  }
  if(this->sample == "ttLF"){
    this->scale_Mu *= SF_ttLF;
    this->scale_El *= SF_ttLF;
  }
}

void makePlots(){
  ofstream outTxt("../output/nEvent.txt");

  std::map<string, DataFile *> m_Data;
  std::vector<MonteFile *> v_Monte;

  //sample List
  m_Data["Mu"] = new DataFile("DataSingleMu");
  m_Data["El"] = new DataFile("DataSingleEl");
  v_Monte.push_back(new MonteFile(NAME_[TTBB_],"t#bar{t}+bb", kRed+4, XSEC_[TTBB_]));
  v_Monte.push_back(new MonteFile(NAME_[TTBJ_], "t#bar{t}+b", kRed+3, XSEC_[TTBJ_]));
  v_Monte.push_back(new MonteFile(NAME_[TTCC_], "t#bar{t}+cc", kRed+2, XSEC_[TTCC_]));
  //v_Monte.push_back(new MonteFile(NAME_[TTJJ_], "ttjj", kRed+1, XSEC_[TTJJ_]));
  v_Monte.push_back(new MonteFile(NAME_[TTLF_], "t#bar{t}+LF", kRed, XSEC_[TTLF_]));
  v_Monte.push_back(new MonteFile(NAME_[TTBKG_], "t#bar{t}+other", kPink, XSEC_[TTBKG_]));
  v_Monte.push_back(new MonteFile(NAME_[WJETS_], "W+Jets", kYellow, XSEC_[WJETS_]));
  v_Monte.push_back(new MonteFile(NAME_[ZJETS_], "Z+Jets", kBlue, XSEC_[ZJETS_]));
  v_Monte.push_back(new MonteFile(NAME_[ZJETS10TO50_], "Z+Jets", kBlue, XSEC_[ZJETS10TO50_]));
  v_Monte.push_back(new MonteFile(NAME_[TCHANNEL_], "Single t", 6, XSEC_[TCHANNEL_]));
  v_Monte.push_back(new MonteFile(NAME_[TBARCHANNEL_], "Single t", 6, XSEC_[TBARCHANNEL_]));
  v_Monte.push_back(new MonteFile(NAME_[TWCHANNEL_], "Single t", 6, XSEC_[TWCHANNEL_]));
  v_Monte.push_back(new MonteFile(NAME_[TBARWCHANNEL_], "Single t", 6, XSEC_[TBARWCHANNEL_]));
  v_Monte.push_back(new MonteFile(NAME_[WW_], "DiBoson", kCyan, XSEC_[WW_]));
  v_Monte.push_back(new MonteFile(NAME_[WZ_], "DiBoson", kCyan, XSEC_[WZ_]));
  v_Monte.push_back(new MonteFile(NAME_[ZZ_], "DiBoson", kCyan, XSEC_[ZZ_]));
  v_Monte.push_back(new MonteFile(NAME_[TTH_], "t#bar{t}+H", kViolet, XSEC_[TTH_]));
  v_Monte.push_back(new MonteFile(NAME_[TTW_], "t#bar{t}+V", kGreen, XSEC_[TTW_]));
  v_Monte.push_back(new MonteFile(NAME_[TTZ_], "t#bar{t}+V", kGreen, XSEC_[TTZ_]));

  int nHist=0;
  for(auto v_itr = v_Monte[TTBB_]->v_histName.begin(); v_itr != v_Monte[TTBB_]->v_histName.end(); ++v_itr){
    cout << "HIST NAME : " << *v_itr << endl;
    outTxt << "HIST NAME : " << *v_itr << endl;
    if( *v_itr == "EventInfo" ) continue;
    std::vector<string> v_postSample;
    v_postSample.push_back("default");

    //Histogram form
    TCanvas *canvas = new TCanvas("c","canvas",800,800);
    
    THStack *hs_tmp = new THStack("st","stack");
   
    TPaveText *label = new TPaveText();
    label->SetX1NDC(0.52);
    label->SetX2NDC(0.94);
    label->SetY1NDC(0.89);
    label->SetY2NDC(0.95);
    label->SetTextFont(42);
    label->AddText("CMS, 35.9 fb^{-1} at #sqrt{s} = 13TeV");
    label->SetFillStyle(0);
    label->SetBorderSize(0);
    label->SetTextSize(0.04);

    TPaveText *label2 = new TPaveText();
    label2->SetX1NDC(0.07);
    label2->SetX2NDC(0.5);
    label2->SetY1NDC(0.89);
    label2->SetY2NDC(0.95);
    label2->SetTextFont(42);
    label2->SetTextColor(kRed);
    label2->AddText("Work in progress");
    label2->SetFillStyle(0);
    label2->SetBorderSize(0);
    label2->SetTextSize(0.06);

    TPaveText *label3 = new TPaveText();
    label3->SetX1NDC(0.13);
    label3->SetX2NDC(0.35);
    label3->SetY1NDC(0.85);
    label3->SetY2NDC(0.80);
    label3->SetTextFont(42);
    label3->SetFillStyle(0);
    label3->SetBorderSize(0);
    label3->SetTextSize(0.05);

    TLegend *leg = new TLegend(0.40, 0.70, 0.89, 0.87);
    leg->SetNColumns(3);
    leg->SetTextSize(0.05);
    leg->SetLineColor(0);
    leg->SetFillColor(0);

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.22, 1, 1.0);
    pad1->SetBottomMargin(0.020);
    //pad1->SetGridx();
    //pad1->SetGridy();
    pad1->SetFrameFillColor(0);
    pad1->SetLogy();
    pad1->Draw();

    TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.20);
    pad2->SetTopMargin(0.045);
    pad2->SetBottomMargin(0.4);
    //pad2->SetGridx();
    pad2->SetGridy();
    pad2->SetFrameFillColor(0);
    pad2->Draw();

    pad1->cd();
    
    int channel = 999;
    if(v_itr->find("Ch0") != string::npos){
      channel = 0;
      label3->AddText("#mu+Jet");
    }
    else if(v_itr->find("Ch1") != string::npos){
      channel = 1;
      label3->AddText("#it{e}+Jet");
    }
    else continue;
    cout << (*v_itr) << endl;
    cout << channel << endl;

    for(auto v_itr2 = v_Monte.begin(); v_itr2 != v_Monte.end(); ++v_itr2){
      cout << "SAMPLE : " << (*v_itr2)->sample << endl;
      string histName = *v_itr + (*v_itr2)->sample;

      auto h_tmp = (TH1D *)(*v_itr2)->file->Get(histName.c_str());
      if( h_tmp == NULL ){
        cout << "FUCKING NULL POINTER" << endl;
      }
      if(channel == 0) {
	h_tmp->Scale((*v_itr2)->scale_Mu); 
      }
      else if(channel == 1){
	cout << "scale : " << (*v_itr2)->scale_El << endl;
	h_tmp->Scale((*v_itr2)->scale_El);
      }
      else continue;
      
      THStack *hs_same = new THStack("stsame","stack");
      auto v_itr3 = find_if(v_postSample.begin(), v_postSample.end(),
	  [v_itr2](string postSample)->bool{ return (*v_itr2)->category == postSample; });
      h_tmp->SetFillColor((*v_itr2)->color);
      h_tmp->SetLineColor((*v_itr2)->color);      
      if( v_itr3 == v_postSample.end() ){
	h_tmp->SetLineColor(kBlack);
	leg->AddEntry(h_tmp, ((*v_itr2)->category).c_str(), "F");
      }
      if( std::next(v_itr2) != v_Monte.end() && (*v_itr2)->category != (*std::next(v_itr2))->category )
	h_tmp->SetLineColor(kBlack);
           
      v_postSample.push_back((*v_itr2)->category);
      
      int nEvent = h_tmp->Integral();
      int nRaw = h_tmp->Integral();

      if(v_itr->find("invMass") != string::npos)
	h_tmp->Rebin(20);
      h_tmp->Draw("hist");
      hs_tmp->Add(h_tmp);

      outTxt << setprecision(10) << (*v_itr2)->sample << " : " << (double)h_tmp->Integral() << endl;
    }
    
    cout << "FIJFIJFIJFIJ" << endl;
    
    TH1D * h_data;
    string hName;
    if(channel == 0) {
      hName = *v_itr + "DataSingleMu";
      h_data = (TH1D *)m_Data["Mu"]->file->Get(hName.c_str());
    }
    else if(channel == 1){
      hName = *v_itr + "DataSingleEl";
      h_data = (TH1D *)m_Data["El"]->file->Get(hName.c_str());
    }
    else break;
   
    const double nData = h_data->Integral();
    outTxt << hName << " : " << nData << endl;
    
    TH1D *h_stackTohisto = (TH1D *)hs_tmp->GetStack()->Last();
    
    double max_data = h_data->GetMaximum();
    double max_hs = hs_tmp->GetMaximum();
    double maxfrac = 100;
    if( max_hs > max_data )
      //h_data->SetMaximum(1.5*max_hs);
      h_data->SetMaximum(max_hs+max_hs*maxfrac);
    else
      //h_data->SetMaximum(1.5*max_data);
      h_data->SetMaximum(max_data+max_data*maxfrac);

    h_data->SetMinimum(1);
    h_data->GetYaxis()->SetTitleSize(0.04);
    h_data->GetYaxis()->SetLabelSize(0.04);
    h_data->GetYaxis()->SetTitleOffset(1.2);

    h_data->GetXaxis()->SetTitleSize(0.0);
    h_data->GetXaxis()->SetLabelSize(0.0);

    h_data->SetStats(0);
    h_data->SetMarkerStyle(20);
    h_data->SetMarkerSize(0.9);

    h_data->SetTitle("");
    h_data->Draw("P e");
    hs_tmp->Draw("hist SAME");
    h_data->Draw("PSAME");
    label->Draw("SAME");
    label2->Draw("SAME");
    label3->Draw("SAME");
    leg->Draw("SAME");
    pad1->RedrawAxis();

    canvas->cd();
    pad2->cd();

    TH1D * h_div = (TH1D *)h_data->Clone("h_div");
    h_div->Divide(h_stackTohisto);
    
    h_div->SetTitle("");
    
    h_div->GetYaxis()->SetTitle("Data/MC");
    h_div->GetYaxis()->SetTitleSize(0.15);
    h_div->GetYaxis()->SetTitleOffset(0.32);
    h_div->GetYaxis()->SetLabelSize(0.14);
    h_div->SetNdivisions(5,"Y");

    h_div->GetXaxis()->SetTitleSize(0.15);
    h_div->GetXaxis()->SetTitleOffset(0.83);
    h_div->GetXaxis()->SetLabelSize(0.14);
    
    h_div->Draw();
    h_div->SetLineColor(kBlack);
    h_div->SetMinimum(0.6);
    h_div->SetMaximum(1.4);
    h_div->Sumw2();
    h_div->SetStats(0);

    if(nHist==0)
      canvas->Print(Form("%sresult.pdf(",OUTPUT.c_str()),"pdf");
    else if(nHist == (v_Monte[TTBB_]->nHist)-2)
      canvas->Print(Form("%sresult.pdf)",OUTPUT.c_str()),"pdf");
    else
      canvas->Print(Form("%sresult.pdf",OUTPUT.c_str()),"pdf");
    ++nHist;
  }
  outTxt.close();
}
