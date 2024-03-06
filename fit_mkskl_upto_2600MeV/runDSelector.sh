#!/bin/bash

function runDSelector() {

root -l -b << EOF
TTree::SetMaxTreeSize(2000000000000LL); // 2TB
.x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C
TChain *chain = new TChain("$reaction_filter");
chain->Add("$data1");
chain->Add("$data2");
chain->Add("$data3");
DPROOFLiteManager::Process_Chain(chain,"$DSelector++", $nthreads, "hist.root", "tree.root"); 
EOF

mv ftree.root $ftree
rm hist.root
rm tree.root
}

nthreads=32

## reconstructed MC
DSelector="DSelector_kskl.C"
reaction_filter="ksmisskl__B3_M16_Tree"
data1="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp17_9M_3584/trees/tree_ksmisskl__B3_M16_gen_amp/tree*"
data2="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp18_25M_3582/trees/tree_ksmisskl__B3_M16_gen_amp/tree*"
data3="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_fa18_16M_3581/trees/tree_ksmisskl__B3_M16_gen_amp/tree*"
hist="hist_acc_fa18.root"
tree="tree_acc_fa18.root"
ftree="ftree_acc.root"
runDSelector

## thrown MC
DSelector="DSelector_thrown.C"
reaction_filter="Thrown_Tree"
data1="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp17_9M_3584/thrown/tree*"
data2="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp18_25M_3582/thrown/tree*"
data3="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_fa18_16M_3581/thrown/tree*"
hist="hist_gen_fa18.root"
tree="tree_gen_fa18.root"
ftree="ftree_gen.root"
runDSelector

rm -r 0.*
