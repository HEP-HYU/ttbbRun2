from ROOT import *
import ROOT
import os
import sys

def rescale(f_sys, h_eventinfo):
    hist_list = []
    hist_list = [x.GetName() for x in f_sys.GetListOfKeys()]
    h_eventinfo_sys = f_sys.Get("EventInfo")

    bratio = 1.0
    if "TT_" in f_sys.GetName() and not "Bkg" in f_sys.GetName(): bratio = 356.4/831.76

    h_out =[]
    for hist in hist_list:
        if hist == "EventInfo" or "Weights" in hist: continue
	h_tmp = f_sys.Get(hist)
	h_tmp.Scale(h_eventinfo.GetBinContent(2)/(h_eventinfo_sys.GetBinContent(2)*bratio))
        h_out.append(h_tmp)

    return h_out

def write_envelope(sys_name, f_name, h_central, h_sys_list, h_eventinfo, h_weights):
    bratio = 1.0
    if "TT_" in f_name and not "Bkg" in f_name: bratio = 356.4/831.76

    #Find maximum, minimum bin errors
    h_sys_weighted_list = []
    for index, hist in enumerate(h_sys_list):
        hist.Scale(h_eventinfo.GetBinContent(2)/(h_weights.GetBinContent(index+1)*bratio))
	h_sys_weighted_list.append(hist)

    nbins = h_central.GetNbinsX()
    h_up = h_central.Clone()
    h_down = h_central.Clone()

    for ibin in range(nbins+1):
	minimum = float("inf")
	maximum = float("-inf")

	for hist in h_sys_weighted_list:
            minimum = min(minimum, hist.GetBinContent(ibin))
            maximum = max(maximum, hist.GetBinContent(ibin))

        h_up.SetBinContent(ibin, maximum)
        h_down.SetBinContent(ibin, minimum)

    upname = "__"+sys_name+"up"
    downname= "__"+sys_name+"down"
    
    h_up.SetName(h_central.GetName()+upname)
    h_down.SetName(h_central.GetName()+downname)

    return [h_up, h_down]

