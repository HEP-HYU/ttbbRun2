import os
import ana
import time

start_time = time.time()

run  = True
syst = True
test = False
#inputDir = '/data/users/seohyun/array/'
inputDir = '/home/seohyun/work/heptool/deepAna/keras/array/'
outputDir = '/home/seohyun/work/heptool/deepAna/keras/hist/'

#ana(inputDir, process, outputDir, sys, flag1=False)
#ana.ana('array/TTLJ_PowhegPythia_ttbbFilter_ttbb','TTLJ_PowhegPythia_ttbbFilter_ttbb',outputDir)

syslist = [
    '__puup', '__pudown',
    '__musfup', '__musfdown', '__mutrgup', '__mutrgdown',
    '__elsfup', '__elsfdown', '__eltrgup', '__eltrgdown',
    '__lfup', '__lfdown', '__hfup', '__hfdown',
    '__hfstat1up', '__hfstat1down', '__hfstat2up', '__hfstat2down',
    '__lfstat1up', '__lfstat1down', '__lfstat2up', '__lfstat2down',
    '__cferr1up', '__cferr1down', '__cferr2up', '__cferr2down',
    '__scale0', '__scale1', '__scale2', '__scale3', '__scale4', '__scale5'
]

if run:
    if test:
        #ana.ana('/home/seohyun/work/heptool/deepAna/keras/array/TTLJ_PowhegPythia_ttbb','TTLJ_PowhegPythia_ttbb',outputDir)
        #ana.ana('/home/seohyun/work/heptool/deepAna/keras/array/TTLJ_PowhegPythia_ttbb__jerdown','TTLJ_PowhegPythia_ttbb__jerdown',outputDir)
        ana.ana('/home/seohyun/work/heptool/deepAna/keras/array/ZZ_Pythia','ZZ_Pythia',outputDir)
        #ana.ana(inputDir+'TT_PowhegHerwig_ttbb','TT_PowhegHerwig_ttbb',outputDir)

    else :
        for process in os.listdir(inputDir):
            if process.endswith(".h5"): continue
            if not ('SYS' in process or '__' in process or 'Herwig' in process or 'Evtgen' in process):
                print("Start "+str(process))
                ana.ana(inputDir+process, process, outputDir)

                if syst and not 'Data' in process:
                    for sys in syslist:
                        if 'scale' in sys and not 'tt' in process: continue
                        ana.ana(inputDir+process, process, outputDir, sys)
            else:
                ana.ana(inputDir+process, process, outputDir)

print("Total running time :%s " %(time.time() - start_time))
