#!/bin/bash

##### No PROOF #####
root -l /d/grid17/gabyrod7/gluex_data/RunPeriod-2017-01/ver60/tree_ksmisskl__B3_M16/tree_ksmisskl__B3_M16_031031.root << EOF
.x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C
//ksmisskl__B3_M16_Tree->Process("DSelector_kskl.C+","",2,1);
ksmisskl__B3_M16_Tree->Process("DSelector_kskl.C+","");
EOF

