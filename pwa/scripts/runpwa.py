import os
from multiprocessing import Pool
import amptools_cfg

nbins = 36
nfits = 100
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
		#cmd = 'kskl_plotter '+fit_name+'.fit -s '+wave+' -a T -F '+wave+' -o hist.root > plot_'+wave+'.log'
		cmd = 'kskl_plotter '+fit_name+'.fit -s '+wave+' -a F -F '+wave+' -o hist.root > plot_'+wave+'.log' # do not acceptance correction
		# kskl_plotter fit_sp17_18bins.fit -s P0+ -a F -F P0+ -o hist.root
		print(cmd)
		os.system(cmd)

	cmd = 'kskl_plotter '+fit_name+'.fit -s '+waves+' -a F -F '+waves+' -o hist.root > plot_'+waves+'.log' # do not acceptance correction
	#cmd = 'kskl_plotter '+fit_name+'.fit -s '+waves+' -a T -F '+waves+' -o hist.root > plot_'+waves+'.log'
	print(cmd)
	os.system(cmd)

if __name__ == '__main__':
	wave_sets = ['Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-']
	wave_sets = ['Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-', 'Pp0+_Pp0-_Pp1+_Pp1-', 'Sp0+_Sp0-_Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-', 'Sp0+_Sp0-_Pp0+_Pp0-_Pp1+_Pp1-']
	#wave_sets = ['Pm1+_Pp0+_Pp0-_Pp1+_Pp1-', 'Pm1-_Pp0+_Pp0-_Pp1+_Pp1-']
	#wave_sets = ['Pp0+_Pp1+_Pp1-']
	#wave_sets = ['Pp0+_Pp1+']
	#wave_sets = ['Sp0+_Sp0-_Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-', 'Sp0+_Sp0-_Pp0+_Pp0-_Pp1+_Pp1-']
	#wave_sets = ['Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-', 'Pp0+_Pp0-_Pp1+_Pp1-', 'Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-_Fm3+']

	#wave_sets = []
	#F_waves = ['Fm3+', 'Fm3-', 'Fm2+', 'Fm2-', 'Fm1+', 'Fm1-', 'Fp0+', 'Fp0-',
	#	   'Fp3+', 'Fp3-', 'Fp2+', 'Fp2-', 'Fp1+', 'Fp1-']
	#for F_wave in F_waves:
	#	wave_sets.append('Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-_'+F_wave)
	#for F_wave in F_waves:
	#	wave_sets.append('Pp0+_Pp0-_Pp1+_Pp1-_Fp3+_'+F_wave)

	paths = pwa_setup(wave_sets, nbins, base_directory)

	p = Pool(nprocess)
	p.map(run_fit, paths)

