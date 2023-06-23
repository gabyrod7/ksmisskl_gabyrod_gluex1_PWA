
void combine() {
	TFile *sp17 = TFile::Open("sp17TPol.root");
	TFile *sp18 = TFile::Open("sp18TPol75.root");
	TFile *fa18 = TFile::Open("fa18TPol.root");
	TFile *gluex1 = TFile::Open("gluex1Tpol.root", "RECREATE");

	vector<string> hnames = {"hPol0", "hPol45", "hPol90", "hPol135"};
	TH1F *h_sp17, *h_sp18, *h_fa18;

	for(auto  hname : hnames) {
		h_sp17 = (TH1F*)sp17->Get(hname.c_str());
		h_sp18 = (TH1F*)sp18->Get(hname.c_str());
		h_fa18 = (TH1F*)fa18->Get(hname.c_str());

		h_sp17->GetXaxis()->SetRangeUser(7.5, 11.4);
		h_sp18->GetXaxis()->SetRangeUser(7.5, 11.4);
		h_fa18->GetXaxis()->SetRangeUser(7.5, 11.4);

		h_sp17->Scale(0.175);
		h_sp18->Scale(0.504);
		h_fa18->Scale(0.321);

		h_sp17->Add(h_sp18);
		h_sp17->Add(h_fa18);

		h_sp17->GetYaxis()->SetRangeUser(-0.1, 0.5);

		h_sp17->Write(hname.c_str());

		delete h_sp17;
		delete h_sp18;
		delete h_fa18;
	}
}
