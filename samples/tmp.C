#include <iostream>
#include <string>

void tmp(){
  std::string sample = "/data/users/seohyun/work/ttbb/TTLJ_ttbb__last";
  std::string out;
  std::string syst;
  out = sample.substr(sample.find_last_of("/")+1,string::npos);
  syst = sample.substr(sample.find_last_of("_")-1, string::npos);

  std::cout << out << std::endl;
  std::cout << syst << std::endl;
}
