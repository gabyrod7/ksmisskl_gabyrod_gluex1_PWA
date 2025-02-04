double barrierFactor ( double q, int spin ); 
double breakupMomentum( double mass0, double mass1, double mass2 ); 
complex<double> BreitWigner(double m, double m0, double width0, int spin, double m_daughter1, double m_daughter2); 
Double_t myfit(Double_t* x, Double_t* par); 
Double_t mysig(Double_t* x, Double_t* par);
Double_t mybkg(Double_t* x, Double_t* par);
TH1F* getAcceptanceCorrectedHist(TString fdata, TString facc, TString fgen, TString hname);

void fit_phi_2bw_argus() {
	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(200000);

	TCanvas *c;
	double min = 1.0, max = 2.60;

	// TH1F *h_sp17 = getAcceptanceCorrectedHist("hists/hist_dat_sp17.root", "hists/hist_acc_sp17.root", "hists/hist_gen_sp17.root", "im_kskl");
	// TH1F *h_sp18 = getAcceptanceCorrectedHist("hists/hist_dat_sp18.root", "hists/hist_acc_sp18.root", "hists/hist_gen_sp18.root", "im_kskl");
	// TH1F *h_fa18 = getAcceptanceCorrectedHist("hists/hist_dat_fa18.root", "hists/hist_acc_fa18.root", "hists/hist_gen_fa18.root", "im_kskl");
	TH1F *h_sp17 = getAcceptanceCorrectedHist("tmp/hist_dat_sp17.root", "tmp/hist_acc_sp17.root", "tmp/hist_gen_sp17.root", "im_kskl");
	TH1F *h_sp18 = getAcceptanceCorrectedHist("tmp/hist_dat_sp18.root", "tmp/hist_acc_sp18.root", "tmp/hist_gen_sp18.root", "im_kskl");
	TH1F *h_fa18 = getAcceptanceCorrectedHist("tmp/hist_dat_fa18.root", "tmp/hist_acc_fa18.root", "tmp/hist_gen_fa18.root", "im_kskl");

	c = new TCanvas();
	h_sp17->GetXaxis()->SetRangeUser(min, max);

	TH1F *h = (TH1F*)h_sp17->Clone("h");
	h->Add(h_sp18);
	h->Add(h_fa18);

	char s[100];
	sprintf(s, "Acceptance Corrected Intensity / %.1f MeV", h->GetBinWidth(10)*1000);
	h->GetYaxis()->SetTitle(s);
	h->GetXaxis()->SetTitle("M(K_{S}K_{L}) (GeV)");

	TF1 *fit = new TF1("fit", myfit, min, max, 11);
	// fit->SetParameters(200, 1.53, 0.20, 1.1, 1.73, 0.12, 3.1, 1000, 1, -1, 1);
	// fit->SetParameter(11, 1);
	// fit->SetParameter(12, 1);

	// fit->SetParameters(380, 1.48, 0.20, 1.1, 1.75, 0.122, 1, 1000, 0.02, -2.14, -3.55);
	// fit->SetParameter(11, 12);

	fit->SetParameters(380, 1.48, 0.20, 1.1, 1.75, 0.122, 1000, 1.75926e-35, -3.61642e+00, -6.33044e+01);
	fit->SetParameter(10, 8.79324e+01);

	fit->FixParameter(1, 1.538);
	fit->FixParameter(2, 0.257);
	fit->FixParameter(4, 1.753);
	fit->FixParameter(5, 0.125);	

	// fit->SetParameters(380, 1.48, 0.20, 1.1, 1.75, 0.122, 1, 1000, 300, -1, 2);
	// fit->SetParameter(11, 2);

	fit->SetParNames("N1", "M1", "#Gamma1", "N2", "M2", "#Gamma2", "Nphi", "N", "m0");
	fit->SetParName(9, "p");
	fit->SetParName(10, "c");

	// fit->FixParameter(2, 0.4);
	// fit->FixParameter(6, 0.122);

	TF1 *sig= new TF1("sig", mysig, min, max, 6);
	sig->SetLineColor(kBlue);

	TF1 *bkg = new TF1("bkg", mybkg, min, max, 4);
	bkg->SetLineColor(kBlack);

	TF1 *phi = new TF1("phi", "[0]*TMath::Voigt(x - 1.022, 9.92e-03, 4e-03, 4)", min, max);
	phi->SetLineColor(kViolet);

	c = new TCanvas();
	h->Fit(fit, "RE");
	// h->Draw();
	// fit->Draw("SAME");
	h->GetYaxis()->SetRangeUser(0.0, 1.05*h->GetMaximum());
	sig->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5));
	sig->Draw("same");
	bkg->SetParameters(fit->GetParameter(7), fit->GetParameter(8), fit->GetParameter(9), fit->GetParameter(10));
	bkg->Draw("SAME");
	phi->SetParameter(0, fit->GetParameter(6));
	phi->Draw("SAME");

	// h->GetYaxis()->SetRangeUser(0, 2000);
	sprintf(s, "Counts / %.1f MeV", h->GetBinWidth(1)*1000);
	h->GetYaxis()->SetTitle(s);
	h->GetYaxis()->SetRangeUser(0, 40000);

	c->SaveAs("pdf_nInterference/fit_phi_2bw_argus.pdf");
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

Double_t mybkg(Double_t* x, Double_t* par) {
	// return pow(x[0] + 0.99, par[0])*TMath::Exp(par[1]*x[0]);
	// return (x[0] - 0.99)*(par[0] + par[1]*x[0] + par[2]*x[0]*x[0]);

	// Below is an implementation of the ARGUS function taken from 
	// https://root.cern.ch/doc/master/classRooArgusBG.html
	double N = par[0];
	double m = x[0] - 0.994;
	double m0 = par[1];
	double p = par[2];
	double c = par[3];

	return N * m * pow(1 - (m/m0)*(m/m0), p) * exp(c * (1 - (m/m0)*(m/m0)));
}

Double_t mysig(Double_t* x, Double_t* par) {
	double N1 = par[0];
	complex<double> bw1 = BreitWigner(x[0], par[1], par[2], 1, 0.497, 0.497);

	double N2 = par[3];
	complex<double> bw2 = BreitWigner(x[0], par[4], par[5], 1, 0.497, 0.497);

	// return N1*norm(bw1 + N2*phase*bw2)+ par[7]*TMath::Voigt(x[0] - 1.022, 9.92e-03, 4e-03, 4);
	return N1*norm(bw1) + N2*norm(bw2)+ par[6]*TMath::Voigt(x[0] - 1.022, 9.92e-03, 4e-03, 4);
}

Double_t myfit(Double_t* x, Double_t* par) {
	return mysig(&x[0], &par[0]) + mybkg(&x[0], &par[7]);
}

TH1F* getAcceptanceCorrectedHist(TString fdata, TString facc, TString fgen, TString hname) {
	TFile *inf = TFile::Open(fdata);
	TH1F *h = (TH1F*)inf->Get(hname);
	TH1F *h_sb = (TH1F*)inf->Get(hname + "_sb");
	h->Add(h_sb, -1);

	TFile *inf2 = TFile::Open(facc);
	TH1F *h_acc = (TH1F*)inf2->Get(hname);

	TFile *inf3 = TFile::Open(fgen);
	TH1F *h_gen = (TH1F*)inf3->Get(hname);

	h_acc->Divide(h_gen);
	h->Divide(h_acc);

	return h;
}
