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
string output = "/home/seohyun/work/ttbb/heptool/output/root/";

std::string ReplaceAll(std::string &str, const std::string &from, const std::string &to){
  size_t start_pos = 0;
  while((start_pos = str.find(from, start_pos)) != std::string::npos){
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return str;
}

void arrange(){
  TFile *f_filter = TFile::Open(Form("%s/hist_TTLJ_PowhegPythia_ttbbFilter_ttbb.root",input.c_str()));

  vector<string> v_histName;

  TIter next(f_filter->GetListOfKeys());
  TKey *key;
  TObject *obj;
  while((key = (TKey *)next())){
    obj = key->ReadObj();
    string tmp = obj->GetName();
    ReplaceAll(tmp, "TTLJ_PowhegPythia_ttbbFilter_ttbb","");
    v_histName.push_back(tmp);
    cout << tmp << endl;
  }

  const int nHist = v_histName.size();
  cout << "Number of histograms: " << nHist << endl;

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
    f_el[i] = TFile::Open(Form("%s/hist_%s.root",input.c_str(),v_dataName_eg[i].c_str()));
  }
  TFile *f_out_mu = TFile::Open(Form("%s/hist_DataSingleMu.root",output.c_str()),"recreate");
  TFile *f_out_el = TFile::Open(Form("%s/hist_DataSingleEl.root",output.c_str()),"recreate");
  string name_mu = "DataSingleMu";
  f_out_mu->cd();
  TH1 *h_mu[nHist];
  for( int i=0; i<nHist; ++i){
    ssize_t pos;
    if(((pos = (v_histName.at(i)).find("Gen",0,3)) != std::string::npos)
	|| ((pos = (v_histName.at(i)).find("Resp",0,4)) != std::string::npos))
      continue;
    if((pos = (v_histName.at(i)).find("EventInfo",0,5)) != std::string::npos)
      continue;
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
    ssize_t pos;
    if(((pos = (v_histName.at(i)).find("Gen",0,3)) != std::string::npos)
	|| ((pos = (v_histName.at(i)).find("Resp",0,4)) != std::string::npos))
      continue;
    if((pos = (v_histName.at(i)).find("EventInfo",0,5)) != std::string::npos)
      continue;
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
    f_in[i] = TFile::Open(Form("%s/hist_%s.root",input.c_str(),v_sampleName[i].c_str()));
    f_out[i] = TFile::Open(Form("%s/hist_%s.root",output.c_str(),v_sampleName_out[i].c_str()),"recreate");
  }

  for(int i=0; i<nSample; ++i){
    f_out[i]->cd();
    for(int j=0; j<nHist; ++j){
      ssize_t pos;
      if(((pos = (v_histName.at(j)).find("Gen",0,3)) != std::string::npos)
	  || ((pos = (v_histName.at(j)).find("Resp",0,4)) != std::string::npos))
        continue;
      if((pos = (v_histName.at(j)).find("EventInfo",0,5)) != std::string::npos)
        continue;
      string histName = v_histName.at(j)+v_sampleName.at(i);
      auto h_tmp = (TH1 *)f_in[i]->Get(histName.c_str());
      string histName2 = v_histName.at(j)+v_sampleName_out.at(i);
      h_tmp->SetName(histName2.c_str());
      h_tmp->Write();
    }
    TH1 *eventInfo = (TH1 *)f_in[i]->Get("EventInfo");
    eventInfo->Write();
    f_out[i]->Write();
    f_out[i]->Close();
  }

  //f_filter = TFile::Open(Form("%s/hist_TTLJ_PowhegPythia_ttbb.root",input.c_str()));
  TFile *f_matrix = TFile::Open(Form("%s/hist_respMatrix_ttbb.root",output.c_str()),"recreate");
  f_matrix->cd();

  for(int i=0; i<2; ++i){
    auto h1 = (TH1 *)f_filter->Get(Form("h_ResponseMatrixDeltaR_Ch%d_S3_TTLJ_PowhegPythia_ttbbFilter_ttbb",i));
    //auto h1 = (TH1 *)f_filter->Get(Form("h_ResponseMatrixDeltaR_Ch%d_S3_TTLJ_PowhegPythia_ttbb",i));
    auto h2 = (TH1 *)f_filter->Get(Form("h_ResponseMatrixInvMass_Ch%d_S3_TTLJ_PowhegPythia_ttbbFilter_ttbb",i));
    //auto h2 = (TH1 *)f_filter->Get(Form("h_ResponseMatrixInvMass_Ch%d_S3_TTLJ_PowhegPythia_ttbb",i));
    //h1->SetName(Form("h_ResponseMatrixDeltaR_Ch%d_S3_TTLJ_PowhegPythia_ttbbFilter_ttbb",i));
    //h2->SetName(Form("h_ResponseMatrixInvMass_Ch%d_S3_TTLJ_PowhegPythia_ttbbFilter_ttbb",i));
    h1->Write();
    h2->Write();
  }

  TH1 *info = (TH1 *)f_filter->Get("EventInfo");
  info->Write();
  f_matrix->Write();
  f_matrix->Close();

  vector<string> v_systematics = {
    "TT_PowhegPythia_SYS_FSRup", "TT_PowhegPythia_SYS_FSRdown",
    "TT_PowhegPythia_SYS_ISRup", "TT_PowhegPythia_SYS_ISRdown",
    "TT_PowhegHerwig", "TT_PowhegPythiaEvtgen", "TT_aMCatNLOPythia",
    "TTLJ_PowhegPythia"
  };
  
  const int nsys = v_systematics.size();
  TFile *f_sysin[nsys], *f_sysout[nsys];
  for(int i=0; i < nsys; ++i){
    f_sysin[i] = TFile::Open(Form("%s/hist_%s_ttbb.root", input.c_str(), v_systematics[i].c_str()));
    f_sysout[i] = TFile::Open(Form("%s/hist_%s_ttbb.root", output.c_str(),v_systematics[i].c_str()),"recreate");
  }
  
  for(int i=0; i<nsys; ++i){
    f_sysout[i]->cd();
    cout << v_systematics.at(i) << endl;
    ssize_t pos;
    for(int j=0; j<nHist; ++j){
      if((pos = (v_histName.at(j)).find("EventInfo",0,5)) != std::string::npos)
        continue;
      string histName = v_histName.at(j)+v_systematics.at(i)+"_ttbb";
      auto h_tmp = (TH1 *)f_sysin[i]->Get(histName.c_str());
      string histName2 = v_histName.at(j)+"ttbb";
      h_tmp->SetName(histName2.c_str());
      h_tmp->Write();
    }
    TH1 *eventInfo = (TH1 *)f_sysin[i]->Get("EventInfo");
    eventInfo->Write();
    f_sysout[i]->Write();
    f_sysout[i]->Close();
  }
}
