
void efficiency() {
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

	TFile *inf1 = TFile::Open("hists/hist_acc.root");
	TFile *inf2 = TFile::Open("hists/hist_gen.root");

	TH1F *h1, *h2, *h3, *h4;
	TLatex latex;
	latex.SetTextSize(0.06);

	h1 = (TH1F*)inf1->Get("im_kskl");
	h2 = (TH1F*)inf2->Get("im_kskl");
	h3 = (TH1F*)inf1->Get("im_kskl2");
	h4 = (TH1F*)inf2->Get("im_kskl2");

	h1->Divide(h2);
	h3->Divide(h4);

	h1->SetMarkerSize(0.0);
	h1->GetYaxis()->SetTitle("Efficiency");
	h1->GetYaxis()->SetRangeUser(0.0, 0.1);

	h1->SetLineColor(kBlue);
	h3->SetLineColor(kRed);

	h1->SetLineWidth(2);
	h3->SetLineWidth(2);

	TCanvas *c = new TCanvas();

	h1->Draw("hist");
	h3->Draw("hist SAME");

	latex.SetTextColor(kBlue);
	latex.DrawLatex(1.2, 0.05, "-t' = (0.2, 0.5) GeV^{2}");
	latex.SetTextColor(kRed);
	latex.DrawLatex(1.2, 0.04, "-t = (0.2, 0.5) GeV^{2}");

	c->SaveAs("pdfs/efficiency.pdf");
}
