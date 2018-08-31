#!/usr/bin/python

from ROOT import TChain, TProof, TFile, TH1D, TH1F, TCanvas

def runAna(dir, file, name):
  chain = TChain("ttbbLepJets/tree","events")
  chain.Add(dir+"/"+file)
  chain.SetProof();
  chain.Process("macro/MyAnalysis.C+",name)

  f = TFile(dir+"/"+file,"read")

  ## save Event Summary histogram ##
  out = TFile("output/root/hist_"+name+".root","update")
  hevt = f.Get("ttbbLepJets/EventInfo")
  hevt.Write()
  out.Write()
  out.Close()

p = TProof.Open("", "workers=8")

### v808 ###
runAna("/data/users/seohyun/ntuple/hep2017/v808/","DataSingleEG.root","DataSingleEl")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","DataSingleMu.root","DataSingleMu")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","WJets_aMCatNLO.root","wjets")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","ZJets_M10to50_aMCatNLO.root","zjets10to50")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","ZJets_M50_aMCatNLO.root","zjets")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","WW_Pythia.root","ww")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","WZ_Pythia.root","wz")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","ZZ_Pythia.root","zz")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","t_Powheg.root","tchannel")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","tbar_Powheg.root","tbarchannel")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","tW_Powheg.root","tWchannel")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","tbarW_Powheg.root","tbarWchannel")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","ttbar_PowhegPythiaBkg.root","ttbkg")

runAna("/data/users/seohyun/ntuple/hep2017/v808/","ttHbb_PowhegPythia.root","ttH")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","ttW_Madgraph.root","ttW")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","ttZ_Madgraph.root","ttZ")

runAna("/data/users/seohyun/ntuple/hep2017/v808/","TTLJ_PowhegPythia_ttbb.root","ttbb")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","TTLJ_PowhegPythia_ttbj.root","ttbj")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","TTLJ_PowhegPythia_ttcc.root","ttcc")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","TTLJ_PowhegPythia_ttjj.root","ttjj")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","TTLJ_PowhegPythia_ttLF.root","ttLF")
runAna("/data/users/seohyun/ntuple/hep2017/v808/","TTLJ_PowhegPythia_ttother.root","ttother")

"""
### v806 ###
runAna("/data/users/jhgoh/hep2016/v806_1/","DataSingleEG.root","DataSingleEl")
runAna("/data/users/jhgoh/hep2016/v806_1/","DataSingleMu.root","DataSingleMu")
runAna("/data/users/jhgoh/hep2016/v806_1/","WJets_aMCatNLO.root","wjets")
runAna("/data/users/jhgoh/hep2016/v806_1/","ZJets_M50_aMCatNLO.root","zjets")
runAna("/data/users/jhgoh/hep2016/v806_1/","ZJets_M10to50_aMCatNLO.root","zjets10to50")
runAna("/data/users/jhgoh/hep2016/v806_1/","WW_Pythia.root","ww")
runAna("/data/users/jhgoh/hep2016/v806_1/","WZ_Pythia.root","wz")
runAna("/data/users/jhgoh/hep2016/v806_1/","ZZ_Pythia.root","zz")
runAna("/data/users/jhgoh/hep2016/v806_1/","t_tchannel_Powheg.root","tchannel")
runAna("/data/users/jhgoh/hep2016/v806_1/","tbar_tchannel_Powheg.root","tbarchannel")
runAna("/data/users/jhgoh/hep2016/v806_1/","tW_Powheg.root","tWchannel")
runAna("/data/users/jhgoh/hep2016/v806_1/","tbarW_Powheg.root","tbarWchannel")
runAna("/data/users/jhgoh/hep2016/v806_1/","ttbar_LepJetsPowhegPythiattbb.root","ttbb")
runAna("/data/users/jhgoh/hep2016/v806_1/","ttbar_LepJetsPowhegPythiattbj.root","ttbj")
runAna("/data/users/jhgoh/hep2016/v806_1/","ttbar_LepJetsPowhegPythiattcc.root","ttcc")
runAna("/data/users/jhgoh/hep2016/v806_1/","ttbar_LepJetsPowhegPythiattLF.root","ttLF")
runAna("/data/users/jhgoh/hep2016/v806_1/","ttbar_LepJetsPowhegPythiatt.root","tt")
runAna("/data/users/jhgoh/hep2016/v806_1/","ttbar_PowhegPythiaBkg.root","ttbkg")
runAna("/data/users/jhgoh/hep2016/v806_1/","ttHbb_PowhegPythia.root","ttH")
runAna("/data/users/jhgoh/hep2016/v806_1/","ttZ_Madgraph.root","ttW")
runAna("/data/users/jhgoh/hep2016/v806_1/","ttW_Madgraph.root","ttZ")

### QCD ###
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_EGEnr_20to30.root","qcdEl20to30")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_EGEnr_30to50.root","qcdEl30to50")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_EGEnr_50to80.root","qcdEl50to80")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_EGEnr_80to120.root","qcdEl80to120")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_EGEnr_120to170.root","qcdEl120to170")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_EGEnr_170to300.root","qcdEl170to300")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_EGEnr_300toInf.root","qcdEl300toInf")

runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_MuEnr_20to30.root","qcdMu20to30")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_MuEnr_30to50.root","qcdMu30to50")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_MuEnr_50to80.root","qcdMu50to80")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_MuEnr_80to120.root","qcdMu80to120")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_MuEnr_120to170.root","qcdMu120to170")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_MuEnr_170to300.root","qcdMu170to300")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_MuEnr_300to470.root","qcdMu300to470")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_MuEnr_470to600.root","qcdMu470to600")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_MuEnr_600to800.root","qcdMu600to800")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_MuEnr_800to1000.root","qcdMu800to1000")
runAna("/data/users/jhgoh/hep2016/v806_1/","QCD_MuEnr_1000toInf.root","qcdMu1000toInf")
"""
### background ###
"""
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

### signal ###
#### Powheg + Pythia ####
runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_tt.root","tt")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttbb.root","ttbb")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttbj.root","ttbj")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttjj.root","ttjj")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttcc.root","ttcc")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttlf.root","ttLF")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttbkg.root","ttbkg")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_ttHbb.root","ttH")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_ttW.root","ttW")
runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_ttZ.root","ttZ")
###### ttbkg == ttother ######
#runAna("/data/users/seohyun/ntuple/hep2017/v806/","TT_powheg_ttother.root","ttother")
"""
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
