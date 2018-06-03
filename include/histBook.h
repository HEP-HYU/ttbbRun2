#include <TObject.h>
#include <TH1.h>
#include <TH2.h>

//SAMPLE LIST
enum class Sample_List_ {
  TTBB = 0,TTBJ,TTCC,/*TTJJ,*/TTLF,TTBKG,WJETS,ZJETS,ZJETS10TO50,
  TCHANNEL,TBARCHANNEL,TWCHANNEL,TBARWCHANNEL,WW,WZ,ZZ,LAST
};

const int TTBB_ = static_cast<int>(Sample_List_::TTBB);
const int TTBJ_ = static_cast<int>(Sample_List_::TTBJ);
const int TTCC_ = static_cast<int>(Sample_List_::TTCC);
//const int TTJJ_ = static_cast<int>(Sample_List_::TTJJ);
const int TTLF_ = static_cast<int>(Sample_List_::TTLF);
const int TTBKG_ = static_cast<int>(Sample_List_::TTBKG);
const int WJETS_ = static_cast<int>(Sample_List_::WJETS);
const int ZJETS_ = static_cast<int>(Sample_List_::ZJETS);
const int ZJETS10TO50_ = static_cast<int>(Sample_List_::ZJETS);
const int TCHANNEL_ = static_cast<int>(Sample_List_::TCHANNEL);
const int TBARCHANNEL_ = static_cast<int>(Sample_List_::TBARCHANNEL);
const int TWCHANNEL_ = static_cast<int>(Sample_List_::TWCHANNEL);
const int TBARWCHANNEL_ = static_cast<int>(Sample_List_::TBARWCHANNEL);
const int WW_ = static_cast<int>(Sample_List_::WW);
const int WZ_ = static_cast<int>(Sample_List_::WZ);
const int ZZ_ = static_cast<int>(Sample_List_::ZZ);

std::vector<string> DATA_ = {"DataSingleMu","DataSingleEl"};
std::vector<string> NAME_ = {
  "ttbb","ttbj","ttcc",/*"ttjj",*/"ttLF","ttbkg","wjets","zjets","zjets10to50",
  "tchannel","tbarchannel","tWchannel","tbarWchannel","ww","wz","zz"
};

