#define MyAnalysis_cxx
#include <TH2.h>
#include <TStyle.h>

#include "MyAnalysis.h"

using namespace TMath;
using namespace std;

void MyAnalysis::Begin(TTree * /*tree*/){
}

void MyAnalysis::SlaveBegin(TTree * /*tree*/){
  std::cout << "SlaveBegin" << std::endl;
  option = GetOption();
  std::string str_opt = option.Data();
  if( option.Contains("__") )
    syst_ext = str_opt.substr(str_opt.find_last_of("_")-1,string::npos);
  else
    syst_ext = "";

  std::cout << "Make HistoBook" << std::endl;
  h_control = new HistoBook(1, syst_ext.c_str());
  h_matrix  = new HistoBook(2, syst_ext.c_str());
  
  std::cout << "Get Hist list" << std::endl;
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
	  fOutput->Add(h_control->h_csv[iChannel][iStep][iJet]);
	}
	
	fOutput->Add(h_control->h_reco_addbjets_deltaR[iChannel][iStep]);
	fOutput->Add(h_control->h_reco_addbjets_invMass[iChannel][iStep]);
	fOutput->Add(h_control->h_reco_addbjets_deltaEta[iChannel][iStep]);
	fOutput->Add(h_control->h_reco_addbjets_deltaPhi[iChannel][iStep]);
	
	fOutput->Add(h_matrix->h_gen_gentop_deltaR[iChannel][iStep]);
	fOutput->Add(h_matrix->h_gen_gentop_invMass[iChannel][iStep]);
	fOutput->Add(h_matrix->h_gen_gentop_deltaEta[iChannel][iStep]);
	fOutput->Add(h_matrix->h_gen_gentop_deltaPhi[iChannel][iStep]);
	fOutput->Add(h_matrix->h_gen_mindR_deltaR[iChannel][iStep]);
	fOutput->Add(h_matrix->h_gen_mindR_invMass[iChannel][iStep]);
	fOutput->Add(h_matrix->h_gen_mindR_deltaEta[iChannel][iStep]);
	fOutput->Add(h_matrix->h_gen_mindR_deltaPhi[iChannel][iStep]);
	
	fOutput->Add(h_matrix->h_respMatrix_gentop_deltaR[iChannel][iStep]);
	fOutput->Add(h_matrix->h_respMatrix_gentop_invMass[iChannel][iStep]);
	fOutput->Add(h_matrix->h_respMatrix_gentop_deltaEta[iChannel][iStep]);
	fOutput->Add(h_matrix->h_respMatrix_gentop_deltaPhi[iChannel][iStep]);
	fOutput->Add(h_matrix->h_respMatrix_mindR_deltaR[iChannel][iStep]);
	fOutput->Add(h_matrix->h_respMatrix_mindR_invMass[iChannel][iStep]);
	fOutput->Add(h_matrix->h_respMatrix_mindR_deltaEta[iChannel][iStep]);
	fOutput->Add(h_matrix->h_respMatrix_mindR_deltaPhi[iChannel][iStep]);
    }//step
  }//channel

  process = option.Data();

  if( process.Contains("tt") || process.Contains("ttH")
      || process.Contains("ttW") || process.Contains("ttZ")){
    pdfweight = {fReader, "pdfweight"};
    scaleweight = {fReader, "scaleweight"};
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
}

