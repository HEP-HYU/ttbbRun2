from ROOT import *
import tdrstyle

def buildGraphFromHist(h_in):
    grp = TGraphErrors()

    for ibin in range(1,h_in.GetNbinsX()+1):
        w = h_in.GetXaxis().GetBinWidth(ibin)
        y = h_in.GetBinContent(ibin)
        err = h_in.GetBinError(ibin)
        if y < 0.0:
            y = 0.0
        grp.SetPoint(ibin-1, h_in.GetBinCenter(ibin), y)
        grp.SetPointError(ibin-1, 0.0, err)

    grp.SetEditable(False)

    return grp

def drawHist(outputDir, lumi, inFile, recoMode='mindR', genMode='mindR'):
    tdrstyle.setTDRStyle()

    gStyle.SetHatchesSpacing(1)
    gStyle.SetHatchesLineWidth(1)

    gStyle.SetPaintTextFormat("4.1f")
    gStyle.SetOptStat(0)
    gStyle.SetOptTitle(0)

    f_in = TFile.Open(outputDir+'/'+inFile)
    
    hist_list = [x.GetName() for x in f_in.GetListOfKeys()]
    
    canvas = TCanvas('','',800,800)
    color = TColor()
    legend = TLegend(0.6,0.7,0.88,0.88)
    labelCMS = tdrstyle.tdrCMSlabel(float(lumi)) 
    labelSim = tdrstyle.tdrCMSSimlabel()

    for hist in hist_list:
        h_in = f_in.Get(hist)

        canvas.cd()
        if 'Bin' in hist:
            h_in.Scale(100)
            h_err = h_in.Clone()
            if 'Acceptance' in h_in.GetName():
                h_in.GetYaxis().SetTitle('Acceptance(%)')
                h_in.SetMinimum(0.0)
                h_in.SetMaximum(h_in.GetMaximum()*1.3)
            elif 'Purity' in h_in.GetName():
                h_in.GetYaxis().SetTitle('Purity(%)')
                h_in.SetMinimum(0.0)
                h_in.SetMaximum(100.0)
            elif 'Stability' in h_in.GetName():
                h_in.GetYaxis().SetTitle('Stability(%)')
                h_in.SetMinimum(0.0)
                h_in.SetMaximum(100.0)

            h_in.GetYaxis().SetTitleOffset(1.3)
            h_in.GetXaxis().SetTitleOffset(1.2)
            h_in.SetLineWidth(2)
            h_in.SetLineColor(kBlue)
            h_in.Draw('hist')
            if 'Acceptance' in h_in.GetName():
                h_err.SetFillStyle(3154)
                h_err.SetFillColor(color.GetColor('#ee556270'))
                h_err.Draw('e2 same')
            labelSim.Draw('same')
        elif h_in.InheritsFrom(TH2.Class()):
            h_in.Draw('box')
            labelSim.Draw('same')
        elif '__' in h_in.GetName() and h_in.InheritsFrom(TH1.Class()):
            h_in.Draw('hist')
            labelSim.Draw('hist same')
        else:
            if 'Gen' in h_in.GetName():
                continue

            grp = buildGraphFromHist(h_in)
            grp.SetMarkerSize(0.8)
            genName = h_in.GetName()
            genName.replace('Reco', 'Gen')

            h_gen = f_in.Get(genName)
            h_gen.SetLineColor(color.GetColor('#cc0000'))
            h_gen.SetLineWidth(2)

            h_genErr = h_gen.Clone()
            h_genErr.SetFillColor(color.GetColor('#556270'))
            h_genErr.SetFillStyle(3154)
            
            if 'diffXsec' in h_in.GetName():
                if 'DeltaR' in h_in.GetName():
                    h_gen.GetYaxis().SetTitle('#frac{d#sigma}{d#DeltaR_{b#bar{b}}}[pb]')
                else:
                    h_gen.GetYaxis().SetTitle('#frac{d#sigma}{dm_{b#bar{b}}}[pb/GeV]')
                h_gen.GetYaxis().SetTitleOffset(2.0)
            else:
                h_gen.GetYaxis().SetTitle('Number of events')
            h_gen.SetMaximum(h_in.GetMaximum()*1.8)

            if 'prefit' in h_in.GetName():
                legend.AddEntry(grp, 'ttbb Reco MC', 'p')
                label = labelSim
            else:
                legend.AddEntry(grp, 'Data', 'p')
                label = labelCMS

            legend.AddEntry(h_gen, 'ttbb Powheg Pythia', 'l')

            h_gen.Draw('hist')
            grp.Draw('p e same')
            legend.Draw('same')
            label.Draw('same')
        
        canvas.Print(outputDir+'/'+h_in.GetName()+'.pdf','pdf')
        canvas.Clear()
        legend.Clear()
