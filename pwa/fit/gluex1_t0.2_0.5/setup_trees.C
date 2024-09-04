#include "TFile.h"
#include "TROOT.h"
#include "ROOT/RDF/RFilter.hxx"
#include "ROOT/RDataFrame.hxx"

#include <vector>
#include <string>
#include <iostream>

void Split(std::string inf_name, std::string data, std::string dir, int n_threads = 4, std::string run = "", std::string filter = "", bool verbose = false);
void runSplit(std::vector<std::string> inf_names, std::string dir, int n_threads = 4, std::string run = "", std::string cuts = "", std::string gen_cuts = "", bool verbose = false);
std::string set_cuts(std::map<std::string, std::string> cuts_list, std::pair<std::string, std::string> change_cut = {"", ""});

void setup_trees(string pathToDSelectorFlatTrees) {
	std::vector<std::string> inf_names;
	std::string dir = "trees/";
	int n_threads = 16;
	std::string run = "";
	std::string cuts = "";
	std::string gen_cuts = "";
	std::string flatTreesDir = "/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PWA/fit_mkskl_upto_2600MeV/DSelector/";

	// List of cuts
	std::map<std::string, std::string> cuts_list = {
		{"mkskl", "mkskl > 1.1 && mkskl < 2.6"},
		{"mmiss", "missing_mass > 0.3 && missing_mass < 0.7"},
		{"mpipi" , "mpipi > 0.48 && mpipi < 0.52"},
		{"mandel_tp", "mandel_tp > 0.2 && mandel_tp < 0.5"},
		{"fs", "flight_significance > 6"},
		{"chisq", "chisq_ndf < 2"},
		{"ntracks", "num_unused_tracks == 0"},
		{"nshowers", "num_unused_showers <= 2"},
		{"proton_z_vertex", "proton_z_vertex > 52 && proton_z_vertex < 78"},
		{"beam_energy", "beam_energy > 8.2 && beam_energy < 8.8"}
	};

	// Set cuts
	cuts = set_cuts(cuts_list);
	gen_cuts = "mandel_tp > 0.2 && mandel_tp < 0.5";

	// GlueX-I data for SDME
	inf_names = {flatTreesDir+"ftree_dat_gluex1.root", flatTreesDir+"ftree_acc_gluex1.root", flatTreesDir+"ftree_gen_gluex1.root"};
	run = "gluex1";
	runSplit(inf_names, dir, n_threads, run, cuts, gen_cuts);
}

void runSplit(std::vector<std::string> inf_names, std::string dir, int n_threads = 4, std::string run = "", std::string cuts = "", std::string gen_cuts = "", bool verbose = false) {
	if(gSystem->AccessPathName(dir.c_str())) {
		std::cout << "Making directory called "+dir << std::endl;
		gSystem->Exec( ("mkdir "+dir).c_str() );
	}

	Split(inf_names[0], "dat", dir, n_threads, run, cuts);
	Split(inf_names[1], "acc", dir, n_threads, run, cuts);
	Split(inf_names[2], "gen", dir, n_threads, run, gen_cuts);
}

void Split(std::string inf_name, std::string data, std::string dir, int n_threads = 4, std::string run = "", std::string filter = "", bool verbose = false) {
	// Parallelize with n threads
	if(n_threads > 0.0)	ROOT::EnableImplicitMT(n_threads);

	// Branches you want to save get
	std::vector<std::string> branches = {"Weight", "pol_angle", "E_Beam", "Px_Beam", "Py_Beam", "Pz_Beam", 
				"NumFinalState", "E_FinalState", "Px_FinalState", "Py_FinalState", "Pz_FinalState", "mandel_t"};

	// make data frame
	// format : tree name, file name, branches to open
	auto df = ROOT::RDataFrame("kskl", inf_name.c_str()).Filter(filter);

	std::cout << std::endl;
	std::cout << "We will now separate the file "+inf_name+" base on polarization angle" << std::endl;
	std::cout << "Number of entries to proccess " << *df.Count() << std::endl;

	if(verbose) {
		std::cout << n_threads << " threads used" << std::endl;
		std::cout << "Branches to save" << std::endl;
		for(auto branch : branches)	std::cout << "- " << branch << std::endl;
	}
	
	// make new data frame with cuts
	// auto dat000 = df.Filter( "amptools_dat == 1 && pol_angle == 0" );
	// auto dat045 = df.Filter( "amptools_dat == 1 && pol_angle == 45" );
	// auto dat090 = df.Filter( "amptools_dat == 1 && pol_angle == 90" );
	// auto dat135 = df.Filter( "amptools_dat == 1 && pol_angle == 135" );
	auto dat000 = df.Filter( "in_time && pol_angle == 0" );
	auto dat045 = df.Filter( "in_time && pol_angle == 45" );
	auto dat090 = df.Filter( "in_time && pol_angle == 90" );
	auto dat135 = df.Filter( "in_time && pol_angle == 135" );

	std::cout << std::endl << "Filters have been defines next we take a Snapshot" << std::endl;

	// save falt tree with specific branches
	dat000.Snapshot("kin", (dir+data+"_"+run+"_000.root").c_str(), branches);
	dat045.Snapshot("kin", (dir+data+"_"+run+"_045.root").c_str(), branches);
	dat090.Snapshot("kin", (dir+data+"_"+run+"_090.root").c_str(), branches);
	dat135.Snapshot("kin", (dir+data+"_"+run+"_135.root").c_str(), branches);

	std::cout << std::endl << "Snapshots have completed" << std::endl;
	std::cout << "Number of entries saved " << *dat000.Count() + *dat045.Count() + *dat090.Count() + *dat135.Count() << std::endl;

	if(data == "dat") {
		std::cout << std::endl << "Proccesing background files" << std::endl;

		// auto bkg000 = df.Filter("pol_angle == 0 && amptools_bkg == 1");
		// auto bkg045 = df.Filter("pol_angle == 45 && amptools_bkg == 1");
		// auto bkg090 = df.Filter("pol_angle == 90 && amptools_bkg == 1");
		// auto bkg135 = df.Filter("pol_angle == 135 && amptools_bkg == 1");
		auto bkg000 = df.Filter("pol_angle == 0 && !in_time");
		auto bkg045 = df.Filter("pol_angle == 45 && !in_time");
		auto bkg090 = df.Filter("pol_angle == 90 && !in_time");
		auto bkg135 = df.Filter("pol_angle == 135 && !in_time");

		bkg000.Snapshot("kin", (dir+"bkg_"+run+"_000.root").c_str(), branches);
		bkg045.Snapshot("kin", (dir+"bkg_"+run+"_045.root").c_str(), branches);
		bkg090.Snapshot("kin", (dir+"bkg_"+run+"_090.root").c_str(), branches);
		bkg135.Snapshot("kin", (dir+"bkg_"+run+"_135.root").c_str(), branches);

		std::cout << std::endl << "Snapshot of background files has been taken" << std::endl;
	}
}

std::string set_cuts(std::map<std::string, std::string> cuts_list, std::pair<std::string, std::string> change_cut = {"", ""}) {
	std::string cuts = "";

	for(auto it = cuts_list.begin(); it != cuts_list.end(); ++it) {
		if(it->first == change_cut.first)	cuts += change_cut.second + " && ";
		else								cuts += it->second + " && ";
	}
	cuts.erase(cuts.size()-4, 4); // remove last " && "

	return cuts;
}
