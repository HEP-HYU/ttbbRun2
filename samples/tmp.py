import os
import sys

samples = []
with open('sample16.txt','r') as f:
    n = 1
    while True:
        line = f.readline()
	if not line: break
	print(str(n)+": "+line[:-1])
	if n == 1:
	    inputDir = line[:-1]
	else:
	    samples.append(line[:-1])
	n += 1

print(inputDir)
print(samples)

for year in xrange(16,18):
    print(year)
