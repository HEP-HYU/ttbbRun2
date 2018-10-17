import numpy as np
from numpy.lib.recfunctions import stack_arrays
from ROOT import *
from root_numpy import tree2array
from array import array
import math
import glob
import pandas as pd
import deepdish.io as io

import printProgress as pp

def makeCombi(inputDir,inputFile,outputDir) :
    print(str(inputDir+"/"+inputFile)+" start")
    chain = TChain("ttbbLepJets/tree")
    chain.Add(inputDir+"/"+inputFile)

    data = False
    if 'Data' in inputDir : data = True
    ttbb = False
    if 'ttbb' in inputDir : ttbb = True

    muon_ch = 0
    muon_pt = 30.0
    muon_eta = 2.1
    electron_ch = 1
    electron_pt = 35.0
    electron_eta = 2.1
    jet_pt = 30.0
    jet_eta = 2.4
    jet_CSV_tight = 0.9535

    jetCombi = []
    for i in xrange(chain.GetEntries()) :
        chain.GetEntry(i)

        lepton_SF = 1.0
        jet_SF_CSV = 1.0
        if not data :
            lepton_SF = chain.lepton_SF[0]
            jet_SF_CSV = chain.jet_SF_CSV[0]

        MET_px = chain.MET*math.cos(chain.MET_phi)
        MET_py = chain.MET*math.sin(chain.MET_phi)
        nu = TLorentzVector(MET_px, MET_py, 0, chain.MET)

        lep = TLorentzVector()
        lep.SetPtEtaPhiE(chain.lepton_pT, chain.lepton_eta, chain.lepton_phi, chain.lepton_E)
        passmuon = False
        passelectron = False
        passmuon = chain.channel == muon_ch and lep.Pt() > muon_pt and abs(lep.Eta()) < muon_eta
        passelectron = chain.channel == electron_ch and lep.Pt() > electron_pt and abs(lep.Eta()) < electron_eta
        if passmuon == False and passelectron == False : continue

        addbjet1 = TLorentzVector(0,0,0,0)
        addbjet2 = TLorentzVector(0,0,0,0)
        if ttbb :
            addbjet1.SetPtEtaPhiE(chain.addbjet1_pt,chain.addbjet1_eta,chain.addbjet1_phi,chain.addbjet1_e)
            addbjet2.SetPtEtaPhiE(chain.addbjet2_pt,chain.addbjet2_eta,chain.addbjet2_phi,chain.addbjet2_e)

        njets = 0
        nbjets = 0
        for iJet in range(len(chain.jet_pT)) :
            jet = TLorentzVector()
            jet.SetPtEtaPhiE(chain.jet_pT[iJet],chain.jet_eta[iJet],chain.jet_phi[iJet],chain.jet_E[iJet])

            if not data : jet *= chain.jet_JER_Nom[iJet]
            if jet.Pt() < jet_pt or abs(jet.Eta()) > jet_eta : continue
            njets += 1
            if chain.jet_CSV[iJet] > jet_CSV_tight : nbjets += 1

        if njets < 6 or nbjets < 3 : continue

        for j in range(len(chain.jet_pT)-1):
            for k in range(j+1, len(chain.jet_pT)):
                if chain.jet_CSV[j] > jet_CSV_tight and chain.jet_CSV[k] > jet_CSV_tight:
                    b1 = TLorentzVector()
                    b2 = TLorentzVector()
                    b1.SetPtEtaPhiE(chain.jet_pT[j], chain.jet_eta[j], chain.jet_phi[j], chain.jet_E[j])
                    b2.SetPtEtaPhiE(chain.jet_pT[k], chain.jet_eta[k], chain.jet_phi[k], chain.jet_E[k])
                    if not data :
                        b1 *= chain.jet_JER_Nom[j]
                        b2 *= chain.jet_JER_Nom[k]
                    jetCombi.append([
                        i,chain.channel,njets,nbjets,
                        float(chain.genweight),float(chain.PUWeight[0]),lepton_SF,jet_SF_CSV,
                        lep.Pt(),lep.Eta(),lep.Phi(),lep.E(),
                        addbjet1.Pt(),addbjet1.Eta(),addbjet1.Phi(),addbjet1.E(),
                        addbjet2.Pt(),addbjet2.Eta(),addbjet2.Phi(),addbjet2.E(),
                        j,b1.Pt(),b1.Eta(),b1.Phi(),b1.E(),chain.jet_CSV[j],
                        k,b2.Pt(),b2.Eta(),b2.Phi(),b2.E(),chain.jet_CSV[k],
                        b1.DeltaR(b2),abs(b1.Eta()-b2.Eta()),b1.DeltaPhi(b2),
                        (b1+b2+nu).Pt(),(b1+b2+nu).Eta(),(b1+b2+nu).Phi(),(b1+b2+nu).M(),
                        (b1+b2+lep).Pt(),(b1+b2+lep).Eta(),(b1+b2+lep).Phi(),(b1+b2+lep).M(),
                        (b1+lep).Pt(),(b1+lep).Eta(),(b1+lep).Phi(),(b1+lep).M(),
                        (b2+lep).Pt(),(b2+lep).Eta(),(b2+lep).Phi(),(b2+lep).M(),
                        (b1+b2).Pt(),(b1+b2).Eta(),(b1+b2).Phi(),(b1+b2).M()
                    ])
    combi = pd.DataFrame(jetCombi, columns=[
        'event','channel','njets','nbjets',
        'genWeight','PUWeight','lepton_SF','jet_SF_CSV',
        'leptonPt','leptonEta','leptonPhi','leptonE',
        'addbjet1_pt','addbjet1_eta','addbjet1_phi','addbjet1_e',
        'addbjet2_pt','addbjet2_eta','addbjet2_phi','addbjet2_e',
        'b1','pt1','eta1','phi1','e1','csv1',
        'b2','pt2','eta2','phi2','e2','csv2',
        'dR','dEta','dPhi',
        'nuPt','nuEta','nuPhi','nuMass',
        'lbPt','lbEta','lbPhi','lbMass',
        'lb1Pt','lb1Eta','lb1Phi','lb1Mass',
        'lb2Pt','lb2Eta','lb2Phi','lb2Mass',
        'diPt','diEta','diPhi','diMass'
    ])
    tmp = inputFile[:-5]
    io.save(outputDir+"/array_"+tmp+".h5",combi)
    print(str(inputDir+"/"+inputFile)+" end")
    #combi.style.format('table')
    #combi.to_csv(outputDir+"/array_"+process+".csv")
    #combi.to_hdf(outputDir+"/array_"+process+".h5",combi)
    #combi.to_pickle(outputDir+"/array_"+process+".pickle")

