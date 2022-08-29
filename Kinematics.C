//Objective of this code is to loop over the o2sime_kine.root and get information about particle kinematics.
 
 void read_o2sim(){

 // Open the O2sim_kine.root as the input file
 
 std::unique_ptr<TFile> mcFile(TFile::Open(("/afs/cern.ch/user/m/mmnyaiza/private/Mphumzi/Sim2_test/o2sim_Kine.root")));
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
 
   // init the reader from the transport kinematics file (assuming here prefix o2sim)
   o2::steer::MCKinematicsReader reader("o2sim", o2::steer::MCKinematicsReader::Mode::kMCKine);
      
   // int nTracks = mcTracksArray ->size();
   //open a root file 
   std::unique_ptr<TFile> myFile( TFile::Open("file.root", "RECREATE") );
   // Create Tree for output file 
  double Phi;
  double eta;
  double theta;
   std::unique_ptr<TTree> Cquarks(new TTree("Cquarks", ""));
   std::unique_ptr<TTree> Bquarks(new TTree("Bquarks", ""));
   std::unique_ptr<TTree> Electrons(new TTree("Electrons", ""));
   std::unique_ptr<TTree> Muons(new TTree("Muons", ""));

 //fill angles for Charm Quarks
   Cquarks->Branch("Phi", &Phi);
   Cquarks->Branch("eta", &eta);
   Cquarks->Branch("theta", &theta);
  
  // angles for Bottom Quarks 
     Bquarks->Branch("Phi", &Phi);
     Bquarks->Branch("eta", &eta);
     Bquarks->Branch("theta", &theta);
 //fill angles for Electrons
    Electrons->Branch("Phi", &Phi);
    Electrons->Branch("eta", &eta);
    Electrons->Branch("theta", &theta);
 
  //fill angles for Muons
      Muons->Branch("Phi", &Phi);
      Muons->Branch("eta", &eta);
      Muons->Branch("theta", &theta);
   
   std::cerr << "We have " << reader.getNEvents(0) << " events "<<endl;

   // loop over all events in the file

   for (int event = 0; event < reader.getNEvents(0); ++event) {

   // get all Monte Carlo tracks for this event
   std::vector<o2::MCTrack> const& tracks = reader.getTracks(event);
  //  vector<int>Muons;
 //   vector<int>Muons2;
//    vector<int>Muons3;
    for(auto track: tracks){
   
     int  pdgCode = track.GetPdgCode();
     int motherNo1= track.getMotherTrackId();
     int MotherNo2 =  track.getSecondMotherTrackId();
     int First_Daughter = track.getFirstDaughterTrackId();
     int Last_Daughter = track.getLastDaughterTrackId();
    // vector<int>Muons;
     Phi = track.GetPhi();
     eta = track.GetEta();
     theta = track.GetTheta();
          
    if(pdgCode ==4 ||pdgCode ==-4 ){
       Cquarks->Fill();
         }
        if(tracks[motherNo1].GetPdgCode() ==5 ||tracks[MotherNo2].GetPdgCode() ==-5 || tracks[motherNo1].GetPdgCode() ==4 ||tracks[MotherNo2].GetPdgCode() ==-4){
          if(tracks[First_Daughter].GetPdgCode() >5 ||tracks[First_Daughter].GetPdgCode() <-4 ){ 
           //fill root files with electrons from charm and bottom quarks 
	   if(tracks[First_Daughter].GetPdgCode() == 11 || tracks[First_Daughter].GetPdgCode() == -11 ||tracks[Last_Daughter].GetPdgCode() == 11 || tracks[Last_Daughter].GetPdgCode() == -11)
	    {
	    Electrons->Fill();
	    }
	    //fill root files with Muons from charm and bottom quarks
	    if(tracks[First_Daughter].GetPdgCode() == 13 || tracks[First_Daughter].GetPdgCode() == -13 ||tracks[Last_Daughter].GetPdgCode() == 13 || tracks[Last_Daughter].GetPdgCode() == -13){
	    Muons->Fill();}

           
	  if (track.GetPdgCode() == 411 || track.GetPdgCode() == -411 || track.GetPdgCode() == 211 || track.GetPdgCode() == -211){
	    //fill root files with electrons from charm and bottom quarks
               if(tracks[First_Daughter].GetPdgCode() == 11 || tracks[First_Daughter].GetPdgCode() == -11 ||tracks[Last_Daughter].GetPdgCode() == 11 || tracks[Last_Daughter].GetPdgCode() == -11){
	     Electrons->Fill();}
            //fill root files with muons from charm and bottom quarks
	        if(tracks[First_Daughter].GetPdgCode() == 13 || tracks[First_Daughter].GetPdgCode() == -13 ||tracks[Last_Daughter].GetPdgCode() == 13 || tracks[Last_Daughter].GetPdgCode() == -13){
	         Muons->Fill();}
	      if (track.GetPdgCode() == 321 || track.GetPdgCode() == 311 || track.GetPdgCode() == 211 || track.GetPdgCode() == 111){
	        
		//fill root files with electrons from charm and bottom quarks
		if(tracks[First_Daughter].GetPdgCode() == 11 || tracks[First_Daughter].GetPdgCode() == -11 ||tracks[Last_Daughter].GetPdgCode() == 11 || tracks[Last_Daughter].GetPdgCode() == -11){
		Electrons->Fill();
		}

		 //fill root files with muons from charm and bottom quarks
	       if(tracks[First_Daughter].GetPdgCode() == 13 || tracks[First_Daughter].GetPdgCode() == -13 ||tracks[Last_Daughter].GetPdgCode() == 13 || tracks[Last_Daughter].GetPdgCode() == -13){
	        Muons->Fill();}
		   
               }
             }	
	  }
        
        Bquarks->Fill();
         }
       }

    }
    Cquarks->Write();
    Bquarks->Write();
    Electrons->Write();
    Muons->Write();
    
  }   
    
    void Kinematics(){
    
    read_o2sim();
     }


