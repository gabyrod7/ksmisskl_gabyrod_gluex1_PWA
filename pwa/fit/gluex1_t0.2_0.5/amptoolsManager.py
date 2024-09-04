#!/bin/python3

import amptoolsCfg
import amptoolsSDME as sdme
import amptoolsPWA as pwa
import os
from sys import argv
import argparse
from multiprocessing import Pool

# ## Do an SDME fit
# analysisType = 'sdme' # 'sdme' or 'pwa'
# nBins = 9
# nProcess = 9
# nFits = 10
# nBootstraps = 100

# pathToDSelectorFlatTrees = '/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PhiSDME/sdme/DSelector/'
# pathToTrees = '/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PhiSDME/sdme/python_dev/kskl/trees/'
# baseDirectory = os.getcwd()
# cleanOldFits = True

# divideData_split = 'split_t' # 'split_t' or 'split_mass'
# divideData_splitFlag= '-e'
# lowerBoundData = 0.15 # lower bound for t if using split_t
# higherBoundData = 1.0 # higher bound for t if using split_t

# runPeriod = 'gluex1'
# fitName = 'Amptools_SDMEfit'
# plotterName = 'phi1020_plotter'
# seedFile = 'seed'

# # setup config file settings
# cfgFileName = 'amptools.cfg'
# cfgFile = amptoolsCfg.amptoolsCfg()
# cfgFile.set_data([runPeriod+'_'+polAngle for runPeriod in [runPeriod] for polAngle in ['000', '045', '090', '135']]) # fit each polarization angle separately for GlueX-I data
# # cfgFile.set_data([runPeriod+'_'+polAngle for runPeriod in ['sp17', 'sp18', 'fa18'] for polAngle in ['000', '045', '090', '135']]) # Fit each polarization angle and each GeluX-I run period separately
# cfgFile.set_particles('Beam Proton KShort KLong')
# cfgFile.set_fname(cfgFileName)
# cfgFile.set_fit_name(fitName)
# cfgFile.set_polarization('hist') # use histogram for polarization fraction
# cfgFile.set_pol_hist_location('/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PhiSDME/polarization_fraction/')
# cfgFile.set_amplitudes('sdme') # use SDME amplitudes
# cfgFile.set_parRange(True) # needed for randomized SDME fits
# # cfgFile.set_bkgAmplitude('uniform') # add incoherent uniform background amplitude
# # cfgFile.set_include_bkg(False) # do not include bkgnd files in fit

## Do a PWA fit
analysisType = 'pwa'
nBins = 30
nProcess = 15
nFits = 50
nBootstraps = 0 # not implemented for PWA

pathToDSelectorFlatTrees = '/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PWA/fit_mkskl_upto_2600MeV/DSelector/'
baseDirectory = os.getcwd()
pathToTrees = f'{baseDirectory}/trees/'
cleanOldFits = True

divideData_split = 'split_mass'
divideData_splitFlag= ''
lowerBoundData = 1.1 # lower bound for mass if using split_mass
higherBoundData = 2.6 # higher bound for mass if using split_mass

runPeriod = 'gluex1'
fitName = 'AmpTools_PWAfit'
plotterName = 'kskl_plotter'
reactionName = 'REACTION'
seedFile = 'seed'
waveSets = ['Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-']
# waveSets = ['Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-', 'Sp0+_Sp0-_Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-']

cfgFileName = 'amptools.cfg'
cfgFile = amptoolsCfg.amptoolsCfg()
cfgFile.set_data([runPeriod+'_'+polAngle for runPeriod in ['gluex1'] for polAngle in ['000', '045', '090', '135']]) # fit each polarization angle separately for GlueX-I data
cfgFile.set_particles('Beam Proton KShort KLong')
cfgFile.set_fname(cfgFileName)
cfgFile.set_fit_name(fitName)
cfgFile.set_reaction_name(reactionName)
cfgFile.set_amplitudes('twopsZlm')
cfgFile.set_polarization('average')
    
