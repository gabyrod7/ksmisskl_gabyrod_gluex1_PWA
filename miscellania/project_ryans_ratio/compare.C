
void compare() {
	gStyle->SetOptStat(0);

	TFile *inf1 = TFile::Open("hist_sp17.root");
	TFile *inf3 = TFile::Open("hist_phi.root");
	TFile *inf4 = TFile::Open("hist_phi1680.root");
	TFile *inf5 = TFile::Open("hist_X1750.root");

	TCanvas *c;
	TLatex t;
	//TH1F *h = new TH1F("h", ";M(K_{S}K_{L});Counts", 102, 0.98, 2.0);

	TH1F *h1 = (TH1F*)inf1->Get("im_kskl");
	TH1F *h2 = (TH1F*)inf1->Get("im_kskl_sb");
	TH1F *h3 = (TH1F*)inf3->Get("im_kskl");
	TH1F *h4 = (TH1F*)inf4->Get("im_kskl");
	TH1F *h5 = (TH1F*)inf5->Get("im_kskl");

	h1->Add(h2, -1);

	double mc_phi1020_integral = h3->Integral();
	double mc_phi1680_integral = h4->Integral();

	h1->SetMarkerStyle(8);
	h3->SetMarkerStyle(24);
	h4->SetMarkerStyle(24);
	h5->SetMarkerStyle(24);

	h1->SetMarkerColor(kBlack);
	h3->SetMarkerColor(kBlue);
	h4->SetMarkerColor(kGreen);
	h5->SetMarkerColor(kViolet);

	double scale = h1->GetMaximum()/h3->GetMaximum();

	h3->Scale(scale);
	h4->Scale(scale);
	h4->Scale(1/28.);
	h5->Scale(scale);
	h5->Scale(1/28.);
//	h5->Scale(0.46*0.03);

//	h->GetYaxis()->SetRangeUser(0, 1.1*h1->GetMaximum());

//	h->Draw();
	c = new TCanvas();
	h1->GetYaxis()->SetRangeUser(0, 1000);
	h1->Draw();
	h3->Draw("SAME");
//	h4->Draw("SAME");
	h5->Draw("SAME");

//	t.DrawLatex(1.1, 0.9*h3->GetMaximum(), "#phi(1020) MC scaled to match data");
//	t.DrawLatex(1.1, 0.8*h3->GetMaximum(), "#phi(1680) MC scaled by 0.1 of #phi(1020)");
//	char s[50];
//	sprintf(s, "#phi(1020) integral = %.2f", mc_phi1020_integral);
//	t.DrawLatex(1.1, 0.6*h3->GetMaximum(), s);
//	sprintf(s, "#phi(1680) integral = %.2f", mc_phi1680_integral);
//	t.DrawLatex(1.1, 0.5*h3->GetMaximum(), s);

	c->SaveAs("compare.pdf");
}
