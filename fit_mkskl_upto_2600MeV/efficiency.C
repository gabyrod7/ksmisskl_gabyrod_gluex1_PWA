TH1F* getAcceptance(string acc, string gen, string histname);

void efficiency() {
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
	gStyle->SetMarkerSize(0.);

	gROOT->ForceStyle();

	TCanvas *c;
    TLegend *lg;
	double min = 1.15, max = 2.60;

	TH1F *h_sp17 = getAcceptance("hists/hist_acc_sp17.root", "hists/hist_gen_sp17.root", "im_kskl");
	TH1F *h_sp18 = getAcceptance("hists/hist_acc_sp18.root", "hists/hist_gen_sp18.root", "im_kskl");
	TH1F *h_fa18 = getAcceptance("hists/hist_acc_fa18.root", "hists/hist_gen_fa18.root", "im_kskl");

    c = new TCanvas();
    h_sp17->GetXaxis()->SetRangeUser(min, max);
    h_sp17->GetYaxis()->SetRangeUser(0, 0.15);
    h_sp17->GetXaxis()->SetTitle("M(K_{S}K_{L}) (GeV)");
    h_sp17->GetYaxis()->SetTitle("Efficiency (%)");

    h_sp17->SetLineColor(kRed);
    h_sp18->SetLineColor(kGreen);
    h_fa18->SetLineColor(kBlue);

    h_sp17->Draw("hist");
    h_sp18->Draw("hist same");
    h_fa18->Draw("hist same");

    lg = new TLegend(0.67, 0.67, 0.95, 0.95);
    lg->AddEntry(h_sp17, "Spring 2017", "l");
    lg->AddEntry(h_sp18, "Spring 2018", "l");
    lg->AddEntry(h_fa18, "Fall 2018", "l");
    lg->Draw();

    c->SaveAs("efficiency.pdf");
}

TH1F* getAcceptance(string acc, string gen, string histname) {
    TFile *inf1 = TFile::Open(acc.c_str());
    TFile *inf2 = TFile::Open(gen.c_str());

    TH1F *h1 = (TH1F*)inf1->Get(histname.c_str());
    TH1F *h2 = (TH1F*)inf2->Get(histname.c_str());

    h1->Divide(h2);

    return h1;
}