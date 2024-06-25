#ifndef DSelector_kskl_h
#define DSelector_kskl_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"

#include "TLorentzRotation.h"

class DSelector_kskl : public DSelector
{
	public:

		DSelector_kskl(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_kskl(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Get_ComboWrappers(void);
		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO

		bool dIsMC;

		bool hasPolarizationAngle;
		int locPolarizationAngle;

		// ANALYZE CUT ACTIONS
		// // Automatically makes mass histograms where one cut is missing
		DHistogramAction_AnalyzeCutActions* dAnalyzeCutActions;

		//CREATE REACTION-SPECIFIC PARTICLE ARRAYS

		//Step 0
		DParticleComboStep* dStep0Wrapper;
		DBeamParticle* dComboBeamWrapper;
		DChargedTrackHypothesis* dProtonWrapper;
		DKinematicData* dMissingKLongWrapper;

		//Step 1
		DParticleComboStep* dStep1Wrapper;
		DChargedTrackHypothesis* dPiMinusWrapper;
		DChargedTrackHypothesis* dPiPlusWrapper;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		TH1I* dHist_MissingMassSquared;
		TH1I* dHist_BeamEnergy;

		TH1F *h1_RFTime;
		TH1F *h1_RFTime_sb;

		TH1F *h1_ChiSqNdf;
		TH1F *h1_FS;
		TH1F *h1_KsProperTime;
		TH1F *h1_NumUnusedTracks;
		TH1F *h1_NumUnusedShowers;
		TH1F *h1_mandelt;
		TH1F *im_pipi;
		TH1F *im_miss;
		TH1F *h1_MissingEnergy;
		TH1F *im_kskl;
		TH1F *im_ksp;
		TH1F *im_klp;

		TH1F *h1_ChiSqNdf_sb;
		TH1F *h1_FS_sb;
		TH1F *h1_KsProperTime_sb;
		TH1F *h1_mandelt_sb;
		TH1F *im_miss_sb;
		TH1F *im_kskl_sb;
		TH1F *im_ksp_sb;
		TH1F *im_klp_sb;

		TH2F *h2_mpipi_rftime;

		TH3F *h3_mpipi_fs_chisq;
		TH3F *h3_mpipi_fs_chisq2;

	ClassDef(DSelector_kskl, 0);
};

void DSelector_kskl::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dProtonWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(1));
	dMissingKLongWrapper = dStep0Wrapper->Get_FinalParticle(2);

	//Step 1
	dStep1Wrapper = dComboWrapper->Get_ParticleComboStep(1);
	dPiMinusWrapper = static_cast<DChargedTrackHypothesis*>(dStep1Wrapper->Get_FinalParticle(0));
	dPiPlusWrapper = static_cast<DChargedTrackHypothesis*>(dStep1Wrapper->Get_FinalParticle(1));
}

#endif // DSelector_kskl_h
