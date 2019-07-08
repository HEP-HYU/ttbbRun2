import os
import sys
from ROOT import *
import ROOT

def runUnfold(dir1, dir2, file, sys=""):
    if sys is not "": os.system('root -l -b -q ttbbDiffXsec.C\'("'+dir1+'","'+dir2+'","'+file+'","'+sys+'")\'')
    else            : os.system('root -l -b -q ttbbDiffXsec.C\'("'+dir1+'","'+dir2+'","'+file+'")\'')

if not os.path.exists("/home/seohyun/work/ttbb/ttbbRun2/output/unfold"):
    os.makedirs("../output/unfold")

inputDir = "/home/seohyun/work/ttbb/ttbbRun2/output/post/"
outputDir = "/home/seohyun/work/ttbb/ttbbRun2/output/unfold/"

#runUnfold(inputDir, outputDir, "hist_ttbb"        )
runUnfold(inputDir, outputDir, "hist_DataSingleEl")
runUnfold(inputDir, outputDir, "hist_DataSingleMu")

