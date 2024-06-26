void fig_mandeltp() {
	gStyle->SetOptStat(0);
	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.01);
	gStyle->SetPadBottomMargin(0.15);
	gStyle->SetPadLeftMargin(0.17);

	gStyle->SetTitleBorderSize(0);
		
	gStyle->SetTitleSize(0.06,"XY");
	gStyle->SetLabelSize(0.06,"XY");
	gStyle->SetLabelOffset(0.02,"XY");  
	
	gStyle->SetTitleSize(0.06,"XY");
	gStyle->SetTitleOffset(1.15,"X");
	gStyle->SetTitleOffset(1.5,"Y");
	
	gStyle->SetMarkerSize(1.5);
	gROOT->ForceStyle();

	TFile *inf1 = TFile::Open("hist_dat_gluex1.root");
	TFile *inf2 = TFile::Open("hist_acc_gluex1.root");

	TLatex t;
	t.SetTextSize(0.08);

	TH1F *h1 = (TH1F*)inf1->Get("h1_mandeltp");
	TH1F *h1_sb = (TH1F*)inf1->Get("h1_mandeltp_sb");
	TH1F *h2 = (TH1F*)inf2->Get("h1_mandeltp");

	h1->SetMarkerStyle(8);
	h2->SetMarkerStyle(35);

	h1->SetMarkerColor(kBlack);
	h2->SetMarkerColor(kRed);
	h1_sb->SetFillColor(kRed);

	TCanvas *c = new TCanvas();
	h1->Draw();
	h1_sb->Draw("SAME HIST");

	t.DrawLatex(0.5, 0.85*h1->GetMaximum(), "GlueX-I Data");
	t.SetTextColor(kRed);
	t.DrawLatex(0.5, 0.73*h1->GetMaximum(), "M(#pi#pi) Sideband");
	t.SetTextColor(kBlack);

	c->SaveAs("figs/mandeltp_Wbackground.pdf");

	h1->Add(h1_sb, -1);
	TH1F* h3 = (TH1F*)h1->Clone("h3");

	// h2->Scale(h1->GetMaximum()/h2->GetMaximum());
	// scale h2 to match h1 at 0.2
	h2->Scale(h1->GetBinContent(h1->GetXaxis()->FindBin(0.2))/h2->GetBinContent(h2->GetXaxis()->FindBin(0.2)));

	h3->SetFillColorAlpha(kGreen, 0.2);

	h1->GetYaxis()->SetRangeUser(0, 1.1*h1->GetMaximum());
	h3->GetXaxis()->SetRangeUser(0.2, 1.0);

	c = new TCanvas();
	h1->Draw();
	h2->Draw("SAME");
	h3->Draw("SAME HIST");
	h1->Draw("SAME");

	t.DrawLatex(0.6, 0.85*h1->GetMaximum(), "GlueX-I Data");
	t.SetTextColor(kRed);
	t.DrawLatex(0.6, 0.73*h1->GetMaximum(), "Monte Carlo");

	c->SaveAs("figs/mandel_tp.pdf");

	TLine *line = new TLine(0.15, 0, 0.15, h1->GetMaximum());
	line->SetLineWidth(2);
	line->Draw();

	// c = new TCanvas();
	// TRatioPlot *rp = new TRatioPlot(h1, h2);
	// rp->Draw();
	// rp->GetLowerRefYaxis()->SetNdivisions(509);
	// rp->GetLowerRefYaxis()->SetRangeUser(0.9, 1.49);
	// c->SaveAs("fig_mandelt.pdf");
}
