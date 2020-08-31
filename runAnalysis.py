#!/usr/bin/python
import os
import sys
import subprocess
import argparse

import python.storeSampleInfo
import python.getQCD
import python.runTSelector

def str2bool(v):
    if v.lower() in ('yes', 'true', 't', 'y', '1', 'True'):
        return True
    elif v.lower() in ('no', 'false', 'f', 'n', '0', 'False'):
        return False
    else:
        raise argparse.ArgumentTypeError('Boolean value expected')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Analyze ntuples')

    parser.add_argument('-y', '--year', required=False, type=int, default=9999, help='Run special year')
    parser.add_argument('-t', '--test', required=False, type=str2bool, default=False, help='Run test sample')
    parser.add_argument('-q', '--qcd',  required=False, type=str2bool, default=False, help='QCD estimation')

    args = parser.parse_args()

    run16  = False 
    run17  = False 
    run18  = False

    if args.year == 16 or args.year == 2016: run16 = True
    if args.year == 17 or args.year == 2017: run17 = True
    if args.year == 18 or args.year == 2018: run18 = True
    if args.year == 9999:
        run16 = True
        run17 = True
        run18 = True

    python.storeSampleInfo.storeSampleList(os.getcwd()+'/samples')

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
                    inputDir = line[:-1]
                else:
                    samples.append(line[:-1])
                n += 1

        data = []
        with open('samples/data'+str(year)+'.txt', 'r') as f:
            while True:
                line = f.readline()
                if not line: break
                data.append(line[:-1])
        
        samples += data
                   
        #Compile TSelector
        python.runAna.runAna(year, inputDir+'/nosplit', "TTLJ_PowhegPythia_ttbb.root", "Nosys_Compile", False, False)
        
        if args.test:
            testDataset = ['TTLJ_PowhegPythia_ttbb']
            for sample in testDataset:
                for item in os.listdir(inputDir+sample):
                    cmd = ['sbatch','job_slurm.sh', str(year), inputDir+'/split/'+sample, item, sample, 'False', 'False']
                    subprocess.call(cmd)

        elif args.qcd:
            for sample in samples:
                cmd = ['sbatch','job_slurm.sh',str(year),inputDir+'/nosplit',sample+'.root','Nosys_'+sample,'False','False']
                subprocess.call(cmd)
                cmd = ['sbatch','job_slurm.sh',str(year),inputDir+'/nosplit',sample+'.root','dataDriven_'+sample,'True','False']
                subprocess.call(cmd)
                cmd = ['sbatch','job_slurm.sh',str(year),inputDir+'/nosplit',sample+'.root','dataDriven_'+sample+'__qcdisoup','True','False']
                subprocess.call(cmd)
                cmd = ['sbatch','job_slurm.sh',str(year),inputDir+'/nosplit',sample+'.root','dataDriven_'+sample+'__qcdisodown','True','False']
                subprocess.call(cmd)
             
            #python.getQCD.getQCDShape(os.getcwd(), year, samples)
        else:
            for sample in samples:
                for item in os.listdir(inputDir+sample):
                    cmd = ['sbatch','job_slurm.sh',str(year),inputDir+sample,item,sample,'False','False']
                    subprocess.call(cmd)
 
            systematics = {}
            with open('samples/systematics'+str(year)+'.txt', 'r') as f:
                while True:
                    line = f.readline()
                    if not line: break
                    line = line.replace('\n','')
                    line = line.split(' ')
                    systematics[line[0]] = line[1]
           
            for sample in samples:
                if "QCD" in sample or "Data" in sample: continue
                jetsyst = ['jerup','jerdown','jecup','jecdown']
                for syst in jetsyst:
                    cmd = ['sbatch','job_slurm.sh',str(year),inputDir+'/nosplit',sample+'.root',sample+'__'+syst,'False','False']
                    subprocess.call(cmd)
            
            for key, value in systematics.items():
                tmp = key.split('_')
                if 'Bkg' in key:
                    outName = tmp[0]+'_'+tmp[1]+'_'+tmp[-1]+'__'+value
                else:
                    outName = tmp[0]+'_'+tmp[1]+'__'+value
                cmd = ['sbatch','job_slurm.sh',str(year),inputDir+'/nosplit',key+'.root',outName, 'False','False']
                subprocess.call(cmd)
