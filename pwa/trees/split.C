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

void split() {
	std::vector<std::string> inf_names;
	std::string dir = "";
	int n_threads = 16;
	std::string run = "";
	std::string cuts = "";
	std::string gen_cuts = "";

	std::map<std::string, std::string> cuts_list = {
		{"mkskl", "mkskl > 1.1 && mkskl < 2.00"},
		{"mmiss", "missing_mass > 0.3 && missing_mass < 0.7"},
		{"mandel_t", "mandel_t > 0.2 && mandel_t < 0.5"},
		{"fs", "flight_significance > 6"},
		{"chisq", "chisq_ndf < 2"},
		{"ntracks", "num_unused_tracks == 0"},
		{"nshowers", "num_unused_showers < 3"},
		{"proton_z_vertex", "proton_z_vertex > 52 && proton_z_vertex < 78"},
		{"beam_energy", "beam_energy > 8.2 && beam_energy < 8.8"}};

	// Set nominal cuts
	cuts = set_cuts(cuts_list);
	gen_cuts = "mkskl > 1.1 && mkskl < 2.00 && mandel_t > 0.2 && mandel_t < 0.5";

	// // Spring 2017 data for SDME
	inf_names = {"ftree_dat_sp17.root", "ftree_acc_sp17.root", "ftree_gen_sp17.root"};
	dir = "sp17";
	runSplit(inf_names, dir, n_threads, run, cuts, gen_cuts);
	// Spring 2018 data for SDME
	inf_names = {"ftree_dat_sp18.root", "ftree_acc_sp18.root", "ftree_gen_sp18.root"};	
	dir = "sp18";
	runSplit(inf_names, dir, n_threads, run, cuts, gen_cuts);
	// // Fall 2018 data for SDME
	inf_names = {"ftree_dat_fa18.root", "ftree_acc_fa18.root", "ftree_gen_fa18.root"};	
	dir = "fa18";
	runSplit(inf_names, dir, n_threads, run, cuts, gen_cuts);
	// GlueX-I data for SDME
	inf_names = {"ftree_dat_gluex1.root", "ftree_acc_gluex1.root", "ftree_gen_gluex1.root"};	
	dir = "gluex1";
	// runSplit(inf_names, dir, n_threads, run, cuts, gen_cuts);
}

