import os
import random
import numpy as np

class amptools_cfg:
    pols_map = {0 : '000', 45 : '045', 90 : '090', 135 : '135', 'AMO' : 'AMO'}

    def __init__(self, wave_set):
        self.wave_set = wave_set 
        self.fname = 'amptools.cfg'
        self.fit_name = 'fit_name'
        self.reaction_name = 'NAME'
        self.pol_info = []
	self.ext = ''
	self.include_bkg = True

    def set_fit_name(self, name):
        self.fit_name = name

    def set_reaction_name(self, name):
        self.reaction_name = name

    def set_pol_info(self, pol_info):
        self.pol_info = pol_info

    def set_wave_set(self, wave_set):
        self.wave_set = wave_set
        self.fname = wave_set+'.cfg'

    def set_fname(self, fname):
        self.fname = fname

    def set_ext(self, ext):
	self.ext = ext

    def set_include_bkg(self, include_bkg):
	self.include_bkg = include_bkg

    def write_zlm(self, f, sum, amp, l, m, fix):
        for pol_angle, pol_frac in self.pol_info:
            f.write('# Amplitude %s and polarization angle %d \n' % (amp, pol_angle))
            if sum == 'Positive':
                line = 'amplitude %s::%sRe::%s Zlm %d %d +1 +1 %.1f %.3f \n' % (self.reaction_name+self.pols_map[pol_angle], sum, amp, l, m, pol_angle, pol_frac) # real term
                f.write(line)
                line = 'amplitude %s::%sIm::%s Zlm %d %d -1 -1 %.1f %.3f \n' % (self.reaction_name+self.pols_map[pol_angle], sum, amp, l, m, pol_angle, pol_frac) # imeginary term
                f.write(line)
            if sum == 'Negative':
                line = 'amplitude %s::%sRe::%s Zlm %d %d +1 -1 %.1f %.3f \n' % (self.reaction_name+self.pols_map[pol_angle], sum, amp, l, m, pol_angle, pol_frac) # real term
                f.write(line)
                line = 'amplitude %s::%sIm::%s Zlm %d %d -1 +1 %.1f %.3f \n' % (self.reaction_name+self.pols_map[pol_angle], sum, amp, l, m, pol_angle, pol_frac) # imeginary term
                f.write(line)

            if fix:
                num1 = random.uniform(-100, 100)
                line = 'initialize %s::%sRe::%s cartesian %.2f 0.0 real \n' % (self.reaction_name+self.pols_map[pol_angle], sum, amp, num1)
                f.write(line)
            else:
                num1, num2 = (random.uniform(-100, 100) , random.uniform(-100, 100))
                line = 'initialize %s::%sRe::%s cartesian %.2f %.2f \n' % (self.reaction_name+self.pols_map[pol_angle], sum, amp, num1, num2)
                f.write(line)

            #line = 'constrain %s::%sRe::%s %s::%sIm::%s \n' % (self.reaction_name+self.pols_map[self.pol_info[0][0]], sum, amp, self.reaction_name+self.pols_map[self.pol_info[0][0]], sum, amp)
            line = 'constrain %s::%sRe::%s %s::%sIm::%s \n' % (self.reaction_name+self.pols_map[pol_angle], sum, amp, self.reaction_name+self.pols_map[pol_angle], sum, amp)
            f.write(line)

        if len(self.pol_info) > 1:
            f.write('# Constrain parameters across datasets \n')
            for pol_angle, pol_frac in self.pol_info:
                #line = 'constrain %s::%sRe::%s %s::%sRe::%s \n' % (self.reaction_name+self.pols_map[self.pol_info[0][0]], sum, amp, self.reaction_name+self.pols_map[pol_angle], sum, amp)
                line = 'constrain %s::%sRe::%s %s::%sRe::%s \n' % (self.reaction_name+self.pols_map[self.pol_info[0][0]], sum, amp, self.reaction_name+self.pols_map[pol_angle], sum, amp)
                f.write(line)
                #line = 'scale %s::%sRe::%s [par_scale_%s] \n' % (self.reaction_name+self.pols_map[self.pol_info[0][0]], sum, amp, self.pols_map[pol_angle])
                line = 'scale %s::%sRe::%s [par_scale_%s] \n' % (self.reaction_name+self.pols_map[pol_angle], sum, amp, self.pols_map[pol_angle])
                f.write(line)
                line = 'constrain %s::%sIm::%s %s::%sIm::%s \n' % (self.reaction_name+self.pols_map[self.pol_info[0][0]], sum, amp, self.reaction_name+self.pols_map[self.pol_info[0][0]], sum, amp)
                f.write(line)

    def write_amptools_config(self):
        print('Generating config file '+self.fname)
        waves = self.wave_set.split('_')

        wave_dictionary = {'Sp0+' : ['Positive', 'Sp0+', 0, 0, True],
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
                           'Fp3-': ['Negative', 'Fp3-', 3, 3, False] }

        pos_ref_sum = False
        neg_ref_sum = False
        for wave in waves:
            if wave[-1] == '+':
                pos_ref_sum = True
            if wave[-1] == '-':
                neg_ref_sum = True
            if pos_ref_sum and neg_ref_sum:
                break

        with open(self.fname, 'w') as f:
            f.write('fit '+self.fit_name+'\n\n')

            f.write('define flat 0\n')
            f.write('\n')

            for pol_angle, pol_frac in self.pol_info:
                if len(self.pol_info) > 1:
                    if pol_angle == 0:
                        f.write('parameter par_scale_%s 1 fixed \n' % (self.pols_map[pol_angle]))
                    else:
                        f.write('parameter par_scale_%s 1 \n' % (self.pols_map[pol_angle]))
            f.write('\n')

            for pol_angle, pol_frac in self.pol_info:
                f.write('reaction '+self.reaction_name+self.pols_map[pol_angle]+' Beam Proton KShort KLong\n')
            f.write('\n')

            for pol_angle, pol_frac in self.pol_info:
                f.write('normintfile '+self.reaction_name+self.pols_map[pol_angle]+' '+self.fit_name+'_'+self.pols_map[pol_angle]+'.ni\n')
            f.write('\n')

            f.write('# Load data \n')
            for pol_angle, pol_frac in self.pol_info:
                f.write('data '+self.reaction_name+self.pols_map[pol_angle]+' ROOTDataReader '+os.getcwd()+'/dat'+self.pols_map[pol_angle]+self.ext+'.root\n')
                f.write('genmc '+self.reaction_name+self.pols_map[pol_angle]+' ROOTDataReader '+os.getcwd()+'/gen'+self.pols_map[pol_angle]+self.ext+'.root\n')
                f.write('accmc '+self.reaction_name+self.pols_map[pol_angle]+' ROOTDataReader '+os.getcwd()+'/acc'+self.pols_map[pol_angle]+self.ext+'.root\n')
                if self.include_bkg:
			f.write('bkgnd '+self.reaction_name+self.pols_map[pol_angle]+' ROOTDataReader '+os.getcwd()+'/bkg'+self.pols_map[pol_angle]+self.ext+'.root\n')
                f.write('\n')
            f.write('\n')

            f.write('# Create sums \n')
            for pol_angle, pol_frac in self.pol_info:
                if pos_ref_sum:
                    f.write('sum '+self.reaction_name+self.pols_map[pol_angle]+' PositiveRe\n')
                    f.write('sum '+self.reaction_name+self.pols_map[pol_angle]+' PositiveIm\n')
                if neg_ref_sum:
                    f.write('sum '+self.reaction_name+self.pols_map[pol_angle]+' NegativeRe\n')
                    f.write('sum '+self.reaction_name+self.pols_map[pol_angle]+' NegativeIm\n')
                f.write('\n')

            for wave in waves:
                sum, amp, l, m, fix = wave_dictionary[wave]
                self.write_zlm(f, sum, amp, l, m, fix)
                f.write('\n')

if __name__ == '__main__':
    x = amptools_cfg('S0+_S0-')
    x.set_fit_name('fit_name')
    x.set_reaction_name('ksks')
    x.set_pol_info([[0, 0.35], [45, 0.35]])
    x.write_amptools_config()
