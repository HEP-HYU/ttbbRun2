import os
import multiprocessing
import ana

test = True
inputDir = '/data/users/seohyun/array/'
outputDir = '/home/seohyun/work/heptool/deepAna/keras/hist/'

if test :
    ana.ana(inputDir+'TTLJ_PowhegPythia_ttbb','TTLJ_PowhegPythia_ttbb','outputDir')

else :
    for process in os.listdir(inputDir) :
        print("Start "+str(process))
        ana.ana(inputDir+process, process, outputDir)
