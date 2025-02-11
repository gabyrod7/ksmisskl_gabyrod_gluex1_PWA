std::string set_cuts(std::map<std::string, std::string> cuts_list, std::pair<std::string, std::string> change_cut = {"", ""});

void RDF_thrown(Int_t n_threads,string inf_name, string opf_name, Bool_t show_cut_report) {
	cout<<" "<< endl;
	cout<<"Run RDataFrame Analysis on subset of gp -> KsKlp Spring 2017 data"<< endl;
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
   	cout << nEvents << endl; 

	TFile *out_file = new TFile(opf_name.c_str(),"RECREATE");

	// bool dIsMC = (my_chain.GetBranch("dIsMC") != NULL);

	//2.) Set the RDataFrame:
	ROOT::RDataFrame rdf(my_chain);
	
	cout <<"...done!"<< endl;
	cout <<" "<< endl;
	
	cout <<"Define variables and cuts..."<< endl;
	
	//3.) Define important variables and analysis conditions
	
	//3.1) Define some variables first:
	auto rdf_variables = rdf.Define("target", "TLorentzVector(0,0,0,0.938)");

	//3.2)Now apply cuts on the newly defined variables:
	std::map<std::string, std::string> cuts_list = {
		// {"mkskl", "mkskl > 1.10 && mkskl < 2.60"},
		{"mandel_tp", "mandel_tp > 0.20 && mandel_tp < 1.0"},
		{"beam_energy", "beam_energy > 8.2 && beam_energy < 8.8"}
	};
	
	string cuts = "";
	
	//Check for energy and momentum balance:
	cuts = set_cuts(cuts_list, {"", ""});
	auto rdf_cut = rdf_variables.Filter(cuts);

	cuts = set_cuts(cuts_list, {"mandel_tp", ""});
	auto rdfMandeltp = rdf_variables.Filter(cuts);

	cout <<"...done!"<< endl;
	cout <<" "<< endl;
	
	//4.) Define Histograms:
	
	cout <<"Set up histograms..."<< endl;
	
	//4.1) Histograms
	int nBins = 72;
	double xMin = 1.16, xMax = 2.60;
	auto im_kskl = rdf_cut.Histo1D({"im_kskl", ";M(K_{S}K_{L});Counts",  nBins, xMin, xMax}, "mkskl");\
	auto im_kskl_tp = rdf_cut.Histo2D({"im_kskl_tp", ";M(K_{S}K_{L});-t';Counts", nBins, xMin, xMax, 100, 0.0, 1.0}, "mkskl", "mandel_tp");

	auto im_kskl_mpipi1 = rdf_cut.Histo1D({"im_kskl_mpipi1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_mpipi2 = rdf_cut.Histo1D({"im_kskl_mpipi2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_mpipi3 = rdf_cut.Histo1D({"im_kskl_mpipi3", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_mmiss1 = rdf_cut.Histo1D({"im_kskl_mmiss1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_mmiss2 = rdf_cut.Histo1D({"im_kskl_mmiss2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	// auto im_kskl_baryonCut = rdf_cut.Filter("mksp > 2 && mklp > 2").Histo1D({"im_kskl_baryonCut", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_mandeltp1 = rdfMandeltp.Filter("mandel_tp > 0.2 && mandel_tp < 0.8").Histo1D({"im_kskl_mandel_tp1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_mandeltp2 = rdfMandeltp.Filter("mandel_tp > 0.15 && mandel_tp < 1.10").Histo1D({"im_kskl_mandel_tp2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_FS1 = rdf_cut.Histo1D({"im_kskl_FS1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_FS2 = rdf_cut.Histo1D({"im_kskl_FS2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_chisq1 = rdf_cut.Histo1D({"im_kskl_chisq1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_chisq2 = rdf_cut.Histo1D({"im_kskl_chisq2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_ntracks1 = rdf_cut.Histo1D({"im_kskl_ntracks1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_nshowers1 = rdf_cut.Histo1D({"im_kskl_nshowers1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_nshowers2 = rdf_cut.Histo1D({"im_kskl_nshowers2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_nshowers3 = rdf_cut.Histo1D({"im_kskl_nshowers3", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_proton_z_vertex1 = rdf_cut.Histo1D({"im_kskl_proton_z_vertex1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_proton_z_vertex2 = rdf_cut.Histo1D({"im_kskl_proton_z_vertex2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_beam_energy1 = rdf_cut.Filter("beam_energy > 8.2 && beam_energy < 8.3").Histo1D({"im_kskl_beam_energy1", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_beam_energy2 = rdf_cut.Filter("beam_energy > 8.3 && beam_energy < 8.4").Histo1D({"im_kskl_beam_energy2", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_beam_energy3 = rdf_cut.Filter("beam_energy > 8.4 && beam_energy < 8.5").Histo1D({"im_kskl_beam_energy3", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_beam_energy4 = rdf_cut.Filter("beam_energy > 8.5 && beam_energy < 8.6").Histo1D({"im_kskl_beam_energy4", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_beam_energy5 = rdf_cut.Filter("beam_energy > 8.6 && beam_energy < 8.7").Histo1D({"im_kskl_beam_energy5", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_beam_energy6 = rdf_cut.Filter("beam_energy > 8.7 && beam_energy < 8.8").Histo1D({"im_kskl_beam_energy6", ";M(K_{S}K_{L});Counts", nBins, xMin, xMax}, "mkskl");
	auto im_kskl_fitRange1 = rdf_cut.Histo1D({"im_kskl_fitRange1", ";M(K_{S}K_{L});Counts", 76, 1.14, 2.66}, "mkskl");
	auto im_kskl_fitRange2 = rdf_cut.Histo1D({"im_kskl_fitRange2", ";M(K_{S}K_{L});Counts", 67, 1.20, 2.54}, "mkskl");
	auto im_kskl_fitRange3 = rdf_cut.Histo1D({"im_kskl_fitRange3", ";M(K_{S}K_{L});Counts", 67, xMin, 2.5}, "mkskl");
	auto im_kskl_fitRange4 = rdf_cut.Histo1D({"im_kskl_fitRange4", ";M(K_{S}K_{L});Counts", 77, xMin, 2.7}, "mkskl");
	auto im_kskl_binning1 = rdf_cut.Histo1D({"im_kskl_binning1", ";M(K_{S}K_{L});Counts", 64, xMin, xMax}, "mkskl");
	auto im_kskl_binning2 = rdf_cut.Histo1D({"im_kskl_binning2", ";M(K_{S}K_{L});Counts", 100, xMin, xMax}, "mkskl");
	
	auto h1_mandeltp = rdfMandeltp.Histo1D({"h1_mandeltp", ";-t';Counts / 1 MeV", 85, 0.15, 1.00}, "mandel_tp");
 
	cout <<" "<< endl;
	
	//5.) Write everything to a file:
	cout <<"Write results to file: RDF_Ana_Results.root ..."<< endl;

	im_kskl->Write();
	im_kskl_tp->Write();

	im_kskl_mpipi1->Write();
	im_kskl_mpipi2->Write();
	im_kskl_mpipi3->Write();
	im_kskl_mmiss1->Write();
	im_kskl_mmiss2->Write();
	im_kskl_mandeltp1->Write();
	im_kskl_mandeltp2->Write();
	im_kskl_FS1->Write();
	im_kskl_FS2->Write();
	im_kskl_chisq1->Write();
	im_kskl_chisq2->Write();
	im_kskl_ntracks1->Write();
	im_kskl_nshowers1->Write();
	im_kskl_nshowers2->Write();
	im_kskl_nshowers3->Write();
	im_kskl_proton_z_vertex1->Write();
	im_kskl_proton_z_vertex2->Write();
	im_kskl_beam_energy1->Write();
	im_kskl_beam_energy2->Write();
	im_kskl_beam_energy3->Write();
	im_kskl_beam_energy4->Write();
	im_kskl_beam_energy5->Write();
	im_kskl_beam_energy6->Write();
	im_kskl_fitRange1->Write();
	im_kskl_fitRange2->Write();
	im_kskl_fitRange3->Write();
	im_kskl_fitRange4->Write();
	im_kskl_binning1->Write();
	im_kskl_binning2->Write();

	h1_mandeltp->Write();

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