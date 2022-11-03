struct Parents{
 //Charm Quark Daughters pdg Spectrum
  OutputObj<TH1F> PdgSpec{TH1F("PdgSpec","PdgSpec", 1000, -10000, 10000)};
 void process(aod::McCollision const& mcCollision, aod::McParticles const& mcParticles){
  for (auto& mcParticle : mcParticles){
   // Loop over mothers and daughters
    if(mcParticle.isPhysicalPrimary()){
     if(mcParticle.has_mothers()){
     // Check first mother
      auto const& mother = mcParticle.mothers_first_as<aod::McParticles>();
      //auto const& daughter=  mcParticle.daughters_as<aod::McParticles>();
//  auto const& D1 = mcParticle.daughters_first_as<aod::McParticles>();
       if(abs(mother.pdgCode()) ==4){
         // now we have all daugher particles with a mother of charm
        if(abs(mcParticle.pdgCode()) !=4){
          // now we have all daughter particle with a mother of charm and we ourselves are not charm
         PdgSpec->Fill(mcParticle.pdgCode());
         PdgSpec->Draw("B");
         LOGF(info, "First mother: %d has pdg code %d mcParticle %d Daughter %d", mother.globalIndex(), mother.pdgCode(), mcParticle.pdgCode());
        }
       }
     }
    }
  }
 }
};

