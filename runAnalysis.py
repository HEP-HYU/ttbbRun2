#!/usr/bin/python
<<<<<<< HEAD
run16  = True
run17  = True
run18  = True
test   = False
do_sys = False

import os
import sys
=======
test = False 
do_sys = True

>>>>>>> fbc2120cbe49b2828e2fe3719e5c264f9736392d
from ROOT import TChain, TProof, TFile, TH1D, TH1F, TCanvas

import macro.getSampleList
import macro.runPostProcess

def runAna(dir, file, name):
<<<<<<< HEAD
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

macro.getSampleList()

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
	runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "ttbb_test")
    else:
	runAna(inputDir, "DataSingleEG.root", "DataSingleEG")
	runAna(inputDir, "DataSingleMu.root", "DataSingleMu")

	for index, sample in enumerate(samples):
	    runAna(inputDir, str(sample)+".root", str(sample))

	if do_sys:
	    for index, value in enumerate(sys_list):
		for sample in enumerate(samples):
		    runAna(inputDir, str(sample)+".root", sample+value)

	    for index, value in enumerate(sys_ttbar):
		runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "TTLJ_PowhegPythia_ttbb"+value)
		runAna(inputDir, "TTLJ_PowhegPythia_ttbj.root", "TTLJ_PowhegPythia_ttbj"+value)
		runAna(inputDir, "TTLJ_PowhegPythia_ttcc.root", "TTLJ_PowhegPythia_ttcc"+value)
		runAna(inputDir, "TTLJ_PowhegPythia_ttLF.root", "TTLJ_PowhegPythia_ttLF"+value)
		runAna(inputDir, "TTLJ_PowhegPythia_ttother.root", "TTLJ_PowhegPythia_ttother"+value)
		if "Run2016" in inputDir:
		    runAna(inputDir, "TTLJ_PowhegPythia_ttbbFilter_ttbb.root", "TTLJ_PowhegPythia_ttbbFilter_ttbb"+value)

    outdir = 'output/root'+str(year)
    os.system('mv output/root '+outdir)

    p.Close()

    os.system('root -l -b -q macro/runGentree.C\'("'+inputDir+'/","'+os.getcwd()+'/output/root'+str(year)+'/")\'')
    os.system('hadd hist_TTLJ_PowhegPythia_ttbb.root output/root'+str(year)+'/hist_TTLJ_PowhegPythia_ttbb.root output/root'+str(year)'/hist_gen.root')
    os.system('mv hist_TTLJ_PowhegPythia_ttbb.root output/root'+str(year)+'/')

    if do_sys:
        macro.runPostProcess(os.getcwd(), samples, year)

=======
  chain = TChain("ttbbLepJets/tree","events")
  chain.Add(dir+"/"+file)
  chain.SetProof();
  chain.Process("macro/MyAnalysis.C+",name)

  f = TFile(dir+"/"+file,"read")

  ## save Event Summary histogram ##
  out = TFile("output/root/hist_"+name+".root","update")
  hevt = f.Get("ttbbLepJets/EventInfo")
  hsw = f.Get("ttbbLepJets/ScaleWeights")
  hevt.Write()
  hsw.Write()
  out.Write()
  out.Close()

p = TProof.Open("", "workers=8")

inputDir = "/data/users/seohyun/ntuple/Run2016/v808/nosplit/"

if test:
  runAna(inputDir, "TTLJ_PowhegPythia_ttbbFilter_ttbb.root", "ttbbFilterttbb")
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
  sys_list2 = ["__scale0", "__scale1","__scale2","__scale3","__scale4","__scale5"]
  for index, value in enumerate(sys_list):
    runAna(inputDir, "TTLJ_PowhegPythia_ttbbFilter_ttbb.root", "ttbbFilterttbb"+value)
  for index, value in enumerate(sys_list2):
    runAna(inputDir, "TTLJ_PowhegPythia_ttbbFilter_ttbb.root", "ttbbFilterttbb"+value)

