void plot_variables(string fit_name = "", string var = "", int nx = 3, int ny = 3, vector<string> subdirs = {"REACTION_sp17_000", "REACTION_sp17_045", "REACTION_sp17_090", "REACTION_sp17_135"});

void drawSDMEangles(string fit_name = "9bins_1.50t", int nx = 3, int ny = 3) {
	gStyle->SetPadTopMargin(0.08);
	gStyle->SetPadRightMargin(0.03);
	gStyle->SetPadBottomMargin(0.20);
	gStyle->SetPadLeftMargin(0.20);

	gStyle->SetTitleBorderSize(0);
	
	gStyle->SetTitleFont(132);
	gStyle->SetTitleSize(1.00);
	gStyle->SetTitleOffset(1.2);
	
	gStyle->SetLabelSize(0.06,"XY");
	gStyle->SetLabelOffset(0.01,"XY");  
	
	gStyle->SetTitleSize(0.08,"XY");
	gStyle->SetTitleOffset(1.3,"X");
	gStyle->SetTitleOffset(1.0,"Y");
	
	gStyle->SetMarkerStyle(8);
	gStyle->SetMarkerSize(1.5);

	plot_variables(fit_name, "M2K", nx, ny);
	plot_variables(fit_name, "cosTheta", nx, ny);
	plot_variables(fit_name, "phi", nx, ny);
	plot_variables(fit_name, "psi", nx, ny);
	plot_variables(fit_name, "p1mom", nx, ny);
	plot_variables(fit_name, "p2mom", nx, ny);
	plot_variables(fit_name, "pmom", nx, ny);
	plot_variables(fit_name, "t", nx, ny);
}

void plot_variables(string fit_name = "", string var = "", int nx = 3, int ny = 3, vector<string> subdirs = {"REACTION_sp17_000", "REACTION_sp17_045", "REACTION_sp17_090", "REACTION_sp17_135"}) {
	gStyle->SetOptStat(0);

	if(gSystem->AccessPathName("figs_sdmeAngles/"))
		gSystem->Exec("mkdir figs_sdmeAngles");

	if(nx == 0 || ny == 0) {
		cout << "nx and ny should both be larger than." << endl;
		exit(0);
	}

	TFile *inf;
	TH1F *h, *h1[nx][ny], *h2[nx][ny], *h3[nx][ny];

	TLatex latex;
	latex.SetTextSize(0.08);

	char text[100];
	int bin;
	double t, te, tlow, thigh;

	FILE *fin = fopen((fit_name+"/tBins.txt").c_str(),"r");

	TCanvas *c = new TCanvas();
	c->Divide(nx, ny);

	int ibin = 0;
	string fname;
	for(int i = 0; i < nx; i++) {
		for(int j = 0; j < ny; j++) {
			ibin = i*ny + j;
			c->cd(ibin + 1);

			fname = fit_name+"/bin_"+to_string(ibin)+"/phi1020_plot.root";

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
					h1[i][j] = (TH1F*)inf->Get((subdir+"/"+var+"dat").c_str());
					h2[i][j] = (TH1F*)inf->Get((subdir+"/"+var+"acc").c_str());
					h3[i][j] = (TH1F*)inf->Get((subdir+"/"+var+"bkg").c_str());
				}
				else {
					h1[i][j]->Add((TH1F*)inf->Get((subdir+"/"+var+"dat").c_str()));
					h2[i][j]->Add((TH1F*)inf->Get((subdir+"/"+var+"acc").c_str()));
					h3[i][j]->Add((TH1F*)inf->Get((subdir+"/"+var+"bkg").c_str()));
				}
			}

			if(h1[i][j] == NULL || h2[i][j] == NULL || h3[i][j] == NULL) {
				cout << "h1, h2, or h3 is NULL" << endl;
				continue;
			}
	
			h1[i][j]->Add(h3[i][j], -1);
	
			h2[i][j]->GetYaxis()->SetRangeUser(0, 1.3*h1[i][j]->GetMaximum());
			h2[i][j]->SetFillColor(kViolet);
			h2[i][j]->GetYaxis()->SetTitleSize(0.06);
			h2[i][j]->GetYaxis()->SetLabelSize(0.06);
			h2[i][j]->GetXaxis()->SetTitleSize(0.06);
			h2[i][j]->GetXaxis()->SetLabelSize(0.06);

			h1[i][j]->SetMarkerStyle(8);
			h1[i][j]->SetMarkerSize(0.6);

			h2[i][j]->Draw("HIST");
			h1[i][j]->Draw("SAME");	
	
			fscanf(fin, "%i,%lf,%lf", &bin, &t, &te);
			tlow = t - te;
			thigh = t + te;

			sprintf(text, "%.2f < t < %.2f GeV", tlow, thigh);
			h2[i][j]->SetTitle(text);
		} // end j loop
	} // end i loop
	c->SaveAs( ("figs_sdmeAngles/"+var+".png").c_str() );
	c->SaveAs( ("figs_sdmeAngles/"+var+".pdf").c_str() );

	// TF1 *f1 = new TF1("f1", "pol1", -0.8, 0.8);
	// for(int i = 0; i < nx; i++) {
	// 	for(int j = 0; j < ny; j++) {
	// 		if(h1[i][j] == NULL || h2[i][j] == NULL)	continue;

	// 		c->cd(i*ny + j + 1);

	// 		h1[i][j]->SetTitle("");
	// 		h1[i][j]->GetYaxis()->SetTitle("Data / Fit Result");
	// 		h1[i][j]->Divide(h2[i][j]);
	// 		h1[i][j]->GetYaxis()->SetRangeUser(0.5, 1.5);
	// 		h1[i][j]->Draw();

	// 		if(var == "cosTheta") {
	// 			h1[i][j]->Fit(f1, "RQ");

	// 			sprintf(text, "slope = %.2f #pm %.2f", f1->GetParameter(1), f1->GetParError(1));
	// 			if(var == "cosTheta")	latex.DrawLatex(-0.4, 1.3, text);
	// 		}
	// 	}
	// }
	// c->SaveAs( ("figs_sdmeAngles/ratio_"+var+".png").c_str() );
	// c->SaveAs( ("figs_sdmeAngles/ratio_"+var+".pdf").c_str() );
}
