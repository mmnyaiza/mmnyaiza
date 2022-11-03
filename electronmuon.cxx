// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
///
/// \brief Accessing MC data and the related MC truth.
/// \author Mphumzi Mnyaiza 
/// \since
#include <iostream>
#include <cstdio>
#include <tuple>
#include <vector>
#include <stdlib.h>
#include "cmath"
#include <string>
#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"
#include "CommonConstants/MathConstants.h"
#include "TDatabasePDG.h"
#include "/scratch/alice_uct/mphumzi/repos/alice/O2/Framework/Core/include/Framework/AnalysisDataModel.h"
#include "/scratch/alice_uct/mphumzi/repos/alice/O2Physics/Common/Core/RecoDecay.h"

using namespace std;
using namespace o2;
using namespace o2::framework;
using namespace constants::math;

 // charm and bottom quarks

struct Charm{

 //charm quark histograms
  OutputObj<TH1F> CpdgD{TH1F("CpdgD", "CpdgD", 500, -5000, 5000)};
  OutputObj<TH1F> phiC{TH1F("phiC", "phiC", 500, -TwoPI, 10)};
  OutputObj<TH1F> etaB{TH1F("etaB", "etaB", 500, -10, 10)};
  
  void process(aod::McCollision const& mcCollision, aod::McParticles const& mcParticles){
   for (auto& mcParticle : mcParticles){
     if (mcParticle.has_daughters()) {
       if(abs(mcParticle.pdgCode()) ==4 || (abs(mcParticle.pdgCode()) ==5)){
         if(abs(mcParticle.mothers_first_as<aod::McParticles>().pdgCode()) !=4 || abs(mcParticle.mothers_first_as<aod::McParticles>().pdgCode()) != 5){
	 phiC->Fill(mcParticle.phi());
	 }
	}
	 //daughters 
	 auto Daughters = mcParticle.daughters_as<aod::McParticles>();
	 //number of daughters 
	 int ddN = Daughters.size();
       for(auto& Daughter : Daughters ){
         if(abs(mcParticle.pdgCode()) == 5){

	// LOGF(info, "Daughters number %d",ddN);
          //if(ddN ==63 ||ddN ==48){
	 LOGF(info, "daughter pdgCode:%d", Daughter.pdgCode()); 
	// }
         }
       }

       /* for (auto& d : mcParticle.daughters_as<aod::McParticles>()){
        // if(abs(d.pdgCode()) >4){
	  // LOGF(info,"First daughter%d", mcParticle.daughtersIds);
//	  LOGF(info, "%d,  %d ,  %f ,  %f ,  %f ,  %f ", mcParticle.pdgCode(), d.pdgCode(),mcParticle.e(), d.e(),mcParticle.pt(), d.pt());
         //  LOGF(info, " Mother PdgCode %d,Mother energy %f, Daughter PdgCode %d, Daughter energy %f", mcParticle.pdgCode(),mcParticle.e(), d.pdgCode(), d.e());
          CpdgD->Fill(d.pdgCode());
	  CpdgD->Draw("B");
	//  }
	  }*/
       // }
       }
      }
    }
  // }
  }; 


// Grouping between MC particles and collisions
struct Parents {
  //Histograms electrons
  OutputObj<TH1F> phiH{TH1F("phi", "phi", 500, -TwoPI, 10)};
  OutputObj<TH1F> etaH{TH1F("eta", "eta", 502, -10, 10)};
  OutputObj<TH1F> PDG_e{TH1F("PDG_e", "PDG_e", 2000, -550, 550)};
  //Histograms Muons 
  OutputObj<TH1F> phiH_m{TH1F("phiH_m", "phiH_m", 500, -TwoPI, 10)};
  OutputObj<TH1F> etaH_m{TH1F("eta_m", "etaH_m", 502, -10, 10)};
  OutputObj<TH1F> PDG_m{TH1F("PDG_m", "PDG_m", 500, -500, 500)};
  //OutputObj<TH1F> etaB{TH1F("etaB", "etaB", 500, -10, 10)};
  //Pdg Spectrums
  OutputObj<TH1F> PDG_e1{TH1F("PDG_e1", "PDG_e1", 2000, -5500, 5500)};

  int pdgElec = 11;
  int pdgMu = 13;
  std::vector<aod::McParticle> Elecs;
  std::vector<aod::McParticle> Muons; 
  std::vector<aod::McParticle> Pairs; 
  
