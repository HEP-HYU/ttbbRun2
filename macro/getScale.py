import os, sys
from ROOT import *
import ROOT

for year in range(16, 19):
    f_fit = open('../samples/scale'+str(year)+'.txt','w')

    fit = {}
    with open('../samples/fit'+str(year)+'.txt','r') as f:
        while True:
            line = f.readline()
	    if not line: break
	    tmp = line.split(' ')
	    fit[tmp[0]] = float(tmp[1])

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
        if 'ttbb' in sample or 'ttW' in sample or 'ttZ' in sample or 'ttH' in sample:
	    if 'Filter' in sample:
	        f_fit.write(sample+' '+str(1.0)+'\n')
	    else:
	        f_fit.write(sample+' '+str(fit['ttbbttX'])+'\n')
	elif 'ttbj' in sample:
	    f_fit.write(sample+' '+str(fit['ttbj'])+'\n')
	elif 'ttLF' in sample or 'ttcc' in sample or 'Bkg' in sample or 'Single' in sample:
	    f_fit.write(sample+' '+str(fit['STttbkgLFcc'])+'\n')
	else:
	    f_fit.write(sample+' '+str(fit['WZJetsQCDVV'])+'\n')
