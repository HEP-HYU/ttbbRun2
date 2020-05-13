#include <TObject.h>
#include <TH1.h>
#include <TH2.h>

std::map<std::string, int> m_lumi = {{"16",35922}, {"17",41529}, {"18",59693}};

const double TTBAR_XSEC_ = 831.76;
const double BRATIO_ = 0.436572; // 2*3*0.1086*0.67
//OBJECT SELECTION for 2016
int MUON_        = 0;
double MUON_PT_  = 30.0;
double MUON_ETA_ = 2.1;
double MUON_PHI_ = 0.0;
double MUON_E_   = 0.0;

int ELECTRON_        = 1;
double ELECTRON_PT_  = 35.0;
double ELECTRON_ETA_ = 2.1;
double ELECTRON_PHI_ = 0.0;
double ELECTRON_E_   = 0.0;

double JET_PT_  = 30.0;
double JET_ETA_ = 2.4;
double JET_PHI_ = 0.0;
double JET_E_   = 0.0;

// CSVv2
double JET_CSV_TIGHT_  = 0.9535;
double JET_CSV_MEDIUM_ = 0.8484;
double JET_CSV_LOOSE_  = 0.0;

double JET_CvsL_TIGHT_  = 0.0;
double JET_CvsL_MEDIUM_ = 0.0;
double JET_CvsL_LOOSE_  = 0.0;

//EVENT SELECTION
const int NUMBER_OF_LEPTONS_ = 1;
const int NUMBER_OF_JETS_    = 6;
const int NUMBER_OF_BJETS_   = 2;
const int NUMBER_OF_CJETS_   = 0;

//HISTOGRAM NAME
const char * RECO_LEP_PT_  = "LeptonPt";
const char * RECO_LEP_ETA_ = "LeptonEta";
const char * RECO_LEP_RELISO_ = "LeptonRelIso";
const char * RECO_N_JETS_  = "nJets";
const char * RECO_N_BJETS_ = "nbJets";
const char * RECO_JET_PT_  = "JetPt";
const char * RECO_JET_ETA_ = "JetEta";
const char * RECO_TRANS_M_ = "TransverseMass";
const char * RECO_CSV_     = "bJetDiscriminator";

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
const double njets_min=6; const double njets_max=10;
//const double njets_width[xNbins_njets+1] = {};
const int nbins_nbjets=5;
const double nbjets_min=0; const double nbjets_max=5;
//const double nbjets_width[xNbins_nbjets+1] = {};

//For unfolding
const int nbins_reco_addbjets_dR=6;
const double reco_addbjets_dR_min=0.4; const double reco_addbjets_dR_max=4.0;
const double reco_addbjets_dR_width[7] = {0.4,0.6,1.0,1.5,2.0,3.0,4.0};

//Gen
const int nbins_gen_addbjets_dR=3;
const double gen_addbjets_dR_min=0.4; const double gen_addbjets_dR_max=4.0;
const double gen_addbjets_dR_width[4] =
{0.4,1.0,2.0,4.0};

//For unfolding
const int nbins_reco_addbjets_M=8;
const double reco_addbjets_M_min=0; double reco_addbjets_M_max=400;
const double reco_addbjets_M_width[9] = {0.0,30.0,60.0,80.0,100.0,135.0,170.0,285.0,400.0};

//Gen
const int nbins_gen_addbjets_M=4;
const double gen_addbjets_M_min=0; const double gen_addbjets_M_max=400;
const double gen_addbjets_M_width[5] =
{0.0,60.0,100.0,170.0,400.0};

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

std::vector<std::string> syst_ttbar = {
  "__swup", "__swdown", "__psup", "__psdown",
  "__tuneup", "__tunedown", "__hdampup", "__hdampdown",
  "__pdfup", "__pdfdown"
};

std::vector<std::string> syst_jet = {
  "__jerup", "__jerdown", "__jecup", "__jecdown"
};

std::vector<std::string> syst_lep16 = {
  "__musfup", "__musfdown", "__mutrgup", "__mutrgdown",
  "__elsfup", "__elsfdown", "__eltrgup", "__eltrgdown",
};

