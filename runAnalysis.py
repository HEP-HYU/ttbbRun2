#!/usr/bin/python
import os
import sys
import time

import argparse

from ROOT import TChain, TProof, TFile, TH1D, TH1F, TCanvas

import macro.getSampleList
import macro.runPostProcess as post

start_time = time.time()

parser = argparse.ArgumentParser(description='Analyze ntuples')

parser.add_argument('-t', '--test', required=False, type=bool, default=False, help='Run test sample')
parser.add_argument('-y', '--year', required=False, type=int, default=9999, help='Run special year')
parser.add_argument('-s', '--syst', required=False, type=bool, default=True, help='Run systematics')

args = parser.parse_args()

run16  = False 
run17  = False 
run18  = False
test   = False 

if args.year == 16 or args.year == 2016: run16 = True
if args.year == 17 or args.year == 2017: run17 = True
if args.year == 18 or args.year == 2018: run18 = True
if args.year == 9999:
    run16 = True
    run17 = True
    run18 = True

if args.test: test = True

def runAna(dir, file, name):
    if not os.path.exists("output/root"):
        os.makedirs("output/root")

    chain = TChain("ttbbLepJets/tree","events")
    chain.Add(dir+"/"+file)
    chain.SetProof();
    chain.Process("macro/MyAnalysis.C+", dir+"/"+name)

    f = TFile(dir+"/"+file,"read")

    ## save Event Summary histogram ##
    out = TFile("output/root/hist_"+name+".root","update")
    hevt = f.Get("ttbbLepJets/EventInfo")
    hsw = f.Get("ttbbLepJets/ScaleWeights")
    hpdf = f.Get("ttbbLepJets/PDFWeights")
    hevt.Write()
    if not hpdf == None: hpdf.Write()
    if not hsw == None: hsw.Write()
    out.Write()
    out.Close()


macro.getSampleList.getSampleList(os.getcwd()+'/samples')

