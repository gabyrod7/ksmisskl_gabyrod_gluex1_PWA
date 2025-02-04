// get the tgraphs in to root files at rootFiles/fit_changeBinning.root and rootFiles/fit_changeRange.root
// make a graph from the root files

void makeGraph() {
	gStyle->SetMarkerStyle(8);
	gStyle->SetMarkerSize(1.);

    TFile *inf1 = TFile::Open("rootFiles/fit_changeBinning.root");
    TFile *inf2 = TFile::Open("rootFiles/fit_changeRange.root");
    TFile *inf3 = TFile::Open("rootFiles/fit_pol1.root");
    TFile *inf4 = TFile::Open("rootFiles/fit_pol3.root");

    TFile *opf = new TFile("rootFiles/graphs_otherVariations.root", "RECREATE");

    TGraphErrors *g, *g1, *g2, *g3, *g4;

    vector<string> hNames = {"m1", "m2", "g1", "g2"};

    TBox *box = new TBox();
    box->SetFillColorAlpha(kGreen, 0.2);

    double pm1 = 1.539, pm1_err = 0.005;
    double pm2 = 1.753, pm2_err = 0.002;
    double pg1 = 0.246, pg1_err = 0.017;
    double pg2 = 0.127, pg2_err = 0.009;

    TCanvas *c;
    for(string hName : hNames) {
        g1 = (TGraphErrors*)inf1->Get(hName.c_str());
        g2 = (TGraphErrors*)inf2->Get(hName.c_str());
        g3 = (TGraphErrors*)inf3->Get(hName.c_str());
        g4 = (TGraphErrors*)inf4->Get(hName.c_str());

        g = new TGraphErrors();
        for(int i = 0; i < g1->GetN(); i++) {
            g->SetPoint(i, i+1, g1->GetY()[i]);
            g->SetPointError(i, 0, g1->GetEY()[i]);
        }
        for(int i = 0; i < g2->GetN(); i++) {
            g->SetPoint(i + g1->GetN(), i + 1 + g1->GetN(), g2->GetY()[i]);
            g->SetPointError(i + g1->GetN(), 0, g2->GetEY()[i]);
        }
        for(int i = 0; i < g3->GetN(); i++) {
            g->SetPoint(i + g1->GetN() + g2->GetN(), i + 1 + g1->GetN() + g2->GetN(), g3->GetY()[i]);
            g->SetPointError(i + g1->GetN() + g2->GetN(), 0, g3->GetEY()[i]);
        }
        for(int i = 0; i < g4->GetN(); i++) {
            g->SetPoint(i + g1->GetN() + g2->GetN() + g3->GetN(), i + 1 + g1->GetN() + g2->GetN() + g3->GetN(), g4->GetY()[i]);
            g->SetPointError(i + g1->GetN() + g2->GetN() + g3->GetN(), 0, g4->GetEY()[i]);
        }

        c = new TCanvas();
        g->GetXaxis()->SetRangeUser(0.75, g1->GetN() + g2->GetN() + g3->GetN() + g4->GetN() + 2);
        g->Draw("AP");

        if(hName == "m1") {
            box->DrawBox(0.75, pm1 - pm1_err, g1->GetN() + g2->GetN() + g3->GetN() + g4->GetN(), pm1 + pm1_err);
            g->SetTitle("; Fit Variation; M_{1} (GeV)");
        }
        else if(hName == "m2") {
            g->SetTitle("; Fit Variation; M_{2} (GeV)");
            box->DrawBox(0.75, pm2 - pm2_err, g1->GetN() + g2->GetN() + g3->GetN() + g4->GetN(), pm2 + pm2_err);
        }
        else if(hName == "g1") {
            g->SetTitle("; Fit Variation; #Gamma_{1} (GeV)");
            box->DrawBox(0.75, pg1 - pg1_err, g1->GetN() + g2->GetN() + g3->GetN() + g4->GetN(), pg1 + pg1_err);
        }
        else if(hName == "g2") {
            g->SetTitle("; Fit Variation; #Gamma_{2} (GeV)");
            box->DrawBox(0.75, pg2 - pg2_err, g1->GetN() + g2->GetN() + g3->GetN() + g4->GetN(), pg2 + pg2_err);
        }
        
        opf->cd();
        g->Write(hName.c_str());
        c->SaveAs((hName+".pdf").c_str());
    }


}