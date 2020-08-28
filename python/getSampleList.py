import os
import sys

def getSampleList(save_path):
    dictNtuplePath = {
        16:'/data/users/seohyun/ntuple/Run2016/V8_1',
        17:'/data/users/seohyun/ntuple/Run2017/V9_6',
        18:'/data/users/seohyun/ntuple/Run2018/V10_3'
        }
    for year in range(16,19):
        path = dictNtuplePath[year]
        f_data = open(save_path+'/data'+str(year)+'.txt','w')
        f_mc   = open(save_path+'/sample'+str(year)+'.txt', 'w')
        f_syst = open(save_path+'/systematic'+str(year)+'.txt','w')

        f_mc.write(path+'\n')

        for item in os.listdir(path+'/split'):
            if 'Data' in item:
                f_data.write(item+'\n')
            elif any(i in item for i in ['Herwig','Evtgen','TT_aMC','SYS']):
                f_syst.write(item+'\n')
            else:
                f_mc.write(item+'\n')

        f_data.close()
        f_mc.close()
        f_syst.close()
