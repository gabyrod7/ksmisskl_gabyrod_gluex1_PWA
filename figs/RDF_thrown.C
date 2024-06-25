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
		{"mkskl", "mkskl > 1.10 && mkskl < 2.60"},
		{"mandel_t", "mandel_tp > 0.20 && mandel_tp < 0.5"},
		{"beam_energy", "beam_energy > 8.2 && beam_energy < 8.8"} };
	
	string cuts = "";

	//Check for energy and momentum balance:
	auto rdf_cut = rdf_variables.Filter("true");
	//auto rdf_cut = rdf_variables.Filter("mandel_t > 0.1 && mandel_t < 0.5");

	cuts = set_cuts(cuts_list, {"mandel_t", ""});
	auto rdfMandelt = rdf_variables.Filter(cuts);

	cout <<"...done!"<< endl;
	cout <<" "<< endl;
	
	//4.) Define Histograms:
	
	cout <<"Set up histograms..."<< endl;
	
	//4.1) Histograms
	auto im_kskl = rdf_cut.Filter("mandel_t > 0.20 && mandel_t < 0.50").Histo1D({"im_kskl", ";M(K_{S}K_{L});Counts",  50, 1.10, 2.10}, "mkskl");
	
	auto h1_mandelt = rdfMandelt.Histo1D({"h1_mandelt", ";M(K_{S}K_{L});Counts / 1 MeV", 100, 0.0, 1.00}, "mandel_t");
	auto h1_mandeltp = rdfMandelt.Histo1D({"h1_mandeltp", ";M(K_{S}K_{L});Counts / 1 MeV", 100, 0.0, 1.00}, "mandel_tp");

	auto h2_mkskl_cosHel = rdf_cut.Histo2D({"h2_mkskl_cosHel", ";M(K_{S}K_{L});cos(#theta_{hel});Counts", 20, 1.10, 2.0, 20, -1, 1}, "mkskl", "cos_hel_ks");
	auto h2_mkskl_phiHel = rdf_cut.Histo2D({"h2_mkskl_phiHel", ";M(K_{S}K_{L});#phi_{hel};Counts", 20, 1.10, 2.0, 20, -TMath::Pi(), TMath::Pi()}, "mkskl", "phi_hel_ks");
 
	cout <<" "<< endl;
	
	//5.) Write everything to a file:
	cout <<"Write results to file: RDF_Ana_Results.root ..."<< endl;

	im_kskl->Write();
	h1_mandelt->Write();
	h1_mandeltp->Write();

	h2_mkskl_cosHel->Write();
	h2_mkskl_phiHel->Write();

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