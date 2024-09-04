import os
from math import sqrt, fabs
from random import randint

def setupFit(cfg_file, nBins, baseDirectory, fitName, cfgFileName, cleanOldFits=False):
    for iBin in range(nBins):
        path = f'{baseDirectory}/{fitName}/bin_{iBin}'
        
        if not os.path.exists(f'{path}/{cfgFileName}'):
            print(f'Create {path}/{cfgFileName}')
            
            cfg_file.set_fitDataLocation(f'{path}')
            cfg_file.set_ext(f'_{iBin}.root')
            cfg_file.set_fname(f'{path}/{cfgFileName}')
            
            print(f'Write {path}/{cfgFileName}')
            cfg_file.write_amptools_config()
        else:
            print(f'{path}/{cfgFileName} exists')
        
        if not os.path.exists(f'{path}/fits_results/'):
            os.mkdir(f'{path}/fits_results/')
            
        if not cleanOldFits:
            continue
        
        if os.path.exists(f'{path}/nominal.fit'):
            # os.system(f'rm {path}/nominal.fit')
            os.remove(f'{path}/nominal.fit')
            print(f'Remove {path}/nominal.fit')
            
        if os.path.exists(f'{path}/fit.log'):
            # os.system(f'rm {path}/fit.log')
            os.remove(f'{path}/fit.log')
            print(f'Remove {path}/fit.log')

def removeAmpToolsCfg(cfgFileName, nBins, baseDirectory, fitName):
    for iBin in range(nBins):
        path = f'{baseDirectory}/{fitName}/bin_{iBin}'
        if os.path.exists(f'{path}/{cfgFileName}'):
            os.remove(f'{path}/{cfgFileName}')
            print(f'Remove {path}/{cfgFileName}')
        else:
            print(f'{path}/{cfgFileName} does not exist')

def readSDMEfit(fitFileName):
    parNames = []
    parVals = []
    parErrs = []
    
    if not os.path.exists(fitFileName):
        return 0, parVals, parErrs
    
    with open(fitFileName, 'r') as f:
        for line in f: # skip lines until we reach the parameters section
          if(line == '+++ Parameter Values and Errors +++\n'):
               break
        line = f.readline()
        npar = int(line) # get number of parameters 

        # Now we read the production amplitudes and SDME parameters of the fit
        for nline, line in enumerate(f, start = 0):
            line = line.strip().split('\t')
            parNames.append(line[0])
            parVals.append(line[1])
            if nline == npar-1:
                break # done reading SDME parameter values

        # The errror is the sqrt of the diagonal of the covariance matrix
        # so we read the nth element of the nth line
        for nline, line in enumerate(f, start = 0):
            line = line.strip().split('\t')
            cov = float(line[nline])
            if cov < 0:
                cov = fabs(cov)
            parErrs.append(sqrt(cov))
            if nline == npar-1:
                break # done reading SDME uncertainties 
            
    return npar, parVals, parErrs

def readFit(fitName, nBins):
    parVals = []
    parErrs = []
    tBins = {}

	#Read bin center and width from tBins.txt
    with open(f'{fitName}/tBins.txt', 'r') as f:
        for line in f:
            line = line.strip().split(',')
            tBins['bin_'+line[0]] = [line[1], line[2]]

    opf = open('sdme.csv', 'w')
    
    for iBin in range(nBins):
        fitFileName = f'{fitName}/bin_{iBin}/nominal.fit'
        nPar, parVals, parErrs = readSDMEfit(fitFileName)
        
        bin = f'bin_{iBin}'
        line = f'{tBins[bin][0]},{tBins[bin][1]},'
        
        if nPar == 0:
            line = line+'1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0\n'
            opf.write(line)
            print(f'Warning: bin number {iBin} did not converge. All SDMEs set to 1 and 0 error')
            continue

        for i in range(nPar-9, nPar): # SDME parameters should be the last 9 parameters
            line += f'{parVals[i]},{parErrs[i]},'
        opf.write(line[:-1]+'\n')

    print('SDMEs fit parameters collected')

def runPlotter(path, plotterName, fitName):
    os.chdir(path)
    
    if os.path.exists('nominal.fit'):
        cmd = f'{plotterName} {fitName}.fit'
        print(f'run {cmd} in {path}')
        os.system(cmd)
    else:
        print(f'no nominal.fit in {path}')

def runFit(path, cfgFileName, seedFile, nFits, fitName):
    print(f'Move to directory {path}')
    os.chdir(path)
    
    cmd = f'fit -c {cfgFileName} -s {seedFile} -r {nFits} > fit.log'
    print(cmd)
    os.system(cmd)
    
    print(f'Done fitting {path}')

    try:
        os.rename(f'{fitName}.fit', 'nominal.fit')
    except:
        print(f'Could not rename {fitName}.fit to nominal.fit - Working direcotry {path}')

