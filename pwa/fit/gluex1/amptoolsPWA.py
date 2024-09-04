import os

def setupFit(cfg_file, nBins, waveSet, baseDirectory, fitName, cfgFileName, cleanOldFits=False):
    for iBin in range(nBins):
        path = f'{baseDirectory}/{fitName}/bin_{iBin}'
        
        if not os.path.exists(f'{path}/{waveSet}'):
            os.mkdir(f'{path}/{waveSet}')
        
        if not os.path.exists(f'{path}/{waveSet}/{cfgFileName}'):
            print(f'Create {path}/{waveSet}/{cfgFileName}')
            
            cfg_file.set_fitDataLocation(f'{path}')
            cfg_file.set_wave_set(waveSet)
            cfg_file.set_ext(f'_{iBin}.root')
            cfg_file.set_fname(f'{path}/{waveSet}/{cfgFileName}')
            
            print(f'Write {path}/{waveSet}/{cfgFileName}')
            cfg_file.write_amptools_config()
        else:
            print(f'{path}/{waveSet}/{cfgFileName} exists')
            
        if not cleanOldFits:
            continue
                 
        if os.path.exists(f'{path}/{waveSet}/fit.log'):
            os.remove(f'{path}/{waveSet}/fit.log')
            print(f'Remove {path}/fit.log')

def removeAmpToolsCfg(cfgFileName, nBins, waveSet, baseDirectory, fitName):
    for iBin in range(nBins):
        path = f'{baseDirectory}/{fitName}/bin_{iBin}/{waveSet}'
        if os.path.exists(f'{path}/{cfgFileName}'):
            os.remove(f'{path}/{cfgFileName}')
            print(f'Remove {path}/{cfgFileName}')

def readFit(path, cfgFileName, fitName, reactionName, runPeriod):
    if os.path.exists(f'{path}'):
        os.chdir(f'{path}')
    else:
        print(f'{path} does not exist')
        return

    cmd = f'getAmpsInBin {cfgFileName} {fitName}.fit 000_045_090_135 true {path.split("/")[-1]} {reactionName}_{runPeriod}'
    print(cmd)
    os.system(cmd)

def runFit(path, cfgFileName, seedFile, nFits, fitName):
    print(f'Move to directory {path}')
    os.chdir(path)
    
    cmd = f'fit -c {cfgFileName} -s {seedFile} -r {nFits} > fit.log'
    print(cmd)
    os.system(cmd)
    
    print(f'Done fitting {path}')

    # try:
    #     os.rename(f'{fitName}.fit', 'nominal.fit')
    # except:
    #     print(f'Could not rename {fitName}.fit to nominal.fit - Working direcotry {path}')
    
    # os.system(f'mv {fitName}*.fit fits_results/')

def runPlotter(path, plotterName, fitName):
    os.chdir(path)
    waveSet = path.split('/')[-1]
    
    partialWaves = waveSet.split('_')
    
    for wave in partialWaves:
        cmd = f'{plotterName} {fitName}.fit -s {wave} -a F -F {wave} -o hist.root > plot_{wave}.log' # do not acceptance correction
        print(cmd)
        os.system(cmd)
 
    cmd = f'{plotterName} {fitName}.fit -s {waveSet} -a F -F {waveSet} -o hist.root > plot_{waveSet}.log' # do not acceptance correction
    print(cmd)
    os.system(cmd)

def drawMassIntensities(nBins, fitName, waveSet):
    cmd = f'root -l -b -q \'drawMassIntensities.C({nBins}, "{fitName}", "{waveSet}")\''
    print(cmd)
    os.system(cmd)
    
def drawAngles(nBins, fitName, waveSet):
    print('Run drawPWAangles.C script')
    cmd = f'root -l -b -q \'drawPWAangles.C({nBins}, "{fitName}", "{waveSet}")\''
    print(f'    {cmd}')
    os.system(cmd)

def runPlotBins():
    cmd = 'root -l -b -q plotBinsPWA.C'
    print(f'    {cmd}')
    os.system(cmd)

def setupTrees(pathToDSelectorFlatTrees):
    print('Run setup_trees.C script')
    #cmd = 'root -b -q -l setup_trees.C'
    cmd = f'root -b -q -l \'setup_trees.C("{pathToDSelectorFlatTrees}")\''
    print(f'    {cmd}')
    os.system(cmd)

def divideData(fitName, nBins, lowerBoundData, higherBoundData, pathToTrees, divideData_split, divideData_splitFlag, runPeriod):
    print('Run divideData.pl script')
    cmd = f'./divideData.pl {fitName} {nBins} {lowerBoundData} {higherBoundData} {pathToTrees} {runPeriod} {divideData_split} {divideData_splitFlag}'
    print(f'    {cmd}')
    os.system(cmd)
