1. Searching additional two b jets using DNN

First, you need to make pandas arrays to train and evaluate. You can do it using runAna.py in deepAna/keras directory.

In runAna.py, there are several options.
~~~
array # If you turn on this option and run, runAna.py make arrays.
array_test # For test. If you turn on this, runAna.py make only test set.
array_train # If you turn on this option with array, runAna.py make training input.
array_syst # If you turn on this option with array, runAna.py make systematic array sets.
~~~

Run as follows,

~~~
$ python runAna.py
~~~

If you make pandas arrays successfully, then you can make model file using model.py

Run as follows,

~~~
$ python model.py
~~~

Then, you can make histogram using runAna.py with analysis option.
~~~
analysis # If you turn on this option, runAna.py make histogram root files.
ana_test # For test.
ana_syst # For systematics
~~~

Run as follows,

~~~
$ python runAna.py
~~~

Then, final root files are made.

2. Differential cross section measurement.
All the codes for measurement are in ttbbDiffXsec directory.

You need to made acceptance distribution first.

~~~
$ root -l -b -q makeCriteria.C
~~~

Then, stability, purity and acceptance plots are made in output/post directory.
And then you can run runUnfold.py. If you run this, you can get unfolding results.

~~~
$ python runUnfold.py
~~~

you can control unfolding option in ttbbDiffXsec.C






