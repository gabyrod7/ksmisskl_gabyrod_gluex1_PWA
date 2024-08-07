#!/bin/bash

function runDSelector() {

root -l -b << EOF
TTree::SetMaxTreeSize(2000000000000LL); // 2TB
.x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C
TChain *chain = new TChain("$reaction_filter");
chain->Add("$data");
DPROOFLiteManager::Process_Chain(chain,"$DSelector++", $nthreads, "hist.root", "tree.root"); 
EOF

mv ftree.root $ftree
rm hist.root
rm tree.root
}

nthreads=16
DSelector="DSelector_kskl.C"
reaction_filter=""
MCsp17="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp17_9M_3854/"
MCsp18="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_sp18_25M_3855/"
MCfa18="/d/grid17/gabyrod7/MC/kskl/gen_amp_kskl_flat_fa18_16M_3856/"

TEMP=`getopt -a -o n:d:p:rt:f: --long nthread:DSelector:runPeriod:runDSelector,dataType:reactionFilter: -- "$@"`
eval set -- "$TEMP"

while true; do
    case "$1" in 
        -n | --nthread) nthreads=$2; shift 2;;
        -d | --DSelector) DSelector=$2; shift 2;;
        -p | --runPeriod) runPeriod=$2; shift 2;;
        -t | --dataType) dataType=$2; shift 2;;
        -f | --reactionFilter) reaction_filter=$2; shift 2;;
        -r | --runDSelector)
            echo
            echo "Running DSelector with the following parameters:"
            echo "runPeriod: $runPeriod"
            echo "dataType: $dataType"
            if [ $dataType == "data" ]; then
                DSelector="DSelector_kskl.C"
                if [ $reaction_filter == "" ]; then
                    reaction_filter="ksmisskl__B3_M16_Tree"
                fi

                echo $DSelector
                echo $reaction_filter
                echo
                
                if [ $runPeriod == "sp17" ]; then
                    data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2017-01/ver60/tree_ksmisskl__B3_M16/tree*"
                    hist="hist_dat_sp17.root"
                    tree="tree_dat_sp17.root"
                    ftree="ftree_dat_sp17.root"
                elif [ $runPeriod == "sp18" ]; then
                    data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-01/ver22/tree_ksmisskl__B3_M16/tree*"
                    hist="hist_dat_sp18.root"
                    tree="tree_dat_sp18.root"
                    ftree="ftree_dat_sp18.root"
                elif [ $runPeriod == "fa18" ]; then
                    data="/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-08/ver20/tree_ksmisskl__B3_M16/tree*"
                    hist="hist_dat_fa18.root"
                    tree="tree_dat_fa18.root"
                    ftree="ftree_dat_fa18.root"
                fi
            elif [ $dataType == "acc" ]; then
                DSelector="DSelector_kskl.C"
                if [ $reaction_filter == "" ]; then
                    reaction_filter="ksmisskl__B3_M16_Tree"
                fi

                echo $DSelector
                echo $reaction_filter
                echo

                if [ $runPeriod == "sp17" ]; then
                    data=$MCsp17"trees/tree_ksmisskl__B3_M16_gen_amp/tree*"
                    hist="hist_acc_sp17.root"
                    tree="tree_acc_sp17.root"
                    ftree="ftree_acc_sp17.root"
                elif [ $runPeriod == "sp18" ]; then
                    data=$MCsp18"root/trees/tree*"
                    hist="hist_acc_sp18.root"
                    tree="tree_acc_sp18.root"
                    ftree="ftree_acc_sp18.root"
                elif [ $runPeriod == "fa18" ]; then
                    data=$MCfa18"trees/tree_ksmisskl__B3_M16_gen_amp/tree*"
                    hist="hist_acc_fa18.root"
                    tree="tree_acc_fa18.root"
                    ftree="ftree_acc_fa18.root"
                fi
            elif [ $dataType == "thrown" ]; then
                DSelector="DSelector_thrown.C"
                if [ $reaction_filter == "" ]; then
                    reaction_filter="Thrown_Tree"
                fi

                echo $DSelector
                echo $reaction_filter
                echo

                if [ $runPeriod == "sp17" ]; then
                    data=$MCsp17"root/thrown/tree*"
                    hist="hist_gen_sp17.root"
                    tree="tree_gen_sp17.root"
                    ftree="ftree_gen_sp17.root"
                elif [ $runPeriod == "sp18" ]; then
                    data=$MCsp18"thrown/tree*"
                    hist="hist_gen_sp18.root"
                    tree="tree_gen_sp18.root"
                    ftree="ftree_gen_sp18.root"
                elif [ $runPeriod == "fa18" ]; then
                    data=$MCfa18"thrown/tree*"
                    hist="hist_gen_fa18.root"
                    tree="tree_gen_fa18.root"
                    ftree="ftree_gen_fa18.root"
                fi
            fi
            runDSelector;
            shift;;
        --) shift; break;;
    esac
done

rm -r 0.*
