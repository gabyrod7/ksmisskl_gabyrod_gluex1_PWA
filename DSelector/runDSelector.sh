#!/bin/bash

function runDSelector() {

root -l -b << EOF
TTree::SetMaxTreeSize(2000000000000LL); // 2TB
.x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C
TChain *chain = new TChain("$reaction_filter");
chain->Add("$data1");
chain->Add("$data2");
chain->Add("$data3");
DPROOFLiteManager::Process_Chain(chain,"$DSelector++", $nthreads, "$hist", "$tree"); 
EOF

mv ftree.root $ftree
mv tree.root $tree
#rm $tree 
}

nthreads=32
data1=""
data2=""
data3=""

### Spring 2017 data
#DSelector="DSelector_kskl.C"
#reaction_filter="ksmisskl__B3_M16_Tree"
##data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2017-01/ver60/tree_ksmisskl__B3_M16/reduced/tree_ksmisskl__B3_M16_03099*"
#data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2017-01/ver60/tree_ksmisskl__B3_M16/reduced/tree*"
##data="tree_dat_sp17.root"
#hist="hist_dat_sp17.root"
#tree="tree_dat_sp17.root"
#ftree="ftree_dat_sp17.root"
#runDSelector
#
## Spring 2018 data
#DSelector="DSelector_kskl.C"
#reaction_filter="ksmisskl__B3_M16_Tree"
##data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-01/tree_ksmisskl__B3_M16/reduced/tree_ksmisskl__B3_M16_04255*"
#data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-01/tree_ksmisskl__B3_M16/reduced/t*"
##data="tree_dat_sp18.root"
#hist="hist_dat_sp18.root"
#tree="tree_dat_sp18.root"
#ftree="ftree_dat_sp18.root"
#runDSelector
#
## Fall 2018 data
#DSelector="DSelector_kskl.C"
#reaction_filter="ksmisskl__B3_M16_Tree"
#data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-08/tree_ksmisskl__B3_M16/reduced/t*"
##data="tree_dat_fa18.root"
#hist="hist_dat_fa18.root"
#tree="tree_dat_fa18.root"
#ftree="ftree_dat_fa18.root"
#runDSelector
#
## GlueX-1 data
DSelector="DSelector_kskl.C"
reaction_filter="ksmisskl__B3_M16_Tree"
data1="/d/grid17/gabyrod7/gluex_data/RunPeriod-2017-01/ver60/tree_ksmisskl__B3_M16/reduced/tree*"
data2="/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-01/tree_ksmisskl__B3_M16/reduced/t*"
data3="/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-08/tree_ksmisskl__B3_M16/reduced/t*"
# data1="tree_dat_gluex1.root"
hist="hist_dat_gluex1.root"
tree="tree_dat_gluex1.root"
ftree="ftree_dat_gluex1.root"
runDSelector

#hadd -f hist_dat_gluex1.root hist_dat_sp17.root hist_dat_sp18.root hist_dat_fa18.root 
#hadd -f ftree_dat_gluex1.root ftree_dat_sp17.root ftree_dat_sp18.root ftree_dat_fa18.root

# # GlueX-1 flat reconstructed MC
# DSelector="DSelector_kskl.C"
# reaction_filter="ksmisskl__B3_M16_Tree"
# data1="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp17_15M/root/trees/t*"
# data2="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_phi_sp18_15M/root/trees/tree_ksmisskl__B3_M16_gen_amp/t*"
# data3="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_phi_fa18_15M/root/trees/tree_ksmisskl__B3_M16_gen_amp/t*"
# hist="hist_acc_gluex1.root"
# tree="tree_acc_gluex1.root"
# ftree="ftree_acc_gluex1.root"
# runDSelector

# # GlueX-1 flat thrown MC
# DSelector="DSelector_thrown.C"
# reaction_filter="Thrown_Tree"
# data1="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp17_15M/root/thrown/t*"
# data2="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_phi_sp18_15M/root/thrown/t*"
# data3="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_phi_fa18_15M/root/thrown/t*"
# hist="hist_gen_gluex1.root"
# tree="tree_gen_gluex1.root"
# ftree="ftree_gen_gluex1.root"
# runDSelector

### Spring 2017 reconstructed flat MC
#DSelector="DSelector_kskl.C"
#reaction_filter="ksmisskl__B3_M16_Tree"
#data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp17_15M/root/trees/t*"
##data2="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_phi_sp17_10M/root/trees/t*"
#hist="hist_acc_sp17.root"
#tree="tree_acc_sp17.root"
#ftree="ftree_acc_sp17.root"
#runDSelector

### Spring 2017 thrown phi MC
#DSelector="DSelector_thrown.C"
#reaction_filter="Thrown_Tree"
#data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_phi_sp17_15M/root/thrown/t*"
##data2="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_phi_sp17_10M/root/thrown/t*"
#hist="hist_gen_sp17.root"
#tree="tree_gen_sp17.root"
#ftree="ftree_gen_sp17.root"
#runDSelector

### Spring 2018 reconstructed phi MC
#DSelector="DSelector_kskl.C"
#reaction_filter="ksmisskl__B3_M16_Tree"
#data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_phi_sp18_15M/root/trees/tree_ksmisskl__B3_M16_gen_amp/t*"
#hist="hist_acc_sp18.root"
#tree="tree_acc_sp18.root"
#ftree="ftree_acc_sp18.root"
#runDSelector
#
### Spring 2018 thrown phi MC
#DSelector="DSelector_thrown.C"
#reaction_filter="Thrown_Tree"
#data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_phi_sp18_15M/root/thrown/t*"
#hist="hist_gen_sp18.root"
#tree="tree_gen_sp18.root"
#ftree="ftree_gen_sp18.root"
##runDSelector
#
### Fall 2018 reconstructed phi MC
#DSelector="DSelector_kskl.C"
#reaction_filter="ksmisskl__B3_M16_Tree"
#data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_phi_fa18_15M/root/trees/tree_ksmisskl__B3_M16_gen_amp/t*"
#hist="hist_acc_fa18.root"
#tree="tree_acc_fa18.root"
#ftree="ftree_acc_fa18.root"
#runDSelector
#
### Fall 2018 thrown phi MC
#DSelector="DSelector_thrown.C"
#reaction_filter="Thrown_Tree"
#data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_phi_fa18_15M/root/thrown/t*"
#hist="hist_gen_fa18.root"
#tree="tree_gen_fa18.root"
#ftree="ftree_gen_fa18.root"
##runDSelector
#
#hadd -f ftree_acc_gluex1.root ftree_acc_sp17.root ftree_acc_sp18.root ftree_acc_fa18.root
##hadd -f ftree_gen_gluex1.root ftree_gen_sp17.root ftree_gen_sp18.root ftree_gen_fa18.root

### Spring 2017 reconstructed MC phi with SCHC model 
#DSelector="DSelector_kskl.C"
#reaction_filter="ksmisskl__B3_M16_Tree"
#data="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_sdme1_sp17_20M/root/trees/tree_ksmisskl__B3_M16_gen_amp/t*"
#hist="hist_acc_sdme1.root"
#tree="tree_acc_sdme1.root"
#ftree="ftree_acc_sdme1.root"
#runDSelector

rm -r 0.*
