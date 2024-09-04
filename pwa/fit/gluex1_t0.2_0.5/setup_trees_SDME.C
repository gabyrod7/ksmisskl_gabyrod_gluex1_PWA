void Split(string inf_name, string data, string dir, int n_threads = 4, string run = "", string filter = "", bool verbose = false);
void runSplit(vector<string> inf_names, string dir, int n_threads = 4, string run = "", string cuts = "", string gen_cuts = "", bool verbose = false);
string set_cuts(map<string, string> cuts_list, pair<string, string> change_cut = {"", ""});

void setup_trees(string pathToDSelectorFlatTrees) {
	vector<string> inf_names;
	string dir = "trees/";
	int n_threads = 16;
	string run = "";
	string cuts = "";
	string gen_cuts = "";

	// List of cuts
	map<string, string> cuts_list = {
		{"mkskl", "mkskl > 1.005 && mkskl < 1.04"},
		{"mmiss", "missing_mass > 0.3 && missing_mass < 0.7"},
		{"mpipi" , "mpipi > 0.48 && mpipi < 0.52"},
		{"mandel_t", "mandel_t > 0.15 && mandel_t < 1.0"},
		{"fs", "flight_significance > 4"},
		{"chisq", "chisq_ndf < 4"},
		{"ntracks", "num_unused_tracks == 0"},
		{"nshowers", "num_unused_showers <= 2"},
		{"proton_z_vertex", "proton_z_vertex > 52 && proton_z_vertex < 78"},
		{"beam_energy", "beam_energy > 8.2 && beam_energy < 8.8"}
	};

	// Set cuts
	cuts = set_cuts(cuts_list);
	gen_cuts = "mandel_t > 0.15 && mandel_t < 1.0";

	// GlueX-I data for SDME
	inf_names = {pathToDSelectorFlatTrees+"ftree_dat_gluex1.root", pathToDSelectorFlatTrees+"ftree_acc_gluex1.root", pathToDSelectorFlatTrees+"ftree_gen_gluex1.root"};
	run = "gluex1";
	runSplit(inf_names, dir, n_threads, run, cuts, gen_cuts);
}

void runSplit(vector<string> inf_names, string dir, int n_threads = 4, string run = "", string cuts = "", string gen_cuts = "", bool verbose = false) {
	if(gSystem->AccessPathName(dir.c_str())) {
		cout << "Making directory called "+dir << endl;
		gSystem->Exec( ("mkdir "+dir).c_str() );
	}

	Split(inf_names[0], "dat", dir, n_threads, run, cuts);
	Split(inf_names[1], "acc", dir, n_threads, run, cuts);
	Split(inf_names[2], "gen", dir, n_threads, run, gen_cuts);
}

void Split(string inf_name, string data, string dir, int n_threads = 4, string run = "", string filter = "", bool verbose = false) {
	// Parallelize with n threads
	if(n_threads > 0.0)	ROOT::EnableImplicitMT(n_threads);

	// Branches you want to save get
	vector<string> branches = {"Weight", "pol_angle", "E_Beam", "Px_Beam", "Py_Beam", "Pz_Beam", 
				"NumFinalState", "E_FinalState", "Px_FinalState", "Py_FinalState", "Pz_FinalState", "mandel_t"};

	// make data frame
	// format : tree name, file name, branches to open
	auto df = ROOT::RDataFrame("kin", inf_name.c_str()).Filter(filter);

	cout << endl;
	cout << "We will now separate the file "+inf_name+" base on polarization angle" << endl;
	cout << "Number of entries to proccess " << *df.Count() << endl;

	if(verbose) {
		cout << n_threads << " threads used" << endl;
		cout << "Branches to save" << endl;
		for(auto branch : branches)	cout << "- " << branch << endl;
	}
	
	// make new data frame with cuts
	auto dat000 = df.Filter( "is_in_time && pol_angle == 0" );
	auto dat045 = df.Filter( "is_in_time && pol_angle == 45" );
	auto dat090 = df.Filter( "is_in_time && pol_angle == 90" );
	auto dat135 = df.Filter( "is_in_time && pol_angle == 135" );

	cout << endl << "Filters have been defines next we take a Snapshot" << endl;

	// save falt tree with specific branches
	dat000.Snapshot("kin", (dir+data+"_"+run+"_000.root").c_str(), branches);
	dat045.Snapshot("kin", (dir+data+"_"+run+"_045.root").c_str(), branches);
	dat090.Snapshot("kin", (dir+data+"_"+run+"_090.root").c_str(), branches);
	dat135.Snapshot("kin", (dir+data+"_"+run+"_135.root").c_str(), branches);

	cout << endl << "Snapshots have completed" << endl;
	cout << "Number of entries saved " << *dat000.Count() + *dat045.Count() + *dat090.Count() + *dat135.Count() << endl;

	if(data == "dat") {
		cout << endl << "Proccesing background files" << endl;

		auto bkg000 = df.Filter("!is_in_time && pol_angle == 0");
		auto bkg045 = df.Filter("!is_in_time && pol_angle == 45");
		auto bkg090 = df.Filter("!is_in_time && pol_angle == 90");
		auto bkg135 = df.Filter("!is_in_time && pol_angle == 135");

		bkg000.Snapshot("kin", (dir+"bkg_"+run+"_000.root").c_str(), branches);
		bkg045.Snapshot("kin", (dir+"bkg_"+run+"_045.root").c_str(), branches);
		bkg090.Snapshot("kin", (dir+"bkg_"+run+"_090.root").c_str(), branches);
		bkg135.Snapshot("kin", (dir+"bkg_"+run+"_135.root").c_str(), branches);

		cout << endl << "Snapshot of background files has been taken" << endl;
	}
}

string set_cuts(map<string, string> cuts_list, pair<string, string> change_cut = {"", ""}) {
	string cuts = "";

	for(auto it = cuts_list.begin(); it != cuts_list.end(); ++it) {
		if(it->first == change_cut.first)	cuts += change_cut.second + " && ";
		else								cuts += it->second + " && ";
	}
	cuts.erase(cuts.size()-4, 4); // remove last " && "

	return cuts;
}
