import os
import sys

import ROOT

import tdrstyle

class DrawSystPlots:
    luminosity = {16:35922, 17:41529, 18:59741}
    def __init__(self, year, root_path, hist_name, output_path):
        self.year = year
        self.root_path = root_path
        self.hist_name = hist_name
        self.output_path = output_path
        self.samples = {} 
        self.xsecs = {}
        self.genevt = {}
        
        self.get_root_files()
        self.draw_histograms()

        if os.path.exists(self.output_path):
            os.makedirs(self.output_path)

    def get_root_files(self):
        with open('../samples/xsec'+str(self.year)+'.txt','r') as f:
            while True:
                line = f.readline()
                if 'Xsec' in line: continue
                if not line: break
                tmp = line.split(' ')
                if tmp[2] < 0: continue
                self.xsecs[tmp[0]] = float(tmp[1])
        with open('../samples/genevt'+str(self.year)+'.txt','r') as f:
            while True:
                line = f.readline()
                if not line: break
                tmp = line.split(' ')
                self.genevt[tmp[0]] = float(tmp[1])
       
        for sample in self.xsecs.keys():
            self.samples[sample] = ROOT.TFile(os.path.join(self.root_path, 'hist_'+sample+'.root'))

    def draw_histograms(self):
        ROOT.gStyle.SetOptStat(0)

        for name, file in self.samples.items():
            if not 'ttbb' in name: continue
            temp = [x.GetName() for x in file.GetListOfKeys()]
            break

        syst_name = []
        for item in temp:
            if not self.hist_name in item: continue
            if item[-2:] == 'up':
                name = (item.split('__')[-1]).replace('up','')
                syst_name.append(name)
        
        for syst in syst_name:
            i = 0
            for name, file in self.samples.items():
                if 'QCD' in name: continue
                if i == 0:
                    TH1Nom = file.Get(self.hist_name)
                    TH1Up = file.Get(self.hist_name+'__'+syst+'up')
                    TH1Down = file.Get(self.hist_name+'__'+syst+'down')

                    TH1Nom.Scale(self.xsecs[name]*DrawSystPlots.luminosity[self.year]/self.genevt[name])
                    TH1Up.Scale(self.xsecs[name]*DrawSystPlots.luminosity[self.year]/self.genevt[name])
                    TH1Down.Scale(self.xsecs[name]*DrawSystPlots.luminosity[self.year]/self.genevt[name])
                    i += 1
                else:
                    tmpNom = file.Get(self.hist_name)
                    tmpUp = file.Get(self.hist_name+'__'+syst+'up')
                    tmpDown = file.Get(self.hist_name+'__'+syst+'down')
                    
                    tmpNom.Scale(self.xsecs[name]*DrawSystPlots.luminosity[self.year]/self.genevt[name])
                    tmpUp.Scale(self.xsecs[name]*DrawSystPlots.luminosity[self.year]/self.genevt[name])
                    tmpDown.Scale(self.xsecs[name]*DrawSystPlots.luminosity[self.year]/self.genevt[name])

                    TH1Nom.Add(tmpNom)
                    TH1Up.Add(tmpUp)
                    TH1Down.Add(tmpDown)
 
            label = ROOT.TPaveText(0.14,0.9549955,0.97,1,"brNDC")
            label.SetBorderSize(0)
            label.SetFillStyle(0)
            label.SetTextAlign(13)
            label.SetTextSize(0.05850585)
            label.AddText("  CMS simulation #font[52]{#scale[0.76]{Work in Progress}}")

            canvas = ROOT.TCanvas("","",1025,568,450,450)
            canvas.Range(0,0,1,1)
            canvas.SetFillColor(0)
            canvas.SetBorderMode(0)
            canvas.SetBorderSize(2)
            canvas.SetTickx(1)
            canvas.SetTicky(1)
            canvas.SetLeftMargin(0.14)
            canvas.SetRightMargin(0.03)
            canvas.SetTopMargin(0.06)
            canvas.SetFrameBorderMode(0)

            pad1 = ROOT.TPad("pad1","pad1",0.0,0.33333,1.0,1.0)
            pad1.Range(-67.46988,-190.601,414.4578,9339.448)
            pad1.SetLeftMargin(0.14)
            pad1.SetRightMargin(0.03)
            pad1.SetBottomMargin(0.02)
            pad1.SetFrameFillColor(0)
            pad1.SetFillColor(0)
            pad1.SetBorderMode(0)
            pad1.SetBorderSize(2)
            pad1.SetTickx(1)
            pad1.SetTicky(1)
            pad1.SetFrameBorderMode(0)

            pad2 = ROOT.TPad("pad2","pad2",0.0,0.0,1.0,0.33333)
            pad2.Range(-67.46988,-0.1539271,414.4578,1.692312)
            pad2.SetTopMargin(0.05)
            pad2.SetBottomMargin(0.30003)
            pad2.SetLeftMargin(0.14)
            pad2.SetRightMargin(0.03)
            pad2.SetGridy()
            pad2.SetFrameFillColor(0)
            pad2.SetTickx(1)
            pad2.SetTicky(1)
            pad2.SetFrameBorderMode(0)

            pad1.Draw()
            pad2.Draw()
            
            pad1.cd()
            TH1Nom.GetXaxis().SetLabelFont(43)
            TH1Nom.GetXaxis().SetTitleFont(43)
            TH1Nom.GetYaxis().SetLabelFont(43)
            TH1Nom.GetYaxis().SetLabelSize(14)
            TH1Nom.GetYaxis().SetTitleSize(15)
            TH1Nom.GetYaxis().SetTitleOffset(2)
            TH1Nom.GetYaxis().SetTitleFont(43)
            TH1Nom.GetXaxis().SetLabelSize(0)
            TH1Nom.GetXaxis().SetTitleSize(0)
            TH1Nom.SetMinimum(0)

            TH1Up.SetLineColor(ROOT.kGreen)
            TH1Down.SetLineColor(ROOT.kRed)

            TH1Nom.Draw("hist")
            TH1Up.Draw("hist same")
            TH1Down.Draw("hist same")
            label.Draw("same")

            pad2.cd()
            TH1RatioUp = TH1Nom.Clone()
            TH1RatioUp.Divide(TH1Up)
            TH1RatioDown = TH1Nom.Clone()
            TH1RatioDown.Divide(TH1Down)
            
            TH1RatioUp.SetMaximum(1.6)
            TH1RatioUp.SetMinimum(0.4)
            TH1RatioUp.GetXaxis().SetLabelOffset(0.021)
            TH1RatioUp.GetXaxis().SetLabelSize(14)
            TH1RatioUp.GetXaxis().SetTitleSize(15)
            TH1RatioUp.GetXaxis().SetTitleOffset(3.3)
            TH1RatioUp.GetYaxis().SetTitle("Nom. / Syst.")
            TH1RatioUp.GetYaxis().SetLabelOffset(0.012)
            TH1RatioUp.GetYaxis().SetLabelSize(14)
            TH1RatioUp.GetYaxis().SetTitleSize(15)
            TH1RatioUp.GetYaxis().SetNdivisions(505)
            
            TH1RatioUp.SetLineColor(ROOT.kGreen)
            TH1RatioDown.SetLineColor(ROOT.kRed)
            
            TH1RatioUp.Draw("p")
            TH1RatioDown.Draw("p same")

            canvas.Print(output_path+'/'+syst+".pdf","pdf")
    

hist = DrawSystPlots(16, '../output/root16_post', 'h_keras_RecoDeltaRvsJetCSV_bin0_Ch2_S2', '../output/pdf/16/syst/')