std::vector<std::string> syst_lep1718 = {
  "__muidup", "__muiddown", "__muisoup", "__muisodown", "__mutrgup", "__mutrgdown",
  "__elidup", "__eliddown", "__elrecoup", "__elrecodown", "__elzvtxup", "__elzvtxdown",
  "__eltrgup", "__eltrgdown"
};

std::vector<std::string> syst_basic = {
  "__puup", "__pudown",
  "__lfup", "__lfdown", "__hfup", "__hfdown",
  "__hfstat1up","__hfstat1down", "__hfstat2up", "__hfstat2down",
  "__lfstat1up","__lfstat1down", "__lfstat2up", "__lfstat2down",
  "__cferr1up", "__cferr1down",  "__cferr2up",  "__cferr2down"
};

class HistoBook{
private:
  std::vector<string> v_chName = {"#mu","#it{e}","lep"};
public:
  HistoBook(){};
  HistoBook(const int _mode, const char *_process);
  //~Histo_Book();
  
  //control plots : mode = 1
  TH1D *h_pv[nChannel][nStep];
  TH1D *h_pv_nosf[nChannel][nStep];
  
  TH1D *h_lepton_pt[nChannel][nStep];
  TH1D *h_lepton_eta[nChannel][nStep];
  TH1D *h_lepton_relIso[nChannel][nStep];
  
  TH1D *h_njets[nChannel][nStep];
  TH1D *h_nbjets[nChannel][nStep];
  
  TH1D *h_jet_pt[nChannel][nStep][nJet];
  TH1D *h_jet_eta[nChannel][nStep][nJet];
  
  TH1D *h_trans_mass[nChannel][nStep];
  
  TH1D *h_csv[nChannel][nStep][nJet];
  TH1D *h_1st_csv[nChannel][nStep];
  TH1D *h_2nd_csv[nChannel][nStep][nbins_csv];
  
  TH1D *h_reco_addbjets_deltaR[nChannel][nStep];
  TH1D *h_reco_addbjets_invMass[nChannel][nStep];
  TH1D *h_reco_addbjets_deltaR2[nChannel][nStep];
  TH1D *h_reco_addbjets_invMass2[nChannel][nStep];
  TH1D *h_reco_addbjets_deltaR3[nChannel][nStep];
  TH1D *h_reco_addbjets_invMass3[nChannel][nStep];

  TH2D *h_reco_deltaRvsInvMass[nChannel][nStep];
  TH1D *h_reco_deltaRvsInvMass_spread[nChannel][nStep];

  //response matrix : mode = 2
  TH1D *h_gen_gentop_deltaR[nChannel][nStep];
  TH1D *h_gen_gentop_invMass[nChannel][nStep];
  TH1D *h_gen_gentop_deltaR2[nChannel][nStep];
  TH1D *h_gen_gentop_invMass2[nChannel][nStep];
 
  TH1D *h_gen_mindR_deltaR[nChannel][nStep];
  TH1D *h_gen_mindR_invMass[nChannel][nStep];
  TH1D *h_gen_mindR_deltaR2[nChannel][nStep];
  TH1D *h_gen_mindR_invMass2[nChannel][nStep];
  
  TH2D *h_respMatrix_gentop_deltaR[nChannel][nStep];
  TH2D *h_respMatrix_gentop_invMass[nChannel][nStep];
  TH2D *h_respMatrix_mindR_deltaR[nChannel][nStep];
  TH2D *h_respMatrix_mindR_invMass[nChannel][nStep];

  TH2D *h_respMatrix_mindR_deltaR2[nChannel][nStep];
  TH2D *h_respMatrix_mindR_invMass2[nChannel][nStep];
  TH2D *h_respMatrix_gentop_deltaR2[nChannel][nStep];
  TH2D *h_respMatrix_gentop_invMass2[nChannel][nStep];

  //etc : mode = 3
  TH1D *h_stability_deltaR[nChannel];
  TH1D *h_stability_invMass[nChannel];
  TH1D *h_purity_deltaR[nChannel];
  TH1D *h_purity_invMass[nChannel];
  TH1D *h_acceptance_deltaR[nChannel];
  TH1D *h_acceptance_invMass[nChannel];
};

