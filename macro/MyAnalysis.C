#define MyAnalysis_cxx
#include <TH2.h>
#include <TStyle.h>

#include "MyAnalysis.h"

using namespace TMath;
using namespace std;

void MyAnalysis::Begin(TTree * /*tree*/){
}

void MyAnalysis::SlaveBegin(TTree * /*tree*/){
  std::cout << "Start SlaveBegin" << std::endl;
  option = GetOption();
  process = option.Data();

/*
 * Systematics 
 * PileUp
 *  __puup, __pudown
 * Muon
 *   __muidup, __muiddown, __muisoup, __muisodown, __mutrgup, __mutrgdown
 * Electron
 *   __elidup, __eliddown, __elrecoup, __elrecodown, __elzvtxup, __elzvtxdown
 *   __eltrgup, __eltrgdown
 * CSV shape
 *   __lfup, __lfdown, __hfup, __hfdown
 *   __hfstat1up, __hfstat1down, __hfstat2up, __hfstat2down
 *   __lfstat1up, __lfstat1down, __lfstat2up, __lfstat2down
 *   __cferr1up, __cferr1down, __cferr2up, __cferr2down
 * Scale Weight(ME)
 *   __scale0, __scale1, __scale2, __scale3, __scale4, __scale5
 * Parton Shower(PS)
 *   __isrup, __isrdown, __fsrup, __fsrdown
 * ME & PS
 *   __hdampup, __hdampdown
 *  Pythia Tune
 *   __tuneup, __tunedown
 */

  if( process.Contains("Data") ){
    v_syst.push_back("");
  }
  else if( process.Contains("__") ){
    //Pythia Tune, ME & PS Matching
    if( process.Contains("tuneup")    ) v_syst.push_back("__tuneup");
    if( process.Contains("tunedown")  ) v_syst.push_back("__tunedown");
    
    if( process.Contains("hdampup")   ) v_syst.push_back("__hdampup");
    if( process.Contains("hdampdown") ) v_syst.push_back("__hdampdown");
    
    if( process.Contains("isrup")   ) v_syst.push_back("__isrup");
    if( process.Contains("isrdown") ) v_syst.push_back("__isrdown");
    if( process.Contains("fsrup")   ) v_syst.push_back("__fsrup");
    if( process.Contains("fsrdown") ) v_syst.push_back("__fsrdown");

    if( process.Contains("jerup") )   v_syst.push_back("__jerup");
    if( process.Contains("jerdown") ) v_syst.push_back("__jerdown");
    if( process.Contains("jecup") )   v_syst.push_back("__jecup");
    if( process.Contains("jecdown") ) v_syst.push_back("__jecdown");
  }
  else{
    v_syst = {"",
      "__puup", "__pudown",
      "__lfup", "__lfdown", "__hfup", "__hfdown",
      "__hfstat1up", "__hfstat1down", "__hfstat2up", "__hfstat2down",
      "__lfstat1up", "__lfstat1down", "__lfstat2up", "__lfstat2down",
      "__cferr1up", "__cferr1down", "__cferr2up", "__cferr2down"
    };
    if( process.Contains("2016") ){
      v_syst.push_back("__musfup");  v_syst.push_back("__musfdown");
      v_syst.push_back("__mutrgup"); v_syst.push_back("__mutrgdown");
      v_syst.push_back("__elsfup");  v_syst.push_back("__elsfdown");
      v_syst.push_back("__eltrgup"); v_syst.push_back("__eltrgdown");
      if( process.Contains("TTLJ") || process.Contains("Bkg") ){
        v_syst.push_back("__scale0"); v_syst.push_back("__scale1"); v_syst.push_back("__scale2");
	v_syst.push_back("__scale3"); v_syst.push_back("__scale4"); v_syst.push_back("__scale5");
	int maxpdf = 103;
	std::string str_tmp;
        for( int i = 0; i < maxpdf; ++i){
          str_tmp = "__pdf" + to_string(i);
	  v_syst.push_back(str_tmp);
	}
      }
    }
    else{
      if( process.Contains("2017") ){
        v_syst.push_back("__prefireup"); v_syst.push_back("__prefiredown");
      }
      v_syst.push_back("__muidup");  v_syst.push_back("__muiddown");
      v_syst.push_back("__muisoup"); v_syst.push_back("__muisodown");
      v_syst.push_back("__mutrgup"); v_syst.push_back("__mutrgdown");
      v_syst.push_back("__elidup");   v_syst.push_back("__eliddown");
      v_syst.push_back("__elrecoup"); v_syst.push_back("__elrecodown");
      v_syst.push_back("__elzvtxup"); v_syst.push_back("__elzvtxdown");
      v_syst.push_back("__eltrgup");  v_syst.push_back("__eltrgdown");
      if( process.Contains("TTLJ") || process.Contains("Bkg") ){
        v_syst.push_back("__scale0"); v_syst.push_back("__scale1"); v_syst.push_back("__scale2");
	v_syst.push_back("__scale3"); v_syst.push_back("__scale4"); v_syst.push_back("__scale5");
	v_syst.push_back("__isrup");  v_syst.push_back("__isrdown");
	v_syst.push_back("__fsrup");  v_syst.push_back("__fsrdown");
	int maxpdf = 104;
	std::string str_tmp;
        for( int i = 0; i < maxpdf; ++i){
          str_tmp = "__pdf" + to_string(i);
	  v_syst.push_back(str_tmp);
	}
      }
    }
  }
  
  if( process.Contains("TTLJ") || process.Contains("Bkg") ){
    pdfweight   = {fReader, "pdfweight"};
    scaleweight = {fReader, "scaleweight"};
    if( !process.Contains("2016") ) psweight = {fReader, "psweight"};
  }
  if( process.Contains("2016") ){
    lepton_pT     = {fReader, "lepton_pT"};
    lepton_E      = {fReader, "lepton_E"};
    jet_pT        = {fReader, "jet_pT"};
    jet_E         = {fReader, "jet_E"};
    jet_CSV       = {fReader, "jet_CSV"};
    jet_SF_CSV_30 = {fReader, "jet_SF_CSV_30"};

    if( process.Contains("ttbb") ){
      addbjet1_pt    = {fReader, "addbjet1_pt"};
      addbjet1_eta   = {fReader, "addbjet1_eta"};
      addbjet1_phi   = {fReader, "addbjet1_phi"};
      addbjet1_e     = {fReader, "addbjet1_e"};
      addbjet2_pt    = {fReader, "addbjet2_pt"};
      addbjet2_eta   = {fReader, "addbjet2_eta"};
      addbjet2_phi   = {fReader, "addbjet2_phi"};
      addbjet2_e     = {fReader, "addbjet2_e"};

      mindRbjet1_pt  = {fReader, "mindRbjet1_pt"};
      mindRbjet1_eta = {fReader, "mindRbjet1_eta"};
      mindRbjet1_phi = {fReader, "mindRbjet1_phi"};
      mindRbjet1_e   = {fReader, "mindRbjet1_e"};
      mindRbjet2_pt  = {fReader, "mindRbjet2_pt"};
      mindRbjet2_eta = {fReader, "mindRbjet2_eta"};
      mindRbjet2_phi = {fReader, "mindRbjet2_phi"};
      mindRbjet2_e   = {fReader, "mindRbjet2_e"};
    }
  }
  else{
    lepton_pT     = {fReader, "lepton_pt"};
    lepton_E      = {fReader, "lepton_e"};
    jet_pT        = {fReader, "jet_pt"};
    jet_E         = {fReader, "jet_e"};
    jet_CSV       = {fReader, "jet_deepCSV"};
    jet_SF_CSV_30 = {fReader, "jet_SF_deepCSV_30"};
    
    if( process.Contains("ttbb") ){
      addbjet1_pt    = {fReader, "addbjet1_pt"};
      addbjet1_eta   = {fReader, "addbjet1_eta"};
      addbjet1_phi   = {fReader, "addbjet1_phi"};
      addbjet1_e     = {fReader, "addbjet1_e"};
      addbjet2_pt    = {fReader, "addbjet2_pt"};
      addbjet2_eta   = {fReader, "addbjet2_eta"};
      addbjet2_phi   = {fReader, "addbjet2_phi"};
      addbjet2_e     = {fReader, "addbjet2_e"};

      mindRbjet1_pt  = {fReader, "mindRjet1_pt"};
      mindRbjet1_eta = {fReader, "mindRjet1_eta"};
      mindRbjet1_phi = {fReader, "mindRjet1_phi"};
      mindRbjet1_e   = {fReader, "mindRjet1_e"};
      mindRbjet2_pt  = {fReader, "mindRjet2_pt"};
      mindRbjet2_eta = {fReader, "mindRjet2_eta"};
      mindRbjet2_phi = {fReader, "mindRjet2_phi"};
      mindRbjet2_e   = {fReader, "mindRjet2_e"};
    }
    
    if( process.Contains("2017") ){
      MUON_ETA_      = 2.4;
      ELECTRON_PT_   = 30.0;
      ELECTRON_ETA_  = 2.4;
      JET_CSV_TIGHT_ = 0.8001;
      prefireweight = {fReader, "prefireweight"};
    }
    if( process.Contains("2018") ){
      MUON_ETA_      = 2.4;
      ELECTRON_PT_   = 30.0;
      ELECTRON_ETA_  = 2.4;
      JET_CSV_TIGHT_ = 0.7527;
    }
  }

  std::cout << "Make HistoBook" << std::endl;
  for(int i = 0; i < v_syst.size(); i++){
    HistoBook *h_tmp1 = new HistoBook(1, v_syst[i].c_str());
    HistoBook *h_tmp2 = new HistoBook(2, v_syst[i].c_str());
    h_control.push_back(h_tmp1);
    h_matrix.push_back(h_tmp2);
  }
   
  std::cout << "Get Hist list" << std::endl;
  for(int i = 0; i < v_syst.size(); i++){
    for(int iChannel=0; iChannel<nChannel; ++iChannel){
      for(int iStep=0; iStep<nStep; ++iStep){
	  fOutput->Add(h_control[i]->h_lepton_pt[iChannel][iStep]);
	  fOutput->Add(h_control[i]->h_lepton_eta[iChannel][iStep]);
	  fOutput->Add(h_control[i]->h_njets[iChannel][iStep]);
	  fOutput->Add(h_control[i]->h_nbjets[iChannel][iStep]);
	  fOutput->Add(h_control[i]->h_trans_mass[iChannel][iStep]);
	  fOutput->Add(h_control[i]->h_jet_pt_sum[iChannel][iStep]);
	  for(int iJet=0; iJet<nJet; ++iJet){
	    fOutput->Add(h_control[i]->h_jet_pt[iChannel][iStep][iJet]);
	    fOutput->Add(h_control[i]->h_jet_eta[iChannel][iStep][iJet]);
	    fOutput->Add(h_control[i]->h_csv[iChannel][iStep][iJet]);
	  }
	  fOutput->Add(h_control[i]->h_reco_addbjets_deltaR[iChannel][iStep]);
	  fOutput->Add(h_control[i]->h_reco_addbjets_invMass[iChannel][iStep]);
	  fOutput->Add(h_control[i]->h_reco_addbjets_deltaEta[iChannel][iStep]);
	  fOutput->Add(h_control[i]->h_reco_addbjets_deltaPhi[iChannel][iStep]);
	  
	  fOutput->Add(h_matrix[i]->h_gen_gentop_deltaR[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_gen_gentop_invMass[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_gen_gentop_deltaEta[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_gen_gentop_deltaPhi[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_gen_mindR_deltaR[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_gen_mindR_invMass[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_gen_mindR_deltaEta[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_gen_mindR_deltaPhi[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_respMatrix_gentop_deltaR[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_respMatrix_gentop_invMass[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_respMatrix_gentop_deltaEta[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_respMatrix_gentop_deltaPhi[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_respMatrix_mindR_deltaR[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_respMatrix_mindR_invMass[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_respMatrix_mindR_deltaEta[iChannel][iStep]);
	  fOutput->Add(h_matrix[i]->h_respMatrix_mindR_deltaPhi[iChannel][iStep]);
      }//step
    }//channel
  }//syst
  std::cout << "Process: " << process << std::endl;
  std::cout << "Number of Systematics: " << v_syst.size() << std::endl;
  std::cout << "End SlaveBegin" << std::endl;
}

Bool_t MyAnalysis::Process(Long64_t entry){
  fReader.SetEntry(entry);
  option = GetOption();
  process = option.Data();
  const int mode = *channel;
  if(mode>2) return kTRUE;
 
  //const double relIso = *lepton_relIso;
  
  //Object selection
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

    std::string syst_ext = v_syst[0];
    
    int njets = 0;
    int nbjets = 0;
    double jet_pt_sum = 0.0;
    multimap<float /*jet_Pt*/, TLorentzVector /*jet_4-momentum*/, greater<float>> m_jets;
    vector<TLorentzVector /*jet_4-momentum*/> v_reco_bjets;
    for (unsigned int iJet = 0; iJet < jet_pT.GetSize() ; ++iJet) {
      float jetSystVar = 1.0;
      if( !process.Contains("Data") ){
        // Jet Energy Correction, Jet Energy Resolution
	if     ( syst_ext == "__jecup"   ) jetSystVar = jet_JER_Nom[iJet] * jet_JES_Up[iJet];
	else if( syst_ext == "__jecdown" ) jetSystVar = jet_JER_Nom[iJet] * jet_JES_Down[iJet];
	else if( syst_ext == "__jerup"   ) jetSystVar = jet_JER_Up[iJet];
	else if( syst_ext == "__jerdown" ) jetSystVar = jet_JER_Down[iJet];
	else                               jetSystVar = jet_JER_Nom[iJet];
      }

      TLorentzVector jet;
      jet.SetPtEtaPhiE(jet_pT[iJet], jet_eta[iJet], jet_phi[iJet], jet_E[iJet]);
      jet *= jetSystVar;

      if ( jet.Pt() <= JET_PT_ || abs(jet.Eta()) >= JET_ETA_ ) continue;

      //m_jets.insert(pair<float,TLorentzVector>(jet_CSV[iJet],jet));
      m_jets.insert(pair<float,TLorentzVector>(jet.Pt(),jet));
      jet_pt_sum += jet.Pt();
      ++njets;
      if( jet_CSV[iJet] > JET_CSV_TIGHT_ ){
	++nbjets;
	v_reco_bjets.push_back(jet);
      }
    }

    double a_jetCSV[6] = {-999,-999,-999,-999,-999,-999};
    double a_jetPt[6] = {-999,-999,-999,-999,-999,-999};
    double a_jetEta[6] = {-999,-999,-999,-999,-999,-999};
    int tmp_idx = 0;
    for(auto m_itr = m_jets.begin(); m_itr != m_jets.end(); ++m_itr){
      if(tmp_idx >= 6) continue;
      a_jetPt[tmp_idx] = m_itr->first;
      //a_jetCSV[tmp_idx] = (m_itr->second).first;
      a_jetEta[tmp_idx] = (m_itr->second).Eta();
      ++tmp_idx;
    }

    TLorentzVector reco_addbjet1, reco_addbjet2;
    double reco_addbjet_deltaR   = -999.0;
    double reco_addbjet_invMass  = -999.0;
    double reco_addbjet_deltaEta = -999.0;
    double reco_addbjet_deltaPhi = -999.0;
    //double reco_addJet_CSV[2] = {-999.0, -999.0};
    if (v_reco_bjets.size() >= 2){
      for(unsigned int i=0; i < v_reco_bjets.size()-1; ++i){
	for(unsigned int j=i+1; j < v_reco_bjets.size(); ++j){
	  double tmp_dR = v_reco_bjets[i].DeltaR(v_reco_bjets[j]);
	
	  if(tmp_dR < abs(reco_addbjet_deltaR)){
	    reco_addbjet1 = v_reco_bjets[i];
	    reco_addbjet2 = v_reco_bjets[j];

	    reco_addbjet_deltaR   = tmp_dR;
	    reco_addbjet_invMass  = (reco_addbjet1 + reco_addbjet2).M();
	    reco_addbjet_deltaEta = abs(reco_addbjet1.Eta() - reco_addbjet2.Eta());
	    reco_addbjet_deltaPhi = abs(reco_addbjet1.DeltaPhi(reco_addbjet2));
	  }
	}
      }
    } 

    TLorentzVector gen_addbjet1, gen_addbjet2;
    TLorentzVector gen_mindRbjet1, gen_mindRbjet2;
    if( process.Contains("ttbb") ){
      gen_addbjet1.SetPtEtaPhiE(*addbjet1_pt, *addbjet1_eta, *addbjet1_phi, *addbjet1_e);
      gen_addbjet2.SetPtEtaPhiE(*addbjet2_pt, *addbjet2_eta, *addbjet2_phi, *addbjet2_e);

      gen_mindRbjet1.SetPtEtaPhiE(*mindRbjet1_pt, *mindRbjet1_eta, *mindRbjet1_phi, *mindRbjet1_e);
      gen_mindRbjet2.SetPtEtaPhiE(*mindRbjet2_pt, *mindRbjet2_eta, *mindRbjet2_phi, *mindRbjet2_e);
    }
    double gen_addbjet_deltaR    = gen_addbjet1.DeltaR(gen_addbjet2);
    double gen_addbjet_invMass   = (gen_addbjet1+gen_addbjet2).M(); 
    double gen_addbjet_deltaEta  = abs(gen_addbjet1.Eta() - gen_addbjet2.Eta());
    double gen_addbjet_deltaPhi  = abs(gen_addbjet1.DeltaPhi(gen_addbjet2));
    
    double gen_mindR_deltaR   = gen_mindRbjet1.DeltaR(gen_mindRbjet2); 
    double gen_mindR_invMass  = (gen_mindRbjet1+gen_mindRbjet2).M(); 
    double gen_mindR_deltaEta = abs(gen_mindRbjet1.Eta() - gen_mindRbjet2.Eta()); 
    double gen_mindR_deltaPhi = abs(gen_mindRbjet1.DeltaPhi(gen_mindRbjet2)); 

    int passchannel = -999;
    bool passlepton = false;
    if     ( passmuon and !passelectron) passchannel = MUON_;
    else if(!passmuon and  passelectron) passchannel = ELECTRON_;

    if( passchannel != -999 ) passlepton = true;

    int passcut = 0;
    if(njets >= NUMBER_OF_JETS_){
      ++passcut;
      if(nbjets >= NUMBER_OF_BJETS_){
	++passcut;
	if(nbjets >= NUMBER_OF_BJETS_+1) ++passcut;
      }
    }

  for(int iSys = 0; iSys < v_syst.size(); ++iSys){
    syst_ext = v_syst[iSys];
    //Event Weight
    double EventWeight = 1;
    if( !option.Contains("Data") ){
      EventWeight *= *genWeight;
      
      //Pile Up weight
      if     ( syst_ext == "__puup"   ) EventWeight *= PUWeight[1];
      else if( syst_ext == "__pudown" ) EventWeight *= PUWeight[2];
      else                              EventWeight *= PUWeight[0];
   
      //Prefire weight, 2017only
      if( process.Contains("2017") ){
	if     ( syst_ext == "__prefireup"   ) EventWeight *= prefireweight[1];
	else if( syst_ext == "__prefiredown" ) EventWeight *= prefireweight[2];
	else                                   EventWeight *= prefireweight[0];
      }
      
      //Lepton Scale Factor
      if( process.Contains("2016") ){
	if( passchannel == 0 ){
	  //mu [0]~[2]:ID/Iso, [3]~[5]:Trigger
	  if     ( syst_ext == "__musfup"    ) EventWeight *= lepton_SF[1];
	  else if( syst_ext == "__musfdown"  ) EventWeight *= lepton_SF[2];
	  else                                 EventWeight *= lepton_SF[0];
	  
	  if     ( syst_ext == "__mutrgup"   ) EventWeight *= lepton_SF[4];
	  else if( syst_ext == "__mutrgdown" ) EventWeight *= lepton_SF[5];
	  else                                 EventWeight *= lepton_SF[3];
	}
	else if( passchannel == 1 ){
	  //el [0]~[2]: ID/Iso/Reco, [3]~[5]: Trigger
	  if     ( syst_ext == "__elsfup"    ) EventWeight *= lepton_SF[1];
	  else if( syst_ext == "__elsfdown"  ) EventWeight *= lepton_SF[2];
	  else                                 EventWeight *= lepton_SF[0];

	  if     ( syst_ext == "__eltrgup"   ) EventWeight *= lepton_SF[4];
	  else if( syst_ext == "__eltrgdown" ) EventWeight *= lepton_SF[5];
	  else                                 EventWeight *= lepton_SF[3];
	}
      }//Run2016
      else{
	if( passchannel == 0 ){
	  //Muon
	  //[0]~[2]:ID, [3]~[5]:Iso, [6]~[8]:Trigger
	  if     ( syst_ext == "__muidup"    ) EventWeight *= lepton_SF[1];
	  else if( syst_ext == "__muiddown"  ) EventWeight *= lepton_SF[2];
	  else                                 EventWeight *= lepton_SF[0];

	  if     ( syst_ext == "__muisoup"   ) EventWeight *= lepton_SF[4];
	  else if( syst_ext == "__muisodown" ) EventWeight *= lepton_SF[5];
	  else        	                       EventWeight *= lepton_SF[3];

	  if     ( syst_ext == "__mutrgup"   ) EventWeight *= lepton_SF[7];
	  else if( syst_ext == "__mutrgdown" ) EventWeight *= lepton_SF[8];
	  else                                 EventWeight *= lepton_SF[6];
	}
	else if( passchannel == 1 ){
	  //Electron
	  //[0]~[2]: ID, [3]~[5]:Reco, [6]~[8]:Zvtx, [9]~[11]: Trigger
	  if     ( syst_ext == "__elsfup"     ) EventWeight *= lepton_SF[1];
	  else if( syst_ext == "__elsfdown"   ) EventWeight *= lepton_SF[2];
	  else                                  EventWeight *= lepton_SF[0];

	  if     ( syst_ext == "__elrecoup"   ) EventWeight *= lepton_SF[4];
	  else if( syst_ext == "__elrecodown" ) EventWeight *= lepton_SF[5];
	  else	                                EventWeight *= lepton_SF[3];

	  if     ( syst_ext == "__elzvtxup"   ) EventWeight *= lepton_SF[7];
	  else if( syst_ext == "__elzvtxdown" )	EventWeight *= lepton_SF[8];
	  else	                                EventWeight *= lepton_SF[6];

	  if     ( syst_ext == "__eltrgup"   )	EventWeight *= lepton_SF[10];
	  else if( syst_ext == "__eltrgdown" )  EventWeight *= lepton_SF[11];
	  else	                                EventWeight *= lepton_SF[9];
	}
      }//Run 2017-2018

      //Scale Weight(ME)
      //[0] = muF up , [1] = muF down, [2] = muR up, [3] = muR up && muF up
      //[4] = muR down, [5] = muF down && muF down
      if     ( syst_ext == "__scale0" ) EventWeight *= scaleweight[0];
      else if( syst_ext == "__scale1" ) EventWeight *= scaleweight[1];
      else if( syst_ext == "__scale2" )	EventWeight *= scaleweight[2];
      else if( syst_ext == "__scale3" )	EventWeight *= scaleweight[3];
      else if( syst_ext == "__scale4" )	EventWeight *= scaleweight[4];
      else if( syst_ext == "__scale5" )	EventWeight *= scaleweight[5];
      else                              EventWeight *= 1.0;
      
      //CSV Shape
      // Systematics for bottom flavor jets:
      //   Light flavor contamination: lf
      //   Linear and quadratic statistical fluctuations: hfstats1 and hfstats2
      // Systematics for light flavor jets:
      //   Heavy flavor contamination: hf
      //   Linear and quadratic statistical fluctuations: lfstats1 and lfstats2
      // Systematics for charm flavor jets:
      //   Linear and quadratic uncertainties: cferr1 and cferr2
      if     ( syst_ext == "__lfup"        ) EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[3];
      else if( syst_ext == "__lfdown"      ) EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[4];
      else if( syst_ext == "__hfup"        ) EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[5];
      else if( syst_ext == "__hfdown"      ) EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[6];
      else if( syst_ext == "__hfstat1up"   ) EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[7];
      else if( syst_ext == "__hfstat1down" ) EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[8];
      else if( syst_ext == "__hfstat2up"   ) EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[9];
      else if( syst_ext == "__hfstat2down" ) EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[10];
      else if( syst_ext == "__lfstat1up"   ) EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[11];
      else if( syst_ext == "__lfstat1down" ) EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[12];
      else if( syst_ext == "__lfstat2up"   ) EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[13];
      else if( syst_ext == "__lfstat2down" ) EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[14];
      else if( syst_ext == "__cferr1up"    ) EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[15];
      else if( syst_ext == "__cferr1down"  ) EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[16];
      else if( syst_ext == "__cferr2up"    ) EventWeight *= jet_SF_CSV_30[0] + jet_SF_CSV_30[17];
      else if( syst_ext == "__cferr2down"  ) EventWeight *= jet_SF_CSV_30[0] - jet_SF_CSV_30[18];
      else	                             EventWeight *= jet_SF_CSV_30[0];

      //Parton Shower
      if( !process.Contains("2016") ){
        if      ( syst_ext == "__isrup"   ) EventWeight *= psweight[0];
        else if ( syst_ext == "__isrdown" ) EventWeight *= psweight[2];
        else if ( syst_ext == "__fsrup"   ) EventWeight *= psweight[1];
        else if ( syst_ext == "__fsrdown" ) EventWeight *= psweight[3];
        else                                EventWeight *= 1.0;
      }

      //PDF Uncertainties
      ssize_t pos;
      if( (pos = syst_ext.find("pdf",0)) != std::string::npos ){
	int maxpdf = 103;
	if( option.Contains("2016") ) maxpdf = 104;
	for( int i = 0; i < maxpdf; ++i){
	  std::string str_tmp = "pdf" + to_string(i);
	  if( (pos = syst_ext.find(str_tmp)) != std::string::npos ){
	    EventWeight *= pdfweight[i];
	    break;
	  }
	}
      }
    }
   
    for(int iCut=0; iCut <= passcut; ++iCut){
      h_control[iSys]->h_lepton_pt[passchannel][iCut] ->Fill(lepton.Pt(),                  EventWeight);
      h_control[iSys]->h_lepton_eta[passchannel][iCut]->Fill(abs(lepton.Eta()),            EventWeight);
      h_control[iSys]->h_njets[passchannel][iCut]     ->Fill(njets,                        EventWeight);
      h_control[iSys]->h_nbjets[passchannel][iCut]    ->Fill(nbjets,                       EventWeight);
      h_control[iSys]->h_trans_mass[passchannel][iCut]->Fill(transverseMass(lepton,p4met), EventWeight);
      h_control[iSys]->h_jet_pt_sum[passchannel][iCut]->Fill(jet_pt_sum,                   EventWeight);
      for(int iJet=0; iJet<nJet; ++iJet){
	h_control[iSys]->h_jet_pt[passchannel][iCut][iJet] ->Fill(a_jetPt[iJet],  EventWeight);
	h_control[iSys]->h_jet_eta[passchannel][iCut][iJet]->Fill(a_jetEta[iJet], EventWeight);
	h_control[iSys]->h_csv[passchannel][iCut][iJet]    ->Fill(a_jetCSV[iJet], EventWeight);
      }
      if( process.Contains("2016") || !process.Contains("Test") ){
	h_control[iSys]->h_reco_addbjets_deltaR[passchannel][iCut]  ->Fill(reco_addbjet_deltaR,   EventWeight);
	h_control[iSys]->h_reco_addbjets_invMass[passchannel][iCut] ->Fill(reco_addbjet_invMass,  EventWeight);
	h_control[iSys]->h_reco_addbjets_deltaEta[passchannel][iCut]->Fill(reco_addbjet_deltaEta, EventWeight);
	h_control[iSys]->h_reco_addbjets_deltaPhi[passchannel][iCut]->Fill(reco_addbjet_deltaPhi, EventWeight);

	h_matrix[iSys]->h_respMatrix_gentop_deltaR[passchannel][iCut]  ->Fill(reco_addbjet_deltaR,   gen_addbjet_deltaR,   EventWeight);
	h_matrix[iSys]->h_respMatrix_gentop_invMass[passchannel][iCut] ->Fill(reco_addbjet_invMass,  gen_addbjet_invMass,  EventWeight);
	h_matrix[iSys]->h_respMatrix_gentop_deltaEta[passchannel][iCut]->Fill(reco_addbjet_deltaEta, gen_addbjet_deltaEta, EventWeight);
	h_matrix[iSys]->h_respMatrix_gentop_deltaPhi[passchannel][iCut]->Fill(reco_addbjet_deltaPhi, gen_addbjet_deltaPhi, EventWeight);
	h_matrix[iSys]->h_respMatrix_mindR_deltaR[passchannel][iCut]   ->Fill(reco_addbjet_deltaR,   gen_mindR_deltaR,     EventWeight);
	h_matrix[iSys]->h_respMatrix_mindR_invMass[passchannel][iCut]  ->Fill(reco_addbjet_invMass,  gen_mindR_invMass,    EventWeight);
	h_matrix[iSys]->h_respMatrix_mindR_deltaEta[passchannel][iCut] ->Fill(reco_addbjet_deltaEta, gen_mindR_deltaEta,   EventWeight);
	h_matrix[iSys]->h_respMatrix_mindR_deltaPhi[passchannel][iCut] ->Fill(reco_addbjet_deltaPhi, gen_mindR_deltaPhi,   EventWeight);
      }
      else{
	if( nevt % 2 == 0 ){
	  h_control[iSys]->h_reco_addbjets_deltaR[passchannel][iCut]  ->Fill(reco_addbjet_deltaR,   EventWeight);
	  h_control[iSys]->h_reco_addbjets_invMass[passchannel][iCut] ->Fill(reco_addbjet_invMass,  EventWeight);
	  h_control[iSys]->h_reco_addbjets_deltaEta[passchannel][iCut]->Fill(reco_addbjet_deltaEta, EventWeight);
	  h_control[iSys]->h_reco_addbjets_deltaPhi[passchannel][iCut]->Fill(reco_addbjet_deltaPhi, EventWeight);
	}
	else{
	  h_matrix[iSys]->h_respMatrix_gentop_deltaR[passchannel][iCut]  ->Fill(reco_addbjet_deltaR,   gen_addbjet_deltaR,   EventWeight);
	  h_matrix[iSys]->h_respMatrix_gentop_invMass[passchannel][iCut] ->Fill(reco_addbjet_invMass,  gen_addbjet_invMass,  EventWeight);
	  h_matrix[iSys]->h_respMatrix_gentop_deltaEta[passchannel][iCut]->Fill(reco_addbjet_deltaEta, gen_addbjet_deltaEta, EventWeight);
	  h_matrix[iSys]->h_respMatrix_gentop_deltaPhi[passchannel][iCut]->Fill(reco_addbjet_deltaPhi, gen_addbjet_deltaPhi, EventWeight);
	  h_matrix[iSys]->h_respMatrix_mindR_deltaR[passchannel][iCut]   ->Fill(reco_addbjet_deltaR,   gen_mindR_deltaR,     EventWeight);
	  h_matrix[iSys]->h_respMatrix_mindR_invMass[passchannel][iCut]  ->Fill(reco_addbjet_invMass,  gen_mindR_invMass,    EventWeight);
	  h_matrix[iSys]->h_respMatrix_mindR_deltaEta[passchannel][iCut] ->Fill(reco_addbjet_deltaEta, gen_mindR_deltaEta,   EventWeight);
	  h_matrix[iSys]->h_respMatrix_mindR_deltaPhi[passchannel][iCut] ->Fill(reco_addbjet_deltaPhi, gen_mindR_deltaPhi,   EventWeight);
	}
      }
      h_matrix[iSys]->h_gen_gentop_deltaR[passchannel][iCut]  ->Fill(gen_addbjet_deltaR,    EventWeight);
      h_matrix[iSys]->h_gen_gentop_invMass[passchannel][iCut] ->Fill(gen_addbjet_invMass,   EventWeight);
      h_matrix[iSys]->h_gen_gentop_deltaEta[passchannel][iCut]->Fill(gen_addbjet_deltaEta,  EventWeight);
      h_matrix[iSys]->h_gen_gentop_deltaPhi[passchannel][iCut]->Fill(gen_addbjet_deltaPhi,  EventWeight);
      h_matrix[iSys]->h_gen_mindR_deltaR[passchannel][iCut]   ->Fill(gen_mindR_deltaR,      EventWeight);
      h_matrix[iSys]->h_gen_mindR_invMass[passchannel][iCut]  ->Fill(gen_mindR_invMass,     EventWeight);
      h_matrix[iSys]->h_gen_mindR_deltaEta[passchannel][iCut] ->Fill(gen_mindR_deltaEta,    EventWeight);
      h_matrix[iSys]->h_gen_mindR_deltaPhi[passchannel][iCut] ->Fill(gen_mindR_deltaPhi,    EventWeight);
    }//cut

    if(passlepton){
      for(int iCut=0; iCut <= passcut; ++iCut){
	h_control[iSys]->h_lepton_pt[2][iCut] ->Fill(lepton.Pt(),                  EventWeight);
	h_control[iSys]->h_lepton_eta[2][iCut]->Fill(abs(lepton.Eta()),            EventWeight);
	h_control[iSys]->h_njets[2][iCut]     ->Fill(njets,                        EventWeight);
	h_control[iSys]->h_nbjets[2][iCut]    ->Fill(nbjets,                       EventWeight);
	h_control[iSys]->h_trans_mass[2][iCut]->Fill(transverseMass(lepton,p4met), EventWeight);
	h_control[iSys]->h_jet_pt_sum[2][iCut]->Fill(jet_pt_sum,                   EventWeight);
	for(int iJet=0; iJet<nJet; ++iJet){
	  h_control[iSys]->h_jet_pt[2][iCut][iJet] ->Fill(a_jetPt[iJet],  EventWeight);
	  h_control[iSys]->h_jet_eta[2][iCut][iJet]->Fill(a_jetEta[iJet], EventWeight);
	  h_control[iSys]->h_csv[2][iCut][iJet]    ->Fill(a_jetCSV[iJet], EventWeight);
	}
	
	if( process.Contains("2016") || !process.Contains("Test") ){
	  h_control[iSys]->h_reco_addbjets_deltaR[2][iCut]  ->Fill(reco_addbjet_deltaR,   EventWeight);
	  h_control[iSys]->h_reco_addbjets_invMass[2][iCut] ->Fill(reco_addbjet_invMass,  EventWeight);
	  h_control[iSys]->h_reco_addbjets_deltaEta[2][iCut]->Fill(reco_addbjet_deltaEta, EventWeight);
	  h_control[iSys]->h_reco_addbjets_deltaPhi[2][iCut]->Fill(reco_addbjet_deltaPhi, EventWeight);
	  h_matrix[iSys]->h_respMatrix_gentop_deltaR[2][iCut]  ->Fill(reco_addbjet_deltaR,   gen_addbjet_deltaR,   EventWeight);
	  h_matrix[iSys]->h_respMatrix_gentop_invMass[2][iCut] ->Fill(reco_addbjet_invMass,  gen_addbjet_invMass,  EventWeight);
	  h_matrix[iSys]->h_respMatrix_gentop_deltaEta[2][iCut]->Fill(reco_addbjet_deltaEta, gen_addbjet_deltaEta, EventWeight);
	  h_matrix[iSys]->h_respMatrix_gentop_deltaPhi[2][iCut]->Fill(reco_addbjet_deltaPhi, gen_addbjet_deltaPhi, EventWeight);
	  h_matrix[iSys]->h_respMatrix_mindR_deltaR[2][iCut]   ->Fill(reco_addbjet_deltaR,   gen_mindR_deltaR,     EventWeight);
	  h_matrix[iSys]->h_respMatrix_mindR_invMass[2][iCut]  ->Fill(reco_addbjet_invMass,  gen_mindR_invMass,    EventWeight);
	  h_matrix[iSys]->h_respMatrix_mindR_deltaEta[2][iCut] ->Fill(reco_addbjet_deltaEta, gen_mindR_deltaEta,   EventWeight);
	  h_matrix[iSys]->h_respMatrix_mindR_deltaPhi[2][iCut] ->Fill(reco_addbjet_deltaPhi, gen_mindR_deltaPhi,   EventWeight);
	}
	else{
	  if( nevt % 2 == 0 ){
	    h_control[iSys]->h_reco_addbjets_deltaR[2][iCut]  ->Fill(reco_addbjet_deltaR,   EventWeight);
	    h_control[iSys]->h_reco_addbjets_invMass[2][iCut] ->Fill(reco_addbjet_invMass,  EventWeight);
	    h_control[iSys]->h_reco_addbjets_deltaEta[2][iCut]->Fill(reco_addbjet_deltaEta, EventWeight);
	    h_control[iSys]->h_reco_addbjets_deltaPhi[2][iCut]->Fill(reco_addbjet_deltaPhi, EventWeight);
	  }
	  else{
	    h_matrix[iSys]->h_respMatrix_gentop_deltaR[2][iCut]  ->Fill(reco_addbjet_deltaR,   gen_addbjet_deltaR,   EventWeight);
	    h_matrix[iSys]->h_respMatrix_gentop_invMass[2][iCut] ->Fill(reco_addbjet_invMass,  gen_addbjet_invMass,  EventWeight);
	    h_matrix[iSys]->h_respMatrix_gentop_deltaEta[2][iCut]->Fill(reco_addbjet_deltaEta, gen_addbjet_deltaEta, EventWeight);
	    h_matrix[iSys]->h_respMatrix_gentop_deltaPhi[2][iCut]->Fill(reco_addbjet_deltaPhi, gen_addbjet_deltaPhi, EventWeight);
	    h_matrix[iSys]->h_respMatrix_mindR_deltaR[2][iCut]   ->Fill(reco_addbjet_deltaR,   gen_mindR_deltaR,     EventWeight);
	    h_matrix[iSys]->h_respMatrix_mindR_invMass[2][iCut]  ->Fill(reco_addbjet_invMass,  gen_mindR_invMass,    EventWeight);
	    h_matrix[iSys]->h_respMatrix_mindR_deltaEta[2][iCut] ->Fill(reco_addbjet_deltaEta, gen_mindR_deltaEta,   EventWeight);
	    h_matrix[iSys]->h_respMatrix_mindR_deltaPhi[2][iCut] ->Fill(reco_addbjet_deltaPhi, gen_mindR_deltaPhi,   EventWeight);
	  }
	}	
	h_matrix[iSys]->h_gen_gentop_deltaR[2][iCut]  ->Fill(gen_addbjet_deltaR,    EventWeight);
	h_matrix[iSys]->h_gen_gentop_invMass[2][iCut] ->Fill(gen_addbjet_invMass,   EventWeight);
	h_matrix[iSys]->h_gen_gentop_deltaEta[2][iCut]->Fill(gen_addbjet_deltaEta,  EventWeight);
	h_matrix[iSys]->h_gen_gentop_deltaPhi[2][iCut]->Fill(gen_addbjet_deltaPhi,  EventWeight);
	h_matrix[iSys]->h_gen_mindR_deltaR[2][iCut]   ->Fill(gen_mindR_deltaR,      EventWeight);
	h_matrix[iSys]->h_gen_mindR_invMass[2][iCut]  ->Fill(gen_mindR_invMass,     EventWeight);
	h_matrix[iSys]->h_gen_mindR_deltaEta[2][iCut] ->Fill(gen_mindR_deltaEta,    EventWeight);
	h_matrix[iSys]->h_gen_mindR_deltaPhi[2][iCut] ->Fill(gen_mindR_deltaPhi,    EventWeight);
      }//cut
    }//passlepton
  }//iSys
  ++nevt;
  return kTRUE;
}

void MyAnalysis::SlaveTerminate(){
  std::cout << "SlaveTerminate" << std::endl;
  option = GetOption();
}

void MyAnalysis::Terminate(){
  std::cout << "Terminate" << std::endl;
  option = GetOption();
  string str_opt = option.Data();

  string outname = str_opt.substr(str_opt.find_last_of("/")+1,string::npos);

  TFile *out = TFile::Open(Form("output/root/hist_%s.root", outname.c_str()), "RECREATE");
  
  TIter next(GetOutputList());
  TObject *obj = 0;
  while( (obj = next()) ){
    const char *name = obj->GetName();
    std::string str(name);
    if(str.find("h_") != std::string::npos ) obj->Write();
  }
  out->Write();
  out->Close();
}

double MyAnalysis::transverseMass(const TLorentzVector & lepton, const TLorentzVector & met){

  TLorentzVector leptonT(lepton.Px(),lepton.Py(),0.,lepton.E()*TMath::Sin(lepton.Theta()));
  TLorentzVector metT(met.Px(), met.Py(), 0, met.E());

  return (leptonT+metT).M();
}
