void mkskl() {
    gStyle->SetOptStat(0);

    TFile *inf1 = TFile::Open("hist_dat_kskl.root");
    TFile *inf2 = TFile::Open("hist_dat_phi.root");

    TFile *opf = new TFile("hist.root", "RECREATE");

    TH1F *h1 = (TH1F*)inf1->Get("im_kskl");
    TH1F *h1_sb = (TH1F*)inf1->Get("im_kskl_sb");

    TH1F *h2 = (TH1F*)inf2->Get("im_kskl");
    TH1F *h2_sb = (TH1F*)inf2->Get("im_kskl_sb");

    TH1F *h3 = (TH1F*)h1->Clone();
    TH1F *h3_sb = (TH1F*)h1_sb->Clone();

    h1->SetMarkerStyle(8);
    h2->SetMarkerStyle(8);
    h3->SetMarkerStyle(8);

    h3->Add(h2);
    h3_sb->Add(h2_sb);
    h3_sb->SetFillColor(kRed);

    char text[50];
    sprintf(text, "Counts / %.0f MeV", h1->GetBinWidth(1)*1000);
    h1->GetYaxis()->SetTitle(text);
    h2->GetYaxis()->SetTitle(text);
    h3->GetYaxis()->SetTitle(text);

    h3->Write("im_kskl");
    h3_sb->Write("im_kskl_sb");

    // h3->Draw();
    // h3_sb->Draw("same hist");

    TCanvas *c = new TCanvas("c", "c", 800, 500);

    TPad *pad1 = new TPad("pad1","",0,0,1,1);
    pad1->SetTopMargin(0.05);
    pad1->SetRightMargin(0.03);
    pad1->SetBottomMargin(0.1);
    pad1->SetLeftMargin(0.1);

    TPad *pad2 = new TPad("pad2","",0.2,0.15,0.95,0.9);
    pad2->SetTopMargin(0.02);
    pad2->SetRightMargin(0.02);
    pad2->SetBottomMargin(0.1);
    pad2->SetLeftMargin(0.1);

    pad1->Draw();
    pad1->cd();
    h3->Draw();
    h3_sb->Draw("SAME HIST");

    pad2->Draw();
    pad2->cd();
    h1->GetXaxis()->SetRangeUser(1.1, 2.6);
    h1->GetYaxis()->SetRangeUser(0, 1.1*h1->GetMaximum());
    h1->Draw();
    h3_sb->Draw("SAME HIST");

	TLatex t;
	t.SetTextSize(0.08);
    pad2->cd();
	t.DrawLatex(2.0, 0.85*h1->GetMaximum(), "GlueX-I Data");
	t.SetTextColor(kRed);
	t.DrawLatex(2.0, 0.73*h1->GetMaximum(), "M(#pi#pi) Sideband");
	t.SetTextColor(kBlack);

    TLine *line = new TLine();
    line->SetLineStyle(2);
    line->SetLineWidth(2);
    line->DrawLineNDC(0, 0, 0, 1);
    line->DrawLineNDC(0, 0, 1, 0);

    c->SaveAs("mkskl_looseCuts.pdf");

    // TLine *line1 = new TLine(0, 0, 1, 1000000);
    // line1->Draw();
}

void format_histogram(TH1F* h) {
    char name[50];
    sprintf(name, "Counts / %.0f MeV", h->GetBinWidth(1)*1000);
	h->GetYaxis()->SetTitle(name);

    h->GetYaxis()->SetRangeUser(0, 1.1*h->GetMaximum());
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetTitleOffset(1.35);
	h->GetYaxis()->SetLabelSize(0.06);
	h->GetYaxis()->SetLabelOffset(0.015);

    h->GetXaxis()->SetTitleSize(0.06);
    h->GetXaxis()->SetLabelSize(0.06);
    h->GetXaxis()->SetTitleOffset(1.3);
	h->GetXaxis()->SetLabelOffset(0.015);

    h->SetMarkerStyle(8);
    h->SetMarkerSize(1.);
}

void mkskl2() {
    gStyle->SetOptStat(0);
    TFile *inf = TFile::Open("hist.root");
    TH1F *h1 = (TH1F*)inf->Get("im_kskl");
    TH1F *h3 = (TH1F*)inf->Get("im_kskl_sb");
    // TH1F *h3 = (TH1F*)inf->Get("im_kskl_kinf_sb");
    // h1->Add(h3, -1);
    TH1F *h2 = (TH1F*)h1->Clone();
    TLine *line1, *lin2;
    TLatex t;

    h1->GetXaxis()->SetRangeUser(0.98, 2.1);
    // h2->Rebin(2);
    h2->GetXaxis()->SetRangeUser(1.1, 2.1);

    format_histogram(h1);
    format_histogram(h2);
    h3->SetFillColor(kRed);

    TCanvas *c = new TCanvas("c", "c", 800, 500);
    TPad *pad1 = new TPad("pad1","",0,0,1,1);
    pad1->SetTopMargin(0.03);
    pad1->SetRightMargin(0.03);
    pad1->SetBottomMargin(0.17);
    pad1->SetLeftMargin(0.17);
    TPad *pad2 = new TPad("pad2","",0.27,0.3,0.95,0.95);
    pad2->SetFillColorAlpha(kGray, 0.2);
    pad2->SetTopMargin(0.02);
    pad2->SetRightMargin(0.02);
    pad2->SetBottomMargin(0.17);
    pad2->SetLeftMargin(0.17);

    pad1->Draw();
    pad1->cd();
    h1->Draw();
    h3->Draw("SAME HIST");
    c->SaveAs("figs/mkskl_phi.pdf");

    pad2->Draw();
    pad2->cd();
    h2->Draw();
    h3->Draw("SAME HIST");

    t.SetTextSize(0.08);
    t.SetTextColor(kBlack);
    t.DrawLatex(1.8, 0.88*h2->GetMaximum(), "Data");
    t.SetTextColor(kRed);
    t.DrawLatex(1.8, 0.75*h2->GetMaximum(), "Sideband");

    c->SaveAs("figs/mkskl.pdf");
}
