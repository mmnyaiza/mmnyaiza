
  int pdgElec = 11;
  int pdgMu = 13;


 void process(aod::McCollision const& mcCollision, aod::McParticles const& mcParticles)
  {
    for (auto& mcParticle : mcParticles) {
     if(mcParticle.isPhysicalPrimary()){
     // final state particles 
      if (mcParticle.getGenStatusCode() > 0) {
        // check for mothers 
         if (mcParticle.has_mothers()) {
          if(abs(mcParticle.pdgCode()) ==pdgElec){
          // Check mothers 
          auto const& mothers = mcParticle.mothers_as<aod::McParticles>();
          auto NumId = mothers.size();
          LOGF(info,"Number of parents: %d", NumId);
          
          auto const& mother1 = mcParticle.mothers_first_as<aod::McParticles>();
          int m1PDG = (*mother1).pdgCode();
          
             if(NumId==2){
             auto const& mother2 = mcParticle.mothers_last_as<aod::McParticles>();
             m2PDG = (*mother2).pdgCode();
              }
             }
           }
         }
       }      
     }
    }      
