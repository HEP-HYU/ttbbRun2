import os
import sys

from ROOT import *
import ROOT

luminosities = {16:35922, 17:41529, 18:59693}

for year in range(16,19):
    tmp_tmp = 0.0
    
    f_nevt = open('../output/cutflow/nevt'+str(year)+'.tex', 'w')
    f_config = open('../plotIt/configs/files'+str(year)+'.yml','w')

    f_config.write("'root"+str(year)+"/hist_DataSingleMu.root':\n")
    f_config.write("  type: data\n")
    f_config.write("  legend: 'Data'\n")
    f_config.write("  pretty-name: 'DataMu'\n")
    f_config.write("  marker-size: 0.6\n")
    f_config.write("  group: GData\n")
    f_config.write("\n")

    f_config.write("'root"+str(year)+"/hist_DataSingleEG.root':\n")
    f_config.write("  type: data\n")
    f_config.write("  legend: 'Data'\n")
    f_config.write("  pretty-name: 'DataEG'\n")
    f_config.write("  marker-size: 0.6\n")
    f_config.write("  group: GData\n")
    f_config.write("\n")

    # MC samples
    genevt = {}
    with open('../samples/genevt'+str(year)+'.txt','r') as f:
        while True:
            line = f.readline()
            if not line: break
            tmp = line.split(' ')
            genevt[tmp[0]] = float(tmp[1])

    sf = {}
    with open('../samples/scale'+str(year)+'.txt','r') as f:
        while True:
	    line = f.readline()
	    if not line: break
	    tmp = line.split(' ')
	    sf[tmp[0]] = float(tmp[1])

    hist_name = 'h_nJets'
    with open('../samples/xsec'+str(year)+'.txt','r') as f:
        f_nevt.write('\\begin{table}\n')
	f_nevt.write('    \\caption{'+str(year)+'cutflow table}\n')
	f_nevt.write('    \\begin{tabular}{|c|c|c|c|c|c|c|c|c|c|}\n')
	f_nevt.write('        \\hlline\\hline\n')
	f_nevt.write('            Sample & Ch0S0 & Ch0S1 & Ch0S2 & Ch0S3 & Ch1S0 & Ch1S0 & Ch1S1 &  Ch1S2 & Ch1S3\n')
	f_nevt.write('            \\hline\n')
        n = 1
	while True:
            line = f.readline()
            if not line: break
            if n != 1:
                tmp = line.split(' ')
		sample = tmp[0]
		xsec = float(tmp[1])
		order = int(tmp[2])
		color = tmp[3]
		legend = tmp[4][:-1]
		if order < 0: continue

                # group
		if "ff6666" in color: group = 'GttBkg'
		elif "#ff66ff" in color: group = 'GttX'
		elif "#990099" in color: group = 'GSingleT'
		elif "#00cccc" in color: group = 'GVV'
		elif "#000099" in color: group = 'GZJets'
		elif "#d0cfd4" in color: group = 'GQCD'
		else: group = ''

		f_config.write("'root"+str(year)+"/hist_"+sample+".root':\n")
		f_config.write("  type: mc\n")
		f_config.write("  pretty-name: '"+sample+"'\n")
		f_config.write("  cross-section: "+str(xsec)+"\n")
		f_config.write("  generated-events: "+str(genevt[sample])+"\n")
		f_config.write("  fill-color: '"+str(color)+"'\n")
		if not group == '': f_config.write("  group: "+group+"\n")
		else: f_config.write("  legend: '"+legend+"'\n")
		f_config.write("  order: "+str(order)+"\n")
		#f_config.write("  scale: "+str(sf[sample])+"\n")
		f_config.write("\n")

		f_sample = ROOT.TFile('../output/root'+str(year)+'/hist_'+sample+'.root')
		scale = (luminosities[year]*xsec/genevt[sample])
		print(sample+": "+str(scale))
		nevt = sample + " "
		for ich in range(0,2):
                    for istep in range(0,4):
                        hist_tmp = f_sample.Get(hist_name+'_Ch'+str(ich)+'_S'+str(istep))
                        hist_tmp.Scale(scale)
                        tmp_evt = hist_tmp.Integral(0, hist_tmp.GetNbinsX()+1)
                        tmp_stat = hist_tmp.IntegralAndError(0, hist_tmp.GetNbinsX()+1, ROOT.Double(tmp_tmp));
                        nevt += '& $%.2f {\scriptstyle\ \pm\ %.2f}$ ' % (tmp_evt, tmp_tmp)
		nevt += '\\ \n'
		f_nevt.write(nevt)
            n += 1

    # Data
    f_muon = ROOT.TFile('../output/root'+str(year)+'/hist_DataSingleMu.root')
    f_elec = ROOT.TFile('../output/root'+str(year)+'/hist_DataSingleEG.root')
    nevt = 'Data'
    for ich in range(0,2):
        for istep in range(0,4):
            hist_muon = f_muon.Get(hist_name+'_Ch'+str(ich)+'_S'+str(istep))
            hist_elec = f_elec.Get(hist_name+'_Ch'+str(ich)+'_S'+str(istep))
            if ich == 0:
                tmp_evt = hist_muon.Integral()+hist_muon.GetBinContent(hist_muon.GetNbinsX()+1)
                tmp_stat = hist_muon.IntegralAndError(0, hist_muon.GetNbinsX()+1, ROOT.Double(tmp_tmp))
            else: 
                tmp_evt = hist_elec.Integral()+hist_elec.GetBinContent(hist_elec.GetNbinsX()+1)
                tmp_stat = hist_elec.IntegralAndError(0, hist_elec.GetNbinsX()+1, ROOT.Double(tmp_tmp))

            nevt += '& $%.2f {\scriptstyle\ \pm\ %.2f}$ ' % (tmp_evt, tmp_tmp)
    nevt += '\n'
    f_nevt.write(nevt)
    f_nevt.write('        \\hline\n')
    f_nevt.write('        \\hline\\hline\n')
    f_nevt.write('    \\end{tabular}\n')
    f_nevt.write('\\end{table}\n')

    f_nevt.close()
    f_config.close()

scale16 = float(35922)/float(137144)
scale17 = float(41529)/float(137144)
scale18 = float(59693)/float(137144)
os.system('cp ../plotIt/configs/files16.yml ../plotIt/configs/files16_copy.yml\n')
os.system('cp ../plotIt/configs/files17.yml ../plotIt/configs/files17_copy.yml\n')
os.system('cp ../plotIt/configs/files18.yml ../plotIt/configs/files18_copy.yml\n')
os.system('sed -i "s/scale: 1/scale: '+str(scale16)+'/g" ../plotIt/configs/files16_copy.yml\n')
os.system('sed -i "s/scale: 1/scale: '+str(scale17)+'/g" ../plotIt/configs/files17_copy.yml\n')
os.system('sed -i "s/scale: 1/scale: '+str(scale18)+'/g" ../plotIt/configs/files18_copy.yml\n')
os.system('cat ../plotIt/configs/files16_copy.yml ../plotIt/configs/files17_copy.yml ../plotIt/configs/files18_copy.yml > ../plotIt/configs/files.yml\n')
