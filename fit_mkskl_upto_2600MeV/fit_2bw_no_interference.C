double barrierFactor ( double q, int spin ); 
double breakupMomentum( double mass0, double mass1, double mass2 ); 
complex<double> BreitWigner(double m, double m0, double width0, int spin, double m_daughter1, double m_daughter2); 
Double_t myfit(Double_t* x, Double_t* par); 
Double_t mysig(Double_t* x, Double_t* par);
Double_t mybw(Double_t* x, Double_t* par);

void fit_2bw_no_interference() {
	gStyle->SetOptStat(0);
	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.03);
	gStyle->SetPadBottomMargin(0.13);
	gStyle->SetPadLeftMargin(0.16);

	gStyle->SetTitleBorderSize(0);

	gStyle->SetTitleFont(132);
	gStyle->SetTitleSize(0.05);
	gStyle->SetTitleOffset(1.2);

	gStyle->SetLabelSize(0.05,"XY");
	gStyle->SetLabelOffset(0.01,"XY");

	gStyle->SetTitleSize(0.05,"XY");
	gStyle->SetTitleOffset(1.1,"X");

	gStyle->SetMarkerStyle(8);
	gStyle->SetMarkerSize(1.);

	gROOT->ForceStyle();

	TCanvas *c;

	TFile *inf = TFile::Open("hist_dat.root");
	// TFile *inf = TFile::Open("hist.root");

	double min = 1.15, max = 2.60;
	TH1F *h = (TH1F*)inf->Get("im_kskl");
	h->GetXaxis()->SetRangeUser(min, max);

	TH1F *sb = (TH1F*)inf->Get("im_kskl_sb");
	h->Add(sb, -1);

	TFile *inf2 = TFile::Open("hist_acc.root");
	TFile *inf3 = TFile::Open("hist_gen.root");

	TH1F *h_acc = (TH1F*)inf2->Get("im_kskl");
	TH1F *h_gen = (TH1F*)inf3->Get("im_kskl");

	h_acc->Divide(h_gen);
	h->Divide(h_acc);

	char s[100];
	sprintf(s, "Acceptance Corrected Intensity / %.0f MeV", h->GetBinWidth(10)*1000);
	h->GetYaxis()->SetTitle(s);
	h->GetXaxis()->SetTitle("M(K_{S}K_{L}) (GeV)");

	TF1 *fit = new TF1("fit", myfit, min, max, 9);
	fit->SetParameters(10, 1.50, 0.20, 10, 1.78, 0.12, 1.71515e+04, -1.92535e+03, -1.80970e+03);
	fit->SetParNames("N1", "M1", "#Gamma1", "N2", "M2", "#Gamma2", "a0", "a1", "a2");
	fit->SetLineWidth(3);
	fit->SetNpx(1000);

	// fit->FixParameter(1, 1.507);
	// fit->FixParameter(2, 0.254);
	// fit->FixParameter(4, 1.753);
	// fit->FixParameter(5, 0.122);

	TF1 *sig= new TF1("sig", mysig, min, max, 7);
	sig->SetLineColor(kBlue);
	sig->SetFillStyle(3002);
	sig->SetLineWidth(0);
	sig->SetFillColorAlpha(kAzure+8, 1.0);
	sig->SetNpx(1000);

	TF1 *bkg = new TF1("bkg", "pol2", min, max);
	bkg->SetLineColor(kBlack);
	bkg->SetLineWidth(3);

	c = new TCanvas();
	h->Fit(fit, "RE");
	h->GetYaxis()->SetRangeUser(0.0, 1.05*h->GetMaximum());
	sig->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5));
	sig->Draw("same");
	bkg->SetParameters(fit->GetParameter(6), fit->GetParameter(7), fit->GetParameter(8));
	bkg->Draw("SAME");

	TF1 *bw1= new TF1("bw1", mybw, min, max, 3);
	bw1->SetLineColor(kViolet);
	bw1->SetLineWidth(3);
	bw1->SetNpx(1000);
	bw1->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2));
	bw1->Draw("SAME");

	TF1 *bw2= new TF1("bw2", mybw, min, max, 3);
	bw2->SetLineColor(kOrange+8);
	bw2->SetLineWidth(3);
	bw2->SetNpx(1000);
	bw2->SetParameters(fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5));
	bw2->Draw("SAME");

	TLegend *lg = new TLegend(0.6, 0.42, 0.96, 0.97);
	lg->SetFillStyle(0);
	lg->SetBorderSize(0);

	lg->AddEntry(h, "Data");
	lg->AddEntry(fit, "Fit", "l");
	lg->AddEntry(sig, "Signal", "lf");
	lg->AddEntry(bkg, "Background", "l");

	lg->AddEntry((TObject*)0, "", "");
	sprintf(s, "#splitline{%s = %.3f #pm %.3f GeV}{%s = %.3f #pm %.3f GeV}", fit->GetParName(1), fit->GetParameter(1), fit->GetParError(1), fit->GetParName(2), fit->GetParameter(2), fit->GetParError(2));
	lg->AddEntry(bw1, s, "l");
	lg->AddEntry((TObject*)0, "", "");
	sprintf(s, "#splitline{%s = %.3f #pm %.3f GeV}{%s = %.3f #pm %.3f GeV}", fit->GetParName(4), fit->GetParameter(4), fit->GetParError(4), fit->GetParName(5), fit->GetParameter(5), fit->GetParError(5));
	lg->AddEntry(bw2, s, "l");
	lg->AddEntry((TObject*)0, "", "");
	sprintf(s, "#chi^{2}/ndf = %.0f/%d = %.2f", fit->GetChisquare(), fit->GetNDF(), fit->GetChisquare()/fit->GetNDF());
	lg->AddEntry((TObject*)0, s, "");
	lg->Draw();

	// sprintf(s, "Counts / %.0f MeV", h->GetBinWidth(10)*1000);
	// h->GetYaxis()->SetTitle(s);

	cout << fit->GetChisquare() << endl;

	c->SaveAs("pdfs/kskl_2bw_no_interference.pdf");

	//c = new TCanvas();
	//auto rp = new TRatioPlot(h);
	//rp->Draw();

	//auto opf = TFile::Open("fit.root", "recreate");
	//h->Write();
}

