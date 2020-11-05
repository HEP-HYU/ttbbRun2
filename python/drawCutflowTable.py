import os
import sys

import ROOT

strTableTemplate = """
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

class DrawYieldsTable:
    def __init__(self, year, root_path, output_path, hist_name='h_nJets'):
        self.year = year
        self.root_path = root_path
        self.output_path = output_path
        self.hist_name = hist_name

        self.luminosity = {16:35922, 17:41529, 18:59741}
        self.data = {}
        self.samples = {}
        self.xsecs = {}
        self.genevt = {}

        self.get_root_files()
        self.draw_yields()

    def get_root_files(self):
        with open('../samples/xsec'+str(self.year)+'.txt','r') as f:
            while True:
                line = f.readline()
                if 'Xsec' in line: continue
                if not line: break
                tmp = line.split(' ')
                if tmp[2] < 0: continue
                self.xsecs[int(tmp[2])] = [tmp[0], float(tmp[1])]
        
        with open('../samples/genevt'+str(self.year)+'.txt','r') as f:
            while True:
                line = f.readline()
                if not line: break
                tmp = line.split(' ')
                self.genevt[tmp[0]] = float(tmp[1])

        self.data['Muon'] = ROOT.TFile(os.patj.join(self.root_path, 'hist_DataSingleMu.root'))
        self.data['Electron'] = ROOT.TFile(os.patj.join(self.root_path, 'hist_DataSingleMu.root'))
        for sample in self.xsecs.keys():
            self.samples[sample] = ROOT.TFile(os.path.join(self.root_path, 'hist_'+sample+'.root'))

    def draw_yields(self):
        for ich in range(0,3):
            steps = [1,3,4,6,7,8,9]
            
            data_nevts = []
            data_str = '      %s ' % (sample_name)
            for istep in step:
                if ich == 0:
                    TH1 = self.data['Muon'].Get(self.hist_name+'_Ch0_S'+str(istep))
                elif ich == 1:
                    TH1 = self.data['Electron'].Get(self.hist_name+'_Ch1_S'+str(istep))
                else:
                    TH1 = self.data['Muon'].Get(self.hist_name+'_Ch0_S'+str(istep))
                    TH1temp = self.data['Electron'].Get(self.hist_name+'_Ch1_S'+str(istep))
                    TH1.Add(TH1temp)

                error = ROOT.Double()
                nevts = hist_tmp.IntegralAndError(1, TH1.GetNbinsX()+1, error)
                data_str += '& $%.1f $ ' % (nevts)
                data_nevts.append(nevts)
             data_str += '\\\\\n'
           
            mc_nevts = [0.0 for i in range(len(step))]
            mc_error = [0.0 for i in range(len(step))]
            mc_dicts = {}
            for item in self.xsecs.items():
                sample_name = item[1][0]
                xsec = item[1][1]
                sample = self.samples[sample_name]
                scale = (self.luminosity[self.year]*xsec/self.genevt[sample_name])

                temp = '      %s ' % (sample_name)
                for idx, istep in enumerate(step):
                    TH1 = sample.Get(self.hist_name+'_Ch'+str(ich)+'_S'+str(istep))
                    TH1.Scale(scale)
                    
                    #nevts = hist_tmp.Integral(0, hist_tmp.GetNbinsX()+1)
                    error = ROOT.Double()
                    nevts = hist_tmp.IntegralAndError(1, TH1.GetNbinsX()+1, error)
                    
                    temp += '& $%.1f {\scriptstyle\ \pm\ %.1f}$ ' % (nevts, error)
                    mc_nevts[idx] += nevts
                    mc_error[idx] = sqrt(mc_error[idx]^2 + error^2)
                temp += '\\\\\n'
                mc_dict[item[0]] = temp
    
            totalMC = '      TotalMC '
            ratio = '      Ratio '
            for idx, value in enumerate(mc_nevts): 
                ratio = float(data_nevts[idx])/float(value)
                totalMC += '& $%.1f {\scriptstyle\ \pm\ %.1f}$ ' % (value, mc_error[idx])
                ratio += ' & $%.2f$ ' % ratio
            totalMC += '\\\\'
            ratio += '\\\\'
        
            str_dict['year'] = str(year)
            str_dict['nRows'] = '|'+'c|'.join(i for i in range(len(step)))
            str_dict['mcNevts'] = '\n'.join(value for value in mc_dict.values())
            str_dict['dataNevts'] = data_str
            str_dict['totalMC'] = totalMC
            str_dict['ratio'] = ratio

            strOut = strTemplate % str_dict
            with open("../output/yields"+str(year)+'_ch'+str(ich)+".tex", 'w') as f: f.write(strOut)

table = DrawYieldsTable(16, '../output/post16', '../output/')
table = DrawYieldsTable(17, '../output/post17', '../output/')
table = DrawYieldsTable(18, '../output/post18', '../output/')
