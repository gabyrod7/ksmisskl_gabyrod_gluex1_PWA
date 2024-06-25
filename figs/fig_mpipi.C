void fig_mpipi() {
	gStyle->SetOptStat(0);
	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.04);
	gStyle->SetPadBottomMargin(0.15);
	gStyle->SetPadLeftMargin(0.17);

	gStyle->SetTitleBorderSize(0);
		
	gStyle->SetTitleSize(0.06,"XY");
	gStyle->SetLabelSize(0.06,"XY");
	gStyle->SetLabelOffset(0.02,"XY");  
	
	gStyle->SetTitleSize(0.06,"XY");
	gStyle->SetTitleOffset(1.15,"X");
	gStyle->SetTitleOffset(1.5,"Y");
	
	gStyle->SetMarkerSize(1.0);
	gROOT->ForceStyle();

	TFile *inf1 = TFile::Open("hist_dat_gluex1.root");
	TFile *inf2 = TFile::Open("hist_acc_gluex1.root");

	TH1F *h1 = (TH1F*)inf1->Get("h1_Mpipi");
	TH1F *h2 = (TH1F*)inf2->Get("h1_Mpipi");
	TH1F *h3 = (TH1F*)h1->Clone();
	TH1F *h4 = (TH1F*)h1->Clone();
	TH1F *h5 = (TH1F*)h1->Clone();

	h2->Scale(h1->GetMaximum()/h2->GetMaximum());
	h3->GetXaxis()->SetRangeUser(0.48, 0.52);
	h4->GetXaxis()->SetRangeUser(0.44, 0.46);
	h5->GetXaxis()->SetRangeUser(0.54, 0.56);

	h1->SetMarkerColor(kBlack);
	h2->SetMarkerColor(kRed);
	h3->SetFillColorAlpha(kGreen, 0.2);
	h4->SetFillColorAlpha(kViolet, 0.2);
	h5->SetFillColorAlpha(kViolet, 0.2);

	h1->SetMarkerStyle(8);
	h2->SetMarkerStyle(35);

	h1->GetYaxis()->SetRangeUser(0, 1.1*h1->GetMaximum());

	TCanvas *c = new TCanvas();
	h1->Draw();
	h3->Draw("SAME HIST");
	// h4->Draw("SAME HIST");
	// h5->Draw("SAME HIST");
	h1->Draw("SAME");
	h2->Draw("SAME");

	TLatex t;
	t.SetTextSize(0.08);
	t.DrawLatex(0.54, 0.85*h1->GetMaximum(), "GlueX-I Data");
	t.SetTextColor(kRed);
	t.DrawLatex(0.54, 0.73*h1->GetMaximum(), "Monte Carlo");

	TLine *line = new TLine(0.3, 0, 0.7, 0.5*h1->GetMaximum());
	line->SetLineWidth(2);
	line->DrawLine(0.48, 0, 0.48, 0.5*h1->GetMaximum());
	line->DrawLine(0.52, 0, 0.52, 0.5*h1->GetMaximum());

	TLine *line2 = new TLine(0.3, 0, 0.7, 0.5*h1->GetMaximum());
	line2->SetLineWidth(2);
	line2->SetLineStyle(2);
	line2->DrawLine(0.44, 0, 0.44, 0.5*h1->GetMaximum());
	line2->DrawLine(0.46, 0, 0.46, 0.5*h1->GetMaximum());
	line2->DrawLine(0.54, 0, 0.54, 0.5*h1->GetMaximum());
	line2->DrawLine(0.56, 0, 0.56, 0.5*h1->GetMaximum());

	c->SaveAs("figs/mpipi.pdf");
}