  // group according to McCollisions
void process(aod::McCollision const& mcCollision, aod::McParticles const& mcParticles){
//  int countelec = 0;
//  int countmuon = 0;
 for (auto& mcParticle : mcParticles) {
 
   if(mcParticle.isPhysicalPrimary()){
   // final state particles 
     if (mcParticle.getGenStatusCode() > 0) {
     // Checks for mothers 
     if (mcParticle.has_mothers()) {
     // Check first mothers
     auto mothers = mcParticle.mothers_as<aod::McParticles>();
     //check Number of Mothers 
     int MothNo = mothers.size();
     //First Mother 
     auto Mother1 = mcParticle.mothers_first_as<aod::McParticles>();
     //Second Mother 
      auto Mother2a = mcParticle.mothers_first_as<aod::McParticles>();
      //test for electrons 
      //test muons
       if(abs(mcParticle.pdgCode()) ==11){
        PDG_e1->Fill(Mother1.pdgCode());
	  PDG_e1->Draw("B1");
	 // PDG_e->GetXaxis()->SetTickLength(5);
         if(abs(Mother1.pdgCode()) ==4 || abs(Mother1.pdgCode()) ==5){
        LOGF(info,"Electron Mother found");
	phiH->Fill(mcParticle.phi());
	etaH->Fill(mcParticle.eta());
	PDG_e->Fill(Mother1.pdgCode());
	 }
	 else if(abs(Mother1.pdgCode()) == 111 || abs(Mother1.pdgCode())== 411 || abs(Mother1.pdgCode())== 221 || abs(Mother1.pdgCode())== 223 ||
                  abs(Mother1.pdgCode()) == 431 || abs(Mother1.pdgCode()) == 421){
	  auto Num2 = Mother1.mothers_as<aod::McParticles>().size();
	   auto M2 = Mother1.mothers_first_as<aod::McParticles>();
	    auto M2b = Mother1.mothers_last_as<aod::McParticles>();
//	    LOGF(info,"M1 %d,  M2 %d, Mid %d, D%d",M2.pdgCode(), M2b.pdgCode(),Mother1.pdgCode() ,mcParticle.pdgCode());
	  //  LOGF(info, "Second Mother Pdg %d",M2b.pdgCode());
          PDG_e1->Fill(M2.pdgCode());
	  PDG_e1->Draw("B1");
//	  PDG_e->GetXaxis()->SetTickLength(5);
	  if(abs(M2.pdgCode()) ==4 || abs(M2.pdgCode()) == 5){
//	  LOGF(info,"Electron Mother found");
	  phiH->Fill(mcParticle.phi());
	  etaH->Fill(mcParticle.eta());
	  PDG_e->Fill(M2.phi());
	  }
	  else if(abs(M2.pdgCode()) == 221 || abs(M2.pdgCode()) == 313|| abs(M2.pdgCode()) ==213 || abs(M2.pdgCode()) ==431 || abs(M2.pdgCode())==223 || 
	        abs(M2.pdgCode()) == 413 || abs(M2.pdgCode()) ==323){
	  auto m3 = M2.mothers_first_as<aod::McParticles>();
          PDG_e1->Fill(m3.pdgCode());
	  PDG_e1->Draw("B1");
	  // PDG_e->GetXaxis()->SetTickLength(5);
	   if(abs(m3.pdgCode()) ==4 || abs(m3.pdgCode()) == 5){
//	     LOGF(info,"Electron Mother found");
	     phiH->Fill(mcParticle.phi());
	     etaH->Fill(mcParticle.eta());
	    // PDG_e->Fill(m3.phi());
	   }
	   else if(abs(m3.pdgCode()) == 331 || abs(m3.pdgCode()) == 411 || abs(m3.pdgCode()) == 21 || abs(m3.pdgCode()) == 333 ||abs(m3.pdgCode()) ==411|| 
	         abs(m3.pdgCode()) == 445 || abs(m3.pdgCode()) == 443|| abs(m3.pdgCode()) ==431||abs(m3.pdgCode()) ==113||abs(m3.pdgCode()) ==531 ){
	   auto m4 =m3.mothers_first_as<aod::McParticles>();
           PDG_e1->Fill(m4.pdgCode());
	  PDG_e1->Draw("B1");
	 // PDG_e->GetXaxis()->SetTickLength(5);
	  // LOGF(info,"Mother is %d",m4.pdgCode());
	    if(abs(m4.pdgCode()) ==4 || abs(m4.pdgCode()) == 5){
	     LOGF(info,"Electron Mother found");
	     phiH->Fill(mcParticle.phi());
	     etaH->Fill(mcParticle.eta());
	    // PDG_e->Fill(m4.phi()); 
	      }
	      else if(abs(m4.pdgCode()) ==445 || abs(m4.pdgCode())==21){
	      auto m5 =m4.mothers_first_as<aod::McParticles>();
              PDG_e1->Fill(m5.pdgCode());
	      PDG_e1->Draw("B1");
	   //   PDG_e->GetXaxis()->SetTickLength(5);
	      if(abs(m5.pdgCode()) ==4 || abs(m5.pdgCode()) == 5){
	        LOGF(info,"Electron Mother found");
	        phiH->Fill(mcParticle.phi());
		etaH->Fill(mcParticle.eta());
 	      // PDG_e->Fill(m5.phi());
	       }
	      }
	   }
	  }
	 }
       }
       if(abs(mcParticle.pdgCode()) ==13){
        PDG_m->Fill(Mother1.pdgCode());
	if(abs(Mother1.pdgCode()) ==4 || abs(Mother1.pdgCode()) ==5){
	LOGF(info,"Muon Mother found");
	phiH_m->Fill(mcParticle.phi());
	etaH_m->Fill(mcParticle.eta());
	}
        else if(abs(Mother1.pdgCode()) ==421 || abs(Mother1.pdgCode())== 333 || abs(Mother1.pdgCode())== 511 || abs(Mother1.pdgCode())== 411 || 
	      abs(Mother1.pdgCode())== 221 || abs(Mother1.pdgCode())== 521 ||abs(Mother1.pdgCode())== 223 || abs(Mother1.pdgCode())== 113 || abs(Mother1.pdgCode())== 431){
	auto mu1 = Mother1.mothers_first_as<aod::McParticles>();
        PDG_m->Fill(mu1.pdgCode());
	 if(abs(mu1.pdgCode()) ==4 || abs(mu1.pdgCode()) == 5){
//	 LOGF(info,"Muon Mother found");
	 phiH_m->Fill(mcParticle.phi());
	 etaH_m->Fill(mcParticle.eta());
	 }
	  else if(abs(mu1.pdgCode()) == 423 ||abs(mu1.pdgCode()) == 413 || abs(mu1.pdgCode()) == 513 || abs(mu1.pdgCode()) == 523 || abs(mu1.pdgCode()) == 521 || 
	        abs(mu1.pdgCode()) ==331 || abs(mu1.pdgCode()) == 433 || abs(mu1.pdgCode()) == 521 ){
	  auto mu2 = mu1.mothers_first_as<aod::McParticles>();
          PDG_m->Fill(mu2.pdgCode());
	 if(abs(mu2.pdgCode()) ==4 || abs(mu2.pdgCode()) == 5){
//	    LOGF(info,"Muon Mother found");
 	   phiH_m->Fill(mcParticle.phi());
	   etaH_m->Fill(mcParticle.eta());
	   }
	   else if(abs(mu2.pdgCode()) ==21 ||abs(mu2.pdgCode()) ==521|| abs(mu2.pdgCode())== 523|| abs(mu2.pdgCode())==531 ){
	    auto mu3 = mu2.mothers_first_as<aod::McParticles>();
             PDG_m->Fill(mu3.pdgCode());
            LOGF(info,"Mother pdgCode for Muons %d",mu3.pdgCode());
	     if(abs(mu3.pdgCode()) ==4 ||abs(mu3.pdgCode()) == 5){
	      phiH_m->Fill(mcParticle.phi());
	      etaH_m->Fill(mcParticle.eta());
	     }
	   }
	 }
	}
       }
     }
    }
   }
  }
 }
};

