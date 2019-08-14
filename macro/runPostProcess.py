from ROOT import *
import ROOT
import os
import sys

def rescale(f_sys, h_eventinfo):
    hist_list = []
    hist_list = [x.GetName() for x in f_sys.GetListOfKeys()]
    h_eventinfo_sys = f_sys.Get("EventInfo")

    bratio = 1.0
    if "ttbar" in f_sys.GetName(): bratio = 356.4/831.76

    h_out =[]
    for hist in hist_list:
        if hist == "EventInfo" or hist == "ScaleWeights": continue
	h_tmp = f_sys.Get(hist)
	h_tmp.Scale(h_eventinfo.GetBinContent(2)/(h_eventinfo_sys.GetBinContent(2)*bratio))
        h_out.append(h_tmp)

    return h_out

def write_envelope(h_central, h_sys_list, h_eventinfo, h_weights):

    #Find maximum, minimum bin errors
    h_sys_weighted_list = []
    for index, hist in enumerate(h_sys_list):
        hist.Scale(h_eventinfo.GetBinContent(2)/h_weights.GetBinContent(index+1))
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

    if "PS" in h_weights.GetName():
	upname = "__psup"
	downname= "__psdown"
    else:
        upname = "__swup"
        downname = "__swdown"
    h_up.SetName(h_central.GetName()+upname)
    h_down.SetName(h_central.GetName()+downname)

    return [h_up, h_down]

