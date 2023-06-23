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
				.Define("mksp", "(ks_p4 + p_p4_kin).M()").Define("mklp", "(kl_p4 + p_p4_kin).M()")
				.Define("ks_phi", "ks_p4_cm.Phi()").Define("p_z", "p_x4_kin.Z()")
				.Define("mpipp", "(pip_p4_kin + p_p4_kin).M()").Define("mpimp", "(pim_p4_kin + p_p4_kin).M()");

	//3.2)Now apply cuts on the newly defined variables:
	
	auto rdf_cut = rdf_variables.Filter("mpipi > 0.48 && mpipi < 0.52 && mmiss > 0.3 && mmiss < 0.7 && fs > 6 && chisq_ndf < 2 && mandel_t > 0.2 && mandel_t < 0.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdf_cut_sb = rdf_variables.Filter("((mpipi > 0.44 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.56)) && mmiss > 0.3 && mmiss < 0.7 && fs > 6 && chisq_ndf < 2 && mandel_t > 0.2 && mandel_t < 0.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	//auto rdf_cut = rdf_variables.Filter("(p_z > 83 && p_z < 86) && mpipi > 0.48 && mpipi < 0.52 && mmiss > 0.3 && mmiss < 0.7 && fs > 4 && chisq_ndf < 4 && mandel_t > 0.15&& mandel_t < 1.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	//auto rdf_cut_sb = rdf_variables.Filter("(p_z > 83 && p_z < 86) && ((mpipi > 0.44 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.56)) && mmiss > 0.3 && mmiss < 0.7 && fs > 4 && chisq_ndf < 4 && mandel_t > 0.15&& mandel_t < 1.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdf_cut2 = rdf_variables.Filter("mpipi > 0.48 && mpipi < 0.52 && mmiss > 0.4 && mmiss < 0.6 && fs > 6 && chisq_ndf < 2 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdf_cut2_sb = rdf_variables.Filter("((mpipi > 0.44 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.56)) && mmiss > 0.4 && mmiss < 0.6 && fs > 6 && chisq_ndf < 2 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdf_cut3 = rdf_variables.Filter("mmiss > 0.3 && mmiss < 0.7 && fs > 4 && chisq_ndf < 4 && num_unused_tracks == 0 && num_unused_showers < 3 && mandel_t > 0.15 && mandel_t < 1.5");
	auto rdfChiSqNdf_cut = rdf_variables.Filter("mmiss > 0.4 && mmiss < 0.6 && fs > 3 && mandel_t > 0.1 && mandel_t < 0.5 && num_unused_tracks == 0");
	auto rdfFlightSignificance_cut = rdf_variables.Filter("mmiss > 0.4 && mmiss < 0.6 && chisq_ndf < 4 && mandel_t > 0.1 && mandel_t < 0.5 && num_unused_tracks == 0");
	auto rdfNumUnusedTracks_cut = rdf_variables.Filter("mmiss > 0.4 && mmiss < 0.6 && chisq_ndf < 2 && fs > 6 && mandel_t > 0.1 && mandel_t < 0.5");
	auto rdfNumUnusedShowers_cut = rdf_variables.Filter("mmiss > 0.4 && mmiss < 0.6 && chisq_ndf < 2 && fs > 6 && mandel_t > 0.1 && mandel_t < 0.5 && num_unused_tracks == 0");

	cout <<"...done!"<< endl;
	cout <<" "<< endl;
	
	//4.) Define Histograms:
	
	cout <<"Set up histograms..."<< endl;
	
	//4.1) Histograms
	auto im_kskl = rdf_cut.Histo1D({"im_kskl", ";M(K_{S}K_{L});Counts", 50 , 1.00, 2.00}, "mkskl", "accidental_weight");
	auto im_kskl2 = rdf_cut.Filter("missing_energy > 2").Histo1D({"im_kskl2", ";M(K_{S}K_{L});Counts", 60 , 1.00, 2.20}, "mkskl", "accidental_weight");
	auto im_ksp = rdf_cut.Histo1D({"im_ksp", ";M(K_{S}p);Counts", 100, 1.00, 4.00}, "mksp", "accidental_weight");
	auto im_klp = rdf_cut.Histo1D({"im_klp", ";M(K_{L}p);Counts", 100, 1.00, 4.00}, "mklp", "accidental_weight");
	auto im_pipp = rdf_cut.Histo1D({"im_pipp", ";M(#pi^{+}p);Counts", 100, 1.00, 4.00}, "mpipp", "accidental_weight");
	auto im_pimp = rdf_cut.Histo1D({"im_pimp", ";M(#pi^{-}p);Counts", 100, 1.00, 4.00}, "mpimp", "accidental_weight");
	auto im_miss = rdf_cut.Histo1D({"im_miss", ";Missing Mass (GeV);Counts", 100, 0.00, 1.00}, "mmiss", "accidental_weight");
	auto im_pipi = rdf_cut3.Filter("mkskl > 1.005 & mkskl < 1.05").Histo1D({"im_pipi", ";M(#pi#pi) (GeV);Counts", 150, 0.40, 0.55}, "mpipi", "accidental_weight");

	auto miss_energy = rdf_cut.Histo1D({"miss_energy", ";Missing Energy (GeV);Counts", 100, 0.00, 10.00}, "missing_energy", "accidental_weight");
	auto miss_energy_sb = rdf_cut_sb.Histo1D({"miss_energy_sb", ";Missing Energy (GeV);Counts", 100 , 0.00, 10.0}, "missing_energy", "accidental_weight");

	auto im_kskl_sb = rdf_cut_sb.Histo1D({"im_kskl_sb", ";M(K_{S}p);Counts", 50 , 1.00, 2.00}, "mkskl", "accidental_weight");
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

	auto h2_mkskl_coshx = rdf_cut.Histo2D({"h2_mkskl_coshx", ";M(K_{S}K_{L});cos(#theta_{hel})", 200, 0.98, 2.98, 100, -1.0, 1.0}, "mkskl", "cos_hel_ks", "accidental_weight");

	cout <<" "<< endl;
	
	//5.) Write everything to a file:
	cout <<"Write results to file: " << opf_name << endl;

	miss_energy->Write();
	miss_energy_sb->Write();

	im_kskl->Write();
	im_kskl2->Write();
	im_ksp->Write();
	im_klp->Write();
	im_pipp->Write();
	im_pimp->Write();
	im_miss->Write();
	im_pipi->Write();

	im_kskl_sb->Write();
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


	auto chisq_sig = rdf_variables.Filter("mpipi > 0.48 && mpipi < 0.52").Histo1D({"chisq_sig", ";M(K_{S}p);Counts",  60, 0, 6}, "chisq_ndf", "accidental_weight");
	auto chisq_bkg = rdf_variables.Filter("(mpipi > 0.44 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.56)").Histo1D({"chisq_bkg", ";M(K_{S}p);Counts",  60, 0, 6}, "chisq_ndf", "accidental_weight");

	chisq_sig->Write();
	chisq_bkg->Write();

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
