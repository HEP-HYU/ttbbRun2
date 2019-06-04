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

std::vector<const char *> V_SYS_ = {
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
  6225.42, //6025.2, //zjets
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
const int MUON_        = 0;
const double MUON_PT_  = 30.0;
const double MUON_ETA_ = 2.1;
const double MUON_PHI_ = 0.0;
const double MUON_E_   = 0.0;

const int ELECTRON_        = 1;
const double ELECTRON_PT_  = 35.0;
const double ELECTRON_ETA_ = 2.1;
const double ELECTRON_PHI_ = 0.0;
const double ELECTRON_E_   = 0.0;

const double JET_PT_  = 30.0;
const double JET_ETA_ = 2.4;
const double JET_PHI_ = 0.0;
const double JET_E_   = 0.0;

const double JET_CSV_TIGHT_  = 0.9535;
const double JET_CSV_MEDIUM_ = 0.8484;
const double JET_CSV_LOOSE_  = 0.0;

const double JET_CvsL_TIGHT_  = 0.0;
const double JET_CvsL_MEDIUM_ = 0.0;
const double JET_CvsL_LOOSE_  = 0.0;

//EVENT SELECTION
const int NUMBER_OF_LEPTONS_ = 1;
const int NUMBER_OF_JETS_    = 6;
const int NUMBER_OF_BJETS_   = 2;
const int NUMBER_OF_CJETS_   = 0;

//HISTOGRAM NAME
//  Control plots
const char * RECO_LEP_PT_  = "LeptonPt";
const char * RECO_LEP_ETA_ = "LeptonEta";
const char * RECO_N_JETS_  = "nJets";
const char * RECO_N_BJETS_ = "nbJets";
const char * RECO_JET_PT_  = "JetPt";
const char * RECO_JET_ETA_ = "JetEta";
const char * RECO_TRANS_M_ = "TransverseMass";
const char * RECO_CSV_     = "bJetDiscriminator";

//  Add b jets plots
const char * RECO_ADD_DR_   = "RecoAddbJetDeltaR";
const char * RECO_ADD_M_    = "RecoAddbJetInvMass";
const char * RECO_ADD_DETA_ = "RecoAddbJetDeltaEta";
const char * RECO_ADD_DPHI_ = "RecoAddbJetDeltaPhi";

const char * GEN_ADD_DR_    = "GenAddbJetDeltaR";
const char * GEN_ADD_M_     = "GenAddbJetInvMass";
const char * GEN_ADD_DETA_  = "GenAddbJetDeltaEta";
const char * GEN_ADD_DPHI_  = "GenAddbJetDeltaPhi";

const char * MATRIX_DR_     = "ResponseMatrixDeltaR";
const char * MATRIX_M_      = "ResponseMatrixInvMass";
const char * MATRIX_DETA_   = "ResponseMatrixDeltaEta";
const char * MATRIX_DPHI_   = "ResponseMatrixDeltaPhi";

//  Criteria
const char * STB_DR_   = "BinStabilityDeltaR";
const char * STB_M_    = "BinStabilityInvMass";
const char * STB_DETA_ = "BinStabilityDeltaEta";
const char * STB_DPHI_ = "BinStabilityDeltaPhi";

const char * PUR_DR_   = "BinPurityDeltaR";
const char * PUR_M_    = "BinPurityInvMass";
const char * PUR_DETA_ = "BinPurityDeltaEta";
const char * PUR_DPHI_ = "BinPurityDeltaPhi";

const char * ACC_DR_   = "BinAcceptanceDeltaR";
const char * ACC_M_    = "BinAcceptanceInvMass";
const char * ACC_DETA_ = "BinAcceptanceDeltaEta";
const char * ACC_DPHI_ = "BinAcceptanceDeltaPhi";

const int nChannel=3; const int nStep=4; const int nJet = 6;

//Bin width
const int nbins_lep_pt=20;
const double lep_pt_min=0; const double lep_pt_max=400;
//const double lep_pt_width[xNbins_lepton_pt+1] = {};
const int nbins_lep_eta=20;
const double lep_eta_min=0; const double lep_eta_max=2.5;
//const double lep_eta_width[xNbins_lepton_eta+1] = {};

const int nbins_jet_pt=20;
const double jet_pt_min=0; const double jet_pt_max=400;
const int nbins_jet_eta=20;
const double jet_eta_min=0; const double jet_eta_max=2.5;

const int nbins_wmass=20;
const double wmass_min=0; const double wmass_max=200;

const int nbins_csv=20;
const double csv_min=0; const double csv_max=1;

const int nbins_njets=10;
const double njets_min=0; const double njets_max=10;
//const double njets_width[xNbins_njets+1] = {};
const int nbins_nbjets=10;
const double nbjets_min=0; const double nbjets_max=10;
//const double nbjets_width[xNbins_nbjets+1] = {};

const int nbins_reco_addbjets_dR=12;
const double reco_addbjets_dR_min=0.4; const double reco_addbjets_dR_max=4.0;
const double reco_addbjets_dR_width[13] = 
{0.4,0.55,0.7,0.85,1.0,1.15,1.3,1.45,1.6,1.8,2.0,3.0,4.0};
const int nbins_gen_addbjets_dR=6;
const double gen_addbjets_dR_min=0.4; const double gen_addbjets_dR_max=4.0;
const double gen_addbjets_dR_width[7] =
{0.4,0.7,1.0,1.3,1.6,2.0,4.0};

const int nbins_reco_addbjets_M=8;
const double reco_addbjets_M_min=0; double reco_addbjets_M_max=400;
const double reco_addbjets_M_width[9] =
{0.0,30.0,60.0,75.0,90.0,135.0,180.0,290.0,400.0};
//{0.0,30.0,60.0,90.0,120.0,180.0,400.0};
const int nbins_gen_addbjets_M=4;
const double gen_addbjets_M_min=0; const double gen_addbjets_M_max=400;
const double gen_addbjets_M_width[5] =
{0.0,60.0,90.0,180.0,400.0};
//{0.0,30.0,60.0,90.0,120.0,180.0,400.0};

const int nbins_reco_addbjets_dEta=12;
const double reco_addbjets_dEta_min=0; const double reco_addbjets_dEta_max=2.4;
//const double reco_addbjets_dEta_width[]={};
const int nbins_gen_addbjets_dEta=6;
const double gen_addbjets_dEta_min=0; const double gen_addbjets_dEta_max = 2.4;
//const double gen_addbjets_dEta_width[]={};

const int nbins_reco_addbjets_dPhi=12;
const double reco_addbjets_dPhi_min=0; const double reco_addbjets_dPhi_max=3.14;
//const double reco_addbjets_dPhi_width[]={};
const int nbins_gen_addbjets_dPhi=6;
const double gen_addbjets_dPhi_min=0; const double gen_addbjets_dPhi_max=3.14;
//const double gen_addbjets_dPhi_width[]={};


class HistoBook{
  private:
    std::vector<string> v_chName = {"#mu","#it{e}","lep"};
  public:
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
    TH1D *h_csv[nChannel][nStep][nJet];
    TH1D *h_reco_addbjets_deltaR[nChannel][nStep];
    TH1D *h_reco_addbjets_invMass[nChannel][nStep];
    TH1D *h_reco_addbjets_deltaEta[nChannel][nStep];
    TH1D *h_reco_addbjets_deltaPhi[nChannel][nStep];
    //response matrix : mode = 2
    TH1D *h_gen_gentop_deltaR[nChannel][nStep];
    TH1D *h_gen_gentop_invMass[nChannel][nStep];
    TH1D *h_gen_gentop_deltaEta[nChannel][nStep];
    TH1D *h_gen_gentop_deltaPhi[nChannel][nStep];
    TH1D *h_gen_mindR_deltaR[nChannel][nStep];
    TH1D *h_gen_mindR_invMass[nChannel][nStep];
    TH1D *h_gen_mindR_deltaEta[nChannel][nStep];
    TH1D *h_gen_mindR_deltaPhi[nChannel][nStep];
    TH2D *h_respMatrix_gentop_deltaR[nChannel][nStep];
    TH2D *h_respMatrix_gentop_invMass[nChannel][nStep];
    TH2D *h_respMatrix_gentop_deltaEta[nChannel][nStep];
    TH2D *h_respMatrix_gentop_deltaPhi[nChannel][nStep];
    TH2D *h_respMatrix_mindR_deltaR[nChannel][nStep];
    TH2D *h_respMatrix_mindR_invMass[nChannel][nStep];
    TH2D *h_respMatrix_mindR_deltaEta[nChannel][nStep];
    TH2D *h_respMatrix_mindR_deltaPhi[nChannel][nStep];
    //etc : mode = 3
    TH1D *h_stability_deltaR[nChannel];
    TH1D *h_stability_invMass[nChannel];
    TH1D *h_stability_deltaEta[nChannel];
    TH1D *h_stability_deltaPhi[nChannel];
    TH1D *h_purity_deltaR[nChannel];
    TH1D *h_purity_invMass[nChannel];
    TH1D *h_purity_deltaEta[nChannel];
    TH1D *h_purity_deltaPhi[nChannel];
    TH1D *h_acceptance_deltaR[nChannel];
    TH1D *h_acceptance_invMass[nChannel];
    TH1D *h_acceptance_deltaEta[nChannel];
    TH1D *h_acceptance_deltaPhi[nChannel];
};

HistoBook::HistoBook(const int _mode, const char *_process){
  if(_mode == 1){   
    for(int iChannel=0; iChannel<nChannel; ++iChannel){
      for(int iStep=0; iStep<nStep; ++iStep){
	h_lepton_pt[iChannel][iStep] = new TH1D(
	    Form("h_%s_Ch%d_S%d%s",RECO_LEP_PT_,iChannel,iStep,_process),"",
	    nbins_lep_pt,
	    lep_pt_min,lep_pt_max
	    //lep_pt_width
	    );
	h_lepton_pt[iChannel][iStep]->SetXTitle(Form("%s p_{T} (GeV)",v_chName[iChannel].c_str()));
	h_lepton_pt[iChannel][iStep]->SetYTitle("Entries");
	h_lepton_pt[iChannel][iStep]->Sumw2();

	h_lepton_eta[iChannel][iStep] = new TH1D(
	    Form("h_%s_Ch%d_S%d%s",RECO_LEP_ETA_,iChannel,iStep,_process),"",
	    nbins_lep_eta,
	    lep_eta_min,lep_eta_max
	    //lep_eta_width
	    );
	h_lepton_eta[iChannel][iStep]->SetXTitle(Form("%s #eta",v_chName[iChannel].c_str()));
	h_lepton_eta[iChannel][iStep]->SetYTitle("Entries");
	h_lepton_eta[iChannel][iStep]->Sumw2();

	h_njets[iChannel][iStep] = new TH1D(
	    Form("h_%s_Ch%d_S%d%s",RECO_N_JETS_,iChannel,iStep,_process),"",
	    nbins_njets,
	    njets_min,njets_max
	    //njets_width
	    );
	h_njets[iChannel][iStep]->SetXTitle("Jet multiplicity");
	h_njets[iChannel][iStep]->SetYTitle("Entries");
	h_njets[iChannel][iStep]->Sumw2();

	h_nbjets[iChannel][iStep] = new TH1D(
	    Form("h_%s_Ch%d_S%d%s",RECO_N_BJETS_,iChannel,iStep,_process),"",
	    nbins_nbjets,
	    nbjets_min,nbjets_max
	    //nbjets_width
	    );
	h_nbjets[iChannel][iStep]->SetXTitle("bJet multiplicity");
	h_nbjets[iChannel][iStep]->SetYTitle("Entries");
	h_nbjets[iChannel][iStep]->Sumw2();
	
	h_jet_pt_sum[iChannel][iStep] = new TH1D(
	    Form("h_%s_sum_Ch%d_S%d%s",RECO_JET_PT_,iChannel,iStep,_process),"",
	    nbins_jet_pt,
	    jet_pt_min,jet_pt_max
	    );
	h_jet_pt_sum[iChannel][iStep]->SetXTitle("Sum of jet p_{T} (GeV)");
	h_jet_pt_sum[iChannel][iStep]->SetYTitle("Entries");
	h_jet_pt_sum[iChannel][iStep]->Sumw2();

	for(int iJet=0; iJet<nJet; ++iJet){
	  h_jet_pt[iChannel][iStep][iJet] = new TH1D(
	      Form("h_%s_%d_Ch%d_S%d%s",RECO_JET_PT_,iJet,iChannel,iStep,_process),"",
	      nbins_jet_pt,
	      jet_pt_min,jet_pt_max
	      );
	  h_jet_pt[iChannel][iStep][iJet]->SetXTitle("Jet p_{T} (GeV)");
	  h_jet_pt[iChannel][iStep][iJet]->SetYTitle("Entries");
	  h_jet_pt[iChannel][iStep][iJet]->Sumw2();

	  h_jet_eta[iChannel][iStep][iJet] = new TH1D(
	      Form("h_%s_%d_Ch%d_S%d%s",RECO_JET_ETA_,iJet,iChannel,iStep,_process),"",
	      nbins_jet_eta,
	      jet_eta_min,jet_eta_max
	      );
	  h_jet_eta[iChannel][iStep][iJet]->SetXTitle("Jet #eta");
	  h_jet_eta[iChannel][iStep][iJet]->SetYTitle("Entries");
	  h_jet_eta[iChannel][iStep][iJet]->Sumw2();

	  h_csv[iChannel][iStep][iJet] = new TH1D(
	      Form("h_%s_%d_Ch%d_S%d%s",RECO_CSV_,iJet,iChannel,iStep,_process),"",
	      nbins_csv,
	      csv_min,csv_max
	      );
	  h_csv[iChannel][iStep][iJet]->SetXTitle("b discriminator");
	  h_csv[iChannel][iStep][iJet]->SetYTitle("Entries");
	  h_csv[iChannel][iStep][iJet]->Sumw2();
	}

	h_trans_mass[iChannel][iStep] = new TH1D(
	    Form("h_%s_Ch%d_S%d%s",RECO_TRANS_M_,iChannel,iStep,_process),"",
	    nbins_wmass,
	    wmass_min,wmass_max
	    );
	h_trans_mass[iChannel][iStep]->SetXTitle("transverse mass (GeV)");
	h_trans_mass[iChannel][iStep]->SetYTitle("Entries");
	h_trans_mass[iChannel][iStep]->Sumw2();

	h_reco_addbjets_deltaR[iChannel][iStep] = new TH1D(
	    Form("h_mindR_%s_Ch%d_S%d%s",RECO_ADD_DR_,iChannel,iStep,_process),"",
	    nbins_reco_addbjets_dR,
	    //reco_addbjets_dR_min,reco_addbjets_dR_max
	    reco_addbjets_dR_width
	    );
	h_reco_addbjets_deltaR[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
	h_reco_addbjets_deltaR[iChannel][iStep]->SetYTitle("Entries");
	h_reco_addbjets_deltaR[iChannel][iStep]->Sumw2();

	h_reco_addbjets_invMass[iChannel][iStep] = new TH1D(
	    Form("h_mindR_%s_Ch%d_S%d%s",RECO_ADD_M_,iChannel,iStep,_process),"",
	    nbins_reco_addbjets_M,
	    //reco_addbjets_M_min,reco_addbjets_M_max
	    reco_addbjets_M_width
	    );
	h_reco_addbjets_invMass[iChannel][iStep]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
	h_reco_addbjets_invMass[iChannel][iStep]->SetYTitle("Entries");
	h_reco_addbjets_invMass[iChannel][iStep]->Sumw2();
	
	h_reco_addbjets_deltaEta[iChannel][iStep] = new TH1D(
	    Form("h_mindR_%s_Ch%d_S%d%s",RECO_ADD_DETA_,iChannel,iStep,_process),"",
	    nbins_reco_addbjets_dEta,
	    reco_addbjets_dEta_min,reco_addbjets_dEta_max
	    //reco_addbjets_dEta_width
	    );
	h_reco_addbjets_deltaEta[iChannel][iStep]->SetXTitle(Form("#Delta#eta_{b#bar{b}}"));
	h_reco_addbjets_deltaEta[iChannel][iStep]->SetYTitle("Entries");
	h_reco_addbjets_deltaEta[iChannel][iStep]->Sumw2();

	h_reco_addbjets_deltaPhi[iChannel][iStep] = new TH1D(
	    Form("h_mindR_%s_Ch%d_S%d%s",RECO_ADD_DPHI_,iChannel,iStep,_process),"",
	    nbins_reco_addbjets_dPhi,
	    reco_addbjets_dPhi_min,reco_addbjets_dPhi_max
	    //reco_addbjets_dPhi_width
	    );
	h_reco_addbjets_deltaPhi[iChannel][iStep]->SetXTitle(Form("#Delta#phi_{b#bar{b}}"));
	h_reco_addbjets_deltaPhi[iChannel][iStep]->SetYTitle("Entries");
	h_reco_addbjets_deltaPhi[iChannel][iStep]->Sumw2();
      }//step
    }//channel
  }//mode
  else if(_mode == 2){
    std::cout << "Make mode2" << std::endl;
    for(int iChannel=0; iChannel<nChannel; ++iChannel){
      for(int iStep=0; iStep<nStep; ++iStep){
	h_gen_gentop_deltaR[iChannel][iStep] = new TH1D(
	    Form("h_gentop_%s_Ch%d_S%d%s",GEN_ADD_DR_,iChannel,iStep,_process),"",
	    nbins_gen_addbjets_dR,
	    //gen_addbjets_dR_min,gen_addbjets_dR_max
	    gen_addbjets_dR_width
	    );
	h_gen_gentop_deltaR[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
	h_gen_gentop_deltaR[iChannel][iStep]->SetYTitle("Entries");
	h_gen_gentop_deltaR[iChannel][iStep]->Sumw2();

	h_gen_gentop_invMass[iChannel][iStep] = new TH1D(
	    Form("h_gentop_%s_Ch%d_S%d%s",GEN_ADD_M_,iChannel,iStep,_process),"",
	    nbins_gen_addbjets_M,
	    //gen_addbjets_M_min,gen_addbjets_M_max
	    gen_addbjets_M_width
	    );
	h_gen_gentop_invMass[iChannel][iStep]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
	h_gen_gentop_invMass[iChannel][iStep]->SetYTitle("Entries");
	h_gen_gentop_invMass[iChannel][iStep]->Sumw2();

	h_gen_gentop_deltaEta[iChannel][iStep] = new TH1D(
	    Form("h_gentop_%s_Ch%d_S%d%s",GEN_ADD_DETA_,iChannel,iStep,_process),"",
	    nbins_gen_addbjets_dEta,
	    gen_addbjets_dEta_min,gen_addbjets_dEta_max
	    //gen_addbjets_dEta_width
	    );
	h_gen_gentop_deltaEta[iChannel][iStep]->SetXTitle(Form("#Delta#eta_{b#bar{b}}(GeV)"));
	h_gen_gentop_deltaEta[iChannel][iStep]->SetYTitle("Entries");
	h_gen_gentop_deltaEta[iChannel][iStep]->Sumw2();

	h_gen_gentop_deltaPhi[iChannel][iStep] = new TH1D(
	    Form("h_gentop_%s_Ch%d_S%d%s",GEN_ADD_DPHI_,iChannel,iStep,_process),"",
	    nbins_gen_addbjets_dPhi,
	    gen_addbjets_dPhi_min,gen_addbjets_dPhi_max
	    //gen_addbjets_dPhi_width
	    );
	h_gen_gentop_deltaPhi[iChannel][iStep]->SetXTitle(Form("#Delta#phi_{b#bar{b}}(GeV)"));
	h_gen_gentop_deltaPhi[iChannel][iStep]->SetYTitle("Entries");
	h_gen_gentop_deltaPhi[iChannel][iStep]->Sumw2();
	
	h_gen_mindR_deltaR[iChannel][iStep] = new TH1D(
	    Form("h_mindR_%s_Ch%d_S%d%s",GEN_ADD_DR_,iChannel,iStep,_process),"",
	    nbins_gen_addbjets_dR,
	    gen_addbjets_dR_min,gen_addbjets_dR_max
	    //gen_addbjets_dR_width
	    );
	h_gen_mindR_deltaR[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
	h_gen_mindR_deltaR[iChannel][iStep]->SetYTitle("Entries");
	h_gen_mindR_deltaR[iChannel][iStep]->Sumw2();

	h_gen_mindR_invMass[iChannel][iStep] = new TH1D(
	    Form("h_mindR_%s_Ch%d_S%d%s",GEN_ADD_M_,iChannel,iStep,_process),"",
	    nbins_gen_addbjets_M,
	    //gen_addbjets_M_min,gen_addbjets_M_max
	    gen_addbjets_M_width
	    );
	h_gen_mindR_invMass[iChannel][iStep]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
	h_gen_mindR_invMass[iChannel][iStep]->SetYTitle("Entries");
	h_gen_mindR_invMass[iChannel][iStep]->Sumw2();

	h_gen_mindR_deltaEta[iChannel][iStep] = new TH1D(
	    Form("h_mindR_%s_Ch%d_S%d%s",GEN_ADD_DETA_,iChannel,iStep,_process),"",
	    nbins_gen_addbjets_dEta,
	    gen_addbjets_dEta_min,gen_addbjets_dEta_max
	    //gen_addbjets_dEta_width
	    );
	h_gen_mindR_deltaEta[iChannel][iStep]->SetXTitle(Form("#Delta#eta_{b#bar{b}}(GeV)"));
	h_gen_mindR_deltaEta[iChannel][iStep]->SetYTitle("Entries");
	h_gen_mindR_deltaEta[iChannel][iStep]->Sumw2();

	h_gen_mindR_deltaPhi[iChannel][iStep] = new TH1D(
	    Form("h_mindR_%s_Ch%d_S%d%s",GEN_ADD_DPHI_,iChannel,iStep,_process),"",
	    nbins_gen_addbjets_dPhi,
	    gen_addbjets_dPhi_min,gen_addbjets_dPhi_max
	    //gen_addbjets_dPhi_width
	    );
	h_gen_mindR_deltaPhi[iChannel][iStep]->SetXTitle(Form("#Delta#phi_{b#bar{b}}(GeV)"));
	h_gen_mindR_deltaPhi[iChannel][iStep]->SetYTitle("Entries");
	h_gen_mindR_deltaPhi[iChannel][iStep]->Sumw2();
	
	h_respMatrix_gentop_deltaR[iChannel][iStep] = new TH2D(
	    Form("h_gentop_%s_Ch%d_S%d%s",MATRIX_DR_,iChannel,iStep,_process),"",
	    nbins_reco_addbjets_dR,
	    //reco_addbjets_dR_min, reco_addbjets_dR_max,
	    reco_addbjets_dR_width,
	    nbins_gen_addbjets_dR,
	    //gen_addbjets_dR_min, gen_addbjets_dR_max
	    gen_addbjets_dR_width
	    	    );
	h_respMatrix_gentop_deltaR[iChannel][iStep]->SetYTitle("Gen. #DeltaR_{b#bar{b}}");
	h_respMatrix_gentop_deltaR[iChannel][iStep]->SetXTitle("Reco. #DeltaR_{b#bar{b}}");
	h_respMatrix_gentop_deltaR[iChannel][iStep]->Sumw2();

        h_respMatrix_gentop_invMass[iChannel][iStep] = new TH2D(
	    Form("h_gentop_%s_Ch%d_S%d%s",MATRIX_M_,iChannel,iStep,_process),"",
	    nbins_reco_addbjets_M,
	    //reco_addbjets_M_min, reco_addbjets_M_max,
	    reco_addbjets_M_width,
	    nbins_gen_addbjets_M,
	    //gen_addbjets_M_min, gen_addbjets_M_max
	    gen_addbjets_M_width
	    );
	h_respMatrix_gentop_invMass[iChannel][iStep]->SetYTitle("Gen. M_{b#bar{b}}(GeV)");
	h_respMatrix_gentop_invMass[iChannel][iStep]->SetXTitle("Reco. M_{b#bar{b}}(GeV)");
	h_respMatrix_gentop_invMass[iChannel][iStep]->Sumw2();

        h_respMatrix_gentop_deltaEta[iChannel][iStep] = new TH2D(
	    Form("h_gentop_%s_Ch%d_S%d%s",MATRIX_DETA_,iChannel,iStep,_process),"",
	    nbins_reco_addbjets_dEta,
	    reco_addbjets_dEta_min, reco_addbjets_dEta_max,
	    //reco_addbjets_dEta_width,
	    nbins_gen_addbjets_dEta,
	    gen_addbjets_dEta_min, gen_addbjets_dEta_max
	    //gen_addbjets_dEta_width
	    );
	h_respMatrix_gentop_deltaEta[iChannel][iStep]->SetYTitle("Gen. #Delta#eta_{b#bar{b}}");
	h_respMatrix_gentop_deltaEta[iChannel][iStep]->SetXTitle("Reco. #Delta#eta_{b#bar{b}}");
	h_respMatrix_gentop_deltaEta[iChannel][iStep]->Sumw2();

        h_respMatrix_gentop_deltaPhi[iChannel][iStep] = new TH2D(
	    Form("h_gentop_%s_Ch%d_S%d%s",MATRIX_DPHI_,iChannel,iStep,_process),"",
	    nbins_reco_addbjets_dPhi,
	    reco_addbjets_dPhi_min, reco_addbjets_dPhi_max,
	    //reco_addbjets_dPhi_width,
	    nbins_gen_addbjets_dPhi,
	    gen_addbjets_dPhi_min, gen_addbjets_dPhi_max
	    //gen_addbjets_dPhi_width
	    );
	h_respMatrix_gentop_deltaPhi[iChannel][iStep]->SetYTitle("Gen. #Delta#phi_{b#bar{b}}");
	h_respMatrix_gentop_deltaPhi[iChannel][iStep]->SetXTitle("Reco. #Delta#phi_{b#bar{b}}");
	h_respMatrix_gentop_deltaPhi[iChannel][iStep]->Sumw2();
	
	h_respMatrix_mindR_deltaR[iChannel][iStep] = new TH2D(
	    Form("h_mindR_%s_Ch%d_S%d%s",MATRIX_DR_,iChannel,iStep,_process),"",
	    nbins_reco_addbjets_dR,
	    //reco_addbjets_dR_min, reco_addbjets_dR_max,
	    reco_addbjets_dR_width,
	    nbins_gen_addbjets_dR,
	    //gen_addbjets_dR_min, gen_addbjets_dR_max
	    gen_addbjets_dR_width
	    	    );
	h_respMatrix_mindR_deltaR[iChannel][iStep]->SetYTitle("Gen. #DeltaR_{b#bar{b}}");
	h_respMatrix_mindR_deltaR[iChannel][iStep]->SetXTitle("Reco. #DeltaR_{b#bar{b}}");
	h_respMatrix_mindR_deltaR[iChannel][iStep]->Sumw2();

        h_respMatrix_mindR_invMass[iChannel][iStep] = new TH2D(
	    Form("h_mindR_%s_Ch%d_S%d%s",MATRIX_M_,iChannel,iStep,_process),"",
	    nbins_reco_addbjets_M,
	    //reco_addbjets_M_min, reco_addbjets_M_max,
	    reco_addbjets_M_width,
	    nbins_gen_addbjets_M,
	    //gen_addbjets_M_min, gen_addbjets_M_max
	    gen_addbjets_M_width
	    );
	h_respMatrix_mindR_invMass[iChannel][iStep]->SetYTitle("Gen. M_{b#bar{b}}(GeV)");
	h_respMatrix_mindR_invMass[iChannel][iStep]->SetXTitle("Reco. M_{b#bar{b}}(GeV)");
	h_respMatrix_mindR_invMass[iChannel][iStep]->Sumw2();

        h_respMatrix_mindR_deltaEta[iChannel][iStep] = new TH2D(
	    Form("h_mindR_%s_Ch%d_S%d%s",MATRIX_DETA_,iChannel,iStep,_process),"",
	    nbins_reco_addbjets_dEta,
	    reco_addbjets_dEta_min, reco_addbjets_dEta_max,
	    //reco_addbjets_dEta_width,
	    nbins_gen_addbjets_dEta,
	    gen_addbjets_dEta_min, gen_addbjets_dEta_max
	    //gen_addbjets_dEta_width
	    );
	h_respMatrix_mindR_deltaEta[iChannel][iStep]->SetYTitle("Gen. #Delta#eta_{b#bar{b}}");
	h_respMatrix_mindR_deltaEta[iChannel][iStep]->SetXTitle("Reco. #Delta#eta_{b#bar{b}}");
	h_respMatrix_mindR_deltaEta[iChannel][iStep]->Sumw2();

        h_respMatrix_mindR_deltaPhi[iChannel][iStep] = new TH2D(
	    Form("h_mindR_%s_Ch%d_S%d%s",MATRIX_DPHI_,iChannel,iStep,_process),"",
	    nbins_reco_addbjets_dPhi,
	    reco_addbjets_dPhi_min, reco_addbjets_dPhi_max,
	    //reco_addbjets_dPhi_width,
	    nbins_gen_addbjets_dPhi,
	    gen_addbjets_dPhi_min, gen_addbjets_dPhi_max
	    //gen_addbjets_dPhi_width
	    );
	h_respMatrix_mindR_deltaPhi[iChannel][iStep]->SetYTitle("Gen. #Delta#phi_{b#bar{b}}");
	h_respMatrix_mindR_deltaPhi[iChannel][iStep]->SetXTitle("Reco. #Delta#phi_{b#bar{b}}");
	h_respMatrix_mindR_deltaPhi[iChannel][iStep]->Sumw2();
      }//step
    }//channel
  }//mode
  else if(_mode == 3){
    for(int iChannel=0; iChannel<nChannel; ++iChannel){	
      h_stability_deltaR[iChannel] = new TH1D(
          Form("h_%s_Ch%d_S3%s",STB_DR_,iChannel,_process),"",
	  nbins_gen_addbjets_dR,
	  //gen_addbjets_dR_min, gen_addbjets_dR_max
	  gen_addbjets_dR_width
	  );
      h_stability_deltaR[iChannel]->SetXTitle("#DeltaR_{b#bar{b}}");
      h_stability_deltaR[iChannel]->SetYTitle("Stability");
      h_stability_deltaR[iChannel]->Sumw2();

      h_stability_invMass[iChannel] = new TH1D(
          Form("h_%s_Ch%d_S3%s",STB_M_,iChannel,_process),"",
	  nbins_gen_addbjets_M,
	  //gen_addbjets_M_min, gen_addbjets_M_max
	  gen_addbjets_M_width
	  );
      h_stability_invMass[iChannel]->SetXTitle("M_{b#bar{b}}(GeV)");
      h_stability_invMass[iChannel]->SetYTitle("Stability");
      h_stability_invMass[iChannel]->Sumw2();

      h_stability_deltaEta[iChannel] = new TH1D(
          Form("h_%s_Ch%d_S3%s",STB_DETA_,iChannel,_process),"",
	  nbins_gen_addbjets_dEta,
	  gen_addbjets_dEta_min, gen_addbjets_dEta_max
	  //gen_addbjets_dEta_width
	  );
      h_stability_deltaEta[iChannel]->SetXTitle("#Delta#eta_{b#bar{b}}");
      h_stability_deltaEta[iChannel]->SetYTitle("Stability");
      h_stability_deltaEta[iChannel]->Sumw2();

      h_stability_deltaPhi[iChannel] = new TH1D(
          Form("h_%s_Ch%d_S3%s",STB_DPHI_,iChannel,_process),"",
	  nbins_gen_addbjets_dPhi,
	  gen_addbjets_dPhi_min, gen_addbjets_dPhi_max
	  //gen_addbjets_dPhi_width
	  );
      h_stability_deltaPhi[iChannel]->SetXTitle("#Delta#phi_{b#bar{b}}");
      h_stability_deltaPhi[iChannel]->SetYTitle("Stability");
      h_stability_deltaPhi[iChannel]->Sumw2();

      h_purity_deltaR[iChannel] = new TH1D(
          Form("h_%s_Ch%d_S3_%s",PUR_DR_,iChannel,_process),"",
	  nbins_gen_addbjets_dR,
	  //gen_addbjets_dR_min, gen_addbjets_dR_max
	  gen_addbjets_dR_width
	  );
      h_purity_deltaR[iChannel]->SetXTitle("#DeltaR_{b#bar{b}}");
      h_purity_deltaR[iChannel]->SetYTitle("Purity");
      h_purity_deltaR[iChannel]->Sumw2();

      h_purity_invMass[iChannel] = new TH1D(
          Form("h_%s_Ch%d_S3%s",PUR_M_,iChannel,_process),"",
	  nbins_gen_addbjets_M,
	  //gen_addbjets_M_min, gen_addbjets_M_max
	  gen_addbjets_M_width
	  );
      h_purity_invMass[iChannel]->SetXTitle("M_{b#bar{b}}(GeV)");
      h_purity_invMass[iChannel]->SetYTitle("Purity");
      h_purity_invMass[iChannel]->Sumw2();

      h_purity_deltaEta[iChannel] = new TH1D(
          Form("h_%s_Ch%d_S3_%s",PUR_DETA_,iChannel,_process),"",
	  nbins_gen_addbjets_dEta,
	  gen_addbjets_dEta_min, gen_addbjets_dEta_max
	  //gen_addbjets_dEta_width
	  );
      h_purity_deltaEta[iChannel]->SetXTitle("#Delta#eta_{b#bar{b}}");
      h_purity_deltaEta[iChannel]->SetYTitle("Purity");
      h_purity_deltaEta[iChannel]->Sumw2();

      h_purity_deltaPhi[iChannel] = new TH1D(
          Form("h_%s_Ch%d_S3_%s",PUR_DPHI_,iChannel,_process),"",
	  nbins_gen_addbjets_dPhi,
	  gen_addbjets_dPhi_min, gen_addbjets_dPhi_max
	  //gen_addbjets_dPhi_width
	  );
      h_purity_deltaPhi[iChannel]->SetXTitle("#DeltaR_{b#bar{b}}");
      h_purity_deltaPhi[iChannel]->SetYTitle("Purity");
      h_purity_deltaPhi[iChannel]->Sumw2();

      h_acceptance_deltaR[iChannel] = new TH1D(
          Form("h_%s_Ch%d%s",ACC_DR_,iChannel,_process),"",
	  nbins_gen_addbjets_dR,
	  //gen_addbjets_dR_min, gen_addbjets_dR_max
	  gen_addbjets_dR_width
	  );
      h_acceptance_deltaR[iChannel]->SetXTitle("#DeltaR_{b#bar{b}}");
      h_acceptance_deltaR[iChannel]->SetYTitle("Acceptance");
      h_acceptance_deltaR[iChannel]->Sumw2();
      
      h_acceptance_invMass[iChannel] = new TH1D(
          Form("h_%s_Ch%d%s",ACC_M_,iChannel,_process),"",
	  nbins_gen_addbjets_M,
	  //gen_addbjets_M_min, gen_addbjets_M_max
	  gen_addbjets_M_width
	  );
      h_acceptance_invMass[iChannel]->SetXTitle("M_{b#bar{b}}(GeV)");
      h_acceptance_invMass[iChannel]->SetYTitle("acceptance");
      h_acceptance_invMass[iChannel]->Sumw2();

      h_acceptance_deltaEta[iChannel] = new TH1D(
          Form("h_%s_Ch%d%s",ACC_DETA_,iChannel,_process),"",
	  nbins_gen_addbjets_dEta,
	  gen_addbjets_dEta_min, gen_addbjets_dEta_max
	  //gen_addbjets_dEta_width
	  );
      h_acceptance_deltaEta[iChannel]->SetXTitle("#Delta#eta_{b#bar{b}}");
      h_acceptance_deltaEta[iChannel]->SetYTitle("Acceptance");
      h_acceptance_deltaEta[iChannel]->Sumw2();
      
      h_acceptance_deltaPhi[iChannel] = new TH1D(
          Form("h_%s_Ch%d%s",ACC_DPHI_,iChannel,_process),"",
	  nbins_gen_addbjets_dPhi,
	  gen_addbjets_dPhi_min, gen_addbjets_dPhi_max
	  //gen_addbjets_dPhi_width
	  );
      h_acceptance_deltaPhi[iChannel]->SetXTitle("#Delta#phi_{b#bar{b}}");
      h_acceptance_deltaPhi[iChannel]->SetYTitle("Acceptance");
      h_acceptance_deltaPhi[iChannel]->Sumw2();
    }//channel
  }//mode
  else
    cout << "Warning : There is no such mode" << endl;
}
