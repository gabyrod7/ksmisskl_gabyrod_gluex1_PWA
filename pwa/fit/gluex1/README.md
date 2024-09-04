# About

The main script in this repo is ```amptoolsManager.py```.
It provides a command-line interface (CLI) for doing and plotting SDME fits and PWA (only two pseudoscalar implemented) using AmpTools.
Inside the ```amptoolsManager.py``` script one can set several variables such as the number of bins the data should be divided into and what kind of fit one wants to do.
A class called ```amptoolsCfg``` is provided in the ```amptoolsCfg.py``` script which provides the functionality to write the config file needed for doing fits that use the standard GlueX implementation of AmpTools.
Two example scripts are provided in this repo that are setup for measuring SDMEs ```example_SDMEfit.py``` and performing a PWA ```example_PWAfit.py```.

A requisite for the scripts in this repo to work is a properly setup GlueX environment, specially ```halld_sim```.
I tried to keep modifications to ```halld_sim``` to a minimum.
There are four changes which are included in the ```halld_sim``` direcotry in this repo.
The changes to ```halld_sim``` are:  (1) ```split_t``` (2) ```getAmpsInBin``` (3) ```phi1020_plotter``` (4) ```kskl_plotter```.
A simple change has made to the ```split_t``` code to save a file called tBins.txt which contains the bin number, bin mean, and bin standard deviation. 
This information is used by the ```drawSDME.C``` script to plot the SDMEs.
```getAmpsInBin``` was written by Lawrence with some minor changes from me and is necessary for plotting the PWA intesities as a function of mass.
The ```phi1020_plotter``` and ```kskl_plotter``` are mainly copy from other plotters.
Remember that if you copy these directories to your ```halld_sim``` (I put them inside ```src/programs/AmplitudeAnalysis/```) then you also need to modify ```SConscript```.

# Notes
* ROOT macros (those ending in .C) are not super general and will often require modifications.
* These are a lot of naming conventions scattered througout the scripts, be careful changing them. The most difficult name schemes to keep general are from the plotter scripts since they are independent of the scripts provided here.
* Change ```setup_trees.C``` to apply the desired event selectinos. This script applies cuts and divides the data by polarization angle. It is meant as a first pass to flat trees from a DSelector.
** The script uses RDataFrames to filter and write new root files. The logic may not be the most general and may require some changes to work with different channels. Make sure to creat a ```pol_angle``` branch with the nominal polarization angles (0, 45, 90, 135).
* ```example_SDMEfit.py``` and ```example_PWAfit.py``` are copies of ```amptoolsManager.py``` for doing SDME and PWA fit. 
* The default was set to  python3 by using the shebang ```#!/bin/python3```.

# Example
The ```amptoolsManager.py``` is the main script but we use the ```example_SDMEfit.py``` script since it is a full fledge example doing an SDME fit.
Make ```example_SDMEfit.py``` executable by running
```
chmod +x example_SDMEfit.py
```
A CLI (command-line interface) is provided by ```example_SDMEfit.py``` check what you can do by running
```
./example_SDMEfit.py -h
```
You will see something like this
```
usage: AmpTools Manager [-h] [-s] [-d] [-S] [-f [FIT [FIT ...]]] [-r] [-D]
                        [-p] [-q] [-b] [-B] [-R]

Run AmpTools fits and plot results. The flags can be provided in any order.
For the initial fit run ./amptoolsManager.py -s -d -S -f -r -D to setup, run
and draw SDMEs.

optional arguments:
  -h, --help            show this help message and exit
  -s, --setupTrees      Take flat trees from DSelector and apply cuts and
                        divide data by polarization angle.
  -d, --divideData      Setup directories and data
  -S, --setupFit        Setup config files for fits
  -f [FIT [FIT ...]], --fit [FIT [FIT ...]]
                        Run fit in (default) all bins of (optional) specific
                        bins
  -r, --readFit         Run readSDMEs()
  -D, --draw            Make plot of SDMEs or PWA mass intensities
  -p, --runPlotter      Run runPlotter()
  -q, --drawAngles      Make plots of angles
  -b, --plotBins        Run plotBins()
  -B, --bootstrap       Run bootstrap
  -R, --rmAmpToolsCfg   Remove amptools.cfg file from all bins
```
If everything is set properly the following line will setup all the data, fit the data and make a plot of the SDMEs.
```
./example_SDMEfit.py -s -d -S -f -r -D
```
If this is your first time using the script then it is unlikely that you will have setup everything correctly.
Therefore, I recommmened you run these flags one at a time and check the output from each step.
Note that these scripts are essentially running commands you would normally run by hand but in a more efficient way.
