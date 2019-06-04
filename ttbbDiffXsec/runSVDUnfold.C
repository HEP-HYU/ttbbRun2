R__LOAD_LIBRARY(libRooUnfold)

#include <iostream>

#include <TSystem.h>

//Output vector
//[0]: SVDUnfold output
//[1]: Error matrix propagated before unfolding
//[2]: Total error matrix

std::vector<TH1 *> runSVDUnfold(TH1 *h_in_, TH2 *h_resp_,
    std::map<const char *, TH1 *> m_bkg_, std::map<const char *, double> m_scale_,
    std::map<const char *, TH2 *> m_sys_,
    int reg = -1, bool scanReg = false)
{
  gSystem->Load("/home/seohyun/tools/RooUnfold/build/libRooUnfold.so");
   
  auto h_in = h_in_;
  auto h_resp = h_resp_;
  auto h_reco = h_resp->ProjectionX();
  auto h_gen = h_resp->ProjectionY();

  if(!m_bkg_.empty()){
    for(auto m_itr=m_bkg_.begin(); m_itr != m_bkg_.end(); ++m_itr){
      auto h_tmp = m_itr->second;
      h_tmp->Scale(m_scale_[m_itr->first]);
      h_in->Add(h_tmp, -1);
    }
  }

  RooUnfold::ErrorTreatment err = RooUnfold::kCovariance;
  RooUnfoldResponse *roo_resp;
  RooUnfold *roo_unfold;

  roo_resp = new RooUnfoldResponse(h_reco, h_gen, h_resp);
  
  if(reg == -1) roo_unfold = new RooUnfoldSvd(roo_resp, h_in);
  else  roo_unfold = new RooUnfoldSvd(roo_resp, h_in, reg);
  auto h_out = (TH1 *)roo_unfold->Hreco(err);
  h_out->SetName(Form("Unfolded_%s", h_in->GetName()));
  //cout << "Regularization Parameter : " << roo_unfold->GetRegParm() << endl;
  
  //roo_unfold = new RooUnfoldInvert(roo_resp, h_in);
  //auto h_out = (TH1 *)roo_unfold->Hreco(err);

  TH2D *h2_ematrix = new TH2D(roo_unfold->Ereco(err));
  h2_ematrix->SetName(Form("%s_EmatrixInput", h_in->GetName()));
  //h2_ematrix->SetName(Form("Ematrix_%s", h_in->GetName()));
  vector<TH1 *> v_output;
  v_output.push_back(h_out);
  v_output.push_back(h2_ematrix);
  
  if(!m_sys_.empty()){
    vector<TH2 *> v_sys_err;
    for(auto m_itr=m_sys_.begin(); m_itr != m_sys_.end(); ++m_itr){
      RooUnfoldResponse *resp_sys = new RooUnfoldResponse(
        (m_itr->second)->ProjectionX(), (m_itr->second)->ProjectionY(), m_itr->second);
      RooUnfold *unfold_sys = new RooUnfoldSvd(resp_sys, h_in, reg);
      auto h_sysout = (TH1 *)unfold_sys->Hreco(err);
      auto h_deltasys = (TH1 *)h_out->Clone();
      h_deltasys->Add(h_sysout, -1);
      h_deltasys->SetName(Form("%s_DeltaSysSource%s", h_in->GetName(), m_itr->first));
      v_output.push_back(h_deltasys);
      //TH2D *h2_err_sys = new TH2D(unfold_sys->Ereco(err));
      //h2_err_sys->SetName(Form("EmatrixSys_%s%s", (*v_itr)->GetName(), h_in->GetName()));
      //v_sys_err.push_back(h2_err_sys);
    }
    //TH2 *h2_err_total = (TH2 *)v_output[1]->Clone();
    //for(auto v_itr=v_sys_err.begin(); v_itr != v_sys_err.end(); ++v_itr){
    //  TH2 *h2_tmp = (TH2 *)(*v_itr)->Clone();
    //  h2_err_total->Add(h2_tmp);
    //}
    //v_output.push_back(h2_err_total);
  }
  return v_output;
}