def makeTrainingInput(outputDir) :
    chain = TChain("ttbbLepJets/tree")
    chain.Add("/data/users/seohyun/ntuple/hep2017/v808/nosplit/TTLJ_PowhegPythia_ttbb.root")

    muon_ch = 0
    muon_pt = 30.0
    muon_eta = 2.1
    electron_ch = 1
    electron_pt = 35.0
    electron_eta = 2.1
    jet_pt = 30.0
    jet_eta = 2.4
    jet_CSV_tight = 0.9535
    jet_CSV_medium = 0.8484

    jetCombi = []
    for idx in xrange(chain.GetEntries()) :
        pp.printProgress(idx, chain.GetEntries(), 'Progress:','Complete',1,25)
        chain.GetEntry(idx)

        MET_px = chain.MET*math.cos(chain.MET_phi)
        MET_py = chain.MET*math.sin(chain.MET_phi)
        nu = TLorentzVector(MET_px, MET_py, 0, chain.MET)

        lep = TLorentzVector()
        lep.SetPtEtaPhiE(chain.lepton_pT, chain.lepton_eta, chain.lepton_phi, chain.lepton_E)

        passmuon = False
        passelectron = False
        passmuon = chain.channel == muon_ch and lep.Pt() > muon_pt and abs(lep.Eta()) < muon_eta
        passelectron = chain.channel == electron_ch and lep.Pt() > electron_pt and abs(lep.Eta()) < electron_eta

        if passmuon == False and passelectron == False : continue

        njets = 0
        nbjets = 0

        for iJet in range(len(chain.jet_pT)):
            jet = TLorentzVector()
            jet.SetPtEtaPhiE(chain.jet_pT[iJet],chain.jet_eta[iJet],chain.jet_phi[iJet],chain.jet_E[iJet])
            jet *= chain.jet_JER_Nom[iJet]
            if jet.Pt() < jet_pt or abs(jet.Eta()) > jet_eta : continue
            njets += 1
            if chain.jet_CSV[iJet] > jet_CSV_tight : nbjets += 1

        if njets < 6 : continue
        if nbjets < 3 : continue

        addbjet1 = TLorentzVector()
        addbjet2 = TLorentzVector()
        addbjet1.SetPtEtaPhiE(chain.addbjet1_pt,chain.addbjet1_eta,chain.addbjet1_phi,chain.addbjet1_e)
        addbjet2.SetPtEtaPhiE(chain.addbjet2_pt,chain.addbjet2_eta,chain.addbjet2_phi,chain.addbjet2_e)
        addbjet1_matched = TLorentzVector(0,0,0,0)
        addbjet2_matched = TLorentzVector(0,0,0,0)

        for i in range(len(chain.jet_pT)):
            tmp = TLorentzVector()
            tmp.SetPtEtaPhiE(chain.jet_pT[i],chain.jet_eta[i],chain.jet_phi[i],chain.jet_E[i])
            tmp *= chain.jet_JER_Nom[i]
            #if tmp.Pt() > jet_pt and abs(tmp.Et()) < jet_eta : 
            if addbjet1.DeltaR( tmp ) < 0.4 :  addbjet1_matched = tmp;
            if addbjet2.DeltaR( tmp ) < 0.4 :  addbjet2_matched = tmp;

        for j in range(len(chain.jet_pT)-1):
            for k in range(j+1, len(chain.jet_pT)):
                if chain.jet_CSV[j] > jet_CSV_tight and chain.jet_CSV[k] > jet_CSV_tight:
                    b1 = TLorentzVector()
                    b2 = TLorentzVector()
                    b1.SetPtEtaPhiE(chain.jet_pT[j], chain.jet_eta[j], chain.jet_phi[j], chain.jet_E[j])
                    b2.SetPtEtaPhiE(chain.jet_pT[k], chain.jet_eta[k], chain.jet_phi[k], chain.jet_E[k])
                    b1 *= chain.jet_JER_Nom[j]
                    b2 *= chain.jet_JER_Nom[k]

                    if (addbjet1_matched.DeltaR(b1) == 0 and addbjet2_matched.DeltaR(b2) == 0) or (addbjet2_matched.DeltaR(b1) == 0  and addbjet1_matched.DeltaR(b2) == 0) : signal = 1
                    else : signal = 0

                    jetCombi.append([
                        signal,idx,b1.DeltaR(b2),abs(b1.Eta()-b2.Eta()),b1.DeltaPhi(b2),
                        (b1+b2+nu).Pt(),(b1+b2+nu).Eta(),(b1+b2+nu).Phi(),(b1+b2+nu).M(),
                        (b1+b2+lep).Pt(),(b1+b2+lep).Eta(),(b1+b2+lep).Phi(),(b1+b2+lep).M(),
                        (b1+lep).Pt(),(b1+lep).Eta(),(b1+lep).Phi(),(b1+lep).M(),
                        (b2+lep).Pt(),(b2+lep).Eta(),(b2+lep).Phi(),(b2+lep).M(),
                        (b1+b2).Pt(),(b1+b2).Eta(),(b1+b2).Phi(),(b1+b2).M(),
                        chain.jet_CSV[j],chain.jet_CSV[k],
                        b1.Pt(),b2.Pt(),b1.Eta(),b2.Eta(),b1.Phi(),b2.Phi(),b1.E(),b2.E()
                    ])

    print "\n"
    combi = pd.DataFrame(jetCombi, columns=[
        'signal','event','dR','dEta','dPhi',
        'nuPt','nuEta','nuPhi','nuMass',
        'lbPt','lbEta','lbPhi','lbMass',
        'lb1Pt','lb1Eta','lb1Phi','lb1Mass',
        'lb2Pt','lb2Eta','lb2Phi','lb2Mass',
        'diPt','diEta','diPhi','diMass',
        'csv1','csv2','pt1','pt2','eta1','eta2','phi1','phi2','e1','e2'
    ])
    io.save(outputDir+"array_train_ttbb.h5",combi)
