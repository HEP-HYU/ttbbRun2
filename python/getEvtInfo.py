import os, sys
from ROOT import *
import ROOT

for year in range(16,19):
    f_genevt = open('../samples/genevt'+str(year)+'.txt','w')

    samples = []
    with open('../samples/sample'+str(year)+'.txt','r') as f:
        n = 1
	while True:
            line = f.readline()
	    if not line: break
	    if n == 1: base_path = line[:-1]
	    else: samples.append(line[:-1])
	    n += 1

    for sample in samples:
        f_ntuple = ROOT.TFile(base_path+'/'+sample+'.root')
	h_info = f_ntuple.Get('ttbbLepJets/EventInfo')
	f_genevt.write(sample+' '+str(h_info.GetBinContent(2))+'\n')

    f_genevt.close()
