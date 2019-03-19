#include <TObject.h>
#include <TH1.h>
#include <TH2.h>

//SAMPLE LIST
enum class Sample_List_ {
  TTBB = 0,TTBJ,TTCC,/*TTJJ,*/TTLF,TTOTHER,TTBKG,WJETS,ZJETS,ZJETS10TO50,
  TCHANNEL,TBARCHANNEL,TWCHANNEL,TBARWCHANNEL,WW,WZ,ZZ,TTH,TTW,TTZ,LAST
};

const int TTBB_ = static_cast<int>(Sample_List_::TTBB);
const int TTBJ_ = static_cast<int>(Sample_List_::TTBJ);
const int TTCC_ = static_cast<int>(Sample_List_::TTCC);
//const int TTJJ_ = static_cast<int>(Sample_List_::TTJJ);
const int TTLF_ = static_cast<int>(Sample_List_::TTLF);
const int TTOTHER_ = static_cast<int>(Sample_List_::TTOTHER);
const int TTBKG_ = static_cast<int>(Sample_List_::TTBKG);
const int WJETS_ = static_cast<int>(Sample_List_::WJETS);
const int ZJETS_ = static_cast<int>(Sample_List_::ZJETS);
const int ZJETS10TO50_ = static_cast<int>(Sample_List_::ZJETS10TO50);
const int TCHANNEL_ = static_cast<int>(Sample_List_::TCHANNEL);
const int TBARCHANNEL_ = static_cast<int>(Sample_List_::TBARCHANNEL);
const int TWCHANNEL_ = static_cast<int>(Sample_List_::TWCHANNEL);
const int TBARWCHANNEL_ = static_cast<int>(Sample_List_::TBARWCHANNEL);
const int WW_ = static_cast<int>(Sample_List_::WW);
const int WZ_ = static_cast<int>(Sample_List_::WZ);
const int ZZ_ = static_cast<int>(Sample_List_::ZZ);
const int TTH_ = static_cast<int>(Sample_List_::TTH);
const int TTW_ = static_cast<int>(Sample_List_::TTW);
const int TTZ_ = static_cast<int>(Sample_List_::TTZ);

std::vector<string> DATA_ = {"DataSingleMu","DataSingleEl"};
std::vector<string> NAME_ = {
  "ttbb","ttbj","ttcc",/*"ttjj",*/"ttLF","ttother","ttbkg","wjets","zjets","zjets10to50",
  "tchannel","tbarchannel","tWchannel","tbarWchannel","ww","wz","zz",
  "ttH","ttW","ttZ"
};

//SCALE
const double LUMINOSITY_ = 35922;
const double BRANCHINGRATIO_ = 0.436572;//2*3*0.1086*0.67
//const double LUMINOSITY_ = 35867;

std::vector<double> XSEC_ = {
  365.34, //ttbb 
  365.34, //ttbj 
  365.34, //ttcc
  //356.34,//ttjj
  365.34, //ttLF
  365.34, //ttother
  831.76, //ttbkg
  61526.7, //wjets
  5765.4, //6025.2, //zjets
  18610.0, //zjets10to50
  136.02, //tchannel
  80.95, //tbarchannel
  35.85, //tWchannel
  35.85, //tbarWchannel
  118.7, //ww
  47.13, //wz
  16.523, //zz
  0.2923, //ttH
  0.2043, //ttW
  0.2529 //ttZ
};
//v808
/*
std::vector<double> XSEC_ = {
  332.704, //ttbb 
  332.704, //ttbj 
  332.704, //ttcc
  //332.704,//ttjj
  332.704, //ttLF
  332.704, //ttohter
  831.76, //ttbkg
  61526.7, //wjets
  5765.4, //6025.2, //zjets
  18610.0, //zjets10to50
  136.02, //t tchannel
  80.95, //tbar tchannel
  35.85, //tWchannel
  35.85, //tbarWchannel
  118.7, //ww
  47.13, //wz
  16.523, //zz
  0.2151, //ttH
  0.61, //ttW
  0.78 //ttZ
};*/
enum class QCD_List_ {
  El20to30 = 0, El30to50, El50to80, El80to120, El120to170, El170to300, El300toInf,
  Mu20to30, Mu30to50, Mu50to80, Mu80to120, Mu120to170, Mu170to300, Mu300to470, Mu470to600,
  Mu600to800, Mu800to1000, Mu1000toInf, LAST
};
const int El1 = static_cast<int>(QCD_List_::El20to30);
const int El2 = static_cast<int>(QCD_List_::El30to50);
const int El3 = static_cast<int>(QCD_List_::El50to80);
const int El4 = static_cast<int>(QCD_List_::El80to120);
const int El5 = static_cast<int>(QCD_List_::El120to170);
const int El6 = static_cast<int>(QCD_List_::El170to300);
const int El7 = static_cast<int>(QCD_List_::El300toInf);
const int Mu1 = static_cast<int>(QCD_List_::Mu20to30);
const int Mu2 = static_cast<int>(QCD_List_::Mu30to50);
const int Mu3 = static_cast<int>(QCD_List_::Mu50to80);
const int Mu4 = static_cast<int>(QCD_List_::Mu80to120);
const int Mu5 = static_cast<int>(QCD_List_::Mu120to170);
const int Mu6 = static_cast<int>(QCD_List_::Mu170to300);
const int Mu7 = static_cast<int>(QCD_List_::Mu300to470);
const int Mu8 = static_cast<int>(QCD_List_::Mu470to600);
const int Mu9 = static_cast<int>(QCD_List_::Mu600to800);
const int Mu10 = static_cast<int>(QCD_List_::Mu800to1000);
const int Mu11 = static_cast<int>(QCD_List_::Mu1000toInf);

