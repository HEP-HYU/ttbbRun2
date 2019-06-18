#! /usr/bin/env python
import pandas as pd

import os

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-f", "--inputFile", metavar='F', type='string', action='store',
                  default = "",
                  dest='inputFile',
                  help='Input file')

parser.add_option("-c", "--cuts", metavar='F', type='string', action='store',
                  default = "",
                  dest='cuts',
                  help='cuts')

(options, args) = parser.parse_args()

print options.inputFile

df = pd.read_hdf(options.inputFile)
df = df.query(options.cuts)

#print df
print "total entries = " , len(df)

