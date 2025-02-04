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

	TFile *inf1 = TFile::Open("hists/hist_dat_gluex1.root");
	TFile *inf2 = TFile::Open("hists/hist_acc_gluex1.root");
	TFile *inf3 = TFile::Open("hists/hist_gen_gluex1.root");

	TH1F *h1 = (TH1F*)inf1->Get("im_kskl");
	TH1F *h1_sb = (TH1F*)inf1->Get("im_kskl_sb");
	TH1F *h2 = (TH1F*)inf2->Get("im_kskl");
	TH1F *h3 = (TH1F*)inf3->Get("im_kskl");

	h1->Add(h1_sb, -1);
	h2->Divide(h3);

	h1->SetMarkerColor(kBlack);
	h2->SetMarkerColor(kRed);

	h1->SetMarkerStyle(8);
	h2->SetMarkerStyle(35);
	h1->GetYaxis()->SetRangeUser(0, 1.1*h1->GetMaximum());
	h1->GetXaxis()->SetRangeUser(1.1, 2.6);
	h1->GetXaxis()->SetTitle("M(K_{S}K_{L}) (GeV)");

	h2->SetMarkerSize(0.0);
	h2->GetYaxis()->SetTitle("Efficiency");
	h2->GetYaxis()->SetRangeUser(0, 0.1);


	TCanvas *c = twoscales(h1, h2);

	// TCanvas *c = new TCanvas();
	// h1->Draw();

	c->SaveAs("figs/mkskl.pdf");
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
   h2->Scale(scale);
   h2->Draw("hist same");
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
