import os
import multiprocessing as mp
from tqdm import tqdm
import time

import ntuple2array as tt
import variableAnalyzer as var
start_time = time.time()

test  = True
array = False
train = False
syst  = False
inputDir = '/data/users/seohyun/ntuple/hep2017/v808/split/'
outputDir = '/home/seohyun/work/heptool/deepAna/keras/array/'
#fileSize = os.path.getsize(inputDir+process+'/Tree_ttbbLepJets_000.root')/(1024.0**3)

if test:
    os.makedirs(outputDir+'test_ttbb')
    tt.makeCombi('/data/users/seohyun/ntuple/hep2017/v808/split/TTLJ_PowhegPythia_ttbb','Tree_ttbbLepJets_002.root' ,'/home/seohyun/work/heptool/deepAna/keras/array/test_ttbb')
#    tt.makeCombi('/data/users/seohyun/ntuple/hep2017/v808/split/ZJets_M10to50_aMCatNLO_part1','Tree_ttbbLepJets_002.root' ,'/home/seohyun/work/heptool/deepAna/keras/array/test_other')
#    tt.makeCombi('/data/users/seohyun/ntuple/hep2017/v808/split/DataSingleMuB','Tree_ttbbLepJets_002.root' ,'/home/seohyun/work/heptool/deepAna/keras/array/test_data')
    os.makedirs(outputDir+'test_ttbb'+'__'+'jecup')
    tt.makeCombi(inputDir+'TTLJ_PowhegPythia_ttbb', 'Tree_ttbbLepJets_002.root', outputDir+'test_ttbb'+'__'+'jecup','jecup')

if train:
    tt.makeTrainingInput(outputDir)
    #var.analyze_variable(outputDir, 'array_train_ttbb.h5')
    #tt.makeCombi('/data/users/seohyun/ntuple/hep2017/v808/nosplit/', 'TTLJ_PowhegPythia_ttbbFilter_ttbb.root', outputDir)

if array:
    for process in os.listdir(inputDir):
        if not os.path.exists(outputDir+process):
            os.makedirs(outputDir+process)
        for item in os.listdir(inputDir+process):
            proc = mp.Process(target=tt.makeCombi, args=(inputDir+process,item,outputDir+process))
            proc.start()
        for item in os.listdir(inputDir+process):
            proc.join()

    if syst:
        syslist = ['jecup','jecdown','jerup','jerdown']
        for sys in syslist:
            for process in os.listdir(inputDir):
                if not ('Data' in process or 'SYS' in process or 'Herwig' in process or 'Evtgen' in process):
                    os.makedirs(outputDir+process+'__'+sys)
                    for item in os.listdir(inputDir+process):
                        proc = mp.Process(target=tt.makeCombi, args=(inputDir+process,item,outputDir+process+'__'+sys,sys))
                        proc.start()
                    for item in os.listdir(inputDir+process):
                        proc.join()

print("Running Time : %s " %(time.time() - start_time))