std::vector<double> QCDXSEC_ = {
  5352960,
  9928000,
  2890800,
  350000,
  62964,
  18810,
  1350,
  2960198.4,
  1652471.46,
  437504.1,
  106033.6648,
  25190.51514,
  8654.49315,
  797.35269,
  79.02553776,
  25.09505908,
  4.707368272,
  1.62131692
};


//tchannel : 44.33, tbarchannel : 26.38
const double TTBAR_XSEC_ = 831.76;
//OBJECT SELECTION
const int MUON_ = 0;
const double MUON_PT_ = 30.0;
const double MUON_ETA_ = 2.1;
const double MUON_PHI_ = 0.0;
const double MUON_E_ = 0.0;

const int ELECTRON_ = 1;
const double ELECTRON_PT_ = 35.0;
const double ELECTRON_ETA_ = 2.1;
const double ELECTRON_PHI_ = 0.0;
const double ELECTRON_E_ = 0.0;

const double JET_PT_ = 30.0;
const double JET_ETA_ = 2.4;
const double JET_PHI_ = 0.0;
const double JET_E_ = 0.0;

const double JET_CSV_TIGHT_ = 0.9535;
const double JET_CSV_MEDIUM_ = 0.8484;
const double JET_CSV_LOOSE_ = 0.0;

const double JET_CvsL_TIGHT_ = 0.0;
const double JET_CvsL_MEDIUM_ = 0.0;
const double JET_CvsL_LOOSE_ = 0.0;

//EVENT SELECTION
const int NUMBER_OF_LEPTONS_ = 1;
const int NUMBER_OF_JETS_ = 6;
const int NUMBER_OF_BJETS_ = 2;
const int NUMBER_OF_CJETS_ = 0;

//HISTOGRAM NAME
//  control plots
const char * RECO_LEPTON_PT_ = "LeptonPt";
const char * RECO_LEPTON_ETA_ = "LeptonEta";
const char * RECO_NUMBER_OF_JETS_ = "nJets";
const char * RECO_NUMBER_OF_BJETS_ = "nBjets";
const char * RECO_JET_PT_ = "JetPt";
const char * RECO_JET_ETA_ = "JetEta";
const char * RECO_TRANSVERSE_MASS_ = "TransverseMass";
const char * RECO_WMASS_ = "WMass";
const char * RECO_B_DISCRIMINATOR_ = "bJetDiscriminator";
//  add b jets plots
const char * RECO_ADDJETS_DELTAR_ = "RecoJetDeltaR";
const char * RECO_ADDJETS_INVARIANT_MASS_ = "RecoJetInvMass";
const char * RECO_ADDBJETS_DELTAR_ = "RecobJetDeltaR";
const char * RECO_ADDBJETS_INVARIANT_MASS_ = "RecobJetInvMass";
const char * RECO_TOTJETS_INVARIANT_MASS_ = "RecoTotJetInvMass";
const char * GEN_ADDJETS_DELTAR_ = "GenJetDeltaR";
const char * GEN_ADDJETS_INVARIANT_MASS_ = "GenJetInvMass";
const char * GEN_ADDBJETS_DELTAR_ = "GenbJetDeltaR";
const char * GEN_ADDBJETS_INVARIANT_MASS_ = "GenbJetInvMass";
const char * GEN_TOTJETS_INVARIANT_MASS_ = "GenTotJetInvMass";
const char * RESPONSE_MATRIX_DELTAR_ = "ResponseMatrixDeltaR";
const char * RESPONSE_MATRIX_INVARIANT_MASS_ = "ResponseMatrixInvMass";

