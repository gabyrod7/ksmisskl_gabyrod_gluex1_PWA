#!/usr/bin/perl

use Cwd;

$lowMass = 1.10; #is a shared lower cutoff for all 3 datas.
$highMass = 2.00; #2 is the upper cutoff of the thrown data and 3ish is the upper cutoff for the reco/data
#$highMass = 2.3; #2 is the upper cutoff of the thrown data and 3ish is the upper cutoff for the reco/data
#$nBins = 65; # not sure why old me chose 65 bins... that is alot of bins and we might not have enough statistics
$nBins=36; # 26 because it is kind of small and (2-0.7)/26 = 0.05 which is nice and round

$fitName = "fit_sp17_36bins";

# put a limit on the number of data events to process
# gen MC and acc MC smaples are not limited
$maxEvts = 1E9;

$workingDir=getcwd();
print "\n\ncurrent working dir: $workingDir";
print "\n===================================\n";

# these files must exist in the workin directory.  If you don't know how
# to generate them or don't have them, see the documentation in gen_3pi
# the Simulation area of the repository

#$baseDir="/d/grid15/ln16/pi0eta/092419/amptools/analysis/amptools_sb/";
$baseDir="/d/grid15/gabyrod7/pwa/kskl/trees/gluex1/";
#$baseDir="/d/grid15/gabyrod7/analysis/kskl/DSelector/for_pwa/sp17_old2/";
#$baseDir=$workingDir;

#$dataFile = "amptools_a0a2a2pi1_onlySig.root";
#$dataFile = "amptools_a0a2a2pi1_onlySB.root";
#$accMCFile = "amptools_flat.root";
#$genMCFile = "amptools_flat_gen.root";
#print "ACCFILE\n";
#print $accMCFile;
#print "\n------------------\n";
#print "GENFILE:\n";
#print $genMCFile;
#print "\n------------------\n";

#$baseGenFileName="amptools_flat_gen_2_";
#$baseAccFileName="amptools_flat_2017_tLT05_sig_";
#$baseBkgFileName="a0a2_posRefD0D2_posRefD1_posM_sb_";
#$baseDatFileName="a0a2_posRefD0D2_posRefD1_posM_tot_";
#
$baseAccFileName="acc";
$baseGenFileName="gen";
$baseBkgFileName="bkg";
$baseDatFileName="dat";

@polTags=qw(000 045 090 135);
#@polTags=qw(000);
print "DATAFILES:\n";
foreach $polTag (@polTags){
    print "$baseDir$baseDatFileName$polTag\.root\n";
}
print "------------------\n";

print "BKGNDFILES:\n";
foreach $polTag (@polTags){
    print "$baseDir$baseBkgFileName$polTag.root\n";
}
print "------------------\n";

print "ACCFILES:\n";
foreach $polTag (@polTags){
    print "$baseDir$baseAccFileName$polTag.root\n";
}
print "------------------\n";

print "GENFILES:\n";
foreach $polTag (@polTags){
    print "$baseDir$baseGenFileName$polTag.root\n";
}
print "------------------\n";


# this file sould be used for partially polarized or unpolarized beam fits

$cfgTempl = "$workingDir/zlm.cfg";
#$cfgTempl = "$workingDir/zlm_etapi_bothReflect_bothM_loop.cfg";
#$cfgTempl = "$workingDir/zlm_etapi_bothReflect.cfg";

### things below here probably don't need to be modified

# this is where the goodies for the fit will end up
$fitDir = "$workingDir/$fitName/";
print "Output fitDir: $fitDir";
print "\n";
mkdir $fitDir unless -d $fitDir;
#`./ramdisk.sh`;

chdir $fitDir;

print "Changing into $fitDir\n";

# use the split_mass command line tool to divide up the
foreach $polTag (@polTags){
    $fileTag="$baseDatFileName$polTag";
    $dataFile="$fileTag.root";
    print "splitting datatag: $dataFile\n";
    system( "split_mass $baseDir$dataFile $fileTag $lowMass $highMass $nBins $maxEvts -T kin:kin" );

    $fileTag="$baseBkgFileName$polTag";
    $dataFile="$fileTag.root";
    print "splitting bkgtag: $dataFile\n";
    system( "split_mass $baseDir$dataFile $fileTag $lowMass $highMass $nBins $maxEvts -T kin:kin" );

    $fileTag="$baseAccFileName$polTag";
    $dataFile="$fileTag.root";
    print "splitting acctag: $dataFile\n";
    system( "split_mass $baseDir$dataFile $fileTag $lowMass $highMass $nBins $maxEvts -T kin:kin" );

    $fileTag="$baseGenFileName$polTag";
    #$fileTag="amptools_flat_gen_000";
    $dataFile="$fileTag.root";
    print "splitting gentag: $dataFile\n";
    system( "split_mass $baseDir$dataFile $fileTag $lowMass $highMass $nBins $maxEvts -T kin:kin" );
}

# make directories to perform the fits in
for( $i = 0; $i < $nBins; ++$i ){

  mkdir "bin_$i" unless -d "bin_$i";
  
  system( "mv *\_$i.root bin_$i" );

  chdir "bin_$i";

#we are essentially copying fit_etapi_moments.cfg and substituting some variables. CFGOUT is going to be a config file in all of our bins. CFGIN is fit_etapi_moments.cfg. Note how fit_etapi_moments.cfg has these place holders defined (DATAFILE,ACCMCFILE,GENMCFILE ... ). They will get replaced here to fit the bin directory. 
  open( CFGOUT, ">bin_$i-full.cfg" );
  open( CFGIN, $cfgTempl ); 

  while( <CFGIN> ){
    foreach $polTag (@polTags){
        s/DATAFILE_$polTag/$baseDatFileName$polTag\_$i.root/;
        s/BKGNDFILE_$polTag/$baseBkgFileName$polTag\_$i.root/;
        s/ACCMCFILE_$polTag/$baseAccFileName$polTag\_$i.root/;
        s/GENMCFILE_$polTag/$baseGenFileName$polTag\_$i.root/;
        s/NIFILE_$polTag/bin_$i\_$polTag.ni/;
    }

    s/FITNAME/bin_$i/;

    print CFGOUT $_;
  }

  close CFGOUT;
  close CFGIN;
  
  system( "touch param_init.cfg" );

  chdir $fitDir;
}

