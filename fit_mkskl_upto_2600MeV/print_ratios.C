void print_ratios() {
    vector<string> runs = {"sp17", "sp18", "fa18"};
    // vector<string> dtype = {"dat"};
    string baseDir = "hists/";

    TFile *inf;
    TH1F *h1, *h2;

    double integral = 0;

    for(string run : runs) {
        inf = TFile::Open((baseDir + "hist_dat_" + run + ".root").c_str(), "READ");
        h1 = (TH1F*)inf->Get("im_kskl");
        h2 = (TH1F*)inf->Get("im_kskl_sb");
        cout << run << " " << h1->GetEntries() - h2->GetEntries() << endl;
        integral += h1->GetEntries() - h2->GetEntries();

        inf->Close();
        // delete h;
    }

    cout << "Total: " << integral << endl;
}
