#include "helper_functions.h"

TH2F* getAcceptanceCorrectedHist2D(TString fdata, TString facc, TString fgen, TString hname);
Double_t myfit(Double_t* x, Double_t* par);
Double_t mysig(Double_t* x, Double_t* par);
Double_t mybkg(Double_t* x, Double_t* par);

void fit_mkskl_tBins_interference() {
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

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(10000);

	gROOT->ForceStyle();

	TCanvas *c;
    TH1F *h;
	double min = 1.16, max = 2.60;
    vector<double> tBins = {0.2, 0.26, 0.36, 0.6, 1.0};
    // vector<double> tBins = {0.2, 1.0};
	TGraphErrors *m1 = new TGraphErrors(tBins.size()-1),
		*m2 = new TGraphErrors(tBins.size()-1),
		*g1 = new TGraphErrors(tBins.size()-1),
		*g2 = new TGraphErrors(tBins.size()-1),
		*phase = new TGraphErrors(tBins.size()-1);

	TH2F *h_sp17 = getAcceptanceCorrectedHist2D("hists/hist_dat_sp17.root", "hists/hist_acc_sp17.root", "hists/hist_gen_sp17.root", "im_kskl_tp");
	TH2F *h_sp18 = getAcceptanceCorrectedHist2D("hists/hist_dat_sp18.root", "hists/hist_acc_sp18.root", "hists/hist_gen_sp18.root", "im_kskl_tp");
	TH2F *h_fa18 = getAcceptanceCorrectedHist2D("hists/hist_dat_fa18.root", "hists/hist_acc_fa18.root", "hists/hist_gen_fa18.root", "im_kskl_tp");

	c = new TCanvas();
	h_sp17->GetXaxis()->SetRangeUser(min, max);

	TH2F *h2 = (TH2F*)h_sp17->Clone("h");
	h2->Add(h_sp18);
	h2->Add(h_fa18);

    TF1 *fit = new TF1("fit", myfit, min, max, 10);
	fit->SetParameters(50, 1.50, 0.22, 10, 1.78, 0.13, 1., 1.71515e+04, -1.92535e+03, -1.80970e+03);
	fit->SetParNames("N1", "M1", "#Gamma1", "N2", "M2", "#Gamma2", "#Delta#phi", "a0", "a1", "a2");
	fit->SetParLimits(0, 0, 1000);
	fit->SetParLimits(1, 1.0, 2.0);
	fit->SetParLimits(2, 0, 0.5);
	fit->SetParLimits(3, 0, 1000);
	fit->SetParLimits(4, 1.0, 2.0);
	fit->SetParLimits(5, 0, 0.4);
	fit->SetParLimits(6, 0, TMath::Pi());
    fit->SetLineWidth(3);
    fit->SetNpx(1000);

    TF1 *sig= new TF1("sig", mysig, min, max, 7);
    sig->SetLineColor(kBlue);
    sig->SetFillStyle(3002);
    sig->SetLineWidth(0);
    sig->SetFillColorAlpha(kAzure+8, 1.0);
    sig->SetNpx(1000);

	TF1 *bw1= new TF1("bw1", mybw, min, max, 3);
	bw1->SetLineColor(kViolet);
	bw1->SetLineWidth(3);
	bw1->SetNpx(1000);

	TF1 *bw2= new TF1("bw2", mybw, min, max, 3);
	bw2->SetLineColor(kOrange+8);
	bw2->SetLineWidth(3);
	bw2->SetNpx(1000);

    TF1 *bkg = new TF1("bkg", mybkg, min, max, 3);
    bkg->SetLineColor(kBlack);
    bkg->SetLineWidth(3);

    for(int iBin = 0; iBin < tBins.size()-1; iBin++) {
        h = (TH1F*)h2->ProjectionX(Form("h_%d", iBin), h2->GetYaxis()->FindBin(tBins[iBin]), h2->GetYaxis()->FindBin(tBins[iBin+1]) - 1);
        h->GetXaxis()->SetRangeUser(min, max);
        // fit->SetParameters(10, 1.50, 0.246, 10, 1.78, 0.127, 1, 1, 1);
        // cout bin numbers
        // cout << h2->GetYaxis()->FindBin(tBins[iBin]) << " " << h2->GetYaxis()->FindBin(tBins[iBin+1]) - 1 << endl;
        // cout << h->Integral() << endl;

        char s[100];
        sprintf(s, "Acceptance Corrected Intensity / %.1f MeV", h->GetBinWidth(10)*1000);
        h->GetYaxis()->SetTitle(s);
        h->GetXaxis()->SetTitle("M(K_{S}K_{L}) (GeV)");

        h->Fit(fit, "RE");
        h->GetYaxis()->SetRangeUser(0.0, 1.05*h->GetMaximum());
        sig->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5), fit->GetParameter(6));
        sig->Draw("same");
        bkg->SetParameters(fit->GetParameter(7), fit->GetParameter(8), fit->GetParameter(9));
        bkg->Draw("SAME");
    	bw1->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2));
	    bw1->Draw("SAME");
	    bw2->SetParameters(fit->GetParameter(3), fit->GetParameter(4), fit->GetParameter(5));
    	bw2->Draw("SAME");

        TLegend *lg = new TLegend(0.6, 0.42, 0.96, 0.97);
        lg->SetFillStyle(0);
        lg->SetBorderSize(0);

        lg->AddEntry(h, "Data");
        lg->AddEntry(fit, "Fit", "l");
        lg->AddEntry(sig, "Signal", "lf");
        lg->AddEntry(bkg, "Background", "l");
        // add tbins to legend
        lg->AddEntry((TObject*)0, Form("#minust = (%.2f, %.2f)", tBins[iBin], tBins[iBin+1]));

        lg->AddEntry((TObject*)0, "", "");
        sprintf(s, "#splitline{%s = %.3f #pm %.3f GeV}{%s = %.3f #pm %.3f GeV}", fit->GetParName(1), fit->GetParameter(1), fit->GetParError(1), fit->GetParName(2), fit->GetParameter(2), fit->GetParError(2));
        lg->AddEntry(bw1, s, "l");
        lg->AddEntry((TObject*)0, "", "");
        sprintf(s, "#splitline{%s = %.3f #pm %.3f GeV}{%s = %.3f #pm %.3f GeV}", fit->GetParName(4), fit->GetParameter(4), fit->GetParError(4), fit->GetParName(5), fit->GetParameter(5), fit->GetParError(5));
        lg->AddEntry(bw2, s, "l");
        lg->AddEntry((TObject*)0, "", "");
        lg->AddEntry((TObject*)0, Form("#Delta#phi= %.2f #pm %.2f", fit->GetParameter(6), fit->GetParError(6)));
        sprintf(s, "#chi^{2}/ndf = %.0f/%d = %.2f", fit->GetChisquare(), fit->GetNDF(), fit->GetChisquare()/fit->GetNDF());
        lg->AddEntry((TObject*)0, s, "");
        lg->Draw();

        c->SaveAs(Form("tBinsInterference/h_tBin_%.2f_%.2f.pdf", tBins[iBin], tBins[iBin+1]));

		double binCenter = (tBins[iBin+1] + tBins[iBin])/2.0;
		double binWidth = (tBins[iBin+1] - tBins[iBin])/2.0;

		m1->SetPoint(iBin, binCenter, fit->GetParameter(1));
		m1->SetPointError(iBin, binWidth, fit->GetParError(1));

		g1->SetPoint(iBin, binCenter, fit->GetParameter(2));
		g1->SetPointError(iBin, binWidth, fit->GetParError(2));

		m2->SetPoint(iBin, binCenter, fit->GetParameter(4));
		m2->SetPointError(iBin, binWidth, fit->GetParError(4));

		g2->SetPoint(iBin, binCenter, fit->GetParameter(5));
		g2->SetPointError(iBin, binWidth, fit->GetParError(5));

		phase->SetPoint(iBin, binCenter, fit->GetParameter(6));
		phase->SetPointError(iBin, binWidth, fit->GetParError(6));
    }

	TBox *box = new TBox(0, 0, 1., 1.0);
	box->SetFillColorAlpha(kGreen, 0.2);

	TLine *line = new TLine(1.16, 1.0, 2.60, 1.0);
	line->SetLineColor(kGreen);
	line->SetLineWidth(2);

	c = new TCanvas();
	m1->SetTitle(";-t (GeV^{2});M_{1} Parameter (GeV)");
	m1->GetXaxis()->SetRangeUser(tBins[0],tBins[tBins.size()-1]);
	m1->GetYaxis()->SetRangeUser(1.46, 1.6);
	m1->Draw("AP");
	box->DrawBox(tBins[0], 1.534, tBins[tBins.size()-1], 1.556);
	line->DrawLine(tBins[0], 1.545, tBins[tBins.size()-1], 1.545);
	c->SaveAs("tBinsInterference/m1.pdf");

	c = new TCanvas();
	g1->SetTitle(";-t (GeV^{2});#Gamma_{1} Parameter (GeV)");
	g1->GetXaxis()->SetRangeUser(tBins[0], tBins[tBins.size()-1]);
	g1->GetYaxis()->SetRangeUser(0., 0.4);
	g1->Draw("AP");
	box->DrawBox(tBins[0], 0.242, tBins[tBins.size()-1], 0.285);
	line->DrawLine(tBins[0], 0.263, tBins[tBins.size()-1], 0.263);
	c->SaveAs("tBinsInterference/g1.pdf");

	c = new TCanvas();
	m2->SetTitle(";-t (GeV^{2});M_{2} Parameter (GeV)");
	m2->GetXaxis()->SetRangeUser(tBins[0], tBins[tBins.size()-1]);
	m2->GetYaxis()->SetRangeUser(1.71, 1.79);
	m2->Draw("AP");
	box->DrawBox(tBins[0], 1.742, tBins[tBins.size()-1], 1.757);
	line->DrawLine(tBins[0], 1.750, tBins[tBins.size()-1], 1.750);
	c->SaveAs("tBinsInterference/m2.pdf");

	c = new TCanvas();
	g2->SetTitle(";-t (GeV^{2});#Gamma_{2} Parameter (GeV)");
	g2->GetXaxis()->SetRangeUser(tBins[0], tBins[tBins.size()-1]);
	g2->GetYaxis()->SetRangeUser(0.0, 0.3);
	g2->Draw("AP");
	box->DrawBox(tBins[0], 0.115, tBins[tBins.size()-1], 0.135);
	line->DrawLine(tBins[0], 0.125, tBins[tBins.size()-1], 0.125);
	c->SaveAs("tBinsInterference/g2.pdf");

	c = new TCanvas();
	phase->SetTitle(";-t (GeV^{2});#Delta#phi Parameter (GeV)");
	phase->GetXaxis()->SetRangeUser(tBins[0], tBins[tBins.size()-1]);
	phase->GetYaxis()->SetRangeUser(0., 4);
	phase->Draw("AP");
	box->DrawBox(tBins[0], 0.35, tBins[tBins.size()-1], 0.99);
	line->DrawLine(tBins[0], 0.67, tBins[tBins.size()-1], 0.67);
	c->SaveAs("tBinsInterference/phase.pdf");
}

TH2F* getAcceptanceCorrectedHist2D(TString fdata, TString facc, TString fgen, TString hname) {
	TFile *inf = TFile::Open(fdata);
	TH2F *h = (TH2F*)inf->Get(hname);
	TH2F *h_sb = (TH2F*)inf->Get(hname + "_sb");
	h->Add(h_sb, -1);

	TFile *inf2 = TFile::Open(facc);
	TH2F *h_acc = (TH2F*)inf2->Get(hname);

	TFile *inf3 = TFile::Open(fgen);
	TH2F *h_gen = (TH2F*)inf3->Get(hname);

	h_acc->Divide(h_gen);
	h->Divide(h_acc);

	return h;
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

Double_t mybkg(Double_t* x, Double_t* par) {
	return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}
