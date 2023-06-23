void plot_mass(int nbins = 0, vector<string> waves = {""});
void plot_angles(int nbins = 0, vector<string> waves = {""}, string var = "", int n = 50, double l = 1.0, double m = -1.0, int nx = 2, int ny = 3);

void CanvasPartition(TCanvas *C, const Int_t Nx = 2, const Int_t Ny = 2, Float_t lMargin = 0.15, Float_t rMargin = 0.05, Float_t bMargin = 0.15, Float_t tMargin = 0.05);

void plot() {
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

	waves = {"Pm1+", "Pm1-", "Pp0+", "Pp0-", "Pp1+", "Pp1-"};
	plot_mass(36, waves);
	plot_angles(18, waves, "cosTheta", 50, -1, 1, 5, 4);
	plot_angles(18, waves, "phi", 50, -3.14, 3.14, 5, 4);
	plot_angles(36, waves, "Phi", 50, -3.14, 3.14, 5, 4);
	plot_angles(18, waves, "psi", 50, -3.14, 3.14, 5, 4);

//	//waves = {"Pm1-", "Pp0+", "Pp0-", "Pp1+", "Pp1-"};
//	//plot_mass(18, waves);
//	//plot_angles(18, waves, "cosTheta", 50, -1, 1, 5, 4);
//	//plot_angles(18, waves, "phi", 50, -3.14, 3.14, 5, 4);
//
//	//waves = {"Pm1+", "Pp0+", "Pp0-", "Pp1+", "Pp1-"};
//	//plot_mass(18, waves);
//	//plot_angles(18, waves, "cosTheta", 50, -1, 1, 5, 4);
//	//plot_angles(18, waves, "phi", 50, -3.14, 3.14, 5, 4);
//
//	waves = {"Pp0+", "Pp0-", "Pp1+", "Pp1-"};
//	plot_mass(18, waves);
//	plot_angles(18, waves, "cosTheta", 50, -1, 1, 5, 4);
//	plot_angles(18, waves, "phi", 50, -3.14, 3.14, 5, 4);
//	plot_angles(18, waves, "Phi", 50, -3.14, 3.14, 5, 4);
//	plot_angles(18, waves, "psi", 50, -3.14, 3.14, 5, 4);

	//waves = {"Pp0+", "Pp1+", "Pp1-"};
	//plot_mass(18, waves);
	//plot_angles(18, waves, "cosTheta", 50, -1, 1, 5, 4);
	//plot_angles(18, waves, "phi", 50, -3.14, 3.14, 5, 4);

	//waves = {"Pp0+", "Pp1+"};
	//plot_mass(18, waves);
	//plot_angles(18, waves, "cosTheta", 50, -1, 1, 5, 4);
	//plot_angles(18, waves, "phi", 50, -3.14, 3.14, 5, 4);

//	waves = {"Sp0+", "Sp0-", "Pm1+", "Pm1-", "Pp0+", "Pp0-", "Pp1+", "Pp1-"};
//	plot_mass(36, waves);
//	//plot_angles(18, waves, "cosTheta", 50, -1, 1, 5, 4);
//	//plot_angles(18, waves, "phi", 50, -3.14, 3.14, 5, 4);
//
//	waves = {"Sp0+", "Sp0-", "Pp0+", "Pp0-", "Pp1+", "Pp1-"};
//	plot_mass(36, waves);
//	//waves = {"Pm1+", "Pm1-", "Pp0+", "Pp0-", "Pp1+", "Pp1-", "Dm2+", "Dm2-", "Dm1+", "Dm1-", "Dp0+", "Dp0-", "Dp1+", "Dp1-", "Dp2+", "Dp2-"};
//	//plot_mass(18, waves);
//
//
//	//**********************************
//	//**********************************
//	//
//	//	plot All P-wave and 1 F-wave
//	//
//	//**********************************
//	//**********************************
//
//	vector<string> f_waves = {"Fm3+", "Fm3-", "Fm2+", "Fm2-", "Fm1+", "Fm1-", "Fp0+", "Fp0-", "Fp1+", "Fp1-", "Fp2+", "Fp2-", "Fp3-"};
//	//vector<string> f_waves = {"Fm3+", "Fm3-", "Fm2+", "Fm2-", "Fm1+", "Fm1-", "Fp0+", "Fp0-", "Fp1+", "Fp1-", "Fp2+", "Fp2-", "Fp3+", "Fp3-"};
//
//	for(auto f_wave : f_waves) {
//		waves = {"Pm1+", "Pm1-", "Pp0+", "Pp0-", "Pp1+", "Pp1-"};
//		//waves = {"Pp0+", "Pp0-", "Pp1+", "Pp1-"};
//		//waves = {"Pp0+", "Pp0-", "Pp1+", "Pp1-", "Fp3+"};
//		waves.push_back(f_wave);
//
//		plot_mass(36, waves);
//	}
}

