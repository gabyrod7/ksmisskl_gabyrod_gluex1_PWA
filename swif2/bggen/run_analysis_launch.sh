#!/bin/bash

swif2 create -workflow gabyrod_bggen_2017-01_ver03_batch01
python launch.py jobs_analysis_bggen_batch1.config 30274 31057
swif2 run -workflow gabyrod_bggen_2017-01_ver03_batch01

swif2 create -workflow gabyrod_bggen_2017-01_ver03_batch02
python launch.py jobs_analysis_bggen_batch2.config 30274 31057
swif2 run -workflow gabyrod_bggen_2017-01_ver03_batch02

swif2 create -workflow gabyrod_bggen_2017-01_ver03_batch03
python launch.py jobs_analysis_bggen_batch3.config 30274 31057
swif2 run -workflow gabyrod_bggen_2017-01_ver03_batch03

swif2 create -workflow gabyrod_bggen_2017-01_ver03_batch04
python launch.py jobs_analysis_bggen_batch4.config 30274 31057
swif2 run -workflow gabyrod_bggen_2017-01_ver03_batch04


