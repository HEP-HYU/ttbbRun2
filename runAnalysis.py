#!/usr/bin/python
test   = False 
do_sys = False

from ROOT import TChain, TProof, TFile, TH1D, TH1F, TCanvas

def runAna(dir, file, name):
    chain = TChain("ttbbLepJets/tree","events")
    chain.Add(dir+"/"+file)
    chain.SetProof();
    chain.Process("macro/MyAnalysis.C+",name)

    f = TFile(dir+"/"+file,"read")

    ## save Event Summary histogram ##
    out = TFile("output/root/hist_"+name+".root","update")
    hevt = f.Get("ttbbLepJets/EventInfo")
    hsw = f.Get("ttbbLepJets/ScaleWeights")
    hevt.Write()
    hsw.Write()
    out.Write()
    out.Close()

p = TProof.Open("", "workers=8")

inputDir = "/data/users/seohyun/ntuple/Run2018/V10_2/nosplit"

if test:
    runAna(inputDir, "ttWToLNu_aMCatNLOMadspinPythia.root",    "ttWtolnu")
else:
    #Response matrix
    runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "ttbbmatrix")

    runAna(inputDir, "DataSingleEG.root","DataSingleEl")
    runAna(inputDir, "DataSingleMu.root","DataSingleMu")

    runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root",    "ttbb")
    runAna(inputDir, "TTLJ_PowhegPythia_ttbj.root",    "ttbj")
    runAna(inputDir, "TTLJ_PowhegPythia_ttcc.root",    "ttcc")
    runAna(inputDir, "TTLJ_PowhegPythia_ttLF.root",    "ttLF")
    runAna(inputDir, "TTLJ_PowhegPythia_ttother.root", "ttother")

    runAna(inputDir, "TTLL_PowhegPythiaBkg.root", "ttbkg_ttll")
    runAna(inputDir, "TTLJ_PowhegPythiaBkg.root", "ttbkg_ttlj")
    runAna(inputDir, "TTJJ_PowhegPythiaBkg.root", "ttbkg_ttjj")

    runAna(inputDir, "ttHToNonbb_PowhegPythia.root",           "ttHtononbb")
    runAna(inputDir, "ttHTobb_PowhegPythia.root",              "ttHtobb")
    runAna(inputDir, "ttWToLNu_aMCatNLOMadspinPythia.root",    "ttWtolnu")
    runAna(inputDir, "ttWToQQ_aMCatNLOMadspinPythia.root",     "ttWtoqq")
    runAna(inputDir, "ttZToLLNuNu_aMCatNLOMadspinPythia.root", "ttZtollnunu")
    runAna(inputDir, "ttZToQQ_aMCatNLOMadspinPythia.root",     "ttZtoqq")

    runAna(inputDir, "WJetsToLNu_MadgraphPythia.root",    "wjets")
    runAna(inputDir, "W1JetsToLNu_MadgraphPythia.root",   "w1jets")
    runAna(inputDir, "W2JetsToLNu_MadgraphPythia.root",   "w2jets")
    runAna(inputDir, "W3JetsToLNu_MadgraphPythia.root",   "w3jets")
    runAna(inputDir, "W4JetsToLNu_MadgraphPythia.root",   "w4jets")
    runAna(inputDir, "ZJets_M10to50_MadgraphPythia.root", "zjets10to50")
    runAna(inputDir, "ZJets_M50_aMCatNLOPythia.root",     "zjets50")
    runAna(inputDir, "WW_Pythia.root",                    "ww")
    runAna(inputDir, "WZ_Pythia.root",                    "wz")
    runAna(inputDir, "ZZ_Pythia.root",                    "zz")
    runAna(inputDir, "SingleTop_s_aMCatNLOPythia.root",   "singletop_s")
    runAna(inputDir, "SingleTop_t_PowhegPythia.root",     "singletop_t")
    runAna(inputDir, "SingleTop_tW_PowhegPythia.root",    "singletop_tW")
    runAna(inputDir, "SingleTbar_t_PowhegPythia.root",    "singletbar_t")
    runAna(inputDir, "SingleTbar_tW_PowhegPythia.root",   "singletbar_tW")

    ### Systematics ###
    ##### List #####
    # PileUp
    #   __puup, __pudown
    # Muon
    #   __muidup, __muiddown, __muisoup, __muisodown, __mutrgup, __mutrgdown
    # Electron
    #   __elidup, __eliddown, __elrecoup, __elrecodown, __elzvtxup, __elzvtxdown
    #   __eltrgup, __eltrgdown
    # CSV shape
    #   __lfup, __lfdown, __hfup, __hfdown
    #   __hfstat1up, __hfstat1down, __hfstat2up, __hfstat2down
    #   __lfstat1up, __lfstat1down, __lfstat2up, __lfstat2down
    #   __cferr1up, __cferr1down, __cferr2up, __cferr2down
    # Scale Weight(ME)
    #   __scale0, __scale1, __scale2, __scale3, __scale4, __scale5
    # Parton Shower(PS)
    #   __isrup, __isrdown, __fsrup, __fsrdown
    # ME & PS
    #   __hdampup, __hdampdown
    # Pythia Tune
    #   __tuneup, __tunedown
    #####
    sys_list = [
	"__jerup", "__jerdown", "__jecup", "__jecdown",
	"__puup", "__pudown",
	"__musfup", "__musfdown", "__mutrgup", "__mutrgdown",
	"__elsfup", "__elsfdown", "__eltrgup", "__eltrgdown",
	"__lfup", "__lfdown", "__hfup", "__hfdown",
	"__hfstat1up", "__hfstat1down", "__hfstat2up", "__hfstat2down",
	"__lfstat1up", "__lfstat1down", "__lfstat2up", "__lfstat2down",
	"__cferr1up", "__cferr1down", "__cferr2up", "__cferr2down"
    ]
    sys_list2 = [
	"__scale0", "__scale1","__scale2","__scale3","__scale4","__scale5",
	"__isrup", "__isrdown", "__fsrup", "__fsrdown", "__pdfup", "__pdfdown"
    ]

    if do_sys:
        for index, value in enumerate(sys_list):
            runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "ttbbmatrix"+value)

	    runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root",    "ttbb"+value)
	    runAna(inputDir, "TTLJ_PowhegPythia_ttbj.root",    "ttbj"+value)
	    runAna(inputDir, "TTLJ_PowhegPythia_ttcc.root",    "ttcc"+value)
	    runAna(inputDir, "TTLJ_PowhegPythia_ttLF.root",    "ttLF"+value)
	    runAna(inputDir, "TTLJ_PowhegPythia_ttother.root", "ttother"+value)

	    runAna(inputDir, "TTLL_PowhegPythiaBkg.root", "ttbkg_ttll"+value)
	    runAna(inputDir, "TTLJ_PowhegPythiaBkg.root", "ttbkg_ttlj"+value)
	    runAna(inputDir, "TTJJ_PowhegPythiaBkg.root", "ttbkg_ttjj"+value)

	    runAna(inputDir, "ttHToNonbb_PowhegPythia.root",           "ttHtononbb"+value)
	    runAna(inputDir, "ttHTobb_PowhegPythia.root",              "ttHtobb"+value)
	    runAna(inputDir, "ttWToLNu_aMCatNLOMadspinPythia.root",    "ttWtolnu"+value)
	    runAna(inputDir, "ttWToQQ_aMCatNLOMadspinPythia.root",     "ttWtoqq"+value)
	    runAna(inputDir, "ttZToLLNuNu_aMCatNLOMadspinPythia.root", "ttZtollnunu"+value)
	    runAna(inputDir, "ttZToQQ_aMCatNLOMadspinPythia.root",     "ttZtoqq"+value)

	    runAna(inputDir, "WJetsToLNu_MadgraphPythia.root",    "wjets"+value)
	    runAna(inputDir, "W1JetsToLNu_MadgraphPythia.root",   "w1jets"+value)
	    runAna(inputDir, "W2JetsToLNu_MadgraphPythia.root",   "w2jets"+value)
	    runAna(inputDir, "W3JetsToLNu_MadgraphPythia.root",   "w3jets"+value)
	    runAna(inputDir, "W4JetsToLNu_MadgraphPythia.root",   "w4jets"+value)
	    runAna(inputDir, "ZJets_M10to50_MadgraphPythia.root", "zjets10to50"+value)
	    runAna(inputDir, "ZJets_M50_aMCatNLOPythia.root",     "zjets50"+value)
	    runAna(inputDir, "WW_Pythia.root",                    "ww"+value)
	    runAna(inputDir, "WZ_Pythia.root",                    "wz"+value)
	    runAna(inputDir, "ZZ_Pythia.root",                    "zz"+value)
	    runAna(inputDir, "SingleTop_s_aMCatNLOPythia.root",   "singletop_s"+value)
	    runAna(inputDir, "SingleTop_t_PowhegPythia.root",     "singletop_t"+value)
	    runAna(inputDir, "SingleTop_tW_PowhegPythia.root",    "singletop_tW"+value)
	    runAna(inputDir, "SingleTbar_t_PowhegPythia.root",    "singletbar_t"+value)
	    runAna(inputDir, "SingleTbar_tW_PowhegPythia.root",   "singletbar_tW"+value)

	for index, value in enumerate(sys_list2):
            runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "ttbbmatrix"+value)
	    
	    runAna(inputDir, "TTLJ_PowhegPythia_ttbb.root", "ttbb"+value)
	    runAna(inputDir, "TTLJ_PowhegPythia_ttbj.root", "ttbj"+value)
	    runAna(inputDir, "TTLJ_PowhegPythia_ttcc.root", "ttcc"+value)
	    runAna(inputDir, "TTLJ_PowhegPythia_ttLF.root", "ttLF"+value)
	    runAna(inputDir, "TTLJ_PowhegPythia_ttother.root", "ttother"+value)

