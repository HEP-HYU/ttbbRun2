from ROOT import *
import ROOT
import os
import sys
base_path = "/home/seohyun/work/ttbb/heptool/output/root/"

#make systematics up down histograms

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

proc_list = ['ttH','ttW','ttZ','ttbb', 'ttbj', 'ttcc', 'ttLF', 'ttother', 'ttbkg']
ps_list = ['isrup','isrdown','fsrup','fsrdown']
sw_list = ['scale0','scale1','scale2','scale3','scale4','scale5']

for proc in proc_list:
    print(proc)
    f_central = TFile.Open(os.path.join(base_path, "hist_"+proc+".root"))

    f_ps_list = []
    if not (proc == 'ttH' or proc == 'ttW' or proc == 'ttZ'):
        for ps in ps_list:
            f_ps_list.append(TFile.Open(os.path.join(base_path, "hist_"+proc+"__"+ps+".root")))
	f_ps_up = TFile.Open(os.path.join(base_path, "hist_"+proc+"__psup.root"),"RECREATE")
        f_ps_down = TFile.Open(os.path.join(base_path, "hist_"+proc+"__psdown.root"),"RECREATE")

	h_psweights = ROOT.TH1D("PSweights","",4,0,4)
	for index, value in enumerate(ps_list):
            h_psweights.GetXaxis().SetBinLabel(index+1,value)
            tmp = (f_ps_list[index].Get("EventInfo")).GetBinContent(2)
            h_psweights.SetBinContent(index+1,tmp)

	ps_sum = h_psweights.Integral()
    else:
        ps_sum = -9999
	f_ps_up = None
	f_ps_down = None

    f_sw_list = []
    for sw in sw_list:
        f_sw_list.append(TFile.Open(os.path.join(base_path, "hist_"+proc+"__"+sw+".root")))

    f_sw_up = TFile.Open(os.path.join(base_path, "hist_"+proc+"__swup.root"),"RECREATE")
    f_sw_down = TFile.Open(os.path.join(base_path, "hist_"+proc+"__swdown.root"),"RECREATE")

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
        if not (proc == 'ttH' or proc == 'ttW' or proc == 'ttZ'):
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
    if f_ps_up is not None: f_ps_up.Close()
    if f_ps_down is not None: f_ps_down.Close()

#Rescaling and merging files
def rescale(f_sys, h_eventinfo):
    hist_list = []
    hist_list = [x.GetName() for x in f_sys.GetListOfKeys()]
    h_eventinfo_sys = f_sys.Get("EventInfo")

    h_out =[]
    for hist in hist_list:
        if hist == "EventInfo" or hist == "ScaleWeights": continue
	h_tmp = f_sys.Get(hist)
	h_tmp.Scale(h_eventinfo.GetBinContent(2)/h_eventinfo_sys.GetBinContent(2))
        h_out.append(h_tmp)

    return h_out

sys_re = ['isrup', 'isrdown', 'fsrup', 'fsrdown', 'hdampup', 'hdampdown', 'tuneup', 'tunedown']
proc_re = ['ttbb', 'ttbj', 'ttcc', 'ttLF', 'ttother']

for proc in proc_re:
    print(proc)
    f_central = TFile.Open(os.path.join(base_path, "hist_"+proc+".root"))
    h_eventinfo = f_central.Get("EventInfo")
    for sys in sys_re:
        f_in = TFile.Open(os.path.join(base_path, "hist_"+proc+"__"+sys+".root"))
	f_out = TFile.Open(os.path.join(base_path, "hist_"+proc+"__"+sys+"_re.root"), "recreate")

	h_out = []
	h_out = rescale(f_in, h_eventinfo)
        f_out.cd()
        for hist in h_out:
            if 'EventInfo' in hist.GetName() or 'ScaleWeights' in hist.GetName(): continue
            hist.Write()
        f_out.Write()
        f_out.Close()
        os.system('rm -rf '+base_path+'hist_'+proc+'__'+sys+'.root')
	os.system('mv '+base_path+'hist_'+proc+'__'+sys+'_re.root '+base_path+'hist_'+proc+'__'+sys+'.root')

proc_list = ['DataSingleEl', 'DataSingleMu', 'ttbb', 'ttbj', 'ttcc', 'ttLF', 'ttother', 'ttbkg',
             'ttH', 'ttW', 'ttZ', 'wjets', 'zjets10to50', 'zjets',
             'ww', 'wz', 'zz', 'tchannel', 'tbarchannel', 'tWchannel',
             'tbarWchannel']

for proc in proc_list:
    print(proc)
    os.system('hadd ../output/post/hist_'+proc+'.root ../output/root/hist_'+proc+'*')
    f_in = TFile.Open('../output/post/hist_'+proc+'.root')
    f_out = TFile.Open('../output/post2/hist_'+proc+'.root','recreate')

    f_out.cd()
    hist_list = []
    hist_list = [x.GetName() for x in f_in.GetListOfKeys()]
    for hist in hist_list:
        h_tmp = f_in.Get(hist)
        hist2 = hist.replace('_'+proc, '')
	print(hist)
        print(hist2)
        h_tmp.SetName(hist2)
        h_tmp.Write()

    f_out.Write()
    f_out.Close()
