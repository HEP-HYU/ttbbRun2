#! /usr/bin/env python
import os
import multiprocessing as mp
import time
from tqdm import tqdm

from ROOT import *
import ROOT

import variableAnalyzer as var
import ana

#Options
ana_test = False
ana_syst = False

start_time = time.time()

ntupleDir = '/data/users/seohyun/ntuple/hep2019/split/'
arrayDir = './array/'
histDir = './hist/'

syslist = [
    '__puup', '__pudown',
    '__musfup', '__musfdown', '__mutrgup', '__mutrgdown',
    '__elsfup', '__elsfdown', '__eltrgup', '__eltrgdown',
    '__lfup', '__lfdown', '__hfup', '__hfdown',
    '__hfstat1up', '__hfstat1down', '__hfstat2up', '__hfstat2down',
    '__lfstat1up', '__lfstat1down', '__lfstat2up', '__lfstat2down',
    '__cferr1up', '__cferr1down', '__cferr2up', '__cferr2down',
    '__scale0', '__scale1', '__scale2', '__scale3', '__scale4', '__scale5',
    '__pdfup', '__pdfdown'
]

if ana_test:
    ana.ana('/home/seohyun/work/heptool/deepAna/keras/array/TTLJ_PowhegPythia_ttbb','TTLJ_PowhegPythia_ttbb','test')

else:
    for process in os.listdir(arrayDir):
	#if not "ttbbFilter" in process: continue
	if process.endswith(".h5"): continue

	if not ('SYS' in process or '__' in process or 'Herwig' in process or 'Evtgen' in process or 'TT_aMCatNLO' in process):
	    print("Start "+str(process))
	    ana.ana(arrayDir+process, process, histDir)

	    if ana_syst and not 'Data' in process:
		for sys in syslist:
		    if 'scale' in sys and not 'tt' in process: continue
		    ana.ana(arrayDir+process, process, histDir, sys)
	else:
	    if   'FSRup'   in process: sys = '__fsrup'
	    elif 'FSRdown' in process: sys = '__fsrdown'
	    elif 'ISRup'   in process: sys = '__isrup'
	    elif 'ISRdown' in process: sys = '__isrdown'
	    elif 'hdampUp' in process: sys = '__hdampup'
	    elif 'hdampDown' in process: sys = '__hdampdown'
	    elif 'TuneUp'  in process: sys = '__tuneup'
	    elif 'TuneDown'in process: sys = '__tunedown'
	    elif 'jecdown' in process: sys = '__jecdown'
	    elif 'jecup'   in process: sys = '__jecup'
	    elif 'jerdown' in process: sys = '__jerdown'
	    elif 'jerup'   in process: sys = '__jerup'
	    elif 'Herwig'  in process: sys = '__powhegherwig'
	    elif 'TT_aMCatNLO'in process: sys = '__amcatnlopythia'
	    elif 'Evtgen'  in process: sys = '__powhegpythiaevtgen'
	    else:                      sys = ''
	    ana.ana(arrayDir+process, process, histDir, sys)

print("Total running time :%s " %(time.time() - start_time))