def runPostProcess(base_path, sample_list, year):
    input_path = base_path+'/output/root'+str(year)+'/'
    print("Year: "+str(year))
    print("Rescaling...")
    sys_rescale = []
    if year == 16:
        sys_rescale = ['isr', 'fsr', 'hdamp', 'tune']
    else:
        sys_rescale = ['tune', 'hdamp']

    for proc in sample_list:
        print("Process: "+proc)
        if not ('TTLL' in proc or 'TTLJ' in proc or 'TTJJ' in proc or 'ttbar' in proc): continue
	f_central = TFile.Open(os.path.join(input_path, "hist_"+proc+".root"))
	h_eventinfo = f_central.Get("EventInfo")
	for sys in sys_rescale:
            if 'ttbar' in proc and 'hdamp' in sys: continue
	    for i in range(2):
	        if i == 1: sys = sys+'up'
		else: sys = sys+'down'

                f_in = TFile.Open(os.path.join(input_path, "hist_"+proc+"__"+sys+".root"))
                f_out = TFile.Open(os.path.join(input_path, "hist_"+proc+"__"+sys+"_re.root"), "recreate")

                h_out = []
                h_out = rescale(f_in, h_eventinfo)
                f_out.cd()
            
	        evt = f_in.Get("EventInfo")
                scw = f_in.Get("ScaleWeights")
                evt.Write()
                scw.Write()

                for hist in h_out:
		   if 'EventInfo' in hist.GetName() or 'ScaleWeights' in hist.GetName(): continue
		   hist.Write()
                f_out.Write()
                f_out.Close()

                os.system('rm -rf '+input_path+'hist_'+proc+'__'+sys+'.root')
                os.system('mv '+input_path+'hist_'+proc+'__'+sys+'_re.root '+input_path+'hist_'+proc+'__'+sys+'.root')

    print("Writing envelope...")
    ps_list = ['isr','fsr']

    os.system("rm -rf "+input_path+"/*__ps*")
    os.system("rm -rf "+input_path+"/*__sw*")

    for proc in sample_list:
        if not ('TTLL' in proc or 'TTLJ' in proc or 'TTJJ' in proc or 'ttbar' in proc): continue
	print("Process: "+proc)
	f_central = TFile.Open(os.path.join(input_path, "hist_"+proc+".root"))

	f_ps_list = []
	for ps in ps_list:
	    f_ps_list.append(TFile.Open(os.path.join(input_path, "hist_"+proc+"__"+ps+".root")))
	    
	f_ps_up = TFile.Open(os.path.join(input_path, "hist_"+proc+"__psup.root"),"RECREATE")
	f_ps_down = TFile.Open(os.path.join(input_path, "hist_"+proc+"__psdown.root"),"RECREATE")

	h_psweights = ROOT.TH1D("PSweights","",4,0,4)
	for index, value in enumerate(ps_list):
            h_psweights.GetXaxis().SetBinLabel(index+1,value)
	    tmp = (f_ps_list[index].Get("EventInfo")).GetBinContent(2)
	    h_psweights.SetBinContent(index+1,tmp)

	ps_sum = h_psweights.Integral()

        f_sw_list = []
	for i in range(6):
	    sw = 'scale'+str(i)
	    f_sw_list.append(TFile.Open(os.path.join(input_path, "hist_"+proc+"__"+sw+".root")))

	f_sw_up = TFile.Open(os.path.join(input_path, "hist_"+proc+"__swup.root"),"RECREATE")
	f_sw_down = TFile.Open(os.path.join(input_path, "hist_"+proc+"__swdown.root"),"RECREATE")

	hist_list = []
	hist_list = [x.GetName() for x in f_central.GetListOfKeys()]

	h_eventinfo = f_central.Get("EventInfo")
	h_scaleweights = f_central.Get("ScaleWeights")
	sw_sum = h_scaleweights.Integral()

	do_sw_process = False
	do_ps_process = False
	if sw_sum > 0: do_sw_process = True
	if ps_sum > 0: do_ps_process = True

	for hist in hist_list:
	    if hist == "EventInfo" or hist == "ScaleWeights": continue
	    h_central = f_central.Get(hist)
	    h_sw_list = []
	    for index, f_sw in enumerate(f_sw_list):
		h_sw_list.append(f_sw.Get(hist+"__"+sw_list[index]))
	    h_ps_list = []
	    for index, f_ps in enumerate(f_ps_list):
	        h_ps_list.append(f_ps.Get(hist+"__"+ps_list[index]))

	    h_sw_new = []
	    h_ps_new = []
	    if do_sw_process:
		h_sw_new = write_envelope(h_central, h_sw_list, h_eventinfo, h_scaleweights)
		f_sw_up.cd()
		h_sw_new[0].Write()
		f_sw_up.Write()
		f_sw_down.cd()
		h_sw_new[1].Write()
		f_sw_down.Write()

	    if do_ps_process:
		h_ps_new = write_envelope(h_central, h_ps_list, h_eventinfo, h_psweights)
		f_ps_up.cd()
		h_ps_new[0].Write()
		f_ps_up.Write()
		f_ps_down.cd()
		h_ps_new[1].Write()
		f_ps_down.Write()

	f_sw_up.Close()
	f_sw_down.Close()
	f_ps_up.Close()
	f_ps_down.Close()

    print("Merging histogram files")
    output_path = base_path+'/output/post'+str(year)+'/'
    if not os.path.exists(output_path): os.makedirs(output_path)
    os.system('cp '+input_path+'hist_DataSingleMu.root '+output_path)
    os.system('cp '+input_path+'hist_DataSingleEl.root '+output_path)
    for proc in sample_list:
	print("Process: "+proc)

	os.system('hadd '+output_path+'hist_'+proc+'.root '+input_path+'hist_'+proc+'*')

	f_tmp = TFile.Open(output_path+'hist_'+proc+'.root', 'update')
	f_tmp.Delete("EventInfo")
	f_tmp.Delete("ScaleWeights")
	f_org = TFile.Open(input_path+"hist_"+proc+".root","read")
	evt = f_org.Get("EventInfo")
	swt = f_org.Get("ScaleWeights")
	f_tmp.cd()
	evt.Write()
	swt.Write()
	f_tmp.Write()
	f_tmp.Close()

    print("All post processes are completed")
