import os
import sys
from ROOT import *
import ROOT

def runAcceptance(year):
    os.system('root -l -b -q makeAcceptance.C\'('+year+')\')')

def runUnfold(dir1, dir2, file, sys=""):
    if sys is not "": os.system('root -l -b -q ttbbDiffXsec.C\'("'+dir1+'","'+dir2+'","'+file+'","'+sys+'")\'')
    else            : os.system('root -l -b -q ttbbDiffXsec.C\'("'+dir1+'","'+dir2+'","'+file+'")\'')

test = True

# Unfold for each year
for year in range(16,19):
    inputDir = '../output/post'+str(year)
    outputDir = '../output/unfold'+str(year)

    if not os.path.exists(outputDir):
        os.makedirs(outputDir)

    runAcceptance(year)

    if test:
        runUnfold(inputDir, outputDir, "hist_ttbb"        )
    else:
        runUnfold(inputDir, outputDir, "hist_DataSingleEl")
        runUnfold(inputDir, outputDir, "hist_DataSingleMu")
