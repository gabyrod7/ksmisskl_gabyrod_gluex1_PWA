#include "DSelector_thrown.h"
//
void DSelector_thrown::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = ""; //"" for none
	//USERS: SET OUTPUT TREE FILES/NAMES //e.g. binning into separate files for AmpTools
	//dOutputTreeFileNameMap["Bin1"] = "mcgen_bin1.root"; //key is user-defined, value is output file name
	//dOutputTreeFileNameMap["Bin2"] = "mcgen_bin2.root"; //key is user-defined, value is output file name
	//dOutputTreeFileNameMap["Bin3"] = "mcgen_bin3.root"; //key is user-defined, value is output file name

	// AmpTools step 1
	dFlatTreeFileName = "ftree.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = "kskl"; //if blank, default name will be chosen

	//Because this function gets called for each TTree in the TChain, we must be careful:
		//We need to re-initialize the tree interface & branch wrappers, but don't want to recreate histograms
	bool locInitializedPriorFlag = dInitializedFlag; //save whether have been initialized previously
	DSelector::Init(locTree); //This must be called to initialize wrappers for each new TTree
	//gDirectory now points to the output file with name dOutputFileName (if any)
	if(locInitializedPriorFlag)
		return; //have already created histograms, etc. below: exit

	dPreviousRunNumber = 0;

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	im_kskl = new TH1F("im_kskl", ";M(K_{S}K_{L});Counts", 100, 1.00, 2.00);

	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want

	// AmpTools step 2
	SetupAmpTools_FlatTree(); // sets most of the branches necesary for AmpTools PWA

	dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("Target_Mass"); // necesary for divideData.pl (I think)
	dFlatTreeInterface->Create_Branch_FundamentalArray<Int_t>("PID_FinalState","NumFinalState");
	// convenient extra variables
	dFlatTreeInterface->Create_Branch_Fundamental<int>("pol_angle"); // I use this to split the trees by polarization angle

	dFlatTreeInterface->Create_Branch_Fundamental<double>("mkskl");
	dFlatTreeInterface->Create_Branch_Fundamental<double>("mandel_t");
	dFlatTreeInterface->Create_Branch_Fundamental<double>("ks_proper_time");

	dFlatTreeInterface->Create_Branch_Fundamental<bool>("amptools_dat");
}

