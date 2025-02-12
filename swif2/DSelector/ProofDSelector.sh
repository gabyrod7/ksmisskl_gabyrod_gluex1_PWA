#!/bin/bash

root -l -b << EOF
//TTree::SetMaxTreeSize(2000000000000LL); // 2TB
gEnv->SetValue( "Proof.Sandbox", "/w/halld-scshelf2101/home/gabyrod/ksmisskl_gabyrod_gluex1_PWA/swif2/DSelector/");
.x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C
TChain *chain = new TChain("ksmisskl__B3_M16_Tree");
chain->Add("/cache/halld/RunPeriod-2017-01/analysis/ver60/tree_ksmisskl__B3_M16/merged/tree_ksmisskl__B3_M16_030274.root");
DPROOFLiteManager::Process_Chain(chain,"DSelector_kskl.C++", 4, "hist.root", "tree.root"); 
EOF

rm hist.root
rm tree.root
