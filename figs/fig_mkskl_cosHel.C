void setColorAxisRange(TH2F* hist);

void fig_mkskl_cosHel() {
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

	TH2F *h1 = (TH2F*)inf1->Get("h2_mkskl_cosHel");
	TH2F *h1_sb = (TH2F*)inf1->Get("h2_mkskl_cosHel_sb");
	TH2F *h2 = (TH2F*)inf2->Get("h2_mkskl_cosHel");
	TH2F *h3 = (TH2F*)inf3->Get("h2_mkskl_cosHel");

	h2->Divide(h3);
	h1->Divide(h2);
	h1_sb->Divide(h2);

	h1->Add(h1_sb, -1);
	h1->GetXaxis()->SetTitle("M(K_{S}K_{L}) (GeV)");

	TCanvas *c = new TCanvas();
	h1->Draw("COLZ");

	c->SaveAs("figs/mkskl_cosHel.pdf");
}

void setColorAxisRange(TH2F* hist) {
    double max = hist->GetMaximum();
    gStyle->SetNumberContours(max);
    const Int_t NRGBs = 5;
    const Int_t NCont = max;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
}
