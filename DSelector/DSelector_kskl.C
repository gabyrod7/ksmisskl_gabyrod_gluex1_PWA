#include "DSelector_kskl.h"

TVector3 calc_helicity_angles(TLorentzVector beam, TLorentzVector recoil, TLorentzVector p1, TLorentzVector p2) {
	TLorentzVector resonance = p1 + p2;
	
	TLorentzRotation resRestBoost( -resonance.BoostVector() );
	
	TLorentzVector beam_res   = resRestBoost * beam;
	TLorentzVector recoil_res = resRestBoost * recoil;
	TLorentzVector p1_res = resRestBoost * p1;

	// Helicity frame
	TVector3 z = -1. * recoil_res.Vect().Unit();
	
	// normal to the production plane
	TVector3 y = (beam.Vect().Unit().Cross(-recoil.Vect().Unit())).Unit();
	
	TVector3 x = y.Cross(z);
	
	TVector3 angles( (p1_res.Vect()).Dot(x),
	(p1_res.Vect()).Dot(y),
	(p1_res.Vect()).Dot(z) );
	

	return angles;
}

TVector3 calc_GJ_angles(TLorentzVector beam, TLorentzVector recoil, TLorentzVector p1, TLorentzVector p2) {
	TLorentzVector resonance = p1 + p2;
	
	TLorentzRotation resRestBoost( -resonance.BoostVector() );
	
	TLorentzVector beam_res   = resRestBoost * beam;
	TLorentzVector recoil_res = resRestBoost * recoil;
	TLorentzVector p1_res = resRestBoost * p1;

	// GJ frame
	TVector3 z = beam_res.Vect().Unit();
	
	// normal to the production plane
	TVector3 y = (beam.Vect().Unit().Cross(-recoil.Vect().Unit())).Unit();
	
	TVector3 x = y.Cross(z);
	
	TVector3 angles( (p1_res.Vect()).Dot(x),
	(p1_res.Vect()).Dot(y),
	(p1_res.Vect()).Dot(z) );
	
	return angles;
}

double calc_vanHove_X(double p1, double p2, double p3) {
	double q = TMath::Sqrt(p1*p1 + p2*p2 + p3*p3);
	double omega = TMath::ATan2( -1*TMath::Sqrt(3.)*p3*p3, p1 + p3) + TMath::Pi();

	return q*TMath::Cos(omega);
}

double calc_vanHove_Y(double p1, double p2, double p3) {
	double q = TMath::Sqrt(p1*p1 + p2*p2 + p3*p3);
	double omega = TMath::ATan2( -1*TMath::Sqrt(3.)*p3*p3, p1 + p3) + TMath::Pi();

	return q*TMath::Sin(omega);

}

