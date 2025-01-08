#!/bin/bash

### Produce histograms to be analyzed with RDataFrames
root -l -b -q 'rdf_ana.C(20, "/d/grid15/gabyrod7/analysis/kskl/DSelector/gluex/ftree_M16.root", "hist_sp17.root", 0)'
root -l -b -q 'rdf_ana.C(20, "/d/grid15/gabyrod7/analysis/kskl/DSelector/mc/ftree_acc_phi_t440.root", "hist_phi.root", 0)'
root -l -b -q 'rdf_ana.C(20, "/d/grid15/gabyrod7/analysis/kskl/DSelector/mc/ftree_acc_phi1680.root", "hist_phi1680.root", 0)'
root -l -b -q 'rdf_ana.C(20, "/d/grid15/gabyrod7/analysis/kskl/DSelector/mc/ftree_acc_X1750.root", "hist_X1750.root", 0)'

#root -l compare.C

