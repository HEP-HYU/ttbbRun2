import os
import multiprocessing as mp
import time
from tqdm import tqdm

from ROOT import *
import ROOT

import ntuple2array as tt
import variableAnalyzer as var
import ana

#Option
array = False
array_test = False
array_run = False
array_train = False
array_syst = False

analysis = True
ana_test = True 
ana_run  = False
ana_syst = False

start_time = time.time()

inputDir = '/home/seohyun/work/heptool/deepAna/keras/array/'
outputDir = '/home/seohyun/work/heptool/deepAna/keras/hist/'

if array:
    if array_test:
        os.makedirs(outputDir+'test_ttbb')
        tt.makeCombi('/data/users/seohyun/ntuple/hep2017/v808/split/TTLJ_PowhegPythia_ttbb','Tree_ttbbLepJets_002.root' ,'/home/seohyun/work/heptool/deepAna/keras/array/test_ttbb')

    if array_train:
        tt.makeTrainingInput(outputDir)
        #var.analyze_variable(outputDir, 'array_train_ttbb.h5')

    if array_run:
        for process in os.listdir(inputDir):
            if not os.path.exists(outputDir+process):
                os.makedirs(outputDir+process)
            for item in os.listdir(inputDir+process):
                proc = mp.Process(target=tt.makeCombi, args=(inputDir+process,item,outputDir+process))
                proc.start()
            for item in os.listdir(inputDir+process):
                proc.join()

        if array_syst:
            syslist = ['jecup','jecdown','jerup','jerdown']
            for sys in syslist:
                for process in os.listdir(inputDir):
                    if not ('Data' in process or 'SYS' in process or 'Herwig' in process or 'Evtgen' in process or 'aMC' in process):
                        os.makedirs(outputDir+process+'__'+sys)
                        for item in os.listdir(inputDir+process):
                            proc = mp.Process(target=tt.makeCombi, args=(inputDir+process,item,outputDir+process+'__'+sys,sys))
                            proc.start()
                        for item in os.listdir(inputDir+process):
                            proc.join()

if analysis:
    syslist = [
        '__puup', '__pudown',
        '__musfup', '__musfdown', '__mutrgup', '__mutrgdown',
        '__elsfup', '__elsfdown', '__eltrgup', '__eltrgdown',
        '__lfup', '__lfdown', '__hfup', '__hfdown',
        '__hfstat1up', '__hfstat1down', '__hfstat2up', '__hfstat2down',
        '__lfstat1up', '__lfstat1down', '__lfstat2up', '__lfstat2down',
        '__cferr1up', '__cferr1down', '__cferr2up', '__cferr2down',
        '__scale0', '__scale1', '__scale2', '__scale3', '__scale4', '__scale5'
    ]

    if ana_test:
        #ana.ana('/home/seohyun/work/heptool/deepAna/keras/array/TTLJ_PowhegPythia_ttbbFilter_ttbb','TTLJ_PowhegPythia_ttbbFilter_ttbb', 'test')
        ana.ana('/home/seohyun/work/heptool/deepAna/keras/array/TTLJ_PowhegPythia_ttbb','TTLJ_PowhegPythia_ttbb','test')

    if ana_run:
        for process in os.listdir(inputDir):
            if process.endswith(".h5"): continue

            if not ('SYS' in process or '__' in process or 'Herwig' in process or 'Evtgen' in process or 'TT_aMCatNLO' in process):
                print("Start "+str(process))
                ana.ana(inputDir+process, process, outputDir)

                if ana_syst and not 'Data' in process:
                    for sys in syslist:
                        if 'scale' in sys and not 'tt' in process: continue
                        ana.ana(inputDir+process, process, outputDir, sys)
            else:
                if   'FSRup'   in process: sys = '__fsrup'
                elif 'FSRdown' in process: sys = '__fsrdown'
                elif 'ISRup'   in process: sys = '__isrup'
                elif 'ISRdown' in process: sys = '__isrdown'
                elif 'hdampUp' in process: sys = '__hdampup'
                elif 'hdampDown' in process: sys = '__hdampdown'
                elif 'TuneUp'  in process: sys = '__tuneup'
                elif 'TuneDown'in process: sys = '__tunedown'
                elif 'jecdown' in process: sys = '__jecdown'
                elif 'jecup'   in process: sys = '__jecup'
                elif 'jerdown' in process: sys = '__jerdown'
                elif 'jerup'   in process: sys = '__jerup'
                elif 'Herwig'  in process: sys = '__powhegherwig'
                elif 'TT_aMCatNLO'in process: sys = '__amcatnlopythia'
                elif 'Evtgen'  in process: sys = '__powhegpythiaevtgen'
                else:                      sys = ''
                ana.ana(inputDir+process, process, outputDir, sys)

    """
    base_path = "/home/seohyun/work/heptool/deepAna/keras/hist/model_25_0.8001.h5"
    f_mu_list = []
    f_mu_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleMuB.root")))
    f_mu_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleMuC.root")))
    f_mu_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleMuD.root")))
    f_mu_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleMuE.root")))
    f_mu_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleMuF.root")))
    f_mu_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleMuG.root")))
    f_mu_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleMuHv2.root")))
    f_mu_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleMuHv3.root")))

    hist_list = [x.GetName() for x in f_mu_list[0].GetListOfKeys()]

    f_out = TFile.Open(os.path.join(base_path, "hist_DataSingleMu.root"), "recreate")
    f_out.cd()

    for hist in hist_list:
        if hist == "EventInfo" or hist == "ScaleWeights": continue
        hs_tmp = THStack("st","");
        for f_mu in f_mu_list:
            hs_tmp.Add(f_mu.Get(hist))
        h_tmp = hs_tmp.GetStack().Last()
        h_tmp.Write()

    f_out.Write()
    f_out.Close()

    f_el_list = []
    f_el_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleEGB.root")))
    f_el_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleEGC.root")))
    f_el_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleEGD.root")))
    f_el_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleEGE.root")))
    f_el_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleEGF.root")))
    f_el_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleEGG.root")))
    f_el_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleEGHv2.root")))
    f_el_list.append(TFile.Open(os.path.join(base_path, "hist_DataSingleEGHv3.root")))

    hist_list2 = [x.GetName() for x in f_el_list[0].GetListOfKeys()]

    f_out2 = TFile.Open(os.path.join(base_path, "hist_DataSingleEl.root"), "recreate")
    f_out2.cd()

    for hist in hist_list2:
        if hist == "EventInfo" or hist == "ScaleWeights": continue
        hs_tmp = THStack("st","");
        for f_el in f_el_list:
            hs_tmp.Add(f_mu.Get(hist))
        h_tmp = hs_tmp.GetStack().Last()
        h_tmp.Write()

    f_out.Write()
    f_out.Close()
    """
print("Total running time :%s " %(time.time() - start_time))
