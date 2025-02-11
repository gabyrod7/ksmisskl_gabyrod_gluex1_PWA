#include "helper_functions.h"

vector<double> fit_2bw_no_interference(string hName, double min = 1.16, double max = 2.60, TString sName = "");
vector<double> fit_2bw_with_interference(string hName, double min = 1.16, double max = 2.60, TString sName = "");

Double_t model1(Double_t* x, Double_t* par);
Double_t modelSig1(Double_t* x, Double_t* par);

Double_t model2(Double_t* x, Double_t* par);
Double_t modelSig2(Double_t* x, Double_t* par);

Double_t modelBkg(Double_t* x, Double_t* par);

void fit_changeRange() {
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

	TGraphErrors *m1 = new TGraphErrors();
	TGraphErrors *m2 = new TGraphErrors();
	TGraphErrors *g1 = new TGraphErrors();
	TGraphErrors *g2 = new TGraphErrors();

	vector<double> pars;
	string hName;

	hName = "im_kskl_fitRange1";
	pars = fit_2bw_no_interference(hName, 1.14, 2.66, "fit_range1");
	m1->SetPoint(0, 0, pars[0]);
	m1->SetPointError(0, 0, pars[1]);
	g1->SetPoint(0, 0, pars[2]);
	g1->SetPointError(0, 0, pars[3]);
	m2->SetPoint(0, 0, pars[4]);
	m2->SetPointError(0, 0, pars[5]);
	g2->SetPoint(0, 0, pars[6]);
	g2->SetPointError(0, 0, pars[7]);

	// fit_2bw_with_interference(hName, 1.10, 2.66, "fit_range1");

	hName = "im_kskl_fitRange2";
	pars = fit_2bw_no_interference(hName, 1.20, 2.54, "fit_range2");
	m1->SetPoint(1, 1, pars[0]);
	m1->SetPointError(1, 0, pars[1]);
	g1->SetPoint(1, 1, pars[2]);
	g1->SetPointError(1, 0, pars[3]);
	m2->SetPoint(1, 1, pars[4]);
	m2->SetPointError(1, 0, pars[5]);
	g2->SetPoint(1, 1, pars[6]);
	g2->SetPointError(1, 0, pars[7]);

	// fit_2bw_with_interference(hName, 1.20, 2.54, "fit_range2");

	hName = "im_kskl_fitRange3";
	pars = fit_2bw_no_interference(hName, 1.16, 2.5, "fit_range3");
	m1->SetPoint(2, 2, pars[0]);
	m1->SetPointError(2, 0, pars[1]);
	g1->SetPoint(2, 2, pars[2]);
	g1->SetPointError(2, 0, pars[3]);
	m2->SetPoint(2, 2, pars[4]);
	m2->SetPointError(2, 0, pars[5]);
	g2->SetPoint(2, 2, pars[6]);
	g2->SetPointError(2, 0, pars[7]);

	hName = "im_kskl_fitRange4";
	pars = fit_2bw_no_interference(hName, 1.16, 2.7, "fit_range4");
	m1->SetPoint(3, 3, pars[0]);
	m1->SetPointError(3, 0, pars[1]);
	g1->SetPoint(3, 3, pars[2]);
	g1->SetPointError(3, 0, pars[3]);
	m2->SetPoint(3, 3, pars[4]);
	m2->SetPointError(3, 0, pars[5]);
	g2->SetPoint(3, 3, pars[6]);
	g2->SetPointError(3, 0, pars[7]);

	// fit_2bw_with_interference(hName, 1.20, 2.54, "fit_range2");

	TFile *opf = TFile::Open("rootFiles/fit_changeRange.root", "RECREATE");
	m1->Write("m1");
	m2->Write("m2");
	g1->Write("g1");
	g2->Write("g2");
}

vector<double> fit_2bw_no_interference(string hName, double min = 1.16, double max = 2.60, TString sName = "") {
	TCanvas *c;

	TH1F *h_sp17 = getAcceptanceCorrectedHist("hists/hist_dat_sp17.root", "hists/hist_acc_sp17.root", "hists/hist_gen_sp17.root", hName.c_str());
	TH1F *h_sp18 = getAcceptanceCorrectedHist("hists/hist_dat_sp18.root", "hists/hist_acc_sp18.root", "hists/hist_gen_sp18.root", hName.c_str());
	TH1F *h_fa18 = getAcceptanceCorrectedHist("hists/hist_dat_fa18.root", "hists/hist_acc_fa18.root", "hists/hist_gen_fa18.root", hName.c_str());

	c = new TCanvas();
	h_sp17->GetXaxis()->SetRangeUser(min, max);

	TH1F *h = (TH1F*)h_sp17->Clone("h");
	h->Add(h_sp18);
	h->Add(h_fa18);

	char s[100];
	sprintf(s, "Acceptance Corrected Intensity / %.1f MeV", h->GetBinWidth(10)*1000);
	h->GetYaxis()->SetTitle(s);
	h->GetXaxis()->SetTitle("M(K_{S}K_{L}) (GeV)");

	TF1 *fit = new TF1("fit", model1, min, max, 9);
	fit->SetParameters(110, 1.50, 0.25, 100, 1.75, 0.12, 3.3+04, -1.7+04, 2.9+03);
	fit->SetParNames("N1", "M1", "#Gamma1", "N2", "M2", "#Gamma2", "a0", "a1", "a2");
	fit->SetLineWidth(3);
	fit->SetNpx(1000);

	TF1 *sig= new TF1("sig", modelSig1, min, max, 6);
	sig->SetLineColor(kBlue);
	sig->SetFillStyle(3002);
	sig->SetLineWidth(0);
	sig->SetFillColorAlpha(kAzure+8, 1.0);
	sig->SetNpx(1000);

	TF1 *bkg = new TF1("bkg", modelBkg, min, max, 3);
	bkg->SetLineColor(kBlack);
	bkg->SetLineWidth(3);

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

	c->SaveAs( "pdf_nInterference/"+sName+".pdf" );

	vector<double> pars = {
		fit->GetParameter(1), fit->GetParError(1),
		fit->GetParameter(2), fit->GetParError(2),
		fit->GetParameter(4), fit->GetParError(4),
		fit->GetParameter(5), fit->GetParError(5)
	};
	return pars;
}

