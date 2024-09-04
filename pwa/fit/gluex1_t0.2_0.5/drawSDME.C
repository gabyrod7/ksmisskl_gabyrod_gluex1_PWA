void set_graph_point(TGraphErrors *g, int n , double x, double ex, double y, double ey);
void draw_graph(TGraphErrors *g, const char* gtitle, float xmin, float xmax, float ymin, float ymax, bool hline, float hval);

void drawSDME(int nBins = 8, double lowT = 0.15, double highT = 0.95) {
	gStyle->SetPadTopMargin(0.03);
	gStyle->SetPadRightMargin(0.01);
	gStyle->SetPadBottomMargin(0.11);
	gStyle->SetPadLeftMargin(0.08);

	gStyle->SetTitleBorderSize(0);
	
	gStyle->SetTitleFont(132);
	gStyle->SetTitleSize(0.05);
	gStyle->SetTitleOffset(1.2);
	
	gStyle->SetLabelSize(0.05,"XY");
	gStyle->SetLabelOffset(0.01,"XY");  
	
	gStyle->SetTitleSize(0.05,"XY");
	gStyle->SetTitleOffset(1.1,"X");
	
	gStyle->SetMarkerStyle(8);
	gStyle->SetMarkerSize(0.0);

	FILE *fin = fopen("sdme.csv","r");
	// FILE *fin2 = fopen("9bins_1.50t/tBins.txt","r");
	TFile *opf = TFile::Open("sdme.root", "RECREATE");

	// double bin_width = (highT-lowT)/(nBins);
	//double bin_width = ( TMath::Log10(highT) - TMath::Log10(lowT) ) / nBins;
	//vector<double> bin = {0.162, 0.189, 0.220, 0.257, 0.300, 0.349, 0.406, 0.474 ,0.551, 0.643, 0.748, 0.874, 1.019, 1.182, 1.380};
	
	int bin;
	double t;
	double te;
	double rho000;
	double rho000e;
	double rho100;
	double rho100e;
	double rho1m10;
	double rho1m10e;
	double rho111;
	double rho111e;
	double rho001;
	double rho001e;
	double rho101;
	double rho101e;
	double rho1m11;
	double rho1m11e;
	double rho102;
	double rho102e;
	double rho1m12;
	double rho1m12e;
	double Psigma;

	TGraphErrors	*g_rho000  = new TGraphErrors(nBins), 
			*g_rho100  = new TGraphErrors(nBins), 
			*g_rho1m10 = new TGraphErrors(nBins), 
			*g_rho111  = new TGraphErrors(nBins), 
			*g_rho001  = new TGraphErrors(nBins), 
			*g_rho101  = new TGraphErrors(nBins), 
			*g_rho1m11 = new TGraphErrors(nBins), 
			*g_rho102  = new TGraphErrors(nBins),
			*g_rho1m12 = new TGraphErrors(nBins),
			*g_Psigma  = new TGraphErrors(nBins);

	for(int iBin = 0; iBin < nBins; iBin++) {

		fscanf(fin,"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf \n", &t, &te, &rho000, &rho000e, &rho100,  &rho100e,  &rho1m10,  &rho1m10e,  &rho111,  &rho111e,  &rho001 ,  &rho001e ,  &rho101 ,  &rho101e,  &rho1m11 ,  &rho1m11e ,  &rho102 ,  &rho102e ,  &rho1m12 ,  &rho1m12e);

		Psigma = 2*rho1m11 - rho001;

		set_graph_point(g_rho000, iBin, t, te, rho000, rho000e);
		set_graph_point(g_rho100, iBin, t, te, rho100, rho100e);
		set_graph_point(g_rho1m10, iBin, t, te, rho1m10, rho1m10e);
		set_graph_point(g_rho111, iBin, t, te, rho111, rho111e);
		set_graph_point(g_rho001, iBin, t, te, rho001, rho001e);
		set_graph_point(g_rho101, iBin, t, te, rho101, rho101e);
		set_graph_point(g_rho1m11, iBin, t, te, rho1m11, rho1m11e);
		set_graph_point(g_rho102, iBin, t, te, rho102, rho102e);
		set_graph_point(g_rho1m12, iBin, t, te, rho1m12, rho1m12e);

		g_Psigma->SetPoint(iBin, t, Psigma);
	}

	TCanvas *c = new TCanvas();
	c->Divide(3,3);

	c->cd(1);
	draw_graph(g_rho000, "#rho_{00}^{0}",   lowT, highT, -0.17, 0.17, kTRUE, 0.00);
	g_rho000->Write("g_rho000");

	c->cd(2);
	draw_graph(g_rho100, "Re(#rho_{10}^{0})",   lowT, highT, -0.17, 0.17, kTRUE, 0.00);
	g_rho100->Write("g_rho100");

	c->cd(3);
	draw_graph(g_rho1m10, "#rho_{1-1}^{0}", lowT, highT, -0.17, 0.17, kTRUE, 0.00);
	g_rho1m10->Write("g_rho1m10");

	c->cd(4);
	draw_graph(g_rho111, "#rho_{11}^{1}",   lowT, highT, -0.17, 0.17, kTRUE, 0.00);
	g_rho111->Write("g_rho111");

	c->cd(5);
	draw_graph(g_rho001, "#rho_{00}^{1}",   lowT, highT, -0.17, 0.17, kTRUE, 0.00);
	g_rho001->Write("g_rho001");

	c->cd(6);
	draw_graph(g_rho101, "Re(#rho_{10}^{1})",   lowT, highT, -0.17, 0.17, kTRUE, 0.00);
	g_rho101->Write("g_rho101");

	c->cd(7);
	draw_graph(g_rho1m11, "#rho_{1-1}^{1}", lowT, highT, 0.25, 0.75, kTRUE, 0.50);
	g_rho1m11->Write("g_rho1m11");

	c->cd(8);
	draw_graph(g_rho102, "Im(#rho_{10}^{2})",   lowT, highT, -0.17, 0.17, kTRUE, 0.00);
	g_rho102->Write("g_rho102");

	c->cd(9);
	draw_graph(g_rho1m12, "Im(#rho_{1-1}^{2})", lowT, highT, -0.75, -0.25, kTRUE, -0.50);
	g_rho1m12->Write("g_rho1m12");

	c->Write("c_sdme");
	c->SaveAs("sdme.pdf");
	c->SaveAs("sdmee.png");

	// c = new TCanvas();
	// g_Psigma->GetYaxis()->SetRangeUser(-1.1, 1.1);
	// g_Psigma->Draw("AP");
	// c->SaveAs("Psigma.pdf");
}

