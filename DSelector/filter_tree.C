#include "TFile.h"
#include "TROOT.h"
#include "ROOT/RDF/RFilter.hxx"
#include "ROOT/RDataFrame.hxx"

#include <vector>
#include <string>
#include <iostream>

void filter_tree(std::string inf_name, int n_threads = 8, bool verbose = false) {
	// Parallelize with n threads
	if(n_threads > 0.0)	ROOT::EnableImplicitMT(n_threads);

	// Branches you want to use get
	//std::vector<std::string> branches = {"run", "event", "kin_chisq", "kin_ndf", "beam_x4_meas", "beam_p4_meas", "beam_x4_kin", "beam_p4_kin", "p_x4_meas", "p_p4_meas", "p_x4_kin", "p_p4_kin", "misskl_p4_kin",
	//					"pim_x4_meas", "pim_p4_meas", "pim_x4_kin", "pim_p4_kin", "pip_x4_meas", "pip_p4_meas", "pip_x4_kin", "pip_p4_kin", "event_weight", "accidental_weight", "is_in_time",
	//					"num_unused_tracks", "num_unused_showers", "num_combos", "mpipi", "missing_mass", "mkskl", "beam_energy", "mandel_t", "ks_proper_time", "flight_significance", "path_length",
	//					"chisq_ndf", "cos_hel_ks", "phi_hel_ks", "cos_gj_ks", "phi_gj_ks", "vanHove_x", "vanHove_y", "ks_p4", "kl_p4", "ks_p4_meas", "missing_p4_meas", "pip_p4_cm", "pim_p4_cm",
	//					"ks_p4_cm", "kl_p4_cm", "recoil_p4_cm", "amptools_dat", "amptools_bkg", "Weight", "E_Beam", "Px_Beam", "Py_Beam", "Pz_Beam", "NumFinalState", "E_FinalState", "Px_FinalState",
	//					"Py_FinalState", "Pz_FinalState", "Target_Mass", "PID_FinalState", "pol_angle"};
	//std::vector<std::string> branches = {"run", "event", "kin_chisq", "kin_ndf", "beam_p4_meas", "beam_x4_kin", "beam_p4_kin", "p_x4_kin", "p_p4_kin", "misskl_p4_kin",
	//					"pim_x4_kin", "pim_p4_kin", "pip_x4_kin", "pip_p4_kin", "event_weight", "accidental_weight", "is_in_time",
	//					"num_unused_tracks", "num_unused_showers", "num_combos", "mpipi", "missing_mass", "mkskl", "beam_energy", "mandel_t", "ks_proper_time", "flight_significance", "path_length",
	//					"chisq_ndf", "cos_hel_ks", "phi_hel_ks", "cos_gj_ks", "phi_gj_ks", "vanHove_x", "vanHove_y", "ks_p4", "kl_p4", "missing_p4_meas", "pip_p4_cm", "pim_p4_cm",
	//					"ks_p4_cm", "kl_p4_cm", "recoil_p4_cm", "amptools_dat", "amptools_bkg", "Weight", "E_Beam", "Px_Beam", "Py_Beam", "Pz_Beam", "NumFinalState", "E_FinalState", "Px_FinalState",
	//					"Py_FinalState", "Pz_FinalState", "Target_Mass", "PID_FinalState", "pol_angle"};
	std::vector<std::string> branches = {"run", "event", "kin_chisq", "kin_ndf", "beam_p4_meas", "beam_x4_kin", "beam_p4_kin", "p_x4_kin", "p_p4_kin", "misskl_p4_kin",
						"pim_x4_kin", "pim_p4_kin", "pip_x4_kin", "pip_p4_kin", "event_weight", "accidental_weight", "is_in_time",
						"num_unused_tracks", "num_unused_showers", "num_combos", "mpipi", "missing_mass", "mkskl", "beam_energy", "mandel_t", "ks_proper_time", "flight_significance", "path_length",
						"cos_hel_ks", "phi_hel_ks", "cos_gj_ks", "phi_gj_ks", "vanHove_x", "vanHove_y", "missing_p4_meas", 
						"amptools_dat", "amptools_bkg", "Weight", "E_Beam", "Px_Beam", "Py_Beam", "Pz_Beam", "NumFinalState", "E_FinalState", "Px_FinalState",
						"Py_FinalState", "Pz_FinalState", "Target_Mass", "PID_FinalState", "pol_angle"};

	// make data frame
	// format : tree name, file name, branches to open
	auto df = ROOT::RDataFrame("kskl", inf_name.c_str(), branches);

	if(verbose) {
		std::cout << n_threads << " threads used" << std::endl;
		std::cout << "Branches to save" << std::endl;
		for(auto branch : branches)	std::cout << "- " << branch << std::endl;
	}

	// save falt tree with specific branches
	df.Snapshot("kskl", "snapshot.root", branches);

	std::cout << std::endl << "Snapshot complete" << std::endl;
}
