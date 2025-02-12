#!/bin/bash

##### No PROOF #####
root -b -l /cache/halld/RunPeriod-2017-01/analysis/ver60/tree_ksmisskl__B3_M16/merged/tree_ksmisskl__B3_M16_030842.root << EOF
.x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C
//ksmisskl__B3_M16_Tree->Process("DSelector_kskl.C+","",2,1);
ksmisskl__B3_M16_Tree->Process("DSelector_kskl.C++","");
EOF


