void fig_protonZ() {
	gStyle->SetOptStat(0);
	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.03);
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

	TH1F *h1 = (TH1F*)inf1->Get("h1_ProtonZVertex");
	TH1F *h1_sb = (TH1F*)inf1->Get("h1_ProtonZVertex_sb");
	TH1F *h2 = (TH1F*)inf2->Get("h1_ProtonZVertex");

	h1->Add(h1_sb, -1);
	TH1F *h3 = (TH1F*)h1->Clone();

	h2->Scale(h1->GetMaximum()/h2->GetMaximum());
	h3->GetXaxis()->SetRangeUser(52, 78);

	h1->SetMarkerColor(kBlack);
	h2->SetMarkerColor(kRed);
	h3->SetFillColorAlpha(kGreen, 0.2);

	h1->SetMarkerStyle(8);
	h2->SetMarkerStyle(35);

	h1->GetYaxis()->SetRangeUser(0, 1.25*h1->GetMaximum());

	TCanvas *c = new TCanvas();
	h1->Draw();
	h3->Draw("SAME HIST");
	h1->Draw("SAME");
	h2->Draw("SAME");

	TLatex t;
	t.SetTextSize(0.08);
	t.DrawLatex(35, 0.92*h1->GetMaximum(), "GlueX-I Data");
	t.SetTextColor(kRed);
	t.DrawLatex(35, 0.80*h1->GetMaximum(), "Monte Carlo");

	// TLine *line = new TLine(0.3, 0, 0.7, 0.5*h1->GetMaximum());
	// line->SetLineWidth(2);
	// line->DrawLine(52, 0, 52, h1->GetMaximum());
	// line->DrawLine(78, 0, 78, h1->GetMaximum());

	c->SaveAs("figs/protonZ.pdf");
}