//SCALE
const double LUMINOSITY_ = 35922;
const double BRANCHINGRATIO_ = 0.436572;//2*3*0.1086*0.67
//const double LUMINOSITY_ = 35867;
std::vector<double> XSEC_ = {
  365.4, //ttbb 
  365.4, //ttbj 
  365.4, //ttcc
  //365.4,//ttjj
  365.4, //ttLF
  365.4, //ttbkg
  61524.0, //wjets
  6025.2, //zjets
  18610.0, //zjets10to50
  136.02, //tchannel
  80.95, //tWchannel
  35.6, //tWchannel
  35.6, //tbarWchannel
  118.7, //ww
  47.13, //wz
  16.523 //zz
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
const char * RECO_LEPTON_PT_ = "LeptonPt";
const char * RECO_LEPTON_ETA_ = "LeptonEta";
const char * RECO_NUMBER_OF_JETS_ = "nJets";
const char * RECO_NUMBER_OF_BJETS_ = "nBjets";
const char * RECO_ADDJETS_DELTAR_ = "RecoJetDeltaR";
const char * RECO_ADDJETS_INVARIANT_MASS_ = "RecoJetInvMass";
const char * RECO_ADDBJETS_DELTAR_ = "RecobJetDeltaR";
const char * RECO_ADDBJETS_INVARIANT_MASS_ = "RecobJetInvMass";
const char * RECO_TOTJETS_INVARIANT_MASS_ = "RecoTotJetInvMass";
const char * GEN_ADDJETS_DELTAR_ = "GenJetDeltaR";
const char * GEN_ADDJETS_INVARIANT_MASS_ = "GenJetInvMass";
const char * GEN_ADDBJETS_DELTAR_ = "GenbJetDetlaR";
const char * GEN_ADDBJETS_INVARIANT_MASS_ = "GenbJetInvMass";
const char * GEN_TOTJETS_INVARIANT_MASS_ = "GenTotJetInvMass";
const char * RESPONSE_MATRIX_DELTAR_ = "ResponseMatrixDeltaR";
const char * RESPONSE_MATRIX_INVARIANT_MASS_ = "ResponseMatrixInvMass";
const char * RESPONSE_MATRIX_TOTALJETS_INVARIANT_MASS_ = "ResponseMatrixTotMass";
const char * BIN_STABILITY_DELTAR_ = "BinStabilityDeltaR";
const char * BIN_STABILITY_INVARIANT_MASS_ = "BinStabilityInvMass";
const char * BIN_STABILITY_TOTALJETS_INVARIANT_MASS_ = "BinStabilityTotMass";
const char * BIN_PURITY_DELTAR_ = "BinPurityDeltaR";
const char * BIN_PURITY_INVARIANT_MASS_ = "BinPurityInvMass";
const char * BIN_PURITY_TOTALJETS_INVARIANT_MASS_ = "BinPurityTotMass";
const char * BIN_ACCEPTANCE_DELTAR_ = "BinAcceptanceDeltaR";
const char * BIN_ACCEPTANCE_INVARIANT_MASS_ = "BinAcceptanceInvMass";

const int nChannel=2; const int nStep=4;

class HistoBook{
  private:
    std::vector<string> v_chName = {"#mu","#it{e}"};
  public:
    const int xNbins_lepton_pt=400;
    const double lepton_pt_min=0; const double lepton_pt_max=400;
    //const double lepton_pt_width[xNbins_lepton_pt+1] = {};
    const int xNbins_lepton_eta=20;
    const double lepton_eta_min=0; double lepton_eta_max=2.5;
    //const double lepton_eta_width[xNbins_lepton_eta+1] = {};

    const int xNbins_njets=10;
    const double njets_min=0; const double njets_max=10;
    //const double njets_width[xNbins_njets+1] = {};
    const int xNbins_nbjets=10;
    const double nbjets_min=0; const double nbjets_max=10;
    //const double nbjets_width[xNbins_nbjets+1] = {};

    const int xNbins_reco_addjets_dR=9;
    //const int xNbins_reco_addjets_dR=25;
    const double reco_addjets_dR_min=0; const double reco_addjets_dR_max=5;
    const double reco_addjets_dR_width[10] = 
    {0.4,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0};
    const int xNbins_reco_addjets_M=10;
    //const int xNbins_reco_addjets_M=20;
    const double reco_addjets_M_min=0; double reco_addjets_M_max=400;
    const double reco_addjets_M_width[11] =
    {0,40,80,120,160,200,240,280,320,360,400};
    
    const int xNbins_gen_addbjets_dR=3;
    //const int xNbins_gen_addbjets_dR=25;
    const double gen_addbjets_dR_min=0; const double gen_addbjets_dR_max=5;
    const double gen_addbjets_dR_width[5] =
    {0.4,1.0,2.5,5.0};
    const int xNbins_gen_addbjets_M=4;
    //const int xNbins_gen_addbjets_M=20;
    const double gen_addbjets_M_min=0; const double gen_addbjets_M_max=400;
    const double gen_addbjets_M_width[5] =
    {0.0,60.0,100.0,170.0,400.0};
   
    const int xNbins_reco_totjets_M=30;
    const double reco_totjets_M_min=0; const double reco_totjets_M_max=3000;
    const int xNbins_gen_totjets_M=30;
    const double gen_totjets_M_min=0; const double gen_totjets_M_max=3000;

    //control plots : mode = 1
    TH1D *h_lepton_pt[nChannel][nStep];
    TH1D *h_lepton_eta[nChannel][nStep];
    TH1D *h_njets[nChannel][nStep];
    TH1D *h_nbjets[nChannel][nStep];
    TH1D *h_reco_addjets_deltaR[nChannel][nStep];
    TH1D *h_reco_addjets_invMass[nChannel][nStep];
    TH1D *h_reco_totjets_invMass[nChannel][nStep];

    //response matrix : mode = 2
    TH2D *h_respMatrix_deltaR[nChannel][nStep];
    TH2D *h_respMatrix_invMass[nChannel][nStep];
    TH2D *h_respMatrix_totMass[nChannel][nStep];
    TH1D *h_gen_addbjets_deltaR_nosel[nChannel];
    TH1D *h_gen_addbjets_invMass_nosel[nChannel];
    TH1D *h_gen_addbjets_deltaR[nChannel][nStep];
    TH1D *h_gen_addbjets_invMass[nChannel][nStep];
    TH1D *h_gen_totjets_invMass[nChannel][nStep];

    //etc : mode = 3
    TH1D *h_stability_deltaR[nChannel][nStep];
    TH1D *h_purity_deltaR[nChannel][nStep];
    TH1D *h_acceptance_deltaR[nChannel][nStep];
    
    TH1D *h_stability_invMass[nChannel][nStep];
    TH1D *h_purity_invMass[nChannel][nStep];
    TH1D *h_acceptance_invMass[nChannel][nStep];

    TH1D *h_stability_totMass[nChannel][nStep];
    TH1D *h_purity_totMass[nChannel][nStep];

    HistoBook(){};
    HistoBook(const int _mode, const char *_process);
    //~Histo_Book();
};

HistoBook::HistoBook(const int _mode, const char *_process){
  if(_mode == 1){
    for(int iChannel=0; iChannel<nChannel; ++iChannel){
      for(int iStep=0; iStep<nStep; ++iStep){
        h_lepton_pt[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",RECO_LEPTON_PT_,iChannel,iStep,_process),"",
	    xNbins_lepton_pt,lepton_pt_min,lepton_pt_max //lepton_pt_width
	    );
	h_lepton_pt[iChannel][iStep]->SetXTitle(Form("%s p_{T} (GeV)",v_chName[iChannel].c_str()));
	h_lepton_pt[iChannel][iStep]->SetYTitle("Entries");
	h_lepton_pt[iChannel][iStep]->Sumw2();

	h_lepton_eta[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",RECO_LEPTON_ETA_,iChannel,iStep,_process),"",
	    xNbins_lepton_eta,lepton_eta_min,lepton_eta_max //lepton_eta_width
	    );
	h_lepton_eta[iChannel][iStep]->SetXTitle(Form("%s #eta",v_chName[iChannel].c_str()));
	h_lepton_eta[iChannel][iStep]->SetYTitle("Entries");
	h_lepton_eta[iChannel][iStep]->Sumw2();

	h_njets[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",RECO_NUMBER_OF_JETS_,iChannel,iStep,_process),"",
	    xNbins_njets,njets_min,njets_max //njets_width
	    );
	h_njets[iChannel][iStep]->SetXTitle("Jet multiplicity");
	h_njets[iChannel][iStep]->SetYTitle("Entries");
	h_njets[iChannel][iStep]->Sumw2();

	h_nbjets[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",RECO_NUMBER_OF_BJETS_,iChannel,iStep,_process),"",
	    xNbins_nbjets,nbjets_min,nbjets_max //nbjets_width
	    );
	h_nbjets[iChannel][iStep]->SetXTitle("bJet multiplicity");
	h_nbjets[iChannel][iStep]->SetYTitle("Entries");
	h_nbjets[iChannel][iStep]->Sumw2();

	h_reco_addjets_deltaR[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",RECO_ADDJETS_DELTAR_,iChannel,iStep,_process),"",
	    xNbins_reco_addjets_dR,reco_addjets_dR_min,reco_addjets_dR_max //reco_addjets_dR_width
	    );
	h_reco_addjets_deltaR[iChannel][iStep]->SetXTitle(Form("#DeltaR_{b#bar{b}}"));
	h_reco_addjets_deltaR[iChannel][iStep]->SetYTitle("Entries");
	h_reco_addjets_deltaR[iChannel][iStep]->Sumw2();

	h_reco_addjets_invMass[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",RECO_ADDJETS_INVARIANT_MASS_,iChannel,iStep,_process),"",
	    xNbins_reco_addjets_M,reco_addjets_M_min,reco_addjets_M_max //reco_addjets_M_width
	    );
	h_reco_addjets_invMass[iChannel][iStep]->SetXTitle(Form("M_{b#bar{b}}(GeV)"));
	h_reco_addjets_invMass[iChannel][iStep]->SetYTitle("Entries");
	h_reco_addjets_invMass[iChannel][iStep]->Sumw2();

	h_reco_totjets_invMass[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",RECO_TOTJETS_INVARIANT_MASS_,iChannel,iStep,_process),"",
	    xNbins_reco_totjets_M,
	    reco_totjets_M_min, reco_totjets_M_max
	    //reco_addjets_M_width
	    );
	h_reco_totjets_invMass[iChannel][iStep]->SetXTitle(Form("M_{jets}(GeV)"));
	h_reco_totjets_invMass[iChannel][iStep]->SetYTitle("Entries");
	h_reco_totjets_invMass[iChannel][iStep]->Sumw2();
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
	    //reco_addjets_dR_width
	    xNbins_gen_addbjets_dR,
	    //gen_addbjets_dR_min, gen_addbjets_dR_max,
	    gen_addbjets_dR_width
	    	    );
	h_respMatrix_deltaR[iChannel][iStep]->SetYTitle("Gen. #DeltaR_{b#bar{b}}");
	h_respMatrix_deltaR[iChannel][iStep]->SetXTitle("Reco. #DeltaR_{b#bar{b}}");
	h_respMatrix_deltaR[iChannel][iStep]->Sumw2();

        h_respMatrix_invMass[iChannel][iStep] = new TH2D(Form("h_%s_Ch%d_S%d_%s",RESPONSE_MATRIX_INVARIANT_MASS_,iChannel,iStep,_process),"",
	    xNbins_reco_addjets_M,
	    reco_addjets_M_min, reco_addjets_M_max,
	    //reco_addjets_M_width
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
      for(int iStep=0; iStep<nStep; ++iStep){
	h_stability_deltaR[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",BIN_STABILITY_DELTAR_,iChannel,iStep,_process),"",
	    xNbins_gen_addbjets_dR,
	    //gen_addbjets_dR_min, gen_addbjets_dR_max
	    gen_addbjets_dR_width
	    );
	h_stability_deltaR[iChannel][iStep]->SetXTitle("#DeltaR_{b#bar{b}}");
	h_stability_deltaR[iChannel][iStep]->SetYTitle("stability");
	h_stability_deltaR[iChannel][iStep]->Sumw2();

	h_purity_deltaR[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",BIN_PURITY_DELTAR_,iChannel,iStep,_process),"",
	    xNbins_gen_addbjets_dR,
	    //gen_addbjets_dR_min, gen_addbjets_dR_max
	    gen_addbjets_dR_width
	    );
	h_purity_deltaR[iChannel][iStep]->SetXTitle("#DeltaR_{b#bar{b}}");
	h_purity_deltaR[iChannel][iStep]->SetYTitle("purity");
	h_purity_deltaR[iChannel][iStep]->Sumw2();

	h_acceptance_deltaR[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",BIN_ACCEPTANCE_DELTAR_,iChannel,iStep,_process),"",
	    xNbins_gen_addbjets_dR,
	    //gen_addbjets_dR_min, gen_addbjets_dR_max
	    gen_addbjets_dR_width
	    );
	h_acceptance_deltaR[iChannel][iStep]->SetXTitle("#DeltaR_{b#bar{b}}");
	h_acceptance_deltaR[iChannel][iStep]->SetYTitle("acceptance");
	h_acceptance_deltaR[iChannel][iStep]->Sumw2();
	
	h_stability_invMass[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",BIN_STABILITY_INVARIANT_MASS_,iChannel,iStep,_process),"",
	    xNbins_gen_addbjets_M,
	    //gen_addbjets_M_min, gen_addbjets_M_max
	    gen_addbjets_M_width
	    );
	h_stability_invMass[iChannel][iStep]->SetXTitle("M_{b#bar{b}}(GeV)");
	h_stability_invMass[iChannel][iStep]->SetYTitle("stability");
	h_stability_invMass[iChannel][iStep]->Sumw2();

	h_purity_invMass[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",BIN_PURITY_INVARIANT_MASS_,iChannel,iStep,_process),"",
	    xNbins_gen_addbjets_M,
	    //gen_addbjets_M_min, gen_addbjets_M_max
	    gen_addbjets_M_width
	    );
	h_purity_invMass[iChannel][iStep]->SetXTitle("M_{b#bar{b}}(GeV)");
	h_purity_invMass[iChannel][iStep]->SetYTitle("purity");
	h_purity_invMass[iChannel][iStep]->Sumw2();

	h_acceptance_invMass[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",BIN_ACCEPTANCE_INVARIANT_MASS_,iChannel,iStep,_process),"",
	    xNbins_gen_addbjets_M,
	    //gen_addbjets_M_min, gen_addbjets_M_max
	    gen_addbjets_M_width
	    );
	h_acceptance_invMass[iChannel][iStep]->SetXTitle("M_{b#bar{b}}(GeV)");
	h_acceptance_invMass[iChannel][iStep]->SetYTitle("acceptance");
	h_acceptance_invMass[iChannel][iStep]->Sumw2();
	
	h_stability_totMass[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",BIN_STABILITY_TOTALJETS_INVARIANT_MASS_,iChannel,iStep,_process),"",
	    xNbins_gen_totjets_M,
	    gen_totjets_M_min, gen_totjets_M_max
	    //gen_addbjets_M_width
	    );
	h_stability_totMass[iChannel][iStep]->SetXTitle("M_{b#bar{b}}(GeV)");
	h_stability_totMass[iChannel][iStep]->SetYTitle("stability");
	h_stability_totMass[iChannel][iStep]->Sumw2();

	h_purity_totMass[iChannel][iStep] = new TH1D(Form("h_%s_Ch%d_S%d_%s",BIN_PURITY_TOTALJETS_INVARIANT_MASS_,iChannel,iStep,_process),"",
	    xNbins_gen_totjets_M,
	    gen_totjets_M_min, gen_totjets_M_max
	    //gen_addbjets_M_width
	    );
	h_purity_totMass[iChannel][iStep]->SetXTitle("M_{b#bar{b}}(GeV)");
	h_purity_totMass[iChannel][iStep]->SetYTitle("purity");
	h_purity_totMass[iChannel][iStep]->Sumw2();


      }//step
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
