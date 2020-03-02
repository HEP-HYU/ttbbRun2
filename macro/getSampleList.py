import os
import sys

def getSampleList(save_path):
    f_list16 = open(save_path+'/sample16.txt', 'w')
    f_data16 = open(save_path+'/data16.txt', 'w')
    f_list17 = open(save_path+'/sample17.txt', 'w')
    f_data17 = open(save_path+'/data17.txt', 'w')
    f_list18 = open(save_path+'/sample18.txt', 'w')
    f_data18 = open(save_path+'/data18.txt', 'w')
 
 
    base_path = '/data/users/seohyun/ntuple/'
    path16 = base_path + 'Run2016/v808/nosplit'
    path17 = base_path + 'Run2017/V9_6/nosplit'
    path18 = base_path + 'Run2018/V10_3/nosplit'

    f_list16.write(path16+'\n')
    f_list17.write(path17+'\n')
    f_list18.write(path18+'\n')
 
    path16 = base_path + 'Run2016/v808/split'
    path17 = base_path + 'Run2017/V9_6/split'
    path18 = base_path + 'Run2018/V10_3/split'

   
    for item in os.listdir(path16):
        if "Data"   in item:
            f_data16.write(item+'\n')
            continue
        if "part"   in item: continue
        if "Herwig" in item: continue
        if "Evtgen" in item: continue
        if "TT_aMC" in item: continue
        if "SYS"    in item: continue
        f_list16.write(item+'\n')
    f_list16.close()
    f_data16.close()

    for item in os.listdir(path17):
        if "Data" in item: 
            f_data17.write(item+'\n')
            continue
        if "part" in item: continue
        if "SYS"  in item: continue
        f_list17.write(item+'\n')
    f_list17.close()

    for item in os.listdir(path18):
        if "Data" in item:
            f_data18.write(item+'\n')
            continue
        if "part" in item: continue
        if "SYS"  in item: continue
        f_list18.write(item+'\n')
    f_list18.close()
