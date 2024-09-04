void plot_angles(int nbins = 0, string fitName = "", vector<string> waves = {""}, string var = "", int nx = 2, int ny = 3);

void CanvasPartition(TCanvas *C, const Int_t Nx = 2, const Int_t Ny = 2, Float_t lMargin = 0.15, Float_t rMargin = 0.05, Float_t bMargin = 0.15, Float_t tMargin = 0.05);

void drawPWAangles(int nBins, string fitName, string waveSet) {
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

	vector<string> waves;
	vector<string> vars;

	stringstream ss(waveSet);

	string wave;
	while(getline(ss, wave, '_')) {
		waves.push_back(wave);
	}

	vars = {"cosTheta", "phi", "Phi", "psi"};
	// waves = {"Pm1+", "Pm1-", "Pp0+", "Pp0-", "Pp1+", "Pp1-"};
	for(string var : vars) {
		plot_angles(nBins, fitName, waves, var, 6, 5);
	}

	// vector<string> f_waves = {"Fm3+", "Fm3-", "Fm2+", "Fm2-", "Fm1+", "Fm1-", "Fp0+", "Fp0-", "Fp1+", "Fp1-", "Fp2+", "Fp2-", "Fp3+", "Fp3-"};

	// for(string f_wave : f_waves) {
	// 	waves = {"Pm1+", "Pm1-", "Pp0+", "Pp0-", "Pp1+", "Pp1-"};
	// 	waves.push_back(f_wave);

	// 	for(string var : vars) {
	// 		plot_angles(nBins, fitName, waves, var, 6, 5);
	// 	}
	// }
}

