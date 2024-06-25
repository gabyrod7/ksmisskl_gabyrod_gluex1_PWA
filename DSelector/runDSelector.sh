#!/bin/bash

function runDSelector() {

root -l -b << EOF
TTree::SetMaxTreeSize(2000000000000LL); // 2TB
.x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C
TChain *chain = new TChain("$reaction_filter");
chain->Add("$data");
DPROOFLiteManager::Process_Chain(chain,"$DSelector++", $nthreads, "hist.root", "tree.root"); 
EOF

mv ftree.root ftrees/$ftree
mv hist.root hists/$hist
mv tree.root trees/$tree
}

nthreads=32

## Spring 2017 data
DSelector="DSelector_kskl.C"
reaction_filter="ksmisskl__B3_M16_Tree"
#data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2017-01/ver60/tree_ksmisskl__B3_M16/tree_ksmisskl__B3_M16_03099*"
data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2017-01/ver60/tree_ksmisskl__B3_M16/tree*"
hist="hist_dat_sp17.root"
tree="tree_dat_sp17.root"
ftree="ftree_dat_sp17.root"
# runDSelector

# Spring 2018 data
DSelector="DSelector_kskl.C"
reaction_filter="ksmisskl__B3_M16_Tree"
#data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-01/tree_ksmisskl__B3_M16/reduced/tree_ksmisskl__B3_M16_04255*"
# data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-01/tree_ksmisskl__B3_M16/reduced/t*"
# data="trees/tree_dat_sp18.root"
data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-01/ver22/tree_ksmisskl__B3_M16/tree*"
hist="hist_dat_sp18.root"
tree="tree_dat_sp18.root"
ftree="ftree_dat_sp18.root"
# runDSelector

# Fall 2018 data
DSelector="DSelector_kskl.C"
reaction_filter="ksmisskl__B3_M16_Tree"
# data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-08/tree_ksmisskl__B3_M16/reduced/t*"
data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-08/ver20/tree_ksmisskl__B3_M16/tree*"
#data="tree_dat_fa18.root"
hist="hist_dat_fa18.root"
tree="tree_dat_fa18.root"
ftree="ftree_dat_fa18.root"
# runDSelector

# hadd -f ftrees/ftree_dat_gluex1.root ftrees/ftree_dat_sp17.root ftrees/ftree_dat_sp18.root ftrees/ftree_dat_fa18.root

# GlueX-I data
DSelector="DSelector_kskl.C"
reaction_filter="ksmisskl__B3_M16_Tree"
data="gluex1_trees/tree*"
hist="hist_dat_gluex1.root"
tree="tree_dat_gluex1.root"
ftree="ftree_dat_gluex1.root"
# runDSelector

## Spring 2017 reconstructed flat MC
DSelector="DSelector_kskl.C"
reaction_filter="ksmisskl__B3_M16_Tree"
# data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp17_15M/root/trees/tree*"
data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp17_18M_3480/root/trees/tree*"
hist="hist_acc_sp17.root"
tree="tree_acc_sp17.root"
ftree="ftree_acc_sp17.root"
# runDSelector

## Spring 2017 thrown MC
DSelector="DSelector_thrown.C"
reaction_filter="Thrown_Tree"
# data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp17_15M/root/thrown/tree*"
data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp17_18M_3480/root/thrown/tree*"
hist="hist_gen_sp17.root"
tree="tree_gen_sp17.root"
ftree="ftree_gen_sp17.root"
# runDSelector

## Spring 2018 reconstructed MC
DSelector="DSelector_kskl.C"
reaction_filter="ksmisskl__B3_M16_Tree"
data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp18_50M_3481/root/trees/tree*"
hist="hist_acc_sp18.root"
tree="tree_acc_sp18.root"
ftree="ftree_acc_sp18.root"
# runDSelector

## Spring 2018 thrown MC
DSelector="DSelector_thrown.C"
reaction_filter="Thrown_Tree"
data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp18_50M_3481/root/thrown/tree*"
hist="hist_gen_sp18.root"
tree="tree_gen_sp18.root"
ftree="ftree_gen_sp18.root"
# runDSelector

