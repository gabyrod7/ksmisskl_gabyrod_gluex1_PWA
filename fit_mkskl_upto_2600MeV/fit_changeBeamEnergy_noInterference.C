#include "helper_functions.h"

vector<double> fit_2bw_no_interference(string hName, string cut, double min = 1.16, double max = 2.60);

Double_t model1(Double_t* x, Double_t* par);
Double_t modelSig1(Double_t* x, Double_t* par);

Double_t model2(Double_t* x, Double_t* par);
Double_t modelSig2(Double_t* x, Double_t* par);

Double_t modelBkg(Double_t* x, Double_t* par);

double calc_pull(double nominal, double nominal_err, double variation, double variation_err);

void fit_changeBeamEnergy_noInterference() {
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

	// gStyle->SetPadGridX(true);

	gROOT->ForceStyle();

	vector<double> pars;
	vector<string> hNames = {"im_kskl", "im_kskl_beam_energy1", "im_kskl_beam_energy2", "im_kskl_beam_energy3", "im_kskl_beam_energy4", "im_kskl_beam_energy5", "im_kskl_beam_energy6"};

	map<string, string> hNamesMap = {
		{"im_kskl", "Nominal"},
		{"im_kskl_beam_energy1", "E_{#gamma} = (8.2,8.3)"},
		{"im_kskl_beam_energy2", "E_{#gamma} = (8.3,8.4)"},
		{"im_kskl_beam_energy3", "E_{#gamma} = (8.4,8.5)"},
		{"im_kskl_beam_energy4", "E_{#gamma} = (8.5,8.6)"},
		{"im_kskl_beam_energy5", "E_{#gamma} = (8.6,8.7)"},
		{"im_kskl_beam_energy6", "E_{#gamma} = (8.7,8.8)"}
	};


	TBox *box = new TBox(1.16, 0, 1.35, 1.0);
	box->SetFillColorAlpha(kGreen, 0.2);

	TLine *line = new TLine(1.16, 1.0, 2.60, 1.0);
	line->SetLineColor(kGreen);
	line->SetLineWidth(2);

	TGraphErrors 
		*m1_ni = new TGraphErrors(hNamesMap.size()-1),
		*g1_ni = new TGraphErrors(hNamesMap.size()-1),
		*m2_ni = new TGraphErrors(hNamesMap.size()-1),
		*g2_ni = new TGraphErrors(hNamesMap.size()-1);
	
	TH1F *frame = new TH1F("frame", "", hNames.size(), 0, hNames.size());
	frame->GetXaxis()->SetRangeUser(0, hNames.size()-1);
	// frame->GetXaxis()->SetLabelAngle(90);

	int count = 0;
	double m1, m1_err, g1, g1_err, m2, m2_err, g2, g2_err;
	for(string hName : hNames) {
		cout << hName << " : " << count << endl;
		frame->GetXaxis()->SetBinLabel(count, hNamesMap[hName].c_str());
		
		pars = fit_2bw_no_interference(hName, hNamesMap[hName]);
		
		if(count == 0) {
			m1 = pars[0];
			m1_err = pars[1];
			g1 = pars[2];
			g1_err = pars[3];
			m2 = pars[4];
			m2_err = pars[5];
			g2 = pars[6];
			g2_err = pars[7];
		}

		m1_ni->SetPoint(count, count-0.5, pars[0]);
		m1_ni->SetPointError(count, 0, pars[1]);
		g1_ni->SetPoint(count, count-0.5, abs(pars[2]));
		g1_ni->SetPointError(count, 0, pars[3]);
		m2_ni->SetPoint(count, count-0.5, pars[4]);
		m2_ni->SetPointError(count, 0, pars[5]);
		g2_ni->SetPoint(count, count-0.5, abs(pars[6]));
		g2_ni->SetPointError(count, 0, pars[7]);

		count++;
		frame->GetXaxis()->SetBinLabel(count, hNamesMap[hName].c_str());
	}

	cout << "m1 pull: " << calc_pull(m1_ni->GetPointY(1), m1_ni->GetErrorY(1), m1_ni->GetPointY(2), m1_ni->GetErrorY(2)) << " | diff. = " << m1_ni->GetPointY(1) - m1_ni->GetPointY(2) << endl;
	cout << "g1 pull: " << calc_pull(g1_ni->GetPointY(1), g1_ni->GetErrorY(1), g1_ni->GetPointY(2), g1_ni->GetErrorY(2)) << " | diff. = " << g1_ni->GetPointY(1) - g1_ni->GetPointY(2) << endl;
	cout << "m2 pull: " << calc_pull(m2_ni->GetPointY(1), m2_ni->GetErrorY(1), m2_ni->GetPointY(2), m2_ni->GetErrorY(2)) << " | diff. = " << m2_ni->GetPointY(1) - m2_ni->GetPointY(2) << endl;
	cout << "g2 pull: " << calc_pull(g2_ni->GetPointY(1), g2_ni->GetErrorY(1), g2_ni->GetPointY(2), g2_ni->GetErrorY(2)) << " | diff. = " << g2_ni->GetPointY(1) - g2_ni->GetPointY(2) << endl;

	TCanvas *c;
	
	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(m1 - 3*m1_err, m1 + 3*m1_err);
	frame->GetYaxis()->SetTitle("M_{1} Parameter (GeV)");
	frame->Draw();
	m1_ni->Draw("P");
	box->DrawBox(0, m1 - m1_err, hNames.size()-1, m1 + m1_err);
	line->DrawLine(0, m1, hNames.size()-1, m1);
	c->SaveAs("pdf_nInterference/beamEnergy_m1Scan.pdf");

	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(g1 - 3*g1_err, g1 + 3*g1_err);
	frame->GetYaxis()->SetTitle("#Gamma_{1} Parameter (GeV)");
	frame->Draw();
	g1_ni->Draw("P");
	box->DrawBox(0, g1 - g1_err, hNames.size()-1, g1 + g1_err);
	line->DrawLine(0, g1, hNames.size()-1, g1);
	c->SaveAs("pdf_nInterference/beamEnergy_g1Scan.pdf");

	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(m2 - 3*m2_err, m2 + 3*m2_err);
	frame->GetYaxis()->SetTitle("M_{2} Parameter (GeV)");
	frame->Draw();
	m2_ni->Draw("P");
	box->DrawBox(0, m2 - m2_err, hNames.size()-1, m2 + m2_err);
	line->DrawLine(0, m2, hNames.size()-1, m2);
	c->SaveAs("pdf_nInterference/beamEnergy_m2Scan.pdf");

	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(g2 - 7*g2_err, g2 + 7*g2_err);
	frame->GetYaxis()->SetTitle("#Gamma_{2} Parameter (GeV)");
	frame->Draw();
	g2_ni->Draw("P");
	box->DrawBox(0, g2 - g2_err, hNames.size()-1, g2 + g2_err);
	line->DrawLine(0, g2, hNames.size()-1, g2);
	c->SaveAs("pdf_nInterference/beamEnergy_g2Scan.pdf");

	line->SetLineColor(kBlack);
	line->SetLineStyle(2);

	TFile *opf = new TFile("rootFiles/graphs_beamEnergy.root", "RECREATE");
	m1_ni->Write("m1");
	g1_ni->Write("g1");
	m2_ni->Write("m2");
	g2_ni->Write("g2");
}

vector<double> fit_2bw_no_interference(string hName, string cut, double min = 1.16, double max = 2.60) {
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
	// fit->SetParameters(110, 1.50, 0.25, 100, 1.75, 0.12, 3.3+04, -1.7+04, 2.9+03);
	if(cut == "E_{#gamma} = (8.2,8.3)") {
		fit->SetParameters(110, 1.50, 0.25, 100, 1.75, 0.12, 1, -1, 1);
	}
	else {
		fit->SetParameters(100, 1.50, 0.25, 100, 1.75, 0.12, 4e3, -1e3, 1e2);
	}
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

	TLatex t;
	t.DrawLatex(1.25, 0.93*h->GetMaximum(), cut.c_str());

	c->SaveAs(("pdf_nInterference/"+hName+".pdf").c_str());

	vector<double> pars = {
		fit->GetParameter(1), fit->GetParError(1),
		fit->GetParameter(2), fit->GetParError(2),
		fit->GetParameter(4), fit->GetParError(4),
		fit->GetParameter(5), fit->GetParError(5)
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

double calc_pull(double nominal, double nominal_err, double variation, double variation_err) {
	return (nominal - variation) / TMath::Sqrt( abs(nominal_err*nominal_err + variation_err*variation_err) );
}