const char * BIN_STABILITY_DELTAR_ = "BinStabilityDeltaR";
const char * BIN_STABILITY_INVARIANT_MASS_ = "BinStabilityInvMass";
const char * BIN_PURITY_DELTAR_ = "BinPurityDeltaR";
const char * BIN_PURITY_INVARIANT_MASS_ = "BinPurityInvMass";
const char * BIN_ACCEPTANCE_DELTAR_ = "BinAcceptanceDeltaR";
const char * BIN_ACCEPTANCE_INVARIANT_MASS_ = "BinAcceptanceInvMass";

const int nChannel=2; const int nStep=4; const int nJet = 6;

const std::vector<const char *> V_SYS_NAME_ = {
  "__puup", "__pudown",
  "__musfup", "__musfdown", "__mutrgup", "__mutrgdown",
  "__elsfup", "__elsfdown", "__eltrgup", "__eltrgdown",
  "__lfup", "__lfdown", "__hfup", "__hfdown",
  "__hfstat1up", "__hfstat1down", "__hfstat2up", "__hfstat2down",
  "__lfstat1up", "__lfstat1down", "__lfstat2up", "__lfstat2down",
  "__cferr1up", "__cferr1down", "__cferr2up", "__cferr2down",
  "__scale0", "__scale1", "__scale2", "__scale3", "__scale4", "__scale5",
  "__ps0", "__ps1", "__ps2", "__ps3"
};
const int nSys = 10;//V_SYS_NAME_.size();

class HistoBook{
  private:
    std::vector<string> v_chName = {"#mu","#it{e}"};
  public:
    const int xNbins_lepton_pt=20;
    const double lepton_pt_min=0; const double lepton_pt_max=400;
    //const double lepton_pt_width[xNbins_lepton_pt+1] = {};
    const int xNbins_lepton_eta=20;
    const double lepton_eta_min=0; const double lepton_eta_max=2.5;
    //const double lepton_eta_width[xNbins_lepton_eta+1] = {};

    const int xNbins_jet_pt=20;
    const double jet_pt_min=0; const double jet_pt_max=400;
    const int xNbins_jet_eta=20;
    const double jet_eta_min=0; const double jet_eta_max=2.5;

    const int xNbins_wmass=20;
    const double wmass_min=0; const double wmass_max=200;
    
    const int xNbins_b_discrimi=20;
    const double b_discrimi_min=0; const double b_discrimi_max=1;

    const int xNbins_njets=10;
    const double njets_min=0; const double njets_max=10;
    //const double njets_width[xNbins_njets+1] = {};
    const int xNbins_nbjets=10;
    const double nbjets_min=0; const double nbjets_max=10;
    //const double nbjets_width[xNbins_nbjets+1] = {};

    const int xNbins_reco_addjets_dR=12;
    const double reco_addjets_dR_min=0.4; const double reco_addjets_dR_max=4.0;
    //const double reco_addjets_dR_width[13] = 
    //{0.4,0.6,1.0,2.0,4.0};
    const int xNbins_reco_addjets_M=12;
    const double reco_addjets_M_min=0; double reco_addjets_M_max=400;
    //const double reco_addjets_M_width[13] =
    //{0.0,60.0,100.0,170.0,400.0};
    
    const int xNbins_gen_addbjets_dR=4;
    const double gen_addbjets_dR_min=0.4; const double gen_addbjets_dR_max=4.0;
    const double gen_addbjets_dR_width[5] =
    {0.4,0.6,1.0,2.0,4.0};
    const int xNbins_gen_addbjets_M=4;
    const double gen_addbjets_M_min=0; const double gen_addbjets_M_max=400;
    const double gen_addbjets_M_width[5] =
    {0.0,60.0,100.0,170.0,400.0};
    
