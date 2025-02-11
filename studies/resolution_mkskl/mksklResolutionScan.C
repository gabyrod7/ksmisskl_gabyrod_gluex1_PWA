Double_t doubleGaussian(Double_t* x, Double_t* par) {
    double N1 = par[0];
    double N2 = par[1];
    double g1 = TMath::Gaus(x[0], par[2], par[3]); // normalized Gaussian
    double g2 = TMath::Gaus(x[0], par[2], par[4]); // normalized Gaussian
    return N1*g1 + N2*g2;
}

void mksklResolutionScan() {
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

    TCanvas *c;
    TFile *inf;
    TH2F *hist2D;
    TH1F *hist;
    TGraphErrors *gr[3];

    TF1 *model = new TF1("fit", doubleGaussian, -0.1, 0.1, 5);
    model->SetParameters(10, 10, 0., 0.03, 0.016);
    model->SetParLimits(3, 0, 1);
    model->SetParLimits(4, 0, 1);

    TLatex t;

    vector<string> rootFiles = {"hist_acc_sp17.root", "hist_acc_sp18.root", "hist_acc_fa18.root"};
    string h2DName = "res_mkskl";

    vector<double> bins;
    double lbin, ubin, binCenter, resolution;
    double binEdge = 1.1;
    double binWidth = 0.04;
    while(binEdge < 2.60+binWidth) {
        bins.push_back(binEdge);
        binEdge += binWidth;
    }

    c = new TCanvas();
    for(int iFile = 0; iFile < rootFiles.size(); iFile++) {
        string rootFile = rootFiles[iFile];
        inf = TFile::Open(rootFile.c_str());
        hist2D = (TH2F*)inf->Get(h2DName.c_str());
        gr[iFile] = new TGraphErrors(bins.size()-1);
        gr[iFile]->SetMarkerColor(iFile+1);
        gr[iFile]->SetMarkerStyle(8);

        for(int iBin = 0; iBin < bins.size()-1; iBin++) {
            lbin = bins[iBin];
            ubin = bins[iBin+1];
            hist = (TH1F*)hist2D->ProjectionX(
                Form("hist_%d_%.2f_%.2f", iFile, lbin, ubin), 
                hist2D->GetYaxis()->FindFixBin(lbin), 
                hist2D->GetYaxis()->FindFixBin(ubin), "");

            // hist->Draw();
            hist->Fit(model, "RE");

            binCenter = (ubin+lbin)/2.;
            resolution = (model->GetParameter(0)*model->GetParameter(3) + model->GetParameter(1)*model->GetParameter(4)) / (model->GetParameter(0)+model->GetParameter(1));
            gr[iFile]->SetPoint(iBin, binCenter, resolution);
            gr[iFile]->SetPointError(iBin, binWidth/2.0, 0);

        	t.DrawLatex(-0.1, 0.9*hist->GetMaximum(), Form("#sigma = %.3f", resolution));

            c->SaveAs(Form("pdfs/hist_%d_%.2f_%.2f.pdf", iFile, lbin, ubin));
        }
    }

    gr[0]->GetYaxis()->SetRangeUser(0, 0.03);
    gr[0]->GetXaxis()->SetRangeUser(1.1, 2.6);

    gr[0]->SetTitle(";M(K_{S}K_{L}) (GeV);Resolution (GeV)");

    c = new TCanvas();
    gr[0]->Draw("AP");
    gr[1]->Draw("P");
    gr[2]->Draw("P");

    TLegend *lg = new TLegend(0.7, 0.3, 0.95, 0.5);
    lg->AddEntry(gr[0], "Spring 2017", "lep");
    lg->AddEntry(gr[1], "Spring 2018", "lep");
    lg->AddEntry(gr[2], "Fall 2018", "lep");
    lg->Draw();

    c->SaveAs("resolutionScan.pdf");
}