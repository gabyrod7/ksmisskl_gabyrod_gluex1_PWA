#include <iostream>
#include <fstream>

TH1F *get_hist(TH3F *h, TString name, double ymin, double ymax, double zmin, double zmax);
TString compose_fit_func(TString sig, TString bkg);
void format_canvas(TCanvas* c);

void decompose_fit(TF1 *fit, TF1 *sig, TF1 *bkg);
double calc_weighted_average(double N1, double x1, double N2, double x2);

void optimize_cuts(TH3F *h3);

void study_chisq_fs() {
	gStyle->SetOptStat(0);

	// check if hists direcotry exists (want to find better way)
	if(gSystem->AccessPathName("hists/")) {
		gSystem->Exec("mkdir hists/");
	}

	TFile *inf = TFile::Open("hist_dat_gluex1.root");
	if(inf == NULL) std::cout << "Name of ROOT file is incorrect" << std::endl;
	else			std::cout << "ROOT file is open" << std::endl;

	TH3F *h3 = (TH3F*)inf->Get("h3_mpipi_fs_chisq");
	if(h3 == NULL)	std::cout << "Histogram is empty" << std::endl;
	else			std::cout << "Loaded 3D histogram" << std::endl;

	optimize_cuts(h3);
}

// void optimize_cuts(TString input_file = "hist_gluex1.root", TString hist_name = "h3_mks2_fs_chisq_ndf") {
void optimize_cuts(TH3F *h3) {
	// ------------------------------------------------
	// INITIALIZE NECESARY VARIABLES
	// ------------------------------------------------
	int n = 21, m = 8;
	char name[50];
	double mean, width, signal, background, r1, r2,
			fs[n], chisq_ndf[m], purity[m][n], yield[m][n], significance[m][n], signal_background[m][n], criteria3[m][n], criteria4[m][n];

	TF1 *fit, *sig, *bkg;
	TH1F *h;
	TLegend *l, *l2 = new TLegend(0.25, 0.77, 0.95, 0.95);
	TGraph *pur[m], *yld[m], *signif[m], *sb[m], *c3[m], *c4[m];
	TBox *box;
	TLatex t;
	t.SetTextSize(0.07);

	// ------------------------------------------------
	// SET BINNING FOR STUDY
	// ------------------------------------------------
	fs[0] = 3;
	chisq_ndf[0] = 2;
	for(int i = 1; i < n; i++) 	fs[i] = fs[i-1] + 0.2;
	for(int i = 1; i < m; i++)	chisq_ndf[i] = chisq_ndf[i-1] + 0.5;

	// ------------------------------------------------
	// CONSTRUCT FITTING FUNCTIONS
	// ------------------------------------------------
	TString signal_func = "gausn(0) + gausn(3)";
	// TString signal_func = "gausn(0)";
	TString background_func = "pol2";
	TString fit_func = compose_fit_func(signal_func, background_func);
	fit = new TF1("fit", fit_func, 0.38, 0.555);
	fit->SetParameters(1, 0.497, 0.008, 1, 0.497, 0.008, -1, 1, 1, 1);

	sig = new TF1("sig", signal_func, 0.38, 0.555);
	bkg = new TF1("bkg", background_func, 0.38, 0.555);

	TCanvas *c = new TCanvas();
	format_canvas(c);
	for(int i = 0; i < m; i++) {
		for(int j = 0; j < n; j++) {
			std::cout << "###############" << std::endl;
			std::cout << "(fs, chisq_ndf) = (" << fs[j] << "," << chisq_ndf[i] << ")" << std::endl;

			sprintf(name, "mks_%d_%d", i, j);
			h = get_hist(h3, name, fs[j], 100., chisq_ndf[i], 100.);
			// if(j == 0)	fit->SetParameters(1, 0.497, 0.008, 1, 0.497, 0.008, 1, -1, -1);
			h->Fit(fit, "ER");

			decompose_fit(fit, sig, bkg);

			mean = calc_weighted_average(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(3), fit->GetParameter(4));
			width = calc_weighted_average(fit->GetParameter(0), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(5));

			signal = sig->Integral(mean - 2*width, mean + 2*width);
			background = bkg->Integral(mean - 2*width, mean + 2*width);

			r1 = signal / background;
			r2 = signal / TMath::Sqrt(signal + background);
			purity[i][j] = signal / (signal + background);
			yield[i][j] = signal / h->GetBinWidth(1);
			signal_background[i][j] = r1;
			significance[i][j] = r2;
			criteria3[i][j] = TMath::Sqrt(2*(signal + background)*TMath::Log(1 + signal/background) - 2*signal);
			criteria4[i][j] = 2*(TMath::Sqrt(signal + background) - TMath::Sqrt(background));

			cout << purity << endl;

			l = new TLegend(0.25, 0.6, 0.65, 0.95);
			l->SetBorderSize(0);
			l->AddEntry(h, "Data", "lep");
			l->AddEntry(fit, "Fit", "l");
			l->AddEntry(sig, "Signal", "l");
			l->AddEntry(bkg, "Background", "l");
			//l->AddEntry((TObject*)0, name, "");
			sprintf(name, "FS > %.1f", fs[j]);
			l->AddEntry((TObject*)0, name, "");
			sprintf(name, "#chi^{2}/ndf < %.1f", chisq_ndf[i]);
			l->AddEntry((TObject*)0, name, "");
			sprintf(name, "#mu = %.1f (MeV)", mean*1000);
			l->AddEntry((TObject*)0, name, "");
			sprintf(name, "#sigma = %.1f (MeV)", width*1000);
			l->AddEntry((TObject*)0, name, "");
			sprintf(name, "S = %.2f", signal);
			l->AddEntry((TObject*)0, name, "");
			// sprintf(name, "S/B = %.2f", r1);
			// l->AddEntry((TObject*)0, name, "");
			// sprintf(name, "S/#sqrt{S+B} = %.2f", r2);
			// l->AddEntry((TObject*)0, name, "");
			sprintf(name, "Purity = %.2f", purity[i][j]);
			l->AddEntry((TObject*)0, name, "");
			auto clone = (TH1F*)h->Clone();
			clone->GetXaxis()->SetRangeUser(0.48,0.52);
			clone->SetFillColorAlpha(kGreen, 0.2);
			clone->Draw("SAME HIST");
			sig->Draw("SAME");
			bkg->Draw("SAME");
			l->Draw("SAME");

			//box = new TBox(0.40, 0, 0.44, 0.25*h->GetMaximum());
			//box->SetFillColorAlpha(kRed, 0.4);
			//box->Draw("SAME");

			// sprintf(name, "#chi^{2}/ndf = %.1f", chisq);
			// t.DrawLatex(0.54, 0.6*h->GetMaximum(), name);

			// if(purity[i][j] > 0.898 && purity[i][j] < 0.902) {
			// 	sprintf(name, "%.2f & %.2f & %.2f & %.2f & %.0f \\\\", fs[j], chisq_ndf[i], chisq, purity[i][j], signal);
			// 	csv << name << endl;
			// }

			sprintf(name, "hists/%d_%d.pdf", i, j);
			c->SaveAs(name);

			//auto rp1 = new TRatioPlot(h);
			//rp1->Draw();
			//rp1->GetLowerRefYaxis()->SetRangeUser(-4, 4);
			//sprintf(name, "hists/ratio_%d_%d.pdf", i, j);
			//c->SaveAs(name);
		}

		pur[i] = new TGraph(n, fs, purity[i]);
		yld[i] = new TGraph(n, fs, yield[i]);
		sb[i] = new TGraph(n, fs, signal_background[i]);
		signif[i] = new TGraph(n, fs, significance[i]);
		c3[i] = new TGraph(n, fs, criteria3[i]);
		c4[i] = new TGraph(n, fs, criteria4[i]);

		pur[i]->SetMarkerStyle(8);
		yld[i]->SetMarkerStyle(8);
		sb[i]->SetMarkerStyle(8);
		signif[i]->SetMarkerStyle(8);
		c3[i]->SetMarkerStyle(8);
		c4[i]->SetMarkerStyle(8);

		pur[i]->SetMarkerColor(i+1);
		yld[i]->SetMarkerColor(i+1);
		sb[i]->SetMarkerColor(i+1);
		signif[i]->SetMarkerColor(i+1);
		c3[i]->SetMarkerColor(i+1);
		c4[i]->SetMarkerColor(i+1);

		sprintf(name, "#chi^{2} < %.1f", chisq_ndf[i]);
		l2->AddEntry(pur[i], name);
	}

	pur[0]->SetTitle(";Flight Significance (#sigma); Purity");
	pur[0]->GetYaxis()->SetRangeUser(0.70, 1.00);
	pur[0]->Draw("AP");
	for(int i = 1; i < m; i++)	pur[i]->Draw("SAME P");
	c->SaveAs("purity.pdf");
	// c = new TCanvas();
	yld[0]->SetTitle(";Flight Significance (#sigma); Yield");
	yld[0]->GetYaxis()->SetRangeUser(95000, 150000);
	yld[0]->Draw("AP");
	for(int i = 1; i < m; i++)	yld[i]->Draw("SAME P");
	l2->SetNColumns(4);
	l2->Draw("SAME");
	c->SaveAs("yield.pdf");
	// c = new TCanvas();
	sb[0]->SetTitle(";Flight Significance (#sigma); #frac{Signal}{Background}");
	sb[0]->GetYaxis()->SetRangeUser(0, 20);
	sb[0]->Draw("AP");
	for(int i = 1; i < m; i++)	sb[i]->Draw("SAME P");
	c->SaveAs("signal_background.pdf");
	// c = new TCanvas();
	signif[0]->SetTitle(";Flight Significance (#sigma); #frac{Signal}{ #sqrt{Signal + Background}}");
	signif[0]->GetYaxis()->SetRangeUser(9, 12);
	signif[0]->Draw("AP");
	for(int i = 1; i < m; i++)	signif[i]->Draw("SAME P");
	c->SaveAs("significance.pdf");
	// c = new TCanvas();
	c3[0]->SetTitle(";Flight Significance (#sigma); TMath::Sqrt(2*(signal + background)*TMath::Log(1 + signal/background) - 2*signal)");
	c3[0]->GetYaxis()->SetRangeUser(26, 28.5);
	c3[0]->Draw("AP");
	for(int i = 1; i < m; i++)	c3[i]->Draw("SAME P");
	c->SaveAs("criteria3.pdf");
	// c = new TCanvas();
	c4[0]->SetTitle(";Flight Significance (#sigma); 2*(TMath::Sqrt(signal + background) - TMath::Sqrt(background))");
	c4[0]->GetYaxis()->SetRangeUser(17.5, 20.5);
	c4[0]->Draw("AP");
	for(int i = 1; i < m; i++)	c4[i]->Draw("SAME P");
	c->SaveAs("criteria4.pdf");
	// sprintf(name, "#chi^{2}/ndf < %.1f", chisq);
	// t.DrawLatex(4.0, 0.66, name);
	// TLine *line = new TLine(fs[0], .9, fs[n-1], .9);
	// line->Draw();

	// sprintf(name, "pdf_of_hists/plot%d.pdf", k);
	c->SaveAs("name");
} // end function