void DSelector_kskl::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "tree.root"; //"" for none
	dOutputTreeFileName = ""; //"" for none
	dFlatTreeFileName = "ftree.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = "kskl"; //if blank, default name will be chosen
	//dSaveDefaultFlatBranches = true; // False: don't save default branches, reduce disk footprint.
	//dSaveTLorentzVectorsAsFundamentaFlatTree = false; // Default (or false): save particles as TLorentzVector objects. True: save as four doubles instead.

	//Because this function gets called for each TTree in the TChain, we must be careful:
		//We need to re-initialize the tree interface & branch wrappers, but don't want to recreate histograms
	bool locInitializedPriorFlag = dInitializedFlag; //save whether have been initialized previously
	DSelector::Init(locTree); //This must be called to initialize wrappers for each new TTree
	//gDirectory now points to the output file with name dOutputFileName (if any)
	if(locInitializedPriorFlag)
		return; //have already created histograms, etc. below: exit

	Get_ComboWrappers();
	dPreviousRunNumber = 0;

	/*********************************** EXAMPLE USER INITIALIZATION: ANALYSIS ACTIONS **********************************/

	// EXAMPLE: Create deque for histogramming particle masses:
	// // For histogramming the phi mass in phi -> K+ K-
	// // Be sure to change this and dAnalyzeCutActions to match reaction
	std::deque<Particle_t> MyPhi;
	MyPhi.push_back(KPlus); MyPhi.push_back(KMinus);

	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
	//false/true below: use measured/kinfit data

	//PID
	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
	//below: value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
	//dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, KPlus, SYS_BCAL));

	//PIDFOM (for charged tracks)
	dAnalysisActions.push_back(new DHistogramAction_PIDFOM(dComboWrapper));
	//dAnalysisActions.push_back(new DCutAction_PIDFOM(dComboWrapper, KPlus, 0.1));
	//dAnalysisActions.push_back(new DCutAction_EachPIDFOM(dComboWrapper, 0.1));

	//MASSES
	//dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, Lambda, 1000, 1.0, 1.2, "Lambda"));
	//dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1));

	//KINFIT RESULTS
	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));

	//CUT MISSING MASS
	//dAnalysisActions.push_back(new DCutAction_MissingMassSquared(dComboWrapper, false, -0.03, 0.02));

	//CUT ON SHOWER QUALITY
	//dAnalysisActions.push_back(new DCutAction_ShowerQuality(dComboWrapper, SYS_FCAL, 0.5));

	//BEAM ENERGY
	dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));
	//dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8.2, 8.8));  // Coherent peak for runs in the range 30000-59999

	//KINEMATICS
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	// ANALYZE CUT ACTIONS
	// // Change MyPhi to match reaction
	dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 0, MyPhi, 1000, 0.9, 2.4, "CutActionEffect" );

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();
	dAnalyzeCutActions->Initialize(); // manual action, must call Initialize()

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	//EXAMPLE MANUAL HISTOGRAMS:
	dHist_MissingMassSquared = new TH1I("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -0.06, 0.06);
	dHist_BeamEnergy = new TH1I("BeamEnergy", ";Beam Energy (GeV)", 600, 0.0, 12.0);

	h1_RFTime = new TH1F("h1_RFTime", ";|t_{Beam} _ t_{RF}| (ns);Counts / 0.1 ns", 280, -14, 14);
	h1_ChiSqNdf = new TH1F("h1_ChiSqNdf", ";#chi^{2}/ndf;Counts", 60, 0, 6);
	h1_FS = new TH1F("h1_FS", ";Flight Significance (#sigma);Counts", 50, 0, 10);
	h1_KsProperTime = new TH1F("h1_KsProperTime", ";K_{S} proper time (s);Counts", 50, 0, 0.5);
	h1_NumUnusedTracks = new TH1F("h1_NumUnusedTracks", ";Number of Unused Tracks;Counts", 4, 0, 4);
	h1_NumUnusedShowers = new TH1F("h1_NumUnusedShowers", ";Number of Unused Showers;Counts", 10, 0, 10);
	h1_mandelt = new TH1F("h1_mandelt", ";-t (GeV^{2});Counts", 150, 0, 1.5);
	im_pipi = new TH1F("im_pipi", ";M(#pi^{+}#pi^{-}) (GeV);Counts", 300, 0.3, 0.6);
	im_miss = new TH1F("im_miss", ";Missing Mass (GeV);Counts", 100, 0, 1);
	im_kskl = new TH1F("im_kskl", ";M(K_{S}K_{L}) (GeV);Counts", 120, 1.00, 2.20);
	im_ksp = new TH1F("im_ksp", ";M(K_{S}p) (GeV);Counts", 110, 2.50, 3.40);
	im_klp = new TH1F("im_klp", ";M(K_{L}p) (GeV);Counts", 110, 2.50, 3.40);

	h1_ChiSqNdf_sb = new TH1F("h1_ChiSqNdf_sb", ";#chi^{2}/ndf;Counts", 60, 0, 6);
	h1_FS_sb = new TH1F("h1_FS_sb", ";Flight Significance (#sigma);Counts", 50, 0, 10);
	h1_KsProperTime_sb = new TH1F("h1_KsProperTime_sb", ";K_{S} proper time (s);Counts", 50, 0, 0.5);
	h1_mandelt_sb = new TH1F("h1_mandelt_sb", ";-t (GeV^{2});Counts", 150, 0, 1.5);
	im_miss_sb = new TH1F("im_miss_sb", ";Missing Mass (GeV);Counts", 100, 0, 1);
	im_kskl_sb = new TH1F("im_kskl_sb", ";M(K_{S}K_{L}) (GeV);Counts", 120, 1.10, 1.20);
	im_ksp_sb = new TH1F("im_ksp_sb", ";M(K_{S}p) (GeV);Counts", 110, 2.50, 3.40);
	im_klp_sb = new TH1F("im_klp_sb", ";M(K_{L}p) (GeV);Counts", 110, 2.50, 3.40);

	h2_mpipi_rftime = new TH2F("h2_mpipi_rftime", ";M(#pi#pi);RFTime (ns)", 100, 0.4, 0.6, 100, -16, 16);

	h3_mpipi_fs_chisq = new TH3F("h3_mpipi_fs_chisq", ";M(#pi^{+}#pi^{-};Flight Significance;#chi^{2}/ndf", 300, 0.3, 0.6, 100, 0, 10, 60, 0, 6);
	h3_mpipi_fs_chisq2 = new TH3F("h3_mpipi_fs_chisq2", ";M(#pi^{+}#pi^{-};Flight Significance;#chi^{2}/ndf", 300, 0.3, 0.6, 100, 0, 10, 60, 0, 6);

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - MAIN TREE *************************/

	//EXAMPLE MAIN TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
	/*
	dTreeInterface->Create_Branch_Fundamental<Int_t>("my_int"); //fundamental = char, int, float, double, etc.
	dTreeInterface->Create_Branch_FundamentalArray<Int_t>("my_int_array", "my_int");
	dTreeInterface->Create_Branch_FundamentalArray<Float_t>("my_combo_array", "NumCombos");
	dTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("my_p4");
	dTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("my_p4_array");
	*/

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - FLAT TREE *************************/

	// RECOMMENDED: CREATE ACCIDENTAL WEIGHT BRANCH
	// dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("accidweight");

	//EXAMPLE FLAT TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
	/*
	dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("flat_my_int"); //fundamental = char, int, float, double, etc.
	dFlatTreeInterface->Create_Branch_FundamentalArray<Int_t>("flat_my_int_array", "flat_my_int");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("flat_my_p4");
	dFlatTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("flat_my_p4_array");
	*/

	dFlatTreeInterface->Create_Branch_Fundamental<double>("event_weight");
	dFlatTreeInterface->Create_Branch_Fundamental<double>("accidental_weight");

	dFlatTreeInterface->Create_Branch_Fundamental<bool>("is_in_time");

	dFlatTreeInterface->Create_Branch_Fundamental<int>("num_unused_tracks");
	dFlatTreeInterface->Create_Branch_Fundamental<int>("num_unused_showers");
	dFlatTreeInterface->Create_Branch_Fundamental<int>("num_combos");

	dFlatTreeInterface->Create_Branch_Fundamental<double>("mpipi");
	dFlatTreeInterface->Create_Branch_Fundamental<double>("missing_mass");
	dFlatTreeInterface->Create_Branch_Fundamental<double>("mkskl");

	dFlatTreeInterface->Create_Branch_Fundamental<double>("beam_energy");
	dFlatTreeInterface->Create_Branch_Fundamental<double>("mandel_t");
	dFlatTreeInterface->Create_Branch_Fundamental<double>("ks_proper_time");

	dFlatTreeInterface->Create_Branch_Fundamental<double>("flight_significance");
	dFlatTreeInterface->Create_Branch_Fundamental<double>("path_length");
	dFlatTreeInterface->Create_Branch_Fundamental<double>("chisq_ndf");

	dFlatTreeInterface->Create_Branch_Fundamental<double>("cos_hel_ks");
	dFlatTreeInterface->Create_Branch_Fundamental<double>("phi_hel_ks");

	dFlatTreeInterface->Create_Branch_Fundamental<double>("cos_gj_ks");
	dFlatTreeInterface->Create_Branch_Fundamental<double>("phi_gj_ks");

	dFlatTreeInterface->Create_Branch_Fundamental<double>("vanHove_x");
	dFlatTreeInterface->Create_Branch_Fundamental<double>("vanHove_y");

//	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("pip_p4");
//	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("pim_p4");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("ks_p4");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("kl_p4");

//	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("pip_p4_meas");
//	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("pim_p4_meas");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("ks_p4_meas");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("missing_p4_meas");

	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("pip_p4_cm");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("pim_p4_cm");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("ks_p4_cm");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("kl_p4_cm");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("recoil_p4_cm");

	// For AmpTools
	dFlatTreeInterface->Create_Branch_Fundamental<bool>("amptools_dat");
	dFlatTreeInterface->Create_Branch_Fundamental<bool>("amptools_bkg");
	SetupAmpTools_FlatTree();

	dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("Target_Mass"); // necesary for divideData.pl (I think)
	dFlatTreeInterface->Create_Branch_FundamentalArray<Int_t>("PID_FinalState","NumFinalState");
	dFlatTreeInterface->Create_Branch_Fundamental<int>("pol_angle");

	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want

	/************************************** DETERMINE IF ANALYZING SIMULATED DATA *************************************/

	dIsMC = (dTreeInterface->Get_Branch("MCWeight") != NULL);
	if(dIsMC)	dFlatTreeInterface->Create_Branch_Fundamental<bool>("dIsMC");
}

