#include "helper_functions.h"

vector<double> fit_2bw_with_interference(string hName, string cut, double min = 1.16, double max = 2.60);

Double_t model1(Double_t* x, Double_t* par);
Double_t modelSig1(Double_t* x, Double_t* par);

Double_t model2(Double_t* x, Double_t* par);
Double_t modelSig2(Double_t* x, Double_t* par);

Double_t modelBkg(Double_t* x, Double_t* par);

double calc_barlow(double nominal, double nominal_err, double variation, double variation_err);

void fit_wInterference_changeEventSelections() {
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
	vector<string> hNames = {"im_kskl", "im_kskl_mpipi1", "im_kskl_mpipi2", "im_kskl_mpipi3", "im_kskl_mmiss1", "im_kskl_mmiss2", "im_kskl_mandel_tp1", "im_kskl_mandel_tp2",
		"im_kskl_FS1", "im_kskl_FS2", "im_kskl_chisq1", "im_kskl_chisq2", "im_kskl_ntracks1",
		"im_kskl_nshowers1", "im_kskl_nshowers2", "im_kskl_nshowers3",  "im_kskl_proton_z_vertex1", "im_kskl_proton_z_vertex2"};

	map<string, string> hNamesMap = {
		{"im_kskl", ""},
		{"im_kskl_mpipi1", "M_{#pi#pi} Region 1"},
		{"im_kskl_mpipi2", "M_{#pi#pi} Region 2"},
 		{"im_kskl_mpipi3", "Wider Sideband"},
		{"im_kskl_mmiss1", "M_{miss} = (0.2,0.8)"},
		{"im_kskl_mmiss2", "M_{miss} = (0.35,0.65)"},
		{"im_kskl_mandel_tp1", "#minust' = (0.25,0.65)"},
		{"im_kskl_mandel_tp2", "#minust' = (0.15,0.8)"},
		{"im_kskl_FS1", "FS > 5"},
		{"im_kskl_FS2", "FS > 7"},
		{"im_kskl_chisq1", "#chi^{2}/ndf < 1.5"},
		{"im_kskl_chisq2", "#chi^{2}/ndf < 2.5"},
		{"im_kskl_ntracks1", "N_{tracks} < 2"},
		{"im_kskl_nshowers1", "N_{showers} < 2"},
		{"im_kskl_nshowers2", "N_{showers} < 4"},
		{"im_kskl_nshowers3", "N_{showers} < 1"},
		{"im_kskl_proton_z_vertex1", "Proton-Z = (51,79)"},
		{"im_kskl_proton_z_vertex2", "Proton-Z = (53,77)"}
		// {"im_kskl_beam_energy1", "E_{#gamma} = (8.2,8.5)"},
		// {"im_kskl_beam_energy2", "E_{#gamma} = (8.5,8.8)"}
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
		*g2_ni = new TGraphErrors(hNamesMap.size()-1),
		*g_phase = new TGraphErrors(hNamesMap.size()-1),
		
		*m1_ni_barlow = new TGraphErrors(hNamesMap.size()-1),
		*g1_ni_barlow = new TGraphErrors(hNamesMap.size()-1),
		*m2_ni_barlow = new TGraphErrors(hNamesMap.size()-1),
		*g2_ni_barlow = new TGraphErrors(hNamesMap.size()-1),
		*phase_barlow = new TGraphErrors(hNamesMap.size()-1);
	
	TH1F *frame = new TH1F("frame", "", hNames.size(), 0, hNames.size());
	frame->GetXaxis()->SetRangeUser(0, hNames.size()-1);
	// frame->GetXaxis()->SetLabelAngle(90);

	int count = 0;
	double m1, m1_err, g1, g1_err, m2, m2_err, g2, g2_err, phase, phase_err;
	for(string hName : hNames) {
		cout << hName << " : " << count << endl;
		frame->GetXaxis()->SetBinLabel(count, hNamesMap[hName].c_str());
		
		pars = fit_2bw_with_interference(hName, hNamesMap[hName]);
		
		if(count == 0) {
			m1 = pars[0];
			m1_err = pars[1];
			g1 = pars[2];
			g1_err = pars[3];
			m2 = pars[4];
			m2_err = pars[5];
			g2 = pars[6];
			g2_err = pars[7];
			phase = pars[8];
			phase_err = pars[9];
		}

		m1_ni->SetPoint(count, count-0.5, pars[0]);
		m1_ni->SetPointError(count, 0, pars[1]);
		g1_ni->SetPoint(count, count-0.5, pars[2]);
		g1_ni->SetPointError(count, 0, pars[3]);
		m2_ni->SetPoint(count, count-0.5, pars[4]);
		m2_ni->SetPointError(count, 0, pars[5]);
		g2_ni->SetPoint(count, count-0.5, pars[6]);
		g2_ni->SetPointError(count, 0, pars[7]);
		g_phase->SetPoint(count, count-0.5, pars[8]);
		g_phase->SetPointError(count, 0, pars[9]);

		m1_ni_barlow->SetPoint(count, count-0.5, calc_barlow(m1, m1_err, pars[0], pars[1]));
		g1_ni_barlow->SetPoint(count, count-0.5, calc_barlow(g1, g1_err, pars[2], pars[3]));
		m2_ni_barlow->SetPoint(count, count-0.5, calc_barlow(m2, m2_err, pars[4], pars[5]));
		g2_ni_barlow->SetPoint(count, count-0.5, calc_barlow(g2, g2_err, pars[6], pars[7]));
		phase_barlow->SetPoint(count, count-0.5, calc_barlow(phase, phase_err, pars[8], pars[9]));

		count++;
		frame->GetXaxis()->SetBinLabel(count, hNamesMap[hName].c_str());
	}

	TCanvas *c;
	
	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(m1 - 5*m1_err, m1 + 5*m1_err);
	frame->GetYaxis()->SetTitle("M_{1} Parameter (GeV)");
	frame->Draw();
	m1_ni->Draw("P");
	box->DrawBox(0, m1 - m1_err, hNames.size()-1, m1 + m1_err);
	line->DrawLine(0, m1, hNames.size()-1, m1);
	c->SaveAs("pdf_wInterference/eventSelectionVariation_m1Scan.pdf");

	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(g1 - 5*g1_err, g1 + 5*g1_err);
	frame->GetYaxis()->SetTitle("#Gamma_{1} Parameter (GeV)");
	frame->Draw();
	g1_ni->Draw("P");
	box->DrawBox(0, g1 - g1_err, hNames.size()-1, g1 + g1_err);
	line->DrawLine(0, g1, hNames.size()-1, g1);
	c->SaveAs("pdf_wInterference/eventSelectionVariation_g1Scan.pdf");

	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(m2 - 5*m2_err, m2 + 5*m2_err);
	frame->GetYaxis()->SetTitle("M_{2} Parameter (GeV)");
	frame->Draw();
	m2_ni->Draw("P");
	box->DrawBox(0, m2 - m2_err, hNames.size()-1, m2 + m2_err);
	line->DrawLine(0, m2, hNames.size()-1, m2);
	c->SaveAs("pdf_wInterference/eventSelectionVariation_m2Scan.pdf");

	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(g2 - 5*g2_err, g2 + 5*g2_err);
	frame->GetYaxis()->SetTitle("#Gamma_{2} Parameter (GeV)");
	frame->Draw();
	g2_ni->Draw("P");
	box->DrawBox(0, g2 - g2_err, hNames.size()-1, g2 + g2_err);
	line->DrawLine(0, g2, hNames.size()-1, g2);
	c->SaveAs("pdf_wInterference/eventSelectionVariation_g2Scan.pdf");

	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(0, TMath::Pi());
	frame->GetYaxis()->SetTitle("#Delta#phi Parameter (GeV)");
	frame->Draw();
	g_phase->Draw("P");
	if(phase-phase_err < 0) {
		box->DrawBox(0, 0, hNames.size()-1, phase + phase_err);
	}
	else {
		box->DrawBox(0, phase - phase_err, hNames.size()-1, phase + phase_err);
	}
	line->DrawLine(0, phase, hNames.size()-1, phase);
	c->SaveAs("pdf_wInterference/eventSelectionVariation_phaseScan.pdf");

	line->SetLineColor(kBlack);
	line->SetLineStyle(2);

	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(-6, 6);
	frame->GetYaxis()->SetTitle("M_{1} Parameter (GeV)");
	frame->Draw();
	line->DrawLine(0, -4, hNames.size()-1, -4);
	line->DrawLine(0, 4, hNames.size()-1, 4);
	m1_ni_barlow->Draw("P");
	c->SaveAs("pdf_wInterference/eventSelectionVariation_m1BarlowScan.pdf");

	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(-6, 6);
	frame->GetYaxis()->SetTitle("#Gamma_{1} Parameter (GeV)");
	frame->Draw();
	line->DrawLine(0, -4, hNames.size()-1, -4);
	line->DrawLine(0, 4, hNames.size()-1, 4);
	g1_ni_barlow->Draw("P");
	c->SaveAs("pdf_wInterference/eventSelectionVariation_g1BarlowScan.pdf");

	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(-6, 6);
	frame->GetYaxis()->SetTitle("M_{2} Parameter (GeV)");
	frame->Draw();
	line->DrawLine(0, -4, hNames.size()-1, -4);
	line->DrawLine(0, 4, hNames.size()-1, 4);
	m2_ni_barlow->Draw("P");
	c->SaveAs("pdf_wInterference/eventSelectionVariation_m2BarlowScan.pdf");

	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(-6, 6);
	frame->GetYaxis()->SetTitle("#Gamma_{2} Parameter (GeV)");
	frame->Draw();
	line->DrawLine(0, -4, hNames.size()-1, -4);
	line->DrawLine(0, 4, hNames.size()-1, 4);
	g2_ni_barlow->Draw("P");
	c->SaveAs("pdf_wInterference/eventSelectionVariation_g2BarlowScan.pdf");

	c = new TCanvas();
	c->SetBottomMargin(0.3);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(-6, 6);
	frame->GetYaxis()->SetTitle("#Delta#phi Parameter (GeV)");
	frame->Draw();
	line->DrawLine(0, -4, hNames.size()-1, -4);
	line->DrawLine(0, 4, hNames.size()-1, 4);
	phase_barlow->Draw("P");
	c->SaveAs("pdf_wInterference/eventSelectionVariation_phaseBarlowScan.pdf");

	TFile *opf = new TFile("rootFiles/graphs_EventSelelctionVariations_wInterference.root", "RECREATE");
	m1_ni->Write("eventSelectionVariation_m1Scan");
	g1_ni->Write("eventSelectionVariation_g1Scan");
	m2_ni->Write("eventSelectionVariation_m2Scan");
	g2_ni->Write("eventSelectionVariation_g2Scan");
}

vector<double> fit_2bw_with_interference(string hName, string cut, double min = 1.16, double max = 2.60) {
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
	// fit->SetParameters(100, 1.50, 0.24, 100, 1.75, 0.13, 0.1, 3.3e+04, -1.7+04, 2.8+03);
	fit->SetParameters(50, 1.50, 0.25, 100, 1.75, 0.12, 0.1, 3.3e+04, -1.7+04, 2.8+03);
	fit->SetParNames("N1", "M1", "#Gamma1", "N2", "M2", "#Gamma2", "#Delta#phi", "a0", "a1", "a2");
	// fit->SetParLimits(6, -5.6, -5.1);
	fit->SetParLimits(0, 0, 10000);
	fit->SetParLimits(1, 1.2, 1.6);
	fit->SetParLimits(2, 0.0, 4.0);
	fit->SetParLimits(3, 0, 10000);
	fit->SetParLimits(4, 1.6, 2.0);
	fit->SetParLimits(5, 0.0, 4.0);
	fit->SetParLimits(6, 0, TMath::Pi());
	fit->SetLineWidth(3);
	fit->SetNpx(1000);
	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(1000000);

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

	TLatex t;
	t.DrawLatex(1.25, 0.93*h->GetMaximum(), cut.c_str());

	c->SaveAs(("pdf_wInterference/"+hName+".pdf").c_str());

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

double calc_barlow(double nominal, double nominal_err, double variation, double variation_err) {
	return (nominal - variation) / TMath::Sqrt( abs(nominal_err*nominal_err - variation_err*variation_err) );
}