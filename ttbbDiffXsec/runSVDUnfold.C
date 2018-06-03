R__LOAD_LIBRARY(libRooUnfold)

#include <iostream>

#include <TSystem.h>

TH1 *runSVDUnfold(TH1 *h_in_, TH2 *h_resp_, int reg){
  gSystem->Load("/home/seohyun/tools/RooUnfold/build/libRooUnfold.so");
  
  auto h_in = h_in_;
  auto h_resp = h_resp_;
  TH1 *h_reco = h_resp->ProjectionX();
  TH1 *h_gen = h_resp->ProjectionY();

  RooUnfold::ErrorTreatment err = RooUnfold::kCovariance;
  RooUnfoldResponse *roo_resp;
  RooUnfold *roo_unfold;

  roo_resp = new RooUnfoldResponse(h_reco, h_gen, h_resp);
  roo_unfold = new RooUnfoldSvd(roo_resp, h_in, reg);
  auto h_out = (TH1 *)roo_unfold->Hreco(err);

  return h_out;
}