void set_graph_point(TGraphErrors *g, int n, double x, double ex, double y, double ey) {
	g->SetPoint(n, x, y);
	g->SetPointError(n, ex, ey);
	g->SetMarkerStyle(8);
	g->SetMarkerColor(kBlack);
}

void draw_graph(TGraphErrors *g, const char* gtitle, float xmin, float xmax, float ymin, float ymax, bool hline, float hval) {
	TH1 *frame = gPad->DrawFrame(xmin,ymin,xmax,ymax);

	g->Draw( "P" );

	g->SetTitle(";-t (GeV^{2})");

	g->GetYaxis()->SetRangeUser(ymin, ymax);
	g->GetXaxis()->SetRangeUser(xmin,xmax);

	TLatex t;
	t.SetTextSize(0.12);
	//if(ymax > 0)
	//	t.DrawLatex(0.50*xmax, 0.80*ymax, gtitle);
	//else
	//	t.DrawLatex(0.50*xmax, 1.20*ymax, gtitle);
	if(ymax < 0)
		t.DrawLatex(0.45*xmax, ymax + (1-0.83)*(ymin - ymax), gtitle);
	else
		t.DrawLatex(0.45*xmax, ymin + 0.83*(ymax - ymin), gtitle);

	if (hline) {
		TLine* l = new TLine(xmin, hval, xmax, hval);
		l->Draw();
	}
}
