Here we combine the different polarization fraction vs. beam energy plots.
These plots were made by Mike Dogger and sent out in an email on February 25, 2021.
The makePolValsV9.tar was sent in that email.
These plots are used in the SDME analysis.
Since each run period has differnt amount of statistics we use the luminosity in the coherent peak as the weight factor.
The luminosity for each GlueX-I run period can be found at https://halldweb.jlab.org/wiki-private/index.php/GlueX_Phase-I_Dataset_Summary.
Here is a table of the relevant information:

run | luminosity | fraction of total
Spring 2017 | 21.8 | 0.175
Spring 2018 | 63.0 | 0.504
Fall 2018 | 40.1 | 0.321 

The combine.C script will create a gluex1Tpol.root file which has the weighted polarization fraction vs. beam energy.