Bool_t DSelector_kskl::Process(Long64_t locEntry)
{
	// The Process() function is called for each entry in the tree. The entry argument
	// specifies which entry in the currently loaded tree is to be processed.
	//
	// This function should contain the "body" of the analysis. It can contain
	// simple or elaborate selection criteria, run algorithms on the data
	// of the event and typically fill histograms.
	//
	// The processing can be stopped by calling Abort().
	// Use fStatus to set the return value of TTree::Process().
	// The return value is currently not used.

	//CALL THIS FIRST
	DSelector::Process(locEntry); //Gets the data from the tree for the entry
	//cout << "RUN " << Get_RunNumber() << ", EVENT " << Get_EventNumber() << endl;
	//TLorentzVector locProductionX4 = Get_X4_Production();

	/******************************************** GET POLARIZATION ORIENTATION ******************************************/

	//Only if the run number changes
	//RCDB environment must be setup in order for this to work! (Will return false otherwise)
	UInt_t locRunNumber = Get_RunNumber();
	if(locRunNumber != dPreviousRunNumber)
	{
		hasPolarizationAngle = dAnalysisUtilities.Get_PolarizationAngle(locRunNumber, locPolarizationAngle);

		dIsPolarizedFlag = dAnalysisUtilities.Get_IsPolarizedBeam(locRunNumber, dIsPARAFlag);
		dPreviousRunNumber = locRunNumber;
	}

	/********************************************* SETUP UNIQUENESS TRACKING ********************************************/

	//ANALYSIS ACTIONS: Reset uniqueness tracking for each action
	//For any actions that you are executing manually, be sure to call Reset_NewEvent() on them here
	Reset_Actions_NewEvent();
	dAnalyzeCutActions->Reset_NewEvent(); // manual action, must call Reset_NewEvent()

	//PREVENT-DOUBLE COUNTING WHEN HISTOGRAMMING
		//Sometimes, some content is the exact same between one combo and the next
			//e.g. maybe two combos have different beam particles, but the same data for the final-state
		//When histogramming, you don't want to double-count when this happens: artificially inflates your signal (or background)
		//So, for each quantity you histogram, keep track of what particles you used (for a given combo)
		//Then for each combo, just compare to what you used before, and make sure it's unique

	//EXAMPLE 1: Particle-specific info:
	set<Int_t> locUsedSoFar_BeamEnergy; //Int_t: Unique ID for beam particles. set: easy to use, fast to search

	//EXAMPLE 2: Combo-specific info:
		//In general: Could have multiple particles with the same PID: Use a set of Int_t's
		//In general: Multiple PIDs, so multiple sets: Contain within a map
		//Multiple combos: Contain maps within a set (easier, faster to search)
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MissingMass;

	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE

	/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

	/*
	Int_t locMyInt = 7;
	dTreeInterface->Fill_Fundamental<Int_t>("my_int", locMyInt);

	TLorentzVector locMyP4(4.0, 3.0, 2.0, 1.0);
	dTreeInterface->Fill_TObject<TLorentzVector>("my_p4", locMyP4);

	for(int loc_i = 0; loc_i < locMyInt; ++loc_i)
		dTreeInterface->Fill_Fundamental<Int_t>("my_int_array", 3*loc_i, loc_i); //2nd argument = value, 3rd = array index
	*/

	/************************************************* LOOP OVER COMBOS *************************************************/

	//Loop over combos
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i)
	{
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);

		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut()) // Is false when tree originally created
			continue; // Combo has been cut previously

		/********************************************** GET PARTICLE INDICES *********************************************/

		//Used for tracking uniqueness when filling histograms, and for determining unused particles

		//Step 0
		Int_t locBeamID = dComboBeamWrapper->Get_BeamID();
		Int_t locProtonTrackID = dProtonWrapper->Get_TrackID();

		//Step 1
		Int_t locPiMinusTrackID = dPiMinusWrapper->Get_TrackID();
		Int_t locPiPlusTrackID = dPiPlusWrapper->Get_TrackID();

		/*********************************************** GET FOUR-MOMENTUM **********************************************/

		// Get P4's: //is kinfit if kinfit performed, else is measured
		//dTargetP4 is target p4
		//Step 0
		TLorentzVector locBeamP4 = dComboBeamWrapper->Get_P4();
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();
		TLorentzVector locMissingKLongP4 = dMissingKLongWrapper->Get_P4();
		//Step 1
		TLorentzVector locPiMinusP4 = dPiMinusWrapper->Get_P4();
		TLorentzVector locPiPlusP4 = dPiPlusWrapper->Get_P4();

		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();
		//Step 1
		TLorentzVector locPiMinusP4_Measured = dPiMinusWrapper->Get_P4_Measured();
		TLorentzVector locPiPlusP4_Measured = dPiPlusWrapper->Get_P4_Measured();

		/********************************************* GET COMBO RF TIMING INFO *****************************************/

		TLorentzVector locBeamX4_Measured = dComboBeamWrapper->Get_X4_Measured();
		Double_t locBunchPeriod = dAnalysisUtilities.Get_BeamBunchPeriod(Get_RunNumber());
		Double_t locDeltaT_RF = dAnalysisUtilities.Get_DeltaT_RF(Get_RunNumber(), locBeamX4_Measured, dComboWrapper);
		Int_t locRelBeamBucket = dAnalysisUtilities.Get_RelativeBeamBucket(Get_RunNumber(), locBeamX4_Measured, dComboWrapper); // 0 for in-time events, non-zero integer for out-of-time photons
		Int_t locNumOutOfTimeBunchesInTree = 3; //YOU need to specify this number
		     //Number of out-of-time beam bunches in tree (on a single side, so that total number out-of-time bunches accepted is 2 times this number for left + right bunches) 

		Bool_t locSkipNearestOutOfTimeBunch = true; // True: skip events from nearest out-of-time bunch on either side (recommended).
		Int_t locNumOutOfTimeBunchesToUse = locSkipNearestOutOfTimeBunch ? locNumOutOfTimeBunchesInTree-1:locNumOutOfTimeBunchesInTree; 
		Double_t locAccidentalScalingFactor = dAnalysisUtilities.Get_AccidentalScalingFactor(Get_RunNumber(), locBeamP4.E(), dIsMC); // Ideal value would be 1, but deviations require added factor, which is different for data and MC.
		Double_t locAccidentalScalingFactorError = dAnalysisUtilities.Get_AccidentalScalingFactorError(Get_RunNumber(), locBeamP4.E()); // Ideal value would be 1, but deviations observed, need added factor.
		Double_t locHistAccidWeightFactor = locRelBeamBucket==0 ? 1 : -locAccidentalScalingFactor/(2*locNumOutOfTimeBunchesToUse) ; // Weight by 1 for in-time events, ScalingFactor*(1/NBunches) for out-of-time
		if(locSkipNearestOutOfTimeBunch && abs(locRelBeamBucket)==1) { // Skip nearest out-of-time bunch: tails of in-time distribution also leak in
			dComboWrapper->Set_IsComboCut(true); 
			continue; 
		} 

		/********************************************* COMBINE FOUR-MOMENTUM ********************************************/

		// DO YOUR STUFF HERE

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locProtonP4_Measured + locPiMinusP4_Measured + locPiPlusP4_Measured;

		TLorentzVector loc2PiP4 = locPiPlusP4 + locPiMinusP4;
		TLorentzVector locDecayingKShortP4 = locPiPlusP4 + locPiMinusP4;
		TLorentzVector loc2PiP4_Measured = locPiPlusP4_Measured + locPiMinusP4_Measured;

		TLorentzVector locKSKL_P4 = locMissingKLongP4 + loc2PiP4;
		TLorentzVector locKSKL_Measured_P4 = loc2PiP4_Measured + locMissingP4_Measured;
		TLorentzVector locPrecoil_P4 = locBeamP4_Measured + dTargetP4 - locProtonP4;
		TLorentzVector locPrecoil_P4_Measured = locBeamP4_Measured + dTargetP4 - locProtonP4_Measured;

		TLorentzVector locProtonX4 = dProtonWrapper->Get_X4();

		// do some boosting
		//TLorentzRotation resRestBoost( -locKSKL_P4.BoostVector() );
		TLorentzRotation resRestBoost( -(locBeamP4_Measured + dTargetP4).BoostVector() );
		TLorentzVector pip_res = resRestBoost * locPiPlusP4;
		TLorentzVector pim_res = resRestBoost * locPiMinusP4;
		TLorentzVector ks_res  = resRestBoost * locDecayingKShortP4;
		TLorentzVector kl_res  = resRestBoost * locMissingKLongP4;
		TLorentzVector recoil_res = resRestBoost * locProtonP4;
		TLorentzVector kskl_res= resRestBoost * locKSKL_P4;

		// Get Helicity angles
		TVector3 angles_hel = calc_helicity_angles(locBeamP4, locProtonP4, locDecayingKShortP4, locMissingKLongP4);
		double cos_hel_ks = angles_hel.CosTheta();
		double phi_hel_ks = angles_hel.Phi();

		// Get Helicity angles
		TVector3 angles_gj = calc_GJ_angles(locBeamP4, locProtonP4, locDecayingKShortP4, locMissingKLongP4);
		double cos_gj_ks = angles_gj.CosTheta();
		double phi_gj_ks = angles_gj.Phi();

		// Get van Hove x and y
		double vanHove_x = calc_vanHove_X(ks_res.Pz(), kl_res.Pz(), recoil_res.Pz());
		double vanHove_y = calc_vanHove_Y(ks_res.Pz(), kl_res.Pz(), recoil_res.Pz());

		double t = -(locProtonP4 - dTargetP4).M2();

		double mkskl = locKSKL_P4.M();
		double mmiss = locMissingP4_Measured.M();
		double mpipi = locDecayingKShortP4.M();
		
		// flight significance cuts
		TLorentzVector locKShort_X4;
		locKShort_X4 = dTreeInterface->Get_TObject<TLorentzVector>("DecayingKShort__X4", loc_i);

		TLorentzVector locFromSpacetimeVertex = locKShort_X4;  //Kshort vertex
		TLorentzVector locStepSpacetimeVertex = dComboBeamWrapper->Get_X4();  //Get production vertex
		TLorentzVector locDeltaSpacetime = locStepSpacetimeVertex - locFromSpacetimeVertex;  //vertex difference
		double locPathLength = locDeltaSpacetime.Vect().Mag(); //pathlength is just the magnitude

		float locPathLengthSigma = Get_Fundamental<Float_t>("DecayingKShort__PathLengthSigma", loc_i);  //Get Sigma from analysis tree
		double locPathLengthSignificance = locPathLength/locPathLengthSigma;  //Calculate significance
		double locDetachedLifeTime = locDeltaSpacetime.T();  //Easy to get lifetime!

		double ks_proper_time = -(1./locDecayingKShortP4.Gamma())*locDetachedLifeTime;

		double event_weight = locHistAccidWeightFactor;
		double sideband_w = -1.0;
		float Weight = 0; // This weight is for AmpTools fits

		double chisq = dComboWrapper->Get_ChiSq_KinFit("");
		double chisq_ndf = chisq / dComboWrapper->Get_NDF_KinFit("");

		bool Ks_Criteria = locDecayingKShortP4.M() > 0.48 && locDecayingKShortP4.M() < 0.52;
		bool Ks_Sideband = (locDecayingKShortP4.M() > 0.44 && locDecayingKShortP4.M() < 0.46) || (locDecayingKShortP4.M() > 0.54 && locDecayingKShortP4.M() < 0.56);
		//bool Ks_Sideband = (locDecayingKShortP4.M() > 0.43 && locDecayingKShortP4.M() < 0.46) || (locDecayingKShortP4.M() > 0.54 && locDecayingKShortP4.M() < 0.55);
		//bool Ks_Sideband = (locDecayingKShortP4.M() > 0.44 && locDecayingKShortP4.M() < 0.46) || (locDecayingKShortP4.M() > 0.54 && locDecayingKShortP4.M() < 0.56);

		if(Ks_Criteria && fabs(locDeltaT_RF) < 2)	event_weight = 1;
		else if(Ks_Criteria && fabs(locDeltaT_RF) > 2)	event_weight = locHistAccidWeightFactor;
		else if(Ks_Sideband && fabs(locDeltaT_RF) < 2)	event_weight = sideband_w;
		else if(Ks_Sideband && fabs(locDeltaT_RF) > 2)	event_weight = sideband_w*locHistAccidWeightFactor;

		// The likelihood in AmpTools is calculated as dat - bkg
		// so the background weights need to be corrected by -1
		if(Ks_Criteria && fabs(locDeltaT_RF) < 2)	Weight = 1;
		else if(Ks_Criteria && fabs(locDeltaT_RF) > 2)	Weight = -locHistAccidWeightFactor;
		else if(Ks_Sideband && fabs(locDeltaT_RF) < 2)	Weight = -sideband_w;

		if(dIsMC)	dFlatTreeInterface->Fill_Fundamental<bool>("dIsMC", dIsMC);

		dFlatTreeInterface->Fill_Fundamental<double>("event_weight", event_weight);
		dFlatTreeInterface->Fill_Fundamental<double>("accidental_weight", locHistAccidWeightFactor);

		dFlatTreeInterface->Fill_Fundamental<int>("num_unused_tracks", dComboWrapper->Get_NumUnusedTracks());
		dFlatTreeInterface->Fill_Fundamental<int>("num_unused_showers", dComboWrapper->Get_NumUnusedShowers());
		dFlatTreeInterface->Fill_Fundamental<int>("num_combos", Get_NumCombos());

		dFlatTreeInterface->Fill_Fundamental<double>("mpipi", locDecayingKShortP4.M());
		dFlatTreeInterface->Fill_Fundamental<double>("missing_mass", locMissingP4_Measured.M());
		dFlatTreeInterface->Fill_Fundamental<double>("mkskl", locKSKL_P4.M());

		dFlatTreeInterface->Fill_Fundamental<double>("beam_energy", locBeamP4.E());
		dFlatTreeInterface->Fill_Fundamental<double>("mandel_t", t);
		dFlatTreeInterface->Fill_Fundamental<double>("ks_proper_time", ks_proper_time);

		dFlatTreeInterface->Fill_Fundamental<double>("flight_significance", locPathLengthSignificance);
		dFlatTreeInterface->Fill_Fundamental<double>("path_length", locPathLength);
		dFlatTreeInterface->Fill_Fundamental<double>("chisq_ndf", chisq_ndf);

		dFlatTreeInterface->Fill_Fundamental<double>("cos_hel_ks", cos_hel_ks);
		dFlatTreeInterface->Fill_Fundamental<double>("phi_hel_ks", phi_hel_ks);

		dFlatTreeInterface->Fill_Fundamental<double>("cos_gj_ks", cos_gj_ks);
		dFlatTreeInterface->Fill_Fundamental<double>("phi_gj_ks", phi_gj_ks);

		dFlatTreeInterface->Fill_Fundamental<double>("vanHove_x", vanHove_x);
		dFlatTreeInterface->Fill_Fundamental<double>("vanHove_y", vanHove_y);