for year in range(16,19):
    if year == 16 and run16 == False: continue
    if year == 17 and run17 == False: continue
    if year == 18 and run18 == False: continue

    samples = []
    with open('samples/sample'+str(year)+'.txt','r') as f:
        n = 1
        while True:
            line = f.readline()
            if not line: break
            if n == 1:
                inputDir = line[:-1]
            else:
                samples.append(line[:-1])
            n += 1
    
    if test:
        p = TProof.Open("", "workers=8")
        post.runPostProcess(os.getcwd(), samples, year)
        #runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "TTLJ_PowhegPythia_ttbb")
	    #runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "ResponseMatrix_ttbb")
	    #runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "ttbbClosureTest")
        p.Close()
    else:
        p = TProof.Open("", "workers=16")
        
        runAna(inputDir, "DataSingleEG.root", "DataSingleEG")
        runAna(inputDir, "DataSingleMu.root", "DataSingleMu")

        if year == 16:
	        runAna(inputDir, "TTLJ_PowhegPythia_ttbbFilter_ttbb.root", "ResponseMatrix_ttbb")
        else: 
	        runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "ResponseMatrix_ttbb")
	    
        for sample in samples:
	        runAna(inputDir, str(sample)+".root", str(sample))
	        if not "QCD" in sample:
	            runAna(inputDir, str(sample)+".root", str(sample)+"__jerup")
	            runAna(inputDir, str(sample)+".root", str(sample)+"__jerdown")
	            runAna(inputDir, str(sample)+".root", str(sample)+"__jecup")
	            runAna(inputDir, str(sample)+".root", str(sample)+"__jecdown")

	    # hdamp, Tune
        if not year == 16:
            tmp_dic = {"TuneCP5Up":"tuneup", "TuneCP5Down":"tunedown", "hdampUp":"hdampup", "hdampDown":"hdampdown"}
            for key, value in tmp_dic.items():
                runAna(inputDir, "TTLJ_PowhegPythia_SYS_"+key+"_ttbb.root", "TTLJ_PowhegPythia_ttbb__"+value)
                runAna(inputDir, "TTLJ_PowhegPythia_SYS_"+key+"_ttbj.root", "TTLJ_PowhegPythia_ttbj__"+value)
                runAna(inputDir, "TTLJ_PowhegPythia_SYS_"+key+"_ttcc.root", "TTLJ_PowhegPythia_ttcc__"+value)
                runAna(inputDir, "TTLJ_PowhegPythia_SYS_"+key+"_ttLF.root", "TTLJ_PowhegPythia_ttLF__"+value)
                runAna(inputDir, "TTLJ_PowhegPythia_SYS_"+key+"_ttother.root", "TTLJ_PowhegPythia_ttother__"+value)
                runAna(inputDir, "TTJJ_PowhegPythiaBkg_SYS_"+key+".root", "TTJJ_PowhegPythiaBkg__"+value)
                runAna(inputDir, "TTLJ_PowhegPythiaBkg_SYS_"+key+".root", "TTLJ_PowhegPythiaBkg__"+value)
                runAna(inputDir, "TTLL_PowhegPythiaBkg_SYS_"+key+".root", "TTLL_PowhegPythiaBkg__"+value)
        else:
            tmp_dic = {"TuneUp":"tuneup", "TuneDown":"tunedown", "hdampUp":"hdampup", "hdampDown":"hdampdown"}
            for key, value in tmp_dic.items():
                if "tune" in value: tmp_name = "TT_PowhegPythia"
                else: tmp_name = "TTLJ_PowhegPythia"
                runAna(inputDir, tmp_name+"_SYS_"+key+"_ttbb.root", tmp_name+"_ttbb__"+value)
                runAna(inputDir, tmp_name+"_SYS_"+key+"_ttbj.root", tmp_name+"_ttbj__"+value)
                runAna(inputDir, tmp_name+"_SYS_"+key+"_ttcc.root", tmp_name+"_ttcc__"+value)
                runAna(inputDir, tmp_name+"_SYS_"+key+"_ttLF.root", tmp_name+"_ttLF__"+value)
                runAna(inputDir, tmp_name+"_SYS_"+key+"_ttother.root", tmp_name+"_ttother__"+value)
                if not "hdamp" in value:
                    runAna(inputDir, "TT_PowhegPythiaBkg_SYS_"+key+".root", "TT_PowhegPythiaBkg__"+value)
	
        # 2016 only (Isr, Fsr)
        if year == 16:
            tmp_name = "TT_PowhegPythia_"
            tmp_name2 = "TT_PowhegPythiaBkg_"
            runAna(inputDir, tmp_name+"SYS_ISRup_ttbb.root",      tmp_name+"ttbb__isrup")
            runAna(inputDir, tmp_name+"SYS_ISRup_ttbj.root",      tmp_name+"ttbj__isrup")
            runAna(inputDir, tmp_name+"SYS_ISRup_ttcc.root",      tmp_name+"ttcc__isrup")
            runAna(inputDir, tmp_name+"SYS_ISRup_ttLF.root",      tmp_name+"ttLF__isrup")
            runAna(inputDir, tmp_name+"SYS_ISRup_ttother.root",   tmp_name+"ttother__isrup")
            runAna(inputDir, tmp_name2+"SYS_ISRup.root",          tmp_name2+"_isrup")
            runAna(inputDir, tmp_name+"SYS_ISRdown_ttbb.root",    tmp_name+"ttbb__isrdown")
            runAna(inputDir, tmp_name+"SYS_ISRdown_ttbj.root",    tmp_name+"ttbj__isrdown")
            runAna(inputDir, tmp_name+"SYS_ISRdown_ttcc.root",    tmp_name+"ttcc__isrdown")
            runAna(inputDir, tmp_name+"SYS_ISRdown_ttLF.root",    tmp_name+"ttLF__isrdown")
            runAna(inputDir, tmp_name+"SYS_ISRdown_ttother.root", tmp_name+"ttother__isrdown")
            runAna(inputDir, tmp_name2+"SYS_ISRdown.root",       tmp_name2+"_isrdown")

            runAna(inputDir, tmp_name+"SYS_FSRup_ttbb.root",      tmp_name+"ttbb__fsrup")
            runAna(inputDir, tmp_name+"SYS_FSRup_ttbj.root",      tmp_name+"ttbj__fsrup")
            runAna(inputDir, tmp_name+"SYS_FSRup_ttcc.root",      tmp_name+"ttcc__fsrup")
            runAna(inputDir, tmp_name+"SYS_FSRup_ttLF.root",      tmp_name+"ttLF__fsrup")
            runAna(inputDir, tmp_name+"SYS_FSRup_ttother.root",   tmp_name+"ttother__fsrup")
            runAna(inputDir, tmp_name2+"SYS_FSRup.root",          tmp_name2+"_fsrup")
            runAna(inputDir, tmp_name+"SYS_FSRdown_ttbb.root",    tmp_name+"ttbb__fsrdown")
            runAna(inputDir, tmp_name+"SYS_FSRdown_ttbj.root",    tmp_name+"ttbj__fsrdown")
            runAna(inputDir, tmp_name+"SYS_FSRdown_ttcc.root",    tmp_name+"ttcc__fsrdown")
            runAna(inputDir, tmp_name+"SYS_FSRdown_ttLF.root",    tmp_name+"ttLF__fsrdown")
            runAna(inputDir, tmp_name+"SYS_FSRdown_ttother.root", tmp_name+"ttother__fsrdown")
            runAna(inputDir, tmp_name2+"SYS_FSRdown.root",        tmp_name2+"_fsrdown")

        outdir = 'output/root'+str(year)
        if os.path.exists(outdir): 
            os.system('mv '+outdir+' '+outdir+'_backup')
            #os.system('rm -rf '+outdir)
        os.system('mv output/root '+outdir)

        p.Close()

#    if args.syst: post.runPostProcess(os.getcwd(), samples, year)

#    os.system('root -l -b -q macro/runGentree.C\'("'+inputDir+'/","'+os.getcwd()+'/output/root'+str(year)+'/")\'')
#    os.system('hadd hist_TTLJ_PowhegPythia_ttbb.root output/root'+str(year)+'/hist_TTLJ_PowhegPythia_ttbb.root output/root'+str(year)+'/hist_gen.root')
#    os.system('mv hist_TTLJ_PowhegPythia_ttbb.root output/root'+str(year)+'/')

print("Total running time: %s" %(time.time() - start_time))
