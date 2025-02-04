TCanvas* twoscales(TH1F* h1, TH1F* h2);

void fig_mkskl() {
	gStyle->SetOptStat(0);
	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.09);
	gStyle->SetPadBottomMargin(0.15);
	gStyle->SetPadLeftMargin(0.14);

	gStyle->SetTitleBorderSize(0);
		
	gStyle->SetTitleSize(0.06,"XY");
	gStyle->SetLabelSize(0.06,"XY");
	gStyle->SetLabelOffset(0.02,"XY");  
	
	gStyle->SetTitleSize(0.06,"XY");
	gStyle->SetTitleOffset(1.15,"X");
	gStyle->SetTitleOffset(1.25,"Y");
	
	gStyle->SetMarkerSize(1.5);
	gROOT->ForceStyle();

	// TFile *inf1 = TFile::Open("hist_dat.root");
	// TFile *inf2 = TFile::Open("hist_acc.root");
	// TFile *inf3 = TFile::Open("hist_gen.root");

	// TH1F *h1 = (TH1F*)inf1->Get("im_kskl");
	// TH1F *h1_sb = (TH1F*)inf1->Get("im_kskl_sb");
	// TH1F *h2 = (TH1F*)inf2->Get("im_kskl");
	// TH1F *h3 = (TH1F*)inf3->Get("im_kskl");

	TFile *inf1_sp17 = TFile::Open("hists/hist_dat_sp17.root");
	TFile *inf2_sp17 = TFile::Open("hists/hist_acc_sp17.root");
	TFile *inf3_sp17 = TFile::Open("hists/hist_gen_sp17.root");
	TFile *inf1_sp18 = TFile::Open("hists/hist_dat_sp18.root");
	TFile *inf2_sp18 = TFile::Open("hists/hist_acc_sp18.root");
	TFile *inf3_sp18 = TFile::Open("hists/hist_gen_sp18.root");
	TFile *inf1_fa18 = TFile::Open("hists/hist_dat_fa18.root");
	TFile *inf2_fa18 = TFile::Open("hists/hist_acc_fa18.root");
	TFile *inf3_fa18 = TFile::Open("hists/hist_gen_fa18.root");

	TH1F *h1_sp17 = (TH1F*)inf1_sp17->Get("im_kskl");
	TH1F *h1_sp17_sb = (TH1F*)inf1_sp17->Get("im_kskl_sb");
	TH1F *h2_sp17 = (TH1F*)inf2_sp17->Get("im_kskl");
	TH1F *h3_sp17 = (TH1F*)inf3_sp17->Get("im_kskl");
	TH1F *h1_sp18 = (TH1F*)inf1_sp18->Get("im_kskl");
	TH1F *h1_sp18_sb = (TH1F*)inf1_sp18->Get("im_kskl_sb");
	TH1F *h2_sp18 = (TH1F*)inf2_sp18->Get("im_kskl");
	TH1F *h3_sp18 = (TH1F*)inf3_sp18->Get("im_kskl");
	TH1F *h1_fa18 = (TH1F*)inf1_fa18->Get("im_kskl");
	TH1F *h1_fa18_sb = (TH1F*)inf1_fa18->Get("im_kskl_sb");
	TH1F *h2_fa18 = (TH1F*)inf2_fa18->Get("im_kskl");
	TH1F *h3_fa18 = (TH1F*)inf3_fa18->Get("im_kskl");

	TH1F *h1 = (TH1F*)h1_sp17->Clone("h1");
	h1->Add(h1_sp18);
	h1->Add(h1_fa18);
	TH1F *h1_sb = (TH1F*)h1_sp17_sb->Clone("h1_sb");
	h1_sb->Add(h1_sp18_sb);
	h1_sb->Add(h1_fa18_sb);
	TH1F *h2 = (TH1F*)h2_sp17->Clone("h2");
	h2->Add(h2_sp18);
	h2->Add(h2_fa18);
	TH1F *h3 = (TH1F*)h3_sp17->Clone("h3");
	h3->Add(h3_sp18);
	h3->Add(h3_fa18);

	h1->Add(h1_sb, -1);
	h2->Divide(h3);

	h1->SetMarkerColor(kBlack);
	h2->SetMarkerColor(kRed);

	h1->SetMarkerStyle(8);
	h1->GetYaxis()->SetRangeUser(0, 1.1*h1->GetMaximum());
	h1->GetXaxis()->SetRangeUser(1.1, 2.6);
	h1->GetXaxis()->SetTitle("M(K_{S}K_{L}) (GeV)");

	h2->SetMarkerStyle(35);
	h2->SetMarkerSize(0.0);
	h2->GetYaxis()->SetTitle("Efficiency");
	h2->GetYaxis()->SetRangeUser(0.0, 0.1);

	TCanvas *c = twoscales(h1, h2);

	c->SaveAs("pdfs/mkskl.pdf");
}

TCanvas* twoscales(TH1F* h1, TH1F* h2) {
	TCanvas *c1 = new TCanvas("c1","different scales hists",600,400);
	gStyle->SetOptStat(kFALSE);

	h1->Draw();
	c1->Update();

	//scale h2 to the pad coordinates
	Float_t rightmax = h2->GetMaximum();
	Float_t scale    = gPad->GetUymax()/rightmax;
	h2->SetLineColor(kRed);
	// h2->SetFillColor(kRed);
	// h2->SetFillStyle(3002);
	// h2->SetMarkerColor(kRed);
	h2->Scale(scale);
	h2->Draw("hist same");
	// h2->Draw("E3 same");
	//draw an axis on the right side
	TGaxis*axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
							gPad->GetUxmax(),gPad->GetUymax(),
							0,rightmax,510,"+L");
	axis->SetLineColor(kRed);
	axis->SetLabelColor(kRed);
	axis->SetTitleColor(kRed);
	axis->SetTitle(h2->GetYaxis()->GetTitle());
	axis->Draw();

	return c1;
}
