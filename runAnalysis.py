#!/usr/bin/python
import os
import sys
import time
from datetime import datetime

import multiprocessing as mp
import argparse

from ROOT import TChain, TProof, TFile, TH1D, TH1F, TCanvas

import macro.getSampleList
import macro.getQCD

def str2bool(v):
    if v.lower() in ('yes', 'true', 't', 'y', '1', 'True'):
        return True
    elif v.lower() in ('no', 'false', 'f', 'n', '0', 'False'):
        return False
    else:
        raise argparse.ArgumentTypeError('Boolean value expected')

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
    
    print("End Process "+str(os.getpid())+": "+str(process))

if __name__ == '__main__':
    start_time = time.time()

    parser = argparse.ArgumentParser(description='Analyze ntuples')

    parser.add_argument('-t', '--test', required=False, type=str2bool, default=False, help='Run test sample')
    parser.add_argument('-y', '--year', required=False, type=int, default=9999, help='Run special year')
    parser.add_argument('-q', '--qcd',  required=False, type=str2bool, default=False, help='QCD estimation')
    parser.add_argument('-d', '--dataset', required=False, type=str, default='all', help='Option: all, data, ttbar, other. run splited dataset')

    args = parser.parse_args()

    run16  = False 
    run17  = False 
    run18  = False
    dataset = "all"

    if args.year == 16 or args.year == 2016: run16 = True
    if args.year == 17 or args.year == 2017: run17 = True
    if args.year == 18 or args.year == 2018: run18 = True
    if args.year == 9999:
        run16 = True
        run17 = True
        run18 = True

    dataset = args.dataset

    macro.getSampleList.getSampleList(os.getcwd()+'/samples')

    for year in range(16,19):
        if year == 16 and run16 == False: continue
        if year == 17 and run17 == False: continue
        if year == 18 and run18 == False: continue

        samples = []
        with open('samples/sample'+str(year)+'.txt','r') as f:
            n = 1
            while True:
                line = f.readline()
                if not line: break
                if n == 1:
                    inputDir_nosplit = line[:-1]
                    inputDir = line[:-8]+'split/'
                else:
                    samples.append(line[:-1])
                n += 1

        data = []
        with open('samples/data'+str(year)+'.txt', 'r') as f:
            while True:
                line = f.readline()
                if not line: break
                data.append(line[:-1])
        
        if args.qcd:
            samples.append("DataSingleMu")
            samples.append("DataSingleEG")
        else:
            samples += data
                   
        #Compiling TSelector
        if dataset == 'data' or dataset == 'all':
            runAna(year, inputDir_nosplit, "TTLJ_PowhegPythia_ttbb.root", "Nosys_Compile", False, False)
        
        if args.test:
            for sample in samples:
                procs = [] 
                for item in os.listdir(inputDir+sample):
                    proc = mp.Process(target=runAna, args=(year, inputDir+sample, item, 'Nosys_'+sample))
                    procs.append(proc)
                    proc.start()
                for proc in procs:
                    proc.join()

            outdir = 'output/root'+str(year)+'_test'
            if os.path.exists(outdir):
                os.system('mv '+outdir+' '+outdir+'_'+str(datetime.today().strftime("%Y%m%d")))
            os.system('mv output/root'+str(year)+' '+outdir)

        elif args.qcd:
            p = TProof.Open("", "workers=8")

            for sample in samples:
                runAna(year, inputDir, str(sample)+".root", "dataDriven_"+str(sample))
                      
            outdir = 'output/root'+str(year)+'_qcd'
            if os.path.exists(outdir): 
                os.system('mv '+outdir+' '+outdir+'_backup_'+str(datetime.today().strftime("%Y%m%d")))
            os.system('mv output/root'+str(year)+' '+outdir)

            macro.getQCD.getQCDShape(os.getcwd(), year, samples)
        else:
            for sample in samples:
                run = False
                if dataset == "data":
                    if any(i in sample for i in ['Data', 'ttLF']): 
                        run = True
                elif dataset == "ttbar":
                    if 'TT' in sample:
                        if not any(i in sample for i in ['ttother', 'ttLF']):
                            run = True
                elif dataset == "other":
                    if not any(i in sample for i in ['Data', 'TT']):
                        run = True
                    elif 'ttother' in sample:
                        run = True
                else:
                    print("There is no such dataset")
                    break
               
                if not run: continue

                procs = [] 
                for item in os.listdir(inputDir+sample):
                    proc = mp.Process(target=runAna, args=(year, inputDir+sample, item, sample))
                    procs.append(proc)
                    proc.start()
                for proc in procs:
                    proc.join()

            if dataset == 'data' or dataset == 'all':
                p = TProof.Open("", "workers=8")

                for sample in samples:
                    if not "QCD" in sample and not "Data" in sample:
                        runAna(year, inputDir_nosplit, str(sample)+".root", str(sample)+"__jerup", False, True)
                        runAna(year, inputDir_nosplit, str(sample)+".root", str(sample)+"__jerdown", False, True)
                        runAna(year, inputDir_nosplit, str(sample)+".root", str(sample)+"__jecup", False, True)
                        runAna(year, inputDir_nosplit, str(sample)+".root", str(sample)+"__jecdown", False, True)

                # hdamp, Tune
                if not year == 16:
                    tmp_dic = {"TuneCP5Up":"tuneup", "TuneCP5Down":"tunedown", "hdampUp":"hdampup", "hdampDown":"hdampdown"}
                    for key, value in tmp_dic.items():
                        runAna(year, inputDir_nosplit, "TTLJ_PowhegPythia_SYS_"+key+"_ttbb.root", "TTLJ_PowhegPythia_ttbb__"+value, False, True)
                        runAna(year, inputDir_nosplit, "TTLJ_PowhegPythia_SYS_"+key+"_ttbj.root", "TTLJ_PowhegPythia_ttbj__"+value, False, True)
                        runAna(year, inputDir_nosplit, "TTLJ_PowhegPythia_SYS_"+key+"_ttcc.root", "TTLJ_PowhegPythia_ttcc__"+value, False, True)
                        runAna(year, inputDir_nosplit, "TTLJ_PowhegPythia_SYS_"+key+"_ttLF.root", "TTLJ_PowhegPythia_ttLF__"+value, False, True)
                        runAna(year, inputDir_nosplit, "TTLJ_PowhegPythia_SYS_"+key+"_ttother.root", "TTLJ_PowhegPythia_ttother__"+value, False, True)
                        runAna(year, inputDir_nosplit, "TTJJ_PowhegPythiaBkg_SYS_"+key+".root", "TTJJ_PowhegPythiaBkg__"+value, False, True)
                        runAna(year, inputDir_nosplit, "TTLJ_PowhegPythiaBkg_SYS_"+key+".root", "TTLJ_PowhegPythiaBkg__"+value, False, True)
                        runAna(year, inputDir_nosplit, "TTLL_PowhegPythiaBkg_SYS_"+key+".root", "TTLL_PowhegPythiaBkg__"+value, False, True)
                else:
                    tmp_dic = {"TuneUp":"tuneup", "TuneDown":"tunedown", "hdampUp":"hdampup", "hdampDown":"hdampdown"}
                    for key, value in tmp_dic.items():
                        if "tune" in value: tmp_name = "TT_PowhegPythia"
                        else: tmp_name = "TTLJ_PowhegPythia"
                        runAna(year, inputDir_nosplit, tmp_name+"_SYS_"+key+"_ttbb.root", tmp_name+"_ttbb__"+value, False, True)
                        runAna(year, inputDir_nosplit, tmp_name+"_SYS_"+key+"_ttbj.root", tmp_name+"_ttbj__"+value, False, True)
                        runAna(year, inputDir_nosplit, tmp_name+"_SYS_"+key+"_ttcc.root", tmp_name+"_ttcc__"+value, False, True)
                        runAna(year, inputDir_nosplit, tmp_name+"_SYS_"+key+"_ttLF.root", tmp_name+"_ttLF__"+value, False, True)
                        runAna(year, inputDir_nosplit, tmp_name+"_SYS_"+key+"_ttother.root", tmp_name+"_ttother__"+value, False, True)
                        if not "hdamp" in value:
                            runAna(year, inputDir_nosplit, "TT_PowhegPythiaBkg_SYS_"+key+".root", "TT_PowhegPythiaBkg__"+value, False, True)
            
                # 2016 only (Isr, Fsr)
                if year == 16:
                    tmp_name = "TT_PowhegPythia_"
                    tmp_name2 = "TT_PowhegPythiaBkg_"
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_ISRup_ttbb.root",      tmp_name+"ttbb__isrup", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_ISRup_ttbj.root",      tmp_name+"ttbj__isrup", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_ISRup_ttcc.root",      tmp_name+"ttcc__isrup", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_ISRup_ttLF.root",      tmp_name+"ttLF__isrup", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_ISRup_ttother.root",   tmp_name+"ttother__isrup", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_ISRdown_ttbb.root",    tmp_name+"ttbb__isrdown", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_ISRdown_ttbj.root",    tmp_name+"ttbj__isrdown", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_ISRdown_ttcc.root",    tmp_name+"ttcc__isrdown", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_ISRdown_ttLF.root",    tmp_name+"ttLF__isrdown", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_ISRdown_ttother.root", tmp_name+"ttother__isrdown", False, True)
     
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_FSRup_ttbb.root",      tmp_name+"ttbb__fsrup", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_FSRup_ttbj.root",      tmp_name+"ttbj__fsrup", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_FSRup_ttcc.root",      tmp_name+"ttcc__fsrup", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_FSRup_ttLF.root",      tmp_name+"ttLF__fsrup", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_FSRup_ttother.root",   tmp_name+"ttother__fsrup", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_FSRdown_ttbb.root",    tmp_name+"ttbb__fsrdown", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_FSRdown_ttbj.root",    tmp_name+"ttbj__fsrdown", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_FSRdown_ttcc.root",    tmp_name+"ttcc__fsrdown", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_FSRdown_ttLF.root",    tmp_name+"ttLF__fsrdown", False, True)
                    runAna(year, inputDir_nosplit, tmp_name+"SYS_FSRdown_ttother.root", tmp_name+"ttother__fsrdown", False, True)               
                    runAna(year, inputDir_nosplit, tmp_name2+"SYS_ISRup.root",          tmp_name2+"_isrup", False, True)
                    runAna(year, inputDir_nosplit, tmp_name2+"SYS_ISRdown.root",        tmp_name2+"_isrdown", False, True)
                    runAna(year, inputDir_nosplit, tmp_name2+"SYS_FSRup.root",          tmp_name2+"_fsrup", False, True)
                    runAna(year, inputDir_nosplit, tmp_name2+"SYS_FSRdown.root",        tmp_name2+"_fsrdown", False, True)

                p.Close()
    print("Total running time: %s" %(time.time() - start_time))
