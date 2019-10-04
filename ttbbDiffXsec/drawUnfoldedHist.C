#include <unistd.h>

#include "../include/tdrstyle.C"
TGraphErrors *buildGraphFromHist(TH1* h){
  TGraphErrors *grp = new TGraphErrors();

  for ( int i=1; i<=h->GetNbinsX(); ++i ) {
    const double w = h->GetXaxis()->GetBinWidth(i);
    const double y = h->GetBinContent(i);
    const double err = h->GetBinError(i);
    grp->SetPoint(i-1, h->GetBinCenter(i), y);
    //grp->SetPointError(i-1, w/2, err);
    grp->SetPointError(i-1, 0, err);
  }
  grp->SetEditable(false);

  return grp;
}
void drawUnfoldedHist(std::string year){
  gROOT->ProcessLine("setTDRStyle();");
  gStyle->SetPaintTextFormat("4.1f");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TGaxis::SetMaxDigits(3);
  std::string input_dir = "../output/unfold/" + year;

  TPaveText *label_cms = tdrCMSlabel();
  TPaveText *label_sim = tdrCMSSimlabel();
  TCanvas *c = new TCanvas("c","c",800,800);
  TLegend *leg = new TLegend(0.6, 0.7, 0.88, 0.88);

  int exidata, exittbb;
  access(Form("%s/hist_unfolded_data.root",input_dir.c_str()), exidata);
  access(Form("%s/hist_unfolded_data.root",input_dir.c_str()), exittbb);
  if(exidata == -1){
    std::cout << "There is no unfolded data root file" << std::endl;
    return 0;
  }
  if(exittbb == -1){
    std::cout << "There is no unfolded ttbb root file" << std::endl;
    return 0;
  }

  TFile *f_data = TFile::Open(Form("%s/hist_unfolded_data.root",input_dir.c_str()));
  TFile *f_ttbb = TFile::Open(Form("%s/hist_unfolded_ttbb.root",input_dir.c_str()));

  for(int ich = 0; ich < 3; ++ich){
    //1. Subtracted data vs ttbb mc
    //auto h_subDataDR = (TH1 *)f_data->Get(Form("Input_h_mindR_RecoAddbJetDeltaR_Ch%d_S3", ich));
    //auto h_subDataM  = (TH1 *)f_data->Get(Form("Input_h_mindR_RecoAddbJetInvMass_Ch%d_S3", ich));
    //auto h_mcInput   = (TH1 *)f_data->Get(Form("Input_h_mindR_RecoAddbJetInvMass_Ch%d_S3", ich));
    //2. Response matrix
    auto h_matrixDR = (TH2 *)f_ttbb->Get(Form("h_mindR_ResponseMatrixDeltaR_Ch%d_S3",ich));
    auto h_matrixM  = (TH2 *)f_ttbb->Get(Form("h_mindR_ResponseMatrixInvMass_Ch%d_S3",ich));
    h_matrixDR->Draw("box");
    label_sim->Draw("same");
    c->Print(Form("%s/h_mindR_ResponseMatrixDeltaR_Ch%d_S3",input_dir.c_str(),ich));
    c->Clear();
    
    h_matrixM->Draw("box");
    label_sim->Draw("same");
    c->Print(Form("%s/h_mindR_ResponseMatrixInvMass_Ch%d_S3",input_dir.c_str(),ich));
    c->Clear();

    //3. x-axis proj. matrix vs Step 3 ttbb mc
    auto h_projDR = h_matrixDR->ProjectionX();
    auto h_projM  = h_matrixM->ProjectionX();
    auto h_mcDR   = (TH1 *)f_ttbb->Get(Form("Input_h_mindR_RecoAddbJetDeltaR_Ch%d_S3", ich));
    auto h_mcM    = (TH1 *)f_ttbb->Get(Form("Input_h_mindR_RecoAddbJetInvMass_Ch%d_S3", ich));
    
    h_projDR->SetLineColor(kRed);
    h_projDR->SetLineWidth(2);
    h_projDR->SetMaximum(h_projDR->GetMaximum()*1.5);
    h_projDR->SetMinimum(0);
    h_mcDR->SetLineWidth(2);
    leg->AddEntry(h_projDR, "x-axis projection", "l");
    leg->AddEntry(h_mcDR, "ttbb MC input", "l");
    h_projDR->Draw("hist");
    h_mcDR->Draw("hist same");
    leg->Draw("same");
    label_sim->Draw("same");
    c->Print(Form("%s/h_xaxisVSmc_dR_Ch%d_S3.pdf",input_dir.c_str(),ich), "pdf");
    leg->Clear();
    c->Clear();

    h_projM->SetLineColor(kRed);
    h_projM->SetLineWidth(2);
    h_projM->SetMaximum(h_projM->GetMaximum()*1.5);
    h_projM->SetMinimum(0);
    h_mcM->SetLineWidth(2);
    leg->AddEntry(h_projM, "x-axis projection", "l");
    leg->AddEntry(h_mcM, "ttbb MC input", "l");
    h_projM->Draw("hist");
    h_mcM->Draw("hist same");
    leg->Draw("same");
    label_sim->Draw("same");
    c->Print(Form("%s/h_xaxisVSmc_M_Ch%d_S3.pdf",input_dir.c_str(),ich), "pdf");
    leg->Clear();
    c->Clear();

    auto h_genDR = (TH1 *)f_data->Get(Form("h_mindR_GenAddbJetDeltaR_Ch%d_S3",ich));
    auto h_genM  = (TH1 *)f_data->Get(Form("h_mindR_GenAddbJetInvMass_Ch%d_S3",ich));
    auto h_diffgenDR = (TH1 *)f_data->Get(Form("diffXsec_h_mindR_GenAddbJetDeltaR_Ch2_nosel"));
    auto h_diffgenM  = (TH1 *)f_data->Get(Form("diffXsec_h_mindR_GenAddbJetInvMass_Ch2_nosel"));
    h_genDR->SetLineColor(kRed);
    h_genDR->SetLineWidth(2);
    h_genDR->SetMaximum(h_genDR->GetMaximum()*1.5);
    h_genM->SetLineColor(kRed);
    h_genM->SetLineWidth(2);
    h_genM->SetMaximum(h_genM->GetMaximum()*1.5);
    if(ich == 0){
      h_diffgenDR->SetLineColor(kRed);
      h_diffgenDR->SetFillColor(kRed);
      h_diffgenDR->SetMaximum(h_diffgenDR->GetMaximum()*1.5);
      h_diffgenM->SetLineColor(kRed);
      h_diffgenM->SetFillColor(kRed);
      h_diffgenM->SetMaximum(h_diffgenM->GetMaximum()*1.5);
    }
    //4. Unfolded ttbb vs Step 3 gen
    auto h_unfmcDR = (TH1 *)f_ttbb->Get(Form("Unfolded_h_mindR_RecoAddbJetDeltaR_Ch%d_S3",ich));
    auto h_unfmcM  = (TH1 *)f_ttbb->Get(Form("Unfolded_h_mindR_RecoAddbJetInvMass_Ch%d_S3",ich));
    h_unfmcDR->SetLineWidth(2);
    h_unfmcM->SetLineWidth(2);
    
    leg->AddEntry(h_genDR, "Step 3 gen", "l");
    leg->AddEntry(h_unfmcDR, "Unfolded MC", "l");
    h_genDR->Draw("hist");
    h_unfmcDR->Draw("hist same");
    leg->Draw("same");
    label_sim->Draw("same");
    c->Print(Form("%s/h_unfMC_dR_Ch%d_S3.pdf",input_dir.c_str(), ich), "pdf");
    leg->Clear();
    c->Clear();

    leg->AddEntry(h_genM, "Step 3 gen", "l");
    leg->AddEntry(h_unfmcM, "Unfolded MC", "l");
    h_genM->Draw("hist");
    h_unfmcM->Draw("hist same");
    leg->Draw("same");
    label_sim->Draw("same");
    c->Print(Form("%s/h_unfMC_M_Ch%d_S3.pdf",input_dir.c_str(), ich), "pdf");
    leg->Clear();
    c->Clear();

    //5. diffXsec ttbb vs diffXsec gen
    auto h_diffmcDR = (TH1 *)f_ttbb->Get(Form("diffXsec_Unfolded_h_mindR_RecoAddbJetDeltaR_Ch%d_S3",ich));
    auto h_diffmcM  = (TH1 *)f_ttbb->Get(Form("diffXsec_Unfolded_h_mindR_RecoAddbJetInvMass_Ch%d_S3",ich));
    h_diffmcDR->SetLineWidth(2);
    h_diffmcM->SetLineWidth(2);

    leg->AddEntry(h_diffgenDR, "Powheg+Pythia", "F");
    leg->AddEntry(h_diffmcDR, "ttbb MC", "p");
    h_diffgenDR->Draw("hist");
    h_diffmcDR->Draw("p e same");
    leg->Draw("same");
    label_sim->Draw("same");
    c->Print(Form("%s/h_xsecMC_dR_Ch%d_S3.pdf",input_dir.c_str(), ich),"pdf");
    leg->Clear();
    c->Clear();

    leg->AddEntry(h_diffgenM, "Powheg+Pythia", "F");
    leg->AddEntry(h_diffmcM, "ttbb MC", "l");
    h_diffgenM->Draw("hist");
    h_diffmcM->Draw("p e same");
    leg->Draw("same");
    label_sim->Draw("same");
    c->Print(Form("%s/h_xsecMC_M_Ch%d_S3.pdf",input_dir.c_str(), ich),"pdf");
    leg->Clear();
    c->Clear();
    
    //6. Unfolded data vs Step 3 gen
    auto h_unfdataDR = buildGraphFromHist((TH1 *)f_data->Get(Form("Unfolded_h_mindR_RecoAddbJetDeltaR_Ch%d_S3",ich)));
    auto h_unfdataM  = buildGraphFromHist((TH1 *)f_data->Get(Form("Unfolded_h_mindR_RecoAddbJetInvMass_Ch%d_S3",ich)));

    leg->AddEntry(h_genDR, "Step 3 gen", "l");
    leg->AddEntry(h_unfdataDR, "Data", "p");
    h_genDR->Draw("hist");
    h_unfdataDR->Draw("p same");
    leg->Draw("same");
    label_sim->Draw("same");
    c->Print(Form("%s/h_unfData_dR_Ch%d_S3.pdf",input_dir.c_str(), ich),"pdf");
    leg->Clear();
    c->Clear();

    leg->AddEntry(h_genM, "Step 3 gen", "l");
    leg->AddEntry(h_unfdataM, "Data", "p");
    h_genM->Draw("hist");
    h_unfdataM->Draw("p same");
    leg->Draw("same");
    label_sim->Draw("same");
    c->Print(Form("%s/h_unfData_M_Ch%d_S3.pdf",input_dir.c_str(), ich),"pdf");
    leg->Clear();
    c->Clear();

    //7. diffXsec data vs diffXsec gen
    auto h_diffdataDR = buildGraphFromHist((TH1 *)f_data->Get(Form("diffXsec_Unfolded_h_mindR_RecoAddbJetDeltaR_Ch%d_S3",ich)));
    auto h_diffdataM  = buildGraphFromHist((TH1 *)f_data->Get(Form("diffXsec_Unfolded_h_mindR_RecoAddbJetDeltaR_Ch%d_S3",ich)));

    leg->AddEntry(h_diffgenDR, "Powheg+Pythia", "F");
    leg->AddEntry(h_diffdataDR, "Data", "p");
    h_diffgenDR->Draw("hist");
    h_diffdataDR->Draw("p same");
    leg->Draw("same");
    label_sim->Draw("same");
    c->Print(Form("%s/h_xsecData_dR_Ch%d_S3.pdf",input_dir.c_str(), ich),"pdf");
    leg->Clear();
    c->Clear();

    leg->AddEntry(h_genM, "Powheg+Pythia", "F");
    leg->AddEntry(h_unfdataM, "Data", "p");
    h_diffgenM->Draw("hist");
    h_diffdataM->Draw("p same");
    leg->Draw("same");
    label_sim->Draw("same");
    c->Print(Form("%s/h_xsecData_M_Ch%d_S3.pdf",input_dir.c_str(), ich),"pdf");
    leg->Clear();
    c->Clear();
  }
}