else:
  # v808 #
  ### Basic plots ###
  runAna(inputDir,"DataSingleEG.root","DataSingleEl")
  runAna(inputDir,"DataSingleMu.root","DataSingleMu")
  runAna(inputDir, "TTLJ_PowhegPythia_ttbbFilter_ttbb.root", "ttbbFilterttbb")

  runAna(inputDir,"TTLJ_PowhegPythia_ttbb.root","ttbb")
  runAna(inputDir,"TTLJ_PowhegPythia_ttbj.root","ttbj")
  runAna(inputDir,"TTLJ_PowhegPythia_ttcc.root","ttcc")
  runAna(inputDir,"TTLJ_PowhegPythia_ttLF.root","ttLF")
  runAna(inputDir,"TTLJ_PowhegPythia_ttother.root","ttother")

  runAna(inputDir,"WJets_aMCatNLO.root","wjets")
  runAna(inputDir,"ZJets_M10to50_aMCatNLO.root","zjets10to50")
  runAna(inputDir,"ZJets_M50_aMCatNLO.root","zjets")
  runAna(inputDir,"WW_Pythia.root","ww")
  runAna(inputDir,"WZ_Pythia.root","wz")
  runAna(inputDir,"ZZ_Pythia.root","zz")
  runAna(inputDir,"t_Powheg.root","tchannel")
  runAna(inputDir,"tbar_Powheg.root","tbarchannel")
  runAna(inputDir,"tW_Powheg.root","tWchannel")
  runAna(inputDir,"tbarW_Powheg.root","tbarWchannel")
  runAna(inputDir,"ttbar_PowhegPythiaBkg.root","ttbkg")

  runAna(inputDir,"ttHbb_PowhegPythia.root","ttH")
  runAna(inputDir,"ttW_Madgraph.root","ttW")
  runAna(inputDir,"ttZ_Madgraph.root","ttZ")

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
  #####
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
  sys_list2 = ["__scale0", "__scale1","__scale2","__scale3","__scale4","__scale5"]

  if do_sys:
    for index, value in enumerate(sys_list):
      runAna(inputDir, "TTLJ_PowhegPythia_ttbbFilter_ttbb.root", "ttbbFilterttbb"+value)

      runAna(inputDir,"TTLJ_PowhegPythia_ttbb.root","ttbb"+value)
      runAna(inputDir,"TTLJ_PowhegPythia_ttbj.root","ttbj"+value)
      runAna(inputDir,"TTLJ_PowhegPythia_ttcc.root","ttcc"+value)
      runAna(inputDir,"TTLJ_PowhegPythia_ttLF.root","ttLF"+value)
      runAna(inputDir,"TTLJ_PowhegPythia_ttother.root","ttother"+value)

      runAna(inputDir,"WJets_aMCatNLO.root","wjets"+value)
      runAna(inputDir,"ZJets_M10to50_aMCatNLO.root","zjets10to50"+value)
      runAna(inputDir,"ZJets_M50_aMCatNLO.root","zjets"+value)
      runAna(inputDir,"WW_Pythia.root","ww"+value)
      runAna(inputDir,"WZ_Pythia.root","wz"+value)
      runAna(inputDir,"ZZ_Pythia.root","zz"+value)
      runAna(inputDir,"t_Powheg.root","tchannel"+value)
      runAna(inputDir,"tbar_Powheg.root","tbarchannel"+value)
      runAna(inputDir,"tW_Powheg.root","tWchannel"+value)
      runAna(inputDir,"tbarW_Powheg.root","tbarWchannel"+value)
      runAna(inputDir,"ttbar_PowhegPythiaBkg.root","ttbkg"+value)

      runAna(inputDir,"ttHbb_PowhegPythia.root","ttH"+value)
      runAna(inputDir,"ttW_Madgraph.root","ttW"+value)
      runAna(inputDir,"ttZ_Madgraph.root","ttZ"+value)

    for index, value in enumerate(sys_list2):
      runAna(inputDir, "TTLJ_PowhegPythia_ttbbFilter_ttbb.root", "ttbbFilterttbb"+value)
      runAna(inputDir,"TTLJ_PowhegPythia_ttbb.root","ttbb"+value)
      runAna(inputDir,"TTLJ_PowhegPythia_ttbj.root","ttbj"+value)
      runAna(inputDir,"TTLJ_PowhegPythia_ttcc.root","ttcc"+value)
      runAna(inputDir,"TTLJ_PowhegPythia_ttLF.root","ttLF"+value)
      runAna(inputDir,"TTLJ_PowhegPythia_ttother.root","ttother"+value)
      runAna(inputDir,"ttbar_PowhegPythiaBkg.root","ttbkg"+value)
      runAna(inputDir,"ttHbb_PowhegPythia.root","ttH"+value)
      runAna(inputDir,"ttW_Madgraph.root","ttW"+value)
      runAna(inputDir,"ttZ_Madgraph.root","ttZ"+value)

    runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttbb.root","ttbb__isrup")
    runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttbj.root","ttbj__isrup")
    runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttcc.root","ttcc__isrup")
    runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttLF.root","ttLF__isrup")
    runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttother.root","ttother__isrup")
    runAna(inputDir, "ttbar_PowhegPythiaBkg_SYS_ISRup.root","ttbkg__isrup")

    runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttbb.root","ttbb__isrdown")
    runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttbj.root","ttbj__isrdown")
    runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttcc.root","ttcc__isrdown")
    runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttLF.root","ttLF__isrdown")
    runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttother.root","ttother__isrdown")
    runAna(inputDir, "ttbar_PowhegPythiaBkg_SYS_ISRdown.root","ttbkg__isrdown")

    runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttbb.root","ttbb__fsrup")
    runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttbj.root","ttbj__fsrup")
    runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttcc.root","ttcc__fsrup")
    runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttLF.root","ttLF__fsrup")
    runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttother.root","ttother__fsrup")
    runAna(inputDir, "ttbar_PowhegPythiaBkg_SYS_FSRup.root","ttbkg__fsrup")

    runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttbb.root","ttbb__fsrdown")
    runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttbj.root","ttbj__fsrdown")
    runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttcc.root","ttcc__fsrdown")
    runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttLF.root","ttLF__fsrdown")
    runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttother.root","ttother__fsrdown")
    runAna(inputDir, "ttbar_PowhegPythiaBkg_SYS_FSRdown.root","ttbkg__fsrdown")

    runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdampUp_ttbb.root", "ttbb__hdampup")
    runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdampUp_ttbj.root", "ttbj__hdampup")
    runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdampUp_ttcc.root", "ttcc__hdampup")
    runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdampUp_ttLF.root", "ttLF__hdampup")
    runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdampUp_ttother.root", "ttother__hdampup")

    runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdampDown_ttbb.root", "ttbb__hdampdown")
    runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdampDown_ttbj.root", "ttbj__hdampdown")
    runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdampDown_ttcc.root", "ttcc__hdampdown")
    runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdampDown_ttLF.root", "ttLF__hdampdown")
    runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdampDown_ttother.root", "ttother__hdampdown")

    runAna(inputDir, "TT_PowhegPythia_SYS_TuneUp_ttbb.root", "ttbb__tuneup")
    runAna(inputDir, "TT_PowhegPythia_SYS_TuneUp_ttbj.root", "ttbj__tuneup")
    runAna(inputDir, "TT_PowhegPythia_SYS_TuneUp_ttcc.root", "ttcc__tuneup")
    runAna(inputDir, "TT_PowhegPythia_SYS_TuneUp_ttLF.root", "ttLF__tuneup")
    runAna(inputDir, "TT_PowhegPythia_SYS_TuneUp_ttother.root", "ttother__tuneup")

    runAna(inputDir, "TT_PowhegPythia_SYS_TuneDown_ttbb.root", "ttbb__tunedown")
    runAna(inputDir, "TT_PowhegPythia_SYS_TuneDown_ttbj.root", "ttbj__tunedown")
    runAna(inputDir, "TT_PowhegPythia_SYS_TuneDown_ttcc.root", "ttcc__tunedown")
    runAna(inputDir, "TT_PowhegPythia_SYS_TuneDown_ttLF.root", "ttLF__tunedown")
    runAna(inputDir, "TT_PowhegPythia_SYS_TuneDown_ttother.root", "ttother__tunedown")
>>>>>>> fbc2120cbe49b2828e2fe3719e5c264f9736392d
