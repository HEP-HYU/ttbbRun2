void scanHist(TH1 *h_in_){
  auto h_in = h_in_;

  cout << "=============== " << h_in->GetName() << " ===============" << endl;
  for(int iBin=1; iBin<h_in->GetNbinsX()+1; ++iBin){
    if(iBin == 1)
      cout << iBin << "st bin value : " << h_in->GetBinContent(iBin) << endl;
    else if(iBin == 2)
      cout << iBin << "nd bin value : " << h_in->GetBinContent(iBin) << endl;
    else if(iBin == 3)
      cout << iBin << "rd bin value : " << h_in->GetBinContent(iBin) << endl;
    else
      cout << iBin << "th bin value : " << h_in->GetBinContent(iBin) << endl;
  }
}

void scanHist(TH2 *h_in_){
  auto h_in = h_in_;

  cout << "=============== " << h_in->GetName() << " ===============" << endl;
  for(int iYbin=1; iYbin<h_in->GetNbinsY()+1; ++iYbin){
    for(int iXbin=1; iXbin<h_in->GetNbinsX()+1; ++iXbin){
	cout << "(" << iXbin << "," << iYbin << ") bin value : " << h_in->GetBinContent(iXbin,iYbin) << endl;
    }
  }
  cout << "Number of events : " << h_in->Integral() << endl;
}