    HistoBook(){};
    HistoBook(const int _mode, const char *_process);
    //~Histo_Book();
    
    //control plots : mode = 1
    TH1D *h_lepton_pt[nChannel][nStep];
    TH1D *h_lepton_eta[nChannel][nStep];
    TH1D *h_njets[nChannel][nStep];
    TH1D *h_nbjets[nChannel][nStep];
    TH1D *h_jet_pt_sum[nChannel][nStep];
    TH1D *h_jet_pt[nChannel][nStep][nJet];
    TH1D *h_jet_eta[nChannel][nStep][nJet];
    TH1D *h_trans_mass[nChannel][nStep];
    TH1D *h_b_discrimi[nChannel][nStep][nJet];
    TH1D *h_reco_addjets_deltaR[nChannel][nStep];
    TH1D *h_reco_addjets_invMass[nChannel][nStep];
    //response matrix : mode = 2
    TH2D *h_respMatrix_deltaR[nChannel][nStep];
    TH2D *h_respMatrix_invMass[nChannel][nStep];
    TH1D *h_gen_addbjets_deltaR_nosel[nChannel];
    TH1D *h_gen_addbjets_invMass_nosel[nChannel];
    TH1D *h_gen_addbjets_deltaR[nChannel][nStep];
    TH1D *h_gen_addbjets_invMass[nChannel][nStep];
    //etc : mode = 3
    TH1D *h_stability_deltaR[nChannel];
    TH1D *h_purity_deltaR[nChannel];
    TH1D *h_acceptance_deltaR[nChannel];
    TH1D *h_stability_invMass[nChannel];
    TH1D *h_purity_invMass[nChannel];
    TH1D *h_acceptance_invMass[nChannel];
};

