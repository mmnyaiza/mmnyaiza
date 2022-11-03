 void plot_root(){
	std::unique_ptr<TFile> myfile( TFile::Open("file.root") );
	if (!myfile ||myfile->IsZombie()) {
   	std::cerr << "Error opening file" << endl;
   	exit(-1);
	}

	// Open the tree o2sim;1
	 std::unique_ptr<TTree> myfileTree((TTree*)myfile->FindObjectAny("Muonsc"));
	 if (!myfileTree) {
	 std::cerr << "Failed to get Muons";
	 return;
	 }
	// Create the pointers for the relevant branches of the input tree
	 myfileTree->SetBranchAddress("Muons", &Phi_muons);


//	auto tree = file->Get<TTree>("Muons");

	//Muons->SetBranchAddress("Phi_muons",&Phi_muons);

	for (int iEntry = 0; myfileTree->LoadTree(iEntry) >= 0; ++iEntry) {
 	// Load the data for the given tree entry
   	myfileTree->GetEntry(iEntry);

 	// Now, `variable` is set to the value of the branch
   	// "branchName" in tree entry `iEntry`
  	 //printf("%d\n", Phi_muons);
	}
 }