vector<double> fit_2bw_with_interference(string hName, double min = 1.16, double max = 2.60, TString sName = "") {
	TCanvas *c;

	TH1F *h_sp17 = getAcceptanceCorrectedHist("hists/hist_dat_sp17.root", "hists/hist_acc_sp17.root", "hists/hist_gen_sp17.root", hName.c_str());
	TH1F *h_sp18 = getAcceptanceCorrectedHist("hists/hist_dat_sp18.root", "hists/hist_acc_sp18.root", "hists/hist_gen_sp18.root", hName.c_str());
	TH1F *h_fa18 = getAcceptanceCorrectedHist("hists/hist_dat_fa18.root", "hists/hist_acc_fa18.root", "hists/hist_gen_fa18.root", hName.c_str());

	h_sp17->GetXaxis()->SetRangeUser(min, max);
	TH1F *h = (TH1F*)h_sp17->Clone("h");
	h->Add(h_sp18);
	h->Add(h_fa18);

	char s[100];
	sprintf(s, "Acceptance Corrected Intensity / %.1f MeV", h->GetBinWidth(10)*1000);
	h->GetYaxis()->SetTitle(s);
	h->GetXaxis()->SetTitle("M(K_{S}K_{L}) (GeV)");

	TF1 *fit = new TF1("fit", model2, min, max, 10);
	fit->SetParameters(100, 1.50, 0.24, 100, 1.75, 0.13, 0.1, 3.3e+04, -1.7+04, 2.8+03);
	fit->SetParNames("N1", "M1", "#Gamma1", "N2", "M2", "#Gamma2", "#Delta#phi", "a0", "a1", "a2");
	// fit->SetParLimits(6, -5.6, -5.1);
	fit->SetLineWidth(3);
	fit->SetNpx(1000);

	TF1 *sig= new TF1("sig", modelSig2, min, max, 7);
	sig->SetLineColor(kBlue);
	sig->SetFillStyle(3002);
	sig->SetLineWidth(0);
	sig->SetFillColor(kAzure+8);
	sig->SetNpx(1000);

	TF1 *bkg = new TF1("bkg", modelBkg, min, max, 3);
	bkg->SetLineColor(kBlack);
	bkg->SetLineWidth(3);
	bkg->SetNpx(1000);

	c = new TCanvas();
	h->Fit(fit, "RE");
	//h->Draw();
	// fit->Draw("SAME");
	h->GetYaxis()->SetRangeUser(0.0, 1.05*h->GetMaximum());
	sig->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5), fit->GetParameter(6));
	sig->Draw("same");
	bkg->SetParameters(fit->GetParameter(7), fit->GetParameter(8), fit->GetParameter(9));
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
	sprintf(s, "%s = %.2f #pm %.2f", fit->GetParName(6), fit->GetParameter(6), fit->GetParError(6));
	lg->AddEntry((TObject*)0, s, "");
	sprintf(s, "#chi^{2}/ndf = %.0f/%d = %.2f", fit->GetChisquare(), fit->GetNDF(), fit->GetChisquare()/fit->GetNDF());
	lg->AddEntry((TObject*)0, s, "");
	lg->Draw();

	c->SaveAs( "pdf_wInterference/"+sName+".pdf" );

	vector<double> pars = {
		fit->GetParameter(1), fit->GetParError(1),
		fit->GetParameter(2), fit->GetParError(2),
		fit->GetParameter(4), fit->GetParError(4),
		fit->GetParameter(5), fit->GetParError(5),
		fit->GetParameter(6), fit->GetParError(6)
	};
	return pars;
}

Double_t model1(Double_t* x, Double_t* par) {
	double N1 = par[0];
	complex<double> bw1 = BreitWigner(x[0], par[1], par[2], 1, 0.497, 0.497);

	double N2 = par[3];
	complex<double> bw2 = BreitWigner(x[0], par[4], par[5], 1, 0.497, 0.497);

	return modelSig1(&x[0], &par[0]) + modelBkg(&x[0], &par[6]);
}

Double_t modelSig1(Double_t* x, Double_t* par) {
	double N1 = par[0];
	complex<double> bw1 = BreitWigner(x[0], par[1], par[2], 1, 0.497, 0.497);

	double N2 = par[3];
	complex<double> bw2 = BreitWigner(x[0], par[4], par[5], 1, 0.497, 0.497);

	return norm(N1*bw1) + norm(N2*bw2);
}

Double_t modelSig2(Double_t* x, Double_t* par) {
	double N1 = par[0];
	complex<double> bw1 = BreitWigner(x[0], par[1], par[2], 1, 0.497, 0.497);

	double N2 = par[3];
	complex<double> phase(cos(par[6]), sin(par[6]));
	complex<double> bw2 = BreitWigner(x[0], par[4], par[5], 1, 0.497, 0.497);

	return norm(N1*bw1 + N2*phase*bw2);
}

Double_t model2(Double_t* x, Double_t* par) {
	return modelSig2(&x[0], &par[0]) + modelBkg(&x[0], &par[7]);
}

Double_t modelBkg(Double_t* x, Double_t* par) {
	return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}
