1. Searching additional two b jets using DNN
First, you need to make pandas arrays to train and evaluate. You can do it using runAna.py

In runAna.py, there are several options.
~~~
array # If you turn on this option and run, runAna.py make arrays.
array_test # For test. If you turn on this, runAna.py make only test set.
array_train # If you turn on this option with array, runAna.py make training input.
array_syst # If you turn on this option with array, runAna.py make systematic array sets.
~~~

~~~
$ python runAna.py
~~~

If you make pandas arrays successfully, then you can make model file using model.py
~~~
$ python model.py
~~

Then, you can make histogram using runAna.py with analysis option.
~~~
analysis # If you turn on this option, runAna.py make histogram root files.
ana_test # For test.
ana_syst # For systematics
~~~