Bool_t MyAnalysis::Process(Long64_t entry){
  fReader.SetEntry(entry);
  option = GetOption();
  process = option.Data();
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
  multimap<float /*jet_Pt*/, TLorentzVector /*jet_4-momentum*/, greater<float>> m_jets;
  vector<TLorentzVector /*jet_4-momentum*/> v_reco_bjets;
  for (unsigned int iJet = 0; iJet < jet_pT.GetSize() ; ++iJet) {
    float jetSystVar = 1.0;
    if( !process.Contains("Data") ){
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

  //Event Weight
  double EventWeight = 1;
  if( !option.Contains("Data") ){
    EventWeight *= *genWeight;
    ssize_t pos;
    //Pile Up weight
    if     ( (pos = syst_ext.find("puup")) != std::string::npos )
      EventWeight *= PUWeight[1];
    else if( (pos = syst_ext.find("pudown")) != std::string::npos )
      EventWeight *= PUWeight[2];
    else
      EventWeight *= PUWeight[0];
 
    //Prefire weight, 2017only
    if(process.Contains("2017")){
      if     ( (pos = syst_ext.find("prefireup")) != std::string::npos )
	EventWeight *= prefireweight[1];
      else if( (pos = syst_ext.find("prefiredown")) != std::string::npos )
	EventWeight *= prefireweight[2];
      else
	EventWeight *= prefireweight[0];
    }
    if(process.Contains("2016")){
      if( passchannel == 0 ){
	//mu [0]~[2]:ID/Iso, [3]~[5]:Trigger
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
    }
    else{
      if( passchannel == 0 ){
	//mu [0]~[2]:ID, [3]~[5]:Iso, [6]~[8]:Trigger
	if     ( (pos = syst_ext.find("muidup",0)) != std::string::npos )
	  EventWeight *= lepton_SF[1];
	else if( (pos = syst_ext.find("muiddown",0)) != std::string::npos )
	  EventWeight *= lepton_SF[2];
	else
	  EventWeight *= lepton_SF[0];

	if     ( (pos = syst_ext.find("muisoup",0)) != std::string::npos )
	  EventWeight *= lepton_SF[4];
	else if( (pos = syst_ext.find("muisodown",0)) != std::string::npos ) 
	  EventWeight *= lepton_SF[5];
	else
	  EventWeight *= lepton_SF[3];

	if     ( (pos = syst_ext.find("mutrgup",0)) != std::string::npos )
	  EventWeight *= lepton_SF[7];
	else if( (pos = syst_ext.find("mutrgdown",0)) != std::string::npos ) 
	  EventWeight *= lepton_SF[8];
	else
	  EventWeight *= lepton_SF[6];
      }
      else if( passchannel == 1){
	//el [0]~[2]: ID, [3]~[5]:Reco, [6]~[8]:Zvtx, [9]~[11]: Trigger
	if     ( (pos = syst_ext.find("elsfup",0)) != std::string::npos )
	  EventWeight *= lepton_SF[1];
	else if( (pos = syst_ext.find("elsfdown",0)) != std::string::npos )
	  EventWeight *= lepton_SF[2];
	else
	  EventWeight *= lepton_SF[0];

	if     ( (pos = syst_ext.find("elrecoup",0)) != std::string::npos )
	  EventWeight *= lepton_SF[4];
	else if( (pos = syst_ext.find("elrecodown",0)) != std::string::npos )
	  EventWeight *= lepton_SF[5];
	else
	  EventWeight *= lepton_SF[3];

	if     ( (pos = syst_ext.find("elzvtxup",0)) != std::string::npos )
	  EventWeight *= lepton_SF[7];
	else if( (pos = syst_ext.find("elzvtxdown",0)) != std::string::npos )
	  EventWeight *= lepton_SF[8];
	else
	  EventWeight *= lepton_SF[6];

	if     ( (pos = syst_ext.find("eltrgup",0)) != std::string::npos )
	  EventWeight *= lepton_SF[10];
	else if( (pos = syst_ext.find("eltrgdown",0)) != std::string::npos )
	  EventWeight *= lepton_SF[11];
	else
	  EventWeight *= lepton_SF[9];
      }
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

    if( (pos = syst_ext.find("pdfup",0)) != std::string::npos )
      EventWeight *= pdfweight[101];
    else if( (pos = syst_ext.find("pdfdown",0)) != std::string::npos )
      EventWeight *= pdfweight[100];
  }
 
  //std::cout << "genWeight: " << *genWeight << std::endl;
  //std::cout << "PUWeight: " << PUWeight[0] << std::endl;
  //std::cout << "lepton SF: " << lepton_SF[0] << std::endl;
  //std::cout << "jet_SF_CSV_30: " << jet_SF_CSV_30[0] << std::endl;
  for(int iCut=0; iCut <= passcut; ++iCut){
    h_control->h_lepton_pt[passchannel][iCut] ->Fill(lepton.Pt(),                  EventWeight);
    h_control->h_lepton_eta[passchannel][iCut]->Fill(abs(lepton.Eta()),            EventWeight);
    h_control->h_njets[passchannel][iCut]     ->Fill(njets,                        EventWeight);
    h_control->h_nbjets[passchannel][iCut]    ->Fill(nbjets,                       EventWeight);
    h_control->h_trans_mass[passchannel][iCut]->Fill(transverseMass(lepton,p4met), EventWeight);
    h_control->h_jet_pt_sum[passchannel][iCut]->Fill(jet_pt_sum,                   EventWeight);
    for(int iJet=0; iJet<nJet; ++iJet){
      h_control->h_jet_pt[passchannel][iCut][iJet] ->Fill(a_jetPt[iJet],  EventWeight);
      h_control->h_jet_eta[passchannel][iCut][iJet]->Fill(a_jetEta[iJet], EventWeight);
      h_control->h_csv[passchannel][iCut][iJet]    ->Fill(a_jetCSV[iJet], EventWeight);
    }
    
    h_control->h_reco_addbjets_deltaR[passchannel][iCut]  ->Fill(reco_addbjet_deltaR,   EventWeight);
    h_control->h_reco_addbjets_invMass[passchannel][iCut] ->Fill(reco_addbjet_invMass,  EventWeight);
    h_control->h_reco_addbjets_deltaEta[passchannel][iCut]->Fill(reco_addbjet_deltaEta, EventWeight);
    h_control->h_reco_addbjets_deltaPhi[passchannel][iCut]->Fill(reco_addbjet_deltaPhi, EventWeight);
    
    h_matrix->h_gen_gentop_deltaR[passchannel][iCut]  ->Fill(gen_addbjet_deltaR,    EventWeight);
    h_matrix->h_gen_gentop_invMass[passchannel][iCut] ->Fill(gen_addbjet_invMass,   EventWeight);
    h_matrix->h_gen_gentop_deltaEta[passchannel][iCut]->Fill(gen_addbjet_deltaEta,  EventWeight);
    h_matrix->h_gen_gentop_deltaPhi[passchannel][iCut]->Fill(gen_addbjet_deltaPhi,  EventWeight);
    h_matrix->h_gen_mindR_deltaR[passchannel][iCut]   ->Fill(gen_mindR_deltaR,      EventWeight);
    h_matrix->h_gen_mindR_invMass[passchannel][iCut]  ->Fill(gen_mindR_invMass,     EventWeight);
    h_matrix->h_gen_mindR_deltaEta[passchannel][iCut] ->Fill(gen_mindR_deltaEta,    EventWeight);
    h_matrix->h_gen_mindR_deltaPhi[passchannel][iCut] ->Fill(gen_mindR_deltaPhi,    EventWeight);

    h_matrix->h_respMatrix_gentop_deltaR[passchannel][iCut]  ->Fill(reco_addbjet_deltaR,   gen_addbjet_deltaR,   EventWeight);
    h_matrix->h_respMatrix_gentop_invMass[passchannel][iCut] ->Fill(reco_addbjet_invMass,  gen_addbjet_invMass,  EventWeight);
    h_matrix->h_respMatrix_gentop_deltaEta[passchannel][iCut]->Fill(reco_addbjet_deltaEta, gen_addbjet_deltaEta, EventWeight);
    h_matrix->h_respMatrix_gentop_deltaPhi[passchannel][iCut]->Fill(reco_addbjet_deltaPhi, gen_addbjet_deltaPhi, EventWeight);
    h_matrix->h_respMatrix_mindR_deltaR[passchannel][iCut]   ->Fill(reco_addbjet_deltaR,   gen_mindR_deltaR,     EventWeight);
    h_matrix->h_respMatrix_mindR_invMass[passchannel][iCut]  ->Fill(reco_addbjet_invMass,  gen_mindR_invMass,    EventWeight);
    h_matrix->h_respMatrix_mindR_deltaEta[passchannel][iCut] ->Fill(reco_addbjet_deltaEta, gen_mindR_deltaEta,   EventWeight);
    h_matrix->h_respMatrix_mindR_deltaPhi[passchannel][iCut] ->Fill(reco_addbjet_deltaPhi, gen_mindR_deltaPhi,   EventWeight);
  }//cut

  if(passlepton){
    for(int iCut=0; iCut <= passcut; ++iCut){
      h_control->h_lepton_pt[2][iCut] ->Fill(lepton.Pt(),                  EventWeight);
      h_control->h_lepton_eta[2][iCut]->Fill(abs(lepton.Eta()),            EventWeight);
      h_control->h_njets[2][iCut]     ->Fill(njets,                        EventWeight);
      h_control->h_nbjets[2][iCut]    ->Fill(nbjets,                       EventWeight);
      h_control->h_trans_mass[2][iCut]->Fill(transverseMass(lepton,p4met), EventWeight);
      h_control->h_jet_pt_sum[2][iCut]->Fill(jet_pt_sum,                   EventWeight);
      for(int iJet=0; iJet<nJet; ++iJet){
	h_control->h_jet_pt[2][iCut][iJet] ->Fill(a_jetPt[iJet],  EventWeight);
	h_control->h_jet_eta[2][iCut][iJet]->Fill(a_jetEta[iJet], EventWeight);
	h_control->h_csv[2][iCut][iJet]    ->Fill(a_jetCSV[iJet], EventWeight);
      }
      
      h_control->h_reco_addbjets_deltaR[2][iCut]  ->Fill(reco_addbjet_deltaR,   EventWeight);
      h_control->h_reco_addbjets_invMass[2][iCut] ->Fill(reco_addbjet_invMass,  EventWeight);
      h_control->h_reco_addbjets_deltaEta[2][iCut]->Fill(reco_addbjet_deltaEta, EventWeight);
      h_control->h_reco_addbjets_deltaPhi[2][iCut]->Fill(reco_addbjet_deltaPhi, EventWeight);
      
      h_matrix->h_gen_gentop_deltaR[2][iCut]  ->Fill(gen_addbjet_deltaR,    EventWeight);
      h_matrix->h_gen_gentop_invMass[2][iCut] ->Fill(gen_addbjet_invMass,   EventWeight);
      h_matrix->h_gen_gentop_deltaEta[2][iCut]->Fill(gen_addbjet_deltaEta,  EventWeight);
      h_matrix->h_gen_gentop_deltaPhi[2][iCut]->Fill(gen_addbjet_deltaPhi,  EventWeight);
      h_matrix->h_gen_mindR_deltaR[2][iCut]   ->Fill(gen_mindR_deltaR,      EventWeight);
      h_matrix->h_gen_mindR_invMass[2][iCut]  ->Fill(gen_mindR_invMass,     EventWeight);
      h_matrix->h_gen_mindR_deltaEta[2][iCut] ->Fill(gen_mindR_deltaEta,    EventWeight);
      h_matrix->h_gen_mindR_deltaPhi[2][iCut] ->Fill(gen_mindR_deltaPhi,    EventWeight);

      h_matrix->h_respMatrix_gentop_deltaR[2][iCut]  ->Fill(reco_addbjet_deltaR,   gen_addbjet_deltaR,   EventWeight);
      h_matrix->h_respMatrix_gentop_invMass[2][iCut] ->Fill(reco_addbjet_invMass,  gen_addbjet_invMass,  EventWeight);
      h_matrix->h_respMatrix_gentop_deltaEta[2][iCut]->Fill(reco_addbjet_deltaEta, gen_addbjet_deltaEta, EventWeight);
      h_matrix->h_respMatrix_gentop_deltaPhi[2][iCut]->Fill(reco_addbjet_deltaPhi, gen_addbjet_deltaPhi, EventWeight);
      h_matrix->h_respMatrix_mindR_deltaR[2][iCut]   ->Fill(reco_addbjet_deltaR,   gen_mindR_deltaR,     EventWeight);
      h_matrix->h_respMatrix_mindR_invMass[2][iCut]  ->Fill(reco_addbjet_invMass,  gen_mindR_invMass,    EventWeight);
      h_matrix->h_respMatrix_mindR_deltaEta[2][iCut] ->Fill(reco_addbjet_deltaEta, gen_mindR_deltaEta,   EventWeight);
      h_matrix->h_respMatrix_mindR_deltaPhi[2][iCut] ->Fill(reco_addbjet_deltaPhi, gen_mindR_deltaPhi,   EventWeight);
    }//cut
  }//passlepton

  return kTRUE;
}

void MyAnalysis::SlaveTerminate(){
  std::cout << "SlaveTerminate" << std::endl;
  option = GetOption();
  
  for(int iChannel=0; iChannel < nChannel; ++iChannel){
    for(int iStep=0; iStep < nStep; ++iStep){
      h_control->h_lepton_pt[iChannel][iStep]->AddBinContent(
          nbins_lep_pt, h_control->h_lepton_pt[iChannel][iStep]->GetBinContent(nbins_lep_pt+1));
      h_control->h_lepton_pt[iChannel][iStep]->ClearUnderflowAndOverflow();

      h_control->h_lepton_eta[iChannel][iStep]->AddBinContent(
          nbins_lep_eta, h_control->h_lepton_pt[iChannel][iStep]->GetBinContent(nbins_lep_eta+1));
      h_control->h_lepton_eta[iChannel][iStep]->ClearUnderflowAndOverflow();
      
      h_control->h_njets[iChannel][iStep]->AddBinContent(
          nbins_njets, h_control->h_njets[iChannel][iStep]->GetBinContent(nbins_njets+1));      
      h_control->h_njets[iChannel][iStep]->ClearUnderflowAndOverflow();
      
      h_control->h_nbjets[iChannel][iStep]->AddBinContent(
          nbins_nbjets, h_control->h_nbjets[iChannel][iStep]->GetBinContent(nbins_nbjets+1));
      h_control->h_nbjets[iChannel][iStep]->ClearUnderflowAndOverflow();
      
      h_control->h_jet_pt_sum[iChannel][iStep]->AddBinContent(
          nbins_jet_pt, h_control->h_jet_pt_sum[iChannel][iStep]->GetBinContent(nbins_jet_pt+1));
      h_control->h_jet_pt_sum[iChannel][iStep]->ClearUnderflowAndOverflow();
    
      h_control->h_trans_mass[iChannel][iStep]->AddBinContent(
          nbins_wmass, h_control->h_trans_mass[iChannel][iStep]->GetBinContent(nbins_wmass+1));
      h_control->h_trans_mass[iChannel][iStep]->ClearUnderflowAndOverflow();

      for(int iJet=0; iJet<nJet; ++iJet){
	h_control->h_jet_pt[iChannel][iStep][iJet]->AddBinContent(
	    nbins_jet_pt, h_control->h_jet_pt[iChannel][iStep][iJet]->GetBinContent(nbins_jet_pt+1));
	h_control->h_jet_pt[iChannel][iStep][iJet]->ClearUnderflowAndOverflow();

	h_control->h_jet_eta[iChannel][iStep][iJet]->AddBinContent(
	    nbins_jet_eta, h_control->h_jet_eta[iChannel][iStep][iJet]->GetBinContent(nbins_jet_eta+1));
	h_control->h_jet_eta[iChannel][iStep][iJet]->ClearUnderflowAndOverflow();

	h_control->h_csv[iChannel][iStep][iJet]->AddBinContent(
	    nbins_csv, h_control->h_csv[iChannel][iStep][iJet]->GetBinContent(nbins_csv+1));
	h_control->h_csv[iChannel][iStep][iJet]->ClearUnderflowAndOverflow();
      }//jet
       
      h_control->h_reco_addbjets_deltaR[iChannel][iStep]->AddBinContent(
          nbins_reco_addbjets_dR, h_control->h_reco_addbjets_deltaR[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dR+1));
      h_control->h_reco_addbjets_deltaR[iChannel][iStep]->ClearUnderflowAndOverflow();
      
      h_control->h_reco_addbjets_invMass[iChannel][iStep]->AddBinContent(
          nbins_reco_addbjets_M, h_control->h_reco_addbjets_invMass[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_M+1));
      h_control->h_reco_addbjets_invMass[iChannel][iStep]->ClearUnderflowAndOverflow();

      h_control->h_reco_addbjets_deltaEta[iChannel][iStep]->AddBinContent(
          nbins_reco_addbjets_dEta, h_control->h_reco_addbjets_deltaEta[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dEta+1));
      h_control->h_reco_addbjets_deltaEta[iChannel][iStep]->ClearUnderflowAndOverflow();
      
      h_control->h_reco_addbjets_deltaPhi[iChannel][iStep]->AddBinContent(
          nbins_reco_addbjets_dPhi, h_control->h_reco_addbjets_deltaPhi[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dPhi+1));
      h_control->h_reco_addbjets_deltaPhi[iChannel][iStep]->ClearUnderflowAndOverflow();
       
      h_matrix->h_gen_gentop_deltaR[iChannel][iStep]->AddBinContent(
          nbins_gen_addbjets_dR, h_matrix->h_gen_gentop_deltaR[iChannel][iStep]->GetBinContent(nbins_gen_addbjets_dR+1));
      h_matrix->h_gen_gentop_deltaR[iChannel][iStep]->ClearUnderflowAndOverflow();

      h_matrix->h_gen_gentop_invMass[iChannel][iStep]->AddBinContent(
          nbins_gen_addbjets_M, h_matrix->h_gen_gentop_invMass[iChannel][iStep]->GetBinContent(nbins_gen_addbjets_M+1));
      h_matrix->h_gen_gentop_invMass[iChannel][iStep]->ClearUnderflowAndOverflow();

      h_matrix->h_gen_gentop_deltaEta[iChannel][iStep]->AddBinContent(
          nbins_gen_addbjets_dEta, h_matrix->h_gen_gentop_deltaEta[iChannel][iStep]->GetBinContent(nbins_gen_addbjets_dEta+1));
      h_matrix->h_gen_gentop_deltaEta[iChannel][iStep]->ClearUnderflowAndOverflow();

      h_matrix->h_gen_gentop_deltaPhi[iChannel][iStep]->AddBinContent(
          nbins_gen_addbjets_dPhi, h_matrix->h_gen_gentop_deltaPhi[iChannel][iStep]->GetBinContent(nbins_gen_addbjets_dPhi+1));
      h_matrix->h_gen_gentop_deltaPhi[iChannel][iStep]->ClearUnderflowAndOverflow();

      h_matrix->h_gen_mindR_deltaR[iChannel][iStep]->AddBinContent(
          nbins_gen_addbjets_dR, h_matrix->h_gen_mindR_deltaR[iChannel][iStep]->GetBinContent(nbins_gen_addbjets_dR+1));
      h_matrix->h_gen_mindR_deltaR[iChannel][iStep]->ClearUnderflowAndOverflow();

      h_matrix->h_gen_mindR_invMass[iChannel][iStep]->AddBinContent(
          nbins_gen_addbjets_M, h_matrix->h_gen_mindR_invMass[iChannel][iStep]->GetBinContent(nbins_gen_addbjets_M+1));
      h_matrix->h_gen_mindR_invMass[iChannel][iStep]->ClearUnderflowAndOverflow();

      h_matrix->h_gen_mindR_deltaEta[iChannel][iStep]->AddBinContent(
          nbins_gen_addbjets_dEta, h_matrix->h_gen_mindR_deltaEta[iChannel][iStep]->GetBinContent(nbins_gen_addbjets_dEta+1));
      h_matrix->h_gen_mindR_deltaEta[iChannel][iStep]->ClearUnderflowAndOverflow();

      h_matrix->h_gen_mindR_deltaPhi[iChannel][iStep]->AddBinContent(
          nbins_gen_addbjets_dPhi, h_matrix->h_gen_mindR_deltaPhi[iChannel][iStep]->GetBinContent(nbins_gen_addbjets_dPhi+1));
      h_matrix->h_gen_mindR_deltaPhi[iChannel][iStep]->ClearUnderflowAndOverflow();

      for(int ixaxis=1; ixaxis <= nbins_reco_addbjets_dR; ++ixaxis){
        auto tmp = h_matrix->h_respMatrix_gentop_deltaR[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_dR)
	         + h_matrix->h_respMatrix_gentop_deltaR[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_dR+1);
	h_matrix->h_respMatrix_gentop_deltaR[iChannel][iStep]->SetBinContent(ixaxis, nbins_gen_addbjets_dR, tmp);
      }
      for(int iyaxis=1; iyaxis <= nbins_gen_addbjets_dR; ++iyaxis){
        auto tmp = h_matrix->h_respMatrix_gentop_deltaR[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dR,   iyaxis)
	         + h_matrix->h_respMatrix_gentop_deltaR[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dR+1, iyaxis);
	h_matrix->h_respMatrix_gentop_deltaR[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_dR, iyaxis, tmp);
      }
      double tmp = h_matrix->h_respMatrix_gentop_deltaR[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dR,   nbins_gen_addbjets_dR)
                 + h_matrix->h_respMatrix_gentop_deltaR[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dR+1, nbins_gen_addbjets_dR+1);
      h_matrix->h_respMatrix_gentop_deltaR[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_dR, nbins_gen_addbjets_dR, tmp);
      h_matrix->h_respMatrix_gentop_deltaR[iChannel][iStep]->ClearUnderflowAndOverflow();

      for(int ixaxis=1; ixaxis <= nbins_reco_addbjets_M; ++ixaxis){
        tmp = h_matrix->h_respMatrix_gentop_invMass[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_M)
	    + h_matrix->h_respMatrix_gentop_invMass[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_M+1);
	h_matrix->h_respMatrix_gentop_invMass[iChannel][iStep]->SetBinContent(ixaxis, nbins_gen_addbjets_M, tmp);
      }
      for(int iyaxis=1; iyaxis <= nbins_gen_addbjets_M; ++iyaxis){
        tmp = h_matrix->h_respMatrix_gentop_invMass[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_M,   iyaxis)
	    + h_matrix->h_respMatrix_gentop_invMass[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_M+1, iyaxis);
	h_matrix->h_respMatrix_gentop_invMass[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_M, iyaxis, tmp);
      }
      tmp = h_matrix->h_respMatrix_gentop_invMass[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_M,   nbins_gen_addbjets_M)
          + h_matrix->h_respMatrix_gentop_invMass[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_M+1, nbins_gen_addbjets_M+1);
      h_matrix->h_respMatrix_gentop_invMass[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_M, nbins_gen_addbjets_M, tmp);
      h_matrix->h_respMatrix_gentop_invMass[iChannel][iStep]->ClearUnderflowAndOverflow();

      for(int ixaxis=1; ixaxis <= nbins_reco_addbjets_dEta; ++ixaxis){
        auto tmp = h_matrix->h_respMatrix_gentop_deltaEta[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_dEta)
	         + h_matrix->h_respMatrix_gentop_deltaEta[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_dEta+1);
	h_matrix->h_respMatrix_gentop_deltaEta[iChannel][iStep]->SetBinContent(ixaxis, nbins_gen_addbjets_dEta, tmp);
      }
      for(int iyaxis=1; iyaxis <= nbins_gen_addbjets_dEta; ++iyaxis){
        auto tmp = h_matrix->h_respMatrix_gentop_deltaEta[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dEta,   iyaxis)
	         + h_matrix->h_respMatrix_gentop_deltaEta[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dEta+1, iyaxis);
	h_matrix->h_respMatrix_gentop_deltaEta[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_dEta, iyaxis, tmp);
      }
      tmp = h_matrix->h_respMatrix_gentop_deltaEta[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dEta,   nbins_gen_addbjets_dEta)
          + h_matrix->h_respMatrix_gentop_deltaEta[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dEta+1, nbins_gen_addbjets_dEta+1);
      h_matrix->h_respMatrix_gentop_deltaEta[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_dEta, nbins_gen_addbjets_dEta, tmp);
      h_matrix->h_respMatrix_gentop_deltaEta[iChannel][iStep]->ClearUnderflowAndOverflow();

      for(int ixaxis=1; ixaxis <= nbins_reco_addbjets_dPhi; ++ixaxis){
        auto tmp = h_matrix->h_respMatrix_gentop_deltaPhi[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_dPhi)
	         + h_matrix->h_respMatrix_gentop_deltaPhi[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_dPhi+1);
	h_matrix->h_respMatrix_gentop_deltaPhi[iChannel][iStep]->SetBinContent(ixaxis, nbins_gen_addbjets_dPhi, tmp);
      }
      for(int iyaxis=1; iyaxis <= nbins_gen_addbjets_dPhi; ++iyaxis){
        auto tmp = h_matrix->h_respMatrix_gentop_deltaPhi[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dPhi,   iyaxis)
	         + h_matrix->h_respMatrix_gentop_deltaPhi[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dPhi+1, iyaxis);
	h_matrix->h_respMatrix_gentop_deltaPhi[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_dPhi, iyaxis, tmp);
      }
      tmp = h_matrix->h_respMatrix_gentop_deltaPhi[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dPhi,   nbins_gen_addbjets_dPhi)
          + h_matrix->h_respMatrix_gentop_deltaPhi[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dPhi+1, nbins_gen_addbjets_dPhi+1);
      h_matrix->h_respMatrix_gentop_deltaPhi[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_dPhi, nbins_gen_addbjets_dPhi, tmp);
      h_matrix->h_respMatrix_gentop_deltaPhi[iChannel][iStep]->ClearUnderflowAndOverflow();

      for(int ixaxis=1; ixaxis <= nbins_reco_addbjets_dR; ++ixaxis){
        auto tmp = h_matrix->h_respMatrix_mindR_deltaR[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_dR)
	         + h_matrix->h_respMatrix_mindR_deltaR[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_dR+1);
	h_matrix->h_respMatrix_mindR_deltaR[iChannel][iStep]->SetBinContent(ixaxis, nbins_gen_addbjets_dR, tmp);
      }
      for(int iyaxis=1; iyaxis <= nbins_gen_addbjets_dR; ++iyaxis){
        auto tmp = h_matrix->h_respMatrix_mindR_deltaR[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dR,   iyaxis)
	         + h_matrix->h_respMatrix_mindR_deltaR[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dR+1, iyaxis);
	h_matrix->h_respMatrix_mindR_deltaR[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_dR, iyaxis, tmp);
      }
      tmp = h_matrix->h_respMatrix_mindR_deltaR[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dR,   nbins_gen_addbjets_dR)
          + h_matrix->h_respMatrix_mindR_deltaR[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dR+1, nbins_gen_addbjets_dR+1);
      h_matrix->h_respMatrix_mindR_deltaR[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_dR, nbins_gen_addbjets_dR, tmp);
      h_matrix->h_respMatrix_mindR_deltaR[iChannel][iStep]->ClearUnderflowAndOverflow();

      for(int ixaxis=1; ixaxis <= nbins_reco_addbjets_M; ++ixaxis){
        tmp = h_matrix->h_respMatrix_mindR_invMass[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_M)
	    + h_matrix->h_respMatrix_mindR_invMass[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_M+1);
	h_matrix->h_respMatrix_mindR_invMass[iChannel][iStep]->SetBinContent(ixaxis, nbins_gen_addbjets_M, tmp);
      }
      for(int iyaxis=1; iyaxis <= nbins_gen_addbjets_M; ++iyaxis){
        tmp = h_matrix->h_respMatrix_mindR_invMass[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_M,   iyaxis)
	    + h_matrix->h_respMatrix_mindR_invMass[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_M+1, iyaxis);
	h_matrix->h_respMatrix_mindR_invMass[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_M, iyaxis, tmp);
      }
      tmp = h_matrix->h_respMatrix_mindR_invMass[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_M,   nbins_gen_addbjets_M)
          + h_matrix->h_respMatrix_mindR_invMass[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_M+1, nbins_gen_addbjets_M+1);
      h_matrix->h_respMatrix_mindR_invMass[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_M, nbins_gen_addbjets_M, tmp);
      h_matrix->h_respMatrix_mindR_invMass[iChannel][iStep]->ClearUnderflowAndOverflow();

      for(int ixaxis=1; ixaxis <= nbins_reco_addbjets_dEta; ++ixaxis){
        auto tmp = h_matrix->h_respMatrix_mindR_deltaEta[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_dEta)
	         + h_matrix->h_respMatrix_mindR_deltaEta[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_dEta+1);
	h_matrix->h_respMatrix_mindR_deltaEta[iChannel][iStep]->SetBinContent(ixaxis, nbins_gen_addbjets_dEta, tmp);
      }
      for(int iyaxis=1; iyaxis <= nbins_gen_addbjets_dEta; ++iyaxis){
        auto tmp = h_matrix->h_respMatrix_mindR_deltaEta[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dEta,   iyaxis)
	         + h_matrix->h_respMatrix_mindR_deltaEta[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dEta+1, iyaxis);
	h_matrix->h_respMatrix_mindR_deltaEta[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_dEta, iyaxis, tmp);
      }
      tmp = h_matrix->h_respMatrix_mindR_deltaEta[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dEta,   nbins_gen_addbjets_dEta)
          + h_matrix->h_respMatrix_mindR_deltaEta[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dEta+1, nbins_gen_addbjets_dEta+1);
      h_matrix->h_respMatrix_mindR_deltaEta[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_dEta, nbins_gen_addbjets_dEta, tmp);
      h_matrix->h_respMatrix_mindR_deltaEta[iChannel][iStep]->ClearUnderflowAndOverflow();

      for(int ixaxis=1; ixaxis <= nbins_reco_addbjets_dPhi; ++ixaxis){
        auto tmp = h_matrix->h_respMatrix_mindR_deltaPhi[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_dPhi)
	         + h_matrix->h_respMatrix_mindR_deltaPhi[iChannel][iStep]->GetBinContent(ixaxis, nbins_gen_addbjets_dPhi+1);
	h_matrix->h_respMatrix_mindR_deltaPhi[iChannel][iStep]->SetBinContent(ixaxis, nbins_gen_addbjets_dPhi, tmp);
      }
      for(int iyaxis=1; iyaxis <= nbins_gen_addbjets_dPhi; ++iyaxis){
        auto tmp = h_matrix->h_respMatrix_mindR_deltaPhi[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dPhi,   iyaxis)
	         + h_matrix->h_respMatrix_mindR_deltaPhi[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dPhi+1, iyaxis);
	h_matrix->h_respMatrix_mindR_deltaPhi[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_dPhi, iyaxis, tmp);
      }
      tmp = h_matrix->h_respMatrix_mindR_deltaPhi[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dPhi,   nbins_gen_addbjets_dPhi)
          + h_matrix->h_respMatrix_mindR_deltaPhi[iChannel][iStep]->GetBinContent(nbins_reco_addbjets_dPhi+1, nbins_gen_addbjets_dPhi+1);
      h_matrix->h_respMatrix_mindR_deltaPhi[iChannel][iStep]->SetBinContent(nbins_reco_addbjets_dPhi, nbins_gen_addbjets_dPhi, tmp);
      h_matrix->h_respMatrix_mindR_deltaPhi[iChannel][iStep]->ClearUnderflowAndOverflow();
    }//step
  }//channel
}

void MyAnalysis::Terminate(){
  std::cout << "Terminate" << std::endl;
  option = GetOption();
  string str_opt = option.Data();

  string outname = str_opt.substr(str_opt.find_last_of("/")+1,string::npos);
  if( option.Contains("__") )
    syst_ext = str_opt.substr(str_opt.find_last_of("_")-1,string::npos);
  else
    syst_ext = "";

  TFile *out = TFile::Open(Form("output/root/hist_%s.root", outname.c_str()), "RECREATE");

  for(int iChannel=0; iChannel<nChannel; ++iChannel){
    for(int iStep=0; iStep<nStep; ++iStep){
      fOutput->FindObject(Form("h_%s_Ch%d_S%d%s",     RECO_LEP_PT_,  iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_%s_Ch%d_S%d%s",     RECO_LEP_ETA_, iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_%s_Ch%d_S%d%s",     RECO_N_JETS_,  iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_%s_Ch%d_S%d%s",     RECO_N_BJETS_, iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_%s_Ch%d_S%d%s",     RECO_TRANS_M_, iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_%s_sum_Ch%d_S%d%s", RECO_JET_PT_,  iChannel, iStep, syst_ext.c_str()))->Write();
      for(int iJet=0; iJet<nJet; ++iJet){
	fOutput->FindObject(Form("h_%s_%d_Ch%d_S%d%s", RECO_JET_PT_,  iJet, iChannel, iStep, syst_ext.c_str()))->Write();
	fOutput->FindObject(Form("h_%s_%d_Ch%d_S%d%s", RECO_JET_ETA_, iJet, iChannel, iStep, syst_ext.c_str()))->Write();
	fOutput->FindObject(Form("h_%s_%d_Ch%d_S%d%s", RECO_CSV_,     iJet, iChannel, iStep, syst_ext.c_str()))->Write();
      }
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s", RECO_ADD_DR_,   iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s", RECO_ADD_M_,    iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s", RECO_ADD_DETA_, iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s", RECO_ADD_DPHI_, iChannel, iStep, syst_ext.c_str()))->Write();

      fOutput->FindObject(Form("h_gentop_%s_Ch%d_S%d%s", GEN_ADD_DR_,   iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_gentop_%s_Ch%d_S%d%s", GEN_ADD_M_,    iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_gentop_%s_Ch%d_S%d%s", GEN_ADD_DETA_, iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_gentop_%s_Ch%d_S%d%s", GEN_ADD_DPHI_, iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s",  GEN_ADD_DR_,   iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s",  GEN_ADD_M_,    iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s",  GEN_ADD_DETA_, iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s",  GEN_ADD_DPHI_, iChannel, iStep, syst_ext.c_str()))->Write();

      fOutput->FindObject(Form("h_gentop_%s_Ch%d_S%d%s", MATRIX_DR_,   iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_gentop_%s_Ch%d_S%d%s", MATRIX_M_,    iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_gentop_%s_Ch%d_S%d%s", MATRIX_DETA_, iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_gentop_%s_Ch%d_S%d%s", MATRIX_DPHI_, iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s",  MATRIX_DR_,   iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s",  MATRIX_M_,    iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s",  MATRIX_DETA_, iChannel, iStep, syst_ext.c_str()))->Write();
      fOutput->FindObject(Form("h_mindR_%s_Ch%d_S%d%s",  MATRIX_DPHI_, iChannel, iStep, syst_ext.c_str()))->Write();
    }//step
  }//channel
  out->Write();
  out->Close();
}

double MyAnalysis::transverseMass(const TLorentzVector & lepton, const TLorentzVector & met){

  TLorentzVector leptonT(lepton.Px(),lepton.Py(),0.,lepton.E()*TMath::Sin(lepton.Theta()));
  TLorentzVector metT(met.Px(), met.Py(), 0, met.E());

  return (leptonT+metT).M();
}
