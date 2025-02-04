TH1F* getAcceptanceCorrectedHist(TString fdata, TString facc, TString fgen, TString hname);
Double_t BreitWigner(Double_t *x, Double_t *par);

void projection() {
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

	TCanvas *c;
	double min = 1.16, max = 2.60;

	TH1F *h_sp17 = getAcceptanceCorrectedHist("hists/hist_dat_sp17.root", "hists/hist_acc_sp17.root", "hists/hist_gen_sp17.root", "im_kskl");
	TH1F *h_sp18 = getAcceptanceCorrectedHist("hists/hist_dat_sp18.root", "hists/hist_acc_sp18.root", "hists/hist_gen_sp18.root", "im_kskl");
	TH1F *h_fa18 = getAcceptanceCorrectedHist("hists/hist_dat_fa18.root", "hists/hist_acc_fa18.root", "hists/hist_gen_fa18.root", "im_kskl");

    TFile *inf = TFile::Open("flux_gluex1.root");
    TH1F *h_lumi = (TH1F*)inf->Get("tagged_lumi");

	c = new TCanvas();

	TH1F *h = (TH1F*)h_sp17->Clone("h");
	h->Add(h_sp18);
	h->Add(h_fa18);

	h->GetXaxis()->SetRangeUser(min, max);
    h->GetYaxis()->SetRangeUser(0, 1.2*h->GetMaximum());
	h->GetYaxis()->SetTitle("Acceptance Corrected Intensity");

	double Y2239_cross_section = 475; // pb
	double lumi = h_lumi->Integral();
	double br_ks_to_pippim = 0.692;
	double scale = Y2239_cross_section*lumi*br_ks_to_pippim*h->GetBinWidth(1); // scale factor assuming Br(Y -> KsKl) = 1
	cout << scale << endl;

	TF1 *model1 = new TF1("proj1", BreitWigner, min, max, 3);
	model1->SetParameters(scale*2, 2.239, 0.140);
	model1->SetParNames("N", "M", "#Gamma");
	model1->SetLineColor(kRed);

	TF1 *model2 = new TF1("proj2", BreitWigner, min, max, 3);
	model2->SetParameters(scale, 2.239, 0.140);
	model2->SetLineColor(kOrange);

	TF1 *model3 = new TF1("proj3", BreitWigner, min, max, 3);
	model3->SetParameters(scale*0.3, 2.239, 0.140);
	model3->SetLineColor(kViolet);


    h->Draw();
	model1->Draw("SAME");
	model2->Draw("SAME");
	model3->Draw("SAME");

	TLegend *lg = new TLegend(0.6, 0.6, 0.97, 0.97);
	lg->AddEntry(model1, "#frac{Br(Y -> KsKl)}{Br(Y -> #phi#pi#pi)} = 2", "l");
	lg->AddEntry(model2, "#frac{Br(Y -> KsKl)}{Br(Y -> #phi#pi#pi)} = 1", "l");
	lg->AddEntry(model3, "#frac{Br(Y -> KsKl)}{Br(Y -> #phi#pi#pi)} = 0.3", "l");
	lg->Draw();

	TLegend *lg2 = new TLegend(0.16, 0.8, 0.45, 0.97);
	lg2->SetTextSize(0.02);
	char s[50];
	sprintf(s, "#sigma(#gamma p -> Yp)*Br(Y -> #phi#pi#pi) = %.1f pb", Y2239_cross_section);
	lg2->AddEntry((TObject*)0, s, "");
	sprintf(s, "Luminosity = %.1f pb^{-1}", lumi);
	lg2->AddEntry((TObject*)0, s, "");
	sprintf(s, "Br(K_{S} #rightarrow #pi^{+}#pi^{-}) = %.3f", br_ks_to_pippim);
	lg2->AddEntry((TObject*)0, s, "");
	sprintf(s, "Bin width = %.1f MeV", 1000*h->GetBinWidth(1));
	lg2->AddEntry((TObject*)0, s, "");
	lg2->Draw();

    c->SaveAs("projections.pdf");
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

Double_t BreitWigner(Double_t *x, Double_t *par) {
   Double_t bw = par[2]/((x[0]-par[1])*(x[0]-par[1]) + par[2]*par[2]/4);
   return par[0]*bw/(2*TMath::Pi());
}

