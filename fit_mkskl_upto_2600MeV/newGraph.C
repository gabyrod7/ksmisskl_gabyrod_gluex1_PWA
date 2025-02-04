
void newGraph() {
	gStyle->SetMarkerStyle(8);
	gStyle->SetMarkerSize(1.);

    TFile *inf1 = TFile::Open("rootFiles/graphs_EventSelelctionVariations.root");
    TFile *inf2 = TFile::Open("rootFiles/graphs_otherVariations.root");

    TGraphErrors *g, *g1, *g2;

    vector<string> hNames = {"m1", "m2", "g1", "g2"};

    TBox *box = new TBox();
    box->SetFillColorAlpha(kGreen, 0.2);

    TCanvas *c = new TCanvas();
    for(string hName : hNames) {
        g = new TGraphErrors();

        g1 = (TGraphErrors*)inf1->Get(hName.c_str());
        g2 = (TGraphErrors*)inf2->Get(hName.c_str());

        for(int i = 1; i < g1->GetN(); i++) {
            g->SetPoint(i, i+1, g1->GetY()[i]);
            g->SetPointError(i, 0, g1->GetEY()[i]);
        }
        for(int i = 0; i < g2->GetN(); i++) {
            g->SetPoint(i + g1->GetN(), i + 1 + g1->GetN(), g2->GetY()[i]);
            g->SetPointError(i + g1->GetN(), 0, g2->GetEY()[i]);
        }

        g->GetXaxis()->SetRangeUser(0.75, g1->GetN() + g2->GetN() + 2);
        g->GetYaxis()->SetRangeUser(g1->GetY()[0] - 5*g1->GetEY()[0], g1->GetY()[0] + 5*g1->GetEY()[0]);
        g->SetTitle(g2->GetTitle());
        g->Draw("AP");
        box->DrawBox(0.75, g1->GetY()[0] - g1->GetEY()[0], g1->GetN() + g2->GetN(), g1->GetY()[0] + g1->GetEY()[0]);
        c->SaveAs((hName + ".pdf").c_str());
    }
}