def runFit(path, cfgFileName, seedFile, nFits, fitName):
    print(f'Move to directory {path}')
    os.chdir(path)
    
    cmd = f'fit -c {cfgFileName} -s {seedFile} -r {nFits} > fit.log'
    print(cmd)
    os.system(cmd)
    
    print(f'Done fitting {path}')

    try:
        os.rename(f'{fitName}.fit', 'nominal.fit')
    except:
        print(f'Could not rename {fitName}.fit to nominal.fit - Working direcotry {path}')
    
    os.system(f'mv {fitName}*.fit fits_results/')

def drawSDME(nBins, lowerBoundData, higherBoundData):
    print('Run drawsdme.C script')
    cmd = f'root -l -b -q \'drawSDME.C({nBins}, {lowerBoundData}, {higherBoundData})\''
    print(f'    {cmd}')
    os.system(cmd)

def drawAngles(fitName):
    print('Run plotter.C script')
    cmd = f'root -l -b -q \'plotter.C("{fitName}", 3, 3)\''
    print(f'    {cmd}')
    os.system(cmd)

def runPlotBins(fitName, nBins):
    cmd = f'root -l -b -q \'plotBinsSDME.C("{fitName}", {nBins})\''
    print(cmd)
    os.system(cmd)

def bootstrapSetup(path, cfgFileName, nBootstraps, fitName):
    if not os.path.exists(f'{path}/bootstrap/'):
        os.mkdir(f'{path}/bootstrap/')
        
    if not os.path.exists(f'{path}/nominal.fit'):
        print(f'No nominal.fit file in {path}')
        return
    
    print(f'nominal fit exists in {path}')
    os.chdir(path)
    
    SDMEs = {}
    # Get SDMEs from converged fit
    with open('nominal.fit', 'r') as f:
        for line in f: # skip lines until we reach the parameters section
            if(line == '+++ Parameter Values and Errors +++\n'):
                break
        line = f.readline() # skip number of parameters

        for line in f:
            arr = line.strip().split('\t')
            SDMEs[arr[0]] = arr[1]
            if arr[0] == 'rho1m12':
                break # rho1m12 is the last SDME, we have what we want from this file
    
    with open(f'{cfgFileName}', 'r') as original:
        with open('bootstrap.cfg', 'w') as bootstrap:
            for line in original:
                if line[0:4] == 'data':
                    line = line.replace('ROOTDataReader', 'ROOTDataReaderBootstrap')
                    line = line.replace('\n', ' XXXX \n')
                elif line[0:9] == 'parameter':
                    arr = line.strip().split(' ')
                    line = f'parameter {arr[1]} {SDMEs[arr[1]]} \n'
                elif line[0:10] == 'initialize':
                    arr = line.strip().split(' ')
                    amp_real = arr[1]+"_re"
                    amp_imag = arr[1]+"_im"
                    line = f'initialize {arr[1]} {arr[2]} {SDMEs[amp_real]} {SDMEs[amp_imag]} real \n'
                bootstrap.write(line)
        
    for iBootstrap in range(nBootstraps):
        if os.path.exists(f'{path}/bootstrap/bootstrap_{iBootstrap}.cfg'):
            continue
        
        cmd = f'cp bootstrap.cfg bootstrap_{iBootstrap}.cfg'
        os.system(cmd)
        
        cmd = f'sed -i s/fit\ {fitName}/fit\ bootstrap_{iBootstrap}/g bootstrap_{iBootstrap}.cfg'
        os.system(cmd)
        
        cmd = f'sed -i s/XXXX/{randint(1, 1000000)}/g bootstrap_{iBootstrap}.cfg'
        os.system(cmd)
        
        cmd = f'mv bootstrap_{iBootstrap}.cfg bootstrap/bootstrap_{iBootstrap}.cfg'
        os.system(cmd)

def runBootstrapFits(path, nBootstraps):
    print(f'Move to directory {path}')
    os.chdir(path)
    
    if not os.path.exists(f'nominal.fit'):
        print(f'No nominal fit in {path}')
        return
    
    print(f'Run {nBootstraps} bootstrap fits in {path}')
    for iBootstrap in range(nBootstraps):
        cmd = f'fit -c bootstrap/bootstrap_{iBootstrap}.cfg > bootstrap_{iBootstrap}.log'
        os.system(cmd)
        
        if iBootstrap%10 == 0:
            print(f'{iBootstrap} of {nBootstraps} bootstraps completed in {path}')
    
    os.system('mv bootstrap*.fit bootstrap/')
    print(f'Bootstap complete in {path}')

def setupTrees(pathToDSelectorTrees):
    print('First step of formating trees for SDME fits.')
    print('     Make sure that path, cuts, etc. are appropriately set in setup_script.C')
    cmd = f'root -b -q -l \'setup_trees.C("{pathToDSelectorTrees}")\''
    print(f'    {cmd}')
    os.system(cmd)

def divideData(fitName, nBins, lowerBoundData, higherBoundData, pathToTrees, divideData_split, divideData_splitFlag, runPeriod):
    print('Run divideData.pl script')
    cmd = f'./divideData.pl {fitName} {nBins} {lowerBoundData} {higherBoundData} {pathToTrees} {runPeriod} {divideData_split} {divideData_splitFlag}'
    print(f'    {cmd}')
    os.system(cmd)
