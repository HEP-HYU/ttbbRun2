#include <string>
#include <vector>
#include <iostream>

#include <TFile.h>
#include <THStack.h>
#include <TObject.h>
#include <TH1.h>
#include <TH2.h>
#include <TKey.h>

//#include "../include/histBook.h"
using namespace std;

string input = "/home/seohyun/work/ttbb/heptool/tmp/";
string output = "/home/seohyun/work/ttbb/heptool/output/";
string ntuple = "/data/users/seohyun/ntuple/hep2017/v808/nosplit/"

void merge(){
  vector<string> v_histName;
  for(int i=0;i<2;++i){
    v_histName.push_back(Form("h_nJets_Ch%d_S3_",i));
    v_histName.push_back(Form("h_nBjets_Ch%d_S3_",i));
    v_histName.push_back(Form("h_RecoJetDeltaR_Ch%d_S3_",i));
    v_histName.push_back(Form("h_RecoJetInvMass_Ch%d_S3_",i));
  }
  const int nHist = v_histName.size();
  vector<string> v_dataName_mu = {
    "DataSingleMuB","DataSingleMuC","DataSingleMuD","DataSingleMuE","DataSingleMuF",
    "DataSingleMuG","DataSingleMuHv2","DataSingleMuHv3"
  };
  vector<string> v_dataName_eg = {
    "DataSingleEGB","DataSingleEGC","DataSingleEGD","DataSingleEGE","DataSingleEGF",
    "DataSingleEGG","DataSingleEGHv2","DataSingleEGHv3"
  };
  const int nData = v_dataName_mu.size();

  TFile *f_mu[nData], *f_el[nData];
  for(int i=0; i<nData; ++i){
    f_mu[i] = TFile::Open(Form("%s/hist_%s.root",input.c_str(),v_dataName_mu[i].c_str()));
    f_el[i] = TFile::Open(Form("%s/hist_%s.root",input.c_str(),v_dataName_el[i].c_str()));
  }
  TFile *f_out_mu = TFile::Open(Form("%s/hist_DataSingleMu.root",output.c_str()),"recreate");
  TFile *f_out_el = TFile::Open(Form("%s/hist_DataSingleEl.root",output.c_str()),"recreate");
  string name_mu = "DataSingleMu";
  f_out_mu->cd();
  TH1 *h_mu[nHist];
  for( int i=0; i<nHist; ++i){
    THStack *hs_tmp = new THStack("st","");
    for(int j=0; j<nData; ++j){
      string histName = v_histName.at(i)+v_dataName_mu[j];
      auto h_tmp = (TH1*)f_mu[j]->Get(histName.c_str());
      hs_tmp->Add(h_tmp);
    }
    string saveName = v_histName.at(i)+name_mu;
    h_mu[i] = (TH1 *)hs_tmp->GetStack()->Last();
    h_mu[i]->SetName(saveName.c_str());
    h_mu[i]->Write();
  }
  f_out_mu->Write();
  f_out_mu->Close();

  string name_el = "DataSingleEl";
  f_out_el->cd();
  TH1 *h_el[nHist];
  for( int i=0; i<nHist; ++i){
    THStack *hs_tmp = new THStack("st","");
    for(int j=0; j<nData; ++j){
      string histName = v_histName.at(i)+v_dataName_eg[j];
      auto h_tmp = (TH1*)f_el[j]->Get(histName.c_str());
      hs_tmp->Add(h_tmp);
    }
    string saveName = v_histName.at(i)+name_el;
    h_el[i] = (TH1 *)hs_tmp->GetStack()->Last();
    h_el[i]->SetName(saveName.c_str());
    h_el[i]->Write();
  }
  f_out_el->Write();
  f_out_el->Close();

  for(int i=0; i<8; ++i){
    f_mu[i]->Close();
    f_el[i]->Close();
  }

  vector<string> v_sampleName = {
    "tbar_Powheg","tbarW_Powheg","t_Powheg","tW_Powheg","TTLJ_PowhegPythia_ttLF","TTLJ_PowhegPythia_ttbb","TTLJ_PowhegPythia_ttbj","TTLJ_PowhegPythia_ttcc","TTLJ_PowhegPythia_ttjj","TTLJ_PowhegPythia_ttother","ttHbb_PowhegPythia","ttW_Madgraph","ttZ_Madgraph","WJets_aMCatNLO","ZJets_M10to50_aMCatNLO","ZJets_M50_aMCatNLO","WW_Pythia","WZ_Pythia","ZZ_Pythia","ttbar_PowhegPythiaBkg"
  };

  vector<string> v_sampleName_out = {
    "tbarchannel","tbarWchannel","tchannel","tWchannel","ttLF","ttbb","ttbj","ttcc","ttjj","ttother","ttH","ttW","ttZ","wjets","zjets10to50","zjets","ww","wz","zz","ttbkg"
  };
  const int nSample = v_sampleName.size();

  TFile *f_in[nSample],*f_out[nSample],*f_tree[nSample];
  for(int i=0;i<20;++i){
    f_in[i] = TFile::Open(Form("%s/hist_%s.root",input.c_str(),sampleName[i].c_str()));
    f_out[i] = TFile::Open(Form("%s/hist_%s.root",output.c_str(),dataName[i].c_str()),"recreate");
    f_tree[i] = TFile::Open(Form("%s/%s.root",ntuple.c_str(),sampleName[i].c_str()));
  }

  
  for(int i=0; i<nSample; ++i){
    f_out[i]->cd();
    for(int j=0; j<nHist; ++j){
      string histName = v_histName.at(j)+sampleName[i];
      auto h_tmp = (TH1 *)f_in[i]->Get(histName.c_str());
      string histName2 = v_histName.at(j)+dataName[i];
      h_tmp->SetName(histName2.c_str());
      h_tmp->Write();
    }
    TH1 *eventInfo = (TH1 *)f_tree[i]->Get("ttbbLepJets/EventInfo");
    eventInfo->Write();
    f_out[i]->Write();
    f_out[i]->Close();
    f_tree[i]->Close();
  }

  TFile *f_matrix = TFile::Open(Form("%s/hist_respMatrix_ttbb.root",output),"recreate");

  vector<string> v_histNameMatrix;
  for(int i=0; i<2; ++i){
    v_histNameMatrix.push_back(Form("h_GenbJetDeltaR_Ch%d_nosel_TTLJ_PowhegPythia_ttbb",i));
    v_histNameMatrix.push_back(Form("h_GenbJetInvMass_Ch%d_nosel_TTLJ_PowhegPythia_ttbb",i));
    v_histNameMatrix.push_back(Form("h_GenbJetDeltaR_Ch%d_S3_TTLJ_PowhegPythia_ttbb",i));
    v_histNameMatrix.push_back(Form("h_GenbJetInvMass_Ch%d_S3_TTLJ_PowhegPythia_ttbb",i));
    v_histNameMatrix.push_back(Form("h_ResponseMatrixDeltaR_Ch%d_S3_TTLJ_PowhegPythia_ttbb",i));
    v_histNameMatrix.push_back(Form("h_ResponseMatrixInvMass_Ch%d_S3_TTLJ_PowhegPythia_ttbb",i));
  }
  f_matrix->cd();
  for(int i=0; i<v_histNameMatrix.size();++i){
    auto h_tmp = (TH1 *)f_in[5]->Get((v_histNameMatrix.at(i)).c_str());
    h_tmp->Write();
  }
  f_matrix->Write();
  f_matrix->Close();
}
