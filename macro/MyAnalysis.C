#define MyAnalysis_cxx
#include <TH2.h>
#include <TStyle.h>

#include "MyAnalysis.h"

using namespace TMath;
using namespace std;

void MyAnalysis::Begin(TTree * /*tree*/){
  option = GetOption();
}

void MyAnalysis::SlaveBegin(TTree * /*tree*/){
  option = GetOption();
  string str_opt = option.Data();
  if( option.Contains("__") )
    syst_ext = str_opt.substr(str_opt.find_first_of("_"),string::npos);
  else
    syst_ext = "";

  h_control = new HistoBook(1, syst_ext.c_str());

  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    for(int iStep=0; iStep<nStep; ++iStep){
	fOutput->Add(h_control->h_lepton_pt[iChannel][iStep]);
	fOutput->Add(h_control->h_lepton_eta[iChannel][iStep]);
	fOutput->Add(h_control->h_njets[iChannel][iStep]);
	fOutput->Add(h_control->h_nbjets[iChannel][iStep]);
	fOutput->Add(h_control->h_trans_mass[iChannel][iStep]);
	fOutput->Add(h_control->h_jet_pt_sum[iChannel][iStep]);
	for(int iJet=0; iJet<nJet; ++iJet){
	  fOutput->Add(h_control->h_jet_pt[iChannel][iStep][iJet]);
	  fOutput->Add(h_control->h_jet_eta[iChannel][iStep][iJet]);
	  fOutput->Add(h_control->h_b_discrimi[iChannel][iStep][iJet]);
	}
	fOutput->Add(h_control->h_reco_addjets_deltaR[iChannel][iStep]);
	fOutput->Add(h_control->h_reco_addjets_invMass[iChannel][iStep]);
    }//step
  }//channel
  
  process = option.Data();
  if( process.Contains("TT") || process.Contains("ttHbb")
      || process.Contains("ttW") || process.Contains("ttZ")){
    pdfweight = {fReader, "pdfweight"};
    scaleweight = {fReader, "scaleweight"};
  }
}