void plot_angles(int nbins = 0, string fitName = "", vector<string> waves = {""}, string var = "", int nx = 2, int ny = 3) {
	gStyle->SetOptStat(0);

	if(nbins == 0) {
		cout << "nbins = 0" << endl << "Enter number of bins to plot" << endl;
		exit(0);
	}

	if(waves[0] == "") {
		cout << "No waves given" << endl;
		exit(0);
	}

	if(gSystem->AccessPathName("figs_pwaAngles/")) {
		std::cout << "Making directory called bins" << std::endl;
		gSystem->Exec("mkdir figs_pwaAngles/");
	}

	if(gSystem->AccessPathName("root_files/")) {
		std::cout << "Making directory called bins" << std::endl;
		gSystem->Exec("mkdir root_files/");
	}

	TFile *inf, *opf;
	TH1F *h, *h1[nx][ny], *h2[nx][ny], *h3[nx][ny];

	TH1F *hmass = new TH1F("hmass", ";M(K_{S}K_{L});", 30, 1.1, 2.6);

	TLatex t;
	t.SetTextSize(0.08);

	string waveset = "";
	string x_title = "";
	double y_max = 0;

	vector<string> pols = {"000", "045", "090", "135"};

	for(auto wave : waves)	waveset += wave+"_";
	waveset.pop_back();

	opf = TFile::Open(("root_files/"+var+"_"+waveset+".root").c_str(), "RECREATE");

	TCanvas *c = new TCanvas();
	CanvasPartition(c, nx, ny, 0.04, 0.03, 0.06, 0.03);

	TPad *pad[nx][ny];

	int ibin = 0;
	string fname;
	for(int i = 0; i < nx; i++) {
		for(int j = 0; j < ny; j++) {
			c->cd(0);
 
			// Get the pads previously created.
			char pname[16];
			sprintf(pname,"pad_%i_%i",i,j);
			pad[i][j] = (TPad*) gROOT->FindObject(pname);
			pad[i][j]->Draw();
			pad[i][j]->SetFillStyle(4000);
			pad[i][j]->SetFrameFillStyle(4000);
			pad[i][j]->cd();

			h1[i][j] = new TH1F( ("h1"+to_string(i)+to_string(j)+waveset).c_str(), ";M(K_{S}K_{L});Intensity", 25, -1, 1);
			h2[i][j] = new TH1F( ("h2"+to_string(i)+to_string(j)+waveset).c_str(), ";M(K_{S}K_{L});Intensity", 25, -1, 1);
			h3[i][j] = new TH1F( ("h3"+to_string(i)+to_string(j)+waveset).c_str(), ";M(K_{S}K_{L});Intensity", 25, -1, 1);
	
			ibin = i*ny + j;
			fname = fitName+"/bin_"+to_string(ibin)+"/"+waveset+"/etapi_plot_"+waveset+".root";
			if(gSystem->AccessPathName(fname.c_str())) {
				cout << fname << " does not exisits" << endl;
				continue;
			}
			inf = TFile::Open( fname.c_str(), "READ");
			cout << "Get histogram from file "+fname << endl;

			for(int iPol = 0; iPol < pols.size(); iPol++) {
				h = (TH1F*)inf->Get( ("REACTION_gluex1_"+pols[iPol]+"_"+var+"dat").c_str() );
				if(iPol == 0) {
					h1[i][j]->SetBins(h->GetNbinsX(), h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
					h2[i][j]->SetBins(h->GetNbinsX(), h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
					h3[i][j]->SetBins(h->GetNbinsX(), h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
				}

				h1[i][j]->Add(h);
				h1[i][j]->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
		
				h = (TH1F*)inf->Get( ("REACTION_gluex1_"+pols[iPol]+"_"+var+"acc").c_str() );
				h2[i][j]->Add(h);
				h2[i][j]->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
		
				h = (TH1F*)inf->Get( ("REACTION_gluex1_"+pols[iPol]+"_"+var+"bkg").c_str() );
				h3[i][j]->Add(h);
				h3[i][j]->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
			} // end loop over polarization angles

			if(h1[i][j] == NULL || h2[i][j] == NULL)	continue;

			h1[i][j]->Add(h3[i][j], -1);
	
			// if(i == 0 && j == 0) {
				y_max = 1.4*h1[i][j]->GetMaximum();
			// }

			x_title = h->GetXaxis()->GetTitle();
			h2[i][j]->GetXaxis()->SetTitle( x_title.c_str() );
	
			h2[i][j]->GetYaxis()->SetRangeUser(0, y_max);
			h2[i][j]->SetFillColor(kViolet);
			h2[i][j]->GetYaxis()->SetTitleSize(0.06);
			h2[i][j]->GetYaxis()->SetLabelSize(0.06);
			h2[i][j]->GetXaxis()->SetTitleSize(0.06);
			h2[i][j]->GetXaxis()->SetLabelSize(0.06);

			h1[i][j]->SetMarkerStyle(8);
			h1[i][j]->SetMarkerSize(0.6);

			h2[i][j]->Draw("HIST");
			h1[i][j]->Draw("SAME");	
	
			// t.DrawLatex(0.4, h1[i][j]->GetMaximum(), ("bin_"+to_string(ibin)).c_str() );
			char bin[100];
			sprintf(bin, "M(K_{S}K_{L}) = %.2f-%.2f", hmass->GetBinLowEdge(ibin+1), hmass->GetBinLowEdge(ibin+2));
			t.DrawLatex(-0.9, 1.1*h1[i][j]->GetMaximum(), bin);

			opf->cd();
			h1[i][j]->Write( ("data_"+to_string(ibin)).c_str() );
			h2[i][j]->Write( ("acc_"+to_string(ibin)).c_str() );

			inf->Close();
		} // end j loop
	} // end i loop

	c->SaveAs( ("figs_pwaAngles/"+var+"_"+waveset+".pdf").c_str() );
}

void CanvasPartition(TCanvas *C, const Int_t Nx = 2, const Int_t Ny = 2, Float_t lMargin = 0.15, Float_t rMargin = 0.05, Float_t bMargin = 0.15, Float_t tMargin = 0.05) {
   if (!C) return;

   // Setup Pad layout:
   Float_t vSpacing = 0.0;
   Float_t vStep  = (1.- bMargin - tMargin - (Ny-1) * vSpacing) / Ny;

   Float_t hSpacing = 0.0;
   Float_t hStep  = (1.- lMargin - rMargin - (Nx-1) * hSpacing) / Nx;

   Float_t vposd,vposu,vmard,vmaru,vfactor;
   Float_t hposl,hposr,hmarl,hmarr,hfactor;

   for (Int_t i=0;i<Nx;i++) {

      if (i==0) {
         hposl = 0.0;
         hposr = lMargin + hStep;
         hfactor = hposr-hposl;
         hmarl = lMargin / hfactor;
         hmarr = 0.0;
      } else if (i == Nx-1) {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep + rMargin;
         hfactor = hposr-hposl;
         hmarl = 0.0;
         hmarr = rMargin / (hposr-hposl);
      } else {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep;
         hfactor = hposr-hposl;
         hmarl = 0.0;
         hmarr = 0.0;
      }

      for (Int_t j=0;j<Ny;j++) {

         if (j==0) {
            vposd = 0.0;
            vposu = bMargin + vStep;
            vfactor = vposu-vposd;
            vmard = bMargin / vfactor;
            vmaru = 0.0;
         } else if (j == Ny-1) {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep + tMargin;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = tMargin / (vposu-vposd);
         } else {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = 0.0;
         }

         C->cd(0);

         char name[16];
         sprintf(name,"pad_%i_%i",i,j);
         TPad *pad = (TPad*) gROOT->FindObject(name);
         if (pad) delete pad;
         pad = new TPad(name,"",hposl,vposd,hposr,vposu);
         pad->SetLeftMargin(hmarl);
         pad->SetRightMargin(hmarr);
         pad->SetBottomMargin(vmard);
         pad->SetTopMargin(vmaru);

         pad->SetFrameBorderMode(0);
         pad->SetBorderMode(0);
         pad->SetBorderSize(0);

         pad->Draw();
      }
   }
}
