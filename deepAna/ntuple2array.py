#! /usr/bin/env python
import numpy as np
from numpy.lib.recfunctions import stack_arrays
from ROOT import *
from root_numpy import tree2array
from array import array
import math
import glob
import pandas as pd
import deepdish.io as io

import os
import multiprocessing as mp
import time
from tqdm import tqdm

import utils as ut

def transversemass(vec1, met):
    tmp1 = TLorentzVector(vec1.Px(), vec1.Py(), 0, vec1.E()*math.sin(vec1.Theta()))
    tmp2 = TLorentzVector(met.Px(), met.Py(), 0, met.E());

    return (tmp1+tmp2).M()

def makeCombi(inputDir, inputFile, outputDir, makeTrainingInput=False, sys=''):
    print(str(inputDir+"/"+inputFile)+" start")
    chain = TChain("ttbbLepJets/tree")
    chain.Add(inputDir+"/"+inputFile)

    data = False
    if 'Data' in inputDir: data = True
    ttbb = False
    if 'ttbb' in inputDir: ttbb = True
    if makeTrainingInput:  ttbb = True

    muon_ch = 0
    muon_pt = 30.0
    muon_eta = 2.4
    electron_ch = 1
    electron_pt = 30.0
    electron_eta = 2.4
    jet_pt = 30.0
    jet_eta = 2.4
    jet_CSV_tight = 0.8001

    jetCombi = []
    for i in xrange(chain.GetEntries()) :
        chain.GetEntry(i)

        lepton_SF = 1.0
        jet_SF_CSV = 1.0
        pdfweight = []
        scaleweight = []
        PUWeight = []
        lepton_SF = []
        jet_SF_CSV_30 = []
        if not data:
            for j in xrange((chain.lepton_SF).size()):
                lepton_SF.append(float(chain.lepton_SF[j]))
            for j in xrange((chain.jet_SF_deepCSV_30).size()):
                jet_SF_CSV_30.append(float(chain.jet_SF_deepCSV_30[j]))
            for j in xrange((chain.PUWeight).size()):
                PUWeight.append(float(chain.PUWeight[j]))
        if 'TT' in inputDir or 'tt' in inputDir:
            for j in xrange((chain.scaleweight).size()):
                scaleweight.append(float(chain.scaleweight[j]))
            for j in xrange((chain.pdfweight).size()):
                pdfweight.append(float(chain.pdfweight[j]))

        MET_px = chain.MET*math.cos(chain.MET_phi)
        MET_py = chain.MET*math.sin(chain.MET_phi)
        nu = TLorentzVector(MET_px, MET_py, 0, chain.MET)

        lep = TLorentzVector()
        lep.SetPtEtaPhiE(chain.lepton_pt, chain.lepton_eta, chain.lepton_phi, chain.lepton_e)
        passmuon = False
        passelectron = False
        passmuon = chain.channel == muon_ch and lep.Pt() > muon_pt and abs(lep.Eta()) < muon_eta
        passelectron = chain.channel == electron_ch and lep.Pt() > electron_pt and abs(lep.Eta()) < electron_eta
        if passmuon == False and passelectron == False:
            continue

        addbjet1 = TLorentzVector(0,0,0,0)
        addbjet2 = TLorentzVector(0,0,0,0)
        if ttbb:
            addbjet1.SetPtEtaPhiE(chain.addbjet1_pt,chain.addbjet1_eta,chain.addbjet1_phi,chain.addbjet1_e)
            addbjet2.SetPtEtaPhiE(chain.addbjet2_pt,chain.addbjet2_eta,chain.addbjet2_phi,chain.addbjet2_e)
        njets = 0
        nbjets = 0
        addbjet1_matched = TLorentzVector(0,0,0,0)
        addbjet2_matched = TLorentzVector(0,0,0,0)

        for iJet in range(len(chain.jet_pt)):
            jet = TLorentzVector()
            jet.SetPtEtaPhiE(chain.jet_pt[iJet],chain.jet_eta[iJet],chain.jet_phi[iJet],chain.jet_e[iJet])

            if not data :
                if   'jecup'   in sys:
                    jet *= chain.jet_JER_Nom[iJet] * chain.jet_JES_Up[iJet]
                elif 'jecdown' in sys:
                    jet *= chain.jet_JER_Nom[iJet] * chain.jet_JES_Down[iJet]
                elif 'jerup'   in sys:
                    jet *= chain.jet_JER_Up[iJet]
                elif 'jerdown' in sys:
                    jet *= chain.jet_JER_Down[iJet]
                else:
                    jet *= chain.jet_JER_Nom[iJet]

            if jet.Pt() < jet_pt or abs(jet.Eta()) > jet_eta: continue
            njets += 1
            if chain.jet_deepCSV[iJet] > jet_CSV_tight: nbjets += 1

            if addbjet1.DeltaR(jet) < 0.4: addbjet1_matched = jet;
            if addbjet2.DeltaR(jet) < 0.4: addbjet2_matched = jet;

        if njets < 6 or nbjets < 3: continue
        #print("addbjet1: "+str(addbjet1.Pt())+" matched: "+str(addbjet1_matched.Pt()))
        #print("addbjet2: "+str(addbjet2.Pt())+" matched: "+str(addbjet2_matched.Pt()))

        for j in range(len(chain.jet_pt)-1):
            for k in range(j+1, len(chain.jet_pt)):
                if chain.jet_deepCSV[j] > jet_CSV_tight and chain.jet_deepCSV[k] > jet_CSV_tight:
                    b1 = TLorentzVector()
                    b2 = TLorentzVector()
                    b1.SetPtEtaPhiE(chain.jet_pt[j], chain.jet_eta[j], chain.jet_phi[j], chain.jet_e[j])
                    b2.SetPtEtaPhiE(chain.jet_pt[k], chain.jet_eta[k], chain.jet_phi[k], chain.jet_e[k])
                    if not data :
                        if   'jecup'   in sys:
                            b1 *= chain.jet_JER_Nom[j] * chain.jet_JES_Up[j]
                            b2 *= chain.jet_JER_Nom[k] * chain.jet_JES_Up[k]
                        elif 'jecdown' in sys:
                            b1 *= chain.jet_JER_Nom[j] * chain.jet_JES_Down[j]
                            b2 *= chain.jet_JER_Nom[k] * chain.jet_JES_Down[k]
                        elif 'jerup'   in sys:
                            b1 *= chain.jet_JER_Up[j]
                            b2 *= chain.jet_JER_Up[k]
                        elif 'jerdown' in sys:
                            b1 *= chain.jet_JER_Down[j]
                            b2 *= chain.jet_JER_Down[k]
                        else                 :
                            b1 *= chain.jet_JER_Nom[j]
                            b2 *= chain.jet_JER_Nom[k]

                    if makeTrainingInput:
                        if (addbjet1_matched.DeltaR(b1) == 0 and addbjet2_matched.DeltaR(b2) == 0) or (addbjet2_matched.DeltaR(b1) == 0  and addbjet1_matched.DeltaR(b2) == 0):
                            signal = 1
                        else:
                            signal = 0

                        jetCombi.append([
                            signal,i,b1.DeltaR(b2),abs(b1.Eta()-b2.Eta()),b1.DeltaPhi(b2),
                            (b1+b2+nu).Pt(),(b1+b2+nu).Eta(),(b1+b2+nu).Phi(),(b1+b2+nu).M(),
                            (b1+b2+lep).Pt(),(b1+b2+lep).Eta(),(b1+b2+lep).Phi(),(b1+b2+lep).M(),
                            (b1+lep).Pt(),(b1+lep).Eta(),(b1+lep).Phi(),(b1+lep).M(),
                            (b2+lep).Pt(),(b2+lep).Eta(),(b2+lep).Phi(),(b2+lep).M(),
                            (b1+b2).Pt(),(b1+b2).Eta(),(b1+b2).Phi(),(b1+b2).M(),
                            chain.jet_deepCSV[j],chain.jet_deepCSV[k],
                            b1.Pt(),b2.Pt(),b1.Eta(),b2.Eta(),b1.Phi(),b2.Phi(),b1.E(),b2.E()
                        ])
                    else:
                        jetCombi.append([
                            #Tree info
                            i, chain.channel, njets, nbjets,
                            chain.genweight, PUWeight,
                            lepton_SF, jet_SF_CSV_30, scaleweight, pdfweight,
                            lep.Pt(), lep.Eta(), lep.Phi(), lep.E(),
                            addbjet1.Pt(), addbjet1.Eta(), addbjet1.Phi(), addbjet1.E(),
                            addbjet2.Pt(), addbjet2.Eta(), addbjet2.Phi(), addbjet2.E(),
                            j,k,
                            #Deep learning variables
                            b1.DeltaR(b2),abs(b1.Eta()-b2.Eta()),b1.DeltaPhi(b2),
                            (b1+b2+nu).Pt(),(b1+b2+nu).Eta(),(b1+b2+nu).Phi(),(b1+b2+nu).M(),
                            (b1+b2+lep).Pt(),(b1+b2+lep).Eta(),(b1+b2+lep).Phi(),(b1+b2+lep).M(),
                            (b1+lep).Pt(),(b1+lep).Eta(),(b1+lep).Phi(),(b1+lep).M(),
                            (b2+lep).Pt(),(b2+lep).Eta(),(b2+lep).Phi(),(b2+lep).M(),
                            (b1+b2).Pt(),(b1+b2).Eta(),(b1+b2).Phi(),(b1+b2).M(),
                            chain.jet_deepCSV[j],chain.jet_deepCSV[k],
                            b1.Pt(),b2.Pt(),b1.Eta(),b2.Eta(),b1.Phi(),b2.Phi(),b1.E(),b2.E()
                            ])

    if makeTrainingInput:
        combi = pd.DataFrame(jetCombi, columns=['signal', 'event']+ut.getVarlist())
    else:
        combi = pd.DataFrame(jetCombi, columns=
            ['event','channel','njets','nbjets',
            'genWeight','PUWeight',
            'lepton_SF','jet_SF_CSV_30', 'scaleweight', 'pdfweight',
            'leptonPt','leptonEta','leptonPhi','leptonE',
            'addbjet1_pt','addbjet1_eta','addbjet1_phi','addbjet1_e',
            'addbjet2_pt','addbjet2_eta','addbjet2_phi','addbjet2_e',
            'b1','b2',
            ] + ut.getVarlist())

    tmp = inputFile[:-5]
    if makeTrainingInput:
        io.save(outputDir+"/array_train_ttbb.h5",combi)
    else:
        io.save(outputDir+"/array_"+tmp+".h5",combi)
   
    print(str(inputDir+"/"+inputFile)+" end")

    #combi.style.format('table')
    #combi.to_csv(outputDir+"/array_"+process+".csv")
    #combi.to_hdf(outputDir+"/array_"+process+".h5",combi)
    #combi.to_pickle(outputDir+"/array_"+process+".pickle")

