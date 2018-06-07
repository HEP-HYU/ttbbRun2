#include <TLorentzVector.h>

#include "../include/histBook.h"
#include "../include/tdrstyle.C"

using namespace std;

void makeMatrix(){
  //FILE
  TFile *inFile = TFile::Open(Form("/data/users/seohyun/ntuple/hep2017/v806/TT_powheg_ttbb.root"));
  TFile *outFile = TFile::Open(Form("../output/root/hist_respMatrix_ttbb_test.root"),"recreate");

  //TREE READER
  TTreeReader treeReader("ttbbLepJets/tree",inFile);
  TTreeReaderValue<Int_t> event = {treeReader, "event"};
  TTreeReaderValue<Int_t> channel = {treeReader, "channel"};
  TTreeReaderValue<Float_t> genWeight = {treeReader, "genweight"};
  TTreeReaderArray<float> PUWeight = {treeReader, "PUWeight"};
  TTreeReaderValue<Float_t> lepton_pT = {treeReader, "lepton_pT"};
  TTreeReaderValue<Float_t> lepton_eta = {treeReader, "lepton_eta"};
  TTreeReaderValue<Float_t> lepton_phi = {treeReader, "lepton_phi"};
  TTreeReaderValue<Float_t> lepton_E = {treeReader, "lepton_E"};
  TTreeReaderArray<float> lepton_SF = {treeReader, "lepton_SF"};
  TTreeReaderArray<float> jet_pT = {treeReader, "jet_pT"};
  TTreeReaderArray<float> jet_eta = {treeReader, "jet_eta"};
  TTreeReaderArray<float> jet_phi = {treeReader, "jet_phi"};
  TTreeReaderArray<float> jet_E = {treeReader, "jet_E"};
  TTreeReaderArray<int> jet_index = {treeReader, "jet_index"};
  TTreeReaderArray<float> jet_CSV = {treeReader, "jet_CSV"};
  TTreeReaderArray<float> jet_SF_CSV = {treeReader, "jet_SF_CSV"};
  TTreeReaderArray<int> kinjet_index = {treeReader, "kinjet_index"};
  TTreeReaderArray<float> jet_JER_Nom = {treeReader, "jet_JER_Nom"};
  TTreeReaderValue<float> addbjet1_pt = {treeReader, "addbjet1_pt"};
  TTreeReaderValue<float> addbjet1_eta = {treeReader, "addbjet1_eta"};
  TTreeReaderValue<float> addbjet1_phi = {treeReader, "addbjet1_phi"};
  TTreeReaderValue<float> addbjet1_e = {treeReader, "addbjet1_e"};
  TTreeReaderValue<float> addbjet2_pt = {treeReader, "addbjet2_pt"};
  TTreeReaderValue<float> addbjet2_eta = {treeReader, "addbjet2_eta"};
  TTreeReaderValue<float> addbjet2_phi = {treeReader, "addbjet2_phi"};
  TTreeReaderValue<float> addbjet2_e = {treeReader, "addbjet2_e"};
  TTreeReaderArray<float> genjet_pT = {treeReader, "genjet_pT"};
  TTreeReaderArray<float> genjet_eta = {treeReader, "genjet_eta"};
  TTreeReaderArray<float> genjet_phi = {treeReader, "genjet_phi"};
  TTreeReaderArray<float> genjet_E = {treeReader, "genjet_E"};

  TTreeReader gentreeReader("ttbbLepJets/gentree", inFile);
  TTreeReaderValue<Int_t> gen_event = {gentreeReader, "event"};
  TTreeReaderValue<Float_t> gen_weight = {gentreeReader, "genweight"};
  TTreeReaderValue<Int_t> gen_channel = {gentreeReader, "genchannel"};
  TTreeReaderValue<float> gen_addbjet1_pt = {gentreeReader, "addbjet1_pt"};
  TTreeReaderValue<float> gen_addbjet1_eta = {gentreeReader, "addbjet1_eta"};
  TTreeReaderValue<float> gen_addbjet1_phi = {gentreeReader, "addbjet1_phi"};
  TTreeReaderValue<float> gen_addbjet1_e = {gentreeReader, "addbjet1_e"};
  TTreeReaderValue<float> gen_addbjet2_pt = {gentreeReader, "addbjet2_pt"};
  TTreeReaderValue<float> gen_addbjet2_eta = {gentreeReader, "addbjet2_eta"};
  TTreeReaderValue<float> gen_addbjet2_phi = {gentreeReader, "addbjet2_phi"};
  TTreeReaderValue<float> gen_addbjet2_e = {gentreeReader, "addbjet2_e"};

  HistoBook *h_matrix = new HistoBook(2, NAME_[TTBB_].c_str());
  HistoBook *h_control = new HistoBook(1, NAME_[TTBB_].c_str());
 
  const int xNbins_gen_addbjets_dR = h_control->xNbins_gen_addbjets_dR;
  const int xNbins_gen_addbjets_M = h_control->xNbins_gen_addbjets_M;
  const int xNbins_reco_addjets_dR = h_control->xNbins_reco_addjets_dR;
  const int xNbins_reco_addjets_M = h_control->xNbins_reco_addjets_M;
  
  int nevt = 0;
  while(treeReader.Next()){
    const int mode = *channel;
    const double eventWeight = jet_SF_CSV[0]*lepton_SF[0]*PUWeight[0]*(*genWeight);
    TLorentzVector lepton;
    lepton.SetPtEtaPhiE(*lepton_pT, *lepton_eta, *lepton_phi, *lepton_E);
    //Object selection
    const bool passmuon = (mode == MUON_) && (lepton.Pt() > MUON_PT_) && (abs(lepton.Eta()) < MUON_ETA_);
    const bool passelectron = (mode == ELECTRON_) && (lepton.Pt() > ELECTRON_PT_) && (abs(lepton.Eta()) < ELECTRON_ETA_);
    if( !passmuon and !passelectron ) continue;

    int njets = 0; int nbjets_t = 0;
    vector<TLorentzVector /*jet 4-momentum*/> v_reco_bjets;
     
    for( unsigned int iJet=0;iJet < jet_pT.GetSize(); ++iJet ){
      TLorentzVector jet;
      jet.SetPtEtaPhiE(jet_pT[iJet],jet_eta[iJet],jet_phi[iJet],jet_E[iJet]);
      jet *= jet_JER_Nom[iJet];
      if( jet.Pt() <= JET_PT_ || abs(jet.Eta()) >= JET_ETA_ ) continue;
      ++njets;
      if( jet_CSV[iJet] > JET_CSV_TIGHT_ ){
	++nbjets_t;
	v_reco_bjets.push_back(jet);
      }
    }//Jet selection

    TLorentzVector reco_addBjet1, reco_addBjet2;
    double reco_addJets_deltaR = -999.0;
    double reco_addJets_invMass = -999.0;
    if(v_reco_bjets.size() >= 2){
      for(int i=0; i<v_reco_bjets.size()-1; ++i){
	for(int j=i+1; j<v_reco_bjets.size(); ++j){
	  double tmp_dR = v_reco_bjets[i].DeltaR(v_reco_bjets[j]);

	  if(tmp_dR<abs(reco_addJets_deltaR)){
	    reco_addBjet1 = v_reco_bjets[i];
	    reco_addBjet2 = v_reco_bjets[j];

	    reco_addJets_deltaR = tmp_dR;
	    reco_addJets_invMass = (reco_addBjet1+reco_addBjet2).M();
	  }
	}
      }
    }

    TLorentzVector gen_addBjet1, gen_addBjet2;
    double gen_addbJets_deltaR = -999.0;
    double gen_addbJets_invMass = -999.0;
    gen_addBjet1.SetPtEtaPhiE(*addbjet1_pt, *addbjet1_eta, *addbjet1_phi, *addbjet1_e);
    gen_addBjet2.SetPtEtaPhiE(*addbjet2_pt, *addbjet2_eta, *addbjet2_phi, *addbjet2_e);

    gen_addbJets_deltaR = gen_addBjet1.DeltaR(gen_addBjet2);
    gen_addbJets_invMass = (gen_addBjet1+gen_addBjet2).M();

    int passchannel = -999;
    int passcut = 0;
    if(passmuon && !passelectron) passchannel = MUON_;
    else if(!passmuon && passelectron) passchannel = ELECTRON_;
    else continue;

    if(njets >= NUMBER_OF_JETS_) ++passcut; //number of jets >= 6
    if(nbjets_t >= NUMBER_OF_BJETS_) ++passcut; //number of bjets >=2
    if(nbjets_t >= NUMBER_OF_BJETS_+1) ++passcut; //number of bjets >=3

    for(int iCut = 0; iCut <= passcut; ++iCut){
      h_control->h_reco_addjets_deltaR[passchannel][iCut]->Fill(reco_addJets_deltaR, eventWeight);
      h_control->h_reco_addjets_invMass[passchannel][iCut]->Fill(reco_addJets_invMass, eventWeight);
      h_matrix->h_respMatrix_deltaR[passchannel][iCut]->Fill(reco_addJets_deltaR, gen_addbJets_deltaR, eventWeight);
      h_matrix->h_respMatrix_invMass[passchannel][iCut]->Fill(reco_addJets_invMass, gen_addbJets_invMass, eventWeight);
      h_matrix->h_gen_addbjets_deltaR[passchannel][iCut]->Fill(gen_addbJets_deltaR, eventWeight);
      h_matrix->h_gen_addbjets_invMass[passchannel][iCut]->Fill(gen_addbJets_invMass, eventWeight);
    }
    ++nevt;
    if(nevt%1000==0) cout << "Event : " << nevt << endl;
  }//Reco tree reader
  
  while(gentreeReader.Next()){
    const int mode = *gen_channel;
    const float genweight = *gen_weight;

    TLorentzVector gen_addBjet1, gen_addBjet2;
    double gen_addbJets_deltaR = -999.0;
    double gen_addbJets_invMass = -999.0;
    gen_addBjet1.SetPtEtaPhiE(*gen_addbjet1_pt, *gen_addbjet1_eta, *gen_addbjet1_phi, *gen_addbjet1_e);
    gen_addBjet2.SetPtEtaPhiE(*gen_addbjet2_pt, *gen_addbjet2_eta, *gen_addbjet2_phi, *gen_addbjet2_e);

    gen_addbJets_deltaR = gen_addBjet1.DeltaR(gen_addBjet2);
    gen_addbJets_invMass = (gen_addBjet1+gen_addBjet2).M();

    if(*gen_channel == MUON_){
      h_matrix->h_gen_addbjets_deltaR_nosel[MUON_]->Fill(gen_addbJets_deltaR, genweight);
      h_matrix->h_gen_addbjets_invMass_nosel[MUON_]->Fill(gen_addbJets_invMass, genweight);
    }
    else if(*gen_channel == ELECTRON_){
      h_matrix->h_gen_addbjets_deltaR_nosel[ELECTRON_]->Fill(gen_addbJets_deltaR, genweight);
      h_matrix->h_gen_addbjets_invMass_nosel[ELECTRON_]->Fill(gen_addbJets_invMass, genweight);
    }
    else
      cout << "Warning : There is no such channel" << endl;
  }
  
  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    h_matrix->h_gen_addbjets_deltaR_nosel[iChannel]->AddBinContent(xNbins_gen_addbjets_dR,
	h_matrix->h_gen_addbjets_deltaR_nosel[iChannel]->GetBinContent(xNbins_gen_addbjets_dR+1));
    h_matrix->h_gen_addbjets_deltaR_nosel[iChannel]->ClearUnderflowAndOverflow();
    h_matrix->h_gen_addbjets_invMass_nosel[iChannel]->AddBinContent(xNbins_gen_addbjets_M,
	h_matrix->h_gen_addbjets_invMass_nosel[iChannel]->GetBinContent(xNbins_gen_addbjets_M+1));

    for(int iStep=0; iStep<nStep; ++iStep){
      h_control->h_reco_addjets_deltaR[iChannel][iStep]->AddBinContent(xNbins_reco_addjets_dR,
	  h_control->h_reco_addjets_deltaR[iChannel][iStep]->GetBinContent(xNbins_reco_addjets_dR+1));
      h_control->h_reco_addjets_deltaR[iChannel][iStep]->ClearUnderflowAndOverflow();
      h_control->h_reco_addjets_invMass[iChannel][iStep]->AddBinContent(xNbins_reco_addjets_M,
	  h_control->h_reco_addjets_invMass[iChannel][iStep]->GetBinContent(xNbins_reco_addjets_M+1));
      h_control->h_reco_addjets_invMass[iChannel][iStep]->ClearUnderflowAndOverflow();
      h_matrix->h_gen_addbjets_deltaR[iChannel][iStep]->AddBinContent(xNbins_gen_addbjets_dR,
	  h_matrix->h_gen_addbjets_deltaR[iChannel][iStep]->GetBinContent(xNbins_gen_addbjets_dR+1));
      h_matrix->h_gen_addbjets_deltaR[iChannel][iStep]->ClearUnderflowAndOverflow();
      h_matrix->h_gen_addbjets_invMass[iChannel][iStep]->AddBinContent(xNbins_gen_addbjets_M,
	  h_matrix->h_gen_addbjets_invMass[iChannel][iStep]->GetBinContent(xNbins_gen_addbjets_M+1));
      h_matrix->h_gen_addbjets_invMass[iChannel][iStep]->ClearUnderflowAndOverflow();

      for(int iYaxis=1; iYaxis<=xNbins_gen_addbjets_dR; ++iYaxis){
	cout << "dR Xaxis Overflow : " << h_matrix->h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(xNbins_reco_addjets_dR+1, iYaxis) << endl;
	double tmp = h_matrix->h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(xNbins_reco_addjets_dR, iYaxis)+
	  h_matrix->h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(xNbins_reco_addjets_dR+1, iYaxis);
	h_matrix->h_respMatrix_deltaR[iChannel][iStep]->SetBinContent(xNbins_reco_addjets_dR, iYaxis, tmp);

	cout << "dR Xaxis Underfolw : " << h_matrix->h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(0, iYaxis) << endl;
	double tmp2 = h_matrix->h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(0, iYaxis)+
	  h_matrix->h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(1, iYaxis);
	h_matrix->h_respMatrix_deltaR[iChannel][iStep]->SetBinContent(1, iYaxis, tmp2);
      }
      for(int iXaxis=1; iXaxis<=xNbins_reco_addjets_dR; ++iXaxis){
	cout << "dR Yaxis Overflow : " << h_matrix->h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(iXaxis, xNbins_gen_addbjets_dR+1) << endl;
        double tmp = h_matrix->h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(iXaxis, xNbins_gen_addbjets_dR)+
	  h_matrix->h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(iXaxis, xNbins_gen_addbjets_dR+1);
	h_matrix->h_respMatrix_deltaR[iChannel][iStep]->SetBinContent(iXaxis, xNbins_gen_addbjets_dR, tmp);

	cout << "dR Yaxis Underflow : " << h_matrix->h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(iXaxis, 0) << endl;
	double tmp2 = h_matrix->h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(iXaxis, 0)+
	  h_matrix->h_respMatrix_deltaR[iChannel][iStep]->GetBinContent(iXaxis, 1);
	h_matrix->h_respMatrix_deltaR[iChannel][iStep]->SetBinContent(iXaxis, 1, tmp2);
      }

      for(int iYaxis=1; iYaxis<=xNbins_gen_addbjets_M; ++iYaxis){
	cout << "M Xaxis Overflow : " << h_matrix->h_respMatrix_invMass[iChannel][iStep]->GetBinContent(xNbins_reco_addjets_M+1, iYaxis) << endl;
	double tmp = h_matrix->h_respMatrix_invMass[iChannel][iStep]->GetBinContent(xNbins_reco_addjets_M, iYaxis)+
	  h_matrix->h_respMatrix_invMass[iChannel][iStep]->GetBinContent(xNbins_reco_addjets_M+1, iYaxis);
	h_matrix->h_respMatrix_invMass[iChannel][iStep]->SetBinContent(xNbins_reco_addjets_M, iYaxis, tmp);

	cout << "M Xaxis Underflow : " << h_matrix->h_respMatrix_invMass[iChannel][iStep]->GetBinContent(0, iYaxis) << endl; 
	double tmp2 = h_matrix->h_respMatrix_invMass[iChannel][iStep]->GetBinContent(0, iYaxis)+
	  h_matrix->h_respMatrix_invMass[iChannel][iStep]->GetBinContent(1, iYaxis);
	h_matrix->h_respMatrix_invMass[iChannel][iStep]->SetBinContent(1, iYaxis, tmp2);
      }
      for(int iXaxis=1; iXaxis<=xNbins_reco_addjets_M; ++iXaxis){
	cout << "M Yaxis Overflow : " << h_matrix->h_respMatrix_invMass[iChannel][iStep]->GetBinContent(iXaxis, xNbins_gen_addbjets_M+1) << endl;
        double tmp = h_matrix->h_respMatrix_invMass[iChannel][iStep]->GetBinContent(iXaxis, xNbins_gen_addbjets_M)+
	  h_matrix->h_respMatrix_invMass[iChannel][iStep]->GetBinContent(iXaxis, xNbins_gen_addbjets_M+1);
	h_matrix->h_respMatrix_invMass[iChannel][iStep]->SetBinContent(iXaxis, xNbins_gen_addbjets_M, tmp);

	cout << "M Yaxis Underflow : " << h_matrix->h_respMatrix_invMass[iChannel][iStep]->GetBinContent(iXaxis, 0) << endl;
	double tmp2 = h_matrix->h_respMatrix_invMass[iChannel][iStep]->GetBinContent(iXaxis, 0)+
	  h_matrix->h_respMatrix_invMass[iChannel][iStep]->GetBinContent(iXaxis, 1);
	h_matrix->h_respMatrix_invMass[iChannel][iStep]->SetBinContent(iXaxis, 1, tmp2);
      }
      h_matrix->h_respMatrix_deltaR[iChannel][iStep]->ClearUnderflowAndOverflow();
      h_matrix->h_respMatrix_invMass[iChannel][iStep]->ClearUnderflowAndOverflow();
    }
  }

  outFile->Write();
  outFile->Close();
}