void runSplit(std::vector<std::string> inf_names, std::string dir, int n_threads = 4, std::string run = "", std::string cuts = "", std::string gen_cuts = "", bool verbose = false) {
	if(gSystem->AccessPathName(dir.c_str())) {
		std::cout << "Making directory called "+dir << std::endl;
		gSystem->Exec( ("mkdir "+dir+"/").c_str() );
	}

	if((dir == "sp17" || dir == "sp18" || dir == "fa18" || dir == "gluex1") && run == "")	run = dir;

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

	// if(data == "dat") {
	// 	TFile *out_file = new TFile("sp17/tmp.root","RECREATE");

	// 	auto im_kskl = df.Filter("amptools_dat").Histo1D({"im_kskl", ";M(K_{S}K_{L});Counts", 50 , 1.00, 2.00}, "mkskl", "Weight");
	// 	auto im_kskl_sb = df.Filter("amptools_bkg").Histo1D({"im_kskl_sb", ";M(K_{S}K_{L});Counts", 50 , 1.00, 2.00}, "mkskl", "Weight");
	// 	auto im_kskl2_sb = df.Filter("!in_time").Histo1D({"im_kskl2_sb", ";M(K_{S}K_{L});Counts", 50 , 1.00, 2.00}, "mkskl", "Weight");
	// 	auto im_kskl3_sb = df.Filter("((mpipi > 0.43 && mpipi < 0.45) || (mpipi > 0.55 && mpipi < 0.57))").Histo1D({"im_kskl3_sb", ";M(K_{S}K_{L});Counts", 50 , 1.00, 2.00}, "mkskl", "Weight");
	// 	auto im_pipi2 = df.Histo1D({"im_pipi2", ";M(#pi#pi);Counts", 200, 0.40, 0.60}, "mpipi", "Weight");
	// 	auto im_pipi = df.Filter("amptools_dat").Histo1D({"im_pipi", ";M(#pi#pi);Counts", 200, 0.40, 0.60}, "mpipi", "Weight");
	// 	auto im_pipi_sb = df.Filter("amptools_bkg").Histo1D({"im_pipi_sb", ";M(#pi#pi);Counts", 200, 0.40, 0.60}, "mpipi", "Weight");
	// 	auto im_pipi2_sb = df.Filter("amptools_bkg && !in_time").Histo1D({"im_pipi2_sb", ";M(#pi#pi);Counts", 200, 0.40, 0.60}, "mpipi", "Weight");
	// 	auto im_pipi3_sb = df.Filter("amptools_bkg && in_time").Histo1D({"im_pipi3_sb", ";M(#pi#pi);Counts", 200, 0.40, 0.60}, "mpipi", "Weight");
	// 	auto im_pipi3 = df.Filter("in_time").Histo1D({"im_pipi3", ";M(#pi#pi);Counts", 200, 0.40, 0.60}, "mpipi", "Weight");
	// 	auto im_pipi4 = df.Filter("!in_time").Histo1D({"im_pipi4", ";M(#pi#pi);Counts", 200, 0.40, 0.60}, "mpipi", "Weight");

	// 	auto h1_mandelt = df.Histo1D({"h1_mandelt", ";-t;Counts", 100, 0.0, 1.00}, "mandel_t", "Weight");
	// 	auto h1_mmiss = df.Histo1D({"h1_mmiss", ";M_{miss};Counts", 100, 0.0, 1.00}, "missing_mass", "Weight");

	// 	im_kskl->Write();
	// 	im_kskl_sb->Write();
	// 	im_kskl2_sb->Write();
	// 	im_kskl3_sb->Write();

	// 	im_pipi->Write();
	// 	im_pipi_sb->Write();
	// 	im_pipi2->Write();
	// 	im_pipi2_sb->Write();
	// 	im_pipi3_sb->Write();
	// 	im_pipi3->Write();
	// 	im_pipi4->Write();

	// 	h1_mandelt->Write();
	// 	h1_mmiss->Write();
	// }

	std::cout << std::endl;
	std::cout << "We will now separate the file "+inf_name+" base on polarization angle" << std::endl;
	std::cout << "Number of entries to proccess " << *df.Count() << std::endl;

	if(verbose) {
		std::cout << n_threads << " threads used" << std::endl;
		std::cout << "Branches to save" << std::endl;
		for(auto branch : branches)	std::cout << "- " << branch << std::endl;
	}
	
	// make new data frame with cuts
	auto dat000 = df.Filter("amptools_dat && pol_angle == 0");
	auto dat045 = df.Filter("amptools_dat && pol_angle == 45");
	auto dat090 = df.Filter("amptools_dat && pol_angle == 90");
	auto dat135 = df.Filter("amptools_dat && pol_angle == 135");

	std::cout << std::endl << "Filters have been defines next we take a Snapshot" << std::endl;

	// save falt tree with specific branches
	// dat000.Snapshot("kin", (dir+"/"+data+"_"+run+"_000.root").c_str(), branches);
	// dat045.Snapshot("kin", (dir+"/"+data+"_"+run+"_045.root").c_str(), branches);
	// dat090.Snapshot("kin", (dir+"/"+data+"_"+run+"_090.root").c_str(), branches);
	// dat135.Snapshot("kin", (dir+"/"+data+"_"+run+"_135.root").c_str(), branches);
	dat000.Snapshot("kin", (dir+"/"+data+"000.root").c_str(), branches);
	dat045.Snapshot("kin", (dir+"/"+data+"045.root").c_str(), branches);
	dat090.Snapshot("kin", (dir+"/"+data+"090.root").c_str(), branches);
	dat135.Snapshot("kin", (dir+"/"+data+"135.root").c_str(), branches);

	std::cout << std::endl << "Snapshots have completed" << std::endl;
	std::cout << "Number of entries saved " << *dat000.Count() + *dat045.Count() + *dat090.Count() + *dat135.Count() << std::endl;

	if(data == "dat") {
		std::cout << std::endl << "Proccesing background files" << std::endl;

		auto bkg000 = df.Filter("amptools_bkg && pol_angle == 0");
		auto bkg045 = df.Filter("amptools_bkg && pol_angle == 45");
		auto bkg090 = df.Filter("amptools_bkg && pol_angle == 90");
		auto bkg135 = df.Filter("amptools_bkg && pol_angle == 135");

		// bkg000.Snapshot("kin", (dir+"/bkg_"+run+"_000.root").c_str(), branches);
		// bkg045.Snapshot("kin", (dir+"/bkg_"+run+"_045.root").c_str(), branches);
		// bkg090.Snapshot("kin", (dir+"/bkg_"+run+"_090.root").c_str(), branches);
		// bkg135.Snapshot("kin", (dir+"/bkg_"+run+"_135.root").c_str(), branches);
		bkg000.Snapshot("kin", (dir+"/bkg000.root").c_str(), branches);
		bkg045.Snapshot("kin", (dir+"/bkg045.root").c_str(), branches);
		bkg090.Snapshot("kin", (dir+"/bkg090.root").c_str(), branches);
		bkg135.Snapshot("kin", (dir+"/bkg135.root").c_str(), branches);

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