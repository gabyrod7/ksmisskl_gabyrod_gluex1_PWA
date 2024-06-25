#!/bin/bash

### Produce histograms to be analyzed with RDataFrames
root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_dat_gluex1.root", "hist_dat_gluex1.root", 0)'
root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_acc_gluex1_u2600.root", "hist_acc_gluex1.root", 0)'
root -l -b -q 'RDF_thrown.C(16, "../DSelector/ftrees/ftree_gen_gluex1_u2600.root", "hist_gen_gluex1.root", 0)'
