#include "helper_functions.h"

Double_t myfit(Double_t* x, Double_t* par); 
Double_t mysig(Double_t* x, Double_t* par);
Double_t mybkg(Double_t* x, Double_t* par);

void fit_wInterference_2bw() {
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
	gStyle->SetMarkerSize(1.0);

	gROOT->ForceStyle();

	TCanvas *c;
	double min = 1.16, max = 2.60;

	TH1F *h_sp17 = getAcceptanceCorrectedHist("hists/hist_dat_sp17.root", "hists/hist_acc_sp17.root", "hists/hist_gen_sp17.root", "im_kskl");
	TH1F *h_sp18 = getAcceptanceCorrectedHist("hists/hist_dat_sp18.root", "hists/hist_acc_sp18.root", "hists/hist_gen_sp18.root", "im_kskl");
	TH1F *h_fa18 = getAcceptanceCorrectedHist("hists/hist_dat_fa18.root", "hists/hist_acc_fa18.root", "hists/hist_gen_fa18.root", "im_kskl");

	h_sp17->GetXaxis()->SetRangeUser(min, max);
	TH1F *h = (TH1F*)h_sp17->Clone("h");
	h->Add(h_sp18);
	h->Add(h_fa18);

	char s[100];
	sprintf(s, "Acceptance Corrected Intensity / %.1f MeV", h->GetBinWidth(10)*1000);
	h->GetYaxis()->SetTitle(s);
	h->GetXaxis()->SetTitle("M(K_{S}K_{L}) (GeV)");

	TF1 *fit = new TF1("fit", myfit, min, max, 10);
	// fit->SetParameters(10, 1.50, 0.22, 10, 1.78, 0.13, 0., 1.71515e+04, -1.92535e+03, -1.80970e+03);
	fit->SetParameters(50, 1.50, 0.25, 100, 1.75, 0.12, 0.1, 3.3e+04, -1.7+04, 2.8+03);
	fit->SetParNames("N1", "M1", "#Gamma1", "N2", "M2", "#Gamma2", "#Delta#phi", "a0", "a1", "a2");
	fit->SetParLimits(0, 0, 10000);
	fit->SetParLimits(1, 1.2, 1.6);
	fit->SetParLimits(2, 0.0, 4.0);
	fit->SetParLimits(3, 0, 10000);
	fit->SetParLimits(4, 1.6, 2.0);
	fit->SetParLimits(5, 0.0, 4.0);
	fit->SetParLimits(6, 0, TMath::Pi());
	fit->SetLineWidth(3);
	fit->SetNpx(1000);

	TF1 *sig= new TF1("sig", mysig, min, max, 7);
	sig->SetLineColor(kBlue);
	sig->SetFillStyle(3002);
	sig->SetLineWidth(0);
	sig->SetFillColor(kAzure+8);
	sig->SetNpx(1000);

	TF1 *bkg = new TF1("bkg", mybkg, min, max, 3);
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

	// sprintf(s, "Counts / %.1f MeV", h->GetBinWidth(10)*1000);
	// h->GetYaxis()->SetTitle(s);

	c->SaveAs("pdfs/kskl_2bw_with_interference.pdf");

	//c = new TCanvas();
	//auto rp = new TRatioPlot(h);
	//rp->Draw();

	//auto opf = TFile::Open("fit.root", "recreate");
	//h->Write();
}

Double_t mybkg(Double_t* x, Double_t* par) {
	return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}

Double_t mysig(Double_t* x, Double_t* par) {
	double N1 = par[0];
	complex<double> bw1 = BreitWigner(x[0], par[1], par[2], 1, 0.497, 0.497);

	double N2 = par[3];
	complex<double> phase(cos(par[6]), sin(par[6]));
	complex<double> bw2 = BreitWigner(x[0], par[4], par[5], 1, 0.497, 0.497);

	return norm(N1*bw1 + N2*phase*bw2);
}

Double_t myfit(Double_t* x, Double_t* par) {
	return mysig(&x[0], &par[0]) + mybkg(&x[0], &par[7]);
}