def main(argv):
    parser = argparse.ArgumentParser(
        prog='AmpTools Manager', 
        description='Run AmpTools fits and plot results. The flags can be provided in any order. For the initial fit run ./amptoolsManager.py -s -d -S -f -r -D to setup, run and draw SDMEs. ',
        epilog='AmpTools Manager finshed'
    )

    parser.add_argument('-s', '--setupTrees', action='store_true', help='Take flat trees from DSelector and apply cuts and divide data by polarization angle.')
    parser.add_argument('-d', '--divideData', action='store_true', help='Setup directories and data')
    parser.add_argument('-S', '--setupFit', action='store_true', help='Setup config files for fits')
    parser.add_argument('-f', '--fit', nargs='*', type=int, help='Run fit in (default) all bins or (optional) specific bins')
    parser.add_argument('-r', '--readFit', action='store_true', help='Run readSDMEs()')
    parser.add_argument('-D', '--draw', action='store_true', help='Make plot of SDMEs or PWA mass intensities')
    parser.add_argument('-p', '--runPlotter', action='store_true', help='Run runPlotter()')
    parser.add_argument('-q', '--drawAngles', action='store_true', help='Make plots of angles')
    parser.add_argument('-b', '--plotBins', action='store_true', help='Run plotBins()')
    parser.add_argument('-B', '--bootstrap', action='store_true', help='Run bootstrap')
    parser.add_argument('-R', '--rmAmpToolsCfg', action='store_true', help=f'Remove {cfgFileName} file from all bins')
    
    args = parser.parse_args()
    # print(args)
    
    if args.setupTrees:
        if analysisType == 'sdme':
            sdme.setupTrees(pathToDSelectorFlatTrees)
        elif analysisType == 'pwa':
            pwa.setupTrees(pathToDSelectorFlatTrees)
        
    if args.divideData:
        if analysisType == 'sdme':
            sdme.divideData(fitName, nBins, lowerBoundData, higherBoundData, pathToTrees, divideData_split, divideData_splitFlag, runPeriod)
        elif analysisType == 'pwa':
            pwa.divideData(fitName, nBins, lowerBoundData, higherBoundData, pathToTrees, divideData_split, divideData_splitFlag, runPeriod)
        
    if args.rmAmpToolsCfg:
        if analysisType == 'sdme':
            sdme.removeAmpToolsCfg(cfgFileName, nBins, baseDirectory, fitName)
        if analysisType == 'pwa':
            for waveSet in waveSets:
                pwa.removeAmpToolsCfg(cfgFileName, nBins, waveSet, baseDirectory, fitName)
    
    if args.setupFit:
        if analysisType == 'sdme':
            sdme.setupFit(cfgFile, nBins, baseDirectory, fitName, cfgFileName, cleanOldFits)
        if analysisType == 'pwa':
            for waveSet in waveSets:
                pwa.setupFit(cfgFile, nBins, waveSet, baseDirectory, fitName, cfgFileName, cleanOldFits)
        
    if analysisType == 'sdme':
        if args.fit != None and args.fit != []:
            paths = [f'{baseDirectory}/{fitName}/bin_{iBin}' for iBin in args.fit]
        else:
            paths = [f'{baseDirectory}/{fitName}/bin_{iBin}' for iBin in range(nBins)]
    elif analysisType == 'pwa':
        if args.fit != None and args.fit != []:
            paths = [f'{baseDirectory}/{fitName}/bin_{iBin}/{waveSet}' for waveSet in waveSets for iBin in args.fit]
        else:
            paths = [f'{baseDirectory}/{fitName}/bin_{iBin}/{waveSet}' for waveSet in waveSets for iBin in range(nBins)]

    if args.fit != None:
        if analysisType == 'sdme':
            runFit = sdme.runFit
        elif analysisType == 'pwa':
            runFit = pwa.runFit
            
        if nProcess == 1:
            for path in paths:
                runFit(path, cfgFileName, seedFile, nFits, fitName)
        else:
            p = Pool(nProcess)
            p.starmap(runFit, [(path, cfgFileName, seedFile, nFits, fitName) for path in paths])

    if args.readFit:
        if analysisType == 'sdme':
            sdme.readFit(fitName, nBins)
        elif analysisType == 'pwa':
            for path in paths:
                pwa.readFit(path, cfgFileName, fitName, reactionName, runPeriod)
        
    if args.draw:
        os.chdir(baseDirectory)

        if analysisType == 'sdme':
            sdme.drawSDME(nBins, lowerBoundData, higherBoundData)
        elif analysisType == 'pwa':
            for waveSet in waveSets:
                pwa.drawMassIntensities(fitName, waveSet)
        
    if args.runPlotter:
        if analysisType == 'sdme':
            runPlotter = sdme.runPlotter
        elif analysisType == 'pwa':
            runPlotter = pwa.runPlotter
                
        if nProcess == 1:
            for path in paths:
                runPlotter(path, plotterName)
        else:
            p = Pool(nProcess)
            p.starmap(runPlotter, [(path, plotterName, fitName) for path in paths])
            
    if args.drawAngles:
        os.chdir(baseDirectory)

        if analysisType == 'sdme':
            sdme.drawAngles(fitName)
        elif analysisType == 'pwa':
            for waveSet in waveSets:
                pwa.drawAngles(nBins, fitName, waveSet)
        
    if args.plotBins:
        if analysisType == 'sdme':
            sdme.runPlotBins(fitName, nBins)
        elif analysisType == 'pwa':
            pwa.runPlotBins()

    if args.bootstrap:
        if analysisType == 'sdme':
            bootstrapSetup = sdme.bootstrapSetup
            runBootstrapFits = sdme.runBootstrapFits
        elif analysisType == 'pwa':
            print('Bootstrap not implemented for PWA.\n Exiting...')
            exit()
        
        if nProcess == 1:
            for path in paths:
                bootstrapSetup(path, cfgFileName, nBootstraps, fitName)
                runBootstrapFits(path)
        else:
            p = Pool(nProcess)
            p.map(bootstrapSetup, [(path, cfgFileName, nBootstraps, fitName) for path in paths])
            p.map(runBootstrapFits, [(path, nBootstraps) for path in paths])

if __name__ == '__main__':
    main(argv[1:])
