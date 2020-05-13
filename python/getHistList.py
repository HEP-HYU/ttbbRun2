import os, sys
from ROOT import *
import ROOT

f_data = ROOT.TFile("../output/root16_post/hist_DataSingleMu.root")
hist_list = [x.GetName() for x in f_data.GetListOfKeys()]

f_hist = open("../plotIt/configs/histos_ttlj.yml", 'w')

for hist in hist_list:
    if "Disc" in hist: continue
    if "Gen"  in hist: continue
    if "Resp" in hist: continue
    if "EventInfo" in hist: continue
    if "ScaleWeights" in hist: continue
    if "bSF" in hist: continue
    f_hist.write("'"+hist+"':\n")
    f_hist.write('  y-axis: "Events"\n')
    f_hist.write('  y-axis-format: "%1%"\n')
    f_hist.write("  save-extensions: ['pdf']\n")
    f_hist.write('  log-y: true\n')
    f_hist.write('  show-ratio: true\n\n')
