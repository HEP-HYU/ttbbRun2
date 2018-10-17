import os
import ana
import time
import subprocess

start_time = time.time()

run = True
test = False 
closureTest = False
inputDir = '/data/users/seohyun/array/'
outputDir = '/home/seohyun/work/heptool/deepAna/keras/hist/'

if closureTest:
    ana.ana(inputDir+'TTLJ_PowhegPythia_ttbb','TTLJ_PowhegPythia_ttbb',outputDir,True)

if run:
    if test:
        ana.ana(inputDir+'TTLJ_PowhegPythia_ttbb','TTLJ_PowhegPythia_ttbb',outputDir)
        #ana.ana(inputDir+'TT_PowhegHerwig_ttbb','TT_PowhegHerwig_ttbb',outputDir)

    else :
        for process in os.listdir(inputDir):
            if process.endswith(".h5"): continue
            print("Start "+str(process))
            ana.ana(inputDir+process, process, outputDir)
        ana.ana(inputDir+'TT_PowhegHerwig_ttbb','TT_PowhegHerwig_ttbb',outputDir)
print("Total running time :%s " %(time.time() - start_time))
