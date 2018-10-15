To run deepAna/keras/ana.py, you must convert root file to hd5 file.

In directory deepAna/keras, Type this: python makeArray.py

makeArray.py has some option:
1. array: making hdf file for all samples.
2. train: making DNN training sample.

You can make model file using deepAna/keras/model.py

You can run deepAna/keras/runAna.py which makes deltaR and invariant mass distribution using DNN.

Option in runAna.py
1. test; with this option, test the code running well or not using TTLJ_PowhegPythia_ttbb sample.
2. closureTest; with this, option closure test run.
