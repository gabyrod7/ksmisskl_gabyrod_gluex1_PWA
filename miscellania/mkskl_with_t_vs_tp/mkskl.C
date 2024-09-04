
void mkskl() {
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

	TFile *inf1 = TFile::Open("hists/hist_dat.root");

	TH1F *h1, *h1_sb, *h2, *h2_sb;
	TLatex latex;
	latex.SetTextSize(0.06);

	h1 = (TH1F*)inf1->Get("im_kskl");
	h1_sb = (TH1F*)inf1->Get("im_kskl_sb");
	h2 = (TH1F*)inf1->Get("im_kskl2");
	h2_sb = (TH1F*)inf1->Get("im_kskl_sb2");

	h1->Add(h1_sb, -1);
	h2->Add(h2_sb, -1);

	// h1->SetMarkerSize(0.0);
	// h1->GetYaxis()->SetTitle("Efficiency");
	h1->GetYaxis()->SetRangeUser(0.0, 1.1*h1->GetMaximum());

	h1->SetLineColor(kBlue);
	h2->SetLineColor(kRed);

	h1->SetLineWidth(2);
	h2->SetLineWidth(2);

	TCanvas *c = new TCanvas();

	h1->Draw("hist");
	h2->Draw("hist SAME");

	latex.SetTextColor(kBlue);
	latex.DrawLatex(1.9, 0.8*h1->GetMaximum(), "-t' = (0.2, 0.5) GeV^{2}");
	latex.SetTextColor(kRed);
	latex.DrawLatex(1.9, 0.9*h1->GetMaximum(), "-t = (0.2, 0.5) GeV^{2}");

	c->SaveAs("pdfs/mkskl.pdf");
}
