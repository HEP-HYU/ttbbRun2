import os
import multiprocessing as mp
from tqdm import tqdm
import time

import ntuple2array as tt
import variableAnalyzer as var
start_time = time.time()

array = True 
train = False
inputDir = '/data/users/seohyun/ntuple/hep2017/v808/split/'
outputDir = '/home/seohyun/work/heptool/deepAna/keras/array/'
#fileSize = os.path.getsize(inputDir+process+'/Tree_ttbbLepJets_000.root')/(1024.0**3)

if train :
    tt.makeTrainingInput(outputDir)
    #var.analyze_variable(outputDir, 'array_train_ttbb.h5')
    #tt.makeCombi('/data/users/seohyun/ntuple/hep2017/v808/nosplit/', 'TTLJ_PowhegPythia_ttbbFilter_ttbb.root', outputDir)

if array :
    for process in os.listdir(inputDir) :
        if not os.path.exists(outputDir+process) :
            os.makedirs(outputDir+process)
        for item in os.listdir(inputDir+process) :
            proc = mp.Process(target=tt.makeCombi, args=(inputDir+process,item,outputDir+process))
            proc.start()
        for item in os.listdir(inputDir+process) :
            proc.join()

print("Running Time : %s " %(time.time() - start_time))
