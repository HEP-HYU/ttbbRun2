import os
import sys

from ROOT import *
import ROOT

def drawYields(year, inDir, lumi):
    tmp_tmp = 0.0
    strTemplate = """
\\begin{table}[htb]
  \\rotatebox{90}{
  \\footnotesize
  \\begin{center}
	\\caption{%(year)s yields table}
	\\begin{tabular}{%(nRows)s}
      \\hline\\hline
      Sample & Ch0S0 & Ch0S1 & Ch0S2 & Ch1S0 & Ch1S1 & Ch1S2 & Ch2S0 & Ch2S1 & Ch2S2 \\\\
      \\hline
%(mcNevts)s
      \\hline
%(dataNevts)s
%(totalMC)s
%(ratio)s
      \\hline\\hline
    \\end{tabular}
  \\end{center}
  }
\\end{table}
    """
    # MC samples
    genevt = {}
    with open('../samples/genevt'+str(year)+'.txt','r') as f:
        while True:
            line = f.readline()
            if not line: break
            tmp = line.split(' ')
            genevt[tmp[0]] = float(tmp[1])

    hist_name = 'h_nJets'
    dicNevts = {}
    dicErrors = {}
    dicOrder = {}
    nData  = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0] 
    nDataErr  = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0] 

    with open('../samples/xsec'+str(year)+'.txt','r') as f:
        while True:
            line = f.readline()
            if not line: break
            if 'Sample Xsec' in line: continue
            tmp = line.split(' ')
            sample = tmp[0]
            xsec = float(tmp[1])
            order = int(tmp[2])
            color = tmp[3]
            group = tmp[4][:-1]
            #if "QCD" in group: continue
            if order < 0: continue
            if not group in dicNevts.keys():
                dicNevts[group]  = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
                dicErrors[group] = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
                dicOrder[order] = group
            
            f_sample = ROOT.TFile(inDir+'/hist_'+sample+'.root')
            scale = (lumi*xsec/genevt[sample])
            print "Sample: "+sample+" Scale: "+str(scale)
            tmp = 0
            for ich in range(0,3):
                for istep in range(0,4):
                    if istep == 2: continue
                    hist_tmp = f_sample.Get(hist_name+'_Ch'+str(ich)+'_S'+str(istep))
                    hist_tmp.Scale(scale)
                    #tmp_evt = hist_tmp.Integral(0, hist_tmp.GetNbinsX()+1)
                    tmp_err = ROOT.Double()
                    tmp_evt = hist_tmp.IntegralAndError(1, hist_tmp.GetNbinsX()+1, tmp_err);
                    if tmp_evt < 0.0: tmp_evt = 0.0
                    dicNevts[group][tmp] += tmp_evt
                    dicErrors[group][tmp] += tmp_err
                    tmp += 1
             
    f_muon = ROOT.TFile(inDir+'/hist_DataSingleMu.root')
    f_elec = ROOT.TFile(inDir+'/hist_DataSingleEG.root')
    tmp = 0
    for ich in range(0,3):
        for istep in range(0,4):
            tmp_evt = 0.0
            tmp_err = ROOT.Double() 
            if istep == 2: continue
            hist_muon = f_muon.Get(hist_name+'_Ch'+str(ich)+'_S'+str(istep))
            hist_elec = f_elec.Get(hist_name+'_Ch'+str(ich)+'_S'+str(istep))
            if ich == 0:
                tmp_evt = hist_muon.IntegralAndError(1, hist_muon.GetNbinsX()+1, tmp_err)
            elif ich == 1:
                tmp_evt = hist_elec.IntegralAndError(1, hist_elec.GetNbinsX()+1, tmp_err)
            else: 
                tmp_evt = hist_muon.IntegralAndError(1, hist_muon.GetNbinsX()+1, tmp_err)
                tmp_err2 = ROOT.Double()
                tmp_evt += hist_elec.IntegralAndError(1, hist_elec.GetNbinsX()+1, tmp_err2)
                tmp_err += tmp_err2
            
            nData[tmp] += tmp_evt
            nDataErr[tmp] += tmp_err
            tmp += 1

    dictIn = {'year':str(year),'nRows':'|c|c|c|c|c|c|c|c|c|c|','mcNevts':'', 'dataNevts':'','totalMC':'','ratio':''}

