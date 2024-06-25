// addhock change to get the percentage of F waves
void plot(vector<string> waves, string fit_dir, int nBins, double &number);
void tmp(int nBins);
double calculateRiemannSum(TGraphErrors* graph);

void plotAmps() {
    vector<double> numbers;
    vector<string> waves;
    string fit_dir = "/d/grid15/gabyrod7/analysis/ksmisskl_gabyrod_gluex1_PWA/pwa/fits/gluex1/fit_sp17_36bins/";
    int nBins = 36;
    double number = 0;

    waves = {"Pm1+", "Pm1-", "Pp0+", "Pp0-", "Pp1+", "Pp1-"};
    plot(waves, fit_dir, nBins, number);

    waves = {"Sp0+", "Sp0-", "Pm1+", "Pm1-", "Pp0+", "Pp0-", "Pp1+", "Pp1-"};
    plot(waves, fit_dir, nBins, number);

    // waves = {"Pm1+", "Pm1-", "Pp0+", "Pp0-", "Pp1+", "Pp1-", "Fm3+"};
    // plot(waves, fit_dir, nBins);

	vector<string> f_waves = {"Fm3+", "Fm3-", "Fm2+", "Fm2-", "Fm1+", "Fm1-", "Fp0+", "Fp0-", "Fp1+", "Fp1-", "Fp2+", "Fp2-", "Fp3+", "Fp3-"};

	for(string f_wave : f_waves) {
		waves = {"Pm1+", "Pm1-", "Pp0+", "Pp0-", "Pp1+", "Pp1-"};
		waves.push_back(f_wave);

        plot(waves, fit_dir, nBins, number);

        numbers.push_back(number);
	}

    string line1 = "";
    string line2 = "";
    char s[100];
    for(int i = 0; i < numbers.size(); i++) {
        line1 += f_waves[i]+" & ";
        sprintf(s, "%.0f & ", numbers[i]*100);
        line2 += s;
    }

    cout << line1 << endl;
    cout << line2 << endl;

    // tmp(nBins);
}

void plot(vector<string> waves, string fit_dir, int nBins, double &number) {
    string waveset;
    string file;
    string line;
    string header;
    vector<string> columns;
    vector<string> phases;
    map<string, TGraphErrors*> graphs;
    map<string, double> amps;
    TFile *opf;
    char s[100];

    // cheap way to get the right mass bin centers
    TH1F *h = new TH1F("h", "h", nBins, 1.1, 2.0);

	TLatex t;
	t.SetTextSize(0.1);

	map<string, string> wave_to_tlatex = { 
        {"Sp0+", "S^{+}_{0}"}, {"Sp0-", "S^{-}_{0}"},
        {"Pm1+", "P^{+}_{-1}"}, {"Pm1-", "P_{-1}^{-}"}, 
        {"Pp0+", "P_{0}^{+}"}, {"Pp0-", "P_{0}^{-}"}, 
        {"Pp1+", "P_{1}^{+}"}, {"Pp1-", "P_{1}^{-}"},
        {"P+", "P^{+}"}, {"P-", "P^{-}"},
        {"Fm1+", "F^{+}_{-1}"}, {"Fm1-", "F_{-1}^{-}"}, 
        {"Fp0+", "F_{0}^{+}"}, {"Fp0-", "F_{0}^{-}"}, 
        {"Fp1+", "F_{1}^{+}"}, {"Fp1-", "F_{1}^{-}"}, 
        {"Fm3+", "F^{+}_{-3}"}, {"Fm3-", "F_{-3}^{-}"}, 
        {"Fm2+", "F_{-2}^{+}"},{"Fm2-", "F_{-2}^{-}"},  
        {"Fm0-", "F_{-2}^{-}"}, {"Fp2+", "F_{2}^{+}"}, 
        {"Fp2-", "F_{2}^{-}"}, {"Fp3+", "F^{+}_{3}"}, {"Fp3-", "F_{3}^{-}"} };

	for(auto wave : waves) {
        waveset += wave+"_";
    }
    waveset.pop_back();

    waves.push_back("P+");
    waves.push_back("P-");
    waves.push_back("all");
    for(string wave : waves) {
        graphs[wave] = new TGraphErrors(nBins);
    }

    opf = TFile::Open(("amps/"+waveset+".root").c_str(), "RECREATE");

    for(int i = 0; i < nBins; i++) {
        file = fit_dir+"bin_"+to_string(i)+"/"+waveset+"/amplitudes.csv";
        ifstream fin(file.c_str());
        if(!fin.is_open()) {
            cout << "Error: file " << file << " not found." << endl;
            return;
        }

        // read header
        if(getline(fin, header)) {
            istringstream iss(header);
            string value;
            while (getline(iss, value, ',')) { 
                // cout << value << endl;
                columns.push_back(value);

                // get phases from the fit
                if(i == 0 && value.find("phase") != std::string::npos && !(value.find("err") != std::string::npos)) {
                    phases.push_back(value);
                    graphs[value] = new TGraphErrors(nBins); 
                }
            }
        }

        // read values
        if(getline(fin, line)) {
            istringstream iss(line);
            string value;
            int index = 0;
            while (getline(iss, value, ',')) {
                // cout << value << endl;
                amps[columns[index++]] = stod(value);
            }
        }

        // // print amps 
        // for(auto const& [key, val] : amps) {
        //     cout << key << " : " << val << endl;
        // }

        for(string wave : waves) {
            graphs[wave]->SetPoint(i, h->GetBinCenter(i+1), amps[wave]);
            graphs[wave]->SetPointError(i, h->GetBinWidth(i+1)/2., amps[wave+"_err"]);
        }

        for(string phase : phases) {
            graphs[phase]->SetPoint(i, h->GetBinCenter(i+1), amps[phase]);
            graphs[phase]->SetPointError(i, h->GetBinWidth(i+1)/2., amps[phase+"_err"]);
        }

        fin.close();
    }

    for(string wave : waves) {
        cout << "Write "+wave+" graph to file" << endl;
        graphs[wave]->Write(wave.c_str());
    }
    for(string phase : phases) {
        cout << "Write "+phase+" graph to file" << endl;
        graphs[phase]->Write(phase.c_str());
    }

    TCanvas *c = new TCanvas("c", "c", 800, 600);
    if((waves.size()-2) % 2)    c->Divide(2, (waves.size()-2)/2+1);
    else                      c->Divide(2, (waves.size()-2)/2);
    
    double max = graphs["all"]->GetHistogram()->GetMaximum();
    graphs["all"]->GetYaxis()->SetRangeUser(0, max);
    graphs["all"]->SetTitle("");
    graphs["all"]->SetMarkerStyle(20);
    for(int i = 1; i < waves.size(); i++) {
        if(waves[i-1] == "P+" || waves[i-1] == "P-") continue;

        c->cd(i);
        graphs["all"]->Draw("AP");
        graphs[waves[i-1]]->Draw("P");

        // cout << graphs[waves[i-1]]->Integral()/graphs["all"]->Integral() << endl;
        cout << waves[i-1] << " " << calculateRiemannSum(graphs[waves[i-1]])/calculateRiemannSum(graphs["all"]) << endl;
        number = calculateRiemannSum(graphs[waves[i-1]])/calculateRiemannSum(graphs["all"]);
        // cout << "   " << graphs[waves[i-1]]->Integral() << endl;
        // cout << "   " << graphs["all"]->Integral() << endl;

        sprintf(s, "%s [%.0f\%]", wave_to_tlatex[waves[i-1]].c_str(), calculateRiemannSum(graphs[waves[i-1]])/calculateRiemannSum(graphs["all"])*100 );
        t.DrawLatex(1.9, 0.8*max, s);
		// t.DrawLatex(1.9, 0.8*max, wave_to_tlatex[waves[i-1]].c_str());
    }
    c->SaveAs(("amps/"+waveset+".pdf").c_str());

    for(int i = 0; i < phases.size(); i++) {
        c->cd(i+1);
        graphs[phases[i]]->Draw("AP");

        t.DrawLatex(1.6, 5, phases[i].c_str());
    }
    c->SaveAs(("amps/"+waveset+"_phases.pdf").c_str());

    c = new TCanvas("c", "c", 800, 600);
    c->Divide(1,2);
    
    c->cd(1);
    graphs["all"]->Draw("AP");
    graphs["P+"]->Draw("P");
    t.DrawLatex(1.9, 0.8*max, wave_to_tlatex["P+"].c_str());

    c->cd(2);
    graphs["all"]->Draw("AP");
    graphs["P-"]->Draw("P");
    t.DrawLatex(1.9, 0.8*max, wave_to_tlatex["P-"].c_str());

    c->SaveAs(("amps/"+waveset+"_P.pdf").c_str());

    opf->Close();
}

