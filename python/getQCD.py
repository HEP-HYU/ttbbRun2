from ROOT import *
import ROOT
import os
import sys

def getQCDShape(base_path, year, sample_list):
    print("Get QCD shape in the sideband region")
    print("YEAR: "+str(year))
    input_path = base_path+'/output/root'+str(year)+'_qcd/'

    f_qcd = TFile.Open(os.path.join(input_path, "hist_dataDriven_QCD.root"), "recreate")
    f_muon = TFile.Open(os.path.join(input_path, "hist_dataDriven_DataSingleMu.root"))
    f_elec = TFile.Open(os.path.join(input_path, "hist_dataDriven_DataSingleEG.root"))

    hist_list = []
    for x in f_muon.GetListOfKeys():
        if "__" in x.GetName(): continue
        hist_list.append(x.GetName())

    lumi = {16:35922, 17:41529, 18:59693}

    genevt = {}
    with open(base_path+'/samples/genevt'+str(year)+'.txt','r') as f:
        while True:
            line = f.readline()
            if not line: break
            tmp = line.split(' ')
            genevt[tmp[0]] = float(tmp[1])

    xsec = {}
    with open(base_path+'/samples/xsec'+str(year)+'.txt','r') as f:
        while True:
            line = f.readline()
            if not line: break
            if 'Sample Xsec' in line: continue
            tmp = line.split(' ')
            xsec[tmp[0]] = float(tmp[1])

    samples = {}
    for proc in sample_list:
        f_tmp = TFile.Open(os.path.join(input_path, "hist_dataDriven_"+proc+".root"))
        samples[proc] = f_tmp

    f_qcd.cd()
    for hist in hist_list:
        if 'Ch0' in hist:
            h_qcd = f_muon.Get(hist)
        elif 'Ch1' in hist:
            h_qcd = f_elec.Get(hist)
        else:
            h_qcd = f_muon.Get(hist)
            tmp = f_elec.Get(hist)
            h_qcd.Add(tmp)

        if "TransverseMass" in hist:
            nevt_data = h_qcd.Integral(0, h_qcd.GetNbinsX()+1)

        for proc in sample_list:
            if any(i in proc for i in ['QCD', 'Filter', 'Data']): continue
            h_tmp = samples[proc].Get(hist)
            scale = lumi[year]*xsec[proc]/genevt[proc]
            h_tmp.Scale(scale)
            h_qcd.Add(h_tmp,-1)

        if "TransverseMass" in hist:
            nevt_qcd = h_qcd.Integral(0, h_qcd.GetNbinsX()+1)
            print("Purity of QCD")
            print("Hist: "+str(hist))
            print("nevt_data: "+str(nevt_data))
            print("nevt_qcd: "+str(nevt_qcd))
            purity = float(nevt_qcd)/float(nevt_data)
            print("Puriy: "+str(purity))

        h_qcd.Write()
    f_qcd.Close()
