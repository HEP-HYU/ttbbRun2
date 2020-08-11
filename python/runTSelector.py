#!/usr/bin/python
import os
import sys

from ROOT import TChain, TProof, TFile, TH1D, TH1F, TCanvas

def str2bool(v):
    if v.lower() in ('yes', 'true', 't', 'y', '1', 'True'):
        return True
    elif v.lower() in ('no', 'false', 'f', 'n', '0', 'False'):
        return False

def runAna(year, dir, file, name, isQCD=False, onProof=False):   
    # tmp: /data/users/seohyun/ntuple/Run2016/v808/nosplit/Nosys_Compile/TTLJ_PowhegPythia_ttbbFilter_ttbb
    # process: Run2016/TTLJ_PowhegPythia_ttbb__jecup/TTLJ_PowhegPythia_ttbb
    tmp = dir+'/'+str(name)+'/'+file[:-5]
    tmp = tmp.split('/')
    process = str(tmp[5])+'/'+str(tmp[-2])+'/'+str(tmp[-1])

    print("Begin Process "+str(os.getpid())+": "+str(process))

    outdir = 'output/root'+str(year)
    if 'Tree' in file:
        outdir += '/'+name
    try:
        os.makedirs(outdir)
    except OSError:
        print(outdir+": already exists")

    if isQCD:
        chain = TChain("ttbbLepJetsQCD/tree", "events")
    else:
        chain = TChain("ttbbLepJets/tree","events")

    chain.Add(dir+"/"+file)
    if onProof:
        p = TProof.Open("","worker=8")
        chain.SetProof()
    chain.Process("macro/MyAnalysis.C+", process)

    f = TFile(dir+"/"+file,"read")

    ## save Event Summary histogram ##
    if 'Tree' in process:
        out = TFile(outdir+'/hist_'+file, 'update')
    else:
        out = TFile(outdir+'/hist_'+name+'.root', 'update')

    hevt = f.Get("ttbbLepJets/EventInfo")
    hsw = f.Get("ttbbLepJets/ScaleWeights")
    hpdf = f.Get("ttbbLepJets/PDFWeights")
    hevt.Write()
    if not hpdf == None: hpdf.Write()
    if not hsw == None: hsw.Write()
    out.Write()
    out.Close()
    
    if onProof:
        p.Close()
    print("End Process "+str(os.getpid())+": "+str(process))

runAna(sys.argv[1], sys.agrv[2], sys.argv[3], sys.argv[4], str2bool(sys.argv[5]), str2bool(sys.argv[6]))
