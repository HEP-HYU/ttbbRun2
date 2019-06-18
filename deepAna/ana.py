import sys, os
import google.protobuf

os.environ["CUDA_VISIBLE_DEVICES"] = "3"

import pandas as pd
import csv
import math
import numpy as np
from array import array
from sklearn.preprocessing import StandardScaler, label_binarize
from sklearn.utils import shuffle
from operator import itemgetter
from itertools import groupby

import ROOT
from ROOT import *

import tensorflow as tf
import keras
from keras.utils import np_utils, multi_gpu_model
from keras.models import Model, Sequential, load_model
from keras.layers import Input, Dense, Activation, Dropout, add
from keras.layers.normalization import BatchNormalization
from keras.regularizers import l2
from keras.optimizers import Adam, SGD
from keras.callbacks import Callback, ModelCheckpoint

import utils as ut

def ana(inputDir, process, outputDir, sys='', flag1=False):

    ##for now,it is different from inputDir
    ##needed for gentree and event info.
    ntuple_path = '/data/users/seohyun/ntuple/Run2017/V9_5/nosplit/'

    if '__' in process:
        process = process.split('__')[0]
    print("Process: "+process+"\n")
    print("Systematics: "+sys+"\n")
    ntuple = process
#    if   'ttbb'          in process: process = 'ttbb'
#    elif 'ttbj'          in process: process = 'ttbj'
#    elif 'ttcc'          in process: process = 'ttcc'
#    elif 'ttLF'          in process: process = 'ttLF'
#    elif 'ttother'       in process: process = 'ttother'
#    elif 'PythiaBkg'     in process: process = 'ttbkg'
#    elif 'ttHbb'         in process: process = 'ttH'
#    elif 'ttW'           in process: process = 'ttW'
#    elif 'ttZ'           in process: process = 'ttZ'
#    elif 't_Powheg'      in process: process = 'tchannel'
#    elif 'tbar_Powheg'   in process: process = 'tbarchannel'
#    elif 'tW_Powheg'     in process: process = 'tWchannel'
#    elif 'tbarW_Powheg'  in process: process = 'tbarWchannel'
#    elif 'WJets'         in process: process = 'wjets'
#    elif 'ZJets_M10to50' in process: process = 'zjets10to50'
#    elif 'ZJets_M50'     in process: process = 'zjets'
#    elif 'WW'            in process: process = 'ww'
#    elif 'WZ'            in process: process = 'wz'
#    elif 'ZZ'            in process: process = 'zz'

    if 'Filter' in inputDir: process = 'ttbbFilter'+process

    print('Save:'+process+sys+'\n')

    timer = ROOT.TStopwatch()
    timer.Start()

    ver = ""
    configDir = ""
    weightDir = ""
    modelfile = ""

    with open('var.txt', 'r') as f :
        while True :
            line = f.readline()
            if not line : break
            tmp = line.split()
            if 'ver' in tmp : ver = tmp[1]
            if 'configDir' in tmp : configDir = tmp[1]
            if 'weightDir' in tmp : weightDir = tmp[1]
            if 'modelfile' in tmp : modelfile = tmp[1]
    if not os.path.exists(outputDir+'/'+modelfile):
        os.makedirs(outputDir+'/'+modelfile)

    closureTest = flag1
    data = False
    ttbb = False
    if 'Data' in process : data = True
    if 'ttbb' in process : ttbb = True
    muon_ch = 0
    muon_pt = 30.0
    muon_eta = 2.1
    electron_ch = 1
    electron_pt = 35.0
    electron_eta = 2.1
    jet_pt = 30.0
    jet_eta = 2.4
    jet_CSV = 0.9535
    jet_CSV_medium = 0.8484
    number_of_jets = 6
    number_of_bjets = 2
    nChannel = 2
    nStep = 4

    selEvent = pd.read_hdf(inputDir+".h5") 

    #print "\nMerge arrays"
    #selEvent = pd.DataFrame([])
    #max_nevt_num = 0
    #for item in os.listdir(inputDir):
    #    #print "Load file : "+str(inputDir)+'/'+str(item)
    #    df = pd.read_hdf(inputDir+'/'+item)
    #    last = 0
    #    if df.size != 0: last = int(df.tail(1)['event'])+1
    #    df['event'] = df['event'] + max_nevt_num
    #    str_query = 'csv1 > '+str(jet_CSV)+' and csv2 > '+str(jet_CSV)+' and njets >= 6 and nbjets >= 3'
    #    df = df.query(str_query)
    #    #df.reset_index(drop=True, inplace=True)
    #    selEvent = pd.concat([selEvent,df], axis=0)
    #    max_nevt_num += last

    #selEvent.reset_index(drop=True, inplace=True)

    nMatchable = 4864
    #ttbbFilter nMatchable: 5557
    countMatchable = False
    if countMatchable :
        df = pd.read_hdf("array/array_ttbb.h5")
        df = df.filter(['signal','event','dR'], axis=1)
        df = df.query('signal > 0')
        #tmpId = df.groupby(['event'])['dR'].transform(max) == df['dR']
        #df = df[tmpId]
        df.reset_index(drop=True, inplace=True)
        nMatchable = len(df.index)
        print(nMatchable)
        f_tmp = open('matchable.txt','w')
        f_tmp.write(str(nMatchable))
        f_tmp.write(str(df))
        f_tmp.close()


    #print(selEvent)
    if closureTest : f_out = ROOT.TFile(outputDir+'/'+modelfile+'/hist_closure.root', 'recreate')
    elif sys == '' : f_out = ROOT.TFile(outputDir+'/'+modelfile+'/hist_'+process+'.root', 'recreate')
    else           : f_out = ROOT.TFile(outputDir+'/'+modelfile+'/hist_'+process+sys+'.root', 'recreate')

    nbins_reco_addjets_dr = 12 #4
    reco_addjets_dr_min = 0.4
    reco_addjets_dr_max = 4.0
    #reco_addjets_dr_width = [0.4,0.6,1.0,2.0,4.0]

    nbins_reco_addjets_mass = 12 #4
    reco_addjets_m_min = 0
    reco_addjets_m_max = 400
    #reco_addjets_mass_width = [0.0,60.0,100.0,170.0,400.0]

    nbins_gen_addjets_dr = 4
    gen_addjets_dr_min = 0.4
    gen_addjets_dr_max = 4.0
    gen_addjets_dr_width = [0.4,0.6,1.0,2.0,4.0]

    nbins_gen_addjets_mass = 4
    gen_addjets_m_min = 0
    gen_addjets_m_max = 400
    gen_addjets_mass_width = [0.0,60.0,100.0,170.0,400.0]
    #gen_addjets_mass_width = [0.0,60.0,80.0,100.0,120.0,140.0,160.0,180.0,200.0,220.0,400.0]

    #Histograms for unfolding
    h_gen_addbjets_deltaR_nosel = [[0] for i in range(nChannel)]
    h_gen_addbjets_invMass_nosel = [[0] for i in range(nChannel)]
    h_njets = [[0] for i in range(nChannel)]
    h_nbjets = [[0] for i in range(nChannel)]
    h_reco_addjets_deltaR = [[0] for i in range(nChannel)]
    h_reco_addjets_invMass = [[0] for i in range(nChannel)]
    h_gen_addbjets_deltaR = [[0] for i in range(nChannel)]
    h_gen_addbjets_invMass = [[0] for i in range(nChannel)]
    h_respMatrix_deltaR = [[0] for i in range(nChannel)]
    h_respMatrix_invMass = [[0] for i in range(nChannel)]
    #h_respMatrix_invMass = [[0]*nStep for i in range(nChannel)]

    #Histograms of DNN input variables
    varlist = ut.getVarlist()
    xlabel = ut.getHistXlabel()
    h_hist = [[i for i in range(len(varlist))] for j in range(nChannel)]

    for iChannel in range(0,nChannel):
        for i in range(len(varlist)):
            histRange = []
            histRange = ut.getHistRange(varlist[i])
            h_hist[iChannel][i] = ROOT.TH1D(
                    'keras_h_'+ varlist[i] + '_Ch' + str(iChannel) + '_S3' + sys,'',
                    int(histRange[0]), float(histRange[1]), float(histRange[2])
                    )
            h_hist[iChannel][i].GetXaxis().SetTitle(xlabel[varlist[i]])
            h_hist[iChannel][i].GetYaxis().SetTitle("Entries")
            h_hist[iChannel][i].Sumw2()

        h_gen_addbjets_deltaR_nosel[iChannel] = ROOT.TH1D(
                "h_gentop_GenAddbJetDeltaR_Ch" + str(iChannel) + "_nosel" + sys,
                "", nbins_gen_addjets_dr,
                #gen_addjets_dr_min, gen_addjets_dr_max
                array('d', gen_addjets_dr_width)
                )
        h_gen_addbjets_deltaR_nosel[iChannel].GetXaxis().SetTitle("#DeltaR_{b#bar{b}}")
        h_gen_addbjets_deltaR_nosel[iChannel].GetYaxis().SetTitle("Entries")
        h_gen_addbjets_deltaR_nosel[iChannel].Sumw2()

        h_gen_addbjets_invMass_nosel[iChannel] = ROOT.TH1D(
                "h_gentop_GenAddbJetInvMass_Ch" + str(iChannel) + "_nosel" + sys,
                "", nbins_gen_addjets_mass,
                #gen_addjets_m_min, gen_addjets_m_max
                array('d', gen_addjets_mass_width)
                )
        h_gen_addbjets_invMass_nosel[iChannel].GetXaxis().SetTitle("m_{b#bar{b}}(GeV)")
        h_gen_addbjets_invMass_nosel[iChannel].GetYaxis().SetTitle("Entries")
        h_gen_addbjets_invMass_nosel[iChannel].Sumw2()

        h_njets[iChannel] = ROOT.TH1D(
                "h_keras_nJets_Ch" + str(iChannel) + "_S3" + sys,
                "",10, 0, 10
                )
        h_njets[iChannel].GetXaxis().SetTitle("Jet multiplicity")
        h_njets[iChannel].GetYaxis().SetTitle("Entries")
        h_njets[iChannel].Sumw2()

        h_nbjets[iChannel] = ROOT.TH1D(
                "h_keras_nbJets_Ch" + str(iChannel) + "_S3" + sys,
                "",10, 0, 10
                )
        h_nbjets[iChannel].GetXaxis().SetTitle("bJet multiplicity")
        h_nbjets[iChannel].GetYaxis().SetTitle("Entries")
        h_nbjets[iChannel].Sumw2()

        h_reco_addjets_deltaR[iChannel] = ROOT.TH1D(
                "h_keras_RecoAddbJetDeltaR_Ch" + str(iChannel) + "_S3" + sys,
                "", nbins_reco_addjets_dr,
                reco_addjets_dr_min, reco_addjets_dr_max
                #array('d', reco_addjets_dr_width)
                )
        h_reco_addjets_deltaR[iChannel].GetXaxis().SetTitle("#DeltaR_{b#bar{b}}")
        h_reco_addjets_deltaR[iChannel].GetYaxis().SetTitle("Entries")
        h_reco_addjets_deltaR[iChannel].Sumw2()

        h_reco_addjets_invMass[iChannel] = ROOT.TH1D(
                "h_keras_RecoAddbJetInvMass_Ch" + str(iChannel) + "_S3" + sys,
                "", nbins_reco_addjets_mass,
                reco_addjets_m_min, reco_addjets_m_max
                #array('d', reco_addjets_mass_width)
                )
        h_reco_addjets_invMass[iChannel].GetXaxis().SetTitle("m_{b#bar{b}}(GeV)")
        h_reco_addjets_invMass[iChannel].GetYaxis().SetTitle("Entries")
        h_reco_addjets_invMass[iChannel].Sumw2()

        h_gen_addbjets_deltaR[iChannel] = ROOT.TH1D(
                "h_gentop_GenAddbJetDeltaR_Ch" + str(iChannel) + "_S3" + sys,
                "", nbins_gen_addjets_dr,
                #gen_addjets_dr_min, gen_addjets_dr_max
                array('d', gen_addjets_dr_width)
                )
        h_gen_addbjets_deltaR[iChannel].GetXaxis().SetTitle("#DeltaR_{b#bar{b}}")
        h_gen_addbjets_deltaR[iChannel].GetYaxis().SetTitle("Entries")
        h_gen_addbjets_deltaR[iChannel].Sumw2()

        h_gen_addbjets_invMass[iChannel] = ROOT.TH1D(
                "h_gentop_GenAddbJetInvMass_Ch" + str(iChannel) + "_S3" + sys,
                "", nbins_gen_addjets_mass,
                #gen_addjets_m_min, gen_addjets_m_max
                array('d', gen_addjets_mass_width)
                )
        h_gen_addbjets_invMass[iChannel].GetXaxis().SetTitle("m_{b#bar{b}}(GeV)")
        h_gen_addbjets_invMass[iChannel].GetYaxis().SetTitle("Entries")
        h_gen_addbjets_invMass[iChannel].Sumw2()

        h_respMatrix_deltaR[iChannel] = ROOT.TH2D(
                "h_keras_ResponseMatrixDeltaR_Ch" + str(iChannel) + "_S3" + sys,"",
                nbins_reco_addjets_dr,
                reco_addjets_dr_min, reco_addjets_dr_max,
                #array('d', reco_addjets_dr_width),
                nbins_gen_addjets_dr,
                #gen_addjets_dr_min, gen_addjets_dr_max
                array('d', gen_addjets_dr_width)
                )
        h_respMatrix_deltaR[iChannel].GetXaxis().SetTitle("Reco. #DeltaR_{b#bar{b}}")
        h_respMatrix_deltaR[iChannel].GetYaxis().SetTitle("Gen. #DeltaR_{b#bar{b}}")
        h_respMatrix_deltaR[iChannel].Sumw2()

        h_respMatrix_invMass[iChannel] = ROOT.TH2D(
                "h_keras_ResponseMatrixInvMass_Ch" + str(iChannel) + "_S3" + sys,
                "", nbins_reco_addjets_mass,
                reco_addjets_m_min, reco_addjets_m_max,
                #array('d', reco_addjets_mass_width),
                nbins_gen_addjets_mass,#
                #gen_addjets_m_min, gen_addjets_m_max
                array('d', gen_addjets_mass_width)
                )
        h_respMatrix_invMass[iChannel].GetXaxis().SetTitle("Reco. m_{b#bar{b}}(GeV)")
        h_respMatrix_invMass[iChannel].GetYaxis().SetTitle("Gen. m_{b#bar{b}}(GeV)")
        h_respMatrix_invMass[iChannel].Sumw2()

    if ttbb == True:
        genchain = TChain("ttbbLepJets/gentree")
        genchain.Add(ntuple_path+"/"+ntuple+".root")

        print "GENTREE RUN"
        for i in xrange(genchain.GetEntries()):
            #if closureTest:
            #    if i%2 == 0 : continue
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

    #f_pred = open('pred.txt','w')
    print "\nLoad modelfile : "+str(modelfile)
    model = load_model(configDir+weightDir+ver+'/'+modelfile)
    model.summary()
    varlist = ut.getVarlist()
    jetCombi = selEvent.filter(varlist)
    scaler = StandardScaler()
    pred = pd.DataFrame([])
    if len(jetCombi) is not 0:
        inputset = np.array(jetCombi)
        inputset_sc = scaler.fit_transform(inputset)
        pred = model.predict(inputset_sc, batch_size = 2000)

    pred = pd.DataFrame(pred, columns=['background','signal'])
    #print(pred)
    #pred = pd.DataFrame(pred, columns=['signal'])
    #f_pred.write('Pred\n'+str(pred)+'\n'+str(type(pred)))
    #f_pred.write('SelEvent\n'+str(selEvent))
    selEvent = pd.concat([selEvent,pred], axis=1)
    #f_pred.write('SelEvent+Pred\n'+str(selEvent))
    idx = selEvent.groupby(['event'])['signal'].transform(max) == selEvent['signal']
    #f_pred.write('\n'+str(idx)+'\n'+str(selEvent[idx])+'\n')
    selEvent = selEvent[idx]
    selEvent.reset_index(drop=True, inplace=True)

    #selEvent.groupby('event').max('signal').reset_index(drop=True, inplace=True)
    #f_pred.write("Groupby\n"+process+"\n"+str(selEvent))
    #groups = selEvent.groupby('event')

    print "\n Fill Hist"
    nEvents = 0
    nEvt_isMatch_DNN = 0
    nEvt_isMatch_mindR = 0
    for index, event in selEvent.iterrows() :
        #maxval = event[1][event[1]['signal'] == event[1]['signal'].max()]
        ut.printProgress(index, len(selEvent.index), 'Progress: ','Complete',1,25)

        passmuon = False
        passelectron = False
        if event['channel'] == 0 : passmuon = True
        if event['channel'] == 1 : passelectron = True

        njets = event['njets']
        nbjets = event['nbjets']

        gen_addbjet1 = TLorentzVector()
        gen_addbjet2 = TLorentzVector()
        gen_addbjet1.SetPtEtaPhiE(event['addbjet1_pt'],event['addbjet1_eta'],event['addbjet1_phi'],event['addbjet1_e'])
        gen_addbjet2.SetPtEtaPhiE(event['addbjet2_pt'],event['addbjet2_eta'],event['addbjet2_phi'],event['addbjet2_e'])

        gen_dR = gen_addbjet1.DeltaR(gen_addbjet2)
        gen_M = (gen_addbjet1+gen_addbjet2).M()

        reco_dR = 9999
        reco_M = 9999
        reco_addbjet1 = TLorentzVector(0,0,0,0)
        reco_addbjet2 = TLorentzVector(0,0,0,0)
        #additional bjets from DNN

        reco_addbjet1.SetPtEtaPhiE(event['pt1'],event['eta1'],event['phi1'],event['e1'])
        reco_addbjet2.SetPtEtaPhiE(event['pt2'],event['eta2'],event['phi2'],event['e2'])
        reco_dR = reco_addbjet1.DeltaR(reco_addbjet2)
        reco_M = (reco_addbjet1+reco_addbjet2).M()

        eventweight = 1.0
        if not data:
            eventweight *= event['genWeight']

            if   'puup'   in sys: eventweight *= event['PUWeight'][1]
            elif 'pudown' in sys: eventweight *= event['PUWeight'][2]
            else                : eventweight *= event['PUWeight'][0]

            if passmuon:
                #[0]~[2]: ID/Iso, [3]~[5]: Trigger
                if   'musfup'   in sys: eventweight *= event['lepton_SF'][1]
                elif 'musfdown' in sys: eventweight *= event['lepton_SF'][2]
                else                  : eventweight *= event['lepton_SF'][0]

                if   'mutrgup'   in sys: eventweight *= event['lepton_SF'][4]
                elif 'mutrgdown' in sys: eventweight *= event['lepton_SF'][5]
                else                   : eventweight *= event['lepton_SF'][3]

            elif passelectron:
                #[0]~[2]: ID/Iso/Reco, [3]~[5]: Trigger
                if   'elsfup'   in sys: eventweight *= event['lepton_SF'][1]
                elif 'elsfdown' in sys: eventweight *= event['lepton_SF'][2]
                else                  : eventweight *= event['lepton_SF'][0]

                if   'eltrgup'   in sys: eventweight *= event['lepton_SF'][4]
                elif 'eltrgdown' in sys: eventweight *= event['lepton_SF'][5]
                else                   : eventweight *= event['lepton_SF'][3]

            #Scale Weight(ME)
            # [0] = muF up, [1] = muF down, [2] = muR up, [3] = muR up && muF up
            # [4] = muR down, [5] = muF down && muF down
            if 'TT' in inputDir or 'tt' in inputDir:
                if   'scale0' in sys: eventweight *= event['scaleweight'][0]
                elif 'scale1' in sys: eventweight *= event['scaleweight'][1]
                elif 'scale2' in sys: eventweight *= event['scaleweight'][2]
                elif 'scale3' in sys: eventweight *= event['scaleweight'][3]
                elif 'scale4' in sys: eventweight *= event['scaleweight'][4]
                elif 'scale5' in sys: eventweight *= event['scaleweight'][5]
                else                : eventweight *= 1.0

            #CSV Shape
            # Systematics for bottom flavor jets:
            #  Light flavor contamination: lf
            #  Linear and quadratic statistical fluctuations: hfstats1 and hfstats2
            # Systematics for light flavor jets:
            #  Heavy flavor contamimation: hf
            #  Linear and quadratic statistical fluctuations: lfstats1 and lfstats2
            # Systematics for charm flavor jets:
            #  Linear and quadratic uncertainties: cferr1 and cferr2
            if   'lfup'        in sys:
                eventweight *= event['jet_SF_CSV_30'][0] + event['jet_SF_CSV_30'][3]
            elif 'lfdown'      in sys:
                eventweight *= event['jet_SF_CSV_30'][0] - event['jet_SF_CSV_30'][4]
            elif 'hfup'        in sys:
                eventweight *= event['jet_SF_CSV_30'][0] + event['jet_SF_CSV_30'][5]
            elif 'hfdown'      in sys:
                eventweight *= event['jet_SF_CSV_30'][0] - event['jet_SF_CSV_30'][6]
            elif 'hfstat1up'   in sys:
                eventweight *= event['jet_SF_CSV_30'][0] + event['jet_SF_CSV_30'][7]
            elif 'hfstat1down' in sys:
                eventweight *= event['jet_SF_CSV_30'][0] - event['jet_SF_CSV_30'][8]
            elif 'hfstat2up'   in sys:
                eventweight *= event['jet_SF_CSV_30'][0] + event['jet_SF_CSV_30'][9]
            elif 'hfstat2down' in sys:
                eventweight *= event['jet_SF_CSV_30'][0] - event['jet_SF_CSV_30'][10]
            elif 'lfstat1up'   in sys:
                eventweight *= event['jet_SF_CSV_30'][0] + event['jet_SF_CSV_30'][11]
            elif 'lfstat1down' in sys:
                eventweight *= event['jet_SF_CSV_30'][0] - event['jet_SF_CSV_30'][12]
            elif 'lfstat2up'   in sys:
                eventweight *= event['jet_SF_CSV_30'][0] + event['jet_SF_CSV_30'][13]
            elif 'lfstat2down' in sys:
                eventweight *= event['jet_SF_CSV_30'][0] - event['jet_SF_CSV_30'][14]
            elif 'cferr1up'    in sys:
                eventweight *= event['jet_SF_CSV_30'][0] + event['jet_SF_CSV_30'][15]
            elif 'cferr1down'  in sys:
                eventweight *= event['jet_SF_CSV_30'][0] - event['jet_SF_CSV_30'][16]
            elif 'cferr2up'    in sys:
                eventweight *= event['jet_SF_CSV_30'][0] + event['jet_SF_CSV_30'][17]
            elif 'cferr2down'  in sys:
                eventweight *= event['jet_SF_CSV_30'][0] - event['jet_SF_CSV_30'][18]
            else                     :
                eventweight *= event['jet_SF_CSV_30'][0]

            if 'TT' in inputDir or 'tt' in inputDir:
                if   'pdfup'   in sys: eventweight *= event['pdfweight'][101]
                elif 'pdfdown' in sys: eventweight *= event['pdfweight'][100]

        #f_pred.write('Pred : '+str(maxval)+'\n')
        #f_pred.write('Score\n'+str(event[1])+'\n')
        #f_pred.write('jet 1 : '+str(reco_addbjet1.Pt())+' jet 2 : '+str(reco_addbjet2.Pt())+'\n')
        #f_pred.write('genjet 1 : '+str(gen_addbjet1.Pt())+' genjet2 : '+str(gen_addbjet2.Pt())+'\n')
        #f_pred.write('reco dR : '+str(reco_addbjet1.DeltaR(reco_addbjet2))+'gen dR : '+str(gen_addbjet1.DeltaR(gen_addbjet2))+'\n')

        passchannel = -999
        passcut = 0

        #matching ratio
        isMatch_DNN = False
        isMatch_DNN = (reco_addbjet1.DeltaR(gen_addbjet1) < 0.5 and reco_addbjet2.DeltaR(gen_addbjet2) < 0.5) or (reco_addbjet1.DeltaR(gen_addbjet2) < 0.5 and reco_addbjet2.DeltaR(gen_addbjet1) < 0.5)
        if passmuon == True and passelectron == False : passchannel = muon_ch
        elif passmuon == False and passelectron == True : passchannel = electron_ch
        else : print "Error!"
        if isMatch_DNN : nEvt_isMatch_DNN += 1
        nEvents += 1

        if closureTest:
            if index%2 == 1:
                h_njets[passchannel].Fill(njets, eventweight)
                h_nbjets[passchannel].Fill(nbjets, eventweight)
                h_reco_addjets_deltaR[passchannel].Fill(reco_dR, eventweight)
                h_reco_addjets_invMass[passchannel].Fill(reco_M, eventweight)
                h_gen_addbjets_deltaR[passchannel].Fill(gen_dR, eventweight)
                h_gen_addbjets_invMass[passchannel].Fill(gen_M, eventweight)
            else:
                h_respMatrix_deltaR[passchannel].Fill(reco_dR, gen_dR, eventweight)
                h_respMatrix_invMass[passchannel].Fill(reco_M, gen_M, eventweight)
        else:
            h_njets[passchannel].Fill(njets, eventweight)
            h_nbjets[passchannel].Fill(nbjets, eventweight)
            h_reco_addjets_deltaR[passchannel].Fill(reco_dR, eventweight)
            h_reco_addjets_invMass[passchannel].Fill(reco_M, eventweight)
            for index, value in enumerate(varlist):
                h_hist[passchannel][index].Fill(event[value], eventweight)
            if ttbb:
                h_gen_addbjets_deltaR[passchannel].Fill(gen_dR, eventweight)
                h_gen_addbjets_invMass[passchannel].Fill(gen_M, eventweight)
                h_respMatrix_deltaR[passchannel].Fill(reco_dR, gen_dR, eventweight)
                h_respMatrix_invMass[passchannel].Fill(reco_M, gen_M, eventweight)

    if ttbb:
        matching_DNN = 0.0
        #matching_mindR = 0.0
        if nEvents is not 0 :
            matching_DNN_able = float(nEvt_isMatch_DNN) / float(nMatchable)
            matching_DNN = float(nEvt_isMatch_DNN) / float(nEvents)
            #matching_mindR = float(nEvt_isMatch_mindR) / float(nEvents)
        #print "\nSelected Events / Total Events : "+str(nEvents)+"/"+str(nTotal)
        print "\nMatching ratio with matchable events from DNN : "+str(matching_DNN_able)+"("+str(nEvt_isMatch_DNN)+"/"+str(nMatchable)+")"
        print "Matching ratio with step 3 events from DNN : "+str(matching_DNN)+"("+str(nEvt_isMatch_DNN)+"/"+str(nEvents)+")"
        #print "Matching Ratio from minimun dR : "+str(matching_mindR)+"("+str(nEvt_isMatch_mindR)+"/"+str(nEvents)+")"
        f_ratio = open('ratio.txt','a')
        f_ratio.write(modelfile)
        f_ratio.write("\nMatching ratio with matchable events from DNN: "+str(matching_DNN_able)+"("+str(nEvt_isMatch_DNN)+"/"+str(nMatchable)+")\n")
        f_ratio.close()

    for iChannel in range(nChannel) :
        h_njets[iChannel].AddBinContent(10,h_njets[iChannel].GetBinContent(11))
        h_nbjets[iChannel].AddBinContent(10,h_nbjets[iChannel].GetBinContent(11))
        h_reco_addjets_deltaR[iChannel].AddBinContent(nbins_reco_addjets_dr, h_reco_addjets_deltaR[iChannel].GetBinContent(nbins_reco_addjets_dr+1))
        h_reco_addjets_invMass[iChannel].AddBinContent(nbins_reco_addjets_mass, h_reco_addjets_invMass[iChannel].GetBinContent(nbins_reco_addjets_mass+1))
        h_gen_addbjets_deltaR[iChannel].AddBinContent(nbins_gen_addjets_dr, h_gen_addbjets_deltaR[iChannel].GetBinContent(nbins_gen_addjets_dr+1))
        h_gen_addbjets_invMass[iChannel].AddBinContent(nbins_gen_addjets_mass, h_gen_addbjets_invMass[iChannel].GetBinContent(nbins_gen_addjets_mass+1))
        h_gen_addbjets_deltaR_nosel[iChannel].AddBinContent(nbins_gen_addjets_dr, h_gen_addbjets_deltaR_nosel[iChannel].GetBinContent(nbins_gen_addjets_dr+1))
        h_gen_addbjets_invMass_nosel[iChannel].AddBinContent(nbins_gen_addjets_mass, h_gen_addbjets_invMass_nosel[iChannel].GetBinContent(nbins_gen_addjets_mass+1))

        for index, value in enumerate(varlist):
            tmp = ut.getHistRange(value)
            h_hist[iChannel][index].AddBinContent(tmp[0], h_hist[iChannel][index].GetBinContent(tmp[0]+1))
            h_hist[iChannel][index].ClearUnderflowAndOverflow()

        for iXaxis in range(1, nbins_reco_addjets_dr+1) :
            tmp = h_respMatrix_deltaR[iChannel].GetBinContent(iXaxis, nbins_gen_addjets_dr)+h_respMatrix_deltaR[iChannel].GetBinContent(iXaxis, nbins_gen_addjets_dr+1)
            h_respMatrix_deltaR[iChannel].SetBinContent(iXaxis, nbins_gen_addjets_dr, tmp)
        for iYaxis in range(1, nbins_gen_addjets_dr+1) :
            tmp = h_respMatrix_deltaR[iChannel].GetBinContent(nbins_reco_addjets_dr, iYaxis)+h_respMatrix_deltaR[iChannel].GetBinContent(nbins_reco_addjets_dr+1, iYaxis)
            h_respMatrix_deltaR[iChannel].SetBinContent(nbins_reco_addjets_dr, iYaxis, tmp)

        for iXaxis in range(1, nbins_reco_addjets_mass+1) :
            tmp = h_respMatrix_invMass[iChannel].GetBinContent(iXaxis, nbins_gen_addjets_mass)+h_respMatrix_invMass[iChannel].GetBinContent(iXaxis, nbins_gen_addjets_mass+1)
            h_respMatrix_invMass[iChannel].SetBinContent(iXaxis, nbins_gen_addjets_mass, tmp)
        for iYaxis in range(1, nbins_gen_addjets_mass+1) :
            tmp = h_respMatrix_invMass[iChannel].GetBinContent(nbins_reco_addjets_mass, iYaxis)+h_respMatrix_invMass[iChannel].GetBinContent(nbins_reco_addjets_mass+1, iYaxis)
            h_respMatrix_invMass[iChannel].SetBinContent(nbins_reco_addjets_mass, iYaxis,tmp)

        tmp = h_respMatrix_deltaR[iChannel].GetBinContent(nbins_reco_addjets_dr+1,nbins_gen_addjets_dr+1)+h_respMatrix_deltaR[iChannel].GetBinContent(nbins_reco_addjets_dr,nbins_gen_addjets_dr)
        h_respMatrix_deltaR[iChannel].SetBinContent(nbins_reco_addjets_dr,nbins_gen_addjets_dr,tmp)
        tmp = h_respMatrix_invMass[iChannel].GetBinContent(nbins_reco_addjets_mass+1,nbins_gen_addjets_mass+1)+h_respMatrix_invMass[iChannel].GetBinContent(nbins_reco_addjets_mass,nbins_gen_addjets_mass)
        h_respMatrix_invMass[iChannel].SetBinContent(nbins_reco_addjets_mass,nbins_gen_addjets_mass,tmp)

        h_njets[iChannel].ClearUnderflowAndOverflow()
        h_nbjets[iChannel].ClearUnderflowAndOverflow()
        h_reco_addjets_deltaR[iChannel].ClearUnderflowAndOverflow()
        h_reco_addjets_invMass[iChannel].ClearUnderflowAndOverflow()
        h_gen_addbjets_deltaR[iChannel].ClearUnderflowAndOverflow()
        h_gen_addbjets_invMass[iChannel].ClearUnderflowAndOverflow()
        h_gen_addbjets_deltaR_nosel[iChannel].ClearUnderflowAndOverflow()
        h_gen_addbjets_invMass_nosel[iChannel].ClearUnderflowAndOverflow()
        h_respMatrix_deltaR[iChannel].ClearUnderflowAndOverflow()
        h_respMatrix_invMass[iChannel].ClearUnderflowAndOverflow()


    f_ntuple = TFile.Open(os.path.join(ntuple_path, ntuple+'.root'),'read')
    h_eventinfo = f_ntuple.Get("ttbbLepJets/EventInfo")
    h_scaleweight = f_ntuple.Get("ttbbLepJets/ScaleWeights")

    f_out.cd()
    h_eventinfo.Write()
    h_scaleweight.Write()
    f_out.Write()
    f_out.Close()
    #f_pred.close()

    keras.backend.clear_session()

    timer.Stop()
    realtime = timer.RealTime()
    cputime = timer.CpuTime()
    print("Real Time : {0:6.2f} seconds, CPU Time : {1:6.2f} seconds").format(realtime,cputime)
