import os
import ana
import time
import subprocess

start_time = time.time()

run = True
test = False
#inputDir = '/data/users/seohyun/array/'
inputDir = '/home/seohyun/work/heptool/deepAna/keras/array/'
outputDir = '/home/seohyun/work/heptool/deepAna/keras/hist/'

ana.ana('array/TTLJ_PowhegPythia_ttbbFilter_ttbb','TTLJ_PowhegPythia_ttbbFilter_ttbb',outputDir)

if run:
    if test:
        ana.ana('/home/seohyun/work/heptool/deepAna/keras/array/TTLJ_PowhegPythia_ttbb','TTLJ_PowhegPythia_ttbb',outputDir)
        #ana.ana('/home/seohyun/work/heptool/deepAna/keras/array/test','TTLJ_PowhegPythia_ttbb',outputDir)
        #ana.ana(inputDir+'TT_PowhegHerwig_ttbb','TT_PowhegHerwig_ttbb',outputDir)

    else :
        for process in os.listdir(inputDir):
            if process.endswith(".h5"): continue
            print("Start "+str(process))
            ana.ana(inputDir+process, process, outputDir)

print("Total running time :%s " %(time.time() - start_time))
