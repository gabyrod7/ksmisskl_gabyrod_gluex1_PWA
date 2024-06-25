#!/d/home/gabyrod7/python/python-3.6.8/bin/python3

import os
from multiprocessing import Pool
import amptools_cfg

nbins = 36
nfits = 50
nprocess = 36
fit_name = 'fit_sp17_36bins'
reaction_name = 'NAME_'
seed_file = 'param_init.cfg'
keep_log = True

base_directory = os.getcwd()

def pwa_setup(wave_sets, nbins, base_directory):
	paths = []
	for wave_set in wave_sets:
		cfg_file = amptools_cfg.amptools_cfg(wave_set)
		cfg_file.set_fit_name(fit_name)
		cfg_file.set_reaction_name(reaction_name)
		cfg_file.set_pol_info([[0, 0.35], [45, 0.35], [90, 0.35], [135, 0.35]])

		for i in range(nbins):
			path = base_directory+'/'+fit_name+'/bin_'+str(i)

			if os.path.exists(path+'/'+wave_set):
				print(path+'/'+wave_set+' already exists')
				paths.append(path+'/'+wave_set+'/')
				continue

			os.chdir(path)
			if not os.path.exists(path+'/'+wave_set):
				os.mkdir(wave_set)
				cfg_file.set_ext('_'+str(i))
				cfg_file.write_amptools_config()

				cmd = 'mv amptools.cfg '+wave_set+'/'
				os.system(cmd)

			paths.append(path+'/'+wave_set+'/')

	return paths

def run_fit(path):
	seed_file = 'param_init.cfg'

	print('Move to directory '+path)
	os.chdir(path)

	cmd = 'fit -c amptools.cfg -s %s -r %d > fit.log' % (seed_file, nfits)
	print(cmd)
	os.system(cmd)

	waves = (path.split('/')[-2])
	for wave in waves.split('_'):
		cmd = 'kskl_plotter '+fit_name+'.fit -s '+wave+' -a F -F '+wave+' -o hist.root > plot_'+wave+'.log' # do not acceptance correction
		print(cmd)
		os.system(cmd)

	cmd = 'kskl_plotter '+fit_name+'.fit -s '+waves+' -a F -F '+waves+' -o hist.root > plot_'+waves+'.log' # do not acceptance correction
	print(cmd)
	os.system(cmd)
 
	cmd = 'getAmpsInBin amptools.cfg fit_sp17_36bins.fit 000_045_090_135 true '+waves
	print(cmd)
	os.system(cmd)

if __name__ == '__main__':
	wave_sets = ['Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-']
	# wave_sets = ['Sp0+_Sp0-_Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-']

	# F_waves = ['Fm3+', 'Fm3-', 'Fm2+', 'Fm2-', 'Fm1+', 'Fm1-', 'Fp0+', 'Fp0-',
	# 	   'Fp3+', 'Fp3-', 'Fp2+', 'Fp2-', 'Fp1+', 'Fp1-']
	# for F_wave in F_waves:
	# 	wave_sets.append('Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-_'+F_wave)
	#for F_wave in F_waves:
	#	wave_sets.append('Pp0+_Pp0-_Pp1+_Pp1-_Fp3+_'+F_wave)

	F_waves = ['Fm3+', 'Fm3-', 'Fm2+', 'Fm2-', 'Fm1+', 'Fp0+', 'Fp0-',
		   'Fp3+', 'Fp3-', 'Fp2+', 'Fp2-', 'Fp1+', 'Fp1-']
	for F_wave in F_waves:
		wave_sets.append('Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-_Fm1-_'+F_wave)

	wave_sets = ['Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-_Fm2+_Fm2-_Fm1+_Fm1-_Fp1+_Fp1-_Fp2+_Fp2-']

	paths = pwa_setup(wave_sets, nbins, base_directory)

	p = Pool(nprocess)
	p.map(run_fit, paths)

