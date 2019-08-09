#! /usr/bin/env python
import ROOT
from ROOT import *
from array import array
import utils as ut

ntuple_path = '/data/users/seohyun/ntuple/Run2017/V9_6/nosplit/'

genchain = TChain("ttbbLepJets/gentree")
genchain.Add(ntuple_path+"/TTLJ_PowhegPythia_ttbb.root")

print "GENTREE RUN"

f_out = ROOT.TFile('gen_ttbb.root', 'recreate')

nbins_gen_addjets_dr = 4
gen_addjets_dr_width = [0.4,0.6,1.0,2.0,4.0]
nbins_gen_addjets_mass = 4
gen_addjets_mass_width = [0.0,60.0,100.0,170.0,400.0]

nChannel = 2
h_gen_addbjets_deltaR_nosel = [[0] for i in range(nChannel)]
h_gen_addbjets_invMass_nosel = [[0] for i in range(nChannel)]

for iChannel in range(0,nChannel):
    h_gen_addbjets_deltaR_nosel[iChannel] = ROOT.TH1D("h_gentop_GenAddbJetDeltaR_Ch" + str(iChannel) + "_nosel","", nbins_gen_addjets_dr, array('d', gen_addjets_dr_width))
    h_gen_addbjets_invMass_nosel[iChannel] = ROOT.TH1D("h_gentop_GenAddbJetInvMass_Ch" + str(iChannel) + "_nosel","", nbins_gen_addjets_mass, array('d', gen_addjets_mass_width))

muon_ch = 0
electron_ch = 1

for i in xrange(genchain.GetEntries()):
    ut.printProgress(i, genchain.GetEntries(), 'Progress:', 'Complete', 1, 50)
    genchain.GetEntry(i)
    addbjet1 = TLorentzVector()
    addbjet2 = TLorentzVector()
    addbjet1.SetPtEtaPhiE(genchain.addbjet1_pt, genchain.addbjet1_eta, genchain.addbjet1_phi, genchain.addbjet1_e)
    addbjet2.SetPtEtaPhiE(genchain.addbjet2_pt, genchain.addbjet2_eta, genchain.addbjet2_phi, genchain.addbjet2_e)

    gendR = addbjet1.DeltaR(addbjet2)
    genM = (addbjet1+addbjet2).M()

    if genchain.genchannel == muon_ch:
	h_gen_addbjets_deltaR_nosel[muon_ch].Fill(gendR,genchain.genweight)
	h_gen_addbjets_invMass_nosel[muon_ch].Fill(genM,genchain.genweight)
    elif genchain.genchannel == electron_ch:
	h_gen_addbjets_deltaR_nosel[electron_ch].Fill(gendR,genchain.genweight)
	h_gen_addbjets_invMass_nosel[electron_ch].Fill(genM,genchain.genweight)
    else: print("Error")

#Add overflow bins 
for iChannel in range(nChannel) :
    h_gen_addbjets_deltaR_nosel[iChannel].AddBinContent(nbins_gen_addjets_dr, h_gen_addbjets_deltaR_nosel[iChannel].GetBinContent(nbins_gen_addjets_dr+1))
    h_gen_addbjets_invMass_nosel[iChannel].AddBinContent(nbins_gen_addjets_mass, h_gen_addbjets_invMass_nosel[iChannel].GetBinContent(nbins_gen_addjets_mass+1))


f_out.cd() 
f_out.Write()
f_out.Close()
print "\nEND"

