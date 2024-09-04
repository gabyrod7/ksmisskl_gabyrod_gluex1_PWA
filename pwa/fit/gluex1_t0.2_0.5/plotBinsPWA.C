void bin(int nBins, vector<string> waves, vector<string> var);

void plotBinsPWA() {
    int nBins = 36;
	vector<string> waves = {"Pm1+", "Pm1-", "Pp0+", "Pp0-", "Pp1+", "Pp1-"};
    vector<string> var = {"cosTheta", "phi", "Phi", "psi"};

    bin(nBins, waves, var);
}

void bin(int nBins, vector<string> waves, vector<string> var) {
    TFile *inf[var.size()];
    TH1F *h1[var.size()], *h2[var.size()];

    TH1F *h = new TH1F("h", "h", nBins, 1.1, 2.0);

    char text[100];

    string fname = "";
    string wave_set = "";
	for(auto wave : waves)	wave_set += wave+"_";
	wave_set.pop_back();

    TCanvas *c = new TCanvas("c", "c", 300, 150);
    c->Divide(2,2);
    for(int i = 0; i < nBins; i++) {
        string bin = to_string(i);

        for(int j = 0; j < var.size(); j++) {
            fname = "root_files/"+var[j]+"_"+wave_set+".root";
            inf[j] = TFile::Open(fname.c_str());
            if (!inf[j]) {
                cout << fname+" not found" << endl;
                return;
            }

            h1[j] = (TH1F*)inf[j]->Get(("data_"+bin).c_str());
            h2[j] = (TH1F*)inf[j]->Get(("acc_"+bin).c_str());
            if (!h1[j] || !h2[j]) {
                cout << "Histogram not found" << endl;
                return;
            }

            c->cd(j+1);
            h2[j]->Draw("hist");
            h1[j]->Draw("same e1");

            if(j == 0) {
                sprintf(text, "M(K_{S}K_{L}) = %.2f - %.2f", h->GetBinLowEdge(i+1), h->GetBinLowEdge(i+2));

                TLatex t;
                t.SetTextSize(0.08);
                t.DrawLatex(-0.1, 0.85*h2[j]->GetMaximum(), text);
            }
        }

        c->SaveAs(("tmp/"+wave_set+"_bin"+bin+".pdf").c_str());
    }
}