// q is the breakup momentum
double barrierFactor ( double q, int spin ) {
	double barrier, z;

	z = ( (q*q) / (0.1973*0.1973) );

	if(spin == 0)
		barrier = 1.0;

	else if(spin == 1)
		barrier = sqrt( (2.0*z) / (z + 1.0) );

	else if(spin == 2)
		barrier = sqrt( (13.0*z*z) / ((z-3.0)*(z-3.0) + 9.0*z) );

	else if(spin == 3)
		barrier = sqrt( (277.0*z*z*z) / (z*(z-15.0)*(z-15.0) + 9.0*(2.0*z-5.0)*(2.0*z-5.0)) );

	else if(spin == 4)
		barrier = sqrt( (12746.0*z*z*z*z) / ((z*z-45.0*z+105.0)*(z*z-45.0*z+105.0) + 25.0*z*(2.0*z-21.0)*(2.0*z-21.0)) );

	else
		barrier = 0.0;

	return barrier;
}

// mass0 = x
// mass1 = mass of daughter particle 1
// mass2 = mass of daughter particle 2
double breakupMomentum( double mass0, double mass1, double mass2 ) {
	double q;
	
	q = sqrt( fabs( mass0*mass0*mass0*mass0 + 
			mass1*mass1*mass1*mass1 +
			mass2*mass2*mass2*mass2 -
			2.0*mass0*mass0*mass1*mass1 -
			2.0*mass0*mass0*mass2*mass2 -
			2.0*mass1*mass1*mass2*mass2  ) ) / (2.0 * mass0);
	
	return q;
}

complex<double> BreitWigner(double m, double m0, double width0, int spin, double m_daughter1, double m_daughter2) {
	double q = breakupMomentum(m, m_daughter1, m_daughter2);
	double q0 = breakupMomentum(m0, m_daughter1, m_daughter2);

	double F = barrierFactor(q, spin);
	double F0 = barrierFactor(q0, spin);

	double Gamma = width0 * (m0/m) * (q/q0) * ((F*F)/(F0*F0));

	complex<double> top(m0*Gamma, 0.0);
	complex<double> bottom(m0*m0 - m*m, -1*m0*Gamma);

	return top/bottom;
}

Double_t mybw(Double_t* x, Double_t* par) {
	double N = par[0];
	complex<double> bw = BreitWigner(x[0], par[1], par[2], 1, 0.497, 0.497);

	return norm(N*bw);
}

Double_t myfit(Double_t* x, Double_t* par) {
	double N1 = par[0];
	complex<double> bw1 = BreitWigner(x[0], par[1], par[2], 1, 0.497, 0.497);

	double N2 = par[3];
	complex<double> bw2 = BreitWigner(x[0], par[4], par[5], 1, 0.497, 0.497);

	return norm(N1*bw1) + norm(N2*bw2) + par[6] + par[7]*x[0] + par[8]*x[0]*x[0];
}

Double_t mysig(Double_t* x, Double_t* par) {
	double N1 = par[0];
	complex<double> bw1 = BreitWigner(x[0], par[1], par[2], 1, 0.497, 0.497);

	double N2 = par[3];
	complex<double> bw2 = BreitWigner(x[0], par[4], par[5], 1, 0.497, 0.497);

	return norm(N1*bw1) + norm(N2*bw2);
}

