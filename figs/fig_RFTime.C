void fig_RFTime() {
	gStyle->SetOptStat(0);
	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.03);
	gStyle->SetPadBottomMargin(0.17);
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

	TFile *inf1 = TFile::Open("../DSelector/hists/hist_dat_gluex1.root");
	// TFile *inf2 = TFile::Open("../DSelector/hists/hist_acc_gluex1.root");

	TH1F *h1 = (TH1F*)inf1->Get("h1_RFTime");
	TH1F *h1_sb = (TH1F*)inf1->Get("h1_RFTime_sb");
	// TH1F *h2 = (TH1F*)inf2->Get("h1_RFTime");

	h1->Add(h1_sb, -1);

	TH1F *h3 = (TH1F*)h1->Clone();
	TH1F *h4 = (TH1F*)h1->Clone();
	TH1F *h5 = (TH1F*)h1->Clone();


	// h2->Scale(h1->GetMaximum()/h2->GetMaximum());

	h1->SetMarkerColor(kBlack);
	// h2->SetMarkerColor(kRed);
	h3->SetFillColorAlpha(kGreen, 0.2);
	h4->SetFillColorAlpha(kViolet, 0.2);
	h5->SetFillColorAlpha(kViolet, 0.2);

	h1->SetMarkerStyle(8);
	// h2->SetMarkerStyle(35);

	h1->GetYaxis()->SetRangeUser(0, 1.1*h1->GetMaximum());
	h3->GetXaxis()->SetRangeUser(-2, 2);
	h4->GetXaxis()->SetRangeUser(-14, -6);
	h5->GetXaxis()->SetRangeUser(6, 14);

	TCanvas *c = new TCanvas();
	h1->Draw();
	h3->Draw("SAME HIST");
	h4->Draw("SAME HIST");
	h5->Draw("SAME HIST");
	h1->Draw("SAME");
	// h2->Draw("SAME");


	TLatex t;
	t.SetTextSize(0.08);
	t.SetTextColorAlpha(kGreen, 0.5);
	t.DrawLatex(4, 0.85*h1->GetMaximum(), "In-time");
	t.SetTextColorAlpha(kViolet, 0.5);
	t.DrawLatex(4, 0.73*h1->GetMaximum(), "Out-of-time");

	c->SaveAs("figs/rftime.pdf");
}