//		dFlatTreeInterface->Fill_TObject<TLorentzVector>("pip_p4", locPiPlusP4);
//		dFlatTreeInterface->Fill_TObject<TLorentzVector>("pim_p4", locPiMinusP4);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("ks_p4", locDecayingKShortP4);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("kl_p4", locMissingKLongP4);

//		dFlatTreeInterface->Fill_TObject<TLorentzVector>("pip_p4_meas", locPiPlusP4_Measured);
//		dFlatTreeInterface->Fill_TObject<TLorentzVector>("pim_p4_meas", locPiMinusP4_Measured);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("ks_p4_meas", loc2PiP4_Measured);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("missing_p4_meas", locMissingP4_Measured);

		dFlatTreeInterface->Fill_TObject<TLorentzVector>("pip_p4_cm", pip_res);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("pim_p4_cm", pim_res);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("ks_p4_cm", ks_res);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("kl_p4_cm", kl_res);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("recoil_p4_cm", recoil_res);

		vector<TLorentzVector> locFinalStateP4;
		locFinalStateP4.push_back(locProtonP4);
		locFinalStateP4.push_back(locDecayingKShortP4);
		locFinalStateP4.push_back(locMissingKLongP4);

		if(Ks_Criteria && fabs(locDeltaT_RF) < 2)
			dFlatTreeInterface->Fill_Fundamental<bool>("amptools_dat", true);
		else
			dFlatTreeInterface->Fill_Fundamental<bool>("amptools_dat", false);

		if((Ks_Criteria && fabs(locDeltaT_RF) > 2) || (Ks_Sideband && fabs(locDeltaT_RF) < 2))
		//if((Ks_Criteria && fabs(locDeltaT_RF) > 2) || (Ks_Sideband && fabs(locDeltaT_RF) > 2) || (Ks_Sideband && fabs(locDeltaT_RF) < 2))
			dFlatTreeInterface->Fill_Fundamental<bool>("amptools_bkg", true);
		else
			dFlatTreeInterface->Fill_Fundamental<bool>("amptools_bkg", false);

		if(fabs(locDeltaT_RF) < 2)
			dFlatTreeInterface->Fill_Fundamental<bool>("is_in_time", true);
		else
			dFlatTreeInterface->Fill_Fundamental<bool>("is_in_time", false);

		dFlatTreeInterface->Fill_Fundamental<float>("Weight", Weight);
		dFlatTreeInterface->Fill_Fundamental<int>("pol_angle", locPolarizationAngle);
		dFlatTreeInterface->Fill_Fundamental<double>("Target_Mass", 0.9382720); // Necesary for divideData.pl not for AmpTools itself (I think)
		dFlatTreeInterface->Fill_Fundamental<int>("PID_FinalState", 2212, 0); // proton
		dFlatTreeInterface->Fill_Fundamental<int>("PID_FinalState", 310, 1);  // KShort
		dFlatTreeInterface->Fill_Fundamental<int>("PID_FinalState", 130, 2);  // KLong 
		FillAmpTools_FlatTree(locBeamP4, locFinalStateP4);

