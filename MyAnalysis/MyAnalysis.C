#define MyAnalysis_cxx
#include "MyAnalysis.h"
#include <TH2.h>
#include <TStyle.h>
using namespace TMath;
using namespace std;

void MyAnalysis::Begin(TTree * /*tree*/){
  option = GetOption();
}

void MyAnalysis::SlaveBegin(TTree * /*tree*/){
  option = GetOption();
  const char *sample = option.Data();

  h_control = new HistoBook(1, sample);

  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    for(int iStep=0; iStep<nStep; ++iStep){
      fOutput->Add(h_control->h_lepton_pt[iChannel][iStep]);
      fOutput->Add(h_control->h_lepton_eta[iChannel][iStep]);
      fOutput->Add(h_control->h_njets[iChannel][iStep]);
      fOutput->Add(h_control->h_nbjets[iChannel][iStep]);
      fOutput->Add(h_control->h_reco_addjets_deltaR[iChannel][iStep]);
      fOutput->Add(h_control->h_reco_addjets_invMass[iChannel][iStep]);
    }
  }
}

Bool_t MyAnalysis::Process(Long64_t entry){
  fReader.SetEntry(entry);
  option = GetOption();
  process = option.Data();

  const int mode = *channel;
  if(mode>2) return kTRUE;

  const float genweight = *genWeight;
  const float puweight = PUWeight[0];
  double EventWeight = puweight*genweight;
  //const double relIso = *lepton_relIso;
  
  //Object selection
  int njets = 0;
  int nbjets = 0;

  TLorentzVector p4met;
  const double met = *MET;
  const double met_phi = *MET_phi;
  const double apt = TMath::Abs(met);
  const double met_x = apt*TMath::Cos(met_phi);
  const double met_y = apt*TMath::Sin(met_phi);
  p4met.SetPxPyPzE( met_x, met_y, 0, met);

  TLorentzVector lepton;
  lepton.SetPtEtaPhiE(*lepton_pT, *lepton_eta, *lepton_phi, *lepton_E);

  const double transverseM = transverseMass(lepton, p4met);
  //const bool isQCD = transverseM < 20;
  //const bool doQCDestimation = false; // change this flags if you want to do the QCD estimation
  //const bool isIso = doQCDestimation ? *lepton_isIso  : false ;

  //Object selection
  const bool passmuon = (mode == MUON_) && (lepton.Pt() > MUON_PT_) && (abs(lepton.Eta()) < MUON_ETA_);
  const bool passelectron = (mode == ELECTRON_) && (lepton.Pt() > ELECTRON_PT_) && (abs(lepton.Eta()) < ELECTRON_ETA_);

  if ( !passmuon and !passelectron ) return kTRUE;

  if( !process.Contains("Data") ) {
    EventWeight *= lepton_SF[0];
    EventWeight *= jet_SF_CSV[0];
  }

  //multimap<float /*jet_CSV*/, TLorentzVector /*jet_4-momentum*/> m_jetCSV;
  vector<TLorentzVector /*kinJet_4-momentum*/> v_reco_bjets;
  for (unsigned int iJet = 0; iJet < jet_pT.GetSize() ; ++iJet) {
    float jetSystVar = 1.0;
    if( !process.Contains("Data") ) jetSystVar = jet_JER_Nom[iJet];
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pT[iJet], jet_eta[iJet], jet_phi[iJet], jet_E[iJet]);
    jet *= jetSystVar;
    if ( jet.Pt() <= JET_PT_ || abs(jet.Eta()) >= JET_ETA_ ) continue;
    ++njets;
    //if( jet_CSV[iJet] > JET_CSV_MEDIUM_ ) ++nbjets;
    if( jet_CSV[iJet] > JET_CSV_TIGHT_ ){
      ++nbjets;
      v_reco_bjets.push_back(jet);
    }
    //if( jet_CvsL[iJet] > -0.1 && jet_CvsL[iJet] > 0.08 )  ++ncjets_m;
  }

  TLorentzVector reco_addJet1, reco_addJet2;
  double reco_addJet_deltaR = -999.0;
  double reco_addJet_invMass = -999.0;
  //double reco_addJet_CSV[2] = {-999.0, -999.0};
  if ( v_reco_bjets.size() >= 2 ){
    for(unsigned int i=0; i<v_reco_bjets.size()-1; ++i){
      for(unsigned int j=i+1; j<v_reco_bjets.size(); ++j){
        double tmp_dR = v_reco_bjets[i].DeltaR(v_reco_bjets[j]);
      
        if(tmp_dR<abs(reco_addJet_deltaR)){
	  reco_addJet1 = v_reco_bjets[i];
	  reco_addJet2 = v_reco_bjets[j];

	  reco_addJet_deltaR = tmp_dR;
	  reco_addJet_invMass = (reco_addJet1+reco_addJet2).M();
	}
      }
    }
  } 
 
  int passchannel = -999;
  int passcut = 0;
 
  if(passmuon && !passelectron) passchannel = MUON_;
  else if(!passmuon && passelectron) passchannel = ELECTRON_;
  //else return kTRUE;

  if(njets >= NUMBER_OF_JETS_){
    ++passcut;
    if(nbjets >= NUMBER_OF_BJETS_){
      ++passcut;
      if(nbjets >= NUMBER_OF_BJETS_+1) ++passcut;
    }
  }

  for(int iCut=0; iCut <= passcut; ++iCut){
    h_control->h_lepton_pt[passchannel][iCut]->Fill(lepton.Pt(),EventWeight);
    h_control->h_lepton_eta[passchannel][iCut]->Fill(abs(lepton.Eta()),EventWeight);
    h_control->h_njets[passchannel][iCut]->Fill(njets,EventWeight);
    h_control->h_nbjets[passchannel][iCut]->Fill(nbjets,EventWeight);
    h_control->h_reco_addjets_deltaR[passchannel][iCut]->Fill(reco_addJet_deltaR,EventWeight);
    h_control->h_reco_addjets_invMass[passchannel][iCut]->Fill(reco_addJet_invMass,EventWeight);
  }
  
  return kTRUE;
}

