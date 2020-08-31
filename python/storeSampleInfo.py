import os, sys
import ROOT

dictNtuplePath = {
    16:'/data/users/seohyun/ntuple/Run2016/V8_1',
    17:'/data/users/seohyun/ntuple/Run2017/V9_7',
    18:'/data/users/seohyun/ntuple/Run2018/V10_3'
    }

def storeSampleList(save_path):
    for year in range(16, 19):
        path = dictNtuplePath[year]
        f_data = open(save_path+'/data'+str(year)+'.txt','w')
        f_mc   = open(save_path+'/sample'+str(year)+'.txt', 'w')
        f_syst = open(save_path+'/systematic'+str(year)+'.txt','w')

        f_mc.write(path+'\n')

        for item in os.listdir(path+'/split'):
            if 'part' in item: continue
            if 'Data' in item:
                f_data.write(item+'\n')
            elif any(i in item for i in ['Herwig','Evtgen','TT_aMC','SYS']):
                f_syst.write(item+'\n')
            else:
                f_mc.write(item+'\n')

        f_data.close()
        f_mc.close()
        f_syst.close()

def storeNumberOfEvents(save_path):
    for year in range(16, 19): 
        path = dictNtuplePath[year]
        f_out = open(save_path+'/genevt'+str(year)+'.txt','w')
        for item in os.listdir(os.path.join(path,'split')):
            if any(i in item for i in ['Data','part']): continue
            print item
            nevt = 0.0
            for file in os.listdir(os.path.join(path,'split',item)):
                TFile = ROOT.TFile.Open(os.path.join(path,'split',item,file)) 
                EventInfo = TFile.Get('ttbbLepJets/EventInfo')
                nevt += EventInfo.GetBinContent(2)
                TFile.Close()
            f_out.write(item+' '+str(int(nevt))+'\n')
        f_out.close()
