
void h2_mkskl_momentumTransfer() {
	gStyle->SetOptStat(0);
	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.11);
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

	TFile *inf1 = TFile::Open("hists/hist_dat.root");

	TH1F *h1, *h1_sb, *h2, *h2_sb;

	h1 = (TH1F*)inf1->Get("mkskl_mandeltp");
	h1_sb = (TH1F*)inf1->Get("mkskl_mandeltp_sb");
	h2 = (TH1F*)inf1->Get("mkskl_mandelt");
	h2_sb = (TH1F*)inf1->Get("mkskl_mandelt_sb");

	h1->Add(h1_sb, -1);
	h2->Add(h2_sb, -1);

	TCanvas *c = new TCanvas();

	h1->Draw("colz");
	c->SaveAs("pdfs/mkskl_mandeltp.pdf");

	h2->Draw("colz");
	c->SaveAs("pdfs/mkskl_mandelt.pdf");
}
