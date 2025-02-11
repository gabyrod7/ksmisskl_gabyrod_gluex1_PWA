// make a plot of variations (no event selections) to the kskl mass fit 

void plot_wInterference_variations() {
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

	gStyle->SetMarkerStyle(8);
	gStyle->SetMarkerSize(1.);

	gROOT->ForceStyle();

    TFile *inf1 = TFile::Open("rootFiles/fit_wInterference_pol1.root");
    // TFile *inf2 = TFile::Open("rootFiles/fit_wInterference_pol3.root");
    TFile *inf3 = TFile::Open("rootFiles/fit_wInterference_expo.root");
    TFile *inf4 = TFile::Open("rootFiles/fit_wInterference_spin3.root");
    TFile *inf5 = TFile::Open("rootFiles/fit_wInterference_noAccCorr.root");
    TFile *inf6 = TFile::Open("rootFiles/fit_wInterference_changeBinning.root");
    TFile *inf7 = TFile::Open("rootFiles/fit_wInterference_changeRange.root");

    map<string, TGraphErrors*> graphs = {
        {"m1", nullptr},
        {"g1", nullptr},
        {"m2", nullptr},
        {"g2", nullptr},
        {"phase", nullptr}
    };
    vector<string> pNames = {"m1", "g1", "m2", "g2", "phase"};
    TGraphErrors *g1, *g2, *g3, *g4, *g5, *g6, *g7;

    TLine *line = new TLine();
    line->SetLineColor(kGreen);
	line->SetLineWidth(2);
    
    TBox *box = new TBox();
    box->SetFillColorAlpha(kGreen, 0.2);

    TH1F *frame = new TH1F("frame", "", 10, 0, 10);
    frame->GetXaxis()->SetBinLabel(1, "pol1");
    frame->GetXaxis()->SetBinLabel(2, "expo");
    frame->GetXaxis()->SetBinLabel(3, "spin3");
    frame->GetXaxis()->SetBinLabel(4, "Fit Range 1.14-2.66 GeV");
    frame->GetXaxis()->SetBinLabel(5, "Fit Range 1.2-2.54 GeV");
    frame->GetXaxis()->SetBinLabel(6, "Fit Range 1.16-2.5 GeV");
    frame->GetXaxis()->SetBinLabel(7, "Fit Range 1.16-2.7 GeV");
    frame->GetXaxis()->SetBinLabel(8, "No Acc. Corr.");
    frame->GetXaxis()->SetBinLabel(9, "22.5 MeV Bins");
    frame->GetXaxis()->SetBinLabel(10, "14.4 MeV Bins");

    double pm1 = 1.539, pm1_err = 0.005;
    double pm2 = 1.753, pm2_err = 0.002;
    double pg1 = 0.246, pg1_err = 0.017;
    double pg2 = 0.127, pg2_err = 0.009;
    double pphase = 0.93, pphase_err = 0.32;

    int count = 0;

    for(auto pName : pNames) {
        g1 = (TGraphErrors*)inf1->Get(pName.c_str());
        // g2 = (TGraphErrors*)inf2->Get(pName.c_str());
        g3 = (TGraphErrors*)inf3->Get(pName.c_str());
        g4 = (TGraphErrors*)inf4->Get(pName.c_str());
        g5 = (TGraphErrors*)inf5->Get(pName.c_str());
        g6 = (TGraphErrors*)inf6->Get(pName.c_str());
        g7 = (TGraphErrors*)inf7->Get(pName.c_str());

        graphs[pName] = new TGraphErrors();
        // cout << pName << "  " << g2->GetPointY(0) << endl;

        graphs[pName]->SetPoint(count, count+0.5, g1->GetPointY(0));
        graphs[pName]->SetPointError(count, 0, g1->GetErrorY(0));

        graphs[pName]->SetPoint(count+1, count+1.5, g3->GetPointY(0));
        graphs[pName]->SetPointError(count+1, 0, g3->GetErrorY(0));

        graphs[pName]->SetPoint(count+2, count+2.5, g4->GetPointY(0));
        graphs[pName]->SetPointError(count+2, 0, g4->GetErrorY(0));
        
        graphs[pName]->SetPoint(count+3, count+3.5, g7->GetPointY(0));
        graphs[pName]->SetPointError(count+3, 0, g7->GetErrorY(0));

        graphs[pName]->SetPoint(count+4, count+4.5, g7->GetPointY(1));
        graphs[pName]->SetPointError(count+4, 0, g7->GetErrorY(1));

        graphs[pName]->SetPoint(count+5, count+5.5, g7->GetPointY(2));
        graphs[pName]->SetPointError(count+5, 0, g7->GetErrorY(2));

        graphs[pName]->SetPoint(count+6, count+6.5, g7->GetPointY(3));
        graphs[pName]->SetPointError(count+6, 0, g7->GetErrorY(3));

        graphs[pName]->SetPoint(count+7, count+7.5, g5->GetPointY(0));
        graphs[pName]->SetPointError(count+7, 0, g5->GetErrorY(0));

        graphs[pName]->SetPoint(count+8, count+8.5, g6->GetPointY(0));
        graphs[pName]->SetPointError(count+8, 0, g6->GetErrorY(0));

        graphs[pName]->SetPoint(count+9, count+9.5, g6->GetPointY(1));
        graphs[pName]->SetPointError(count+9, 0, g6->GetErrorY(1));
    }

    TCanvas *c = new TCanvas();
    c->SetBottomMargin(0.4);
	frame->LabelsOption("v");
	frame->GetYaxis()->SetRangeUser(pm1 - 8*pm1_err, pm1 + 8*pm1_err);
	frame->GetYaxis()->SetTitle("M_{1} Parameter (GeV)");
    frame->Draw();
    graphs["m1"]->Draw("P");
    line->DrawLine(0, pm1, 10, pm1);
    box->DrawBox(0, pm1 - pm1_err, 10, pm1 + pm1_err);
    c->SaveAs("pdf_wInterference/otherVariation_m1Scan.pdf");

    frame->GetYaxis()->SetRangeUser(pm2 - 8*pm2_err, pm2 + 8*pm2_err);
    frame->GetYaxis()->SetTitle("M_{2} Parameter (GeV)");
    frame->Draw();
    graphs["m2"]->Draw("P");
    line->DrawLine(0, pm2, 10, pm2);
    box->DrawBox(0, pm2 - pm2_err, 10, pm2 + pm2_err);
    c->SaveAs("pdf_wInterference/otherVariation_m2Scan.pdf");

    frame->GetYaxis()->SetRangeUser(pg1 - 5 *pg1_err, pg1 + 5*pg1_err);
    frame->GetYaxis()->SetTitle("#Gamma_{1} Parameter (GeV)");
    frame->Draw();
    graphs["g1"]->Draw("P");
    line->DrawLine(0, pg1, 10, pg1);
    box->DrawBox(0, pg1 - pg1_err, 10, pg1 + pg1_err);
    c->SaveAs("pdf_wInterference/otherVariation_g1Scan.pdf");

    frame->GetYaxis()->SetRangeUser(pg2 - 5*pg2_err, pg2 + 5*pg2_err);
    frame->GetYaxis()->SetTitle("#Gamma_{2} Parameter (GeV)");
    frame->Draw();
    graphs["g2"]->Draw("P");
    line->DrawLine(0, pg2, 10, pg2);
    box->DrawBox(0, pg2 - pg2_err, 10, pg2 + pg2_err);
    c->SaveAs("pdf_wInterference/otherVariation_g2Scan.pdf");

    frame->GetYaxis()->SetRangeUser(0, TMath::Pi());
    frame->GetYaxis()->SetTitle("#Delta#phi Parameter (GeV)");
    frame->Draw();
    graphs["phase"]->Draw("P");
    line->DrawLine(0, pphase, 10, pphase);
    box->DrawBox(0, pphase - pphase_err, 10, pphase + pphase_err);
    c->SaveAs("pdf_wInterference/otherVariation_phaseScan.pdf");
}