Bool_t MyAnalysis::Process(Long64_t entry){
  fReader.SetEntry(entry);
  option = GetOption();
  process = option.Data();

  cout << scaleweight[0] << endl;
  const int mode = *channel;
  if(mode>2) return kTRUE;
 
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
  p4met.SetPxPyPzE(met_x, met_y, 0, met);

  TLorentzVector lepton;
  lepton.SetPtEtaPhiE(*lepton_pT, *lepton_eta, *lepton_phi, *lepton_E);

  //const bool isQCD = transverseM < 20;
  //const bool doQCDestimation = false; // change this flags if you want to do the QCD estimation
  //const bool isIso = doQCDestimation ? *lepton_isIso  : false ;

  //Object selection
  const bool passmuon = (mode == MUON_) && (lepton.Pt() > MUON_PT_) && (abs(lepton.Eta()) < MUON_ETA_);
  const bool passelectron = (mode == ELECTRON_) && (lepton.Pt() > ELECTRON_PT_) && (abs(lepton.Eta()) < ELECTRON_ETA_);

  if ( !passmuon and !passelectron ) return kTRUE;

  double jet_pt_sum = 0.0;
  multimap<float /*jet_CSV*/, TLorentzVector /*jet_4-momentum*/> m_jets;
  vector<TLorentzVector /*jet_4-momentum*/> v_reco_bjets;
  for (unsigned int iJet = 0; iJet < jet_pT.GetSize() ; ++iJet) {
    float jetSystVar = 1.0;
    if( !process.Contains("Data") ){
      if     ( syst_ext == "jecup"   ) jetSystVar = jet_JER_Nom[iJet] * jet_JES_Up[iJet];
      else if( syst_ext == "jecdown" ) jetSystVar = jet_JER_Nom[iJet] * jet_JES_Down[iJet];
      else if( syst_ext == "jerup"   ) jetSystVar = jet_JER_Up[iJet];
      else if( syst_ext == "jerdown" ) jetSystVar = jet_JER_Down[iJet];
      else                             jetSystVar = jet_JER_Nom[iJet];
    }

    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pT[iJet], jet_eta[iJet], jet_phi[iJet], jet_E[iJet]);
    jet *= jetSystVar;

    if ( jet.Pt() <= JET_PT_ || abs(jet.Eta()) >= JET_ETA_ ) continue;

    m_jets.insert(pair<float,TLorentzVector>(jet_CSV[iJet],jet));
    jet_pt_sum += jet.Pt();
    ++njets;
    //if( jet_CSV[iJet] > JET_CSV_MEDIUM_ ) ++nbjets;
    if( jet_CSV[iJet] > JET_CSV_TIGHT_ ){
      ++nbjets;
      v_reco_bjets.push_back(jet);
    }
    //if( jet_CvsL[iJet] > -0.1 && jet_CvsL[iJet] > 0.08 )  ++ncjets_m;
  }

  double a_jetCSV[6] = {0,0,0,0,0,0};
  double a_jetPt[6] = {0,0,0,0,0,0};
  double a_jetEta[6] = {0,0,0,0,0,0};
  int tmp_idx = 0;
  for(auto m_itr = m_jets.begin(); m_itr != m_jets.end(); ++m_itr){
    if(tmp_idx >= 6) continue;
    a_jetCSV[tmp_idx] = m_itr->first;
    a_jetPt[tmp_idx] = (m_itr->second).Pt();
    a_jetEta[tmp_idx] = (m_itr->second).Eta();
    ++tmp_idx;
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

 // if( !process.Contains("Data") ) {
 //   EventWeight *= lepton_SF[0];
 //   EventWeight *= jet_SF_CSV[0];
 // }

  for(int iCut=0; iCut <= passcut; ++iCut){
    double EventWeight = 1;
    if( !option.Contains("Data") ){
      EventWeight *= *genWeight;
      ssize_t pos;
      if     ( (pos = syst_ext.find("puup",0)) != std::string::npos )
	EventWeight *= PUWeight[1];
      else if( (pos = syst_ext.find("pudown",0)) != std::string::npos )
	EventWeight *= PUWeight[2];
      else
	EventWeight *= PUWeight[0];
    
      if( passchannel == 0 ){
	//mu [0]~[2]: ID/Iso, [3]~[5]: Trigger
	if     ( (pos = syst_ext.find("musfup",0)) != std::string::npos )
	  EventWeight *= lepton_SF[1];
	else if( (pos = syst_ext.find("musfdown",0)) != std::string::npos )
	  EventWeight *= lepton_SF[2];
	else
	  EventWeight *= lepton_SF[0];

	if     ( (pos = syst_ext.find("mutrgup",0)) != std::string::npos )
	  EventWeight *= lepton_SF[4];
	else if( (pos = syst_ext.find("mutrgdown",0)) != std::string::npos ) 
	  EventWeight *= lepton_SF[5];
	else
	  EventWeight *= lepton_SF[3];
      }
      else if( passchannel == 1){
	//el [0]~[2]: ID/Iso/Reco, [3]~[5]: Trigger
	if     ( (pos = syst_ext.find("elsfup",0)) != std::string::npos )
	  EventWeight *= lepton_SF[1];
	else if( (pos = syst_ext.find("elsfdown",0)) != std::string::npos )
	  EventWeight *= lepton_SF[2];
	else
	  EventWeight *= lepton_SF[0];

	if     ( (pos = syst_ext.find("eltrgup",0)) != std::string::npos )
	  EventWeight *= lepton_SF[4];
	else if( (pos = syst_ext.find("eltrgdown",0)) != std::string::npos )
	  EventWeight *= lepton_SF[5];
	else
	  EventWeight *= lepton_SF[3];
      }
      
      //Scale Weight(ME)
      //[0] = muF up , [1] = muF down, [2] = muR up, [3] = muR up && muF up
      //[4] = muR down, [5] = muF down && muF down
      if(option.Contains("TT") || option.Contains("ttHbb")
	  || option.Contains("ttW") || option.Contains("ttZ")){
	if     ( (pos = syst_ext.find("scale0",0)) != std::string::npos )
	  EventWeight *= scaleweight[0];
	else if( (pos = syst_ext.find("scale1",0)) != std::string::npos )
	  EventWeight *= scaleweight[1];
	else if( (pos = syst_ext.find("scale2",0)) != std::string::npos )
	  EventWeight *= scaleweight[2];
	else if( (pos = syst_ext.find("scale3",0)) != std::string::npos )
	  EventWeight *= scaleweight[3];
	else if( (pos = syst_ext.find("scale4",0)) != std::string::npos )
	  EventWeight *= scaleweight[4];
	else if( (pos = syst_ext.find("scale5",0)) != std::string::npos )
	  EventWeight *= scaleweight[5];
	else EventWeight *= 1.0;
      }
      //CSV Shape
      // Systematics for bottom flavor jets:
      //   Light flavor contamination: lf
      //   Linear and quadratic statistical fluctuations: hfstats1 and hfstats2
      // Systematics for light flavor jets:
      //   Heavy flavor contamination: hf
      //   Linear and quadratic statistical fluctuations: lfstats1 and lfstats2
      // Systematics for charm flavor jets:
      //   Linear and quadratic uncertainties: cferr1 and cferr2
      if     ( (pos = syst_ext.find("lfup",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[3];
      else if( (pos = syst_ext.find("lfdown",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[4];
      else if( (pos = syst_ext.find("hfup",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[5];
      else if( (pos = syst_ext.find("hfdown",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[6];
      else if( (pos = syst_ext.find("hfstat1up",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[7];
      else if( (pos = syst_ext.find("hfstat1down",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[8];
      else if( (pos = syst_ext.find("hfstat2up",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[9];
      else if( (pos = syst_ext.find("hfstat2down",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[10];
      else if( (pos = syst_ext.find("lfstat1up",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[11];
      else if( (pos = syst_ext.find("lfstat1down",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[12];
      else if( (pos = syst_ext.find("lfstat2up",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[13];
      else if( (pos = syst_ext.find("lfstat2down",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[14];
      else if( (pos = syst_ext.find("cferr1up",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[15];
      else if( (pos = syst_ext.find("cferr1down",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[16];
      else if( (pos = syst_ext.find("cferr2up",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[17];
      else if( (pos = syst_ext.find("cferr2down",0)) != std::string::npos )
	EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[18];
      else
	EventWeight *= jet_SF_CSV_30[0];
    }

    h_control->h_lepton_pt[passchannel][iCut]->Fill(lepton.Pt(),EventWeight);
    h_control->h_lepton_eta[passchannel][iCut]->Fill(abs(lepton.Eta()),EventWeight);
    h_control->h_njets[passchannel][iCut]->Fill(njets,EventWeight);
    h_control->h_nbjets[passchannel][iCut]->Fill(nbjets,EventWeight);
    h_control->h_trans_mass[passchannel][iCut]->Fill(transverseMass(lepton,p4met), EventWeight);
    h_control->h_jet_pt_sum[passchannel][iCut]->Fill(jet_pt_sum, EventWeight);
    h_control->h_reco_addjets_deltaR[passchannel][iCut]->Fill(reco_addJet_deltaR,EventWeight);
    h_control->h_reco_addjets_invMass[passchannel][iCut]->Fill(reco_addJet_invMass,EventWeight);
    for(int iJet=0; iJet<nJet; ++iJet){
      h_control->h_jet_pt[passchannel][iCut][iJet]->Fill(a_jetPt[iJet], EventWeight);
      h_control->h_jet_eta[passchannel][iCut][iJet]->Fill(a_jetEta[iJet], EventWeight);
      h_control->h_b_discrimi[passchannel][iCut][iJet]->Fill(a_jetCSV[iJet], EventWeight);
    }//jet
  }//cut
  
  return kTRUE;
}

void MyAnalysis::SlaveTerminate(){
  option = GetOption();
  
  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    for(int iStep=0; iStep<nStep; ++iStep){
      h_control->h_lepton_pt[iChannel][iStep]->AddBinContent(h_control->xNbins_lepton_pt,
	  h_control->h_lepton_pt[iChannel][iStep]->GetBinContent(h_control->xNbins_lepton_pt+1));
      h_control->h_lepton_pt[iChannel][iStep]->AddBinContent(1,
	  h_control->h_lepton_pt[iChannel][iStep]->GetBinContent(0));
      h_control->h_lepton_pt[iChannel][iStep]->ClearUnderflowAndOverflow();

      h_control->h_lepton_eta[iChannel][iStep]->AddBinContent(h_control->xNbins_lepton_eta,
	  h_control->h_lepton_pt[iChannel][iStep]->GetBinContent(h_control->xNbins_lepton_eta+1));
      h_control->h_lepton_eta[iChannel][iStep]->AddBinContent(1,
	  h_control->h_lepton_pt[iChannel][iStep]->GetBinContent(0));
      h_control->h_lepton_eta[iChannel][iStep]->ClearUnderflowAndOverflow();
      
      h_control->h_njets[iChannel][iStep]->AddBinContent(h_control->xNbins_njets,
	  h_control->h_njets[iChannel][iStep]->GetBinContent(h_control->xNbins_njets+1));      
      h_control->h_njets[iChannel][iStep]->AddBinContent(1,
	  h_control->h_njets[iChannel][iStep]->GetBinContent(0));
      h_control->h_njets[iChannel][iStep]->ClearUnderflowAndOverflow();
      
      h_control->h_nbjets[iChannel][iStep]->AddBinContent(h_control->xNbins_nbjets,
	  h_control->h_nbjets[iChannel][iStep]->GetBinContent(h_control->xNbins_nbjets+1));
      h_control->h_nbjets[iChannel][iStep]->AddBinContent(1,
	  h_control->h_nbjets[iChannel][iStep]->GetBinContent(0));
      h_control->h_nbjets[iChannel][iStep]->ClearUnderflowAndOverflow();
      
      h_control->h_jet_pt_sum[iChannel][iStep]->AddBinContent(h_control->xNbins_jet_pt,
	  h_control->h_jet_pt_sum[iChannel][iStep]->GetBinContent(h_control->xNbins_jet_pt+1));
      h_control->h_jet_pt_sum[iChannel][iStep]->AddBinContent(1,
	  h_control->h_jet_pt_sum[iChannel][iStep]->GetBinContent(0));
      h_control->h_jet_pt_sum[iChannel][iStep]->ClearUnderflowAndOverflow();
    
      h_control->h_trans_mass[iChannel][iStep]->AddBinContent(h_control->xNbins_wmass,
	  h_control->h_trans_mass[iChannel][iStep]->GetBinContent(h_control->xNbins_wmass+1));
      h_control->h_trans_mass[iChannel][iStep]->AddBinContent(1,
	  h_control->h_trans_mass[iChannel][iStep]->GetBinContent(0));
      h_control->h_trans_mass[iChannel][iStep]->ClearUnderflowAndOverflow();

      h_control->h_reco_addjets_deltaR[iChannel][iStep]->AddBinContent(h_control->xNbins_reco_addjets_dR,
	  h_control->h_reco_addjets_deltaR[iChannel][iStep]->GetBinContent(h_control->xNbins_reco_addjets_dR+1));
      h_control->h_reco_addjets_deltaR[iChannel][iStep]->ClearUnderflowAndOverflow();
      
      h_control->h_reco_addjets_invMass[iChannel][iStep]->AddBinContent(h_control->xNbins_reco_addjets_M,
	  h_control->h_reco_addjets_invMass[iChannel][iStep]->GetBinContent(h_control->xNbins_reco_addjets_M+1));
      h_control->h_reco_addjets_invMass[iChannel][iStep]->ClearUnderflowAndOverflow();

      for(int iJet=0; iJet<nJet; ++iJet){
	h_control->h_jet_pt[iChannel][iStep][iJet]->AddBinContent(h_control->xNbins_jet_pt,
	    h_control->h_jet_pt[iChannel][iStep][iJet]->GetBinContent(h_control->xNbins_jet_pt+1));
	h_control->h_jet_pt[iChannel][iStep][iJet]->AddBinContent(1,
	    h_control->h_jet_pt[iChannel][iStep][iJet]->GetBinContent(0));
	h_control->h_jet_pt[iChannel][iStep][iJet]->ClearUnderflowAndOverflow();

	h_control->h_jet_eta[iChannel][iStep][iJet]->AddBinContent(h_control->xNbins_jet_eta,
	    h_control->h_jet_eta[iChannel][iStep][iJet]->GetBinContent(h_control->xNbins_jet_eta+1));
	h_control->h_jet_eta[iChannel][iStep][iJet]->AddBinContent(1,
	    h_control->h_jet_eta[iChannel][iStep][iJet]->GetBinContent(0));
	h_control->h_jet_eta[iChannel][iStep][iJet]->ClearUnderflowAndOverflow();

	h_control->h_b_discrimi[iChannel][iStep][iJet]->AddBinContent(h_control->xNbins_b_discrimi,
	    h_control->h_b_discrimi[iChannel][iStep][iJet]->GetBinContent(h_control->xNbins_b_discrimi));
	h_control->h_b_discrimi[iChannel][iStep][iJet]->AddBinContent(1,
	    h_control->h_b_discrimi[iChannel][iStep][iJet]->GetBinContent(0));
	h_control->h_b_discrimi[iChannel][iStep][iJet]->ClearUnderflowAndOverflow();
      }//jet
    }//step
  }//channel
}

void MyAnalysis::Terminate(){
  option = GetOption();
  string str_opt = option.Data();
  if( option.Contains("__") )
    syst_ext = str_opt.substr(str_opt.find_first_of("_"),string::npos);
  else
    syst_ext = "";

  TFile *out = TFile::Open(Form("output/root/hist_%s.root", option.Data()), "RECREATE");

  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    for(int iStep=0; iStep<nStep; ++iStep){
      fOutput->FindObject(Form("h_%s_Ch%d_S%d%s",RECO_LEPTON_PT_,iChannel,iStep,syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_%s_Ch%d_S%d%s",RECO_LEPTON_ETA_,iChannel,iStep,syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_%s_Ch%d_S%d%s",RECO_NUMBER_OF_JETS_,iChannel,iStep,syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_%s_Ch%d_S%d%s",RECO_NUMBER_OF_BJETS_,iChannel,iStep,syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_%s_Ch%d_S%d%s",RECO_TRANSVERSE_MASS_,iChannel,iStep,syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_%s_sum_Ch%d_S%d%s",RECO_JET_PT_,iChannel,iStep,syst_ext.c_str()))->Write();
      for(int iJet=0; iJet<nJet; ++iJet){
	fOutput->FindObject(Form("h_%s_%d_Ch%d_S%d%s",RECO_JET_PT_,iJet,iChannel,iStep,syst_ext.c_str()))->Write();
	fOutput->FindObject(Form("h_%s_%d_Ch%d_S%d%s",RECO_JET_ETA_,iJet,iChannel,iStep,syst_ext.c_str()))->Write();
	fOutput->FindObject(Form("h_%s_%d_Ch%d_S%d%s",RECO_B_DISCRIMINATOR_,iJet,iChannel,iStep,syst_ext.c_str()))->Write();
      }//jet
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s",RECO_ADDJETS_DELTAR_,iChannel,iStep,syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s",RECO_ADDJETS_INVARIANT_MASS_,iChannel,iStep,syst_ext.c_str()))->Write();
    }//step
  }//channel
  out->Write();
  out->Close();
}

double MyAnalysis::transverseMass( const TLorentzVector & lepton, const TLorentzVector & met){

  TLorentzVector leptonT(lepton.Px(),lepton.Py(),0.,lepton.E()*TMath::Sin(lepton.Theta()));
  TLorentzVector metT(met.Px(), met.Py(), 0, met.E());

  return (leptonT+metT).M();
}
