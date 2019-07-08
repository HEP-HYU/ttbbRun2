import math

from ROOT import *
import ROOT

def calError(h_nom, h_sys, delta=True):
    error = []
    h_delta = h_sys.Clone()
    if not delta:
        for ibin in range(1, h_nom.GetNbinsX()+1):
            value = h_sys.GetBinContent(ibin) - h_nom.GetBinContent(ibin)
            h_delta.SetBinContent(ibin, value)

    for ibin in range(1,h_nom.GetNbinsX()+1):
        err = 0.0
        if h_nom.GetBinContent(ibin) > 0.0:
            err = (h_delta.GetBinContent(ibin)/h_nom.GetBinContent(ibin))*100
	error.append(err)

    return error

f_err = open('../output/unfold/syst_errors.tex', 'w')
f_list = []
f_list.append(TFile.Open("../output/unfold/hist_unfolded_hist_DataSingleMu.root"))
f_list.append(TFile.Open("../output/unfold/hist_unfolded_hist_DataSingleEl.root"))
syst_list = [
    "__jer", "__jec",
    "__musf", "__mutrg",
    "__elsf", "__eltrg",
    "__lf", "__hf",
    "__hfstat1", "__hfstat2",
    "__lfstat1", "__lfstat2",
    "__cferr1",  "__cferr2",
    "__pu",
    "__ps", "__sw",
    "__hdamp", "__tune"
]
syst_lep  = ["__musf", "__mutrg", "__elsf", "__eltrg"]
syst_btag = [
    "__lf", "__hf", "__hfstat1", "__hfstat2",
    "__lfstat1", "__lfstat2", "__cferr1", "__cferr2"
]
genmode = "mindR"
histlist = ["RecoAddbJetDeltaR", "RecoAddbJetInvMass", "RecoAddbJetDeltaPhi", "RecoAddbJetDeltaEta"]
for f_in in f_list:
    ich = 999
    if   "Mu" in f_in.GetName(): ich = 0
    elif "El" in f_in.GetName(): ich = 1
    f_err.write('%'+f_in.GetName()+'\n')
    f_err.write('% Channel: '+str(ich)+'\n')
    for hist in histlist:
        # Calculate Matrix Uncertainties
        histname = "diffXsec_Unfolded_h_"+genmode+"_"+hist+"_Ch"+str(ich)+"_S3"
	print(histname)
        h_nom = f_in.Get(histname)
        binnum = h_nom.GetNbinsX()
	sys_tot_sqaure = [0 for i in range(binnum)]
	row = 'c'
	for i in range(binnum): row += 'c'
	tmpname = hist.replace('_',' ')
	f_err.write('\\begin{table}\n')
	f_err.write('    \\caption{'+tmpname+'}\n')
	f_err.write('    \\begin{center}\n')
	f_err.write('        \\begin{tabular}{'+row+'}\n')
	f_err.write('            \\hline\\hline\n')
	col = ''
	for i in range(binnum): col += ' & '+str(i+1)+' bin'
	f_err.write('            Source '+col+'\\\\\n')
	f_err.write('            \\hline\n')
	f_err.write('            Matrix uncertainties \\\\\n')
        syst_errors = {}
        for sys in syst_list:
            if "ps" in sys or "sw" in sys or "hdamp" in sys or "tune" in sys: continue
            histname = "diffXsec_h_"+genmode+"_"+hist+"_Ch"+str(ich)+"_S3_DeltaSysSource"+sys
            if 'Ch0' in hist and 'el' in sys: continue
            elif 'Ch1' in hist and 'mu' in sys: continue
            
	    errors = []
            err_up = calError(h_nom,f_in.Get(histname+'up'))
            err_down = calError(h_nom, f_in.Get(histname+'down'))
            for index2, up in enumerate(err_up):
                tmp_e = max(abs(up), abs(err_down[index2]))
                errors.append(tmp_e)
                sys_tot_sqaure[index2] = pow(tmp_e,2)
            syst_errors[sys] = errors
        
	err_btag = []
        str_btag = ''
	for idx, value in enumerate(syst_errors[syst_btag[0]]):
            tmp_esquare = pow(value, 2)
	    for idx2, value in enumerate(syst_btag):
	        if idx2 == 0: continue
		tmp_esquare += pow(syst_errors[value][idx], 2)
	    err_btag.append(math.sqrt(tmp_esquare))
        for i in err_btag: str_btag += ' & $\\pm$'+str(format(i,'.3f')) + '\\%'
        f_err.write('            bTag'+str_btag+'\\\\\n')

	err_lep = []
        str_lep = ''
	for idx, value in enumerate(syst_errors[syst_lep[0]]):
            tmp_esquare = pow(value, 2)
	    for idx2, value in enumerate(syst_lep):
	        if idx2 == 0: continue
		tmp_esquare += pow(syst_errors[value][idx], 2)
	    err_lep.append(math.sqrt(tmp_esquare))
        for i in err_lep: str_lep += ' & $\\pm$'+str(format(i,'.3f')) + '\\%'
        f_err.write('            lepton'+str_lep+'\\\\\n')

        for key, value in syst_errors.items():
            tmp = ''
            for i in value: tmp += ' & $\\pm$'+str(format(i,'.3f')) + '\\%'
            tmpkey = key.replace('__','')
            f_err.write('            '+tmpkey+tmp+'\\\\\n')
        
	# Calculate Acceptance Uncertainties
	f_err.write('            \\hline\n')
	f_err.write('            Acceptance uncertainties \\\\\n')
        syst_errors = {}
        for sys in syst_list:
            histname = "diffXsec_Unfolded_h_"+genmode+"_"+hist+"_Ch"+str(ich)+"_S3_Acceptance"+sys
            if 'Ch0' in hist and 'el' in sys: continue
            elif 'Ch1' in hist and 'mu' in sys: continue
            errors = []
            err_up = calError(h_nom,f_in.Get(histname+'up'), False)
            err_down = calError(h_nom, f_in.Get(histname+'down'), False)
            for index2, up in enumerate(err_up):
                tmp_e = max(abs(up), abs(err_down[index2]))
                errors.append(tmp_e)
                sys_tot_sqaure[index2] += pow(tmp_e,2)
            syst_errors[sys] = errors
        
	err_btag = []
        str_btag = ''
	for idx, value in enumerate(syst_errors[syst_btag[0]]):
            tmp_esquare = pow(value, 2)
	    for idx2, value in enumerate(syst_btag):
	        if idx2 == 0: continue
		tmp_esquare += pow(syst_errors[value][idx], 2)
	    err_btag.append(math.sqrt(tmp_esquare))
        for i in err_btag: str_btag += ' & $\\pm$'+str(format(i,'.3f')) + '\\%'
        f_err.write('            bTag'+str_btag+'\\\\\n')

	err_lep = []
        str_lep = ''
	for idx, value in enumerate(syst_errors[syst_lep[0]]):
            tmp_esquare = pow(value, 2)
	    for idx2, value in enumerate(syst_lep):
	        if idx2 == 0: continue
		tmp_esquare += pow(syst_errors[value][idx], 2)
	    err_lep.append(math.sqrt(tmp_esquare))
        for i in err_lep: str_lep += ' & $\\pm$'+str(format(i,'.3f')) + '\\%'
        f_err.write('            lepton'+str_lep+'\\\\\n')

        for key, value in syst_errors.items():
            tmp = ''
            for i in value: tmp += ' & $\\pm$'+str(format(i,'.3f')) + '\\%'
            tmpkey = key.replace('__','')
            f_err.write('            '+tmpkey+tmp+'\\\\\n')

        # Calculate Background Uncertainties
	f_err.write('            \\hline\n')
	f_err.write('            Background uncertainties \\\\\n')
        syst_errors = {}
        for sys in syst_list:
            histname = "diffXsec_Unfolded_h_"+genmode+"_"+hist+"_Ch"+str(ich)+"_S3_Background"+sys
            if 'Ch0' in hist and 'el' in sys: continue
            elif 'Ch1' in hist and 'mu' in sys: continue
            errors = []
            err_up = calError(h_nom,f_in.Get(histname+'up'), False)
            err_down = calError(h_nom, f_in.Get(histname+'down'), False)
            for index2, up in enumerate(err_up):
                tmp_e = max(abs(up), abs(err_down[index2]))
                errors.append(tmp_e)
                sys_tot_sqaure[index2] += pow(tmp_e,2)
            syst_errors[sys] = errors
        
	err_btag = []
        str_btag = ''
	for idx, value in enumerate(syst_errors[syst_btag[0]]):
            tmp_esquare = pow(value, 2)
	    for idx2, value in enumerate(syst_btag):
	        if idx2 == 0: continue
		tmp_esquare += pow(syst_errors[value][idx], 2)
	    err_btag.append(math.sqrt(tmp_esquare))
        for i in err_btag: str_btag += ' & $\\pm$'+str(format(i,'.3f')) + '\\%'
        f_err.write('            bTag'+str_btag+'\\\\\n')

	err_lep = []
        str_lep = ''
	for idx, value in enumerate(syst_errors[syst_lep[0]]):
            tmp_esquare = pow(value, 2)
	    for idx2, value in enumerate(syst_lep):
	        if idx2 == 0: continue
		tmp_esquare += pow(syst_errors[value][idx], 2)
	    err_lep.append(math.sqrt(tmp_esquare))
        for i in err_lep: str_lep += ' & $\\pm$'+str(format(i,'.3f')) + '\\%'
        f_err.write('            lepton'+str_lep+'\\\\\n')

        for key, value in syst_errors.items():
            tmp = ''
            for i in value: tmp += ' & $\\pm$'+str(format(i,'.3f')) + '\\%'
            tmpkey = key.replace('__','')
            f_err.write('            '+tmpkey+tmp+'\\\\\n')

        # Write total systematic uncertainties
        tmp2 = ''
	for value in sys_tot_sqaure:
            tmp2 += ' & $\\pm$' + str(format(math.sqrt(value),'.3f')) + '\\%'
	f_err.write('            Total sys. unc.'+tmp2+'\\\\\n')
        f_err.write('            \\hline\\hline\n')
	f_err.write('        \\end{tabular}\n')
	f_err.write('    \\end{center}\n')
	f_err.write('\\end{table}\n')

f_err.close()

