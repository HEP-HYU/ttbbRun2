import os
import ntuple2array as tt

with open("var.txt",'r') as f :
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

"""
with open("input.txt",'r') as f:
  while True :
    line = f.readline()
    if not line : break
    tmp = line.split()
    inputDir = tmp[0]
    inputFile = tmp[1]
    process = tmp[2]
    tt.makeCombi(inputDir, inputFile, process, 'input')
"""
