void draw(TGraphErrors *g1, TGraphErrors *g2);

void plot_tBins() {
	gStyle->SetOptStat(0);
	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.03);
	gStyle->SetPadBottomMargin(0.13);
	gStyle->SetPadLeftMargin(0.16);

	gStyle->SetTitleBorderSize(0);

	gStyle->SetTitleFont(132);
	gStyle->SetTitleSize(0.05);
	gStyle->SetTitleOffset(1.2);

	gStyle->SetLabelSize(0.05,"XY");
	gStyle->SetLabelOffset(0.01,"XY");

	gStyle->SetTitleSize(0.05,"XY");
	gStyle->SetTitleOffset(1.1,"X");

	// gStyle->SetMarkerStyle(8);
	gStyle->SetMarkerSize(1.);

    gROOT->ForceStyle();

    double par_nInterference, par_wInterference;
    int j;
    TCanvas *c;
    TLegend *leg;
    TLine *line_nInterference = new TLine(), 
        *line_wInterference = new TLine();
    line_nInterference->SetLineColor(kBlack);
    line_wInterference->SetLineColor(kRed);
    line_wInterference->SetLineStyle(2);
    line_nInterference->SetLineWidth(2);
    line_wInterference->SetLineWidth(2);

    TFile *inf1 = TFile::Open("rootFiles/tBins_nInterference.root");
    TFile *inf2 = TFile::Open("rootFiles/tBins_wInterference.root");

    TGraphErrors *g_nInterference[4], *g_wInterference[5];

    vector<string> graphNames = {"m1", "g1", "m2", "g2"};
    for(int i = 0; i < graphNames.size(); i++) {
        string name = graphNames[i];
        g_nInterference[i] = (TGraphErrors*)inf1->Get(name.c_str());
        g_wInterference[i] = (TGraphErrors*)inf2->Get(name.c_str());

        g_nInterference[i]->SetMarkerStyle(8);
        g_wInterference[i]->SetMarkerStyle(25);

        g_wInterference[i]->SetMarkerColor(kRed);
        g_wInterference[i]->SetLineColor(kRed);

        g_nInterference[i]->GetXaxis()->SetRangeUser(0.2, 1.0);
        g_wInterference[i]->GetXaxis()->SetRangeUser(0.2, 1.0);
    }
    g_wInterference[4] = (TGraphErrors*)inf2->Get("phase");
    g_wInterference[4]->SetMarkerStyle(25);
    g_wInterference[4]->SetLineColor(kRed);
    g_wInterference[4]->SetMarkerColor(kRed);
    g_wInterference[4]->GetXaxis()->SetRangeUser(0.2, 1.0);

    leg = new TLegend(0.6, 0.2, 0.95, 0.55);
    leg->SetFillStyle(0);
    // leg->SetBorderSize(0);
    leg->AddEntry(g_nInterference[0], "No Interference", "lep");
    leg->AddEntry(g_wInterference[1], "With Interference", "lep");
    leg->AddEntry(line_nInterference, "Nominal", "l");
    // leg->AddEntry(line_wInterference, "Nominal (WI)", "l");

    c = new TCanvas();
    j = 0;
    par_nInterference = 1.538;
    par_wInterference = 1.538;
    draw(g_nInterference[j], g_wInterference[j]);
    line_nInterference->DrawLine(0.2, par_nInterference, 1.0, par_nInterference);
    // line_wInterference->DrawLine(0.2, par_wInterference, 1.0, par_wInterference);
    leg->Draw();
    c->SaveAs("tBinsParameters/m1.pdf");

    j = 1;
    par_nInterference = 0.257;
    par_wInterference = 0.257;
    draw(g_nInterference[j], g_wInterference[j]);
    line_nInterference->DrawLine(0.2, par_nInterference, 1.0, par_nInterference);
    // line_wInterference->DrawLine(0.2, par_wInterference, 1.0, par_wInterference);
    c->SaveAs("tBinsParameters/g1.pdf");

    j = 2;
    par_nInterference = 1.753;
    par_wInterference = 1.753;
    draw(g_nInterference[j], g_wInterference[j]);
    line_nInterference->DrawLine(0.2, par_nInterference, 1.0, par_nInterference);
    // line_wInterference->DrawLine(0.2, par_wInterference, 1.0, par_wInterference);
    c->SaveAs("tBinsParameters/m2.pdf");

    j = 3;
    par_nInterference = 0.125;
    par_wInterference = 0.125;
    draw(g_nInterference[j], g_wInterference[j]);
    line_nInterference->DrawLine(0.2, par_nInterference, 1.0, par_nInterference);
    // line_wInterference->DrawLine(0.2, par_wInterference, 1.0, par_wInterference);
    c->SaveAs("tBinsParameters/g2.pdf");

    j = 4;
    par_wInterference = 0.84;
    g_wInterference[j]->Draw("AP");
    line_wInterference->SetLineColor(kBlack);
    line_wInterference->SetLineStyle(1);
    line_wInterference->DrawLine(0.2, par_wInterference, 1.0, par_wInterference);
    c->SaveAs("tBinsParameters/phase.pdf");
}

void draw(TGraphErrors *g1, TGraphErrors *g2) {
    g1->Draw("AP");
    g2->Draw("P");
}