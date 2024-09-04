void plotBinsSDME(string fit_name = "9bins_1.0t", int nBins = 9) {
	gStyle->SetOptStat(0);

	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.03);
	gStyle->SetPadBottomMargin(0.20);
	gStyle->SetPadLeftMargin(0.16);

	gStyle->SetTitleBorderSize(0);
	
	gStyle->SetTitleFont(132);
	gStyle->SetTitleSize(1.00);
	gStyle->SetTitleOffset(1.2);
	
	gStyle->SetLabelSize(0.07,"XY");
	gStyle->SetLabelOffset(0.03,"X");  
	gStyle->SetLabelOffset(0.01,"Y");  
	
	gStyle->SetTitleSize(0.07,"XY");
	gStyle->SetTitleOffset(1.0,"X");
	gStyle->SetTitleOffset(1.0,"Y");

	// gStyle->SetMarkerStyle(8);
	// gStyle->SetMarkerSize(1.0);

	vector<string> subdirs = {"REACTION_sp17_000", "REACTION_sp17_045", "REACTION_sp17_090", "REACTION_sp17_135"};
	vector<string> vars = {"cosTheta", "phi", "Phi", "psi"};
	string fname;
	char text[100];

	TH1F *h1[4], *h2[4], *h3[4];
	TFile *inf;

	if(gSystem->AccessPathName("figs_sdmeAngles/")) {
		std::cout << "Making directory called bins" << std::endl;
		gSystem->Exec("mkdir figs_sdmeAngles/");
	}

	for(int iBin = 0; iBin < nBins; iBin++) {
		TCanvas *c = new TCanvas("c", "c", 150, 100);
		c->Divide(2, 2);

		for(int ivar = 0; ivar < vars.size(); ivar++) {
			c->cd(ivar + 1);
			// if(ivar == 0) gPad->SetFillColor(kRed);
			// if(ivar == 1) gPad->SetFillColor(kViolet);
			// if(ivar == 2) gPad->SetFillColor(kBlue);
			// if(ivar == 3) gPad->SetFillColor(kGreen);

			fname = fit_name+"/bin_"+to_string(iBin)+"/phi1020_plot.root";

			if(gSystem->AccessPathName(fname.c_str())) {
				cout << fname << " does not exist." << endl;
				continue;
			}

			cout << "Get histogram from file "+fname << endl;
			inf = TFile::Open(fname.c_str(), "READ");

			for(auto subdir : subdirs) {
				if(inf->GetDirectory(subdir.c_str()) == NULL) {
					cout << subdir << " does not exist." << endl;
					continue;
				}
				inf->cd(subdir.c_str());

				if(subdir == subdirs[0]) {
					h1[ivar] = (TH1F*)inf->Get((subdir+"/"+vars[ivar]+"dat").c_str());
					h2[ivar] = (TH1F*)inf->Get((subdir+"/"+vars[ivar]+"acc").c_str());
					h3[ivar] = (TH1F*)inf->Get((subdir+"/"+vars[ivar]+"bkg").c_str());
				}
				else {
					h1[ivar]->Add((TH1F*)inf->Get((subdir+"/"+vars[ivar]+"dat").c_str()));
					h2[ivar]->Add((TH1F*)inf->Get((subdir+"/"+vars[ivar]+"acc").c_str()));
					h3[ivar]->Add((TH1F*)inf->Get((subdir+"/"+vars[ivar]+"bkg").c_str()));
				}
			}

			if(h1[ivar] == NULL || h2[ivar] == NULL || h3[ivar] == NULL) {
				cout << "h1, h2, or h3 is NULL" << endl;
				continue;
			}

			h1[ivar]->SetMarkerSize(0.1);
			h1[ivar]->SetMarkerStyle(8);
			h2[ivar]->SetFillColorAlpha(kGreen, 0.5);
			h3[ivar]->SetFillColorAlpha(kRed, 0.5);

			THStack *hs = new THStack("hs", "");
			hs->Add(h3[ivar]);
			hs->Add(h2[ivar]);
			
			hs->Draw("HIST");
			h1[ivar]->Draw("SAME");

			if(vars[ivar] == "cosTheta") {
				hs->GetXaxis()->SetTitle("cos(#theta)");
				sprintf(text, "Candidates / %.2f", h2[ivar]->GetBinWidth(1));
			}
			else if(vars[ivar] == "phi") {
				hs->GetXaxis()->SetTitle("#phi (deg)");
				sprintf(text, "Candidates / %.0f (deg)", h2[ivar]->GetBinWidth(1));
			}
			else if(vars[ivar] == "Phi") {
				hs->GetXaxis()->SetTitle("#Phi (deg)");
				sprintf(text, "Candidates / %.0f (deg)", h2[ivar]->GetBinWidth(1));
			}
			else if(vars[ivar] == "psi") {
				hs->GetXaxis()->SetTitle("#Phi - #phi (deg)");
				sprintf(text, "Candidates / %.0f (deg)", h2[ivar]->GetBinWidth(1));
			}
			hs->GetYaxis()->SetTitle(text);
		}

		c->SaveAs(("figs_sdmeAngles/bin_"+to_string(iBin)+".pdf").c_str());
	}
}
