import math

from ROOT import *
import ROOT

def calError(h_nom, h_sys):
    error = []
    for ibin in range(1,h_nom.GetNbinsX()+1):
	err = (h_sys.GetBinContent(ibin)/h_nom.GetBinContent(ibin))*100
	error.append(err)

    return error


f_list = []
f_list.append(TFile.Open("../test/hist_unfolded_hist_DataSingleMu.root"))
f_list.append(TFile.Open("../test/hist_unfolded_hist_DataSingleEl.root"))
f_list.append(TFile.Open("../test/hist_unfolded_hist_ttbb.root"))

hist_list1 = [
    "h_unfolded_dR_Ch0_S3", "h_unfolded_M_Ch0_S3",
    "h_unfolded_dR_Ch1_S3", "h_unfolded_M_Ch1_S3"
]

hist_list2 = [
    "h_RecoJetDeltaR_Ch0_S3_DeltaSysSource", "h_RecoJetInvMass_Ch0_S3_DeltaSysSource",
    "h_RecoJetDeltaR_Ch1_S3_DeltaSysSource", "h_RecoJetInvMass_Ch1_S3_DeltaSysSource"
]

syst_list = [
    "__ps", "__sw",
    "__jer", "__jec",
    "__musf", "__mutrg",
    "__elsf", "__eltrg",
    "__lf", "__hf",
    "__hfstat1", "__hfstat2",
    "__lfstat1", "__lfstat2",
    "__cferr1",  "__cferr2"
]


f_err = open('syst_errors.tex', 'w')
for f_in in f_list:
    print(f_in.GetName())
    f_err.write('%'+f_in.GetName()+'\n')
    for index, hist in enumerate(hist_list1):
        h_nom = f_in.Get(hist)
	binnum = h_nom.GetNbinsX()
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
	syst_errors = {}
	sys_tot_sqaure = [0 for i in range(binnum)]
        for sys in syst_list:
            if 'ps' in sys or 'sw' in sys or 'je' in sys: continue
            if 'Ch0' in hist and 'el' in sys: continue
            elif 'Ch1' in hist and 'mu' in sys: continue
            print(hist)
            print(hist_list2[index])
            print(hist_list2[index]+sys+'up')
            errors = []
            print(f_in.Get(hist_list2[index]+sys+'up').GetName())
            err_up = calError(h_nom,f_in.Get(hist_list2[index]+sys+'up'))
            err_down = calError(h_nom, f_in.Get(hist_list2[index]+sys+'down'))
            for index2, up in enumerate(err_up):
                tmp_e = max(abs(up), abs(err_down[index2]))
                errors.append(tmp_e)
		sys_tot_sqaure[index2] = pow(tmp_e,2)
            syst_errors[sys] = errors

        for key, value in syst_errors.items():
            tmp = ''
            for i in value: tmp += ' & $\\pm$'+str(format(i,'.3f')) + '\\%'
            tmpkey = key.replace('__','')
            f_err.write('            '+tmpkey+tmp+'\\\\\n')
	f_err.write('            \\hline\n')
	tmp2 = ''
	for value in sys_tot_sqaure:
            tmp2 += ' & $\\pm$' + str(format(math.sqrt(value),'.3f')) + '\\%'
	f_err.write('            Total sys. unc.'+tmp2+'\\\\\n')
        f_err.write('            \\hline\\hline\n')
	f_err.write('        \\end{tabular}\n')
	f_err.write('    \\end{center}\n')
	f_err.write('\\end{table}\n')

f_err.close()

