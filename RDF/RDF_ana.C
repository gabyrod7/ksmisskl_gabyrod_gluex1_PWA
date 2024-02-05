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
	auto rdf_variables = rdf.Define("fs", "flight_significance").Define("mmiss", "missing_p4_meas.M()").Define("missing_energy", "missing_p4_meas.E()")
				.Define("mksp", "(ks_p4 + p_p4_kin).M()").Define("mklp", "(kl_p4 + p_p4_kin).M()").Define("mmiss2", "missing_p4_meas.M2()")
				.Define("ks_phi", "ks_p4_cm.Phi()").Define("p_z", "p_x4_kin.Z()").Define("ks_costheta_cm", "ks_p4_cm.CosTheta()")
				.Define("mpipp", "(pip_p4_kin + p_p4_kin).M()").Define("mpimp", "(pim_p4_kin + p_p4_kin).M()");

	//3.2)Now apply cuts on the newly defined variables:
	
	std::map<std::string, std::string> cuts_list = {
		// {"mkskl", "mkskl > 1.1 && mkskl < 2.00"},
		{"mmiss", "missing_mass > 0.3 && missing_mass < 0.7"},
		{"mandel_t", "mandel_t > 0.2 && mandel_t < 0.5"},
		{"fs", "flight_significance > 6"},
		{"chisq", "chisq_ndf < 2"},
		{"ntracks", "num_unused_tracks == 0"},
		{"nshowers", "num_unused_showers < 3"},
		{"proton_z_vertex", "proton_z_vertex > 52 && proton_z_vertex < 78"},
		{"beam_energy", "beam_energy > 8.2 && beam_energy < 8.8"}};

	string cuts = set_cuts(cuts_list);
	string signal = "(mpipi > 0.48 && mpipi < 0.52)";
	string sideband = "((mpipi > 0.43 && mpipi < 0.45) || (mpipi > 0.55 && mpipi < 0.57))";

	auto rdf_cut = rdf_variables.Filter(cuts);
	auto rdf_cutp = rdf_variables.Filter("mmiss > 0.3 && mmiss < 0.7 && fs > 6 && chisq_ndf < 2 && mandel_t > 0.2 && mandel_t < 0.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdf_cut4 = rdf_variables.Filter("mpipi > 0.48 && mpipi < 0.52 && mmiss > 0.3 && mmiss < 0.7 && mandel_t > 0.2 && mandel_t < 0.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdf_cut_v2 = rdf_variables.Filter("amptools_dat == 1 && mmiss > 0.3 && mmiss < 0.7 && fs > 6 && chisq_ndf < 2 && mandel_t > 0.2 && mandel_t < 0.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	// auto rdf_cut = rdf_variables.Filter("mpipi > 0.48 && mpipi < 0.52 && mmiss > 0.3 && mmiss < 0.7 && fs > 6 && chisq_ndf < 2 && mandel_t > 0.2 && mandel_t < 0.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdf_cut_sb = rdf_variables.Filter("((mpipi > 0.44 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.56)) && mmiss > 0.3 && mmiss < 0.7 && fs > 6 && chisq_ndf < 2 && mandel_t > 0.2 && mandel_t < 0.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdf_cut_sb2 = rdf_variables.Filter("amptools_bkg == true && mmiss > 0.3 && mmiss < 0.7 && fs > 6 && chisq_ndf < 2 && mandel_t > 0.2 && mandel_t < 0.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdf_cut_x = rdf_variables.Filter("mmiss > 0.3 && mmiss < 0.7 && fs > 6 && chisq_ndf < 2 && mandel_t > 0.2 && mandel_t < 0.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	//auto rdf_cut = rdf_variables.Filter("(p_z > 83 && p_z < 86) && mpipi > 0.48 && mpipi < 0.52 && mmiss > 0.3 && mmiss < 0.7 && fs > 4 && chisq_ndf < 4 && mandel_t > 0.15&& mandel_t < 1.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	//auto rdf_cut_sb = rdf_variables.Filter("(p_z > 83 && p_z < 86) && ((mpipi > 0.44 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.56)) && mmiss > 0.3 && mmiss < 0.7 && fs > 4 && chisq_ndf < 4 && mandel_t > 0.15&& mandel_t < 1.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdf_cut2 = rdf_variables.Filter("mpipi > 0.48 && mpipi < 0.52 && mmiss > 0.4 && mmiss < 0.6 && fs > 6 && chisq_ndf < 2 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdf_cut2_sb = rdf_variables.Filter("((mpipi > 0.44 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.56)) && mmiss > 0.4 && mmiss < 0.6 && fs > 6 && chisq_ndf < 2 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdf_cut3 = rdf_variables.Filter("mmiss > 0.3 && mmiss < 0.7 && fs > 6 && chisq_ndf < 2 && mandel_t > 0.2 && mandel_t < 0.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdfChiSqNdf_cut = rdf_variables.Filter("mmiss > 0.4 && mmiss < 0.6 && fs > 3 && mandel_t > 0.1 && mandel_t < 0.5 && num_unused_tracks == 0");
	auto rdfFlightSignificance_cut = rdf_variables.Filter("mmiss > 0.4 && mmiss < 0.6 && chisq_ndf < 4 && mandel_t > 0.1 && mandel_t < 0.5 && num_unused_tracks == 0");
	auto rdfNumUnusedTracks_cut = rdf_variables.Filter("mmiss > 0.4 && mmiss < 0.6 && chisq_ndf < 2 && fs > 6 && mandel_t > 0.1 && mandel_t < 0.5");
	auto rdfNumUnusedShowers_cut = rdf_variables.Filter("mmiss > 0.4 && mmiss < 0.6 && chisq_ndf < 2 && fs > 6 && mandel_t > 0.1 && mandel_t < 0.5 && num_unused_tracks == 0");

	cout <<"...done!"<< endl;
	cout <<" "<< endl;
	
	//4.) Define Histograms:
	
	cout <<"Set up histograms..."<< endl;
	
	//4.1) Histograms
	auto kscosThetaCM = rdf_cut.Histo1D({"kscosThetaCM", ";K_{S} cos#theta CoM;Counts", 100, -1, 1}, "ks_costheta_cm", "accidental_weight");
	auto cos_mkskl = rdf_cut.Histo2D({"cos_mkskl", ";M(K_{S}K_{L});K_{S} cos#theta CoM",  50, 1.10, 2.0, 40, -1, 1}, "mkskl", "ks_costheta_cm", "accidental_weight");
	auto van_hove = rdf_cut.Histo2D({"van_hove", ";X;Y",  40, -4, 4, 40, -4, 4}, "vanHove_x", "vanHove_y", "accidental_weight");

	auto cosHX_mkskl = rdf_cutp.Filter("mpipi > 0.48 && mpipi < 0.52").Histo2D({"cosHX_mkskl", ";M(K_{S}K_{L});K_{S} cos#theta Helicity",  50, 1.10, 2.0, 40, -1, 1}, "mkskl", "cos_hel_ks", "accidental_weight");
	auto cosHX_mkskl_sb = rdf_cutp.Filter("(mpipi > 0.43 && mpipi < 0.45) || (mpipi > 0.55 && mpipi < 0.57)").Histo2D({"cosHX_mkskl_sb", ";M(K_{S}K_{L});K_{S} cos#theta Helicity",  50, 1.10, 2.0, 40, -1, 1}, "mkskl", "cos_hel_ks", "accidental_weight");

	auto cosHX = rdf_cutp.Filter("mpipi > 0.48 && mpipi < 0.52").Histo1D({"cosHX", ";M(K_{S}K_{L});K_{S} cos#theta Helicity",   40, -1, 1}, "cos_hel_ks", "accidental_weight");
	auto cosHX_sb = rdf_cutp.Filter("(mpipi > 0.43 && mpipi < 0.45) || (mpipi > 0.55 && mpipi < 0.57)").Histo1D({"cosHX_sb", ";K_{S} cos#theta Helicity", 40, -1, 1}, "cos_hel_ks", "accidental_weight");

	auto im_kskl = rdf_cut.Filter(signal).Histo1D({"im_kskl", ";M(K_{S}K_{L});Counts", 50 , 1.00, 2.00}, "mkskl", "accidental_weight");
	auto im_kskl_sb = rdf_cut.Filter(sideband).Histo1D({"im_kskl_sb", ";M(K_{S}K_{L});Counts", 50 , 1.00, 2.00}, "mkskl", "accidental_weight");

	auto im_kskl2 = rdf_cut.Filter("amptools_dat").Histo1D({"im_kskl2", ";M(K_{S}K_{L});Counts", 50 , 1.00, 2.00}, "mkskl", "Weight");
	auto im_kskl2_sb = rdf_cut.Filter("amptools_bkg").Histo1D({"im_kskl2_sb", ";M(K_{S}K_{L});Counts", 50 , 1.00, 2.00}, "mkskl", "Weight");
	auto im_kskl2_sb2 = rdf_cut.Filter(sideband+" && in_time").Histo1D({"im_kskl2_sb2", ";M(K_{S}K_{L});Counts", 50 , 1.00, 2.00}, "mkskl", "Weight");
	auto im_kskl2_sb3 = rdf_cut.Filter(signal+" && !in_time").Histo1D({"im_kskl2_sb3", ";M(K_{S}K_{L});Counts", 50 , 1.00, 2.00}, "mkskl", "Weight");
	auto im_kskl2_sb4 = rdf_cut.Filter(sideband+" && !in_time").Histo1D({"im_kskl2_sb4", ";M(K_{S}K_{L});Counts", 50 , 1.00, 2.00}, "mkskl", "Weight");

	auto im_ksp = rdf_cut.Histo1D({"im_ksp", ";M(K_{S}p);Counts", 100, 1.00, 4.00}, "mksp", "accidental_weight");
	auto im_klp = rdf_cut.Histo1D({"im_klp", ";M(K_{L}p);Counts", 100, 1.00, 4.00}, "mklp", "accidental_weight");
	auto im_pipp = rdf_cut.Histo1D({"im_pipp", ";M(#pi^{+}p);Counts", 100, 1.00, 4.00}, "mpipp", "accidental_weight");
	auto im_pimp = rdf_cut.Histo1D({"im_pimp", ";M(#pi^{-}p);Counts", 100, 1.00, 4.00}, "mpimp", "accidental_weight");
	auto im_miss = rdf_cut.Histo1D({"im_miss", ";Missing Mass (GeV);Counts", 100, 0.00, 1.00}, "mmiss", "accidental_weight");
	auto im_pipi = rdf_cut3.Histo1D({"im_pipi", ";M(#pi#pi) (GeV);Counts", 200, 0.40, 0.60}, "mpipi", "accidental_weight");
	// auto im_pipi2 = rdf_cut_sb2.Histo1D({"im_pipi2", ";M(#pi#pi) (GeV);Counts", 200, 0.40, 0.60}, "mpipi", "Weight");
	// auto im_pipi3 = rdf_cut_sb2.Filter("is_in_time == true").Histo1D({"im_pipi3", ";M(#pi#pi) (GeV);Counts", 200, 0.40, 0.60}, "mpipi", "Weight");
	// auto im_pipi4 = rdf_cut_sb2.Filter("is_in_time == false").Histo1D({"im_pipi4", ";M(#pi#pi) (GeV);Counts", 200, 0.40, 0.60}, "mpipi", "Weight");
	// auto im_pipi5 = rdf_cut_v2.Histo1D({"im_pipi5", ";M(#pi#pi) (GeV);Counts", 200, 0.40, 0.60}, "mpipi", "accidental_weight");

	auto miss_energy = rdf_cut.Histo1D({"miss_energy", ";Missing Energy (GeV);Counts", 100, 0.00, 10.00}, "missing_energy", "accidental_weight");
	auto miss_energy_sb = rdf_cut_sb.Histo1D({"miss_energy_sb", ";Missing Energy (GeV);Counts", 100 , 0.00, 10.0}, "missing_energy", "accidental_weight");

	auto im_miss2 = rdf_cut.Histo1D({"im_miss2", ";Missing Mass^{2} (GeV);Counts", 100, 0.00, 2.00}, "mmiss2", "accidental_weight");
	auto im_miss2_sb = rdf_cut_sb.Histo1D({"im_miss2_sb", ";Missing Mass (GeV);Counts", 100, 0.00, 2.00}, "mmiss2", "accidental_weight");

	auto im_ksp_sb = rdf_cut_sb.Histo1D({"im_ksp_sb", ";M(K_{S}p);Counts", 100, 1.00, 4.00}, "mksp", "accidental_weight");
	auto im_klp_sb = rdf_cut_sb.Histo1D({"im_klp_sb", ";M(K_{L}p);Counts", 100, 1.00, 4.00}, "mklp", "accidental_weight");
	auto im_pipp_sb = rdf_cut_sb.Histo1D({"im_pipp_sb", ";M(#pi^{+}p);Counts", 100, 1.00, 4.00}, "mpipp", "accidental_weight");
	auto im_pimp_sb = rdf_cut_sb.Histo1D({"im_pimp_sb", ";M(#pi^{-}p);Counts", 100, 1.00, 4.00}, "mpimp", "accidental_weight");

	auto h1_p_z = rdf_cut.Histo1D({"h1_p_z", ";recoil proton Z (cm);Counts", 100, 0.00, 100.00}, "p_z", "accidental_weight");
	auto h1_p_z_sb = rdf_cut_sb.Histo1D({"h1_p_z_sb", ";recoil proton Z (cm);Counts", 100, 0.00, 100.00}, "p_z", "accidental_weight");

	auto h1_t = rdf_cut2.Histo1D({"h1_t", ";-t;Counts",  100, 0, 1}, "mandel_t", "accidental_weight");
	auto h1_t_sb = rdf_cut2_sb.Histo1D({"h1_t_sb", ";-t;Counts",  100, 0, 1}, "mandel_t", "accidental_weight");

	auto h2_mpipi_t = rdf_cut3.Histo2D({"h2_mpipi_t", ";M(#pi#pi);-t;Counts",  100, 0.4, 0.6, 150, 0, 1.5}, "mpipi", "mandel_t", "accidental_weight");

	auto h1_path_length = rdf_cut2.Histo1D({"h1_path_length", ";Path Length;Counts",  100, 0, 100}, "path_length", "accidental_weight");
	auto h1_path_length_sb = rdf_cut2_sb.Histo1D({"h1_path_length", ";Path Length;Counts",  100, 0, 100}, "path_length", "accidental_weight");

	auto h1_ksphi = rdf_cut2.Histo1D({"h1_ksphi", ";-t;Counts",  180, -3.14, 3.14}, "ks_phi", "accidental_weight");
	auto h1_ksphi_sb = rdf_cut2_sb.Histo1D({"h1_ksphi_sb", ";-t;Counts",  180, -3.14, 3.14}, "ks_phi", "accidental_weight");

	auto h2_mpipi_ChiSqNdf = rdfChiSqNdf_cut.Histo2D({"h2_mpipi_ChiSqNdf", ";M(#pi^{+}#pi^{-});#chi^{2]/ndf", 100, 0.30, 0.70, 500, 0.0, 5.0}, "mpipi", "chisq_ndf", "accidental_weight");
	auto h2_mpipi_FlightSignificance = rdfFlightSignificance_cut.Histo2D({"h2_mpipi_FlightSignificance", ";M(#pi^{+}#pi^{-});Flight Significance", 100, 0.30, 0.70, 180, 2.0, 20.0}, "mpipi", "fs", "accidental_weight");
	auto h2_mpipi_NumUnusedTracks = rdfNumUnusedTracks_cut.Histo2D({"h2_mpipi_NumUnusedTracks", ";M(#pi^{+}#pi^{-});Flight Significance", 100, 0.30, 0.70, 4, 0.0, 4.0}, "mpipi", "num_unused_tracks", "accidental_weight");
	auto h2_mpipi_NumUnusedShowers = rdfNumUnusedShowers_cut.Histo2D({"h2_mpipi_NumUnusedShowers", ";M(#pi^{+}#pi^{-});Flight Significance", 100, 0.30, 0.70, 10, 0.0, 10.0}, "mpipi", "num_unused_showers", "accidental_weight");

	auto h2_mkskl_mksp = rdf_cut.Histo2D({"h2_mkskl_mksp", ";M(K_{S}K_{L});M(K_{S}p)", 200, 0.98, 2.98, 100, 1.0, 4.0}, "mkskl", "mksp", "accidental_weight");
	auto h2_mkskl_mklp = rdf_cut.Histo2D({"h2_mkskl_mklp", ";M(K_{S}K_{L});M(K_{L}p)", 200, 0.98, 2.98, 100, 1.0, 4.0}, "mkskl", "mklp", "accidental_weight");
	auto h2_mkskl_mpipp = rdf_cut.Histo2D({"h2_mkskl_mpipp", ";M(K_{S}K_{L});M(#pi^{+}p)", 200, 0.98, 2.98, 100, 1.0, 4.0}, "mkskl", "mpipp", "accidental_weight");
	auto h2_mkskl_mpimp = rdf_cut.Histo2D({"h2_mkskl_mpimp", ";M(K_{S}K_{L});M(#pi^{-}p)", 200, 0.98, 2.98, 100, 1.0, 4.0}, "mkskl", "mpimp", "accidental_weight");

	auto h2_mkskl_coshx = rdf_cut.Histo2D({"h2_mkskl_coshx", ";M(K_{S}K_{L});cos(#theta_{hel})", 50, 1.00, 2.0, 40, -1.0, 1.0}, "mkskl", "cos_hel_ks", "accidental_weight");
	auto h2_mkskl_coshx_sb = rdf_cut_sb.Histo2D({"h2_mkskl_coshx_sb", ";M(K_{S}K_{L});cos(#theta_{hel})", 50, 1.00, 2.0, 40, -1.0, 1.0}, "mkskl", "cos_hel_ks", "accidental_weight");
	auto h2_mkskl_coshx2 = rdf_cut_x.Filter("amptools_dat == true").Histo2D({"h2_mkskl_coshx2", ";M(K_{S}K_{L});cos(#theta_{hel})", 50, 1.00, 2.0, 40, -1.0, 1.0}, "mkskl", "cos_hel_ks", "Weight");
	auto h2_mkskl_coshx_sb2 = rdf_cut_x.Filter("amptools_bkg == true").Histo2D({"h2_mkskl_coshx_sb2", ";M(K_{S}K_{L});cos(#theta_{hel})", 50, 1.00, 2.0, 40, -1.0, 1.0}, "mkskl", "cos_hel_ks", "Weight");

	// auto h2_mpipi_MissingEnergy = rdf_cut3.Filter("mandel_t < 0.5").Histo2D({"h2_mpipi_MissingEnergy", ";M(#pi^{+}#pi^{-});Missing Energy (GeV)", 150, 0.40, 0.55, 100, 0.0, 10.}, "mpipi", "missing_energy", "accidental_weight");
	// auto h2_mpipi_MissingEnergy2 = rdf_cut3.Filter("mandel_t > 0.5").Histo2D({"h2_mpipi_MissingEnergy2", ";M(#pi^{+}#pi^{-});Missing Energy (GeV)", 150, 0.40, 0.55, 100, 0.0, 10.}, "mpipi", "missing_energy", "accidental_weight");

	// auto h2_kskl_MissingEnergy = rdf_cut.Filter("mandel_t < 0.5").Histo2D({"h2_kskl_MissingEnergy", ";M(K_{S}K_{L});Missing Energy (GeV)", 100, 0.98, 1.98, 100, 0.0, 10.}, "mkskl", "missing_energy", "accidental_weight");
	// auto h2_kskl_MissingEnergy2 = rdf_cut.Filter("mandel_t > 0.5").Histo2D({"h2_kskl_MissingEnergy2", ";M(K_{S}K_{L});Missing Energy (GeV)", 100, 0.98, 1.98, 100, 0.0, 10.}, "mkskl", "missing_energy", "accidental_weight");

	// auto h2_MissingEnergy_mandelt = rdf_cut2.Histo2D({"h2_MissingEnergy_mandelt", ";Missing Energy (GeV);-t (GeV^{2})", 100, 0.0, 10., 50, 0, 1}, "missing_energy", "mandel_t", "accidental_weight");

	auto h2_fs_chisq = rdf_cut4.Histo2D({"h2_fs_chisq", ";FS;#chi^2", 70, 3, 10, 60, 0, 6}, "fs", "chisq_ndf", "accidental_weight");

	cout <<" "<< endl;
	
	//5.) Write everything to a file:
	cout <<"Write results to file: " << opf_name << endl;

	cosHX_mkskl->Write();
	cosHX_mkskl_sb->Write();

	cosHX->Write();
	cosHX_sb->Write();

	van_hove->Write();
	kscosThetaCM->Write();
	cos_mkskl->Write();

	im_miss2->Write();
	im_miss2_sb->Write();

	miss_energy->Write();
	miss_energy_sb->Write();

	im_kskl->Write();
	im_kskl_sb->Write();

	im_kskl2->Write();
	im_kskl2_sb->Write();
	im_kskl2_sb2->Write();
	im_kskl2_sb3->Write();
	im_kskl2_sb4->Write();

	im_ksp->Write();
	im_klp->Write();
	im_pipp->Write();
	im_pimp->Write();
	im_miss->Write();
	im_pipi->Write();
	// im_pipi2->Write();
	// im_pipi3->Write();
	// im_pipi4->Write();
	// im_pipi5->Write();

	im_ksp_sb->Write();
	im_klp_sb->Write();
	im_pipp_sb->Write();
	im_pimp_sb->Write();

	h1_p_z->Write();
	h1_p_z_sb->Write();

	h1_t->Write();
	h1_t_sb->Write();
	h2_mpipi_t->Write();

	h1_path_length->Write();
	h1_path_length_sb->Write();

	h1_ksphi->Write();
	h1_ksphi_sb->Write();

	h2_mpipi_ChiSqNdf->Write();
	h2_mpipi_FlightSignificance->Write();
	h2_mpipi_NumUnusedTracks->Write();
	h2_mpipi_NumUnusedShowers->Write();
	
	h2_mkskl_mksp->Write();
	h2_mkskl_mklp->Write();
	h2_mkskl_mpipp->Write();
	h2_mkskl_mpimp->Write();

	h2_mkskl_coshx->Write();
	h2_mkskl_coshx_sb->Write();
	h2_mkskl_coshx2->Write();
	h2_mkskl_coshx_sb2->Write();

	// h2_MissingEnergy_mandelt->Write();

	// h2_mpipi_MissingEnergy->Write();
	// h2_mpipi_MissingEnergy2->Write();

	// h2_kskl_MissingEnergy->Write();
	// h2_kskl_MissingEnergy2->Write();

	h2_fs_chisq->Write();

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
		if(it->first == change_cut.first)	cuts += change_cut.second + " && ";
		else								cuts += it->second + " && ";
	}
	cuts.erase(cuts.size()-4, 4); // remove last " && "

	return cuts;
}