if __name__ == '__main__':
    #Options
    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog [options] option
    convert root ntuple to array 
    """

    parser.add_option("-t", "--test", dest="test",
		      action = 'store_true',
		      default=False,
		      help='Test single root file')

    parser.add_option("-d", "--deep", dest="deep",
		      action = 'store_true',
		      default=False,
		      help='Run on signal sample for deep learning train')

    parser.add_option("-a", "--all", dest="all",
		      action = 'store_true',
		      default=False,
		      help='all root fiels in input directory')

    parser.add_option("-s", "--sys", dest="sys",
		      action = 'store_true',
		      default=False,
		      help='all root fiels in input directory')

    (options,args) = parser.parse_args()

    ntupleDir = '/data/users/seohyun/ntuple/hep2019/split/'
    arrayDir = './array/'

    processes = []
    if len(args) is 1:
      f = open(args[0], "r") 
      processes = f.read().splitlines()
    else:
      processes = os.listdir(ntupleDir) 

    start_time = time.time()

    if options.deep:
	makeCombi('/data/users/seohyun/ntuple/hep2019/nosplit/', 'TTLJ_PowhegPythia_ttbb.root', arrayDir, True)

    if options.test:
	if not os.path.exists("test"):
	  os.makedirs('./test')
	makeCombi(ntupleDir+'TTLJ_PowhegPythia_ttbb','Tree_ttbbLepJets_002.root' ,'./test')

    if options.all:
	for process in processes:
	    if not os.path.exists(arrayDir+process):
		os.makedirs(arrayDir+process)
	    for item in os.listdir(ntupleDir+process):
		proc = mp.Process(target=makeCombi, args=(ntupleDir+process,item, arrayDir+process))
		proc.start()
	    for item in os.listdir(ntupleDir+process):
		proc.join()

	if options.sys:
	    syslist = ['jecup','jecdown','jerup','jerdown']
	    for sys in syslist:
		for process in processes:
		    if not ('Data' in process or 'SYS' in process or 'Herwig' in process or 'Evtgen' in process or 'aMC' in process):
			os.makedirs(arrayDir+process+'__'+sys)
			for item in os.listdir(ntupleDir+process):
			    proc = mp.Process(target=makeCombi, args=(ntupleDir+process,item,arrayDir+process+'__'+sys,sys))
			    proc.start()
			for item in os.listdir(ntupleDir+process):
			    proc.join()

    print("Total running time :%s " %(time.time() - start_time))