//		if(locBeamP4.E() > 8.2 && locBeamP4.E() < 8.8 && locKSKL_P4.M() > 1.10 && locKSKL_P4.M() < 2.10 && chisq_ndf < 4.0 && locPathLengthSignificance > 4.0 && t < 1.5 && dComboWrapper->Get_NumUnusedTracks() < 1 && dComboWrapper->Get_NumUnusedShowers() < 3 && mmiss > 0.3 && mmiss < 0.7 && mpipi > 0.48 && mpipi < 0.52)
//			h1_RFTime->Fill(locDeltaT_RF);

		if(locBeamP4.E() > 8.2 && locBeamP4.E() < 8.8 && chisq_ndf < 2.0 && locPathLengthSignificance > 6.0 && t < 1.0 && dComboWrapper->Get_NumUnusedTracks() < 1 && dComboWrapper->Get_NumUnusedShowers() < 3 && mmiss > 0.3 && mmiss < 0.7 && mpipi > 0.48 && mpipi < 0.52)
			im_kskl->Fill(mkskl, locHistAccidWeightFactor);
		if(locBeamP4.E() > 8.2 && locBeamP4.E() < 8.8 && chisq_ndf < 2.0 && locPathLengthSignificance > 6.0 && t < 1.0 && dComboWrapper->Get_NumUnusedTracks() < 1 && dComboWrapper->Get_NumUnusedShowers() < 3 && mmiss > 0.3 && mmiss < 0.7 && Ks_Sideband)
			im_kskl_sb->Fill(mkskl, locHistAccidWeightFactor);

		if(locSkipNearestOutOfTimeBunch && abs(locRelBeamBucket)==1) { // Skip nearest out-of-time bunch: tails of in-time distribution also leak in
			dComboWrapper->Set_IsComboCut(true); 
			continue; 
		} 

		//FILL FLAT TREE
		if(locBeamP4.E() > 8.2 && locBeamP4.E() < 8.8 && locKSKL_P4.M() > 1.10 && chisq_ndf < 5.1 && locPathLengthSignificance > 2.9 && t < 1.5 && dComboWrapper->Get_NumUnusedTracks() < 3 && dComboWrapper->Get_NumUnusedShowers() < 5)
			Fill_FlatTree();
		else {
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}

		if(Ks_Criteria && fabs(locDeltaT_RF) < 2)	h2_mpipi_rftime->Fill(mpipi, locDeltaT_RF); 
		else if(Ks_Criteria && fabs(locDeltaT_RF) > 2)	h2_mpipi_rftime->Fill(mpipi, locDeltaT_RF); 
		else if(Ks_Sideband && fabs(locDeltaT_RF) < 2)	h2_mpipi_rftime->Fill(mpipi, locDeltaT_RF); 