void plot_mass(int nbins = 0, vector<string> waves = {""}) {
	if(nbins == 0) {
		cout << "nbins = 0" << endl << "Enter number of bins to plot" << endl;
		exit(0);
	}

	if(waves[0] == "") {
		cout << "No waves given" << endl;
		exit(0);
	}

	TFile *inf, *opf;
	TH1F *h, *h1, *h2[waves.size()], *h3;

	string waveset = "";
	string xtitle = "";
	string dir = "";

	vector<string> pols = {"000", "045", "090", "135"};
	map<string, TH1F*> hist_map;
	map<string, string> wave_to_tlatex = { {"Sp0+", "S^{+}_{0}"}, {"Sp0-", "S^{-}_{0}"},
						{"Pm1+", "P^{+}_{-1}"}, {"Pm1-", "P_{-1}^{-}"}, {"Pp0+", "P_{0}^{+}"}, {"Pp0-", "P_{0}^{-}"}, {"Pp1+", "P_{1}^{+}"}, {"Pp1-", "P_{1}^{-}"},
						{"Fm1+", "F^{+}_{-1}"}, {"Fm1-", "F_{-1}^{-}"}, {"Fp0+", "F_{0}^{+}"}, {"Fp0-", "F_{0}^{-}"}, {"Fp1+", "F_{1}^{+}"}, {"Fp1-", "F_{1}^{-}"}, 
						{"Fm3+", "F^{+}_{-3}"}, {"Fm3-", "F_{-3}^{-}"}, {"Fm2+", "F_{-2}^{+}"},{"Fm2-", "F_{-2}^{-}"},  {"Fm0-", "F_{-2}^{-}"}, {"Fp2+", "F_{2}^{+}"}, {"Fp2-", "F_{2}^{-}"},
						{"Fp3+", "F^{+}_{3}"}, {"Fp3-", "F_{3}^{-}"}};

	for(auto wave : waves)	waveset += wave+"_";
	waveset.pop_back();
	//waves.push_back("_"+waveset);

	h1 = new TH1F("h1", ";;Intensity", nbins, 1.1, 2.0);
	h3 = new TH1F("h3", ";;Intensity", nbins, 1.1, 2.0);
	//for(int i = 0; i < waves.size(); i++)	h2[i] = new TH1F( ("h_"+to_string(i)).c_str() , ";M(K_{S}K_{L});Intensity", 20, 1.0, 2.0);
	for(int i = 0; i < waves.size(); i++) {
		h2[i] = new TH1F( ("h_"+to_string(i)).c_str() , ";;Intensity", nbins, 1.1, 2.0);
	}

	for(int i = 0; i < nbins; i++) {
		cout << "include data for bin_"+to_string(i)+" and waveset "+waveset << endl;

		for(int j = 0; j < waves.size(); j++) {
			dir = "fit_sp17_"+to_string(nbins)+"bins/bin_"+to_string(i)+"/"+waveset+"/etapi_plot_"+waves[j]+".root";
			inf = TFile::Open( dir.c_str(), "READ");
			cout << dir << endl;

			if(inf == NULL)	continue;
	
			for(auto pol : pols) {
				if(j == 0) {
					h = (TH1F*)inf->Get( ("NAME_"+pol+"_Metapi_25MeVBindat").c_str() );
					//h = (TH1F*)inf->Get( ("NAME_"+pol+"_Metapidat").c_str() );
					//h = (TH1F*)inf->Get( ("Mkskl_dat").c_str() );
					h1->Add(h);

					h = (TH1F*)inf->Get( ("NAME_"+pol+"_Metapi_25MeVBinbkg").c_str() );
					//h = (TH1F*)inf->Get( ("NAME_"+pol+"_Metapibkg").c_str() );
					h3->Add(h);
				}
	
				//h = (TH1F*)inf->Get("Mkskl_acc");
				h = (TH1F*)inf->Get( ("NAME_"+pol+"_Metapi_25MeVBinacc").c_str() );
				//h = (TH1F*)inf->Get( ("NAME_"+pol+"_Metapiacc").c_str() );
				//h = (TH1F*)inf->Get( ("NAME_"+pol+"_Metapigen").c_str() );
				h2[j]->Add(h);
			} // end loop over polarization angles
	
			xtitle = h->GetXaxis()->GetTitle();

			inf->Close();
		} // end loop over waveset

		if(nbins == 35) break;
	} // end loop over bins

	h1->Add(h3, -1);

	opf = TFile::Open( ("hists/"+waveset+".root").c_str(), "RECREATE");
	h1->Write("all");

	gStyle->SetOptStat(0);

	TCanvas *c = new TCanvas();
	//TCanvas *c = new TCanvas("c", "c", 500, 500, 500, 500);
	//c->Divide(waves.size());
      //CanvasPartition(c, 2, 3);

      //TPad *pad[2][3];
//
//	TPad *pad[2][2];

	if(waves.size() % 2 == 0)
		c->Divide(2, waves.size()/2);
	else
		c->Divide(2, waves.size()/2+1);

	TLatex t;
	t.SetTextSize(0.08);

	h1->GetYaxis()->SetRangeUser(0, 1.35*h1->GetMaximum());
	h1->GetXaxis()->SetTitle( xtitle.c_str() );

	char s[100];
	for(int i = 0; i < waves.size(); i++) {
		c->cd(i+1);
		h1->Draw();
		h2[i]->SetFillColor(kYellow);
		h2[i]->GetYaxis()->SetMaxDigits(3);
		h2[i]->Draw("SAME HIST");

		h2[i]->Write( waves[i].c_str() );

		TLegend *lg = new TLegend(0.35, 0.75, 0.95, 0.9);
		lg->SetNColumns(2);
		lg->AddEntry(h1, "Data", "lep");
		sprintf(s, " [%.1f%%]", h2[i]->Integral()/h1->Integral()*100);
		lg->AddEntry(h2[i], (wave_to_tlatex[waves[i]]+s).c_str(), "f");

		lg->Draw();

		//sprintf(s, "#int %s = %.0f", wave_to_tlatex[waves[i]].c_str(), h2[i]->Integral());
		//t.DrawLatex(1.65, 0.8*h1->GetMaximum(), s);
		//t.DrawLatex(1.8, 0.8*h1->GetMaximum(), wave_to_tlatex[waves[i]].c_str());
	}

	//c->SaveAs("sp17.png");
	c->SaveAs( ("hists/hmass_"+waveset+".pdf").c_str() );
}

