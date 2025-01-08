complex<double> BreitWigner(double m, double m0, double width0, int spin, double m_daughter1, double m_daughter2);
complex<double> BreitWigner2(double m, double m0, double width0, int spin, double m_daughter1, double m_daughter2);
complex<double> BreitWigner3(double m, double m0, double width0, int spin, double m_daughter1, double m_daughter2);
double breakupMomentum( double mass0, double mass1, double mass2);

Double_t myfit1(Double_t* x, Double_t* par);
Double_t myfit1_spin0(Double_t* x, Double_t* par);
Double_t myfit2(Double_t* x, Double_t* par);
Double_t myfit3(Double_t* x, Double_t* par);
Double_t myfit3_spin0(Double_t* x, Double_t* par);
Double_t myfit4(Double_t* x, Double_t* par);
Double_t myfit4_spin0(Double_t* x, Double_t* par);

void bw() {
	TCanvas *c;

	double min = 1.00, max = 2.00;

	TF1 *fit1 = new TF1("fit1", myfit1, min, max, 3);
	fit1->SetParameters(350, 1.50, 0.100);
	fit1->SetParameter(0, 1/fit1->Integral(min, max));
	fit1->SetParNames("N", "M", "#Gamma");
	fit1->SetLineColor(kRed);
	fit1->SetLineStyle(1);

	TF1 *fit1_spin0 = new TF1("fit1_spin0", myfit1_spin0, min, max, 3);
	fit1_spin0->SetParameters(350, 1.50, 0.100);
	fit1_spin0->SetParameter(0, 1/fit1_spin0->Integral(min, max));
	fit1_spin0->SetParNames("N", "M", "#Gamma");
	fit1_spin0->SetLineColor(kRed);
	fit1_spin0->SetLineStyle(1);

	TF1 *fit2 = new TF1("fit2", myfit2, min, max, 3);
	fit2->SetParameters(350, 1.50, 0.100);
	fit2->SetParameter(0, 1/fit2->Integral(min, max));
	fit2->SetLineColor(kBlue);
	fit2->SetLineStyle(2);

	TF1 *fit3 = new TF1("fit3", myfit3, min, max, 3);
	fit3->SetParameters(350, 1.50, 0.100);
	fit3->SetParameter(0, 1/fit3->Integral(min, max));
	fit3->SetLineColor(kViolet);
	fit3->SetLineStyle(3);

	TF1 *fit3_spin0 = new TF1("fit3_spin0", myfit3_spin0, min, max, 3);
	fit3_spin0->SetParameters(350, 1.50, 0.100);
	fit3_spin0->SetParameter(0, 1/fit3_spin0->Integral(min, max));
	fit3_spin0->SetLineColor(kViolet);
	fit3_spin0->SetLineStyle(3);

	TF1 *fit4 = new TF1("fit4", myfit4, min, max, 3);
	fit4->SetParameters(350, 1.50, 0.100);
	fit4->SetParameter(0, 1/fit4->Integral(min, max));
	fit4->SetLineColor(kOrange);
	fit4->SetLineStyle(4);

	TF1 *fit4_spin0 = new TF1("fit4_spin0", myfit4_spin0, min, max, 3);
	fit4_spin0->SetParameters(350, 1.50, 0.100);
	fit4_spin0->SetParameter(0, 1/fit4_spin0->Integral(min, max));
	fit4_spin0->SetLineColor(kOrange);
	fit4_spin0->SetLineStyle(4);

	c = new TCanvas();
	fit1->Draw();
	fit1_spin0->Draw("SAME");
	fit2->Draw("SAME");
	fit3->Draw("SAME");
	fit3_spin0->Draw("SAME");
	fit4->Draw("SAME");
	fit4_spin0->Draw("SAME");

	c->SaveAs("bw.pdf");
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

// Following https://github.com/JeffersonLab/halld_sim/blob/master/src/libraries/AMPTOOLS_AMPS/BreitWigner.cc
complex<double> BreitWigner2(double m, double m0, double width0, int spin, double m_daughter1, double m_daughter2) {
	double q = breakupMomentum(m, m_daughter1, m_daughter2);
	double q0 = breakupMomentum(m0, m_daughter1, m_daughter2);

	double F = barrierFactor(q, spin);
	double F0 = barrierFactor(q0, spin);

	double Gamma = width0 * (m0/m) * (q/q0) * ((F*F)/(F0*F0));

	complex<double> top(m0*width0/3.1416, 0.0);
	complex<double> bottom(m0*m0 - m*m, -1*m0*Gamma);

	return F*top/bottom;
}

// taken from https://pdg.lbl.gov/2019/reviews/rpp2019-rev-resonances.pdf
complex<double> BreitWigner3(double m, double m0, double width0, int spin, double m_daughter1, double m_daughter2) {
	double q = breakupMomentum(m, m_daughter1, m_daughter2);
	double q0 = breakupMomentum(m0, m_daughter1, m_daughter2);

	double F = barrierFactor(q, spin);
	double F0 = barrierFactor(q0, spin);

	double Gamma = width0 * (m0/m) * pow(q/q0, 2*spin + 1) * ((F*F)/(F0*F0));

	complex<double> top(m0*width0/3.1416, 0.0);
	complex<double> bottom(m0*m0 - m*m, -1*m0*Gamma);

	return F*top/bottom;
}


Double_t myfit1(Double_t* x, Double_t* par) {
	double N1 = par[0];
	complex<double> bw1 = BreitWigner(x[0], par[1], par[2], 1, 0.497, 0.497);

	return norm(N1*bw1);
}

Double_t myfit1_spin0(Double_t* x, Double_t* par) {
	double N1 = par[0];
	complex<double> bw1 = BreitWigner(x[0], par[1], par[2], 0, 0.497, 0.497);

	return norm(N1*bw1);
}

// Breit-Wigner function. Taken from https://twiki.cern.ch/twiki/pub/CMSPublic/WorkBookHowToFit/BW.C.txt
Double_t myfit2(Double_t* x, Double_t* par) {
	Double_t arg1 = 14.0/22.0; // 2 over pi
	Double_t arg2 = par[1]*par[1]*par[2]*par[2]; // M = par[1] Gamma = par[2]  
	Double_t arg3 = ((x[0]*x[0]) - (par[1]*par[1]))*((x[0]*x[0]) - (par[1]*par[1]));
	Double_t arg4 = x[0]*x[0]*x[0]*x[0]*((par[2]*par[2])/(par[1]*par[1]));

	return par[0]*arg1*arg2/(arg3 + arg4);
}

Double_t myfit3(Double_t* x, Double_t* par) {
	double N = par[0];
	complex<double> bw = BreitWigner2(x[0], par[1], par[2], 1, 0.497, 0.497);

	return norm(N*bw);
}

Double_t myfit3_spin0(Double_t* x, Double_t* par) {
	double N = par[0];
	complex<double> bw = BreitWigner2(x[0], par[1], par[2], 0, 0.497, 0.497);

	return norm(N*bw);
}

Double_t myfit4(Double_t* x, Double_t* par) {
	double N = par[0];
	complex<double> bw = BreitWigner3(x[0], par[1], par[2], 1, 0.497, 0.497);

	return norm(N*bw);
}

Double_t myfit4_spin0(Double_t* x, Double_t* par) {
	double N = par[0];
	complex<double> bw = BreitWigner3(x[0], par[1], par[2], 0, 0.497, 0.497);

	return norm(N*bw);
}

