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

from ROOT import *
import ROOT

import tensorflow as tf
import keras
from keras.utils import np_utils, multi_gpu_model
from keras.models import Model, Sequential, load_model
from keras.layers import Input, Dense, Activation, Dropout, add
from keras.layers.normalization import BatchNormalization
from keras.regularizers import l2
from keras.optimizers import Adam, SGD
from keras.callbacks import Callback, ModelCheckpoint

import printProgress as pp

def ana(inputDir, process, outputDir,flag1=False) :
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

    print "Load modelfile : "+str(modelfile)
    model = load_model(configDir+weightDir+ver+'/'+modelfile)
    model.summary()

    if not os.path.exists(outputDir+'/'+modelfile):
        os.makedirs(outputDir+'/'+modelfile)

    closureTest = flag1
    data = False
    ttbb = False
    if 'Data' in process : data = True
    if 'ttbb' in process : ttbb = True

    df = pd.read_hdf("array/array_train_ttbb.h5")
    nMatchable = 4864
    countMatchable = False
    if countMatchable :
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
    number_of_bjets = 3
    nChannel = 2
    nStep = 4

    if closureTest : f_out = ROOT.TFile(outputDir+'/'+modelfile+'/hist_closure.root', 'recreate')
    else : f_out = ROOT.TFile(outputDir+'/'+modelfile+'/hist_'+process+'.root', 'recreate')

    RECO_NUMBER_OF_JETS_ = "nJets"
    RECO_NUMBER_OF_BJETS_ = "nBjets"
    RECO_ADDJETS_DELTAR_ = "RecoJetDeltaR"
    RECO_ADDJETS_INVARIANT_MASS_ = "RecoJetInvMass"
    GEN_ADDBJETS_DELTAR_ = "GenbJetDeltaR"
    GEN_ADDBJETS_INVARIANT_MASS_ = "GenbJetInvMass"
    RESPONSE_MATRIX_DELTAR_ = "ResponseMatrixDeltaR"
    RESPONSE_MATRIX_INVARIANT_MASS_ = "ResponseMatrixInvMass"

    nbins_reco_addjets_dr = 4
    reco_addjets_dr_width = [0.4,0.6,1.0,2.0,4.0]

    nbins_reco_addjets_mass = 4
    reco_addjets_mass_width = [0.0,60.0,100.0,170.0,400.0]

    nbins_gen_addjets_dr = 4
    gen_addjets_dr_width = [0.4,0.6,1.0,2.0,4.0]

    nbins_gen_addjets_mass = 4
    gen_addjets_mass_width = [0.0,60.0,100.0,170.0,400.0]

    nbins_delta_r = 20
    delta_r_min = 0
    delta_r_max = 4
    #delta_r_width = []

    nbins_delta_eta = 20
    delta_eta_min = 0
    delta_eta_max = 2.5
    #delta_eta_width = []

    nbins_delta_phi = 20
    delta_phi_min = 0
    delta_phi_max = math.pi
    #delta_phi_width = []

    nbins_pt = 20
    pt_min = 0
    pt_max = 400
    #pt_width = []

    nbins_eta = 20
    eta_min = -2.5
    eta_max = 2.5
    #eta_width = []

    nbins_phi = 20
    phi_min = -math.pi
    phi_max = math.pi
    #phi_width = []

    nbins_mass = 20
    mass_min = 0
    mass_max = 400
    #mass_width = []

    nbins_energy = 20
    energy_min = 0
    energy_max = 400
    #energy_widht = []

    nbins_csv = 20
    csv_min = 0.95
    csv_max = 1
    #csv_width = []

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
    h_dR = [[0] for i in range(nChannel)]
    h_dEta = [[0] for i in range(nChannel)]
    h_dPhi = [[0] for i in range(nChannel)]
    h_nuPt = [[0] for i in range(nChannel)]
    h_nuEta = [[0] for i in range(nChannel)]
    h_nuPhi = [[0] for i in range(nChannel)]
    h_nuMass = [[0] for i in range(nChannel)]
    h_lbPt = [[0] for i in range(nChannel)]
    h_lbEta = [[0] for i in range(nChannel)]
    h_lbPhi = [[0] for i in range(nChannel)]
    h_lbMass = [[0] for i in range(nChannel)]
    h_lb1Pt = [[0] for i in range(nChannel)]
    h_lb1Eta = [[0] for i in range(nChannel)]
    h_lb1Phi = [[0] for i in range(nChannel)]
    h_lb1Mass = [[0] for i in range(nChannel)]
    h_lb2Pt = [[0] for i in range(nChannel)]
    h_lb2Eta = [[0] for i in range(nChannel)]
    h_lb2Phi = [[0] for i in range(nChannel)]
    h_lb2Mass = [[0] for i in range(nChannel)]
    h_diPt = [[0] for i in range(nChannel)]
    h_diEta = [[0] for i in range(nChannel)]
    h_diPhi = [[0] for i in range(nChannel)]
    h_diMass = [[0] for i in range(nChannel)]
    h_csv1 = [[0] for i in range(nChannel)]
    h_csv2 = [[0] for i in range(nChannel)]
    h_pt1 = [[0] for i in range(nChannel)]
    h_pt2 = [[0] for i in range(nChannel)]
    h_eta1 = [[0] for i in range(nChannel)]
    h_eta2 = [[0] for i in range(nChannel)]
    h_phi1 = [[0] for i in range(nChannel)]
    h_phi2 = [[0] for i in range(nChannel)]
    h_e1 = [[0] for i in range(nChannel)]
    h_e2 = [[0] for i in range(nChannel)]

    for iChannel in range(0,nChannel) :
        h_gen_addbjets_deltaR_nosel[iChannel] = ROOT.TH1D(
                "h_" + GEN_ADDBJETS_DELTAR_ + "_Ch" + str(iChannel) + "_nosel_" + process,
                "", nbins_gen_addjets_dr, array('d', gen_addjets_dr_width)
                )
        h_gen_addbjets_deltaR_nosel[iChannel].GetXaxis().SetTitle("#DeltaR_{b#bar{b}}")
        h_gen_addbjets_deltaR_nosel[iChannel].GetYaxis().SetTitle("Entries")
        h_gen_addbjets_deltaR_nosel[iChannel].Sumw2()

        h_gen_addbjets_invMass_nosel[iChannel] = ROOT.TH1D(
                "h_" + GEN_ADDBJETS_INVARIANT_MASS_ + "_Ch" + str(iChannel) + "_nosel_" + process,
                "", nbins_gen_addjets_mass, array('d', gen_addjets_mass_width)
                )
        h_gen_addbjets_invMass_nosel[iChannel].GetXaxis().SetTitle("m_{b#bar{b}}(GeV)")
        h_gen_addbjets_invMass_nosel[iChannel].GetYaxis().SetTitle("Entries")
        h_gen_addbjets_invMass_nosel[iChannel].Sumw2()

        h_njets[iChannel] = ROOT.TH1D(
                "h_" + RECO_NUMBER_OF_JETS_ + "_Ch" + str(iChannel) + "_S3_" + process,
                "",10, 0, 10
                )
        h_njets[iChannel].GetXaxis().SetTitle("Jet multiplicity")
        h_njets[iChannel].GetYaxis().SetTitle("Entries")
        h_njets[iChannel].Sumw2()

        h_nbjets[iChannel] = ROOT.TH1D(
                "h_" + RECO_NUMBER_OF_BJETS_ + "_Ch" + str(iChannel) + "_S3_" + process,
                "",10, 0, 10
                )
        h_nbjets[iChannel].GetXaxis().SetTitle("bJet multiplicity")
        h_nbjets[iChannel].GetYaxis().SetTitle("Entries")
        h_nbjets[iChannel].Sumw2()

        h_reco_addjets_deltaR[iChannel] = ROOT.TH1D(
                "h_" + RECO_ADDJETS_DELTAR_ + "_Ch" + str(iChannel) + "_S3_" + process,
                "", nbins_reco_addjets_dr, array('d', reco_addjets_dr_width)
                )
        h_reco_addjets_deltaR[iChannel].GetXaxis().SetTitle("#DeltaR_{b#bar{b}}")
        h_reco_addjets_deltaR[iChannel].GetYaxis().SetTitle("Entries")
        h_reco_addjets_deltaR[iChannel].Sumw2()

        h_reco_addjets_invMass[iChannel] = ROOT.TH1D(
                "h_" + RECO_ADDJETS_INVARIANT_MASS_ + "_Ch" + str(iChannel) + "_S3_" + process,
                "", nbins_reco_addjets_mass, array('d', reco_addjets_mass_width)
                )
        h_reco_addjets_invMass[iChannel].GetXaxis().SetTitle("m_{b#bar{b}}(GeV)")
        h_reco_addjets_invMass[iChannel].GetYaxis().SetTitle("Entries")
        h_reco_addjets_invMass[iChannel].Sumw2()

        h_gen_addbjets_deltaR[iChannel] = ROOT.TH1D(
                "h_" + GEN_ADDBJETS_DELTAR_ + "_Ch" + str(iChannel) + "_S3_" + process,
                "", nbins_gen_addjets_dr, array('d', gen_addjets_dr_width)
                )
        h_gen_addbjets_deltaR[iChannel].GetXaxis().SetTitle("#DeltaR_{b#bar{b}}")
        h_gen_addbjets_deltaR[iChannel].GetYaxis().SetTitle("Entries")
        h_gen_addbjets_deltaR[iChannel].Sumw2()

        h_gen_addbjets_invMass[iChannel] = ROOT.TH1D(
                "h_" + GEN_ADDBJETS_INVARIANT_MASS_ + "_Ch" + str(iChannel) + "_S3_" + process,
                "", nbins_gen_addjets_mass, array('d', gen_addjets_mass_width)
                )
        h_gen_addbjets_invMass[iChannel].GetXaxis().SetTitle("m_{b#bar{b}}(GeV)")
        h_gen_addbjets_invMass[iChannel].GetYaxis().SetTitle("Entries")
        h_gen_addbjets_invMass[iChannel].Sumw2()

        h_respMatrix_deltaR[iChannel] = ROOT.TH2D(
                "h_" + RESPONSE_MATRIX_DELTAR_ + "_Ch" + str(iChannel) + "_S3_" + process,"",
                nbins_reco_addjets_dr, array('d', reco_addjets_dr_width),
                nbins_gen_addjets_dr, array('d', gen_addjets_dr_width)
                )
        h_respMatrix_deltaR[iChannel].GetXaxis().SetTitle("Reco. #DeltaR_{b#bar{b}}")
        h_respMatrix_deltaR[iChannel].GetYaxis().SetTitle("Gen. #DeltaR_{b#bar{b}}")
        h_respMatrix_deltaR[iChannel].Sumw2()

        h_respMatrix_invMass[iChannel] = ROOT.TH2D(
                "h_" + RESPONSE_MATRIX_INVARIANT_MASS_ + "_Ch" + str(iChannel) + "_S3_" + process,
                "", nbins_reco_addjets_mass, array('d', reco_addjets_mass_width),
                nbins_gen_addjets_mass, array('d', gen_addjets_mass_width)
                )
        h_respMatrix_invMass[iChannel].GetXaxis().SetTitle("Reco. m_{b#bar{b}}(GeV)")
        h_respMatrix_invMass[iChannel].GetYaxis().SetTitle("Gen. m_{b#bar{b}}(GeV)")
        h_respMatrix_invMass[iChannel].Sumw2()

        h_dR[iChannel] = ROOT.TH1D(
                "h_deltaR_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_delta_r, delta_r_min, delta_r_max #array('d', delta_r_width)
                )
        h_dR[iChannel].GetXaxis().SetTitle("#DeltaR_{b#bar{b}}")
        h_dR[iChannel].GetYaxis().SetTitle("Entries")
        h_dR[iChannel].Sumw2()

        h_dEta[iChannel] = ROOT.TH1D(
                "h_deltaEta_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_delta_eta, delta_eta_min, delta_eta_max #array('d', delta_eta_width)
                )
        h_dEta[iChannel].GetXaxis().SetTitle("#Delta#eta_{b#bar{b}}")
        h_dEta[iChannel].GetYaxis().SetTitle("Entries")
        h_dEta[iChannel].Sumw2()

        h_dPhi[iChannel] = ROOT.TH1D(
                "h_deltaPhi_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_delta_phi, delta_phi_min, delta_phi_max #array('d', delta_phi_width)
                )
        h_dPhi[iChannel].GetXaxis().SetTitle("#Delta#phi_{b#bar{b}}")
        h_dPhi[iChannel].GetYaxis().SetTitle("Entries")
        h_dPhi[iChannel].Sumw2()

        h_nuPt[iChannel] = ROOT.TH1D(
                "h_Metb2SumPt_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_pt, pt_min, pt_max
                )
        h_nuPt[iChannel].GetXaxis().SetTitle("p_{T}_{#nub#bar{b}}(GeV)")
        h_nuPt[iChannel].GetYaxis().SetTitle("Entries")
        h_nuPt[iChannel].Sumw2()

        h_nuEta[iChannel] = ROOT.TH1D(
                "h_Metb2SumEta_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_eta, eta_min, eta_max
                )
        h_nuEta[iChannel].GetXaxis().SetTitle("#eta_{#nub#bar{b}}")
        h_nuEta[iChannel].GetYaxis().SetTitle("Entries")
        h_nuEta[iChannel].Sumw2()

        h_nuPhi[iChannel] = ROOT.TH1D(
                "h_Metb2SumPhi_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_phi, phi_min, phi_max
                )
        h_nuPhi[iChannel].GetXaxis().SetTitle("#phi_{#nub#bar{b}}")
        h_nuPhi[iChannel].GetYaxis().SetTitle("Entries")
        h_nuPhi[iChannel].Sumw2()

        h_nuMass[iChannel] = ROOT.TH1D(
                "h_Metb2SumMass_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_mass, mass_min, mass_max
                )
        h_nuMass[iChannel].GetXaxis().SetTitle("m_{#nub#bar{b}}(GeV)")
        h_nuMass[iChannel].GetYaxis().SetTitle("Entries")
        h_nuMass[iChannel].Sumw2()

        h_lbPt[iChannel] = ROOT.TH1D(
                "h_Leptonb4SumPt_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_pt, pt_min, pt_max
                )
        h_lbPt[iChannel].GetXaxis().SetTitle("p_{T}_{lb#bar{b}}(GeV)")
        h_lbPt[iChannel].GetYaxis().SetTitle("Entries")
        h_lbPt[iChannel].Sumw2()

        h_lbEta[iChannel] = ROOT.TH1D(
                "h_Leptonb4SumEta_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_eta, eta_min, eta_max
                )
        h_lbEta[iChannel].GetXaxis().SetTitle("#eta_{lb#bar{b}}")
        h_lbEta[iChannel].GetYaxis().SetTitle("Entries")
        h_lbEta[iChannel].Sumw2()

        h_lbPhi[iChannel] = ROOT.TH1D(
                "h_Leptonb4SumPhi_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_phi, phi_min, phi_max
                )
        h_lbPhi[iChannel].GetXaxis().SetTitle("#phi_{lb#bar{b}}")
        h_lbPhi[iChannel].GetYaxis().SetTitle("Entries")
        h_lbPhi[iChannel].Sumw2()

        h_lbMass[iChannel] = ROOT.TH1D(
                "h_Leptonb4SumMass_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_mass, mass_min, mass_max
                )
        h_lbMass[iChannel].GetXaxis().SetTitle("m_{lb#bar{b}}(GeV)")
        h_lbMass[iChannel].GetYaxis().SetTitle("Entries")
        h_lbMass[iChannel].Sumw2()

        h_lb1Pt[iChannel] = ROOT.TH1D(
                "h_Leptonbjet1Pt_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_pt, pt_min, pt_max
                )
        h_lb1Pt[iChannel].GetXaxis().SetTitle("p_{T}_{lb_{1}}(GeV)")
        h_lb1Pt[iChannel].GetYaxis().SetTitle("Entries")
        h_lb1Pt[iChannel].Sumw2()

        h_lb1Eta[iChannel] = ROOT.TH1D(
                "h_Leptonbjet1Eta_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_eta, eta_min, eta_max
                )
        h_lb1Eta[iChannel].GetXaxis().SetTitle("#eta_{lb_{1}}")
        h_lb1Eta[iChannel].GetYaxis().SetTitle("Entries")
        h_lb1Eta[iChannel].Sumw2()

        h_lb1Phi[iChannel] = ROOT.TH1D(
                "h_Leptonbjet1Phi_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_phi, phi_min, phi_max
                )
        h_lb1Phi[iChannel].GetXaxis().SetTitle("#phi_{lb_{1}}")
        h_lb1Phi[iChannel].GetYaxis().SetTitle("Entries")
        h_lb1Phi[iChannel].Sumw2()

        h_lb1Mass[iChannel] = ROOT.TH1D(
                "h_Leptonbjet1Mass_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_mass, mass_min, mass_max
                )
        h_lb1Mass[iChannel].GetXaxis().SetTitle("m_{lb_{1}}(GeV)")
        h_lb1Mass[iChannel].GetYaxis().SetTitle("Entries")
        h_lb1Mass[iChannel].Sumw2()

        h_lb2Pt[iChannel] = ROOT.TH1D(
                "h_Leptonbjet2Pt_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_pt, pt_min, pt_max
                )
        h_lb2Pt[iChannel].GetXaxis().SetTitle("p_{T}_{lb_{2}}(GeV)")
        h_lb2Pt[iChannel].GetYaxis().SetTitle("Entries")
        h_lb2Pt[iChannel].Sumw2()

        h_lb2Eta[iChannel] = ROOT.TH1D(
                "h_Leptonbjet2Eta_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_eta, eta_min, eta_max
                )
        h_lb2Eta[iChannel].GetXaxis().SetTitle("#eta_{lb_{2}}")
        h_lb2Eta[iChannel].GetYaxis().SetTitle("Entries")
        h_lb2Eta[iChannel].Sumw2()

        h_lb2Phi[iChannel] = ROOT.TH1D(
                "h_Leptonbjet2Phi_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_phi, phi_min, phi_max
                )
        h_lb2Phi[iChannel].GetXaxis().SetTitle("#phi_{lb_{2}}")
        h_lb2Phi[iChannel].GetYaxis().SetTitle("Entries")
        h_lb2Phi[iChannel].Sumw2()

        h_lb2Mass[iChannel] = ROOT.TH1D(
                "h_Leptonbjet2Mass_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_mass, mass_min, mass_max
                )
        h_lb2Mass[iChannel].GetXaxis().SetTitle("m_{lb_{2}}(GeV)")
        h_lb2Mass[iChannel].GetYaxis().SetTitle("Entries")
        h_lb2Mass[iChannel].Sumw2()

        h_diPt[iChannel] = ROOT.TH1D(
                "h_b4SumPt_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_pt, pt_min, pt_max
                )
        h_diPt[iChannel].GetXaxis().SetTitle("p_{T}_{b#bar{b}}(GeV)")
        h_diPt[iChannel].GetYaxis().SetTitle("Entries")
        h_diPt[iChannel].Sumw2()

        h_diEta[iChannel] = ROOT.TH1D(
                "h_b4SumEta_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_eta, eta_min, eta_max
                )
        h_diEta[iChannel].GetXaxis().SetTitle("#eta_{b#bar{b}}")
        h_diEta[iChannel].GetYaxis().SetTitle("Entries")
        h_diEta[iChannel].Sumw2()

        h_diPhi[iChannel] = ROOT.TH1D(
                "h_b4SumPhi_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_phi, phi_min, phi_max
                )
        h_diPhi[iChannel].GetXaxis().SetTitle("#phi_{b#bar{b}}")
        h_diPhi[iChannel].GetYaxis().SetTitle("Entries")
        h_diPhi[iChannel].Sumw2()

        h_diMass[iChannel] = ROOT.TH1D(
                "h_b4SumMass_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_mass, mass_min, mass_max
                )
        h_diMass[iChannel].GetXaxis().SetTitle("m_{b#bar{b}}(GeV)")
        h_diMass[iChannel].GetYaxis().SetTitle("Entries")
        h_diMass[iChannel].Sumw2()

        h_csv1[iChannel] = ROOT.TH1D(
                "h_bjet1CSV_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_csv, csv_min, csv_max
                )
        h_csv1[iChannel].GetXaxis().SetTitle("CSVv2_{b_{1}}")
        h_csv1[iChannel].GetYaxis().SetTitle("Entries")
        h_csv1[iChannel].Sumw2()

        h_csv2[iChannel] = ROOT.TH1D(
                "h_bjet2CSV_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_csv, csv_min, csv_max
                )
        h_csv2[iChannel].GetXaxis().SetTitle("CSVv2_{b_{2}}")
        h_csv2[iChannel].GetYaxis().SetTitle("Entries")
        h_csv2[iChannel].Sumw2()

        h_pt1[iChannel] = ROOT.TH1D(
                "h_bjet1Pt_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_pt, pt_min, pt_max
                )
        h_pt1[iChannel].GetXaxis().SetTitle("p_{T}_{b_{1}}(GeV)")
        h_pt1[iChannel].GetYaxis().SetTitle("Entries")
        h_pt1[iChannel].Sumw2()

        h_pt2[iChannel] = ROOT.TH1D(
                "h_bjet2Pt_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_pt, pt_min, pt_max
                )
        h_pt2[iChannel].GetXaxis().SetTitle("p_{T}_{b_{2}}(GeV)")
        h_pt2[iChannel].GetYaxis().SetTitle("Entries")
        h_pt2[iChannel].Sumw2()

        h_eta1[iChannel] = ROOT.TH1D(
                "h_bjet1Eta_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_eta, eta_min, eta_max
                )
        h_eta1[iChannel].GetXaxis().SetTitle("#eta_{b_{1}}")
        h_eta1[iChannel].GetYaxis().SetTitle("Entries")
        h_eta1[iChannel].Sumw2()

        h_eta2[iChannel] = ROOT.TH1D(
                "h_bjet2Eta_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_eta, eta_min, eta_max
                )
        h_eta2[iChannel].GetXaxis().SetTitle("#eta_{b_{2}}")
        h_eta2[iChannel].GetYaxis().SetTitle("Entries")
        h_eta2[iChannel].Sumw2()

        h_phi1[iChannel] = ROOT.TH1D(
                "h_bjet1Phi_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_phi, phi_min, phi_max
                )
        h_phi1[iChannel].GetXaxis().SetTitle("#phi_{b_{1}}")
        h_phi1[iChannel].GetYaxis().SetTitle("Entries")
        h_phi1[iChannel].Sumw2()

        h_phi2[iChannel] = ROOT.TH1D(
                "h_bjet2Phi_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_phi, phi_min, phi_max
                )
        h_phi2[iChannel].GetXaxis().SetTitle("#phi_{b_{2}}")
        h_phi2[iChannel].GetYaxis().SetTitle("Entries")
        h_phi2[iChannel].Sumw2()

        h_e1[iChannel] = ROOT.TH1D(
                "h_bjet1Energy_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_energy, energy_min, energy_max
                )
        h_e1[iChannel].GetXaxis().SetTitle("E_{b_{1}}(GeV)")
        h_e1[iChannel].GetYaxis().SetTitle("Entries")
        h_e1[iChannel].Sumw2()

        h_e2[iChannel] = ROOT.TH1D(
                "h_bjet2Energy_Ch"+str(iChannel)+"_S3_"+process,"",
                nbins_energy, energy_min, energy_max
                )
        h_e2[iChannel].GetXaxis().SetTitle("E_{b_{2}}(GeV)")
        h_e2[iChannel].GetYaxis().SetTitle("Entries")
        h_e2[iChannel].Sumw2()

    if ttbb == True :
        genchain = TChain("ttbbLepJets/gentree")
        if("Herwig" in process): genchain.Add("/data/users/seohyun/ntuple/hep2017/v806/"+process+".root")
        else: genchain.Add("/data/users/seohyun/ntuple/hep2017/v808/nosplit/"+process+".root")

        print "GENTREE RUN"
        for i in range(1000):#xrange(genchain.GetEntries()) :
            #if closureTest:
            #    if i%2 == 0 : continue
            pp.printProgress(i, genchain.GetEntries(), 'Progress:', 'Complete', 1, 50)
            genchain.GetEntry(i)
            addbjet1 = TLorentzVector()
            addbjet2 = TLorentzVector()
            addbjet1.SetPtEtaPhiE(genchain.addbjet1_pt, genchain.addbjet1_eta, genchain.addbjet1_phi, genchain.addbjet1_e)
            addbjet2.SetPtEtaPhiE(genchain.addbjet2_pt, genchain.addbjet2_eta, genchain.addbjet2_phi, genchain.addbjet2_e)

            gendR = addbjet1.DeltaR(addbjet2)
            genM = (addbjet1+addbjet2).M()

            if genchain.genchannel == muon_ch :
                h_gen_addbjets_deltaR_nosel[muon_ch].Fill(gendR,genchain.genweight)
                h_gen_addbjets_invMass_nosel[muon_ch].Fill(genM,genchain.genweight)
            elif genchain.genchannel == electron_ch :
                h_gen_addbjets_deltaR_nosel[electron_ch].Fill(gendR,genchain.genweight)
                h_gen_addbjets_invMass_nosel[electron_ch].Fill(genM,genchain.genweight)
            else : print("Error")

    print "\nTREE RUN"
    nEvents = 0
    nEvt_isMatch_DNN = 0
    nEvt_isMatch_mindR = 0
    f_pred = open('pred.txt','w')
    for item in os.listdir(inputDir) :
        #print "Load file : "+str(inputDir)+'/'+str(item)
        df = pd.read_hdf(inputDir+'/'+item)
        str_query = 'csv1 > '+str(jet_CSV)+' and csv2 > '+str(jet_CSV)+' and njets >= 6 and nbjets >= 3'
        selEvent = df.query(str_query)
        selEvent.reset_index(drop=True, inplace=True)
        if len(selEvent.index) == 0 : continue
        nTotal = selEvent['event'].iloc[-1]
        jetCombi = selEvent.filter([
            'dR','dEta','dPhi',
            'nuPt','nuEta','nuPhi','nuMass',
            'lbPt','lbEta','lbPhi','lbMass',
            'lb1Pt','lb1Eta','lb1Phi','lb1Mass',
            'lb2Pt','lb2Eta','lb2Phi','lb2Mass',
            'diPt','diEta','diPhi','diMass',
            'csv1','csv2','pt1','pt2','eta1','eta2','phi1','phi2','e1','e2'
        ])
        scaler = StandardScaler()
        if len(jetCombi) is not 0 :
            inputset = np.array(jetCombi)
            inputset_sc = scaler.fit_transform(inputset)
            pred = model.predict(inputset_sc, batch_size = 2000)

        pred = pd.DataFrame(pred, columns=['background','signal'])
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
        f_pred.write("Groupby\n"+item+"\n"+str(selEvent))
        #groups = selEvent.groupby('event')
        for index, event in selEvent.iterrows() :
            #maxval = event[1][event[1]['signal'] == event[1]['signal'].max()]
            pp.printProgress(event['event'], nTotal, str(item)+':','Complete',1,25)

            eventweight = event['PUWeight']*event['genWeight']
            if not data :  eventweight *= event['lepton_SF']*event['jet_SF_CSV']

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
                h_dR[passchannel].Fill(reco_dR, eventweight)
                h_dEta[passchannel].Fill(event['dEta'], eventweight)
                h_dPhi[passchannel].Fill(event['dPhi'], eventweight)
                h_nuPt[passchannel].Fill(event['nuPt'], eventweight)
                h_nuEta[passchannel].Fill(event['nuEta'], eventweight)
                h_nuPhi[passchannel].Fill(event['nuPhi'], eventweight)
                h_nuMass[passchannel].Fill(event['nuMass'], eventweight)
                h_lbPt[passchannel].Fill(event['lbPt'], eventweight)
                h_lbEta[passchannel].Fill(event['lbEta'], eventweight)
                h_lbPhi[passchannel].Fill(event['lbPhi'], eventweight)
                h_lbMass[passchannel].Fill(event['lbMass'], eventweight)
                h_lb1Pt[passchannel].Fill(event['lb1Pt'], eventweight)
                h_lb1Eta[passchannel].Fill(event['lb1Eta'], eventweight)
                h_lb1Phi[passchannel].Fill(event['lb1Phi'], eventweight)
                h_lb1Mass[passchannel].Fill(event['lb1Mass'], eventweight)
                h_lb2Pt[passchannel].Fill(event['lb2Pt'], eventweight)
                h_lb2Eta[passchannel].Fill(event['lb2Eta'], eventweight)
                h_lb2Phi[passchannel].Fill(event['lb2Phi'], eventweight)
                h_lb2Mass[passchannel].Fill(event['lb2Mass'], eventweight)
                h_diPt[passchannel].Fill(event['diPt'], eventweight)
                h_diEta[passchannel].Fill(event['diEta'], eventweight)
                h_diPhi[passchannel].Fill(event['diPhi'], eventweight)
                h_diMass[passchannel].Fill(event['diMass'], eventweight)
                h_csv1[passchannel].Fill(event['csv1'], eventweight)
                h_csv2[passchannel].Fill(event['csv2'], eventweight)
                h_pt1[passchannel].Fill(event['pt1'], eventweight)
                h_pt2[passchannel].Fill(event['pt2'], eventweight)
                h_eta1[passchannel].Fill(event['eta1'], eventweight)
                h_eta2[passchannel].Fill(event['eta2'], eventweight)
                h_phi1[passchannel].Fill(event['phi1'], eventweight)
                h_phi2[passchannel].Fill(event['phi2'], eventweight)
                h_e1[passchannel].Fill(event['e1'], eventweight)
                h_e2[passchannel].Fill(event['e2'], eventweight)
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
        print "Matching ratio with matchable events from DNN : "+str(matching_DNN_able)+"("+str(nEvt_isMatch_DNN)+"/"+str(nMatchable)+")"
        print "Matching ratio with step 3 events from DNN : "+str(matching_DNN)+"("+str(nEvt_isMatch_DNN)+"/"+str(nEvents)+")"
        #print "Matching Ratio from minimun dR : "+str(matching_mindR)+"("+str(nEvt_isMatch_mindR)+"/"+str(nEvents)+")"
        f_ratio = open('ratio.txt','a')
        f_ratio.write(modelfile)
        f_ratio.write("\nMatching ratio with matchable events from DNN: "+str(matching_DNN_able)+"("+str(nEvt_isMatch_DNN)+"/"+str(nMatchable)+")")
        f_ratio.close()

    for iChannel in range(nChannel) :
        h_njets[iChannel].AddBinContent(10,h_njets[iChannel].GetBinContent(11))
        h_nbjets[iChannel].AddBinContent(10,h_nbjets[iChannel].GetBinContent(11))
        h_reco_addjets_deltaR[iChannel].AddBinContent(nbins_reco_addjets_dr, h_reco_addjets_deltaR[iChannel].GetBinContent(nbins_reco_addjets_dr+1))
        h_reco_addjets_invMass[iChannel].AddBinContent(nbins_reco_addjets_mass, h_reco_addjets_invMass[iChannel].GetBinContent(nbins_reco_addjets_mass+1))
        h_gen_addbjets_deltaR[iChannel].AddBinContent(nbins_gen_addjets_dr, h_gen_addbjets_deltaR[iChannel].GetBinContent(nbins_gen_addjets_dr+1))
        h_gen_addbjets_invMass[iChannel].AddBinContent(nbins_gen_addjets_mass, h_gen_addbjets_invMass[iChannel].GetBinContent(nbins_gen_addjets_mass+1))

        h_dR[iChannel].AddBinContent(nbins_delta_r, h_dR[iChannel].GetBinContent(nbins_delta_r+1))
        h_dEta[iChannel].AddBinContent(nbins_delta_eta, h_dEta[iChannel].GetBinContent(nbins_delta_eta+1))
        h_dPhi[iChannel].AddBinContent(nbins_delta_phi, h_dPhi[iChannel].GetBinContent(nbins_delta_phi+1))
        h_nuPt[iChannel].AddBinContent(nbins_pt, h_nuPt[iChannel].GetBinContent(nbins_pt+1))
        h_nuEta[iChannel].AddBinContent(nbins_eta, h_nuEta[iChannel].GetBinContent(nbins_eta+1))
        h_nuPhi[iChannel].AddBinContent(nbins_phi, h_nuPhi[iChannel].GetBinContent(nbins_phi+1))
        h_nuMass[iChannel].AddBinContent(nbins_mass, h_nuMass[iChannel].GetBinContent(nbins_mass+1))
        h_lbPt[iChannel].AddBinContent(nbins_pt, h_lbPt[iChannel].GetBinContent(nbins_pt+1))
        h_lbEta[iChannel].AddBinContent(nbins_eta, h_lbEta[iChannel].GetBinContent(nbins_eta+1))
        h_lbPhi[iChannel].AddBinContent(nbins_phi, h_lbPhi[iChannel].GetBinContent(nbins_phi+1))
        h_lbMass[iChannel].AddBinContent(nbins_mass, h_lbMass[iChannel].GetBinContent(nbins_mass+1))
        h_lb1Pt[iChannel].AddBinContent(nbins_pt, h_lb1Pt[iChannel].GetBinContent(nbins_pt+1))
        h_lb1Eta[iChannel].AddBinContent(nbins_eta, h_lb1Eta[iChannel].GetBinContent(nbins_eta+1))
        h_lb1Phi[iChannel].AddBinContent(nbins_phi, h_lb1Phi[iChannel].GetBinContent(nbins_phi+1))
        h_lb1Mass[iChannel].AddBinContent(nbins_mass, h_lb1Mass[iChannel].GetBinContent(nbins_mass+1))
        h_lb2Pt[iChannel].AddBinContent(nbins_pt, h_lb2Pt[iChannel].GetBinContent(nbins_pt+1))
        h_lb2Eta[iChannel].AddBinContent(nbins_eta, h_lb2Eta[iChannel].GetBinContent(nbins_eta+1))
        h_lb2Phi[iChannel].AddBinContent(nbins_phi, h_lb2Phi[iChannel].GetBinContent(nbins_phi+1))
        h_lb2Mass[iChannel].AddBinContent(nbins_mass, h_lb2Mass[iChannel].GetBinContent(nbins_mass+1))
        h_diPt[iChannel].AddBinContent(nbins_pt, h_diPt[iChannel].GetBinContent(nbins_pt+1))
        h_diEta[iChannel].AddBinContent(nbins_eta, h_diEta[iChannel].GetBinContent(nbins_eta+1))
        h_diPhi[iChannel].AddBinContent(nbins_phi, h_diPhi[iChannel].GetBinContent(nbins_phi+1))
        h_diMass[iChannel].AddBinContent(nbins_mass, h_diMass[iChannel].GetBinContent(nbins_mass+1))
        h_csv1[iChannel].AddBinContent(nbins_csv, h_csv1[iChannel].GetBinContent(nbins_csv+1))
        h_csv2[iChannel].AddBinContent(nbins_csv, h_csv2[iChannel].GetBinContent(nbins_csv+1))
        h_pt1[iChannel].AddBinContent(nbins_pt, h_pt1[iChannel].GetBinContent(nbins_pt+1))
        h_pt2[iChannel].AddBinContent(nbins_pt, h_pt2[iChannel].GetBinContent(nbins_pt+1))
        h_eta1[iChannel].AddBinContent(nbins_eta, h_eta1[iChannel].GetBinContent(nbins_eta+1))
        h_eta2[iChannel].AddBinContent(nbins_eta, h_eta2[iChannel].GetBinContent(nbins_eta+1))
        h_phi1[iChannel].AddBinContent(nbins_phi, h_phi1[iChannel].GetBinContent(nbins_phi+1))
        h_phi2[iChannel].AddBinContent(nbins_phi, h_phi2[iChannel].GetBinContent(nbins_phi+1))
        h_e1[iChannel].AddBinContent(nbins_energy, h_e1[iChannel].GetBinContent(nbins_energy+1))
        h_e2[iChannel].AddBinContent(nbins_energy, h_e2[iChannel].GetBinContent(nbins_energy+1))
        h_dR[iChannel].AddBinContent(1, h_dR[iChannel].GetBinContent(0))
        h_dEta[iChannel].AddBinContent(1, h_dEta[iChannel].GetBinContent(0))
        h_dPhi[iChannel].AddBinContent(1, h_dPhi[iChannel].GetBinContent(0))
        h_nuPt[iChannel].AddBinContent(1, h_nuPt[iChannel].GetBinContent(0))
        h_nuEta[iChannel].AddBinContent(1, h_nuEta[iChannel].GetBinContent(0))
        h_nuPhi[iChannel].AddBinContent(1, h_nuPhi[iChannel].GetBinContent(0))
        h_nuMass[iChannel].AddBinContent(1, h_nuMass[iChannel].GetBinContent(0))
        h_lbPt[iChannel].AddBinContent(1, h_lbPt[iChannel].GetBinContent(0))
        h_lbEta[iChannel].AddBinContent(1, h_lbEta[iChannel].GetBinContent(0))
        h_lbPhi[iChannel].AddBinContent(1, h_lbPhi[iChannel].GetBinContent(0))
        h_lbMass[iChannel].AddBinContent(1, h_lbMass[iChannel].GetBinContent(0))
        h_lb1Pt[iChannel].AddBinContent(1, h_lb1Pt[iChannel].GetBinContent(0))
        h_lb1Eta[iChannel].AddBinContent(1, h_lb1Eta[iChannel].GetBinContent(0))
        h_lb1Phi[iChannel].AddBinContent(1, h_lb1Phi[iChannel].GetBinContent(0))
        h_lb1Mass[iChannel].AddBinContent(1, h_lb1Mass[iChannel].GetBinConetnt(0))
        h_lb2Pt[iChannel].AddBinContent(1, h_lb2Pt[iChannel].GetBinContent(0))
        h_lb2Eta[iChannel].AddBinContent(1, h_lb2Eta[iChannel].GetBinContent(0))
        h_lb2Phi[iChannel].AddBinContent(1, h_lb2Phi[iChannel].GetBinContent(0))
        h_lb2Mass[iChannel].AddBinContent(1, h_lb2Mass[iChannel].GetBinConetnt(0))
        h_diPt[iChannel].AddBinContent(1, h_diPt[iChannel].GetBinContent(0))
        h_diEta[iChannel].AddBinContent(1, h_diEta[iChannel].GetBinContent(0))
        h_diPhi[iChannel].AddBinContent(1, h_diPhi[iChannel].GetBinContent(0))
        h_diMass[iChannel].AddBinContent(1, h_diMass[iChannel].GetBinContent(0))
        h_csv1[iChannel].AddBinContent(1, h_csv1[iChannel].GetBinContent(0))
        h_csv2[iChannel].AddBinContent(1, h_csv2[iChannel].GetBinContent(0))
        h_pt1[iChannel].AddBinContent(1, h_pt1[iChannel].GetBinContent(0))
        h_pt2[iChannel].AddBinContent(1, h_pt2[iChannel].GetBinContent(0))
        h_eta1[iChannel].AddBinContent(1, h_eta1[iChannel].GetBinContent(0))
        h_eta2[iChannel].AddBinContent(1, h_eta2[iChannel].GetBinContent(0))
        h_phi1[iChannel].AddBinContent(1, h_phi1[iChannel].GetBinContent(0))
        h_phi2[iChannel].AddBinContent(1, h_phi2[iChannel].GetBinContent(0))
        h_e1[iChannel].AddBinContent(1, h_e1[iChannel].GetBinContent(0))
        h_e2[iChannel].AddBinContent(1, h_e2[iChannel].GetBinContent(0))

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
        h_respMatrix_deltaR[iChannel].ClearUnderflowAndOverflow()
        h_respMatrix_invMass[iChannel].ClearUnderflowAndOverflow()
        h_dR[iChannel].ClearUnderflowAndOverflow()
        h_dEta[iChannel].ClearUnderflowAndOverflow()
        h_dPhi[iChannel].ClearUnderflowAndOverflow()
        h_nuPt[iChannel].ClearUnderflowAndOverflow()
        h_nuEta[iChannel].ClearUnderflowAndOverflow()
        h_nuPhi[iChannel].ClearUnderflowAndOverflow()
        h_nuMass[iChannel].ClearUnderflowAndOverflow()
        h_lbPt[iChannel].ClearUnderflowAndOverflow()
        h_lbEta[iChannel].ClearUnderflowAndOverflow()
        h_lbPhi[iChannel].ClearUnderflowAndOverflow()
        h_lbMass[iChannel].ClearUnderflowAndOverflow()
        h_lb1Pt[iChannel].ClearUnderflowAndOverflow()
        h_lb1Eta[iChannel].ClearUnderflowAndOverflow()
        h_lb1Phi[iChannel].ClearUnderflowAndOverflow()
        h_lb1Mass[iChannel].ClearUnderflowAndOverflow()
        h_lb2Pt[iChannel].ClearUnderflowAndOverflow()
        h_lb2Eta[iChannel].ClearUnderflowAndOverflow()
        h_lb2Phi[iChannel].ClearUnderflowAndOverflow()
        h_lb2Mass[iChannel].ClearUnderflowAndOverflow()
        h_diPt[iChannel].ClearUnderflowAndOverflow()
        h_diEta[iChannel].ClearUnderflowAndOverflow()
        h_diPhi[iChannel].ClearUnderflowAndOverflow()
        h_diMass[iChannel].ClearUnderflowAndOverflow()
        h_csv1[iChannel].ClearUnderflowAndOverflow()
        h_csv2[iChannel].ClearUnderflowAndOverflow()
        h_pt1[iChannel].ClearUnderflowAndOverflow()
        h_pt2[iChannel].ClearUnderflowAndOverflow()
        h_eta1[iChannel].ClearUnderflowAndOverflow()
        h_eta2[iChannel].ClearUnderflowAndOverflow()
        h_phi1[iChannel].ClearUnderflowAndOverflow()
        h_phi2[iChannel].ClearUnderflowAndOverflow()
        h_e1[iChannel].ClearUnderflowAndOverflow()
        h_e2[iChannel].ClearUnderflowAndOverflow()

    if("Herwig" in process):
        f = TFile("/data/users/seohyun/ntuple/hep2017/v806/"+process+".root")
    else:
        f = TFile("/data/users/seohyun/ntuple/hep2017/v808/nosplit/"+process+".root")

    h_evt = f.Get("ttbbLepJets/EventInfo")
    f_out.cd()
    h_evt.Write()
    f_out.Write()
    f_out.Close()
    f_pred.close()

    timer.Stop()
    realtime = timer.RealTime()
    cputime = timer.CpuTime()
    print("Real Time : {0:6.2f} seconds, CPU Time : {1:6.2f} seconds").format(realtime,cputime)