//		else if(Ks_Sideband && fabs(locDeltaT_RF) > 2)	h2_mpipi_rftime->Fill(mpipi, locDeltaT_RF); 

		if(mmiss > 0.3 && mmiss < 0.7)
			im_pipi->Fill(locDecayingKShortP4.M(), locHistAccidWeightFactor);
		if(Ks_Criteria) {
			im_miss->Fill(locMissingP4_Measured.M(), locHistAccidWeightFactor);
			h1_NumUnusedTracks->Fill(dComboWrapper->Get_NumUnusedTracks(), locHistAccidWeightFactor);
			h1_NumUnusedShowers->Fill(dComboWrapper->Get_NumUnusedShowers(), locHistAccidWeightFactor);

			if(mmiss > 0.3 && mmiss < 0.7 && dComboWrapper->Get_NumUnusedTracks() < 1 && dComboWrapper->Get_NumUnusedShowers() < 3) {
				if(locPathLengthSignificance > 4)
					h1_ChiSqNdf->Fill(chisq_ndf, locHistAccidWeightFactor);
				if(chisq_ndf < 4)
					h1_FS->Fill(locPathLengthSignificance, locHistAccidWeightFactor);

				if(locPathLengthSignificance > 4 && chisq_ndf < 4) {
					h1_KsProperTime->Fill(ks_proper_time, locHistAccidWeightFactor);
					h1_mandelt->Fill(t, locHistAccidWeightFactor);
					im_ksp->Fill((locProtonP4 + locDecayingKShortP4).M(), locHistAccidWeightFactor);
					im_klp->Fill((locProtonP4 + locMissingKLongP4).M(), locHistAccidWeightFactor);
				}
			}
		}

		if(Ks_Sideband) {
			im_miss_sb->Fill(locMissingP4_Measured.M(), locHistAccidWeightFactor);

			if(mmiss > 0.3 && mmiss < 0.7 && dComboWrapper->Get_NumUnusedTracks() < 1 && dComboWrapper->Get_NumUnusedShowers() < 3) {
				if(locPathLengthSignificance > 4)
					h1_ChiSqNdf_sb->Fill(chisq_ndf, locHistAccidWeightFactor);
				if(chisq_ndf < 4)
					h1_FS_sb->Fill(locPathLengthSignificance, locHistAccidWeightFactor);

				if(locPathLengthSignificance > 4 && chisq_ndf < 4) {
					h1_KsProperTime_sb->Fill(ks_proper_time, locHistAccidWeightFactor);
					h1_mandelt_sb->Fill(t, locHistAccidWeightFactor);
					im_ksp_sb->Fill((locProtonP4 + locDecayingKShortP4).M(), locHistAccidWeightFactor);
					im_klp_sb->Fill((locProtonP4 + locMissingKLongP4).M(), locHistAccidWeightFactor);
				}
			}
		}

		if(t > 0.15 && t < 1.5 && mmiss > 0.3 && mmiss < 0.7)
			h3_mpipi_fs_chisq->Fill(mpipi, locPathLengthSignificance, chisq_ndf, locHistAccidWeightFactor);
		if(t > 0.15 && t < 1.5 && mmiss > 0.3 && mmiss < 0.7 && dComboWrapper->Get_NumUnusedTracks() < 1 && dComboWrapper->Get_NumUnusedShowers() < 3)
			h3_mpipi_fs_chisq2->Fill(mpipi, locPathLengthSignificance, chisq_ndf, locHistAccidWeightFactor);

		/******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/

		//// Loop through the analysis actions, executing them in order for the active particle combo
		//dAnalyzeCutActions->Perform_Action(); // Must be executed before Execute_Actions()
		//if(!Execute_Actions()) //if the active combo fails a cut, IsComboCutFlag automatically set
		//	continue;

		//if you manually execute any actions, and it fails a cut, be sure to call:
			//dComboWrapper->Set_IsComboCut(true);

		/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

		/*
		TLorentzVector locMyComboP4(8.0, 7.0, 6.0, 5.0);
		//for arrays below: 2nd argument is value, 3rd is array index
		//NOTE: By filling here, AFTER the cuts above, some indices won't be updated (and will be whatever they were from the last event)
			//So, when you draw the branch, be sure to cut on "IsComboCut" to avoid these.
		dTreeInterface->Fill_Fundamental<Float_t>("my_combo_array", -2*loc_i, loc_i);
		dTreeInterface->Fill_TObject<TLorentzVector>("my_p4_array", locMyComboP4, loc_i);
		*/

		/**************************************** EXAMPLE: HISTOGRAM BEAM ENERGY *****************************************/

		//Histogram beam energy (if haven't already)
		if(locUsedSoFar_BeamEnergy.find(locBeamID) == locUsedSoFar_BeamEnergy.end())
		{
			dHist_BeamEnergy->Fill(locBeamP4.E()); // Fills in-time and out-of-time beam photon combos
			//dHist_BeamEnergy->Fill(locBeamP4.E(),locHistAccidWeightFactor); // Alternate version with accidental subtraction

			locUsedSoFar_BeamEnergy.insert(locBeamID);
		}

		/************************************ EXAMPLE: HISTOGRAM MISSING MASS SQUARED ************************************/

		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();

		//Uniqueness tracking: Build the map of particles used for the missing mass
			//For beam: Don't want to group with final-state photons. Instead use "Unknown" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMass;
		locUsedThisCombo_MissingMass[Unknown].insert(locBeamID); //beam
		locUsedThisCombo_MissingMass[Proton].insert(locProtonTrackID);
		locUsedThisCombo_MissingMass[PiMinus].insert(locPiMinusTrackID);
		locUsedThisCombo_MissingMass[PiPlus].insert(locPiPlusTrackID);

		//compare to what's been used so far
		if(locUsedSoFar_MissingMass.find(locUsedThisCombo_MissingMass) == locUsedSoFar_MissingMass.end())
		{
			//unique missing mass combo: histogram it, and register this combo of particles
			dHist_MissingMassSquared->Fill(locMissingMassSquared); // Fills in-time and out-of-time beam photon combos
			//dHist_MissingMassSquared->Fill(locMissingMassSquared,locHistAccidWeightFactor); // Alternate version with accidental subtraction

			locUsedSoFar_MissingMass.insert(locUsedThisCombo_MissingMass);
		}

		//E.g. Cut
		//if((locMissingMassSquared < -0.04) || (locMissingMassSquared > 0.04))
		//{
		//	dComboWrapper->Set_IsComboCut(true);
		//	continue;
		//}

		/****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/

		// RECOMMENDED: FILL ACCIDENTAL WEIGHT
		// dFlatTreeInterface->Fill_Fundamental<Double_t>("accidweight",locHistAccidWeightFactor);

		/*
		//FILL ANY CUSTOM BRANCHES FIRST!!
		Int_t locMyInt_Flat = 7;
		dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int", locMyInt_Flat);

		TLorentzVector locMyP4_Flat(4.0, 3.0, 2.0, 1.0);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4", locMyP4_Flat);

		for(int loc_j = 0; loc_j < locMyInt_Flat; ++loc_j)
		{
			dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int_array", 3*loc_j, loc_j); //2nd argument = value, 3rd = array index
			TLorentzVector locMyComboP4_Flat(8.0, 7.0, 6.0, 5.0);
			dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4_array", locMyComboP4_Flat, loc_j);
		}
		*/

		//FILL FLAT TREE
		//Fill_FlatTree(); //for the active combo
	} // end of combo loop

	//FILL HISTOGRAMS: Num combos / events surviving actions
	Fill_NumCombosSurvivedHists();

	/******************************************* LOOP OVER THROWN DATA (OPTIONAL) ***************************************/