void MyAnalysis::SlaveTerminate(){
  option = GetOption();
  
  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    for(int iStep=0; iStep<nStep; ++iStep){
      h_control->h_lepton_pt[iChannel][iStep]->AddBinContent(h_control->xNbins_lepton_pt,
	  h_control->h_lepton_pt[iChannel][iStep]->GetBinContent(h_control->xNbins_lepton_pt+1));
      h_control->h_lepton_pt[iChannel][iStep]->ClearUnderflowAndOverflow();
      h_control->h_lepton_eta[iChannel][iStep]->AddBinContent(h_control->xNbins_lepton_eta,
	  h_control->h_lepton_pt[iChannel][iStep]->GetBinContent(h_control->xNbins_lepton_eta+1));
      h_control->h_lepton_eta[iChannel][iStep]->ClearUnderflowAndOverflow();
      h_control->h_njets[iChannel][iStep]->AddBinContent(h_control->xNbins_njets,
	  h_control->h_njets[iChannel][iStep]->GetBinContent(h_control->xNbins_njets+1));
      h_control->h_njets[iChannel][iStep]->ClearUnderflowAndOverflow();
      h_control->h_nbjets[iChannel][iStep]->AddBinContent(h_control->xNbins_nbjets,
	  h_control->h_nbjets[iChannel][iStep]->GetBinContent(h_control->xNbins_nbjets+1));
      h_control->h_nbjets[iChannel][iStep]->ClearUnderflowAndOverflow();
      h_control->h_reco_addjets_deltaR[iChannel][iStep]->AddBinContent(h_control->xNbins_reco_addjets_dR,
	  h_control->h_reco_addjets_deltaR[iChannel][iStep]->GetBinContent(h_control->xNbins_reco_addjets_dR+1));
      h_control->h_reco_addjets_deltaR[iChannel][iStep]->ClearUnderflowAndOverflow();
      h_control->h_reco_addjets_invMass[iChannel][iStep]->AddBinContent(h_control->xNbins_reco_addjets_M,
	  h_control->h_reco_addjets_invMass[iChannel][iStep]->GetBinContent(h_control->xNbins_reco_addjets_M+1));
      h_control->h_reco_addjets_invMass[iChannel][iStep]->ClearUnderflowAndOverflow();
    }
  }
}

void MyAnalysis::Terminate(){
  option = GetOption();

  //TFile *out = TFile::Open(Form("output/root/sys/hist_%s.root", option.Data()), "RECREATE");
  TFile *out = TFile::Open(Form("output/root/hist_%s.root", option.Data()), "RECREATE");

  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    for(int iStep=0; iStep<nStep; ++iStep){
      fOutput->FindObject(Form("h_%s_Ch%d_S%d_%s",RECO_LEPTON_PT_,iChannel,iStep,option.Data()))->Write();
      fOutput->FindObject(Form("h_%s_Ch%d_S%d_%s",RECO_LEPTON_ETA_,iChannel,iStep,option.Data()))->Write();
      fOutput->FindObject(Form("h_%s_Ch%d_S%d_%s",RECO_NUMBER_OF_JETS_,iChannel,iStep,option.Data()))->Write();
      fOutput->FindObject(Form("h_%s_Ch%d_S%d_%s",RECO_NUMBER_OF_BJETS_,iChannel,iStep,option.Data()))->Write();
      fOutput->FindObject(Form("h_%s_Ch%d_S%d_%s",RECO_ADDJETS_DELTAR_,iChannel,iStep,option.Data()))->Write();
      fOutput->FindObject(Form("h_%s_Ch%d_S%d_%s",RECO_ADDJETS_INVARIANT_MASS_,iChannel,iStep,option.Data()))->Write();
    }
  }
  out->Write();
  out->Close();
}

double MyAnalysis::transverseMass( const TLorentzVector & lepton, const TLorentzVector & met){

  TLorentzVector leptonT(lepton.Px(),lepton.Py(),0.,lepton.E()*TMath::Sin(lepton.Theta()));
  TLorentzVector metT(met.Px(), met.Py(), 0, met.E());

  return (leptonT+metT).M();
}