def runPostProcess(base_path, sample_list, year):
    input_path = base_path+'/output/root'+str(year)+'/'
    print("Year: "+str(year))
    
    print("Rescaling...")
    
    sys_rescale = ['tune', 'hdamp']
    
    for proc in sample_list:
        if not 'TT' in proc: continue
        print("Process: "+proc)
	
	f_central = TFile.Open(os.path.join(input_path, "hist_"+proc+".root"),"UPDATE")
	h_eventinfo = f_central.Get("EventInfo")
	
	for sys in sys_rescale:
	    for i in range(2):
	        if year == 16 and 'Bkg' in proc and 'hdamp' in sys: continue
	        tmp = ""
	        if i == 0: tmp = sys+'up'
		else: tmp = sys+'down'
		print("Sys. sample: "+tmp)
		if year == 16 and 'tune' in sys and not 'Bkg' in proc: tmp2 = 'TT'+proc[4:]
		else: tmp2 = proc

                f_in = TFile.Open(os.path.join(input_path, "hist_"+tmp2+"__"+tmp+".root"))

                h_out = []
                h_out = rescale(f_in, h_eventinfo)

		f_central.cd()
                for hist in h_out:
		   if 'EventInfo' in hist.GetName() or 'Weights' in hist.GetName(): continue
		   hist.Write()
        f_central.Close()

    print("Writing envelope...")

    for proc in sample_list:
        if not 'TT' in proc: continue
	print("Process: "+proc)
	
	f_central = TFile.Open(os.path.join(input_path, "hist_"+proc+".root"),"UPDATE")
	filename = f_central.GetName()

	hist_list = []
	for x in f_central.GetListOfKeys():
	    if "__" in x.GetName(): continue
	    hist_list.append(x.GetName())

	h_eventinfo = f_central.Get("EventInfo")
	h_scaleweights = f_central.Get("ScaleWeights")
	h_pdfweights = f_central.Get("PDFWeights")

        if year == 16:
	    if 'Filter' in proc: continue
	    ps_list = ["isrup", "fsrup", "isrdown", "fsrdown"]
	    if not 'Bkg' in proc: tmp = "TT"+proc[4:]
	    else: tmp = proc
	    f_ps_list = []
	    for ps in ps_list:
		f_ps_list.append(TFile.Open(os.path.join(input_path, "hist_"+tmp+"__"+ps+".root")))
		
	    h_psweights = ROOT.TH1D("PSweights","",4,0,4)
	    for index, value in enumerate(ps_list):
		h_psweights.GetXaxis().SetBinLabel(index+1,value)
		tmp = (f_ps_list[index].Get("EventInfo")).GetBinContent(2)
		h_psweights.SetBinContent(index+1,tmp)

	else:
	    ntuple_loc = "/data/users/seohyun/ntuple/Run20"+str(year)
	    if year == 17: ntuple_loc += "/V9_6/nosplit/"
	    if year == 18: ntuple_loc += "/V10_2/nosplit/"
	    f_ntuple = TFile.Open(os.path.join(ntuple_loc, proc+".root"))
	    h_psweights = f_ntuple.Get("ttbbLepJets/PSWeights")
	    #h_psweights = f_central.Get("PSWeights")

        print("Nhist: "+str(len(hist_list)))
	for hist in hist_list:
	    if hist == "EventInfo" or "Weights" in hist: continue
	    print(hist)
	    h_central = f_central.Get(hist)
	    h_sw_list = []
	    for i in range(6):
	        h_sw_list.append(f_central.Get(hist+"__scale"+str(i)))
	    h_pdf_list = []
	    if year == 16: maxpdf = 102
	    else: maxpdf = 103
	    for i in range(maxpdf):
	        h_pdf_list.append(f_central.Get(hist+"__pdf"+str(i)))
	    h_ps_list = []
	    ps_list = ["isrup", "fsrup", "isrdown", "fsrdown"]
	    if year == 16:
	        for index, f_ps in enumerate(f_ps_list):
	            h_ps_list.append(f_ps.Get(hist+"__"+ps_list[index]))
	    else:
	        for ps in ps_list:
		    h_ps_list.append(f_central.Get(hist+"__"+ps))
	   
	    h_sw_new = []
	    h_sw_new = write_envelope("sw", filename, h_central, h_sw_list, h_eventinfo, h_scaleweights)
	    f_central.cd()
	    h_sw_new[0].Write()
	    h_sw_new[1].Write()

	    h_pdf_new = []
	    h_pdf_new = write_envelope("pdf", filename, h_central, h_pdf_list, h_eventinfo, h_pdfweights)
	    f_central.cd()
	    h_pdf_new[0].Write()
	    h_pdf_new[1].Write()    
	   
	    h_ps_new = []
	    h_ps_new = write_envelope("ps", filename, h_central, h_ps_list, h_eventinfo, h_psweights)
	    f_central.cd()
	    h_ps_new[0].Write()
	    h_ps_new[1].Write()
        
	f_central.Close()
     
    # Add JER, JEC
    for proc in sample_list:
        if 'QCD' in proc: continue
        print("Process: "+proc)
	f_central = TFile.Open(os.path.join(input_path, "hist_"+proc+".root"),"UPDATE")
	jets = ["jerup", "jerdown", "jecup", "jecdown"]
	f_list = []
	for jet in jets:
	    f_list.append(TFile.Open(os.path.join(input_path, "hist_"+proc+"__"+jet+".root")))

	hist_list = []
	hist_list = [x.GetName() for x in f_central.GetListOfKeys()]

	f_central.cd()
	for hist in hist_list:
	    if "__" in hist: continue
	    if "Info" in hist or "Weights" in hist: continue
	    for index, value in enumerate(f_list):
	        tmp = value.Get(hist+"__"+jets[index])
		tmp.Write()

        f_central.Close()

    # For Response matrix, Add external ttbb sample`s histograms
    f_matrix = TFile.Open(os.path.join(input_path, "hist_ResponseMatrix_ttbb.root"), "UPDATE")
    f_ttbb = TFile.Open(os.path.join(input_path, "hist_TTLJ_PowhegPythia_ttbb.root"))
    sys_list = ["sw", "ps", "hdamp", "tune", "pdf"]
    hist_list = [x.GetName() for x in f_matrix.GetListOfKeys()]
    f_matrix.cd()
    for hist in hist_list:
        if "__" in hist or "Info" in hist or "Weights" in hist: continue
	for item in sys_list:
	    tmpup = f_ttbb.Get(hist+"__"+item+"up")
	    tmpdown = f_ttbb.Get(hist+"__"+item+"down")
	    tmpup.Write()
	    tmpdown.Write()
    f_matrix.Close()
    f_ttbb.Close()

    print("All post processes are completed")