/*
	//Thrown beam: just use directly
	if(dThrownBeam != NULL)
		double locEnergy = dThrownBeam->Get_P4().E();

	//Loop over throwns
	for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dThrownWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/
	/****************************************** LOOP OVER OTHER ARRAYS (OPTIONAL) ***************************************/
/*
	//Loop over beam particles (note, only those appearing in combos are present)
	for(UInt_t loc_i = 0; loc_i < Get_NumBeam(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dBeamWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over charged track hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumChargedHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dChargedHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over neutral particle hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumNeutralHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dNeutralHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/

	/************************************ EXAMPLE: FILL CLONE OF TTREE HERE WITH CUTS APPLIED ************************************/

	Bool_t locIsEventCut = true;
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i) {
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);
		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut())
			continue;
		locIsEventCut = false; // At least one combo succeeded
		break;
	}
	if(!locIsEventCut && dOutputTreeFileName != "")
		Fill_OutputTree();


	return kTRUE;
}

void DSelector_kskl::Finalize(void)
{
	//Save anything to output here that you do not want to be in the default DSelector output ROOT file.

	//Otherwise, don't do anything else (especially if you are using PROOF).
		//If you are using PROOF, this function is called on each thread,
		//so anything you do will not have the combined information from the various threads.
		//Besides, it is best-practice to do post-processing (e.g. fitting) separately, in case there is a problem.

	//DO YOUR STUFF HERE

	//CALL THIS LAST
	DSelector::Finalize(); //Saves results to the output file
}
