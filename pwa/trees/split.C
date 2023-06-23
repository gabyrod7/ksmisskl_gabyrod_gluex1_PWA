#include "TFile.h"
#include "TROOT.h"
#include "ROOT/RDF/RFilter.hxx"
#include "ROOT/RDataFrame.hxx"

#include <vector>
#include <string>
#include <iostream>

void Split(std::string inf_name, std::string data, std::string run, int n_threads = 4, std::string ext = "", std::string filter = "", bool verbose = false);
void runSplit(std::vector<std::string> inf_names, std::string dir, int n_threads = 4, std::string ext = "", std::string cuts = "", std::string gen_cuts = "", bool verbose = false);

void split() {
	std::vector<std::string> inf_names;
	std::string dir;
	int n_threads = 20;
	std::string ext = "";
	std::string cuts;
	std::string gen_cuts;

	// GlueX-I data for PWA
	inf_names = {"ftree_dat_gluex1.root", "ftree_acc_gluex1.root", "ftree_gen_gluex1.root"};	
	dir = "gluex1";
	cuts = " missing_mass > 0.4 && missing_mass < 0.6 && flight_significance > 6 && chisq_ndf < 2 && num_unused_tracks == 0 && num_unused_showers < 3 && mandel_t > 0.2 && mandel_t < 0.5 && mkskl > 1.10";
	gen_cuts = " mandel_t > 0.2 && mandel_t < 0.5 && mkskl > 1.10";
	runSplit(inf_names, dir, n_threads, ext, cuts, gen_cuts);
}

void runSplit(std::vector<std::string> inf_names, std::string dir, int n_threads = 4, std::string ext = "", std::string cuts = "", std::string gen_cuts = "", bool verbose = false) {
	if(gSystem->AccessPathName(dir.c_str())) {
		std::cout << "Making directory called "+dir << std::endl;
		gSystem->Exec( ("mkdir "+dir+"/").c_str() );
	}

	Split(inf_names[0], "dat", dir, n_threads, ext, cuts);
	Split(inf_names[1], "acc", dir, n_threads, ext, cuts);
	Split(inf_names[2], "gen", dir, n_threads, ext, gen_cuts);
}

void Split(std::string inf_name, std::string data, std::string run, int n_threads = 4, std::string ext = "", std::string filter = "", bool verbose = false) {
	// Parallelize with n threads
	if(n_threads > 0.0)	ROOT::EnableImplicitMT(n_threads);

	// Branches you want to use get
	std::vector<std::string> branches = {"Weight", "pol_angle", "E_Beam", "Px_Beam", "Py_Beam", "Pz_Beam", 
				"NumFinalState", "E_FinalState", "Px_FinalState", "Py_FinalState", "Pz_FinalState", "mandel_t"};

	// Make data frame
	auto df = ROOT::RDataFrame("kskl", inf_name.c_str()).Filter(filter.c_str());

	std::cout << std::endl;
	std::cout << "We will now separate the file "+inf_name+" base on polarization angle" << std::endl;
	std::cout << "Number of entries to proccess " << *df.Count() << std::endl;

	if(verbose) {
		std::cout << n_threads << " threads used" << std::endl;
		std::cout << "Branches to save" << std::endl;
		for(auto branch : branches)	std::cout << "- " << branch << std::endl;
	}
	
	// make new data frame with cuts
	auto dat000 = df.Filter("amptools_dat == 1 && pol_angle == 0");
	auto dat045 = df.Filter("amptools_dat == 1 && pol_angle == 45");
	auto dat090 = df.Filter("amptools_dat == 1 && pol_angle == 90");
	auto dat135 = df.Filter("amptools_dat == 1 && pol_angle == 135");

	std::cout << std::endl << "Filters have been defines next we take a Snapshot" << std::endl;

	// save falt tree with specific branches
	dat000.Snapshot("kin", run+ext+"/"+data+"000.root", branches);
	dat045.Snapshot("kin", run+ext+"/"+data+"045.root", branches);
	dat090.Snapshot("kin", run+ext+"/"+data+"090.root", branches);
	dat135.Snapshot("kin", run+ext+"/"+data+"135.root", branches);

	std::cout << std::endl << "Snapshots have completed" << std::endl;

	if(data == "dat") {
		std::cout << std::endl << "Proccesing background files" << std::endl;

		// auto bkg000 = df.Filter("Weight != 1 && pol_angle == 0");
		// auto bkg045 = df.Filter("Weight != 1 && pol_angle == 45");
		// auto bkg090 = df.Filter("Weight != 1 && pol_angle == 90");
		// auto bkg135 = df.Filter("Weight != 1 && pol_angle == 135");
		auto bkg000 = df.Filter("pol_angle == 0 && amptools_bkg == 1");
		auto bkg045 = df.Filter("pol_angle == 45 && amptools_bkg == 1");
		auto bkg090 = df.Filter("pol_angle == 90 && amptools_bkg == 1");
		auto bkg135 = df.Filter("pol_angle == 135 && amptools_bkg == 1");

		bkg000.Snapshot("kin", run+ext+"/bkg000.root", branches);
		bkg045.Snapshot("kin", run+ext+"/bkg045.root", branches);
		bkg090.Snapshot("kin", run+ext+"/bkg090.root", branches);
		bkg135.Snapshot("kin", run+ext+"/bkg135.root", branches);

		std::cout << std::endl << "Snapshot of background files has been taken" << std::endl;
	}
}
