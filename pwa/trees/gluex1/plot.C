// Script for making histograms of data that will be used in SDME fits.
// This script is meant to be copied to the directory where the data one
// is interested in is located.

#include <iostream>
#include <fstream>
#include "TLorentzRotation.h"

void plot() {
	TFile *inf;
	vector<string> datasets = {"dat", "acc", "gen", "bkg"};
	vector<string> pols = {"000", "045", "090", "135"};

	int nBins = 50;
	char s[50];
	double lmass = 1.00, umass = 2.00;
	TH1F *hist[4], *hist_all[4], *h1_be, *h1_mandelt, *mpipi;
	TH2F *cosHX[4], *coshx[4], *Phi[4], *phi[4];

	hist[0] = new TH1F("h_dat", ";M(K_{S}K_{L});Counts", nBins, lmass, umass), 
	hist[1] = new TH1F("h_acc", ";M(K_{S}K_{L});Counts", nBins, lmass, umass),
	hist[2] = new TH1F("h_gen", ";M(K_{S}K_{L});Counts", nBins, lmass, umass), 
	hist[3] = new TH1F("h_bkg", ";M(K_{S}K_{L});Counts", nBins, lmass, umass),

	hist_all[0] = new TH1F("h_dat_all", ";M(K_{S}K_{L});Counts", nBins, lmass, umass), 
	hist_all[1] = new TH1F("h_acc_all", ";M(K_{S}K_{L});Counts", nBins, lmass, umass),
	hist_all[2] = new TH1F("h_gen_all", ";M(K_{S}K_{L});Counts", nBins, lmass, umass), 
	hist_all[3] = new TH1F("h_bkg_all", ";M(K_{S}K_{L});Counts", nBins, lmass, umass),

	h1_be = new TH1F("h1_be", ";Beam energy (GeV);Counts", 100, 8, 9);
	h1_mandelt= new TH1F("h1_mandelt", ";-t (GeV^{2});Counts", 7, 0.15, 1.55);
	mpipi = new TH1F("mpipi", ";M(#pi#pi);Counts", 400, 0.3, 0.7);

	cosHX[0] = new TH2F("cosHX_dat", ";M(K_{S}K_{L});cos(#theta_{HX})", nBins, lmass, umass, 30, -1, 1), 
	cosHX[1] = new TH2F("cosHX_acc", ";M(K_{S}K_{L});cos(#theta_{HX})", nBins, lmass, umass, 30, -1, 1),
	cosHX[2] = new TH2F("cosHX_gen", ";M(K_{S}K_{L});cos(#theta_{HX})", nBins, lmass, umass, 30, -1, 1), 
	cosHX[3] = new TH2F("cosHX_bkg", ";M(K_{S}K_{L});cos(#theta_{HX})", nBins, lmass, umass, 30, -1, 1),

	coshx[0] = new TH2F("coshx_dat_all", ";M(K_{S}K_{L});cos(#theta_{HX})", nBins, lmass, umass, 30, -1, 1);
	coshx[1] = new TH2F("coshx_acc_all", ";M(K_{S}K_{L});cos(#theta_{HX})", nBins, lmass, umass, 30, -1, 1);
	coshx[2] = new TH2F("coshx_gen_all", ";M(K_{S}K_{L});cos(#theta_{HX})", nBins, lmass, umass, 30, -1, 1);
	coshx[3] = new TH2F("coshx_bkg_all", ";M(K_{S}K_{L});cos(#theta_{HX})", nBins, lmass, umass, 30, -1, 1);

	Phi[0] = new TH2F("Phi_dat", ";M(K_{S}K_{L});#phi_{HX}", nBins, lmass, umass, 40, -3.14, 3.14);
	Phi[1] = new TH2F("Phi_acc", ";M(K_{S}K_{L});#phi_{HX}", nBins, lmass, umass, 40, -3.14, 3.14);
	Phi[2] = new TH2F("Phi_gen", ";M(K_{S}K_{L});#phi_{HX}", nBins, lmass, umass, 40, -3.14, 3.14);
	Phi[3] = new TH2F("Phi_bkg", ";M(K_{S}K_{L});#phi_{HX}", nBins, lmass, umass, 40, -3.14, 3.14);

	phi[0] = new TH2F("phi_dat_all", ";M(K_{S}K_{L});#phi_{HX}", nBins, lmass, umass, 40, -3.14, 3.14);
	phi[1] = new TH2F("phi_acc_all", ";M(K_{S}K_{L});#phi_{HX}", nBins, lmass, umass, 40, -3.14, 3.14);
	phi[2] = new TH2F("phi_gen_all", ";M(K_{S}K_{L});#phi_{HX}", nBins, lmass, umass, 40, -3.14, 3.14);
	phi[3] = new TH2F("phi_bkg_all", ";M(K_{S}K_{L});#phi_{HX}", nBins, lmass, umass, 40, -3.14, 3.14);

	TFile	*opf = TFile::Open("histograms.root", "RECREATE");

	// Loop through data sets
	for(int i = 0; i < pols.size(); i++) {
		for(int j = 0; j < datasets.size(); j++) {
			cout << "Running over file: " << datasets[j]+pols[i]+".root" << endl;
			inf = TFile::Open( (datasets[j]+pols[i]+".root").c_str() );

			TTree* tree;
			inf->GetObject("kin",tree);

			// ******************************
			// LOAD VARS AND BRANCH ADDRESSSES
			// ******************************
			int 	particles;
			float	particle_es[3], particle_pxs[3], particle_pys[3], particle_pzs[3],
				beam_e, beam_px, beam_py, beam_pz, weight=1;
			
			tree->SetBranchAddress("NumFinalState", &particles);
			tree->SetBranchAddress("E_FinalState", particle_es);
			tree->SetBranchAddress("Px_FinalState", particle_pxs);
			tree->SetBranchAddress("Py_FinalState", particle_pys);
			tree->SetBranchAddress("Pz_FinalState", particle_pzs);
			tree->SetBranchAddress("E_Beam", &beam_e);
			tree->SetBranchAddress("Px_Beam", &beam_px);
			tree->SetBranchAddress("Py_Beam", &beam_py);
			tree->SetBranchAddress("Pz_Beam", &beam_pz);
			tree->SetBranchAddress("Weight", &weight);
			
			TLorentzVector beam_p4;
			TLorentzVector target_p4(0,0,0,0.9382719);
			TLorentzVector recoil_p4;
			TLorentzVector p1_p4;
			TLorentzVector p2_p4;

			// ******************************
			// READ IN DATA
			// ******************************
			Long64_t nentries = tree->GetEntries();
			cout << "There are nentries: " << nentries << endl;

			// Fill histograms
			for(int k = 0; k < nentries; k++) {
//				if(k > 1000) break;
				tree->GetEntry(k);

				beam_p4.SetPxPyPzE(beam_px,beam_py,beam_pz,beam_e);
				recoil_p4.SetPxPyPzE(particle_pxs[0],particle_pys[0],particle_pzs[0],particle_es[0]);
				p1_p4.SetPxPyPzE(particle_pxs[1],particle_pys[1],particle_pzs[1],particle_es[1]);
				p2_p4.SetPxPyPzE(particle_pxs[2],particle_pys[2],particle_pzs[2],particle_es[2]);

				TLorentzVector resonance = p1_p4 + p2_p4;

				hist[j]->Fill(resonance.M(), weight);

				TLorentzRotation resRestBoost( -resonance.BoostVector() );

				TLorentzVector beam_res = resRestBoost * beam_p4;
				TLorentzVector recoil_res = resRestBoost * recoil_p4;
				TLorentzVector p1_res = resRestBoost * p1_p4;

				TVector3 z_hx = -1. * recoil_res.Vect().Unit();
				TVector3 y_hx = (beam_p4.Vect().Unit().Cross(-recoil_p4.Vect().Unit())).Unit();
				TVector3 x_hx = y_hx.Cross(z_hx);

				TVector3 angles( (p1_res.Vect()).Dot(x_hx), (p1_res.Vect()).Dot(y_hx), (p1_res.Vect()).Dot(z_hx) );
				double ks1_phi_hx = angles.Phi();
				double cos1_hx = angles.CosTheta();

				//cosHX[j]->Fill(resonance.M(), cos1_hx, weight);
				//Phi[j]->Fill(resonance.M(), ks1_phi_hx, weight);
				cosHX[j]->Fill(resonance.M2(), cos1_hx, weight);
				Phi[j]->Fill(resonance.M2(), ks1_phi_hx, weight);

				if(datasets[j] == "dat") {
					h1_be->Fill(beam_p4.E(), weight);
					h1_mandelt->Fill( -(target_p4 - recoil_p4).M2(), weight);
					mpipi->Fill(p1_res.M(), weight);
				}
				if(datasets[j] == "bkg") {
					h1_mandelt->Fill( -(target_p4 - recoil_p4).M2(), -weight);
					mpipi->Fill(p1_res.M(), -weight);
				}
			} // loop over entries

			inf->Close();
		} // loop over data set types (dat, acc, gen, ...)

		opf->cd();
		gDirectory->mkdir(pols[i].c_str())->cd();

		hist[0]->Write( ("hist_"+datasets[0]).c_str() );
		hist[1]->Write( ("hist_"+datasets[1]).c_str() );
		hist[2]->Write( ("hist_"+datasets[2]).c_str() );
		hist[3]->Write( ("hist_"+datasets[3]).c_str() );

		cosHX[0]->Write( ("cosHX_"+datasets[0]).c_str() );
		cosHX[1]->Write( ("cosHX_"+datasets[1]).c_str() );
		cosHX[2]->Write( ("cosHX_"+datasets[2]).c_str() );
		cosHX[3]->Write( ("cosHX_"+datasets[3]).c_str() );

		hist_all[0]->Add(hist[0]);
		hist_all[1]->Add(hist[1]);
		hist_all[2]->Add(hist[2]);
		hist_all[3]->Add(hist[3]);

		coshx[0]->Add(cosHX[0]);
		coshx[1]->Add(cosHX[1]);
		coshx[2]->Add(cosHX[2]);
		coshx[3]->Add(cosHX[3]);

		phi[0]->Add(Phi[0]);
		phi[1]->Add(Phi[1]);
		phi[2]->Add(Phi[2]);
		phi[3]->Add(Phi[3]);

		hist[0]->Reset();
		hist[1]->Reset();
		hist[2]->Reset();
		hist[3]->Reset();

		cosHX[0]->Reset();
		cosHX[1]->Reset();
		cosHX[2]->Reset();
		cosHX[3]->Reset();

		h1_mandelt->Write("mandelstam_t");
	} // loop over polarizations

	opf->cd();
	hist_all[0]->Write("hist_all");
	hist_all[1]->Write("hist_acc");
	hist_all[2]->Write("hist_gen");
	hist_all[3]->Write("hist_bkg");
//	hist_all[1]->Divide(hist_all[2]);
	hist_all[1]->Write("hist_eff");

	hist_all[0]->Add(hist_all[3], -1);
	hist_all[0]->Write("hist_sig");
	hist_all[1]->Draw();

	coshx[0]->Add(coshx[3], -1);
	coshx[0]->Write("coshx");

	phi[0]->Add(phi[3], -1);
	phi[0]->Write("phi");

	h1_be->Write("beam_energy");
	h1_mandelt->Write("mandelstam_t");
	mpipi->Write("mpipi");
} // end of function
