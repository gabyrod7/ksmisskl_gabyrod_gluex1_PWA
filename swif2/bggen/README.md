# Analysis launch
I run an analysis launch over Spring 2017 data reconstructing the reaction  ```1_14__m10_16_14``` wich correspond to photoproduction of $K_SK_Lp$ where the $K_L$ is a missing particle.
The reaction is specified in the ```jana_analysis.config``` file.
General purpose bggen files have been generated for wide use in the collaboration and can be found [here](https://halldweb.jlab.org/wiki/index.php/Simulations#General_2).
I specifically reconstruct the [bggen-2017-01-ver03_31](https://halldweb.jlab.org/wiki/index.php/Bggen-2017-01-ver03_31) files which were divided into 4 batches and generated 500M events.
There are four jobs.config files, one for each batch, the ```run_analysis_launch.sh``` script will: (1) creat a swif2 worklow for each job (2) luanch each jobs (3) run each workflow.
As is implemented this creates a single job per file which causes it to take a long time to "lunch" all jobs. For this reason I recomend using ```screen```, ```nohub``` or something similar to have the script run in the background.

# Running a new analysis launch
If someone want to run their own analysis launch the I recommend looking at the links in the Resources section of this README.
That being said, the necesary files for running an analysis launch are: (1) ```jobs.config``` (2) ```launch.py``` (3) ```script.sh```.
Only the ```jobs.config``` file need to be modified by the user.
The ```jana_analysis.config``` file is necesary to specify a reaction that goes in the ```jobs.config``` file.
One should also be careful to specify the correct xml file, in this case I have a copy in this directory for the appropriate version.
The ```run_analysis_launch.sh``` and ```merge.py``` scripts I wrote for convenience.
The ```run_analysis_launch.sh``` script will simply run all the jobs.
The ```merge.py``` script will merge all the files from the analysis launch into a single file per run number.

# Resources
The GlueX wiki has a [HOWTO](https://halldweb1.jlab.org/wiki/index.php/HOWTO_run_an_analysis_launch_on_bggen_simulations) on running an analysis lunch. 
Running an analysis launch was also dicussed during the [2022 Software Tutorial](https://halldweb.jlab.org/wiki/index.php/GlueX_Tutorial_2022), specifically see the talk by Alex "CUE, GlueX Environment and Batch Farm" which has [slides](https://halldweb.jlab.org/DocDB/0055/005595/001/session2a_slides.pdf) and [video](https://iu.mediaspace.kaltura.com/media/t/1_kxnhfmrb) available.
The slides from this talk were specially helpful for me.
An older tutorial can also be found on [Github](https://github.com/JeffersonLab/hd_utilities/tree/master/AnalysisHowTo/ThrownTopology), there the script ```plotTopology.C``` is very useful for making plots for comparing different topologies like in these [slides](https://halldweb.jlab.org/DocDB/0041/004111/002/ThrownTopology.pdf).

# Example Analysis launch
Create a swif2 workflow
```
ifarm1801.jlab.org> swif2 create -workflow gabyrod_bggen_2017-01_ver03_batch01
workflow_id                            = 42449
workflow_name                          = gabyrod_bggen_2017-01_ver03_batch01
workflow_user                          = gabyrod
```
Launch all jobs (might take a while)
```
ifarm1801.jlab.org> python launch.py jobs_analysis_bggen.config 30274 31057                                        
id                = 12437370                     
name              = gabyrod_bggen_2017-01_ver03_batch01_030274_000
...
```
Run the workflow 
```
ifarm1801.jlab.org> swif2 run -workflow gabyrod_bggen_2017-01_ver03_batch01
```
Check the status of your jobs every once in a while
```
swif2 status -workflow gabyrod_bggen_2017-01_ver03_batch01
```
"Oops, I made a mistake need to cancel and/or delete the jobs". No problem usee
```
swif2 cancel -delete -workflow gabyrod_bggen_2017-01_ver03_batch01
```
Two types of problems that may occur are SLURM_NODE_FAIL and SLURM_CANCELLED retry to run the job with the command
```
swif2 retry-jobs -workflow gabyrod_bggen_2017-01_ver03_batch01 -problems SLURM_NODE_FAIL
swif2 retry-jobs -workflow gabyrod_bggen_2017-01_ver03_batch01 -problems SLURM_CANCELLED
```
More commands can be found in [Alex's slides](https://halldweb.jlab.org/DocDB/0055/005595/001/session2a_slides.pdf)
