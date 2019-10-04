#!/usr/bin/python
run16  = False 
run17  = False 
run18  = True
test   = True 
do_sys = False

import os
import sys

from ROOT import TChain, TProof, TFile, TH1D, TH1F, TCanvas

import macro.getSampleList
import macro.runPostProcess

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
    hevt.Write()
    hsw.Write()
    out.Write()
    out.Close()

### Systematics ###
##### List #####
# PileUp
#   __puup, __pudown
# Muon
#   __muidup, __muiddown, __muisoup, __muisodown, __mutrgup, __mutrgdown
# Electron
#   __elidup, __eliddown, __elrecoup, __elrecodown, __elzvtxup, __elzvtxdown
#   __eltrgup, __eltrgdown
# CSV shape
#   __lfup, __lfdown, __hfup, __hfdown
#   __hfstat1up, __hfstat1down, __hfstat2up, __hfstat2down
#   __lfstat1up, __lfstat1down, __lfstat2up, __lfstat2down
#   __cferr1up, __cferr1down, __cferr2up, __cferr2down
# Scale Weight(ME)
#   __scale0, __scale1, __scale2, __scale3, __scale4, __scale5
# Parton Shower(PS)
#   __isrup, __isrdown, __fsrup, __fsrdown
# ME & PS
#   __hdampup, __hdampdown
# Pythia Tune
#   __tuneup, __tunedown
#####

macro.getSampleList.getSampleList(os.getcwd()+'/samples')

sys_list = [
    "__jerup", "__jerdown", "__jecup", "__jecdown",
    "__puup", "__pudown",
    "__musfup", "__musfdown", "__mutrgup", "__mutrgdown",
    "__elsfup", "__elsfdown", "__eltrgup", "__eltrgdown",
    "__lfup", "__lfdown", "__hfup", "__hfdown",
    "__hfstat1up", "__hfstat1down", "__hfstat2up", "__hfstat2down",
    "__lfstat1up", "__lfstat1down", "__lfstat2up", "__lfstat2down",
    "__cferr1up", "__cferr1down", "__cferr2up", "__cferr2down"
]
sys_ttbar = [
    "__scale0", "__scale1","__scale2","__scale3","__scale4","__scale5",
    "__isrup", "__isrdown", "__fsrup", "__fsrdown", "__pdfup", "__pdfdown"
]

