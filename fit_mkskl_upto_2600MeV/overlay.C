#include "helper_functions.h"

TH2F* getAcceptanceCorrectedHist2D(TString fdata, TString facc, TString fgen, TString hname);
Double_t myfit(Double_t* x, Double_t* par);
Double_t mysig(Double_t* x, Double_t* par);
Double_t mybkg(Double_t* x, Double_t* par);

void overlay() {
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
    TH1F *h[4];
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

	TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
    for(int iBin = 0; iBin < tBins.size()-1; iBin++) {
        h[iBin] = (TH1F*)h2->ProjectionX(Form("h_%d", iBin), h2->GetYaxis()->FindBin(tBins[iBin]), h2->GetYaxis()->FindBin(tBins[iBin+1]) - 1);
        h[iBin]->GetXaxis()->SetRangeUser(min, max);
		h[iBin]->SetLineColor(iBin+1);
		h[iBin]->SetLineWidth(2);

		leg->AddEntry(h[iBin], Form("-t'=(%.2f,%.2f)", tBins[iBin], tBins[iBin+1]), "l");

		if(iBin == 0) {
			h[iBin]->Draw("HIST");
		}
		else {
			h[iBin]->Scale(h[0]->GetMaximum()/h[iBin]->GetMaximum());
			h[iBin]->Draw("SAME HIST");
		}
    }
	h[0]->GetYaxis()->SetRangeUser(0, 1.1*h[0]->GetMaximum());
	leg->Draw();
	c->SaveAs("tmp.pdf");
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
