import os
import sys
from ROOT import *
import ROOT

def runUnfold(dir1, dir2, file, sys=""):
    if sys is not "": os.system('root -l -b -q ttbbDiffXsec.C\'("'+dir1+'","'+dir2+'","'+file+'","'+sys+'")\'')
    else            : os.system('root -l -b -q ttbbDiffXsec.C\'("'+dir1+'","'+dir2+'","'+file+'")\'')

if not os.path.exists("/home/seohyun/work/ttbb/heptool/output/unfold"):
    os.makedirs("../output/unfold")

inputDir = "/home/seohyun/work/ttbb/heptool/output/post/"
outputDir = "/home/seohyun/work/ttbb/heptool/test/"

"""
sys_list = [
#    "__psup",     "__psdown",      "__swup",      "__swdown",
#    "__jerup",    "__jerdown",     "__jecup",     "__jecdown",
    "__musfup",   "__musfdown",    "__mutrgup",   "__mutrgdown",
    "__elsfup",   "__elsfdown",    "__eltrgup",   "__eltrgdown",
    "__lfup",     "__lfdown",      "__hfup",      "__hfdown",
    "__hfstat1up","__hfstat1down", "__hfstat2up", "__hfstat2down",
    "__lfstat1up","__lfstat1down", "__lfstat2up", "__lfstat2down",
    "__cferr1up", "__cferr1down",  "__cferr2up",  "__cferr2down"
]
"""
runUnfold(inputDir, outputDir, "hist_ttbb"        )
#runUnfold(inputDir, outputDir, "hist_DataSingleEl")
#runUnfold(inputDir, outputDir, "hist_DataSingleMu")

#for syst in sys_list:
#    runUnfold(inputDir, outputDir, "hist_DataSingleEl", syst)
#    runUnfold(inputDir, outputDir, "hist_DataSingleMu", syst)

"""
f_gen = TFile.Open(os.path.join(outputDir, "hist_gen.root"), "recreate")
f_input = TFile.Open(os.path.join(outputDir, "hist_unfolded_hist_ttbb.root"),"read")

f_gen.cd()
tmp = f_input.Get("h_GenbJetDeltaR_Ch0_S3")
tmp.SetName("h_unfolded_dR_Ch0_S3")
tmp.Write()
tmp = f_input.Get("h_GenbJetDeltaR_Ch1_S3")
tmp.SetName("h_unfolded_dR_Ch1_S3")
tmp.Write()
tmp = f_input.Get("h_GenbJetInvMass_Ch0_S3")
tmp.SetName("h_unfolded_M_Ch0_S3")
tmp.Write()
tmp = f_input.Get("h_GenbJetInvMass_Ch1_S3")
tmp.SetName("h_unfolded_M_Ch1_S3")
tmp.Write()
tmp = f_input.Get("h_diffXsec_dR_lep")
tmp.SetName("h_diffXsec_dR_Ch0")
tmp.Write()
tmp.SetName("h_diffXsec_dR_Ch1")
tmp.Write()
tmp = f_input.Get("h_diffXsec_M_lep")
tmp.SetName("h_diffXsec_M_Ch0")
tmp.Write()
tmp.SetName("h_diffXsec_M_Ch1")
tmp.Write()
f_gen.Write()
f_gen.Close()
f_input.Close()
"""
#os.system('mv '+outputDir+'hist_gen.root /home/seohyun/work/ttbb/heptool/output/unfold/')
#os.system('hadd /home/seohyun/work/ttbb/heptool/output/unfold/hist_unfolded_hist_DataSingleMu.root '+outputDir+'hist_unfolded_hist_DataSingleMu*')
#os.system('hadd /home/seohyun/work/ttbb/heptool/output/unfold/hist_unfolded_hist_DataSingleEl.root '+outputDir+'hist_unfolded_hist_DataSingleEl*')

