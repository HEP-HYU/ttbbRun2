import sys, os
import math
from tqdm import tqdm

def getVarlist():
    #list backup
    column = ['dR','dEta','dPhi',
        'nuPt','nuEta','nuPhi','nuMass',
        'lbPt','lbEta','lbPhi','lbMass',
        'lb1Pt','lb1Eta','lb1Phi','lb1Mass',
        'lb2Pt','lb2Eta','lb2Phi','lb2Mass',
        'diPt','diEta','diPhi','diMass',
        'csv1','csv2','pt1','pt2','eta1','eta2','phi1','phi2','e1','e2']

    #column = [
    #    'dR','dEta', 'dPhi',
    #    'nudR','nudPhi','nuPt','numT',
    #    'lepdR','lepdEta', 'lepdPhi', 'lepPt', 'lepM',
    #    'lb1dR', 'lb1dEta', 'lb1dPhi', 'lb1Pt', 'lb1M',
    #    'lb2dR', 'lb2dEta', 'lb2dPhi', 'lb2Pt', 'lb2M',
    #    'nub1dR', 'nub1dPhi', 'nub1Pt', 'nub1mT',
    #    'nub2dR', 'nub2dPhi', 'nub2Pt', 'nub2mT',
    #    'diPt','diEta','diM',
    #    'b1Pt','b1E','b2Pt','b2E'
    #]
    return column

def getHistXlabel():
    #list backup
    dictionary = {
        'dR':'#DeltaR_{b#bar{b}}', 'dEta':'#Delta#eta_{b#bar{b}}','dPhi':'#Delta#phi_{b#bar{b}}',
        'nuPt':'pT_{b#bar{b}#nu}', 'nuEta':'#eta_{b#bar{b}#nu}', 'nuPhi':'#phi_{b#bar{b}#nu}', 'nuMass':'m_{b#bar{b}#nu}',
        'lbPt':'pT_{b#bar{b}l}', 'lbEta':'#eta_{b#bar{b}l}', 'lbPhi':'#phi_{b#bar{b}l}', 'lbMass':'m_{b#bar{b}l}',
        'lb1Pt':'pT_{b1l}', 'lb1Eta':'#eta_{b1l}', 'lb1Phi':'#phi_{b1l}', 'lb1Mass':'m_{b1l}',
        'lb2Pt':'pT_{b2l}', 'lb2Eta':'#eta_{b2l}', 'lb2Phi':'#phi_{b2l}', 'lb2Mass':'m_{b2l}',
        'diPt':'pT_{b#bar{b}}', 'diEta':'#eta_{b#bar{b}}', 'diPhi':'#phi_{b#bar{b}}', 'diMass':'m_{b#bar{b}}',
        'csv1':'csv_{b1}', 'csv2':'csv_{b2}','pt1':'pT_{b1}','pt2':'pT_{b2}', 'eta1':'#eta_{b1}', 'eta2':'#eta_{b2}',
        'phi1':'#phi_{b1}', 'phi2':'#phi_{b2}', 'e1':'E_{b1}', 'e2':'E_{b2}'
    }

    #dictionary = {
    #    'dR':'#DeltaR_{b#bar{b}}', 'dEta':'#Delta#eta_{b#bar{b}}','dPhi':'#Delta#phi_{b#bar{b}}',
    #    'nudR':'#DeltaR_{b#bar{b}#nu}', 'nudPhi':'#Delta#phi_{b#bar{b}#nu}',
    #    'nuPt':'pT_{b#bar{b}#nu}', 'numT':'mT_{b#bar{b}#nu}',
    #    'lepdR':'#DeltaR_{b#bar{b}l}', 'lepdEta':'#Delta#eta_{b#bar{b}l}',
    #    'lepdPhi':'#Delta#phi_{b#bar{b}l}', 'lepPt':'pT_{b#bar{b}l}', 'lepM':'m_{b#bar{b}l}',
    #    'lb1dR':'#DeltaR_{b1l}', 'lb1dEta':'#Delta#eta_{b1l}', 'lb1dPhi':'#Delta#phi_{b1l}',
    #    'lb1Pt':'pT_{b1l}', 'lb1M':'m_{b1l}',
    #    'lb2dR':'#DeltaR_{b2l}', 'lb2dEta':'#Delta#eta_{b2l}', 'lb2dPhi':'#Delta#phi_{b2l}',
    #    'lb2Pt':'pT_{b2l}', 'lb2M':'m_{b2l}',
    #    'nub1dR':'#DeltaR_{b1#nu}', 'nub1dPhi':'#Delta#phi_{b1#nu}',
    #    'nub1Pt':'pT_{b1#nu}', 'nub1mT':'mT_{b1#nu}',
    #    'nub2dR':'#DeltaR_{b2#nu}', 'nub2dPhi':'#Delta#phi_{b2#nu}',
    #    'nub2Pt':'pT_{b2#nu}', 'nub2mT':'mT_{b2#nu}',
    #    'diPt':'pT_{b#bar{b}}', 'diEta':'#eta_{b#bar{b}}', 'diM':'m_{b#bar{b}}',
    #    'b1Pt':'pT_{b1}', 'b1E':'E_{b1}', 'b2Pt':'pT_{b2}', 'b2E':'E_{b2}'
    #}

    return dictionary

def getHistRange(var):

    histRange = []

    if 'dR' in var:
        histRange = [20, 0, 4]
    elif 'dEta' in var:
        histRange = [20, 0, 2.5]
    elif 'dPhi' in var:
        histRange = [20, 0, math.pi]
    elif 'Pt' in var:
        histRange = [20, 0, 400]
    elif ('Eta' in var) and (not 'dEta' in var):
        histRange = [20, -2.5, 2.5]
    elif ('Phi' in var) and (not 'dPhi' in var):
        histRange = [20, -math.pi, math.pi]
    elif ('mT' in var) or ('M' in var):
        histRange = [20, 0, 400]
    elif ('E' in var) and (not 'Et' in var) or ('e' in var):
        histRange = [20, 0, 400]
    elif 'csv' in var:
        histRange = [20, 0, 1]
    else:
        histRange = [20, 0, 10]
        print("Variable does not exist")

    return histRange

def printProgress(iteration, total, prefix = '', suffix = '', decimals = 1, barLength = 100):
    nEvent = str(iteration) + '/' + str(total)
    formatStr = "{0:." + str(decimals) + "f}"
    percent = formatStr.format(100*(iteration/float(total)))
    filledLength = int(round(barLength * iteration/float(total)))
    bar = '#'*filledLength + '-'*(barLength-filledLength)
    sys.stdout.write('\r%s |%s| %s%s %s %s' % (prefix, bar, percent, '%', suffix, nEvent)),
    if iteration == total:
        sys.stdout.write('\n')
    sys.stdout.flush()

