import os
import sys

def getSampleList():
    base_path = '/data/users/seohyun/ntuple/'

    f_list16 = open('../samples/sample16.txt', 'w')
    path16 = base_path + 'Run2016/v808/nosplit'
    f_list16.write(path16+'\n')

    for item in os.listdir(path16):
	if "Data"  in item: continue
	if "part"  in item: continue
	if "ISR"   in item: continue
	if "FSR"   in item: continue
	if "Tune"  in item: continue
	if "hdamp" in item: continue
	if "Herwig" in item: continue
	if "Evtgen" in item: continue
	if "TT_aMC" in item: continue

	f_list16.write(item[:-5]+'\n')
    f_list16.close()

    f_list17 = open('../samples/sample17.txt', 'w')
    path17 = base_path + 'Run2017/V9_6/nosplit'
    f_list17.write(path17+'\n')

    for item in os.listdir(path17):
	if "Data" in item: continue
	if "part" in item: continue

	f_list17.write(item[:-5]+'\n')
    f_list17.close()

    f_list18 = open('../samples/sample18.txt', 'w')
    path18 = base_path + 'Run2018/V10_2/nosplit'
    f_list18.write(path18+'\n')

    for item in os.listdir(path18):
	if "Data" in item: continue
	if "part" in item: continue

	f_list18.write(item[:-5]+'\n')
    f_list18.close()
