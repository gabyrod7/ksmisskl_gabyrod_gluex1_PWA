std::string set_cuts(std::map<std::string, std::string> cuts_list, std::pair<std::string, std::string> change_cut = {"", ""});

void RDF_ana(Int_t n_threads,string inf_name, string opf_name, Bool_t show_cut_report) {	
	cout<<" "<< endl;
	cout<<"Run RDataFrame Analysis"<< endl;
	cout<<" "<< endl;
    
	if(n_threads > 0.0)	ROOT::EnableImplicitMT(n_threads);
	
	TStopwatch watch;
	
	watch.Reset();
	watch.Start();
	
	cout <<"Load data and set ROOT DataFrame..."<< endl;
	
	//1.) Define the chain:
	TChain my_chain("kskl");
	my_chain.Add(inf_name.c_str());
	Int_t nEvents = my_chain.GetEntries();
   	cout << "Events to process " << nEvents << endl; 

	TFile *out_file = new TFile(opf_name.c_str(),"RECREATE");

	//2.) Set the RDataFrame:
	ROOT::RDataFrame rdf(my_chain);
	
	cout <<"...done!"<< endl;
	cout <<" "<< endl;
	
	cout <<"Define variables and cuts..."<< endl;
	
	//3.) Define important variables and analysis conditions
	
	//3.1) Define some variables first:
	auto rdf_variables = rdf.Define("fs", "flight_significance").Define("misse", "missing_p4_meas.E()").Define("mmiss", "missing_mass")
				.Define("mksp", "(ks_p4 + p_p4_kin).M()").Define("mklp", "(kl_p4 + p_p4_kin).M()")
				.Define("ks_phi", "ks_p4_cm.Phi()").Define("p_z", "p_x4_kin.Z()")
				.Define("ksphi", "ks_p4.Phi()*175/3.14159265359")
				.Define("mpipp", "(pip_p4_kin + p_p4_kin).M()").Define("mpimp", "(pim_p4_kin + p_p4_kin).M()")
				.Define("special_weight", "accidental_weight * 0.5");

	// 3.2) Make list of nominal cuts
	std::map<std::string, std::string> cuts_list = {
		{"mkskl", "mkskl > 1.10 && mkskl < 2.70"},
		{"mmiss", "missing_mass > 0.3 && missing_mass < 0.7"},
		{"mandel_tp", "mandel_tp > 0.20 && mandel_tp < 1.0"},
		{"flight_significance", "flight_significance > 6"},
		{"chisq", "chisq_ndf < 2"},
		{"ntracks", "num_unused_tracks == 0"},
		{"nshowers", "num_unused_showers < 3"},
		{"proton_z_vertex", "proton_z_vertex > 52 && proton_z_vertex < 78"},
		{"beam_energy", "beam_energy > 8.2 && beam_energy < 8.8"} 
	};
	
	string cuts = "";
	string signal = "(mpipi > 0.48 && mpipi < 0.52)";
	string sideband = "((mpipi > 0.44 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.56))";

	//3.3)Now apply cuts on the newly defined variables:
	cuts = set_cuts(cuts_list, {"", ""});
	auto rdf_cut = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"mmiss", ""});
	auto rdf_cut_mmiss = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"mandel_tp", ""});
	auto rdf_cut_mandel_tp = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"flight_significance", ""});
	auto rdf_cut_FS = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"chisq", ""});
	auto rdf_cut_chisq = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"ntracks", ""});
	auto rdf_cut_ntracks = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"nshowers", ""});
	auto rdf_cut_nshowers = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"proton_z_vertex", ""});
	auto rdf_cut_proton_z_vertex = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"beam_energy", ""});
	auto rdf_cut_beam_energy = rdf_variables.Filter(cuts);

	cout <<"...done!"<< endl;
	cout <<" "<< endl;
	
	//4.) Define Histograms:
	
	cout <<"Set up histograms..."<< endl;
	
	//4.1) Histograms
	int nBins = 72;
	double xMin = 1.16, xMax = 2.60;
	auto im_kskl = rdf_cut.Filter(signal).Histo1D({"im_kskl", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	auto im_kskl_sb = rdf_cut.Filter(sideband).Histo1D({"im_kskl_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_tp = rdf_cut.Filter(signal).Histo2D({"im_kskl_tp", ";M(K_{S}K_{L});-t';Counts", nBins, xMin, xMax, 100, 0.0, 1.0}, "mkskl", "mandel_tp", "accidental_weight");
	// auto im_kskl_tp_sb = rdf_cut.Filter(sideband).Histo2D({"im_kskl_tp_sb", ";M(K_{S}K_{L});-t';Counts", nBins, xMin, xMax, 100, 0.0, 1.0}, "mkskl", "mandel_tp", "accidental_weight");

	// auto im_kskl_mmiss1 = rdf_cut_mmiss.Filter(signal+"&& mmiss > 0.2 && mmiss < 0.8").Histo1D({"im_kskl_mmiss1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_mmiss1_sb = rdf_cut_mmiss.Filter(sideband+"&& mmiss > 0.2 && mmiss < 0.8").Histo1D({"im_kskl_mmiss1_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_mmiss2 = rdf_cut_mmiss.Filter(signal+"&& mmiss > 0.35 && mmiss < 0.65").Histo1D({"im_kskl_mmiss2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_mmiss2_sb = rdf_cut_mmiss.Filter(sideband+"&& mmiss > 0.35 && mmiss < 0.65").Histo1D({"im_kskl_mmiss2_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_baryonCut = rdf_cut.Filter(signal+"&& mksp > 2 && mklp > 2").Histo1D({"im_kskl_baryonCut", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_baryonCut_sb = rdf_cut.Filter(sideband+"&& mksp > 2 && mklp > 2").Histo1D({"im_kskl_baryonCut_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_mandel_tp1 = rdf_cut_mandel_tp.Filter(signal+"&& mandel_tp > 0.25 && mandel_tp < 0.9").Histo1D({"im_kskl_mandel_tp1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_mandel_tp1_sb = rdf_cut_mandel_tp.Filter(sideband+"&& mandel_tp > 0.25 && mandel_tp < 0.9").Histo1D({"im_kskl_mandel_tp1_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_mandel_tp2 = rdf_cut_mandel_tp.Filter(signal+"&& mandel_tp > 0.15 && mandel_tp < 1.10").Histo1D({"im_kskl_mandel_tp2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_mandel_tp2_sb = rdf_cut_mandel_tp.Filter(sideband+"&& mandel_tp > 0.15 && mandel_tp < 1.10").Histo1D({"im_kskl_mandel_tp2_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_FS1 = rdf_cut_FS.Filter(signal+"&& fs > 5").Histo1D({"im_kskl_FS1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_FS1_sb = rdf_cut_FS.Filter(sideband+"&& fs > 5").Histo1D({"im_kskl_FS1_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_FS2 = rdf_cut_FS.Filter(signal+"&& fs > 7").Histo1D({"im_kskl_FS2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_FS2_sb = rdf_cut_FS.Filter(sideband+"&& fs > 7").Histo1D({"im_kskl_FS2_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_chisq1 = rdf_cut_chisq.Filter(signal+"&& chisq_ndf < 1.5").Histo1D({"im_kskl_chisq1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_chisq1_sb = rdf_cut_chisq.Filter(sideband+"&& chisq_ndf < 1.5").Histo1D({"im_kskl_chisq1_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_chisq2 = rdf_cut_chisq.Filter(signal+"&& chisq_ndf < 2.5").Histo1D({"im_kskl_chisq2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_chisq2_sb = rdf_cut_chisq.Filter(sideband+"&& chisq_ndf < 2.5").Histo1D({"im_kskl_chisq2_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_ntracks1 = rdf_cut_ntracks.Filter(signal+"&& num_unused_tracks < 2").Histo1D({"im_kskl_ntracks1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_ntracks1_sb = rdf_cut_ntracks.Filter(sideband+"&& num_unused_tracks < 2").Histo1D({"im_kskl_ntracks1_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_nshowers1 = rdf_cut_nshowers.Filter(signal+"&& num_unused_showers < 2").Histo1D({"im_kskl_nshowers1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_nshowers1_sb = rdf_cut_nshowers.Filter(sideband+"&& num_unused_showers < 2").Histo1D({"im_kskl_nshowers1_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_nshowers2 = rdf_cut_nshowers.Filter(signal+"&& num_unused_showers < 4").Histo1D({"im_kskl_nshowers2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_nshowers2_sb = rdf_cut_nshowers.Filter(sideband+"&& num_unused_showers < 4").Histo1D({"im_kskl_nshowers2_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_nshowers3 = rdf_cut_nshowers.Filter(signal+"&& num_unused_showers < 1").Histo1D({"im_kskl_nshowers3", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_nshowers3_sb = rdf_cut_nshowers.Filter(sideband+"&& num_unused_showers < 1").Histo1D({"im_kskl_nshowers3_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_proton_z_vertex1 = rdf_cut_proton_z_vertex.Filter(signal+"&& proton_z_vertex > 51 && proton_z_vertex < 79").Histo1D({"im_kskl_proton_z_vertex1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_proton_z_vertex1_sb = rdf_cut_proton_z_vertex.Filter(sideband+"&& proton_z_vertex > 51 && proton_z_vertex < 79").Histo1D({"im_kskl_proton_z_vertex1_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_proton_z_vertex2 = rdf_cut_proton_z_vertex.Filter(signal+"&& proton_z_vertex > 53 && proton_z_vertex < 77").Histo1D({"im_kskl_proton_z_vertex2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_proton_z_vertex2_sb = rdf_cut_proton_z_vertex.Filter(sideband+"&& proton_z_vertex > 53 && proton_z_vertex < 77").Histo1D({"im_kskl_proton_z_vertex2_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_beam_energy1 = rdf_cut_beam_energy.Filter(signal+"&& beam_energy > 8.2 && beam_energy < 8.5").Histo1D({"im_kskl_beam_energy1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_beam_energy1_sb = rdf_cut_beam_energy.Filter(sideband+"&& beam_energy > 8.2 && beam_energy < 8.5").Histo1D({"im_kskl_beam_energy1_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_beam_energy2 = rdf_cut_beam_energy.Filter(signal+"&& beam_energy > 8.5 && beam_energy < 8.8").Histo1D({"im_kskl_beam_energy2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_beam_energy2_sb = rdf_cut_beam_energy.Filter(sideband+"&& beam_energy > 8.5 && beam_energy < 8.8").Histo1D({"im_kskl_beam_energy2_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// signal = "(mpipi > 0.485 && mpipi < 0.515)";
	// sideband = "((mpipi > 0.445 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.555))";
	// auto im_kskl_mpipi1 = rdf_cut.Filter(signal).Histo1D({"im_kskl_mpipi1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_mpipi1_sb = rdf_cut.Filter(sideband).Histo1D({"im_kskl_mpipi1_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// signal = "(mpipi > 0.475 && mpipi < 0.525)";
	// sideband = "((mpipi > 0.435 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.565))";
	// auto im_kskl_mpipi2 = rdf_cut.Filter(signal).Histo1D({"im_kskl_mpipi2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_mpipi2_sb = rdf_cut.Filter(sideband).Histo1D({"im_kskl_mpipi2_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");

	// signal = "(mpipi > 0.48 && mpipi < 0.52)";
	// sideband = "((mpipi > 0.42 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.58))";
	// auto im_kskl_mpipi3 = rdf_cut.Filter(signal).Histo1D({"im_kskl_mpipi3", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_mpipi3_sb = rdf_cut.Filter(sideband).Histo1D({"im_kskl_mpipi3_sb", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl", "special_weight");

	// signal = "(mpipi > 0.48 && mpipi < 0.52)";
	// sideband = "((mpipi > 0.44 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.56))";

	// auto im_kskl_fitRange1 = rdf_cut.Filter(signal).Histo1D({"im_kskl_fitRange1", ";M(K_{S}K_{L});Counts", 78, 1.10, 2.66}, "mkskl", "accidental_weight");
	// auto im_kskl_fitRange1_sb = rdf_cut.Filter(sideband).Histo1D({"im_kskl_fitRange1_sb", ";M(K_{S}K_{L});Counts", 78, 1.10, 2.66}, "mkskl", "accidental_weight");

	// auto im_kskl_fitRange2 = rdf_cut.Filter(signal).Histo1D({"im_kskl_fitRange2", ";M(K_{S}K_{L});Counts", 67, 1.20, 2.54}, "mkskl", "accidental_weight");
	// auto im_kskl_fitRange2_sb = rdf_cut.Filter(sideband).Histo1D({"im_kskl_fitRange2_sb", ";M(K_{S}K_{L});Counts", 67, 1.20, 2.54}, "mkskl", "accidental_weight");

	// auto im_kskl_binning1 = rdf_cut.Filter(signal).Histo1D({"im_kskl_binning1", ";M(K_{S}K_{L});Counts", 64, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_binning1_sb = rdf_cut.Filter(sideband).Histo1D({"im_kskl_binning1_sb", ";M(K_{S}K_{L});Counts", 64, xMin, xMax}, "mkskl", "accidental_weight");

	// auto im_kskl_binning2 = rdf_cut.Filter(signal).Histo1D({"im_kskl_binning2", ";M(K_{S}K_{L});Counts", 100, xMin, xMax}, "mkskl", "accidental_weight");
	// auto im_kskl_binning2_sb = rdf_cut.Filter(sideband).Histo1D({"im_kskl_binning2_sb", ";M(K_{S}K_{L});Counts", 100, xMin, xMax}, "mkskl", "accidental_weight");

	cout <<" "<< endl;
	
	//5.) Write everything to a file:
	cout <<"Write results to file: " << opf_name << endl;

	im_kskl->Write();
	im_kskl_sb->Write();

	// im_kskl_tp->Write();
	// im_kskl_tp_sb->Write();

	// im_kskl_mpipi1->Write();
	// im_kskl_mpipi1_sb->Write();

	// im_kskl_mpipi2->Write();
	// im_kskl_mpipi2_sb->Write();

	// im_kskl_mpipi3->Write();
	// im_kskl_mpipi3_sb->Write();

	// im_kskl_mmiss1->Write();
	// im_kskl_mmiss1_sb->Write();

	// im_kskl_mmiss2->Write();
	// im_kskl_mmiss2_sb->Write();

	// im_kskl_baryonCut->Write();
	// im_kskl_baryonCut_sb->Write();

	// im_kskl_mandel_tp1->Write();
	// im_kskl_mandel_tp1_sb->Write();

	// im_kskl_mandel_tp2->Write();
	// im_kskl_mandel_tp2_sb->Write();

	// im_kskl_FS1->Write();
	// im_kskl_FS1_sb->Write();

	// im_kskl_FS2->Write();
	// im_kskl_FS2_sb->Write();

	// im_kskl_chisq1->Write();
	// im_kskl_chisq1_sb->Write();

	// im_kskl_chisq2->Write();
	// im_kskl_chisq2_sb->Write();

	// im_kskl_ntracks1->Write();
	// im_kskl_ntracks1_sb->Write();

	// im_kskl_nshowers1->Write();
	// im_kskl_nshowers1_sb->Write();

	// im_kskl_nshowers2->Write();
	// im_kskl_nshowers2_sb->Write();

	// im_kskl_nshowers3->Write();
	// im_kskl_nshowers3_sb->Write();

	// im_kskl_proton_z_vertex1->Write();
	// im_kskl_proton_z_vertex1_sb->Write();

	// im_kskl_proton_z_vertex2->Write();
	// im_kskl_proton_z_vertex2_sb->Write();

	// im_kskl_beam_energy1->Write();
	// im_kskl_beam_energy1_sb->Write();

	// im_kskl_beam_energy2->Write();
	// im_kskl_beam_energy2_sb->Write();

	// im_kskl_fitRange1->Write();
	// im_kskl_fitRange1_sb->Write();

	// im_kskl_fitRange2->Write();
	// im_kskl_fitRange2_sb->Write();

	// im_kskl_binning1->Write();
	// im_kskl_binning1_sb->Write();

	// im_kskl_binning2->Write();
	// im_kskl_binning2_sb->Write();

	out_file->Write();
	out_file->Close();
	
	cout <<"...done!"<< endl;
	cout <<" "<< endl;
	
	watch.Stop();
	Double_t ana_time = watch.RealTime();

	if(n_threads > 0 && show_cut_report)
		cout << "Most set n_threads to 0 to view cut report" << endl;

	if(n_threads == 0 && show_cut_report){
		cout <<"  "<< endl;
		cout <<"Cut Report:"<< endl;
		auto allCutsReport = rdf_cut.Report();
		allCutsReport->Print();
		cout <<"  "<< endl;
	}

	Double_t report_time = 0.0;
	TString ana_time_unit = "s";
	//----------------------------------------------
	if(ana_time < 60.0){
	    report_time = ana_time;
	}else if(ana_time >= 60.0 && ana_time < 3600.0){
	    report_time = ana_time / 60.0;
	    ana_time_unit = "min";
	}else if(ana_time >= 3600.0){
	    report_time = ana_time / 3600.0;
	    ana_time_unit = "h";
	}
	//----------------------------------------------
	
	cout <<"Processed "<<nEvents<<" events in: "<<report_time<<ana_time_unit<< endl;
	cout <<"Have a great day!"<< endl;
	cout <<" "<< endl;
}

std::string set_cuts(std::map<std::string, std::string> cuts_list, std::pair<std::string, std::string> change_cut = {"", ""}) {
	std::string cuts = "";

	for(auto it = cuts_list.begin(); it != cuts_list.end(); ++it) {
		if(it->first == change_cut.first)	{
			if(change_cut.second != "")
				cuts += change_cut.second + " && ";
			else // if change to cut is an empty string, then remove cut
				continue;
		}
		else								cuts += it->second + " && ";
	}
	cuts.erase(cuts.size()-4, 4); // remove last " && "

	return cuts;
}