HistoBook::HistoBook(const int _mode, const char *_process){
  if(_mode == 1){   
    //std::cout << "Make mode1" << std::endl;
    for(int iChannel=0; iChannel<nChannel; ++iChannel){
      for(int iStep=0; iStep<nStep; ++iStep){
        h_pv[iChannel][iStep] = new TH1D(
            Form("h_PV_Ch%d_S%d%s",iChannel,iStep,_process),"",
            60, 0, 60);
        h_pv[iChannel][iStep]->SetXTitle("Good PV");
        h_pv[iChannel][iStep]->SetYTitle("Entries");
        h_pv[iChannel][iStep]->Sumw2();

        h_pv_nosf[iChannel][iStep] = new TH1D(
            Form("h_PV_noSF_Ch%d_S%d%s",iChannel,iStep,_process),"",
            60, 0, 60);
        h_pv_nosf[iChannel][iStep]->SetXTitle("Good PV without reweighting");
        h_pv_nosf[iChannel][iStep]->Sumw2();

        h_lepton_pt[iChannel][iStep] = new TH1D(
	          Form("h_%s_Ch%d_S%d%s",RECO_LEP_PT_,iChannel,iStep,_process),"",
	          nbins_lep_pt, lep_pt_min, lep_pt_max);
	      h_lepton_pt[iChannel][iStep]->SetXTitle(Form("%s p_{T} (GeV)",v_chName[iChannel].c_str()));
	      h_lepton_pt[iChannel][iStep]->SetYTitle("Entries");
	      h_lepton_pt[iChannel][iStep]->Sumw2();

	      h_lepton_eta[iChannel][iStep] = new TH1D(
	          Form("h_%s_Ch%d_S%d%s",RECO_LEP_ETA_,iChannel,iStep,_process),"",
	          nbins_lep_eta, lep_eta_min, lep_eta_max);
	      h_lepton_eta[iChannel][iStep]->SetXTitle(Form("%s #eta",v_chName[iChannel].c_str()));
	      h_lepton_eta[iChannel][iStep]->SetYTitle("Entries");
	      h_lepton_eta[iChannel][iStep]->Sumw2();

	      h_lepton_relIso[iChannel][iStep] = new TH1D(
	          Form("h_%s_Ch%d_S%d%s",RECO_LEP_RELISO_,iChannel,iStep,_process),"",
	          20, 0, 1);
	      h_lepton_relIso[iChannel][iStep]->SetXTitle(Form("%s relIso",v_chName[iChannel].c_str()));
	      h_lepton_relIso[iChannel][iStep]->SetYTitle("Entries");
	      h_lepton_relIso[iChannel][iStep]->Sumw2();

        if(iStep == 0){
          h_njets[iChannel][iStep] = new TH1D(
              Form("h_%s_Ch%d_S%d%s", RECO_N_JETS_, iChannel, iStep, _process), "",
              10, 0, 10);
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(1, "0");
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(2, "1");
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(3, "2");
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(4, "3");
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(5, "4");
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(6, "5");
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(7, "6");
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(8, "7");
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(9, "8");
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(10, "#geq 9");
        }
      	else{
          h_njets[iChannel][iStep] = new TH1D(
              Form("h_%s_Ch%d_S%d%s",RECO_N_JETS_,iChannel,iStep,_process),"",
              4, 6, 10);
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(1, "6");
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(2, "7");
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(3, "8");
              h_njets[iChannel][iStep]->GetXaxis()->SetBinLabel(4, "#geq 9");
        }
        h_njets[iChannel][iStep]->SetXTitle("Jet multiplicity");
        h_njets[iChannel][iStep]->SetYTitle("Entries");
        h_njets[iChannel][iStep]->Sumw2();

        h_nbjets[iChannel][iStep] = new TH1D(
            Form("h_%s_Ch%d_S%d%s",RECO_N_BJETS_,iChannel,iStep,_process),"",
            nbins_nbjets, nbjets_min, nbjets_max);
        h_nbjets[iChannel][iStep]->SetXTitle("bJet multiplicity");
        h_nbjets[iChannel][iStep]->SetYTitle("Entries");
        h_nbjets[iChannel][iStep]->GetXaxis()->SetBinLabel(1, "0");
        h_nbjets[iChannel][iStep]->GetXaxis()->SetBinLabel(2, "1");
        h_nbjets[iChannel][iStep]->GetXaxis()->SetBinLabel(3, "2");
        h_nbjets[iChannel][iStep]->GetXaxis()->SetBinLabel(4, "3");
        h_nbjets[iChannel][iStep]->GetXaxis()->SetBinLabel(5, "#geq 4");
        h_nbjets[iChannel][iStep]->Sumw2();
        
        for(int iJet=0; iJet<nJet; ++iJet){
          h_jet_pt[iChannel][iStep][iJet] = new TH1D(
              Form("h_%s_%d_Ch%d_S%d%s",RECO_JET_PT_,iJet,iChannel,iStep,_process),"",
              nbins_jet_pt, jet_pt_min, jet_pt_max);
          h_jet_pt[iChannel][iStep][iJet]->SetXTitle("Jet p_{T} (GeV)");
          h_jet_pt[iChannel][iStep][iJet]->SetYTitle("Entries");
          h_jet_pt[iChannel][iStep][iJet]->Sumw2();

          h_jet_eta[iChannel][iStep][iJet] = new TH1D(
              Form("h_%s_%d_Ch%d_S%d%s",RECO_JET_ETA_,iJet,iChannel,iStep,_process),"",
              nbins_jet_eta, jet_eta_min, jet_eta_max);
          h_jet_eta[iChannel][iStep][iJet]->SetXTitle("Jet #eta");
          h_jet_eta[iChannel][iStep][iJet]->SetYTitle("Entries");
          h_jet_eta[iChannel][iStep][iJet]->Sumw2();

          h_csv[iChannel][iStep][iJet] = new TH1D(
              Form("h_%s_%d_Ch%d_S%d%s",RECO_CSV_,iJet,iChannel,iStep,_process),"",
              nbins_csv, csv_min, csv_max);
          h_csv[iChannel][iStep][iJet]->SetXTitle("b discriminator");
          h_csv[iChannel][iStep][iJet]->SetYTitle("Entries");
          h_csv[iChannel][iStep][iJet]->Sumw2();
        }

        h_1st_csv[iChannel][iStep] = new TH1D(
            Form("h_%s_1st_Ch%d_S%d%s", RECO_CSV_, iChannel, iStep, _process),"",
            nbins_csv, csv_min, csv_max);
        h_1st_csv[iChannel][iStep]->SetXTitle("b discriminator");
        h_1st_csv[iChannel][iStep]->SetYTitle("Entries");
        h_1st_csv[iChannel][iStep]->Sumw2();

        for(int iregion = 0; iregion < nbins_csv; iregion++){
          h_2nd_csv[iChannel][iStep][iregion] = new TH1D(
              Form("h_%s_2nd_Region%d_Ch%d_S%d%s", RECO_CSV_,iregion,iChannel,iStep,_process),"",
              nbins_csv, csv_min, csv_max);
          h_2nd_csv[iChannel][iStep][iregion]->SetXTitle("b discriminator");
          h_2nd_csv[iChannel][iStep][iregion]->SetYTitle("Entries");
          h_2nd_csv[iChannel][iStep][iregion]->Sumw2(); 
        }

        h_trans_mass[iChannel][iStep] = new TH1D(
            Form("h_%s_Ch%d_S%d%s",RECO_TRANS_M_,iChannel,iStep,_process),"",
            nbins_wmass, wmass_min, wmass_max);
        h_trans_mass[iChannel][iStep]->SetXTitle("transverse mass (GeV)");
        h_trans_mass[iChannel][iStep]->SetYTitle("Entries");
        h_trans_mass[iChannel][iStep]->Sumw2();

        h_reco_addbjets_deltaR[iChannel][iStep] = new TH1D(
            Form("h_mindR_%s_Ch%d_S%d%s",RECO_ADD_DR_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_dR, reco_addbjets_dR_width);
        h_reco_addbjets_deltaR[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
        h_reco_addbjets_deltaR[iChannel][iStep]->SetYTitle("Entries");
        h_reco_addbjets_deltaR[iChannel][iStep]->Sumw2();

        h_reco_addbjets_invMass[iChannel][iStep] = new TH1D(
            Form("h_mindR_%s_Ch%d_S%d%s",RECO_ADD_M_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_M, reco_addbjets_M_width);
        h_reco_addbjets_invMass[iChannel][iStep]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
        h_reco_addbjets_invMass[iChannel][iStep]->SetYTitle("Entries");
        h_reco_addbjets_invMass[iChannel][iStep]->Sumw2();

        h_reco_addbjets_deltaR2[iChannel][iStep] = new TH1D(
            Form("h_mindR_%s2_Ch%d_S%d%s",RECO_ADD_DR_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_dR, reco_addbjets_dR_width);
        h_reco_addbjets_deltaR2[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
        h_reco_addbjets_deltaR2[iChannel][iStep]->SetYTitle("Entries");
        h_reco_addbjets_deltaR2[iChannel][iStep]->Sumw2();

        h_reco_addbjets_invMass2[iChannel][iStep] = new TH1D(
            Form("h_mindR_%s2_Ch%d_S%d%s",RECO_ADD_M_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_M, reco_addbjets_M_width);
        h_reco_addbjets_invMass2[iChannel][iStep]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
        h_reco_addbjets_invMass2[iChannel][iStep]->SetYTitle("Entries");
        h_reco_addbjets_invMass2[iChannel][iStep]->Sumw2();

        h_reco_addbjets_deltaR3[iChannel][iStep] = new TH1D(
            Form("h_mindR_%s3_Ch%d_S%d%s",RECO_ADD_DR_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_dR, reco_addbjets_dR_width);
        h_reco_addbjets_deltaR3[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
        h_reco_addbjets_deltaR3[iChannel][iStep]->SetYTitle("Entries");
        h_reco_addbjets_deltaR3[iChannel][iStep]->Sumw2();

        h_reco_addbjets_invMass3[iChannel][iStep] = new TH1D(
            Form("h_mindR_%s3_Ch%d_S%d%s",RECO_ADD_M_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_M, reco_addbjets_M_width);
        h_reco_addbjets_invMass3[iChannel][iStep]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
        h_reco_addbjets_invMass3[iChannel][iStep]->SetYTitle("Entries");
        h_reco_addbjets_invMass3[iChannel][iStep]->Sumw2();
        
        h_reco_deltaRvsInvMass[iChannel][iStep] = new TH2D(
            Form("h_mindR_RecoDeltaRvsInvMass_Ch%d_S%d%s",iChannel,iStep,_process),"",
            nbins_reco_addbjets_dR, reco_addbjets_dR_width,
            nbins_reco_addbjets_M, reco_addbjets_M_width);
        h_reco_deltaRvsInvMass[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
        h_reco_deltaRvsInvMass[iChannel][iStep]->SetYTitle(Form("M_{b#bar{b}}(GeV)"));
        h_reco_deltaRvsInvMass[iChannel][iStep]->Sumw2();

        int nbin = nbins_reco_addbjets_dR * nbins_reco_addbjets_M;
        int len_dR = sizeof(reco_addbjets_dR_width)/sizeof(*reco_addbjets_dR_width);
        int len_M = sizeof(reco_addbjets_M_width)/sizeof(*reco_addbjets_M_width);
        int len_tot = 0;
        double spread_width[nbin];
        int tmp = 1; 
        spread_width[0] = 0.0;
        
        for(int i=0; i < len_dR-1; i++){
          for(int j=0; j < len_M-1; j++){
            double value = (reco_addbjets_dR_width[i+1]-reco_addbjets_dR_width[i])*(reco_addbjets_M_width[j+1]-reco_addbjets_M_width[j]);
            spread_width[tmp] = spread_width[tmp-1]+value;
            ++tmp;
          }
        }

        h_reco_deltaRvsInvMass_spread[iChannel][iStep] = new TH1D(
            Form("h_mindR_RecoDeltaRvsInvMass_spread_Ch%d_S%d%s",iChannel,iStep,_process),"",
            nbin, spread_width);
        h_reco_deltaRvsInvMass_spread[iChannel][iStep]->SetXTitle("Bin width = Width_{dR} x Width_{M}");
        h_reco_deltaRvsInvMass_spread[iChannel][iStep]->SetYTitle("Entries");
        h_reco_deltaRvsInvMass_spread[iChannel][iStep]->Sumw2();
      }//step
    }//channel
  }//mode
  else if(_mode == 2){
    //std::cout << "Make mode2" << std::endl;
    for(int iChannel=0; iChannel<nChannel; ++iChannel){
      for(int iStep=0; iStep<nStep; ++iStep){
        h_gen_gentop_deltaR[iChannel][iStep] = new TH1D(
            Form("h_gentop_%s_Ch%d_S%d%s",GEN_ADD_DR_,iChannel,iStep,_process),"",
            nbins_gen_addbjets_dR, gen_addbjets_dR_width);
        h_gen_gentop_deltaR[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
        h_gen_gentop_deltaR[iChannel][iStep]->SetYTitle("Entries");
        h_gen_gentop_deltaR[iChannel][iStep]->Sumw2();

        h_gen_gentop_invMass[iChannel][iStep] = new TH1D(
            Form("h_gentop_%s_Ch%d_S%d%s",GEN_ADD_M_,iChannel,iStep,_process),"",
            nbins_gen_addbjets_M, gen_addbjets_M_width);
        h_gen_gentop_invMass[iChannel][iStep]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
        h_gen_gentop_invMass[iChannel][iStep]->SetYTitle("Entries");
        h_gen_gentop_invMass[iChannel][iStep]->Sumw2();
 
        h_gen_gentop_deltaR2[iChannel][iStep] = new TH1D(
            Form("h_gentop_%s2_Ch%d_S%d%s",GEN_ADD_DR_,iChannel,iStep,_process),"",
            nbins_gen_addbjets_dR, gen_addbjets_dR_width);
        h_gen_gentop_deltaR2[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
        h_gen_gentop_deltaR2[iChannel][iStep]->SetYTitle("Entries");
        h_gen_gentop_deltaR2[iChannel][iStep]->Sumw2();

        h_gen_gentop_invMass2[iChannel][iStep] = new TH1D(
            Form("h_gentop_%s2_Ch%d_S%d%s",GEN_ADD_M_,iChannel,iStep,_process),"",
            nbins_gen_addbjets_M, gen_addbjets_M_width);
        h_gen_gentop_invMass2[iChannel][iStep]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
        h_gen_gentop_invMass2[iChannel][iStep]->SetYTitle("Entries");
        h_gen_gentop_invMass2[iChannel][iStep]->Sumw2();
        
        h_gen_mindR_deltaR[iChannel][iStep] = new TH1D(
            Form("h_mindR_%s_Ch%d_S%d%s",GEN_ADD_DR_,iChannel,iStep,_process),"",
            nbins_gen_addbjets_dR, gen_addbjets_dR_width);
        h_gen_mindR_deltaR[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
        h_gen_mindR_deltaR[iChannel][iStep]->SetYTitle("Entries");
        h_gen_mindR_deltaR[iChannel][iStep]->Sumw2();

        h_gen_mindR_invMass[iChannel][iStep] = new TH1D(
            Form("h_mindR_%s_Ch%d_S%d%s",GEN_ADD_M_,iChannel,iStep,_process),"",
            nbins_gen_addbjets_M, gen_addbjets_M_width);
        h_gen_mindR_invMass[iChannel][iStep]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
        h_gen_mindR_invMass[iChannel][iStep]->SetYTitle("Entries");
        h_gen_mindR_invMass[iChannel][iStep]->Sumw2();
         
        h_gen_mindR_deltaR2[iChannel][iStep] = new TH1D(
            Form("h_mindR_%s2_Ch%d_S%d%s",GEN_ADD_DR_,iChannel,iStep,_process),"",
            nbins_gen_addbjets_dR, gen_addbjets_dR_width);
        h_gen_mindR_deltaR2[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
        h_gen_mindR_deltaR2[iChannel][iStep]->SetYTitle("Entries");
        h_gen_mindR_deltaR2[iChannel][iStep]->Sumw2();

        h_gen_mindR_invMass2[iChannel][iStep] = new TH1D(
            Form("h_mindR_%s2_Ch%d_S%d%s",GEN_ADD_M_,iChannel,iStep,_process),"",
            nbins_gen_addbjets_M, gen_addbjets_M_width);
        h_gen_mindR_invMass2[iChannel][iStep]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
        h_gen_mindR_invMass2[iChannel][iStep]->SetYTitle("Entries");
        h_gen_mindR_invMass2[iChannel][iStep]->Sumw2();
       
        h_respMatrix_gentop_deltaR[iChannel][iStep] = new TH2D(
            Form("h_gentop_%s_Ch%d_S%d%s",MATRIX_DR_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_dR, reco_addbjets_dR_width,
            nbins_gen_addbjets_dR, gen_addbjets_dR_width);
        h_respMatrix_gentop_deltaR[iChannel][iStep]->SetYTitle("Gen. #DeltaR_{b#bar{b}}");
        h_respMatrix_gentop_deltaR[iChannel][iStep]->SetXTitle("Reco. #DeltaR_{b#bar{b}}");
        h_respMatrix_gentop_deltaR[iChannel][iStep]->Sumw2();

        h_respMatrix_gentop_invMass[iChannel][iStep] = new TH2D(
            Form("h_gentop_%s_Ch%d_S%d%s",MATRIX_M_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_M, reco_addbjets_M_width,
            nbins_gen_addbjets_M, gen_addbjets_M_width);
        h_respMatrix_gentop_invMass[iChannel][iStep]->SetYTitle("Gen. M_{b#bar{b}}(GeV)");
        h_respMatrix_gentop_invMass[iChannel][iStep]->SetXTitle("Reco. M_{b#bar{b}}(GeV)");
        h_respMatrix_gentop_invMass[iChannel][iStep]->Sumw2();
        
        h_respMatrix_mindR_deltaR[iChannel][iStep] = new TH2D(
            Form("h_mindR_%s_Ch%d_S%d%s",MATRIX_DR_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_dR, reco_addbjets_dR_width,
            nbins_gen_addbjets_dR, gen_addbjets_dR_width);
        h_respMatrix_mindR_deltaR[iChannel][iStep]->SetYTitle("Gen. #DeltaR_{b#bar{b}}");
        h_respMatrix_mindR_deltaR[iChannel][iStep]->SetXTitle("Reco. #DeltaR_{b#bar{b}}");
        h_respMatrix_mindR_deltaR[iChannel][iStep]->Sumw2();

        h_respMatrix_mindR_invMass[iChannel][iStep] = new TH2D(
            Form("h_mindR_%s_Ch%d_S%d%s",MATRIX_M_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_M, reco_addbjets_M_width,
            nbins_gen_addbjets_M, gen_addbjets_M_width);
        h_respMatrix_mindR_invMass[iChannel][iStep]->SetYTitle("Gen. M_{b#bar{b}}(GeV)");
        h_respMatrix_mindR_invMass[iChannel][iStep]->SetXTitle("Reco. M_{b#bar{b}}(GeV)");
        h_respMatrix_mindR_invMass[iChannel][iStep]->Sumw2();

        h_respMatrix_gentop_deltaR2[iChannel][iStep] = new TH2D(
            Form("h_gentop_%s2_Ch%d_S%d%s",MATRIX_DR_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_dR, reco_addbjets_dR_width,
            nbins_gen_addbjets_dR, gen_addbjets_dR_width);
        h_respMatrix_gentop_deltaR2[iChannel][iStep]->SetYTitle("Gen. #DeltaR_{b#bar{b}}");
        h_respMatrix_gentop_deltaR2[iChannel][iStep]->SetXTitle("Reco. #DeltaR_{b#bar{b}}");
        h_respMatrix_gentop_deltaR2[iChannel][iStep]->Sumw2();

        h_respMatrix_gentop_invMass2[iChannel][iStep] = new TH2D(
            Form("h_gentop_%s2_Ch%d_S%d%s",MATRIX_M_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_M, reco_addbjets_M_width,
            nbins_gen_addbjets_M, gen_addbjets_M_width);
        h_respMatrix_gentop_invMass2[iChannel][iStep]->SetYTitle("Gen. M_{b#bar{b}}(GeV)");
        h_respMatrix_gentop_invMass2[iChannel][iStep]->SetXTitle("Reco. M_{b#bar{b}}(GeV)");
        h_respMatrix_gentop_invMass2[iChannel][iStep]->Sumw2();
        
        h_respMatrix_mindR_deltaR2[iChannel][iStep] = new TH2D(
            Form("h_mindR_%s2_Ch%d_S%d%s",MATRIX_DR_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_dR, reco_addbjets_dR_width,
            nbins_gen_addbjets_dR, gen_addbjets_dR_width);
        h_respMatrix_mindR_deltaR2[iChannel][iStep]->SetYTitle("Gen. #DeltaR_{b#bar{b}}");
        h_respMatrix_mindR_deltaR2[iChannel][iStep]->SetXTitle("Reco. #DeltaR_{b#bar{b}}");
        h_respMatrix_mindR_deltaR2[iChannel][iStep]->Sumw2();

        h_respMatrix_mindR_invMass2[iChannel][iStep] = new TH2D(
            Form("h_mindR_%s2_Ch%d_S%d%s",MATRIX_M_,iChannel,iStep,_process),"",
            nbins_reco_addbjets_M, reco_addbjets_M_width,
            nbins_gen_addbjets_M, gen_addbjets_M_width);
        h_respMatrix_mindR_invMass2[iChannel][iStep]->SetYTitle("Gen. M_{b#bar{b}}(GeV)");
        h_respMatrix_mindR_invMass2[iChannel][iStep]->SetXTitle("Reco. M_{b#bar{b}}(GeV)");
        h_respMatrix_mindR_invMass2[iChannel][iStep]->Sumw2();
      }//step
    }//channel
  }//mode
  else if(_mode == 3){
    for(int iChannel=0; iChannel<nChannel; ++iChannel){	
      h_stability_deltaR[iChannel] = new TH1D(
          Form("h_%s_Ch%d_S3%s",STB_DR_,iChannel,_process),"",
          nbins_gen_addbjets_dR, gen_addbjets_dR_width);
      h_stability_deltaR[iChannel]->SetXTitle("#DeltaR_{b#bar{b}}");
      h_stability_deltaR[iChannel]->SetYTitle("Stability");
      h_stability_deltaR[iChannel]->Sumw2();

      h_stability_invMass[iChannel] = new TH1D(
          Form("h_%s_Ch%d_S3%s",STB_M_,iChannel,_process),"",
          nbins_gen_addbjets_M, gen_addbjets_M_width);
      h_stability_invMass[iChannel]->SetXTitle("M_{b#bar{b}}(GeV)");
      h_stability_invMass[iChannel]->SetYTitle("Stability");
      h_stability_invMass[iChannel]->Sumw2();

      h_purity_deltaR[iChannel] = new TH1D(
          Form("h_%s_Ch%d_S3_%s",PUR_DR_,iChannel,_process),"",
          nbins_gen_addbjets_dR, gen_addbjets_dR_width);
      h_purity_deltaR[iChannel]->SetXTitle("#DeltaR_{b#bar{b}}");
      h_purity_deltaR[iChannel]->SetYTitle("Purity");
      h_purity_deltaR[iChannel]->Sumw2();

      h_purity_invMass[iChannel] = new TH1D(
          Form("h_%s_Ch%d_S3%s",PUR_M_,iChannel,_process),"",
          nbins_gen_addbjets_M, gen_addbjets_M_width);
      h_purity_invMass[iChannel]->SetXTitle("M_{b#bar{b}}(GeV)");
      h_purity_invMass[iChannel]->SetYTitle("Purity");
      h_purity_invMass[iChannel]->Sumw2();

      h_acceptance_deltaR[iChannel] = new TH1D(
          Form("h_%s_Ch%d%s",ACC_DR_,iChannel,_process),"",
          nbins_gen_addbjets_dR, gen_addbjets_dR_width);
      h_acceptance_deltaR[iChannel]->SetXTitle("#DeltaR_{b#bar{b}}");
      h_acceptance_deltaR[iChannel]->SetYTitle("Acceptance");
      h_acceptance_deltaR[iChannel]->Sumw2();
      
      h_acceptance_invMass[iChannel] = new TH1D(
          Form("h_%s_Ch%d%s",ACC_M_,iChannel,_process),"",
          nbins_gen_addbjets_M, gen_addbjets_M_width);
      h_acceptance_invMass[iChannel]->SetXTitle("M_{b#bar{b}}(GeV)");
      h_acceptance_invMass[iChannel]->SetYTitle("acceptance");
      h_acceptance_invMass[iChannel]->Sumw2();
    }//channel
  }//mode
  else
    cout << "Warning : There is no such mode" << endl;
}
