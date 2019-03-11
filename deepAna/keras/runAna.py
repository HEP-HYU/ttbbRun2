import os
import multiprocessing as mp
import time
from tqdm import tqdm

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
ana_test = False
ana_run  = True
ana_syst = True

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
                    if not ('Data' in process or 'SYS' in process or 'Herwig' in process or 'Evtgen' in process):
                    os.makedirs(outputDir+process+'__'+sys)
                        for item in os.listdir(inputDir+process):
                            proc = mp.Process(target=tt.makeCombi, args=(inputDir+process,item,outputDir+process+'__'+sys,sys))
                            proc.start()
                        for item in os.listdir(inputDir+process):
                            proc.join()

if anaysis:
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
        ana.ana('/home/seohyun/work/heptool/deepAna/keras/array/ZZ_Pythia','ZZ_Pythia',outputDir)

    if ana_run:
        for process in os.listdir(inputDir):
            if process.endswith(".h5"): continue

            if not ('SYS' in process or '__' in process or 'Herwig' in process or 'Evtgen' in process):
                print("Start "+str(process))
                ana.ana(inputDir+process, process, outputDir)

                if ana_syst and not 'Data' in process:
                    for sys in syslist:
                        if 'scale' in sys and not 'tt' in process: continue
                        ana.ana(inputDir+process, process, outputDir, sys)
            else:
                ana.ana(inputDir+process, process, outputDir)

print("Total running time :%s " %(time.time() - start_time))