TH1F *get_hist(TH3F *h, TString name, double ymin, double ymax, double zmin, double zmax) {
	TH1F *hist;
	char title[50];
	hist = (TH1F*)h->ProjectionX(name,
				h->GetYaxis()->FindFixBin(ymin), h->GetYaxis()->FindFixBin(ymax),
				h->GetZaxis()->FindFixBin(0.0), h->GetZaxis()->FindFixBin(zmin), "");

	// sprintf(title, "FS^{1} = %.1f, FS^{2} = %.1f", ymin, zmin);
	hist->SetTitle("");

	hist->GetXaxis()->SetTitle("M(#pi^{+}#pi^{-}) (GeV)");
	hist->GetXaxis()->SetRangeUser(0.38, 0.555);

	sprintf(title, "Counts / %.0f MeV", 1000*hist->GetBinWidth(1));
	hist->GetYaxis()->SetTitle(title);

	hist->GetYaxis()->SetRangeUser(0, 1.1*hist->GetMaximum());

	hist->GetYaxis()->SetTitleSize(0.06);
    hist->GetYaxis()->SetTitleOffset(1.35);
    hist->GetYaxis()->SetLabelSize(0.06);
    hist->GetXaxis()->SetTitleSize(0.06);
    hist->GetXaxis()->SetLabelSize(0.06);
	hist->GetXaxis()->SetLabelOffset(0.02);
	hist->GetXaxis()->SetTitleOffset(1.3);
    hist->SetMarkerStyle(8);
    hist->SetMarkerSize(1.5);

	return hist;
}

TString compose_fit_func(TString sig, TString bkg) {
	TString fit;
	TF1 *f = new TF1("f", sig, 0.4, 0.6);

	char index[5];
	sprintf(index, "(%d)", f->GetNpar());

	fit.Append(sig);
	fit.Append(" + ");
	fit.Append(bkg);
	fit.Append(index);

	return fit;
}

void decompose_fit(TF1 *fit, TF1 *sig, TF1 *bkg) {
	for(int i = 0; i < sig->GetNpar(); i++)
		sig->SetParameter(i, fit->GetParameter(i));
	for(int i = 0; i < bkg->GetNpar(); i++)
		bkg->SetParameter(i, fit->GetParameter(i+sig->GetNpar()));

	sig->SetLineColor(kBlue);
	bkg->SetLineColor(kBlack);
}

double calc_weighted_average(double N1, double x1, double N2, double x2) {
	return (N1*x1 + N2*x2) / (N1 + N2);
}

void format_canvas(TCanvas* c) {
    c->SetTopMargin(0.03);
    c->SetRightMargin(0.03);
    c->SetBottomMargin(0.17);
    c->SetLeftMargin(0.17);

    // c->SetGridx();
    // c->SetGridy();
}