void plot_angles(int nbins = 0, vector<string> waves = {""}, string var = "", int n = 50, double l = -1.0, double m = 1.0, int nx = 2, int ny = 3) {
	gStyle->SetOptStat(0);

	if(nbins == 0) {
		cout << "nbins = 0" << endl << "Enter number of bins to plot" << endl;
		exit(0);
	}

	if(waves[0] == "") {
		cout << "No waves given" << endl;
		exit(0);
	}

	TFile *inf;
	TH1F *h, *h1[nx][ny], *h2[nx][ny], *h3[nx][ny];

	TLatex t;
	t.SetTextSize(0.08);

	string waveset = "";
	string x_title = "";

	vector<string> pols = {"000", "045", "090", "135"};

	for(auto wave : waves)	waveset += wave+"_";
	waveset.pop_back();

	TCanvas *c = new TCanvas();
	CanvasPartition(c, nx, ny, 0.12, 0.03, 0.12, 0.03);

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

			h1[i][j] = new TH1F( ("h1"+to_string(i)+to_string(j)+waveset).c_str(), ";M(K_{S}K_{L});Intensity", n, l, m);
			h2[i][j] = new TH1F( ("h2"+to_string(i)+to_string(j)+waveset).c_str(), ";M(K_{S}K_{L});Intensity", n, l, m);
			h3[i][j] = new TH1F( ("h2"+to_string(i)+to_string(j)+waveset).c_str(), ";M(K_{S}K_{L});Intensity", n, l, m);
	
			ibin = i*ny + j;
			cout << ibin << endl;
			fname = "fit_sp17_"+to_string(nbins)+"bins/bin_"+to_string(ibin)+"/"+waveset+"/etapi_plot_"+waveset+".root";
			inf = TFile::Open( fname.c_str(), "READ");
			//cout << "include data for bin_"+to_string(ibin)+" and waveset "+waveset << endl;
			cout << "Get histogram from file "+fname << endl;

			if(inf == NULL)	continue;
		
			for(auto pol : pols) {
				h = (TH1F*)inf->Get( ("NAME_"+pol+"_"+var+"dat").c_str() );
				h1[i][j]->Add(h);
				h1[i][j]->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
//				delete h;
		
				h = (TH1F*)inf->Get( ("NAME_"+pol+"_"+var+"acc").c_str() );
				h2[i][j]->Add(h);
				h2[i][j]->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
//				delete h;
		
				h = (TH1F*)inf->Get( ("NAME_"+pol+"_"+var+"bkg").c_str() );
				h3[i][j]->Add(h);
				h3[i][j]->GetXaxis()->SetTitle(h->GetXaxis()->GetTitle());
//				delete h;
			} // end loop over polarization angles
	
			if(h1[i][j] == NULL || h2[i][j] == NULL)	continue;
	
			h1[i][j]->Add(h3[i][j], -1);
	
			x_title = h->GetXaxis()->GetTitle();
			h2[i][j]->GetXaxis()->SetTitle( x_title.c_str() );
	
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
	
			t.DrawLatex(0.4, h1[i][j]->GetMaximum(), ("bin_"+to_string(ibin)).c_str() );

			inf->Close();
		} // end j loop
	} // end i loop
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
