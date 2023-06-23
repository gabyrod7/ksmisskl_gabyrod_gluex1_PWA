#!/bin/bash

### Produce histograms to be analyzed with RDataFrames
root -l -b -q 'RDF_ana.C(20, "../DSelector/ftree_dat_gluex1.root", "hist_dat.root", 0)'
root -l -b -q 'RDF_ana.C(20, "../DSelector/ftree_acc_gluex1.root", "hist_acc.root", 0)'
