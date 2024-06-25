#!/bin/bash

### Produce histograms to be analyzed with RDataFrames
root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_dat_gluex1.root", "hist_dat.root", 0)'
root -l -b -q 'RDF_ana.C(16, "ftree_acc.root", "hist_acc.root", 0)'
root -l -b -q 'RDF_thrown.C(16, "ftree_gen.root", "hist_gen.root", 0)'