## Fall 2018 reconstructed MC
DSelector="DSelector_kskl.C"
reaction_filter="ksmisskl__B3_M16_Tree"
data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_fa18_32M_3482/root/trees/tree*"
hist="hist_acc_fa18.root"
tree="tree_acc_fa18.root"
ftree="ftree_acc_fa18.root"
# runDSelector

## Fall 2018 thrown MC
DSelector="DSelector_thrown.C"
reaction_filter="Thrown_Tree"
data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_fa18_32M_3482/root/thrown/tree*"
hist="hist_gen_fa18.root"
tree="tree_gen_fa18.root"
ftree="ftree_gen_fa18.root"
# runDSelector

# hadd -f ftrees/ftree_acc_gluex1.root ftrees/ftree_acc_sp17.root ftrees/ftree_acc_sp18.root ftrees/ftree_acc_fa18.root
# hadd -f ftrees/ftree_gen_gluex1.root ftrees/ftree_gen_sp17.root ftrees/ftree_gen_sp18.root ftrees/ftree_gen_fa18.root

## Spring 2017 reconstructed MC u2600
DSelector="DSelector_kskl.C"
reaction_filter="ksmisskl__B3_M16_Tree"
data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp17_9M_3584/trees/tree_ksmisskl__B3_M16_gen_amp/tree*"
hist="hist_acc_sp17_u2600.root"
tree="tree_acc_sp17_u2600.root"
ftree="ftree_acc_sp17_u2600.root"
# runDSelector

## Spring 2017 thrown MC
DSelector="DSelector_thrown.C"
reaction_filter="Thrown_Tree"
data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp17_9M_3584/thrown/tree*"
hist="hist_gen_sp17_u2600.root"
tree="tree_gen_sp17_u2600.root"
ftree="ftree_gen_sp17_u2600.root"
runDSelector

## Spring 2018 reconstructed MC u2600
DSelector="DSelector_kskl.C"
reaction_filter="ksmisskl__B3_M16_Tree"
data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp18_25M_3582/trees/tree_ksmisskl__B3_M16_gen_amp/tree*"
hist="hist_acc_sp18_u2600.root"
tree="tree_acc_sp18_u2600.root"
ftree="ftree_acc_sp18_u2600.root"
# runDSelector

## Spring 2018 thrown MC
DSelector="DSelector_thrown.C"
reaction_filter="Thrown_Tree"
data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp18_25M_3582/thrown/tree*"
hist="hist_gen_sp18_u2600.root"
tree="tree_gen_sp18_u2600.root"
ftree="ftree_gen_sp18_u2600.root"
runDSelector

## Fall 2018 reconstructed MC u2600
DSelector="DSelector_kskl.C"
reaction_filter="ksmisskl__B3_M16_Tree"
data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_fa18_16M_3581/trees/tree_ksmisskl__B3_M16_gen_amp/tree*"
hist="hist_acc_fa18_u2600.root"
tree="tree_acc_fa18_u2600.root"
ftree="ftree_acc_fa18_u2600.root"
# runDSelector

## Fall 2018 thrown MC
DSelector="DSelector_thrown.C"
reaction_filter="Thrown_Tree"
data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_fa18_16M_3581/thrown/tree*"
hist="hist_gen_fa18_u2600.root"
tree="tree_gen_fa18_u2600.root"
ftree="ftree_gen_fa18_u2600.root"
runDSelector

# hadd -f ftrees/ftree_acc_gluex1_u2600.root ftrees/ftree_acc_sp17_u2600.root ftrees/ftree_acc_sp18_u2600.root ftrees/ftree_acc_fa18_u2600.root
hadd -f ftrees/ftree_gen_gluex1_u2600.root ftrees/ftree_gen_sp17_u2600.root ftrees/ftree_gen_sp18_u2600.root ftrees/ftree_gen_fa18_u2600.root

rm -r 0.*