#%(mcNevts)s
#%(dataNevts)s
#%(totalMC)s
#%(ratio)s
    totalMC = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
    totalErr = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]

    for key, value in dicOrder.items():
        dictIn['mcNevts'] += '      %s ' % (value)
        for i in range(9):
            totalMC[i] += dicNevts[value][i]
            totalErr[i] += dicErrors[value][i]
            dictIn['mcNevts'] += '& $%.1f {\scriptstyle\ \pm\ %.1f}$ ' % (dicNevts[value][i], dicErrors[value][i])
        dictIn['mcNevts'] += '\\\\\n'

    dictIn['dataNevts'] += '      Data '
    dictIn['totalMC'] += '      TotalMC '
    dictIn['ratio'] += '      Ratio '
    for i in range(9):
        ratio = nData[i]/totalMC[i]
        dictIn['dataNevts'] += '& $%.0f {\scriptstyle\ \pm\ %.1f}$ ' % (nData[i], nDataErr[i])
        dictIn['totalMC'] += '& $%.1f {\scriptstyle\ \pm\ %.1f}$ ' % (totalMC[i], totalErr[i])
        dictIn['ratio'] += '& $%.2f$ ' % (ratio)
    dictIn['dataNevts'] += '\\\\'
    dictIn['totalMC'] += '\\\\'
    dictIn['ratio'] += '\\\\'
    
    #nevt += '& $%.2f {\scriptstyle\ \pm\ %.2f}$ ' % (tmp_evt, tmp_stat)
    strOut = strTemplate % dictIn
    with open("../output/yields"+str(year)+".tex", 'w') as f: f.write(strOut)

"""
for year in range(16,19):
    input_loc = '../output/root'+str(year)+'_post/'

    f_nevt = open('../output/cutflow/nevt'+str(year)+'.tex', 'w')
    f_config = open('../plotIt/configs/files/files'+str(year)+'.yml','w')

    f_config.write(input_loc+"/hist_DataSingleMu.root':\n")
    f_config.write("  type: data\n")
    f_config.write("  legend: 'Data'\n")
    f_config.write("  pretty-name: 'DataMu'\n")
    f_config.write("  marker-size: 0.6\n")
    f_config.write("  group: GData\n")
    f_config.write("\n")

    f_config.write(input_loc+"/hist_DataSingleEG.root':\n")
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

		f_config.write(input_loc+"/hist_"+sample+".root':\n")
		f_config.write("  type: mc\n")
		f_config.write("  pretty-name: '"+sample+"'\n")
		f_config.write("  cross-section: "+str(xsec)+"\n")
		f_config.write("  generated-events: "+str(genevt[sample])+"\n")
		f_config.write("  fill-color: '"+str(color)+"'\n")
		if not group == '': f_config.write("  group: "+group+"\n")
		else: f_config.write("  legend: '"+legend+"'\n")
		f_config.write("  order: "+str(order)+"\n")
		f_config.write("\n")

		f_sample = ROOT.TFile(input_loc+'/hist_'+sample+'.root')
		scale = (luminosities[year]*xsec/genevt[sample])
		print(sample+": "+str(scale))
		nevt = sample + " "
		for ich in range(0,2):
                    for istep in range(0,4):
                        hist_tmp = f_sample.Get(hist_name+'_Ch'+str(ich)+'_S'+str(istep))
                        hist_tmp.Scale(scale)
                        tmp_evt = hist_tmp.Integral(0, hist_tmp.GetNbinsX()+1)
                        tmp_stat = hist_tmp.IntegralAndError(0, hist_tmp.GetNbinsX()+1, ROOT.Double(tmp_tmp));
                        nevt += '& $%.2f {\scriptstyle\ \pm\ %.2f}$ ' % (tmp_evt, tmp_stat)
		nevt += '\\ \n'
		f_nevt.write(nevt)
            n += 1

    # Data
            nevt += '& $%.2f {\scriptstyle\ \pm\ %.2f}$ ' % (tmp_evt, tmp_stat)
    nevt += '\n'
    f_nevt.write(nevt)
    f_nevt.write('        \\hline\n')
    f_nevt.write('        \\hline\\hline\n')
    f_nevt.write('    \\end{tabular}\n')
    f_nevt.write('\\end{table}\n')

    f_nevt.close()
    f_config.close()
"""
drawYields(16, '../output/root16_post', 35922)
drawYields(17, '../output/root17_post', 41529)
drawYields(18, '../output/root18_post', 59693)
