// void function example
 #include <iostream>
 using namespace std;

  void printmessage ()
 {

 // Open the .root data file with MC information 
     std::unique_ptr<TFile> mcFile(TFile::Open(("/scratch/alice_uct/nina/projects/simulations/ev2000_2/o2sim_Kine.root ")));
     if (!mcFile || mcFile->IsZombie()) {
     std::cerr << "Failed to open input Kine file " << ("o2sim_Kine.root");
     return;
     }
  
 // Open the tree o2sim;1
    std::unique_ptr<TTree> mcTree((TTree*)mcFile->FindObjectAny("o2sim"));
    if (!mcTree) {
    std::cerr << "Failed to get o2sim tree";
    return;
    }
 
    // init the reader from the context
    o2::steer::MCKinematicsReader reader("/scratch/alice_uct/nina/projects/simulations/ev2000_2/collisioncontext.root");
 
    // Create the pointers for the relevant branches of the input tree
    std::vector<o2::MCTrack>* mcTracksArray = nullptr;
    mcTree->SetBranchAddress("MCTrack", &mcTracksArray);
    std::vector<o2::TrackReference>* mcTruthArrayPtr = nullptr;
    mcTree->SetBranchAddress("TrackRefs", &mcTruthArrayPtr);
  
    //creates a root file 

    std::unique_ptr<TFile> myFile( TFile::Open("Mphumzi.root", "RECREATE") );
    
    // Create Tree for output file 
       std::unique_ptr<TTree> fTreePt(new TTree("Pt_Tree", ""));

    // Initialise Tree parameters and create tree and tree branches
   
    int First_MotherID;
    int secondMotherID;
    int lastDaughterID;
    int First_DaughterID;
    int pdgCode;
    const char * StringProcess;
    int Process;
    int StatusCode;
    // vector for all electrons and muons 
    vector<int>elecMu;

    // motion related 
   
     Double_t Pt;
     Double_t P;
     Double_t Phi;
     Double_t Eta;
     Double_t Theta;
     Double_t Rapidity;


 vector<int> Daughters;

       fTreePt->Branch("Pt",  &Pt );
       fTreePt->Branch("Phi",  &Phi );
       fTreePt->Branch("Eta", &Eta);
    mcTree->GetEntry(0);
    int nTracks = mcTracksArray->size();
    std::cerr << "We have " << nTracks << " tracks "<<endl;
    std::cerr << "We have " << mcTracksArray->size() << " labels "<<endl;

    std::cerr <<"/*------------- Event Process (anti-charm)-----------------------*/"<<endl;
    std::cout <<left<<setw(10)<<"TrackNo"<<left<<setw(10)<<"PdgCode"<<left<<setw(10)<<"Mother1"<<left<<setw(10)<<"Mother2"<<left<<setw(10)
    <<"Daughter1"<<left<<setw(10)<<"Daughter2"<<left<<setw(10)<<"StatusCode" <<left <<setw(10) <<endl;

    
    for (int itrack = 0; itrack<nTracks; itrack++){
   // Printf("Track number %i",itrack);
   // nTracks = itrack;
    auto& mctruth = *mcTracksArray;
    auto& label = mctruth[itrack];
 
    First_MotherID   = label.getMotherTrackId();
    First_DaughterID = label.getFirstDaughterTrackId();
    lastDaughterID   = label.getLastDaughterTrackId();
    pdgCode          = label.GetPdgCode();
    secondMotherID   = label.getSecondMotherTrackId();
    Process          = label.getProcess();
    StringProcess    = label.getProdProcessAsString();
    StatusCode       = label.getStatusCode();
    Pt               = label.GetPt();
    Phi              = label.GetPhi();
    Eta              = label.GetEta();
   
  

    if(pdgCode ==-4 || pdgCode ==4 ){
    std::cout <<left<<setw(10)<<itrack<<left <<setw(10) <<pdgCode <<left <<setw(10) << First_MotherID <<left <<setw(10) <<secondMotherID 
    <<left <<setw(10)
    <<First_DaughterID <<left <<setw(10) <<lastDaughterID <<left<<setw(10) <<StatusCode  <<endl;
      
       }
   //std::cout<<"vector following"<<endl;
//    vector<int> Daughters;

   if(pdgCode ==4 || pdgCode ==-4){
   if(lastDaughterID >0){
      Daughters.push_back(First_DaughterID);
      Daughters.push_back(lastDaughterID);
         }
       }
      
       
    }
  
   
   sort( Daughters.begin(), Daughters.end() );
   Daughters.erase( unique( Daughters.begin(), Daughters.end() ), Daughters.end() );

   
 //  for (auto i = Daughters.begin(); i != Daughters.end(); ++i){
//       std::cout <<left<<setw(10)<< *i <<left <<setw(10)<< " "<<endl;}


    
      std::cout<<"----------Separation like to a second decay process ------"<<endl;
      // create a vector that stores daughters 
      vector<int> Daughters2;

    for(int itrack:Daughters){
     auto& mctruth = *mcTracksArray;
     auto& label = mctruth[itrack];
   
    First_DaughterID = label.getFirstDaughterTrackId();
    lastDaughterID   = label.getLastDaughterTrackId();
    pdgCode          = label.GetPdgCode();
    First_MotherID   = label.getMotherTrackId();
    secondMotherID   = label.getSecondMotherTrackId();
    Process          = label.getProcess();
    StringProcess    = label.getProdProcessAsString();
    StatusCode       = label.getStatusCode();

   
        std::cout <<left<<setw(10)<<itrack<<left <<setw(10) <<pdgCode <<left <<setw(10) << First_MotherID <<left <<setw(10) <<secondMotherID
	 <<left <<setw(10)
	 <<First_DaughterID <<left <<setw(10) <<lastDaughterID <<left<<setw(10) <<StatusCode  <<endl;
      if(lastDaughterID >-1){
	 Daughters2.push_back(First_DaughterID);
	 Daughters2.push_back(lastDaughterID);
} 
      
            if(pdgCode ==-13 ||pdgCode ==13 ||pdgCode ==-11 ||pdgCode ==11){
             elecMu.push_back(itrack);

		  }
      }    
      sort( Daughters2.begin(), Daughters2.end() );
         Daughters2.erase( unique( Daughters2.begin(), Daughters2.end() ), Daughters2.end() );
        

	
	//define a third vesctor 
	vector<int>Daughters3;
	//iteration of a vector 

	std::cout<<"----------Separation like to a 3rd decay process ------"<<endl;


	for(int itrack:Daughters2){

        auto& mctruth = *mcTracksArray;
        auto& label = mctruth[itrack];

         First_DaughterID = label.getFirstDaughterTrackId();
         lastDaughterID   = label.getLastDaughterTrackId();
         pdgCode          = label.GetPdgCode();
         First_MotherID   = label.getMotherTrackId();
         secondMotherID   = label.getSecondMotherTrackId();
	 Process          = label.getProcess();
	 StringProcess    = label.getProdProcessAsString();
	 StatusCode       = label.getStatusCode();


       std::cout <<left<<setw(10)<<itrack<<left <<setw(10) <<pdgCode <<left <<setw(10) << First_MotherID <<left <<setw(10) <<secondMotherID
                 <<left <<setw(10)
		 <<First_DaughterID <<left <<setw(10) <<lastDaughterID <<left<<setw(10) <<StatusCode  <<endl;
      
               if(lastDaughterID >487){
              Daughters3.push_back(First_DaughterID);
	      Daughters3.push_back(lastDaughterID);
	      }
	       if(pdgCode ==-13 ||pdgCode ==13 ||pdgCode ==-11 ||pdgCode ==11){
                elecMu.push_back(itrack);
                }

	}
	 
	 sort( Daughters3.begin(), Daughters3.end() );
	 Daughters3.erase( unique( Daughters3.begin(), Daughters3.end() ), Daughters3.end() );

	for (auto i = Daughters3.begin(); i != Daughters3.end(); ++i){
	         //std::cout <<left<<setw(10)<< *i <<left <<setw(10)<< " "<<endl;
	}
	// 4th decay step
	// vector of Daughters
	std::cout<<"----------Separation like to a 4th decay process ------"<<endl;
	vector<int>Daughters4;

        for(int itrack:Daughters3){

	   auto& mctruth = *mcTracksArray;
	   auto& label = mctruth[itrack];

	  First_DaughterID = label.getFirstDaughterTrackId();
	  lastDaughterID   = label.getLastDaughterTrackId();
	  pdgCode          = label.GetPdgCode();
          First_MotherID   = label.getMotherTrackId();
          secondMotherID   = label.getSecondMotherTrackId();
	  Process          = label.getProcess();
	  StringProcess    = label.getProdProcessAsString();
	  StatusCode       = label.getStatusCode();

	 std::cout <<left<<setw(10)<<itrack<<left <<setw(10) <<pdgCode <<left <<setw(10) << First_MotherID <<left <<setw(10) <<secondMotherID
	             <<left <<setw(10)
		     <<First_DaughterID <<left <<setw(10) <<lastDaughterID <<left<<setw(10) <<StatusCode  <<endl;

		 if(lastDaughterID >-1){
		   Daughters4.push_back(First_DaughterID);
		   Daughters4.push_back(lastDaughterID);
		  }
        if(pdgCode ==-13 ||pdgCode ==13 ||pdgCode ==-11 ||pdgCode ==11){

          elecMu.push_back(itrack);
          }

	}


  sort( Daughters4.begin(), Daughters4.end() );
  Daughters4.erase( unique( Daughters4.begin(), Daughters4.end() ), Daughters4.end() );
  
  std::cout<<"----------Separation like to a 5th decay process ------"<<endl;
  
  
	
   vector<int> Daughters5 ={3113,3126,805,806,1096,3023,977,995,975,976,969,970,7747,7752,7781,7785,7771,7792,1091,1090,3024,3025,835,836,883,834};

     sort( Daughters5.begin(), Daughters5.end() );
     Daughters5.erase( unique( Daughters5.begin(), Daughters5.end() ), Daughters5.end() );
    
    for(int itrack:Daughters5){

       auto& mctruth = *mcTracksArray;
       auto& label = mctruth[itrack];

      First_DaughterID = label.getFirstDaughterTrackId();
      lastDaughterID   = label.getLastDaughterTrackId();
      pdgCode          = label.GetPdgCode();
      First_MotherID   = label.getMotherTrackId();
      secondMotherID   = label.getSecondMotherTrackId();
      Process          = label.getProcess();
      StringProcess    = label.getProdProcessAsString();
      StatusCode       = label.getStatusCode();

      std::cout <<left<<setw(10)<<itrack<<left <<setw(10) <<pdgCode <<left <<setw(10) << First_MotherID <<left <<setw(10) <<secondMotherID
                <<left <<setw(10)
                <<First_DaughterID <<left <<setw(10) <<lastDaughterID <<left<<setw(10) <<StatusCode  <<endl;
																								              if(pdgCode ==-13 ||pdgCode ==13 ||pdgCode ==-11 ||pdgCode ==11){
      elecMu.push_back(itrack);
     }
   }
   for (auto i = elecMu.begin(); i != elecMu.end(); ++i){
          std::cout <<left<<setw(10)<< *i <<left <<setw(10)<< " "<<endl;
	          }

    for(int itrack:elecMu){

       auto& mctruth = *mcTracksArray;
       auto& label = mctruth[itrack];

    Pt               = label.GetPt();
    Phi              = label.GetPhi();
    Eta              = label.GetEta();

   fTreePt->Fill();

    
 } 
  fTreePt->Write(); 
   
  }
