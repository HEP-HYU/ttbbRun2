void preProcess(std::string year){
  std::cout << "Run Pre-process, year: " << year << std::endl;
  std::vector<const char *> v_process = {"data_obs", "ttbb", "ttbj", "ttcc", "ttLF", "ttbkg", "other"};
  std::vector<const char *> v_variable = {"DeltaR", "InvMass"};
  TFile *f_out;
  if ( year != "19" ){
    f_out = TFile::Open(Form("../output/postfit/%s/hist_postfit.root", year.c_str()), "recreate");
    TFile *f_post = TFile::Open(Form("../output/postfit/datacards_200114_run20%s_2/ttbb/postfit_shapes_ttbb_Discriminant_bb_All.root", year.c_str()));
    for( int i=0; i < 2; ++i ){
      f_out->cd();
      for( unsigned int j = 0; j < v_process.size(); ++j ){
        auto tmp0 = (TH1 *)f_post->Get(Form("bb_%s_postfit/%s", v_variable[i], v_process[j]));
        tmp0->SetName(Form("h_mindR_RecoAddbJet%s_Ch2_S3_%s", v_variable[i], v_process[j]));
        tmp0->Scale(2.0);
        tmp0->Write();
        if( j == 1 ){
          auto *h_ratio = (TH1 *)tmp0->Clone();
          auto *h_prefit0 = (TH1 *)f_post->Get(Form("bb_%s_prefit/ttbb", v_variable[i]));
          h_prefit0->Scale(2.0);
          h_ratio->Divide(h_prefit0);
          h_ratio->SetName(Form("h_mindR_RecoAddbJet%s_Ch2_S3_ratio", v_variable[i]));
          h_ratio->Write();
          h_ratio->Draw("hist");
        }
      }
    }
  }
  else{
    TFile *f_post17 = TFile::Open(Form("../output/postfit/datacards_200114_run2017_2/ttbb/postfit_shapes_ttbb_Discriminant_bb_All.root"));
    TFile *f_post18 = TFile::Open(Form("../output/postfit/datacards_200114_run2018_2/ttbb/postfit_shapes_ttbb_Discriminant_bb_All.root"));
    f_out = TFile::Open(Form("../output/postfit/19/hist_postfit.root"), "recreate");
    for( int i = 0; i < 2; ++i ){
      f_out->cd();
      for ( unsigned int j = 0; j < v_process.size(); ++j ){
        auto tmp0 = (TH1 *)f_post17->Get(Form("bb_%s_postfit/%s", v_variable[i], v_process[j]));
        auto tmp1 = (TH1 *)f_post18->Get(Form("bb_%s_postfit/%s", v_variable[i], v_process[j]));
        tmp0->Scale(2.0);
        tmp1->Scale(2.0);
        tmp0->Add(tmp1);
        tmp0->SetName(Form("h_mindR_RecoAddbJet%s_Ch2_S3_%s", v_variable[i], v_process[j]));
        tmp0->Write();
        if( j == 1 ){
          auto *h_ratio = (TH1 *)tmp0->Clone();
          auto *h_prefit0 = (TH1 *)f_post17->Get(Form("bb_%s_prefit/ttbb", v_variable[i]));
          auto *h_prefit1 = (TH1 *)f_post18->Get(Form("bb_%s_prefit/ttbb", v_variable[i]));
          h_prefit0->Scale(2.0);
          h_prefit1->Scale(2.0);
          h_prefit0->Add(h_prefit1);
          h_ratio->Divide(h_prefit0);
          h_ratio->SetName(Form("h_mindR_RecoAddbJet%s_Ch2_S3_ratio", v_variable[i]));
          h_ratio->Write();
          h_ratio->Draw("hist");
        }
      }
    }
  }
  f_out->Close();
}
