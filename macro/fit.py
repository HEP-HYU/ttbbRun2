import os, sys
import subprocess

from ROOT import *
import ROOT

luminosities = {16:35922, 17:41529, 18:59693}
for year in range(16,19):
    samples = []
    with open('../samples/sample'+str(year)+'.txt','r') as f:
        n = 1
	while True:
            line = f.readline()
            if not line: break
            if n != 1:
                samples.append(line[:-1])
            n += 1

    genevt = {}
    with open('../samples/genevt'+str(year)+'.txt','r') as f:
        while True:
            line = f.readline()
            if not line: break
            tmp = line.split(' ')
            genevt[tmp[0]] = float(tmp[1])

    xsec = {}
    with open('../samples/xsec'+str(year)+'.txt', 'r') as f:
        n = 1
        while True:
            line = f.readline()
            if not line: break
            if n != 1:
                tmp = line.split(' ')
		xsec[tmp[0]] = float(tmp[1])
            n += 1


    f_template = ROOT.TFile('../output/template/hist_temp'+str(year)+'.root','recreate')
    f_template.cd()
    data_name = ['DataSingleMu', 'DataSingleEG']
    step = 1

    for ich in range(0,3):
        hist_name = 'h_nbJets_Ch'+str(ich)+'_S'+str(step)
	if ich < 2:
            f_data = ROOT.TFile('../output/root'+str(year)+'/hist_'+data_name[ich]+'.root')
            h_tmp = f_data.Get(hist_name)
	    h_tmp.SetName(hist_name+'_'+data_name[ich])
	    f_template.cd()
	    h_tmp.Write()
	    f_data.Close()
	else:
	    f_mu = ROOT.TFile('../output/root'+str(year)+'/hist_DataSingleMu.root')
	    f_el = ROOT.TFile('../output/root'+str(year)+'/hist_DataSingleEG.root')
	    h_mu = f_mu.Get(hist_name)
	    h_el = f_el.Get(hist_name)
	    h_mu.Add(h_el)
	    h_mu.SetName(hist_name+'_DataSingleLep')
	    f_template.cd()
	    h_mu.Write()
	    f_mu.Close()
	    f_el.Close()

        # Merge ttbar background histograms
        str_tmp = 'TTLJ_PowhegPythia_ttother'
        f_ttother = ROOT.TFile('../output/root'+str(year)+'/hist_'+str_tmp+'.root')
	h_ttbkg = f_ttother.Get(hist_name)
	h_ttbkg.Scale(luminosities[year]*xsec[str_tmp]/genevt[str_tmp])
        for sample in samples:
            if not 'Bkg' in sample: continue
            f_bkg = ROOT.TFile('../output/root'+str(year)+'/hist_'+sample+'.root')
            h_tmp = f_bkg.Get(hist_name)
            h_tmp.Scale(luminosities[year]*xsec[sample]/genevt[sample])
            h_ttbkg.Add(h_tmp)
            f_bkg.Close()

        f_template.cd()
	h_ttbkg.SetName(hist_name+'_ttbkg')
	h_ttbkg.Write()
	hh_ttbkg = h_ttbkg.Clone()
	f_ttother.Close()

        # Merge W + Jets histograms
	if year > 16:
	    str_wjets = ['W1JetsToLNu_MadgraphPythia', 'W2JetsToLNu_MadgraphPythia',
	        'W3JetsToLNu_MadgraphPythia', 'W4JetsToLNu_MadgraphPythia']
            f_w1jets = ROOT.TFile('../output/root'+str(year)+'/hist_'+str_wjets[0]+'.root')
            h_wjets = f_w1jets.Get(hist_name)
	    h_wjets.Scale(luminosities[year]*xsec[str_wjets[0]]/genevt[str_wjets[0]])
	    for i in range(1,4):
	        f_wjets = ROOT.TFile('../output/root'+str(year)+'/hist_'+str_wjets[i]+'.root')
		h_tmp = f_wjets.Get(hist_name)
		h_tmp.Scale(luminosities[year]*xsec[str_wjets[i]]/genevt[str_wjets[i]])
		h_wjets.Add(h_tmp)
		f_wjets.Close()

	    f_template.cd()
	    h_wjets.SetName(hist_name+'_WJets')
	    h_wjets.Write()
	    hh_wjets = h_wjets.Clone()
	    f_w1jets.Close()
	else:
	    str_w = 'WJets_aMCatNLO'
	    f_wjets = ROOT.TFile('../output/root'+str(year)+'/hist_'+str_w+'.root')
	    h_wjets = f_wjets.Get(hist_name)
	    h_wjets.Scale(luminosities[year]*xsec[str_w]/genevt[str_w])
	    f_template.cd()
	    h_wjets.SetName(hist_name+'_WJets')
	    h_wjets.Write()
	    hh_wjets = h_wjets.Clone()
	    f_wjets.Close()

        # Merge Z + Jets histograms
	ii = 0
	for sample in samples:
	    if not 'ZJets' in sample: continue
	    if ii == 0:
	        f_zjets = ROOT.TFile('../output/root'+str(year)+'/hist_'+sample+'.root')
		h_zjets = f_zjets.Get(hist_name)
		h_zjets.Scale(luminosities[year]*xsec[sample]/genevt[sample])
		ii += 1
	    else:
	        f_zjzj = ROOT.TFile('../output/root'+str(year)+'/hist_'+sample+'.root')
		h_tmp = f_zjzj.Get(hist_name)
		h_tmp.Scale(luminosities[year]*xsec[sample]/genevt[sample])
		h_zjets.Add(h_tmp)
		f_zjzj.Close()
        f_template.cd()
	h_zjets.SetName(hist_name+'_ZJets')
	h_zjets.Write()
	hh_zjets = h_zjets.Clone()
	f_zjets.Close()

        # Merge Single Top histograms
        if year == 16: str_st0 = 'SingleTop_t_Powheg'
	else: str_st0 = 'SingleTop_t_PowhegPythia'
        f_st0 = ROOT.TFile('../output/root'+str(year)+'/hist_'+str_st0+'.root')
	h_st = f_st0.Get(hist_name)
	h_st.Scale(luminosities[year]*xsec[str_st0]/genevt[str_st0])
	for sample in samples:
	    if str_st0 in sample: continue
	    if not 'SingleT' in sample: continue
	    f_st = ROOT.TFile('../output/root'+str(year)+'/hist_'+sample+'.root')
	    h_tmp = f_st.Get(hist_name)
	    h_tmp.Scale(luminosities[year]*xsec[sample]/genevt[sample])
	    h_st.Add(h_tmp)
	    f_st.Close()

	f_template.cd()
	h_st.SetName(hist_name+'_SingleTop')
	h_st.Write()
	hh_st = h_st.Clone()
	f_st0.Close()

        # Merge QCD histograms
        str_qcd0 = 'QCD_Pt-1000toInf_MuEnriched'
	f_qcd0 = ROOT.TFile('../output/root'+str(year)+'/hist_'+str_qcd0+'.root')
	h_qcd = f_qcd0.Get(hist_name)
	h_qcd.Scale(luminosities[year]*xsec[str_qcd0]/genevt[str_qcd0])
	for sample in samples:
	    if str_qcd0 in sample: continue
	    if not 'QCD' in sample: continue
	    f_qcd = ROOT.TFile('../output/root'+str(year)+'/hist_'+sample+'.root')
	    h_tmp = f_qcd.Get(hist_name)
	    h_tmp.Scale(luminosities[year]*xsec[sample]/genevt[sample])
	    h_qcd.Add(h_tmp)
	    f_qcd.Close()
	f_template.cd()
	h_qcd.SetName(hist_name+'_QCD')
	h_qcd.Write()
	hh_qcd = h_qcd.Clone()
	f_qcd0.Close()

        # Merge VV histograms
	f_ww = ROOT.TFile('../output/root'+str(year)+'/hist_WW_Pythia.root')
	f_wz = ROOT.TFile('../output/root'+str(year)+'/hist_WZ_Pythia.root')
	f_zz = ROOT.TFile('../output/root'+str(year)+'/hist_ZZ_Pythia.root')
	h_vv = f_ww.Get(hist_name)
	h_vv.Scale(luminosities[year]*xsec['WW_Pythia']/genevt['WW_Pythia'])
	h_tmp = f_wz.Get(hist_name)
	h_tmp.Scale(luminosities[year]*xsec['WZ_Pythia']/genevt['WZ_Pythia'])
	h_vv.Add(h_tmp)
	h_tmp = f_zz.Get(hist_name)
	h_tmp.Scale(luminosities[year]*xsec['ZZ_Pythia']/genevt['ZZ_Pythia'])
	f_template.cd()
	h_vv.Add(h_tmp)
	h_vv.SetName(hist_name+'_VV')
	h_vv.Write()
	hh_vv = h_vv.Clone()
	f_ww.Close()
	f_wz.Close()
	f_zz.Close()

        # Merge ttX histograms
	if year == 16: str_ttX0 = 'ttHbb_PowhegPythia'
	else: str_ttX0 = 'ttHTobb_PowhegPythia'
	f_ttX0 = ROOT.TFile('../output/root'+str(year)+'/hist_'+str_ttX0+'.root')
	h_ttX = f_ttX0.Get(hist_name)
	h_ttX.Scale(luminosities[year]*xsec[str_ttX0]/genevt[str_ttX0])
	for sample in samples:
	    if str_ttX0 in sample: continue
	    if (not 'ttH' in sample) or (not 'ttW' in sample) or (not 'ttZ' in sample): continue
	    f_ttX = ROOT.TFile('../output/root'+str(year)+'/hist_'+sample+'.root')
	    h_tmp = f_ttX.Get(hist_name)
	    h_tmp.Scale(luminosities[year]*xsec[sample]/genevt[sample])
	    h_ttX.Add(h_tmp)
	    f_qcd.Close()
	f_template.cd()
	h_ttX.SetName(hist_name+'_ttX')
	h_ttX.Write()
	hh_ttX = h_ttX.Clone()
	f_qcd0.Close()

        # Add ttbar samples
	for sample in samples:
	    if not 'TTLJ' in sample: continue
	    if 'Filter' in sample: continue
	    if 'ttother' in sample or 'Bkg' in sample: continue
	    f_tt = ROOT.TFile('../output/root'+str(year)+'/hist_'+sample+'.root')
	    h_tt = f_tt.Get(hist_name)
	    h_tt.Scale(luminosities[year]*xsec[sample]/genevt[sample])
	    h_tt.SetName(hist_name+'_'+sample[-4:])
	    f_template.cd()
	    h_tt.Write()
	    if 'ttbb' in sample: hh_ttbb = h_tt.Clone()
	    if 'ttLF' in sample: hh_ttLF = h_tt.Clone()
	    if 'ttcc' in sample: hh_ttcc = h_tt.Clone()
	    f_tt.Close()

        #h_ttX, h_vv, h_st, h_zjets, h_wjets, h_ttbkg, h_ttLF, h_ttcc, h_qcd

        h_group0 = hh_ttbb
	h_group0.Add(hh_ttX)
	h_group0.SetName(hist_name+'_ttbbttX')
        h_group1 = hh_wjets
        h_group1.Add(hh_zjets)
        h_group1.Add(hh_qcd)
        h_group1.Add(hh_vv)
        h_group1.SetName(hist_name+'_WZJetsQCDVV')
        h_group2 = hh_ttLF
        h_group2.Add(hh_ttcc)
        h_group2.Add(hh_st)
        h_group2.Add(hh_ttbkg)
        h_group2.SetName(hist_name+'_STttbkgLFcc')
        f_template.cd()
	h_group0.Write()
        h_group1.Write()
        h_group2.Write()

    f_template.Close()

    for ich in range(0,3):
        hist_name = hist_name[:-7]+"_Ch"+str(ich)+"_S"+str(step)
        if not os.path.exists("../output/fit/"+str(year)):
            os.mkdir('../output/fit/'+str(year))

        cmd = ['root','-l','-b','-q','histFactory.C+('+str(year)+', "'+hist_name+'")']

        with open('../output/fit/'+str(year)+'/result'+str(ich)+'.txt', 'w') as f_out:
            subprocess.call(cmd, stdout=f_out)

