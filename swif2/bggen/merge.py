# After doing an "Analysis Launch" for bggen I found many root trees per run number.
# This script merges those trees into a single tree using the hadd function.
# This leaves a single tree per run number which is placed in a location of your choosing.
# Note this script was moved from the original location and should to be updates before running.

import os

for i in range(4,5):
	path = '/volatile/halld/home/gabyrod/bggen/batch'+str(i)+'/tree_ksmisskl__B3_M16/'
	new_dir = 'tree_ksmisskl__B3_M16/merged/'
	path_to = os.getcwd()+new_dir
	
	dir_list = os.listdir(path) # get directory for each run number i.e. 030450, 030893, ...
	
	for directory in dir_list:
		fnames = os.listdir(path+directory)
	
		cmd = 'hadd -f batch'+str(i)+'/'+new_dir+'tree_ksmisskl__B3_M16_'+directory+'.root'
		for fname in fnames:
			cmd += ' batch'+str(i)+'/tree_ksmisskl__B3_M16/'+directory+'/'+fname
		print(cmd)
		os.system(cmd)
