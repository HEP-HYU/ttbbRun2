import os
import sys
from ROOT import *
import ROOT

import argparse
import subprocess

parser = argparse.ArgumentParser(description='Unfolding process')

parser.add_argument('-d', '--data', required=True, help='Input data type')
parser.add_argument('-a', '--acceptance', required=False, default="False", help='Run acceptance')
parser.add_argument('-y', '--year', required=False, type=int, default=9999, help='Run special year')
parser.add_argument('-s', '--sys', required=False, default="False", help='Run systematics')
parser.add_argument('-t', '--tunfold', required=False, default="True", help='Unfold by TUnfold')
parser.add_argument('-m', '--matrix', required=False, default="", help='Set input matrix')

args = parser.parse_args()

def runAcceptance(year, ttbb, runSys):
    cmd = ['root', '-l', '-b', '-q', 'makeAcceptance.C("'+str(year)+'", "'+ttbb+'", '+runSys+')']
    with open('../output/unfold/'+str(year)+'/acceptance.txt','w') as f_out:
        subprocess.call(cmd, stdout=f_out)

def runUnfold(year, matrix_name, isData, runSys, useTUnfold):
    cmd = ['root', '-l', '-b', '-q', 'ttbbDiffXsec.C+("'+str(year)+'", "'+matrix_name+'", '+isData+', '+runSys+', '+useTUnfold+')']
    with open('../output/unfold/'+str(year)+'/result.txt','w') as f_out:
        subprocess.call(cmd, stdout=f_out)

for year in range(16,19):
    # Arguments
    # year
    if args.year == 16 or args.year == 2016:
        if year != 16: continue
    if args.year == 17 or args.year == 2017:
        if year != 17: continue
    if args.year == 18 or args.year == 2018:
        if year != 18: continue
    # sys
    runSys = 'false'
    args.sys = (args.sys).lower()
    if args.sys == 'true': runSys = 'true'
    # acceptance
    runAcc = False
    args.acceptance = (args.acceptance).lower()
    if args.acceptance == "true": runAcc = True
    if runAcc: runAcceptance(year, "TTLJ_PowhegPythia_ttbb", runSys)
    # data
    isData = 'false'
    args.data = (args.data).lower()
    if args.data == 'true': isData = 'true'
    # tunfold
    useTUnfold = 'false'
    args.tunfold = (args.tunfold).lower()
    if args.tunfold == 'true': useTUnfold = 'true'
    # matrix
    if args.matrix == "":
        if year == 16: matrix_name = "TTLJ_PowhegPythia_ttbbFilter_ttbb"
        else: matrix_name = "TTLJ_PowhegPythia_ttbb"
    else:
        matrix_name = args.matrix

    if not os.path.exists('../output/unfold'):
        os.mkdir('../output/unfold')

    if not os.path.exists('../output/unfold/'+str(year)):
        os.mkdir('../output/unfold/'+str(year))

    runUnfold(year, matrix_name, isData, runSys, useTUnfold)

