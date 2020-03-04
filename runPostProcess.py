import os
import sys
import glob
import time
from datetime import datetime

import multiprocessing as mp
import subprocess
import argparse

from ROOT import *

import macro.postProcess as pp

def mergeHist(input_dir):
    tmp = input_dir.split('/')
    output_dir = tmp[0]+'/'+tmp[1]
    input_sample = tmp[-1]
    root_list = glob.glob(input_dir+'/*')
    cmd = ['hadd', str(output_dir)+'/hist_'+str(input_sample)+'.root']+glob.glob(input_dir+'/*')
    subprocess.call(cmd)

def runGentree(year):
    f_txt = open('samples/sample'+str(year)+'.txt','r')
    ntuple_loc = f_txt.readline()[:-1]
    f_txt.close()
    cmd = ['root', '-l', '-b', '-q', 
           'macro/runGentree.C("'+str(ntuple_loc)+'", "'+str(os.getcwd())+"/output/root"+str(year)+'")']
    subprocess.call(cmd)

def str2bool(v):
    if v.lower() in ('yes', 'true', 't', 'y', '1', 'True'):
        return True
    elif v.lower() in ('no', 'false', 'f', 'n', '0', 'False'):
        return False
    else:
        raise argparse.ArgumentTypeError('Boolean value expected')

if __name__ == '__main__':
    start_time = time.time()

    parser = argparse.ArgumentParser(description='Run post process')

    parser.add_argument('-y', '--year', required=False, type=int, default=9999, help='Run special year')
    parser.add_argument('-m', '--merge', required=False, type=str2bool, default=True, help='Merge histograms')
    parser.add_argument('-p', '--post', required=False, type=str2bool, default=True, help='Run post process')
    parser.add_argument('-g', '--gen', required=False, type=str2bool, default=True, help='Run gen tree')
    
    args = parser.parse_args()

    run16 = False
    run17 = False
    run18 = False
    
    if args.year == 16 or args.year == 2016: 
        run16 = True
    elif args.year == 17 or args.year == 2017: 
        run17 = True
    elif args.year == 18 or args.year == 2018: 
        run18 = True
    else:
        run16 = True
        run17 = True
        run18 = True

    for year in range(16,19):
        if year == 16 and run16 == False: continue
        if year == 17 and run17 == False: continue
        if year == 18 and run18 == False: continue

        input_dir = 'output/root'+str(year)
        os.system('rm -rf '+input_dir+'/hist_Nosys_Compile.root')

        if args.merge:
            print("Merge histogram root files")
            procs = []
            dir_list = filter(os.path.isdir, glob.glob('output/root'+str(year)+'/*'))
            for item in dir_list:
                proc = mp.Process(target=mergeHist, args=(item,))
                procs.append(proc)
                proc.start()
            for proc in procs:
                proc.join()
            
            for item in os.listdir(input_dir):
                if not item.endswith('.root'):
                    os.system('rm -rf '+item)

        if args.post:
            print("Run post process")
            procs = []
            for item in os.listdir(input_dir):
                if not item.endswith('.root'):
                    continue
                if '__' in item or 'gen' in item:
                    continue
                proc = mp.Process(target=pp.postProcess, args=(os.getcwd(), item, year))
                procs.append(proc)
                proc.start()
            for proc in procs:
                proc.join()

        if args.gen:
            print("Run gentree")
            runGentree(year)

    print("Total running time: %s" %(time.time() - start_time))
