import random

class amptoolsCfg:
	def __init__(self):
		self.data = []
		self.waveSet = ''
		self.fname = 'amptools.cfg'
		self.particles = 'particle1 particle2 particle3 particle4'
		self.fitName = 'fitName'
		self.ROOTDataReader = 'ROOTDataReader'
		self.reactionName = 'REACTION'
		self.ext = ''
		self.includeBkgndFile = True
		self.parRange = False
		self.polarization = 'average'
		self.polHistLocation = ''
		self.costum_polInfo = 'polInfo.cfg'
		self.amplitudes = ''
		self.bkgAmplitude = ''
		self.fitDataLocation = ''

	def set_data(self, data):
		self.data = data
	
	def set_particles(self, particles):
		self.particles = particles

	def set_ROOTDataReader(self, reader):
		self.ROOTDataReader = reader

	def set_fit_name(self, name):
		self.fitName = name

	def set_reaction_name(self, name):
		self.reactionName = name

	def set_wave_set(self, waveSet):
		self.waveSet = waveSet

	def set_fname(self, fname):
		self.fname = fname

	def set_ext(self, ext):
		self.ext = ext

	def set_include_bkg(self, includeBkgndFile):
		self.includeBkgndFile = includeBkgndFile

	def set_parRange(self, parRange):
		self.parRange = parRange
  
	def set_polarization(self, polarization):
		self.polarization = polarization
  
	def set_pol_hist_location(self, polHistLocation):
		self.polHistLocation = polHistLocation
  
	def set_costum_polInfo(self, costum_polInfo):
		self.costum_polInfo = costum_polInfo

	def set_amplitudes(self, amplitudes):
		self.amplitudes = amplitudes
  
	def set_bkgAmplitude(self, bkgAmplitude):
		self.bkgAmplitude = bkgAmplitude
  
	def set_fitDataLocation(self, fitDataLocation):
		self.fitDataLocation = fitDataLocation

	# Write the polarization information to the config file for the case of fixed average polarization
	def write_pol_average(self, f):
		polAngle = {'sp17_000' : 1.8, 'sp17_045' : 47.9, 'sp17_090' : 94.5, 'sp17_135' : -41.6, 
	      			'sp18_000' : 4.1, 'sp18_045' : 48.5, 'sp18_090' : 94.2, 'sp18_135' : -42.4, 
					'fa18_000' : 3.3, 'fa18_045' : 48.3, 'fa18_090' : 92.9, 'fa18_135' : -42.1,
					'gluex1_000' : 3.4, 'gluex1_045' : 48.3, 'gluex1_090' : 93.8, 'gluex1_135' : -42.2,
	      			'sp20_000' : 1.4, 'sp20_045' : 47.1, 'sp20_090' : 93.4, 'sp20_135' : -42.2}
		
		polFrac	 = {'sp17_000' : 0.3537, 'sp17_045' : 0.3484, 'sp17_090' : 0.3472, 'sp17_135' : 0.3512, 
	      			'sp18_000' : 0.3420, 'sp18_045' : 0.3474, 'sp18_090' : 0.3478, 'sp18_135' : 0.3517, 
					'fa18_000' : 0.3563, 'fa18_045' : 0.3403, 'fa18_090' : 0.3430, 'fa18_135' : 0.3523,
					'gluex1_000' : 0.3486, 'gluex1_045' : 0.3452, 'gluex1_090' : 0.3461, 'gluex1_135' : 0.3518,
	      			'sp20_000' : 0.3525, 'sp20_045' : 0.3535, 'sp20_090' : 0.3536, 'sp20_135' : 0.3721}

		for dat in self.data:
			# f.write('define pol_{0} {1:.1f} {2:.4f}\n'.format(dat, polAngle[dat], polFrac[dat]))
			f.write(f'define pol_{dat} {polAngle[dat]:.1f} {polFrac[dat]:.4f}\n')
		f.write('\n')

	# Write the polarization information to the config file for the case of polarization histograms
	def write_pol_hist(self, f):
		if self.polHistLocation == '':
			print('Error: No polarization histogram location specified')
			return

		polAngle = {'sp17_000' : 1.8, 'sp17_045' : 47.9, 'sp17_090' : 94.5, 'sp17_135' : -41.6, 
	      			'sp18_000' : 4.1, 'sp18_045' : 48.5, 'sp18_090' : 94.2, 'sp18_135' : -42.4, 
					'fa18_000' : 3.3, 'fa18_045' : 48.3, 'fa18_090' : 92.9, 'fa18_135' : -42.1,
     				'gluex1_000' : 3.3, 'gluex1_045' : 48.3, 'gluex1_090' : 92.9, 'gluex1_135' : -42.1,
	      			'sp20_000' : 4.1, 'sp20_045' : 48.5, 'sp20_090' : 94.2, 'sp20_135' : -42.4}

		polFrac  = {'sp17_000' : self.polHistLocation+'sp17TPol.root hPol0', 'sp17_045' : self.polHistLocation+'sp17TPol.root hPol45', 
	     			'sp17_090' : self.polHistLocation+'sp17TPol.root hPol90', 'sp17_135' : self.polHistLocation+'sp17TPol.root hPol135',

	     			'sp18_000' : self.polHistLocation+'sp18TPol75.root hPol0', 'sp18_045' : self.polHistLocation+'sp18TPol75.root hPol45', 
					'sp18_090' : self.polHistLocation+'sp18TPol75.root hPol90', 'sp18_135' : self.polHistLocation+'sp18TPol75.root hPol135',
					
					'fa18_000' : self.polHistLocation+'fa18TPol.root hPol0', 'fa18_045' : self.polHistLocation+'fa18TPol.root hPol45',
					'fa18_090' : self.polHistLocation+'fa18TPol.root hPol90', 'fa18_135' : self.polHistLocation+'fa18TPol.root hPol135',

	     			'gluex1_000' : self.polHistLocation+'gluex1Tpol.root hPol0', 'gluex1_045' : self.polHistLocation+'gluex1Tpol.root hPol45', 
					'gluex1_090' : self.polHistLocation+'gluex1Tpol.root hPol90', 'gluex1_135' : self.polHistLocation+'gluex1Tpol.root hPol135',
     
	     			'sp20_000' : self.polHistLocation+'sp20TPol75.root hPol0', 'sp20_045' : self.polHistLocation+'sp20TPol75.root hPol45', 
					'sp20_090' : self.polHistLocation+'sp20TPol75.root hPol90', 'sp20_135' : self.polHistLocation+'sp20TPol75.root hPol135'}

		for dat in self.data:
			# f.write('define pol_{0} {1:.1f} {2}\n'.format(dat, polAngle[dat], polFrac[dat]))
			f.write(f'define pol_{dat} {polAngle[dat]} {polFrac[dat]}\n')
		f.write('\n')

	# Write the polarization information to the config file for the case of custom polarization information
	# The information is read from the file polInfo.cfg by default but may be changed by the user
	# The contents in the polInfo.cfg file will be written to the config file as is
	def write_pol_custom(self, f):
		f.write('# Custom polarization information \n')
		with open(self.costum_polInfo, 'r') as polInfo:
			for line in polInfo:
				f.write(line)
		f.write('\n')

	def write_twopsZlm(self, f):
		# The convention for the wave names is as follows:
		# Wave, orbital angular momentum (note so keep all strings the same length we 0+ rather than just 0), reflectivity (p/m = +/-)
		# Example: Sp0+ = S-wave, 0 orbital angular momentum, positive reflectivity
		wave_dictionary = { 
			'Sp0+' : ['Positive', 'Sp0+', 0, 0, True],
			'Sp0-' : ['Negative', 'Sp0-', 0, 0, True],

			'Pm1+': ['Positive', 'Pm1+', 1, -1, False],
			'Pm1-': ['Negative', 'Pm1-', 1, -1, False],
			'Pp0+': ['Positive', 'Pp0+', 1, 0, False],
			'Pp0-': ['Negative', 'Pp0-', 1, 0, False],
			'Pp1+': ['Positive', 'Pp1+', 1, 1, False],
			'Pp1-': ['Negative', 'Pp1-', 1, 1, False],

			'Dm2+': ['Positive', 'Dm2+', 2, -2, False],
			'Dm2-': ['Negative', 'Dm2-', 2, -2, False],
			'Dm1+': ['Positive', 'Dm1+', 2, -1, False],
			'Dm1-': ['Negative', 'Dm1-', 2, -1, False],
			'Dp0+': ['Positive', 'Dp0+', 2, 0, False],
			'Dp0-': ['Negative', 'Dp0-', 2, 0, False],
			'Dp1+': ['Positive', 'Dp1+', 2, 1, False],
			'Dp1-': ['Negative', 'Dp1-', 2, 1, False],
			'Dp2+': ['Positive', 'Dp2+', 2, 2, False],
			'Dp2-': ['Negative', 'Dp2-', 2, 2, False],

			'Fm3+': ['Positive', 'Fm3+', 3, -3, False],
			'Fm3-': ['Negative', 'Fm3-', 3, -3, False],
			'Fm2+': ['Positive', 'Fm2+', 3, -2, False],
			'Fm2-': ['Negative', 'Fm2-', 3, -2, False],
			'Fm1+': ['Positive', 'Fm1+', 3, -1, False],
			'Fm1-': ['Negative', 'Fm1-', 3, -1, False],
			'Fp0+': ['Positive', 'Fp0+', 3, 0, False],
			'Fp0-': ['Negative', 'Fp0-', 3, 0, False],
			'Fp1+': ['Positive', 'Fp1+', 3, 1, False],
			'Fp1-': ['Negative', 'Fp1-', 3, 1, False],
			'Fp2+': ['Positive', 'Fp2+', 3, 2, False],
			'Fp2-': ['Negative', 'Fp2-', 3, 2, False],
			'Fp3+': ['Positive', 'Fp3+', 3, 3, False],
			'Fp3-': ['Negative', 'Fp3-', 3, 3, False] 
		}
                           
		f.write('# Create sums\n')
		for dat in self.data:
			if '+' in  self.waveSet:
				# f.write('sum {0}_{1} PositiveRe\n'.format(self.reactionName, dat))
				# f.write('sum {0}_{1} PositiveIm\n'.format(self.reactionName, dat))
				f.write(f'sum {self.reactionName}_{dat} PositiveRe\n')
				f.write(f'sum {self.reactionName}_{dat} PositiveIm\n')
			if '-' in  self.waveSet:
				# f.write('sum {0}_{1} NegativeRe\n'.format(self.reactionName, dat))
				# f.write('sum {0}_{1} NegativeIm\n'.format(self.reactionName, dat))
				f.write(f'sum {self.reactionName}_{dat} NegativeRe\n')
				f.write(f'sum {self.reactionName}_{dat} NegativeIm\n')
			f.write('\n')

		if len(self.data) > 1:
			for dat in self.data:
				if dat == self.data[0]:
					# f.write('parameter par_scale_{0} 1.0 fixed \n'.format(dat))
					f.write(f'parameter par_scale_{dat} 1.0 fixed \n')
					continue
				# f.write('parameter par_scale_{0} 1.0 \n'.format(dat))
				f.write(f'parameter par_scale_{dat} 1.0 \n')
			f.write('\n')

		waves = self.waveSet.split('_')
		for wave in waves:
			for dat in self.data:
				f.write(f'# Amplitude for wave {wave} and polarization angle {dat} \n')
				if wave[-1] == '+':
					f.write(f'amplitude {self.reactionName}_{dat}::PositiveRe::{wave} Zlm {wave_dictionary[wave][2]} {wave_dictionary[wave][3]} +1 +1 pol_{dat}\n')
					f.write(f'amplitude {self.reactionName}_{dat}::PositiveIm::{wave} Zlm {wave_dictionary[wave][2]} {wave_dictionary[wave][3]} -1 -1 pol_{dat}\n')

					if wave_dictionary[wave][4]:
						f.write(f'initialize {self.reactionName}_{dat}::PositiveRe::{wave} cartesian {random.uniform(0, 100)} 0.0 real\n')
					else:
						f.write(f'initialize {self.reactionName}_{dat}::PositiveIm::{wave} cartesian {random.uniform(-100, 100)} {random.uniform(-100, 100)} \n')

					f.write(f'constrain {self.reactionName}_{dat}::PositiveRe::{wave} {self.reactionName}_{dat}::PositiveIm::{wave}\n')
					
				if wave[-1] == '-':
					f.write(f'amplitude {self.reactionName}_{dat}::NegativeRe::{wave} Zlm {wave_dictionary[wave][2]} {wave_dictionary[wave][3]} +1 -1 pol_{dat}\n')
					f.write(f'amplitude {self.reactionName}_{dat}::NegativeIm::{wave} Zlm {wave_dictionary[wave][2]} {wave_dictionary[wave][3]} -1 +1 pol_{dat}\n')

					if wave_dictionary[wave][4]:
						f.write(f'initialize {self.reactionName}_{dat}::NegativeRe::{wave} cartesian {random.uniform(0, 100)} 0.0 real\n')
					else:
						f.write(f'initialize {self.reactionName}_{dat}::NegativeIm::{wave} cartesian {random.uniform(-100, 100)} {random.uniform(-100, 100)} \n')

					f.write(f'constrain {self.reactionName}_{dat}::NegativeRe::{wave} {self.reactionName}_{dat}::NegativeIm::{wave} \n')
				f.write('\n')

			if len(self.data) > 1:
				f.write('# Constrain parameters across datasets \n')
				for dat in self.data:
					if dat == self.data[0]:
						if wave[-1] == '+':
							f.write(f'scale {self.reactionName}_{dat}::PositiveRe::{wave} [par_scale_{dat}] \n')
						if wave [-1] == '-' :
							f.write(f'scale {self.reactionName}_{dat}::NegativeRe::{wave} [par_scale_{dat}] \n')
						continue
					if wave[-1] == '+':
						f.write(f'constrain {self.reactionName}_{dat}::PositiveRe::{wave} {self.reactionName}_{self.data[0]}::PositiveRe::{wave} \n')
						f.write(f'scale {self.reactionName}_{dat}::PositiveRe::{wave} [par_scale_{dat}] \n')
					if wave[-1] == '-':
						f.write(f'constrain {self.reactionName}_{dat}::NegativeRe::{wave} {self.reactionName}_{self.data[0]}::NegativeRe::{wave} \n')
						f.write(f'scale {self.reactionName}_{dat}::NegativeRe::{wave} [par_scale_{dat}] \n')
			f.write('\n')

	def write_sdme(self, f):
		f.write('# Create sums\n')
		for dat in self.data:
			f.write(f'sum {self.reactionName}_{dat} sum_{dat}\n')
		f.write('\n')

		f.write('parameter rho000  0.01\n')
		f.write('parameter rho100  0.01\n')
		f.write('parameter rho1m10 0.01\n')
		f.write('parameter rho111  0.01\n')
		f.write('parameter rho001  0.01\n')
		f.write('parameter rho101  0.01\n')
		f.write('parameter rho1m11 0.5\n')
		f.write('parameter rho102  -0.01\n')
		f.write('parameter rho1m12 -0.5\n\n')

		if self.parRange:
			f.write('keyword parRange 3 3\n')
			f.write('parRange rho000  -0.01 0.01\n')
			f.write('parRange rho100  -0.01 0.01\n')
			f.write('parRange rho1m10 -0.01 0.01\n')
			f.write('parRange rho111  -0.01 0.01\n')
			f.write('parRange rho001  -0.01 0.01\n')
			f.write('parRange rho101  -0.01 0.01\n')
			f.write('parRange rho1m11 0.45 0.55\n')
			f.write('parRange rho102  -0.01 0.01\n')
			f.write('parRange rho1m12 -0.55 -0.45\n\n')

		for dat in self.data:
			f.write(f'amplitude {self.reactionName}_{dat}::sum_{dat}::SDME TwoPiAngles [rho000] [rho100] [rho1m10] [rho111] [rho001] [rho101] [rho1m11] [rho102] [rho1m12] pol_{dat}\n')
			f.write(f'initialize {self.reactionName}_{dat}::sum_{dat}::SDME cartesian {random.uniform(0, 100):.2f} 0.0 real \n')

	def write_uniformBkg(self, f):
		f.write('\n# Background \n')

		for dat in self.data:
			f.write(f'sum {self.reactionName}_{dat} sum_Background\n')
		f.write('\n')
  
		for dat in self.data:
			f.write(f'amplitude {self.reactionName}_{dat}::sum_Background::Uniform Uniform\n')
			f.write(f'initialize {self.reactionName}_{dat}::sum_Background::Uniform cartesian 1 0 real\n')

	def write_amptools_config(self):
		print(f'Generating config file {self.fname}')
		
		with open(self.fname, 'w') as f:
			f.write('# Writting AmpTools config file with amptools_cfg class\n')
			if self.amplitudes == 'sdme':
				f.write('# Write SDME amplitudes \n')
			f.write(f'fit {self.fitName}\n\n')

			for dat in self.data:
				f.write(f'reaction {self.reactionName}_{dat} {self.particles}\n')
			f.write('\n')

			for dat in self.data:
				f.write(f'normintfile {self.reactionName}_{dat} {self.fitName}_{dat}.ni\n')
			f.write('\n')

			f.write('# Load data\n')
			for dat in self.data:
				f.write(f'data {self.reactionName}_{dat} {self.ROOTDataReader} {self.fitDataLocation}/dat_{dat}{self.ext}\n')
				f.write(f'accmc {self.reactionName}_{dat} {self.ROOTDataReader} {self.fitDataLocation}/acc_{dat}{self.ext}\n')
				f.write(f'genmc {self.reactionName}_{dat} {self.ROOTDataReader} {self.fitDataLocation}/gen_{dat}{self.ext}\n')
				if self.includeBkgndFile:
					f.write(f'bkgnd {self.reactionName}_{dat} {self.ROOTDataReader} {self.fitDataLocation}/bkg_{dat}{self.ext}\n\n')
				else:
					f.write('\n')
			
			f.write('# Define polarization information\n')
			write_pol = {'hist' : self.write_pol_hist, 'average' : self.write_pol_average, 'custom' : self.write_pol_custom}
			write_pol[self.polarization](f)

			write_amplitude = {'sdme' : self.write_sdme, 'twopsZlm' : self.write_twopsZlm}
			write_amplitude[self.amplitudes](f)
   
			write_bkgAmplitude = {'uniform' : self.write_uniformBkg}
			if self.bkgAmplitude != '':
				write_bkgAmplitude[self.bkgAmplitude](f)

if __name__ == '__main__':
    z = [runPeriod+'_'+polAngle for runPeriod in ['sp17', 'sp18', 'fa18'] for polAngle in ['000', '045', '090', '135']]
    x = amptoolsCfg()
    x.set_data(z)
    x.set_particles('Beam Proton KShort KLong')
    x.write_amptools_config()