// Access from tracks to MC particle
/*
struct PhiDifference {
  
    int pdgElec = 11;
    int pdgMu = 13;
    int pdgPi = 211;
    int pdgPi0 = 111;
  
  //Histograms
  OutputObj<TH1F> phiDiff{TH1F("phiDiff", "phiDiff ", 100, -10, 10)}; //phi difference muons and electrons 
  OutputObj<TH1F> eta{TH1F("Phi", "Phi_{MC} ", 1000, 0, 10)};
  OutputObj<TH1F> Pt{TH1F("Phi", "Phi_{MC} ", 1000, 0, 10)};
  //vectors
  std::vector<aod::McParticle> Elec;
  std::vector<aod::McParticle> Muons;
  std::vector<aod::McParticle> EMpairs;

  // group according to reconstructed Collisions
  void process(soa::Join<aod::Collisions, aod::McCollisionLabels>::iterator const& collision, soa::Join<aod::Tracks, aod::McTrackLabels> const& tracks,
               aod::McParticles const& mcParticles, aod::McCollisions const& mcCollisions)
  {
  int countelec = 0;
  int countmuon = 0;
	  // Elec.clear();
	  // Muons.clear();

     for (auto& mctrack : tracks) {
    // if (!mctrack.has_mcParticle()) {
        // LOGF(warning, "No MC particle for track, skip...");
	// continue;
	// }
	 auto particle = mctrack.mcParticle();
	 //Mothers 
	 auto mother1 = particle.mothers_first_as<aod::McParticles>();
	 auto mother1pdg = mother1.pdgCode();
	 //electrons
            
	  if (particle.isPhysicalPrimary()) {
	   if(particle.getGenStatusCode()>0){
	    if(abs(particle.pdgCode()) == 11 ){
	     countelec++;
//	      LOGF(info,"Electron Mother :%d",mother1pdg);
             if(abs(mother1pdg) == 4){
	      Elec.push_back(particle);
             }
	     else if (abs(mother1pdg) == 431 || abs(mother1pdg) ==  411 ||abs(mother1pdg) ==  221|| abs(mother1pdg) == 111){
	     auto m2 = mother1.mothers_first_as<aod::McParticles>();
	      auto m2pdg = m2.pdgCode();
//	      LOGF(info,"Electron Mother2 :%d",m2pdg);
	      if(abs(m2pdg) == 4){
	         Elec.push_back(particle);
		 }

	     }
	  }
	 //muons
	 
	 if(abs(particle.pdgCode()) == 13 ){
	  countmuon++;
	//  LOGF(info,"muon Mother :%d",mother1pdg);
	   if(abs(mother1pdg) == 4){
	    Muons.push_back(particle);
	   }
	   else if (abs(mother1pdg) ==333 || abs(mother1pdg) == 421 || abs(mother1pdg) == 411 ){
	   auto m2 = mother1.mothers_first_as<aod::McParticles>();
	   auto m2pdg = m2.pdgCode();
	//  LOGF(info,"muon Mother2 :%d",m2pdg);
	   if(abs(m2pdg) == 4){
	   Muons.push_back(particle);
	   }
	   else if(abs(m2pdg) ==413 ||abs(m2pdg) ==423){
	   auto m3 = m2.mothers_first_as<aod::McParticles>();
	    auto m3pdg = m3.pdgCode();
	    if(abs(m3pdg) == 4){
	 //   LOGF(info,"muon Mother3 :%d",m3pdg);
              Muons.push_back(particle);
             }
           }
	  }
	 }
	 //sorting electrons vectors 
	// sort(Elec.begin(),Elec.end());
	 std::sort(Elec.std::begin(), Elec.std::end() );
	 Elec.std::erase(std::unique( Elec.std::begin(), Elec.std::end() ), Elec.std::end());
	 //sorting muons vectors 
//        sort( Muons.begin(), Muons.end() );
//	Muons.erase(unique(Muons.begin(), Muons.end()), Muons.end() );

	   for(auto i = 0;  i  < Elec.size(); i++){
  //          LOGF(info,"electron vector pdg %d",Elec[i].pdgCode());
	     for(auto j= 0; j < Muons.size(); j++){
//	     LOGF(info, "Muon vector pdg%d",Muons[j].pdgCode());
             //correlations happens when we have oposite signs
	     auto esign = Elec[i].pdgCode();
	     auto msign = Muons[j].pdgCode();
              if(esign * msign < 0){
//	      LOGF(info, "Possible correlation");
	      double dphi = Elec[i].phi() - Muons[j].phi(); 
   	      if(dphi<0) {dphi = dphi + TwoPI;}
	      else if(dphi>TwoPI) {dphi = dphi - TwoPI;}
	      phiDiff->Fill(dphi);
	      }
	     }
	    }
	  }  
	 } 
     }
                             
  }
};*/

struct PhysicalPrimaryCharge{   
 //OutputObj<TH1F> chargeH{TH1F("charge_prim", "charge_prim", 100, -5, 5)};
 OutputObj<TH1F> chargeH{TH1F("charge_prim", "charge_prim", 80, -2.5, 2.5)};
 Service<TDatabasePDG> pdgDB;
 int primCount = 0;

 void process(aod::McCollision const& mcCollision, aod::McParticles const& mcParticles){
    for (auto& mcParticle : mcParticles){
                                    
     if (mcParticle.isPhysicalPrimary()){
        primCount++;
        auto Pdgcode = pdgDB->GetParticle(mcParticle.pdgCode());
          if (!Pdgcode) {continue;}
	   if(abs(mcParticle.pdgCode()) ==4){
           chargeH->Fill(Pdgcode->Charge() / 3.); //charge comes in units of 1/3
           }
	  }
         }
       }
    };
    
WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    //adaptAnalysisTask<VertexDistribution>(cfgc),
    adaptAnalysisTask<Charm>(cfgc),
    adaptAnalysisTask<Parents>(cfgc),
//    adaptAnalysisTask<PhiDifference>(cfgc),
   adaptAnalysisTask<PhysicalPrimaryCharge>(cfgc)};
}
