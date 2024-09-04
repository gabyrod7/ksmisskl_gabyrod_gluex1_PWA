#!/bin/bash

### Produce histograms to be analyzed with RDataFrames
root -l -b -q 'RDF_ana.C(16, "/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PWA/fit_mkskl_upto_2600MeV/DSelector/ftree_dat_sp17.root", "hists/hist_dat_sp17.root", 0)'
root -l -b -q 'RDF_ana.C(16, "/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PWA/fit_mkskl_upto_2600MeV/DSelector/ftree_acc_sp17.root", "hists/hist_acc_sp17.root", 0)'
root -l -b -q 'RDF_thrown.C(16, "/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PWA/fit_mkskl_upto_2600MeV/DSelector/ftree_gen_sp17.root", "hists/hist_gen_sp17.root", 0)'

root -l -b -q 'RDF_ana.C(16, "/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PWA/fit_mkskl_upto_2600MeV/DSelector/ftree_dat_sp18.root", "hists/hist_dat_sp18.root", 0)'
root -l -b -q 'RDF_ana.C(16, "/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PWA/fit_mkskl_upto_2600MeV/DSelector/ftree_acc_sp18.root", "hists/hist_acc_sp18.root", 0)'
root -l -b -q 'RDF_thrown.C(16, "/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PWA/fit_mkskl_upto_2600MeV/DSelector/ftree_gen_sp18.root", "hists/hist_gen_sp18.root", 0)'

root -l -b -q 'RDF_ana.C(16, "/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PWA/fit_mkskl_upto_2600MeV/DSelector/ftree_dat_fa18.root", "hists/hist_dat_fa18.root", 0)'
root -l -b -q 'RDF_ana.C(16, "/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PWA/fit_mkskl_upto_2600MeV/DSelector/ftree_acc_fa18.root", "hists/hist_acc_fa18.root", 0)'
root -l -b -q 'RDF_thrown.C(16, "/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PWA/fit_mkskl_upto_2600MeV/DSelector/ftree_gen_fa18.root", "hists/hist_gen_fa18.root", 0)'

hadd -f hists/hist_dat.root hists/hist_dat_sp17.root hists/hist_dat_sp18.root hists/hist_dat_fa18.root
hadd -f hists/hist_acc.root hists/hist_acc_sp17.root hists/hist_acc_sp18.root hists/hist_acc_fa18.root
hadd -f hists/hist_gen.root hists/hist_gen_sp17.root hists/hist_gen_sp18.root hists/hist_gen_fa18.root

# root -l -b -q 'RDF_ana.C(16, "..//d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PWA/fit_mkskl_upto_2600MeV/DSelector/ftrees/ftree_dat_gluex1.root", "hists/hist_tmp.root", 0)'
