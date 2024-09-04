TCanvas* twoscales(TH1F* h1, TH1F* h2);

void compare() {
	gStyle->SetOptStat(0);
	gStyle->SetPadTopMargin(0.07);
	gStyle->SetPadRightMargin(0.03);
	gStyle->SetPadBottomMargin(0.15);
	gStyle->SetPadLeftMargin(0.15);

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
	TFile *inf2 = TFile::Open("hists/hist_acc.root");
	TFile *inf3 = TFile::Open("hists/hist_gen.root");
	TFile *inf4 = TFile::Open("Pm1+_Pm1-_Pp0+_Pp0-_Pp1+_Pp1-.root");

	TH1F *h1 = (TH1F*)inf1->Get("im_kskl");
	TH1F *h1_sb = (TH1F*)inf1->Get("im_kskl_sb");
	TH1F *h2 = (TH1F*)inf2->Get("im_kskl");
	TH1F *h3 = (TH1F*)inf3->Get("im_kskl");
	TGraphErrors *graph = (TGraphErrors*)inf4->Get("all");

	h1->Add(h1_sb, -1);
	h2->Divide(h3);
	h1->Divide(h2);

	graph->GetXaxis()->SetRangeUser(1.1, 2.6);
	graph->GetYaxis()->SetRangeUser(0, 120000);
	graph->SetMarkerStyle(20);
	graph->SetMarkerColor(kBlack);
	graph->GetXaxis()->SetTitle("M(K_{S}K_{L}) (GeV)");
	graph->GetYaxis()->SetTitle("Intensity [a. u.]");
	graph->SetTitle("");

	h1->SetMarkerStyle(24);
	h1->SetMarkerColor(kRed);
	h1->Scale(1.25);

	TCanvas *c = new TCanvas("c","c",800,600);

	graph->Draw("ap");
	h1->Draw("same");

	TLegend *leg = new TLegend(0.56,0.7,0.95,0.9);
	leg->AddEntry(graph, "PWA Total Intensity", "lep");
	leg->AddEntry(h1, "Histogram Method (S = 1.25)", "lep");
	leg->SetTextSize(0.04);
	leg->Draw();

	c->SaveAs("mkskl.pdf");
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
	// h2->SetFillColor(kRed);
	// h2->SetFillStyle(3002);
	// h2->SetMarkerColor(kRed);
	h2->Scale(scale);
	h2->Draw("hist same");
	// h2->Draw("E3 same");
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
