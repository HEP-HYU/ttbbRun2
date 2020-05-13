import os
import sys
import glob
import time

import multiprocessing as mp
import subprocess
import argparse

from ROOT import *

import python.postProcess as pp

def str2bool(v):
    if v.lower() in ('yes', 'true', 't', 'y', '1', 'True'):
        return True
    elif v.lower() in ('no', 'false', 'f', 'n', '0', 'False'):
        return False
    else:
        raise argparse.ArgumentTypeError('Boolean value expected')

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
           'macro/runGentree.C("'+str(ntuple_loc)+'", "'+str(os.getcwd())+"/output/root"+str(year)+'/post")']
    subprocess.call(cmd)

if __name__ == '__main__':
    start_time = time.time()

    parser = argparse.ArgumentParser(description='Run post process')

    parser.add_argument('-y', '--year', required=False, type=int, default=9999, help='Run special year')
    parser.add_argument('-m', '--merge', required=False, type=str2bool, default=False, help='Merge histograms')
    parser.add_argument('-p', '--post', required=False, type=str2bool, default=False, help='Run post process')
    parser.add_argument('-g', '--gen', required=False, type=str2bool, default=False, help='Run gen tree')
    
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

        input_dir = str(os.getcwd())+'/output/root'+str(year)
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
                if os.path.isdir(input_dir+'/'+item):
                    os.system('rm -rf '+input_dir+'/'+item)

            today = time.localtime()
            dirName = "%04d%02d%02d" % (today.tm_year, today.tm_mon, today.tm_mday)
            if not os.path.exists(os.getcwd()+'/output/backup/'+str(dirName)):
                os.makedirs(os.getcwd()+'/output/backup/'+str(dirName))
            cmd = ['cp -r', input_dir,  str(os.getcwd())+'/output/backup/'+str(dirName)+'/']
            subprocess.call(cmd)

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

        if args.merge and args.post and args.gen:
            print("Save files")
            output_dir = str(os.getcwd())+'/output/root'+str(year)+'/post'
            cmd = ['hadd', str(output_dir)+'/hist_DataSingleMu.root']+glob.glob(input_dir+'/hist_DataSingleMu*')
            subprocess.call(cmd)
            cmd = ['hadd', str(output_dir)+'/hist_DataSingleEG.root']+glob.glob(input_dir+'/hist_DataSingleEG*')
            subprocess.call(cmd)
            for file in glob.glob(input_dir+'/hist_QCD*'):
                subprocess.call('cp '+file+' '+output_dir)
            cmd = ['cp -r', str(output_dir), str(os.getcwd())+'/output/post'+str(year)]
            subprocess.call(cmd)
            cmd = ['rm -rf', str(input_dir)]
            subprocess.call(cmd)
            subprocess.call('mv '+os.getcwd()+'/output/post'+str(year)+' '+os.getcwd()+'/output/root'+str(year))

    print("Total running time: %s" %(time.time() - start_time))
