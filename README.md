Part1. making response matrix and unfolding input

To run deepAna/keras/ana.py, you must convert root file to hd5 file.

In directory deepAna/keras, Type this: python makeArray.py

makeArray.py has some options:
1. array: making hdf file for all samples.
2. train: making DNN training sample.

You can train DNN model file to run deepAna/keras/model.py

To start analysis, run deepAna/keras/runAna.py

runAna.py has some options:
1. test: with this option, test the code running well or not using TTLJ_PowhegPythia_ttbb sample.
2. closureTest: with this, option closure test run.

Part2. making control plots

Part3. unfolding
To start unfolding, type root -l -b -q ttbbDiffXsec.C in ttbbDiffXsec

ttbbDiffXsec.C has arguments:
bool useTUnfold_ = false
bool scanbyLcurve_ = true
double tauMin_ = 0.
double tauMax_ = 0.
bool fixTau_ = false
float fixedTau_ = 0.
