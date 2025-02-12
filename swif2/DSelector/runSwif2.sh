#!/bin/bash

swif2 create -workflow dselector_2017-01_ver60_batch03
python launch.py jobs_root_analysis_sp17.config 30274 31057
swif2 run -workflow dselector_2017-01_ver60_batch03

#swif2 create -workflow dselector_2018-01_ver22_batch02
#python launch.py jobs_root_analysis_sp18.config 40856 42559
#swif2 run -workflow dselector_2018-01_ver22_batch02
#
#swif2 create -workflow dselector_2018-08_ver20_batch02 
#python launch.py jobs_root_analysis_fa18.config 50685 51768
#swif2 run -workflow dselector_2018-08_ver20_batch02
#
#swif2 create -workflow dselector_2019-11_ver09_batch01 
#python launch.py jobs_root_analysis_sp20.config 71350 73266
#swif2 run -workflow dselector_2019-11_ver09_batch01

