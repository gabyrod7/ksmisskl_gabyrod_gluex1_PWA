#!/bin/bash

# ### Produce histograms to be analyzed with RDataFrames
root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_dat_gluex1.root", "hists/hist_dat_gluex1.root", 0)'
root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_acc_gluex1.root", "hists/hist_acc_gluex1.root", 0)'
root -l -b -q 'RDF_thrown.C(16, "../DSelector/ftrees/ftree_gen_gluex1.root", "hists/hist_gen_gluex1.root", 0)'

# root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_dat_sp17.root", "hists/hist_dat_sp17.root", 0)'
# root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_acc_sp17.root", "hists/hist_acc_sp17.root", 0)'
# root -l -b -q 'RDF_thrown.C(16, "../DSelector/ftrees/ftree_gen_sp17.root", "hists/hist_gen_sp17.root", 0)'

# root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_dat_sp18.root", "hists/hist_dat_sp18.root", 0)'
# root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_acc_sp18.root", "hists/hist_acc_sp18.root", 0)'
# root -l -b -q 'RDF_thrown.C(16, "../DSelector/ftrees/ftree_gen_sp18.root", "hists/hist_gen_sp18.root", 0)'

# root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_dat_fa18.root", "hists/hist_dat_fa18.root", 0)'
# root -l -b -q 'RDF_ana.C(16, "../DSelector/ftrees/ftree_acc_fa18.root", "hists/hist_acc_fa18.root", 0)'
# root -l -b -q 'RDF_thrown.C(16, "../DSelector/ftrees/ftree_gen_fa18.root", "hists/hist_gen_fa18.root", 0)'
