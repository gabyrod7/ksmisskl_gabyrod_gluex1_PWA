#!/usr/bin/env python3
import os
import glob 

files_location = ['/d/grid17/gabyrod7/gluex_data/RunPeriod-2017-01/ver60/tree_ksmisskl__B3_M16/tree*',
                    '/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-01/ver22/tree_ksmisskl__B3_M16/tree*',
                    '/d/grid17/gabyrod7/gluex_data/RunPeriod-2018-08/ver20/tree_ksmisskl__B3_M16/tree*']

for run_period in files_location:
    for fname in glob.glob(run_period):
        tree = fname.split('/')[-1]
        cmd = f'ln -s {fname} gluex1_trees/{tree}'
        print(cmd)
        os.system(cmd)