void fig_mkskl_phiHel() {
	gStyle->SetOptStat(0);

	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.07);
	gStyle->SetPadBottomMargin(0.17);
	gStyle->SetPadLeftMargin(0.13);

	gStyle->SetTitleBorderSize(0);
		
	gStyle->SetTitleSize(0.06,"XY");
	gStyle->SetLabelSize(0.06,"XY");
	gStyle->SetLabelOffset(0.02,"XY");  
	gStyle->SetLabelOffset(1,"Z");  
	
	gStyle->SetTitleSize(0.06,"XY");
	gStyle->SetTitleOffset(1.15,"X");
	gStyle->SetTitleOffset(1.,"Y");
	
	gStyle->SetMarkerSize(1.5);
	gROOT->ForceStyle();

	TFile *inf1 = TFile::Open("hist_dat_gluex1.root");
	TFile *inf2 = TFile::Open("hist_acc_gluex1.root");
	TFile *inf3 = TFile::Open("hist_gen_gluex1.root");

	TH2F *h1 = (TH2F*)inf1->Get("h2_mkskl_phiHel");
	TH2F *h1_sb = (TH2F*)inf1->Get("h2_mkskl_phiHel_sb");
	TH2F *h2 = (TH2F*)inf2->Get("h2_mkskl_phiHel");
	TH2F *h3 = (TH2F*)inf3->Get("h2_mkskl_phiHel");

	h2->Divide(h3);
	h1->Divide(h2);
	h1_sb->Divide(h2);

	h1->Add(h1_sb, -1);
	// h2->Add(h2_sb, -1);

	h1->GetYaxis()->SetTitle("#phi_{hel}");
	h1->GetXaxis()->SetTitle("M(K_{S}K_{L}) (GeV)");

	TCanvas *c = new TCanvas();
	h1->Draw("COLZ");

	c->SaveAs("figs/mkskl_phiHel.pdf");
}