Bool_t DSelector_thrown::Process(Long64_t locEntry)
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

	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE

	/******************************************* LOOP OVER THROWN DATA ***************************************/

	//Thrown beam: just use directly
	double locBeamEnergyUsedForBinning = 0.0;
	if(dThrownBeam != NULL)
		locBeamEnergyUsedForBinning = dThrownBeam->Get_P4().E();

	int count_proton = 0, count_ks = 0, count_kl = 0, count_pip = 0, count_pim = 0;

	TLorentzVector	locProtonP4_Thrown,
			locProtonX4_Thrown,
			locDecayingKShortP4_Thrown,
			locDecayingKShortX4_Thrown,
			locMissingKLongP4_Thrown,
			locPiPlus1P4_Thrown,
			locPiPlus1X4_Thrown,
			locPiMinus1P4_Thrown,
			locPiMinus1X4_Thrown,
			locBeamP4;

	TLorentzVector locTargetP4(0, 0, 0, 0.9382720);

	//Loop over throwns
	for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dThrownWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
		Particle_t locPID = dThrownWrapper->Get_PID();
		TLorentzVector locThrownP4 = dThrownWrapper->Get_P4();
		TLorentzVector locThrownX4 = dThrownWrapper->Get_X4();
		cout << "Thrown " << loc_i << ": " << locPID << ", " << locThrownP4.Px() << ", " << locThrownP4.Py() << ", " << locThrownP4.Pz() << ", " << locThrownP4.E() << endl;

		if(locPID == 1) {
			locBeamP4 = locThrownP4;
		}
		if(locPID == 14) {
			locProtonP4_Thrown = locThrownP4;
			locProtonX4_Thrown = locThrownX4;
			count_proton++;
		}
		else if(locPID == 16) {
			locDecayingKShortP4_Thrown = locThrownP4;
			locDecayingKShortX4_Thrown = locThrownX4;
			count_ks++;
		}
		else if(locPID == 10) {
			locMissingKLongP4_Thrown = locThrownP4;
			count_kl++;
		}
		else if(locPID == 8) {
			locPiPlus1P4_Thrown = locThrownP4;
			locPiPlus1X4_Thrown = locThrownX4;
			count_pip++;
		}
		else if(locPID == 9) {
			locPiMinus1P4_Thrown = locThrownP4;
			locPiMinus1X4_Thrown = locThrownX4;
			count_pim++;
		}
	}
	cout << endl;

	double t = -(locProtonP4_Thrown - dTargetP4).M2();
	double ks_proper_time = -(1./locDecayingKShortP4_Thrown.Gamma())*locDecayingKShortX4_Thrown.T();

	im_kskl->Fill((locDecayingKShortP4_Thrown + locMissingKLongP4_Thrown).M());
	dFlatTreeInterface->Fill_Fundamental<double>("mkskl", (locDecayingKShortP4_Thrown + locMissingKLongP4_Thrown).M());

	dFlatTreeInterface->Fill_Fundamental<double>("mandel_t", t);
	dFlatTreeInterface->Fill_Fundamental<double>("ks_proper_time", ks_proper_time);

	// AmpTools step 3
	// set ordering of pions for amplitude analysis
	vector<TLorentzVector> locFinalStateP4;
	locFinalStateP4.push_back(locProtonP4_Thrown);
	locFinalStateP4.push_back(locDecayingKShortP4_Thrown);
	locFinalStateP4.push_back(locMissingKLongP4_Thrown);

	dFlatTreeInterface->Fill_Fundamental<float>("Weight", 1.0 );

	dFlatTreeInterface->Fill_Fundamental<int>("pol_angle", locPolarizationAngle);
	dFlatTreeInterface->Fill_Fundamental<float>("Target_Mass", 0.9382720); // Necesary for divideData.pl not for AmpTools itself (I think)
	dFlatTreeInterface->Fill_Fundamental<int>("PID_FinalState", 2212, 0); // proton
	dFlatTreeInterface->Fill_Fundamental<int>("PID_FinalState", 310, 1);  // KShort
	dFlatTreeInterface->Fill_Fundamental<int>("PID_FinalState", 130, 2);  // KLong
	FillAmpTools_FlatTree(dThrownBeam->Get_P4(), locFinalStateP4);
	//FillAmpTools_FlatTree(locBeamP4, locFinalStateP4);

	dFlatTreeInterface->Fill_Fundamental<bool>("amptools_dat", true);

	//if(count_proton == 1 && count_ks == 1 && count_kl == 1)
	if(count_proton == 1 && count_ks == 1 && count_kl == 1 && count_pip == 1 && count_pim == 1)
		Fill_FlatTree();

	//OR Manually:
	//BEWARE: Do not expect the particles to be at the same array indices from one event to the next!!!!
	//Why? Because while your channel may be the same, the pions/kaons/etc. will decay differently each event.

	//BRANCHES: https://halldweb.jlab.org/wiki/index.php/Analysis_TTreeFormat#TTree_Format:_Simulated_Data
	TClonesArray** locP4Array = dTreeInterface->Get_PointerToPointerTo_TClonesArray("Thrown__P4");
	TBranch* locPIDBranch = dTreeInterface->Get_Branch("Thrown__PID");
/*
	Particle_t locThrown1PID = PDGtoPType(((Int_t*)locPIDBranch->GetAddress())[0]);
	TLorentzVector locThrown1P4 = *((TLorentzVector*)(*locP4Array)->At(0));
	cout << "Particle 1: " << locThrown1PID << ", " << locThrown1P4.Px() << ", " << locThrown1P4.Py() << ", " << locThrown1P4.Pz() << ", " << locThrown1P4.E() << endl;
	Particle_t locThrown2PID = PDGtoPType(((Int_t*)locPIDBranch->GetAddress())[1]);
	TLorentzVector locThrown2P4 = *((TLorentzVector*)(*locP4Array)->At(1));
	cout << "Particle 2: " << locThrown2PID << ", " << locThrown2P4.Px() << ", " << locThrown2P4.Py() << ", " << locThrown2P4.Pz() << ", " << locThrown2P4.E() << endl;
*/


	/******************************************* BIN THROWN DATA INTO SEPARATE TREES FOR AMPTOOLS ***************************************/

/*
	//THESE KEYS MUST BE DEFINED IN THE INIT SECTION (along with the output file names)
	if((locBeamEnergyUsedForBinning >= 8.0) && (locBeamEnergyUsedForBinning < 9.0))
		Fill_OutputTree("Bin1"); //your user-defined key
	else if((locBeamEnergyUsedForBinning >= 9.0) && (locBeamEnergyUsedForBinning < 10.0))
		Fill_OutputTree("Bin2"); //your user-defined key
	else if((locBeamEnergyUsedForBinning >= 10.0) && (locBeamEnergyUsedForBinning < 11.0))
		Fill_OutputTree("Bin3"); //your user-defined key
*/

	return kTRUE;
}

void DSelector_thrown::Finalize(void)
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