void tmp(int nBins) {
    TGraphErrors *g = new TGraphErrors(nBins);
    TGraphErrors *g_all = new TGraphErrors(nBins);
    TFile *inf;

    vector<string> waves = {"Pm1+", "Pm1-", "Pp0+", "Pp0-", "Pp1+", "Pp1-"};
	vector<string> f_waves = {"Fm3+", "Fm3-", "Fm2+", "Fm2-", "Fm1+", "Fm1-", "Fp0+", "Fp0-", "Fp1+", "Fp1-", "Fp2+", "Fp2-", "Fp3+", "Fp3-"};

    string waveset;
	for(auto wave : waves) {
        waveset += wave+"_";
    }
    waveset.pop_back();

    inf = TFile::Open(("tmp/"+waveset+".root").c_str());

    TCanvas *c = new TCanvas("c", "c", 800, 600);
    g = (TGraphErrors*)inf->Get("Pp1-");
    g_all = (TGraphErrors*)inf->Get("all");
    g->GetYaxis()->SetRangeUser(0, g->GetHistogram()->GetMaximum());
    g->SetLineColor(kRed);
    g->Draw("AP");
    cout << g->Integral()/g_all->Integral() << endl;
    cout << calculateRiemannSum(g)/calculateRiemannSum(g_all) << endl;
    // cout << g->Integral() << endl;
    // cout << g_all->Integral() << endl;
    inf->Close();
    for(string f_wave : f_waves) {
        inf = TFile::Open(("tmp/"+waveset+"_"+f_wave+".root").c_str());
        g = (TGraphErrors*)inf->Get("Pp1-");
        g->Draw("P");

        inf->Close();

        // cout << f_wave << " : " << g->Integral()/g_all->Integral() << endl;
    }
    c->SaveAs("tmp/tmp.pdf");
}

double calculateRiemannSum(TGraphErrors* graph) {
    double sum = 0.0;
    double x, y;
    
    for (int i = 0; i < graph->GetN() - 1; ++i) {
        graph->GetPoint(i, x, y);
        double dx = graph->GetErrorX(i)*2;
        sum += y * dx;
    }
    
    return sum;
}
