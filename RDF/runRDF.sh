#!/bin/bash

### Produce histograms to be analyzed with RDataFrames
# root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_dat_sp17.root", "hist_dat_sp17.root", 0)'
# root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_acc_sp17.root", "hist_acc_sp17.root", 0)'

# root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_dat_sp18.root", "hist_dat_sp18.root", 0)'
# root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_acc_sp18.root", "hist_acc_sp18.root", 0)'

# root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_dat_fa18.root", "hist_dat_fa18.root", 0)'
# root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_acc_fa18.root", "hist_acc_fa18.root", 0)'

root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_dat_gluex1.root", "hist_dat.root", 0)'
root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_acc_gluex1.root", "hist_acc.root", 0)'
