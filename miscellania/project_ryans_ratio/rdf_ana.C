void rdf_ana(Int_t n_threads,string inf_name, string opf_name, Bool_t show_cut_report) {	
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
	auto rdf_variables = rdf.Define("fs", "flight_significance").Define("mmiss", "missing_mass")
				.Define("mksp", "(ks_p4 + p_p4_kin).M()").Define("mklp", "(kl_p4 + p_p4_kin).M()")
				.Define("mpipp", "(pip_p4_kin + p_p4_kin).M()").Define("mpimp", "(pim_p4_kin + p_p4_kin).M()")
				.Define("mkskl_2", "mkskl + 0.07");

	//3.2)Now apply cuts on the newly defined variables:
	
	auto rdf_cut = rdf_variables.Filter("mpipi > 0.48 && mpipi < 0.52 && mmiss > 0.4 && mmiss < 0.6 && fs > 6 && chisq_ndf < 2 && mandel_t > 0.2 && mandel_t < 0.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdf_cut2 = rdf_variables.Filter("((mpipi > 0.44 && mpipi < 0.46) || (mpipi > 0.54 && mpipi < 0.56)) && mmiss > 0.4 && mmiss < 0.6 && fs > 6 && chisq_ndf < 2 && mandel_t > 0.2 && mandel_t < 0.5 && num_unused_tracks == 0 && num_unused_showers < 3");
	auto rdfChiSqNdf_cut = rdf_variables.Filter("mmiss > 0.4 && mmiss < 0.6 && fs > 3 && mandel_t > 0.1 && mandel_t < 0.5 && num_unused_tracks == 0");
	auto rdfFlightSignificance_cut = rdf_variables.Filter("mmiss > 0.4 && mmiss < 0.6 && chisq_ndf < 4 && mandel_t > 0.1 && mandel_t < 0.5 && num_unused_tracks == 0");
	auto rdfNumUnusedTracks_cut = rdf_variables.Filter("mmiss > 0.4 && mmiss < 0.6 && chisq_ndf < 2 && fs > 6 && mandel_t > 0.1 && mandel_t < 0.5");
	auto rdfNumUnusedShowers_cut = rdf_variables.Filter("mmiss > 0.4 && mmiss < 0.6 && chisq_ndf < 4 && fs > 2 && mandel_t > 0.1 && mandel_t < 0.5 && num_unused_tracks == 0");

	cout <<"...done!"<< endl;
	cout <<" "<< endl;
	
	//4.) Define Histograms:
	
	cout <<"Set up histograms..."<< endl;
	
	//4.1) Histograms
	int nbins_phi = 50;
	int nbins_hmass = 80;
	int nbins = 130;
	double edges[nbins+1];
	for(int i = 0; i < nbins_phi; i++)	edges[i] = 1.0 + i*0.002;
	for(int i = 0; i < nbins_hmass+1; i++)	edges[nbins_phi+i] = 1.1 + i*0.015;
	for(int i = 0; i < nbins+1; i++)		cout << edges[i] << endl;
	auto im_kskl = rdf_cut.Histo1D({"im_kskl", ";M(K_{S}K_{L});Counts", nbins, edges}, "mkskl", "Weight");
	
	//auto im_kskl = rdf_cut.Histo1D({"im_kskl", ";M(K_{S}K_{L});Counts",  80, 0.98, 2.20}, "mkskl", "Weight");
	auto im_kskl_sb = rdf_cut2.Histo1D({"im_kskl_sb", ";M(K_{S}K_{L});Counts",  80, 0.98, 2.20}, "mkskl", "Weight");
	auto im_kskl2 = rdf_cut.Histo1D({"im_kskl2", ";M(K_{S}K_{L});Counts",  80, 0.98, 2.20}, "mkskl_2", "Weight");

	cout <<" "<< endl;
	
	//5.) Write everything to a file:
	cout <<"Write results to file: " << opf_name << endl;

	im_kskl->Write();
	im_kskl_sb->Write();
	im_kskl2->Write();

//	im_ksp->Write();
//	im_klp->Write();
//	im_pipp->Write();
//	im_pimp->Write();
//
//	h2_mpipi_ChiSqNdf->Write();
//	h2_mpipi_FlightSignificance->Write();
//	h2_mpipi_NumUnusedTracks->Write();
//	h2_mpipi_NumUnusedShowers->Write();
//	
//	h2_mkskl_mksp->Write();
//	h2_mkskl_mklp->Write();
//	h2_mkskl_mpipp->Write();
//	h2_mkskl_mpimp->Write();
//
//	h2_mkskl_coshx->Write();

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