HistoBook::HistoBook(const int _mode, const char *_process){
  if(_mode == 1){   
    for(int iChannel=0; iChannel<nChannel; ++iChannel){
      for(int iStep=0; iStep<nStep; ++iStep){
	h_lepton_pt[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d%s",RECO_LEPTON_PT_,iChannel,iStep,_process),"",
	    xNbins_lepton_pt,lepton_pt_min,lepton_pt_max //lepton_pt_width
	    );
	h_lepton_pt[iChannel][iStep]->SetXTitle(Form("%s p_{T} (GeV)",v_chName[iChannel].c_str()));
	h_lepton_pt[iChannel][iStep]->SetYTitle("Entries");
	h_lepton_pt[iChannel][iStep]->Sumw2();

	h_lepton_eta[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d%s",RECO_LEPTON_ETA_,iChannel,iStep,_process),"",
	    xNbins_lepton_eta,lepton_eta_min,lepton_eta_max //lepton_eta_width
	    );
	h_lepton_eta[iChannel][iStep]->SetXTitle(Form("%s #eta",v_chName[iChannel].c_str()));
	h_lepton_eta[iChannel][iStep]->SetYTitle("Entries");
	h_lepton_eta[iChannel][iStep]->Sumw2();

	h_njets[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d%s",RECO_NUMBER_OF_JETS_,iChannel,iStep,_process),"",
	    xNbins_njets,njets_min,njets_max //njets_width
	    );
	h_njets[iChannel][iStep]->SetXTitle("Jet multiplicity");
	h_njets[iChannel][iStep]->SetYTitle("Entries");
	h_njets[iChannel][iStep]->Sumw2();

	h_nbjets[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d%s",RECO_NUMBER_OF_BJETS_,iChannel,iStep,_process),"",
	    xNbins_nbjets,nbjets_min,nbjets_max //nbjets_width
	    );
	h_nbjets[iChannel][iStep]->SetXTitle("bJet multiplicity");
	h_nbjets[iChannel][iStep]->SetYTitle("Entries");
	h_nbjets[iChannel][iStep]->Sumw2();
	
	h_jet_pt_sum[iChannel][iStep] = new TH1D(Form("h_%s_sum_Ch%d_S%d%s",RECO_JET_PT_,iChannel,iStep,_process),"",
	    xNbins_jet_pt,jet_pt_min,jet_pt_max
	    );
	h_jet_pt_sum[iChannel][iStep]->SetXTitle("Sum of jet p_{T} (GeV)");
	h_jet_pt_sum[iChannel][iStep]->SetYTitle("Entries");
	h_jet_pt_sum[iChannel][iStep]->Sumw2();

	for(int iJet=0; iJet<nJet; ++iJet){
	  h_jet_pt[iChannel][iStep][iJet] = new TH1D(Form("h_%s_%d_Ch%d_S%d%s",RECO_JET_PT_,iJet,iChannel,iStep,_process),"",
	      xNbins_jet_pt,jet_pt_min,jet_pt_max
	      );
	  h_jet_pt[iChannel][iStep][iJet]->SetXTitle("Jet p_{T} (GeV)");
	  h_jet_pt[iChannel][iStep][iJet]->SetYTitle("Entries");
	  h_jet_pt[iChannel][iStep][iJet]->Sumw2();

	  h_jet_eta[iChannel][iStep][iJet] = new TH1D(Form("h_%s_%d_Ch%d_S%d%s",RECO_JET_ETA_,iJet,iChannel,iStep,_process),"",
	      xNbins_jet_eta,jet_eta_min,jet_eta_max
	      );
	  h_jet_eta[iChannel][iStep][iJet]->SetXTitle("Jet #eta");
	  h_jet_eta[iChannel][iStep][iJet]->SetYTitle("Entries");
	  h_jet_eta[iChannel][iStep][iJet]->Sumw2();

	  h_b_discrimi[iChannel][iStep][iJet] = new TH1D(Form("h_%s_%d_Ch%d_S%d%s",RECO_B_DISCRIMINATOR_,iJet,iChannel,iStep,_process),"",
	      xNbins_b_discrimi,b_discrimi_min,b_discrimi_max
	      );
	  h_b_discrimi[iChannel][iStep][iJet]->SetXTitle("b discriminator");
	  h_b_discrimi[iChannel][iStep][iJet]->SetYTitle("Entries");
	  h_b_discrimi[iChannel][iStep][iJet]->Sumw2();
	}

	h_trans_mass[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d%s",RECO_TRANSVERSE_MASS_,iChannel,iStep,_process),"",
	    xNbins_wmass,wmass_min,wmass_max
	    );
	h_trans_mass[iChannel][iStep]->SetXTitle("transverse mass (GeV)");
	h_trans_mass[iChannel][iStep]->SetYTitle("Entries");
	h_trans_mass[iChannel][iStep]->Sumw2();

	h_reco_addjets_deltaR[iChannel][iStep] = new TH1D(Form("h_mindR_%s_Ch%d_S%d%s",RECO_ADDJETS_DELTAR_,iChannel,iStep,_process),"",
	    xNbins_reco_addjets_dR,
	    reco_addjets_dR_min,reco_addjets_dR_max
	    //reco_addjets_dR_width
	    );
	h_reco_addjets_deltaR[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
	h_reco_addjets_deltaR[iChannel][iStep]->SetYTitle("Entries");
	h_reco_addjets_deltaR[iChannel][iStep]->Sumw2();

	h_reco_addjets_invMass[iChannel][iStep] = new TH1D(Form("h_mindR_%s_Ch%d_S%d%s",RECO_ADDJETS_INVARIANT_MASS_,iChannel,iStep,_process),"",
	    xNbins_reco_addjets_M,
	    reco_addjets_M_min,reco_addjets_M_max
	    //reco_addjets_M_width
	    );
	h_reco_addjets_invMass[iChannel][iStep]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
	h_reco_addjets_invMass[iChannel][iStep]->SetYTitle("Entries");
	h_reco_addjets_invMass[iChannel][iStep]->Sumw2();
      }//step
    }//channel
  }//mode
  else if(_mode == 2){
    for(int iChannel=0; iChannel<nChannel; ++iChannel){
      h_gen_addbjets_deltaR_nosel[iChannel] = new TH1D(Form("h_%s_Ch%d_nosel_%s",GEN_ADDBJETS_DELTAR_,iChannel,_process),"",
	  xNbins_gen_addbjets_dR,
	  //gen_addbjets_dR_min,gen_addbjets_dR_max 
	  gen_addbjets_dR_width
	  );
      h_gen_addbjets_deltaR_nosel[iChannel]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
      h_gen_addbjets_deltaR_nosel[iChannel]->SetYTitle("Entries");
      h_gen_addbjets_deltaR_nosel[iChannel]->Sumw2();

      h_gen_addbjets_invMass_nosel[iChannel] = new TH1D(Form("h_%s_Ch%d_nosel_%s",GEN_ADDBJETS_INVARIANT_MASS_,iChannel,_process),"",
	  xNbins_gen_addbjets_M,
	  //gen_addbjets_M_min,gen_addbjets_M_max
	  gen_addbjets_M_width
	  );
      h_gen_addbjets_invMass_nosel[iChannel]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
      h_gen_addbjets_invMass_nosel[iChannel]->SetYTitle("Entries");
      h_gen_addbjets_invMass_nosel[iChannel]->Sumw2();

      for(int iStep=0; iStep<nStep; ++iStep){
	h_gen_addbjets_deltaR[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",GEN_ADDBJETS_DELTAR_,iChannel,iStep,_process),"",
	    xNbins_gen_addbjets_dR,
	    //gen_addbjets_dR_min,gen_addbjets_dR_max
	    gen_addbjets_dR_width
	    );
	h_gen_addbjets_deltaR[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
	h_gen_addbjets_deltaR[iChannel][iStep]->SetYTitle("Entries");
	h_gen_addbjets_deltaR[iChannel][iStep]->Sumw2();

	h_gen_addbjets_invMass[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",GEN_ADDBJETS_INVARIANT_MASS_,iChannel,iStep,_process),"",
	    xNbins_gen_addbjets_M,
	    //gen_addbjets_M_min,gen_addbjets_M_max
	    gen_addbjets_M_width
	    );
	h_gen_addbjets_invMass[iChannel][iStep]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
	h_gen_addbjets_invMass[iChannel][iStep]->SetYTitle("Entries");
	h_gen_addbjets_invMass[iChannel][iStep]->Sumw2();

	h_respMatrix_deltaR[iChannel][iStep] = new TH2D(Form("h_%s_Ch%d_S%d_%s",RESPONSE_MATRIX_DELTAR_,iChannel,iStep,_process),"",
	    xNbins_reco_addjets_dR,
	    reco_addjets_dR_min, reco_addjets_dR_max,
	    //reco_addjets_dR_width,
	    xNbins_gen_addbjets_dR,
	    //gen_addbjets_dR_min, gen_addbjets_dR_max
	    gen_addbjets_dR_width
	    	    );
	h_respMatrix_deltaR[iChannel][iStep]->SetYTitle("Gen. #DeltaR_{b#bar{b}}");
	h_respMatrix_deltaR[iChannel][iStep]->SetXTitle("Reco. #DeltaR_{b#bar{b}}");
	h_respMatrix_deltaR[iChannel][iStep]->Sumw2();

        h_respMatrix_invMass[iChannel][iStep] = new TH2D(Form("h_%s_Ch%d_S%d_%s",RESPONSE_MATRIX_INVARIANT_MASS_,iChannel,iStep,_process),"",
	    xNbins_reco_addjets_M,
	    reco_addjets_M_min, reco_addjets_M_max,
	    //reco_addjets_M_width,
	    xNbins_gen_addbjets_M,
	    //gen_addbjets_M_min, gen_addbjets_M_max
	    gen_addbjets_M_width
	    );
	h_respMatrix_invMass[iChannel][iStep]->SetYTitle("Gen. M_{b#bar{b}}(GeV)");
	h_respMatrix_invMass[iChannel][iStep]->SetXTitle("Reco. M_{b#bar{b}}(GeV)");
	h_respMatrix_invMass[iChannel][iStep]->Sumw2();
      }//step
    }//channel
  }//mode
  else if(_mode == 3){
    for(int iChannel=0; iChannel<nChannel; ++iChannel){
      h_acceptance_deltaR[iChannel] = new TH1D(Form("h_%s_Ch%d%s",BIN_ACCEPTANCE_DELTAR_,iChannel,_process),"",
	  xNbins_gen_addbjets_dR,
	  //gen_addbjets_dR_min, gen_addbjets_dR_max
	  gen_addbjets_dR_width
	  );
      h_acceptance_deltaR[iChannel]->SetXTitle("#DeltaR_{b#bar{b}}");
      h_acceptance_deltaR[iChannel]->SetYTitle("acceptance");
      h_acceptance_deltaR[iChannel]->Sumw2();
      
      h_acceptance_invMass[iChannel] = new TH1D(Form("h_%s_Ch%d%s",BIN_ACCEPTANCE_INVARIANT_MASS_,iChannel,_process),"",
	  xNbins_gen_addbjets_M,
	  //gen_addbjets_M_min, gen_addbjets_M_max
	  gen_addbjets_M_width
	  );
      h_acceptance_invMass[iChannel]->SetXTitle("M_{b#bar{b}}(GeV)");
      h_acceptance_invMass[iChannel]->SetYTitle("acceptance");
      h_acceptance_invMass[iChannel]->Sumw2();
	
      h_stability_deltaR[iChannel] = new TH1D(Form("h_%s_Ch%d_S3%s",BIN_STABILITY_DELTAR_,iChannel,_process),"",
	  xNbins_gen_addbjets_dR,
	  //gen_addbjets_dR_min, gen_addbjets_dR_max
	  gen_addbjets_dR_width
	  );
      h_stability_deltaR[iChannel]->SetXTitle("#DeltaR_{b#bar{b}}");
      h_stability_deltaR[iChannel]->SetYTitle("stability");
      h_stability_deltaR[iChannel]->Sumw2();

      h_purity_deltaR[iChannel] = new TH1D(Form("h_%s_Ch%d_S3_%s",BIN_PURITY_DELTAR_,iChannel,_process),"",
	  xNbins_gen_addbjets_dR,
	  //gen_addbjets_dR_min, gen_addbjets_dR_max
	  gen_addbjets_dR_width
	  );
      h_purity_deltaR[iChannel]->SetXTitle("#DeltaR_{b#bar{b}}");
      h_purity_deltaR[iChannel]->SetYTitle("purity");
      h_purity_deltaR[iChannel]->Sumw2();

      h_stability_invMass[iChannel] = new TH1D(Form("h_%s_Ch%d_S3%s",BIN_STABILITY_INVARIANT_MASS_,iChannel,_process),"",
	  xNbins_gen_addbjets_M,
	  //gen_addbjets_M_min, gen_addbjets_M_max
	  gen_addbjets_M_width
	  );
      h_stability_invMass[iChannel]->SetXTitle("M_{b#bar{b}}(GeV)");
      h_stability_invMass[iChannel]->SetYTitle("stability");
      h_stability_invMass[iChannel]->Sumw2();

      h_purity_invMass[iChannel] = new TH1D(Form("h_%s_Ch%d_S3%s",BIN_PURITY_INVARIANT_MASS_,iChannel,_process),"",
	  xNbins_gen_addbjets_M,
	  //gen_addbjets_M_min, gen_addbjets_M_max
	  gen_addbjets_M_width
	  );
      h_purity_invMass[iChannel]->SetXTitle("M_{b#bar{b}}(GeV)");
      h_purity_invMass[iChannel]->SetYTitle("purity");
      h_purity_invMass[iChannel]->Sumw2();
    }//channel
  }//mode
  else
    cout << "Warning : There is no such mode" << endl;
}

//void Histo_Book::SetDataHist(TH1 *data){
void SetDataHist(TH1 *data, bool norm){
  data->SetStats(0);
  data->SetLineColor(kBlack);
  data->SetMarkerStyle(20);
  data->SetMarkerSize(2);
  data->SetMarkerColor(kBlack);
  if(norm)
    data->Scale(1/data->GetEntries());
}

//void Histo_Book::SetMCLineHist(TH1 *mc, int color){
void SetMCLineHist(TH1 *mc, int color, bool norm){
  mc->SetStats(0);
  mc->SetLineWidth(2);
  mc->SetLineColor(color);
  if(norm)
    mc->Scale(1/mc->GetEntries());
  else
    mc->Scale(1);
}

void SetMCFillHist(TH1 *mc, int color, bool norm){
  mc->SetStats(0);
  mc->SetLineColor(kBlack);
  mc->SetFillColor(color);
  if(norm)
    mc->Scale(1/mc->GetEntries());
  else
    mc->Scale(1);
}
