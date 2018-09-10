import os
import ntuple2array as tt
import multiprocessing
from functools import partial
import time

start_time = time.time()

array = True
train = True

inputSet = []
proc = []
if array :
    with open("input.txt",'r') as f:
        print("make input array")
        while True :
            line = f.readline()
            if not line : break
            tmp = line.split()
            inputDir = tmp[0]
            inputFile = tmp[1]
            tt.makeCombi(inputFile)

#number_of_workers = 7
#pool = multiprocessing.Pool(number_of_workers)
#pool.map(tt.makeCombi,inputSet)
#pool.close()
#pool.join()

#tt.makeCombi(inputDir, inputFile, process, 'input')

if train :
    with open("var.txt",'r') as f :
        print "make training input sample"
        output = ""
        while True :
            line = f.readline()
            if not line : break
            tmp = line.split()
            if "trainInput" in tmp : output = tmp[1]

        path_dir = 'input/'
        file_list = os.listdir(path_dir)

        if not output in file_list :
            tt.makeTrainingInput(path_dir, output)
        else : print "training sample already exists"

print("Real Time : %s " %(time.time() - start_time))
