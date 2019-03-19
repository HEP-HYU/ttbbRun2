/*******************************************************************
 * Last Update : 2017-07-30                                        *
 * Developer : Seohyun                                             *
 * This macro is used as substitute for makePlots.py               *
 *******************************************************************/

#include <iostream>
#include <string>
#include <vector>

#include <TFile.h>
#include <TObject.h>
#include <TKey.h>
#include <TCanvas.h>
#include <TPad.h>

#include "histBook.h"
#include "tdrstyle.C"

string INPUT = "../output/post/";
string OUTPUT = "../output/pdf/";

std::string ReplaceAll(std::string &str, const std::string &from, const std::string &to);
 
const double SF_ttbb = 0.992;
const double SF_ttbj = 0.997;
const double SF_ttcc = 0.871;
const double SF_ttLF = 0.890;
//const double SF_ttbb = 1;
//const double SF_ttbj = 1;
//const double SF_ttcc = 1;
//const double SF_ttLF = 1;

//-----------------------------------------------------------------//
//  Histogram file reader                                          //
//-----------------------------------------------------------------//
class DataFile{
  private:
  public:
    TFile *file;
    string data;
    vector<string> v_histName;
    int nHist;
    
    DataFile(string data);
    DataFile(DataFile &dataFile){};
    ~DataFile(){};
};

class MonteFile{
  private:
  public:
    TFile *file;
    string sample;
    string category;
    vector<string> v_histName;
    double scale_Mu;
    double scale_El;
    int color;
    int nHist;
    
    MonteFile(string sample, string category, int color, double xsec);
    MonteFile(MonteFile &MonetFile){};
    ~MonteFile(){};
};

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

std::string ReplaceAll(std::string &str, const std::string &from, const std::string &to){
  size_t start_pos = 0;
  while((start_pos = str.find(from, start_pos)) != std::string::npos){
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return str;
}
