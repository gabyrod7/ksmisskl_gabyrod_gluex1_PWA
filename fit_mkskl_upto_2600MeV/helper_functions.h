#ifndef helper_functions
#define helper_functions

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

// Double_t myfit(Double_t* x, Double_t* par) {
// 	double N1 = par[0];
// 	complex<double> bw1 = BreitWigner(x[0], par[1], par[2], 1, 0.497, 0.497);

// 	double N2 = par[3];
// 	complex<double> bw2 = BreitWigner(x[0], par[4], par[5], 1, 0.497, 0.497);

// 	return norm(N1*bw1) + norm(N2*bw2) + par[6] + par[7]*x[0] + par[8]*x[0]*x[0];
// }

// Double_t mysig(Double_t* x, Double_t* par) {
// 	double N1 = par[0];
// 	complex<double> bw1 = BreitWigner(x[0], par[1], par[2], 1, 0.497, 0.497);

// 	double N2 = par[3];
// 	complex<double> bw2 = BreitWigner(x[0], par[4], par[5], 1, 0.497, 0.497);

// 	return norm(N1*bw1) + norm(N2*bw2);
// }

// Double_t mybkg(Double_t* x, Double_t* par) {
// 	return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
// }

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

#endif