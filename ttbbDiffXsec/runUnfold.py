import os
import sys
from ROOT import *
import ROOT
def runAcceptance(year):
    os.system('root -l -b -q makeAcceptance.C\'("'+year+'")\')')

def runUnfold(dir1, dir2, file, sys=""):
    if sys is not "": os.system('root -l -b -q ttbbDiffXsec.C\'("'+dir1+'","'+dir2+'","'+file+'","'+sys+'")\'')
    else            : os.system('root -l -b -q ttbbDiffXsec.C\'("'+dir1+'","'+dir2+'","'+file+'")\'')

if not os.path.exists("../output/unfold"):
    os.makedirs("../output/unfold")

inputDir = "../output/post/"
outputDir = "../output/unfold/"

<<<<<<< HEAD


runUnfold(inputDir, outputDir, "hist_ttbb"        )
#runUnfold(inputDir, outputDir, "hist_DataSingleEl")
#runUnfold(inputDir, outputDir, "hist_DataSingleMu")
=======
#runUnfold(inputDir, outputDir, "hist_ttbb"        )
runUnfold(inputDir, outputDir, "hist_DataSingleEl")
runUnfold(inputDir, outputDir, "hist_DataSingleMu")
>>>>>>> fbc2120cbe49b2828e2fe3719e5c264f9736392d

