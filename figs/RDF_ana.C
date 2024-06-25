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
				.Define("ksphi", "ks_p4.Phi()*180/3.14159265359")
				.Define("mpipp", "(pip_p4_kin + p_p4_kin).M()").Define("mpimp", "(pim_p4_kin + p_p4_kin).M()");

	// 3.2) Make list of nominal cuts
	std::map<std::string, std::string> cuts_list = {
		{"mkskl", "mkskl > 1.10 && mkskl < 2.00"},
		{"mmiss", "missing_mass > 0.3 && missing_mass < 0.7"},
		{"mandel_t", "mandel_t > 0.20 && mandel_t < 0.5"},
		{"flight_significance", "flight_significance > 6"},
		{"chisq", "chisq_ndf < 2"},
		{"ntracks", "num_unused_tracks == 0"},
		{"nshowers", "num_unused_showers < 3"},
		{"proton_z_vertex", "proton_z_vertex > 52 && proton_z_vertex < 78"},
		{"beam_energy", "beam_energy > 8.2 && beam_energy < 8.8"} };
	
	string cuts = "";
	string signal = "(mpipi > 0.48 && mpipi < 0.52)";
	string sideband = "((mpipi > 0.44 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.56))";
	// string sideband = "((mpipi > 0.44 && mpipi < 0.45) || (mpipi > 0.55 && mpipi < 0.56))";

	//3.3)Now apply cuts on the newly defined variables:
	cuts = set_cuts(cuts_list, {"mkskl", ""});
	auto rdf_cut = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"mandel_t", "mandel_t > 0.2 && mandel_t < 1"});
	auto rdf_cut2 = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"mandel_t", "mandel_t > 0.20 && mandel_t < 0.5"});
	auto rdfBaryons = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"mandel_t", ""});
	auto rdfMandelt = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"mmiss", ""});
	auto rdfMissingMass_cut = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"chisq", ""});
	auto rdfChiSqNdf_cut = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"flight_significance", ""});
	auto rdfFlightSignificance_cut = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"ntracks", ""});
	auto rdfUnusedTracks_cut = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"nshowers", ""});
	auto rdfUnusedShowers_cut = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"proton_z_vertex", ""});
	auto rdfProtonZVertex_cut = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list);
	auto rdfMpipi_cut = rdf_variables.Filter(cuts);

	cout <<"...done!"<< endl;
	cout <<" "<< endl;
	
	//4.) Define Histograms:
	
	cout <<"Set up histograms..."<< endl;
	
	//4.1) Histograms
	auto im_kskl = rdf_cut.Filter(signal).Histo1D({"im_kskl", ";M(K_{S}K_{L});Counts", 50, 1.10, 2.10}, "mkskl", "accidental_weight");
	auto im_kskl_sb = rdf_cut.Filter(sideband).Histo1D({"im_kskl_sb", ";M(K_{S}K_{L});Counts", 50, 1.10, 2.10}, "mkskl", "accidental_weight");

	auto im_kskl_BaryonCut1 = rdf_cut.Filter(signal+"&& mklp < 2.0").Histo1D({"im_kskl_BaryonCut1", ";M(K_{S}K_{L});Counts", 50, 1.10, 2.10}, "mkskl", "accidental_weight");
	auto im_kskl_BaryonCut1_sb = rdf_cut.Filter(sideband+"&& mklp < 2.0").Histo1D({"im_kskl_BaryonCut1_sb", ";M(K_{S}K_{L});Counts", 50, 1.10, 2.10}, "mkskl", "accidental_weight");

	auto h1_mandelt = rdfMandelt.Filter(signal).Histo1D({"h1_mandelt", ";-t (GeV^{2});Counts", 100, 0, 1.0}, "mandel_t", "accidental_weight");
	auto h1_mandelt_sb = rdfMandelt.Filter(sideband).Histo1D({"h1_mandelt_sb", ";-t (GeV^{2});Counts", 100, 0, 1.0}, "mandel_t", "accidental_weight");
	
	auto h1_mandeltp = rdfMandelt.Filter(signal).Histo1D({"h1_mandeltp", ";-t' (GeV^{2});Counts", 100, 0, 1.0}, "mandel_tp", "accidental_weight");
	auto h1_mandeltp_sb = rdfMandelt.Filter(sideband).Histo1D({"h1_mandeltp_sb", ";-t' (GeV^{2});Counts", 100, 0, 1.0}, "mandel_tp", "accidental_weight");

	auto h1_Mksp = rdfBaryons.Filter(signal).Histo1D({"h1_Mksp", ";M(K_{S}p) (GeV);Counts", 75, 1.4, 3.7}, "mksp", "accidental_weight");
	auto h1_Mksp_sb = rdfBaryons.Filter(sideband).Histo1D({"h1_Mksp_sb", ";M(K_{S}p)  (GeV^{2});Counts", 75, 1.4, 3.7}, "mksp", "accidental_weight");

	auto h1_Mklp = rdfBaryons.Filter(signal).Histo1D({"h1_Mklp", ";M(K_{L}p) (GeV);Counts", 75, 1.4, 3.7}, "mklp", "accidental_weight");
	auto h1_Mklp_sb = rdfBaryons.Filter(sideband).Histo1D({"h1_Mklp_sb", ";M(K_{L}p)  (GeV^{2});Counts", 75, 1.4, 3.7}, "mklp", "accidental_weight");

	// auto h1_Mksp2 = rdfMandelt.Filter(signal+" && mandel_t > 0.20 && mandel_t < 0.5").Histo1D({"h1_Mksp2", ";M(K_{S}p) (GeV);Counts", 100, 1.0, 4.0}, "mksp", "accidental_weight");
	// auto h1_Mksp2_sb = rdfMandelt.Filter(sideband+" && mandel_t > 0.20 && mandel_t < 0.5").Histo1D({"h1_Mksp2_sb", ";M(K_{S}p)  (GeV^{2});Counts", 100, 1.0, 4.0}, "mksp", "accidental_weight");

	// auto h1_Mklp2 = rdfMandelt.Filter(signal+" && mandel_t > 0.2 && mandel_t < 0.5").Histo1D({"h1_Mklp2", ";M(K_{L}p) (GeV);Counts", 100, 1.0, 4.0}, "mklp", "accidental_weight");
	// auto h1_Mklp2_sb = rdfMandelt.Filter(sideband+" && mandel_t > 0.2 && mandel_t < 0.5").Histo1D({"h1_Mklp2_sb", ";M(K_{L}p)  (GeV^{2});Counts", 100, 1.0, 4.0}, "mklp", "accidental_weight");

	auto h1_Mpipp = rdfBaryons.Filter(signal).Histo1D({"h1_Mpipp", ";M(#pi^{+}p) (GeV);Counts", 75, 1.0, 3.5}, "mpipp", "accidental_weight");
	auto h1_Mpipp_sb = rdfBaryons.Filter(sideband).Histo1D({"h1_Mpipp_sb", ";M(#pi^{+}p)  (GeV^{2});Counts", 75, 1.0, 3.5}, "mpipp", "accidental_weight");

	// auto h1_Mpipp2 = rdfMandelt.Filter(signal+" && mandel_t > 0.2 && mandel_t < 0.5").Histo1D({"h1_Mpipp2", ";M(#pi^{+}p) (GeV);Counts", 100, 1.0, 4.0}, "mpipp", "accidental_weight");
	// auto h1_Mpipp2_sb = rdfMandelt.Filter(sideband+" && mandel_t > 0.2 && mandel_t < 0.5").Histo1D({"h1_Mpipp2_sb", ";M(#pi^{+}p)  (GeV^{2});Counts", 100, 1.0, 4.0}, "mpipp", "accidental_weight");

	auto h1_Mpimp = rdfBaryons.Filter(signal).Histo1D({"h1_Mpimp", ";M(#pi^{-}p) (GeV);Counts", 100, 1.0, 3.5}, "mpimp", "accidental_weight");
	auto h1_Mpimp_sb = rdfBaryons.Filter(sideband).Histo1D({"h1_Mpimp_sb", ";M(#pi^{-}p)  (GeV^{2});Counts", 100, 1.0, 3.5}, "mpimp", "accidental_weight");

	// auto h1_Mpimp2 = rdfMandelt.Filter(signal+" && mandel_t > 0.2 && mandel_t < 0.5").Histo1D({"h1_Mpimp2", ";M(#pi^{-}p) (GeV);Counts", 100, 1.0, 4.0}, "mpimp", "accidental_weight");
	// auto h1_Mpimp2_sb = rdfMandelt.Filter(sideband+" && mandel_t > 0.2 && mandel_t < 0.5").Histo1D({"h1_Mpimp2_sb", ";M(#pi^{-}p)  (GeV^{2});Counts", 100, 1.0, 4.0}, "mpimp", "accidental_weight");

	auto h1_MissingMass = rdfMissingMass_cut.Filter(signal).Histo1D({"h1_MissingMass", ";Missing Mass (GeV);Counts",  100, 0, 1}, "missing_mass", "accidental_weight");
	auto h1_MissingMass_sb = rdfMissingMass_cut.Filter(sideband).Histo1D({"h1_MissingMass_sb", ";Missing Mass (GeV);Counts",  100, 0, 1}, "missing_mass", "accidental_weight");

	auto h1_fs = rdfFlightSignificance_cut.Filter(signal).Histo1D({"h1_FS", ";Flight Significance (#sigma);Counts",  35, 3, 10}, "fs", "accidental_weight");
	auto h1_fs_sb = rdfFlightSignificance_cut.Filter(sideband).Histo1D({"h1_FS_sb", ";Flight Significance (#sigma);Counts",  35, 3, 10}, "fs", "accidental_weight");

	auto h1_ChisqNdf = rdfChiSqNdf_cut.Filter(signal).Histo1D({"h1_ChisqNdf", ";#chi^{2}/ndf;Counts",  60, 0, 6}, "chisq_ndf", "accidental_weight");
	auto h1_ChisqNdf_sb = rdfChiSqNdf_cut.Filter(sideband).Histo1D({"h1_ChisqNdf_sb", ";#chi^{2}/ndf;Counts",  60, 0, 6}, "chisq_ndf", "accidental_weight");

	auto h1_UnusedTracks = rdfUnusedTracks_cut.Filter(signal).Histo1D({"h1_UnusedTracks", ";Number of Unused Tracks;Counts",  4, 0, 4}, "num_unused_tracks", "accidental_weight");
	auto h1_UnusedTracks_sb = rdfUnusedTracks_cut.Filter(sideband).Histo1D({"h1_UnusedTracks_sb", ";Number of Unused Tracks;Counts",  4, 0, 4}, "num_unused_tracks", "accidental_weight");

	auto h1_UnusedShowers = rdfUnusedShowers_cut.Filter(signal).Histo1D({"h1_UnusedShowers", ";Number of Unused Showers;Counts",  10, 0, 10}, "num_unused_showers", "accidental_weight");
	auto h1_UnusedShowers_sb = rdfUnusedShowers_cut.Filter(sideband).Histo1D({"h1_UnusedShowers_sb", ";Number of Unused Showers;Counts",  10, 0, 10}, "num_unused_showers", "accidental_weight");

	auto h1_ProtonZVertex = rdfProtonZVertex_cut.Filter(signal).Histo1D({"h1_ProtonZVertex", ";Proton Z vertex (cm);Counts",  70, 30, 100}, "proton_z_vertex", "accidental_weight");
	auto h1_ProtonZVertex_sb = rdfProtonZVertex_cut.Filter(sideband).Histo1D({"h1_ProtonZVertex_sb", ";Proton Z-vertex (cm);Counts",  70, 30, 100}, "proton_z_vertex", "accidental_weight");

	auto h1_Mpipi = rdfMpipi_cut.Histo1D({"h1_Mpipi", ";M(#pi^{+}#pi^{-});Counts",  100, 0.3, 0.7}, "mpipi", "accidental_weight");

	auto h2_mkskl_cosHel = rdf_cut2.Filter(signal).Histo2D({"h2_mkskl_cosHel", ";M(K_{S}K_{L});cos(#theta_{hel});Counts", 20, 1.10, 2.0, 20, -1, 1}, "mkskl", "cos_hel_ks", "accidental_weight");
	auto h2_mkskl_cosHel_sb = rdf_cut2.Filter(sideband).Histo2D({"h2_mkskl_cosHel_sb", ";M(K_{S}K_{L});cos(#theta_{hel});Counts", 20, 1.10, 2.0, 20, -1, 1}, "mkskl", "cos_hel_ks", "accidental_weight");

	auto h2_mkskl_cosHel2 = rdf_cut2.Filter(signal+" && mklp > 2.0").Histo2D({"h2_mkskl_cosHel2", ";M(K_{S}K_{L});cos(#theta_{hel});Counts", 40, 1.10, 2.0, 20, -1, 1}, "mkskl", "cos_hel_ks", "accidental_weight");
	auto h2_mkskl_cosHel3 = rdf_cut2.Filter(signal+" && mksp > 2.0").Histo2D({"h2_mkskl_cosHel3", ";M(K_{S}K_{L});cos(#theta_{hel});Counts", 40, 1.10, 2.0, 20, -1, 1}, "mkskl", "cos_hel_ks", "accidental_weight");

	auto h2_mkskl_phiHel = rdf_cut2.Filter(signal).Histo2D({"h2_mkskl_phiHel", ";M(K_{S}K_{L});#phi_{hel};Counts", 20, 1.10, 2.00, 20, -TMath::Pi(), TMath::Pi()}, "mkskl", "phi_hel_ks", "accidental_weight");
	auto h2_mkskl_phiHel_sb = rdf_cut2.Filter(sideband).Histo2D({"h2_mkskl_phiHel_sb", ";M(K_{S}K_{L});#phi_{hel};Counts", 20, 1.10, 2.00, 20, -TMath::Pi(), TMath::Pi()}, "mkskl", "phi_hel_ks", "accidental_weight");

	// make 2D hist of mkskl vs t and mkskl vs t'
	auto h2_mkskl_t = rdfMandelt.Filter(signal).Histo2D({"h2_mkskl_t", ";M(K_{S}K_{L});-t (GeV^{2});Counts", 20, 1.10, 2.0, 20, 0, 1.0}, "mkskl", "mandel_t", "accidental_weight");
	auto h2_mkskl_tp = rdfMandelt.Filter(signal).Histo2D({"h2_mkskl_tp", ";M(K_{S}K_{L});-t' (GeV^{2});Counts", 20, 1.10, 2.0, 20, 0, 1.0}, "mkskl", "mandel_tp", "accidental_weight");

	auto h2_mkskl_t2 = rdfMandelt.Filter(signal).Histo2D({"h2_mkskl_t2", ";M(K_{S}K_{L});-t (GeV^{2});Counts", 50, 1.10, 2.6, 80, 0, 20.0}, "mkskl", "mandel_t", "accidental_weight");
	auto h2_mkskl_tp2 = rdfMandelt.Filter(signal).Histo2D({"h2_mkskl_tp2", ";M(K_{S}K_{L});-t' (GeV^{2});Counts", 50, 1.10, 2.6, 80, 0, 20.0}, "mkskl", "mandel_tp", "accidental_weight");

	cout <<" "<< endl;
	
	//5.) Write everything to a file:
	cout <<"Write results to file: " << opf_name << endl;

	im_kskl->Write();
	im_kskl_sb->Write();

	im_kskl_BaryonCut1->Write();
	im_kskl_BaryonCut1_sb->Write();

	h1_mandelt->Write();
	h1_mandelt_sb->Write();

	h1_mandeltp->Write();
	h1_mandeltp_sb->Write();

	h1_Mksp->Write();
	h1_Mksp_sb->Write();

	// h1_Mksp2->Write();
	// h1_Mksp2_sb->Write();

	h1_Mklp->Write();
	h1_Mklp_sb->Write();

	// h1_Mklp2->Write();
	// h1_Mklp2_sb->Write();

	h1_Mpipp->Write();
	h1_Mpipp_sb->Write();

	// h1_Mpipp2->Write();
	// h1_Mpipp2_sb->Write();

	h1_Mpimp->Write();
	h1_Mpimp_sb->Write();

	// h1_Mpimp2->Write();
	// h1_Mpimp2_sb->Write();

	h1_MissingMass->Write();
	h1_MissingMass_sb->Write();

	h1_fs->Write();
	h1_fs_sb->Write();

	h1_ChisqNdf->Write();
	h1_ChisqNdf_sb->Write();

	h1_UnusedTracks->Write();
	h1_UnusedTracks_sb->Write();

	h1_UnusedShowers->Write();
	h1_UnusedShowers_sb->Write();

	h1_ProtonZVertex->Write();
	h1_ProtonZVertex_sb->Write();

	h1_Mpipi->Write();

	h2_mkskl_cosHel->Write();
	h2_mkskl_cosHel_sb->Write();

	h2_mkskl_cosHel2->Write();
	h2_mkskl_cosHel3->Write();

	h2_mkskl_phiHel->Write();
	h2_mkskl_phiHel_sb->Write();

	h2_mkskl_t->Write();
	h2_mkskl_tp->Write();

	h2_mkskl_t2->Write();
	h2_mkskl_tp2->Write();

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