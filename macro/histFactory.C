#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "TFile.h"
#include "TROOT.h"
#include "TSystem.h"
#include "RooWorkspace.h"
#include "RooAbsData.h"
#include "RooRealVar.h"
#include "RooStats/HistFactory/Measurement.h"
#include "RooStats/HistFactory/MakeModelAndMeasurementsFast.h"

void histFactory(int year, std::string hist_name){
  ssize_t pos;
  std:;string input_name = "../output/template/hist_temp"+std::to_string(year)+".root";
  
  ifstream f_sample(Form("../samples/sample%d.txt", year));
  std::vector<std::string> v_sample = {
    "ttbbttX", "ttbj", "STttbkgLFcc", "WZJetsQCDVV"
    //"ttbb", "ttbj", "ttcc", "ttLF", "ttbkg", "ttX", "SingleTop", "WJets", "ZJets", "VV", "QCD"
  };
  
  TFile *f_template = TFile::Open(input_name.c_str());
  std::vector<std::string> v_data = {"DataSingleMu", "DataSingleEG", "DataSingleLep"};
  
  int ich = 999;
  if((pos = hist_name.find("Ch0",0)) != std::string::npos) ich = 0;
  else if((pos = hist_name.find("Ch1",0)) != std::string::npos) ich = 1;
  else ich = 2;

  std::cout << "Start Fitting" << std::endl;

  RooStats::HistFactory::Measurement meas("meas", "meas");
  meas.SetOutputFilePrefix(Form("../output/fit/%d/%d",year, ich));
  meas.SetPOI("ttbb");
  meas.SetPOI("ttbj");
  meas.SetLumi(1.0);
  meas.AddConstantParam("Lumi");

  std::cout << "Set channel" << std::endl;
  RooStats::HistFactory::Channel channel("tempFit", input_name);
  channel.SetData(hist_name+"_"+v_data[ich], input_name);

  std::cout << "Set background" << std::endl;
  RooStats::HistFactory::Sample sample[v_sample.size()];
  int i = 0;
  for(auto v_itr = v_sample.begin(); v_itr != v_sample.end(); ++v_itr){
    std::cout << "Sample: " << *v_itr << std::endl;
    sample[i].SetInputFile(input_name);
    sample[i].SetName(*v_itr);
    sample[i].SetHistoName(hist_name+"_"+(*v_itr));
    sample[i].ActivateStatError(sample[i].GetHistoName(), sample[i].GetInputFile());
    //if((pos = (*v_itr).find("QCD",0)) != std::string::npos)
    //  sample[i].AddNormFactor(*v_itr, 1.0, 0.5, 1.5);
    //else sample[i].AddNormFactor(*v_itr, 1.0, 0.9, 1.1);
    sample[i].AddNormFactor(*v_itr, 1.0, 0.5, 2.0);

    channel.AddSample(sample[i]);
    ++i;
  }
  std::cout << "Add channel" << std::endl;
  meas.AddChannel(channel);
  std::cout << "CollectHistograms" << std::endl;
  meas.CollectHistograms();

  RooWorkspace *w = RooStats::HistFactory::MakeModelAndMeasurementFast(meas);
}
