#!/usr/bin/python

from ROOT import TChain, TProof, TFile, TH1D, TH1F, TCanvas

def runAna(dir, file, name):
  chain = TChain("ttbbLepJets/tree","events")
  chain.Add(dir+"/"+file)
#  chain.SetProof();
  chain.Process("macro/MyAnalysis.C+",name)

  f = TFile(dir+"/"+file,"read")

  ## save Event Summary histogram ##
  out = TFile("output/root/hist_"+name+".root","update")
  hevt = f.Get("ttbbLepJets/EventInfo")
  hevt.Write()
  out.Write()
  out.Close()

#p = TProof.Open("", "workers=8")

inputDir = "/data/users/seohyun/ntuple/hep2017/v808/nosplit/"

# v808 #
### Basic plots ###
runAna(inputDir,"DataSingleEG.root","DataSingleEl")
runAna(inputDir,"DataSingleMu.root","DataSingleMu")
runAna(inputDir, "TTLJ_PowhegPythia_ttbbFilter_ttbb.root", "ttbbFilter_ttbb")

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
    "__puup","__pudown",
    "__muidup","__muiddown","__muisoup","__muisodown","__mutrgup","__mutrgdown",
    "__elidup","__eliddown","__elrecoup","__elrecodown","__elzvtxup","__elzvtxdown",
    "__eltrgup","__eltrgdown",
    "__lfup","__lfdown","__hfup","__hfdown",
    "__hfstat1up","__hfstat1down","__hfstat2up","__hfstat2down",
    "__lfstat1up","__lfstat1down","__lfstat2up","__lfstat2down",
    "__cferr1up","__cferr1down","__cferr2up","__cferr2down",
    "__scale0","__scale1","__scale2","__scale3","__scale4","__scale5"
]

for index, value in enumerate(sys_list):
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

runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttbb","ttbb__isrup")
runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttbj","ttbj__isrup")
runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttcc","ttcc__isrup")
runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttLF","ttLF__isrup")
runAna(inputDir, "TT_PowhegPythia_SYS_ISRup_ttother","ttother__isrup")
runAna(inputDir, "ttbar_PowhegPythiaBkg_SYS_ISRup","ttbkg__isrup")

runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttbb","ttbb__isrdown")
runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttbj","ttbj__isrdown")
runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttcc","ttcc__isrdown")
runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttLF","ttLF__isrdown")
runAna(inputDir, "TT_PowhegPythia_SYS_ISRdown_ttother","ttother__isrdown")
runAna(inputDir, "ttbar_PowhegPythiaBkg_SYS_ISRdown","ttbkg__isrdown")

runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttbb","ttbb__fsrup")
runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttbj","ttbj__fsrup")
runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttcc","ttcc__fsrup")
runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttLF","ttLF__fsrup")
runAna(inputDir, "TT_PowhegPythia_SYS_FSRup_ttother","ttother__fsrup")
runAna(inputDir, "ttbar_PowhegPythiaBkg_SYS_FSRup","ttbkg__fsrup")

runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttbb","ttbb__fsrdown")
runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttbj","ttbj__fsrdown")
runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttcc","ttcc__fsrdown")
runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttLF","ttLF__fsrdown")
runAna(inputDir, "TT_PowhegPythia_SYS_FSRdown_ttother","ttother__fsrdown")
runAna(inputDir, "ttbar_PowhegPythiaBkg_SYS_FSRdown","ttbkg__fsrdown")

runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdamuUp_ttbb", "ttbb__hdampup")
runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdamuUp_ttbj", "ttbj__hdampup")
runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdamuUp_ttcc", "ttcc__hdampup")
runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdamuUp_ttLF", "ttLF__hdampup")
runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdamuUp_ttother", "ttother__hdampup")
runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdamuUp_ttbb", "ttbb__hdampdown")
runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdamuUp_ttbj", "ttbj__hdampdown")
runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdamuUp_ttcc", "ttcc__hdampdown")
runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdamuUp_ttLF", "ttLF__hdampdown")
runAna(inputDir, "TTLJ_PowhegPythia_SYS_hdamuUp_ttother", "ttother__hdampdown")

