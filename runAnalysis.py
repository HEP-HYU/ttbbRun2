#!/usr/bin/python

from ROOT import TChain, TProof, TFile, TH1D, TH1F, TCanvas

def runAna(dir, file, name):
  chain = TChain("ttbbLepJets/tree","events")
  chain.Add(dir+"/"+file)
  chain.SetProof();
  chain.Process("MyAnalysis/MyAnalysis.C+",name)

  f = TFile(dir+"/"+file,"read")

  ## save Event Summary histogram ##
  out = TFile("output/root/hist_"+name+".root","update")
  hevt = f.Get("ttbbLepJets/EventInfo")
  hevt.Write()
  out.Write()
  out.Close()

p = TProof.Open("", "workers=8")

### background ###
'''
runAna("/data/users/seohyun/ntuple/hep2017/v806/","SingleElectron_Run2016.root","DataSingleEl")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","SingleMuon_Run2016.root","DataSingleMu")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","WJets.root","wjets")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","DYJets.root","zjets")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","DYJets_10to50.root","zjets10to50")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","WW.root","ww")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","WZ.root","wz")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","ZZ.root","zz")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","SingleTop_t.root","tchannel")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","SingleTbar_t.root","tbarchannel")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","SingleTop_tW.root","tWchannel")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","SingleTbar_tW.root","tbarWchannel")
'''
### signal ###
#### Powheg + Pythia ####
#runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_tt.root","tt")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttbb.root","ttbb")
#runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttbj.root","ttbj")
#runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttjj.root","ttjj")
#runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttcc.root","ttcc")
#runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttlf.root","ttLF")
#runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttother.root","ttother")
#runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttbkg.root","ttbkg")
###### ttbkg == ttother ######

'''
#### Powheg + Pythia + EvtGen ####
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegPythiaEvt.root","tt")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegPythiaEvt_ttbb.root","ttbb")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegPythiaEvt_ttbj.root","ttbj")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegPythiaEvt_ttcc.root","ttcc")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegPythiaEvt_ttLF.root","ttLF")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegPythiaEvt_ttjj.root","ttjj")

#### Powheg + Herwig ####
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegHerwig.root","tt")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegHerwig_ttbb.root","ttbb")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegHerwig_ttbj.root","ttbj")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegHerwig_ttcc.root","ttcc")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegHerwig_ttLF.root","ttLF")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegHerwig_ttjj.root","ttjj")

#### aMCatNLO + Pythia ####
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_aMCatNLOPythia.root","tt")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_aMCatNLOPythia_ttbb.root","ttbb")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_aMCatNLOPythia_ttbj.root","ttbj")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_aMCatNLOPythia_ttcc.root","ttcc")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_aMCatNLOPythia_ttLF.root","ttLF")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_aMCatNLOPythia_ttjj.root","ttjj")

#### Powheg + Pythia ( sys central ) ####
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegPythia_SYS_Central.root","tt")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegPythia_SYS_Central_ttbb.root","ttbb")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegPythia_SYS_Central_ttbj.root","ttbj")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegPythia_SYS_Central_ttcc.root","ttcc")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegPythia_SYS_Central_ttLF.root","ttLF")
#runAna("/data/users/seohyun/ntuple/hep2017/Generator/","TT_PowhegPythia_SYS_Central_ttjj.root","ttjj")

runAna("/data/users/seohyun/ntuple/hep2017/Generator/","Tree_LepJets_FallSkim_v8-0-6_Spring16-80X_36814pb-1_ttbb_aMCatNLOttbb.root","ttbb")
'''