for year in range(16,19):
    if year == 16 and run16 == False: continue
    if year == 17 and run17 == False: continue
    if year == 18 and run18 == False: continue

    p = TProof.Open("", "workers=8")
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
        runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "ttbbClosureTest")
    else:
	runAna(inputDir, "DataSingleEG.root", "DataSingleEG")
	runAna(inputDir, "DataSingleMu.root", "DataSingleMu")

        if year != 16:
	    runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "ttbbClosureTest")
	for sample in samples:
	    if "part" in sample: continue
	    if "SYS" in sample: continue
	    runAna(inputDir, str(sample)+".root", str(sample))

	if do_sys:
	    for value in sys_list:
		for sample in samples:
		    if "SYS" in sample: continue
		    runAna(inputDir, str(sample)+".root", sample+value)

	    for index, value in enumerate(sys_ttbar):
		runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "TTLJ_PowhegPythia_ttbb"+value)
		runAna(inputDir, "TTLJ_PowhegPythia_ttbj.root", "TTLJ_PowhegPythia_ttbj"+value)
		runAna(inputDir, "TTLJ_PowhegPythia_ttcc.root", "TTLJ_PowhegPythia_ttcc"+value)
		runAna(inputDir, "TTLJ_PowhegPythia_ttLF.root", "TTLJ_PowhegPythia_ttLF"+value)
		runAna(inputDir, "TTLJ_PowhegPythia_ttother.root", "TTLJ_PowhegPythia_ttother"+value)
		if "Run2016" in inputDir:
		    runAna(inputDir, "TTLJ_PowhegPythia_ttbbFilter_ttbb.root", "TTLJ_PowhegPythia_ttbbFilter_ttbb"+value)
            # pdf
	    for i in range(0,104):
	        if year == 16 and i == 103: continue
	        runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "TTLJ_PowhegPythia_ttbb__pdf"+str(i))
		runAna(inputDir, "TTLJ_PowhegPythia_ttbj.root", "TTLJ_PowhegPythia_ttbj__pdf"+str(i))
		runAna(inputDir, "TTLJ_PowhegPythia_ttcc.root", "TTLJ_PowhegPythia_ttcc__pdf"+str(i))
		runAna(inputDir, "TTLJ_PowhegPythia_ttLF.root", "TTLJ_PowhegPythia_ttLF__pdf"+str(i))
		runAna(inputDir, "TTLJ_PowhegPythia_ttother.root", "TTLJ_PowhegPythia_ttother__pdf"+str(i))


            # hdamp, Tune
	    for sample in samples:
	        if not "SYS" in sample: continue
		if sample.find("tt") is not -1:
		    tmp = sample[sample.find("SYS"):] + sample[sample.find("tt"):]
		else:
		    tmp = sample[sample.find("SYS")-1:]
		if "hdampUp"   in sample: tmp += "__hdampup"
		if "hdampDown" in sample: tmp += "__hdampdown"
		if "Tune" in sample and "Up"   in sample: tmp += "__tuneup"
		if "Tune" in sample and "Down" in sample: tmp += "__tunedown"
		runAna(inputDir, str(sample)+".root", tmp)

            # 2016 only (Isr, Fsr)
	    if year == 16:
	        runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttbb.root",    "ttbb__isrup")
	        runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttbj.root",    "ttbj__isrup")
	        runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttcc.root",    "ttcc__isrup")
	        runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttLF.root",    "ttLF__isrup")
	        runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttother.root", "ttother__isrup")
		runAna(inputDir, "TT_PowhegPythiaBkg_SYS_ISRup.root",      "ttbkg__isrup")
	        runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttbb.root",    "ttbb__isrdown")
	        runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttbj.root",    "ttbj__isrdown")
	        runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttcc.root",    "ttcc__isrdown")
	        runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttLF.root",    "ttLF__isrdown")
	        runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttother.root", "ttother__isrdown")
		runAna(inputDir, "TT_PowhegPythiaBkg_SYS_ISRdown.root",      "ttbkg__isrdown")

	        runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttbb.root",    "ttbb__fsrup")
	        runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttbj.root",    "ttbj__fsrup")
	        runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttcc.root",    "ttcc__fsrup")
	        runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttLF.root",    "ttLF__fsrup")
	        runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttother.root", "ttother__fsrup")
		runAna(inputDir, "TT_PowhegPythiaBkg_SYS_FSRup.root",      "ttbkg__fsrup")
	        runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttbb.root",    "ttbb__fsrdown")
	        runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttbj.root",    "ttbj__fsrdown")
	        runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttcc.root",    "ttcc__fsrdown")
	        runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttLF.root",    "ttLF__fsrdown")
	        runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttother.root", "ttother__fsrdown")
		runAna(inputDir, "TT_PowhegPythiaBkg_SYS_FSRdown.root",      "ttbkg__fsrdown")

    #outdir = 'output/root'+str(year)
    #if os.path.exists(outdir):
    #    os.system('rm -rf'+outdir)
    #os.system('mv output/root '+outdir)

    p.Close()

    #os.system('root -l -b -q macro/runGentree.C\'("'+inputDir+'/","'+os.getcwd()+'/output/root'+str(year)+'/")\'')
    #os.system('hadd hist_TTLJ_PowhegPythia_ttbb.root output/root'+str(year)+'/hist_TTLJ_PowhegPythia_ttbb.root output/root'+str(year)+'/hist_gen.root')
    #os.system('mv hist_TTLJ_PowhegPythia_ttbb.root output/root'+str(year)+'/')

    #if do_sys:
    #    macro.runPostProcess(os.getcwd(), samples, year)
