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
/// \author
/// \since

#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"
#include "CommonConstants/MathConstants.h"
#include "TDatabasePDG.h"
#include "DetectorsVertexing/DCAFitterN.h"
#include "PWGHF/DataModel/HFSecondaryVertex.h"
#include "Common/Core/trackUtilities.h"
#include "ReconstructionDataFormats/DCA.h"
using namespace o2;
using namespace o2::framework;
using namespace constants::math;
using namespace o2::aod;
using namespace o2::framework::expressions;
using namespace o2;
using namespace o2::framework;
using namespace o2::framework::expressions;
using namespace o2::aod::hf_cand;
using namespace o2::aod::hf_cand_prong2;
 


// Simple analysis of PhysicalPrimary particles
 struct PhysicalPrimaryCharge {
 OutputObj<TH1F> charge{TH1F("charge_prim", "charge_prim", 100, -5, 5)};
 Service<TDatabasePDG> pdgDB;

 void process(aod::McParticles const& mcParticles)
	{
        for (auto& particle : mcParticles) {
        if (!particle.isPhysicalPrimary()) {
        continue;
        }
        auto pdgParticle =pdgDB->GetParticle( particle.pdgCode());
        if (!pdgParticle) {
        continue;
     } 
  charge->Fill(pdgParticle->Charge() / 3.); // note that charge comes in units of 1/3
     }
  }
 };

// group according to McCollisions
  // void process(aod::McCollision const& mcCollision, aod::McParticles const& mcParticles)
   //  {
 // access MC truth information with mcCollision() and mcParticle() methods
// LOGF(info, "MC. vtx-z = %f", mcCollision.posZ());
// LOGF(info, "First: %d | Length: %d", mcParticles.begin().index(), mcParticles.size());
// int count = 0;
// for (auto& mcParticle : mcParticles) {
// if (mcParticle.isPhysicalPrimary()) {
 //phiH->Fill(mcParticle.phi());
 //etaH->Fill(mcParticle.eta());
// count++;
 // Loop over mothers and daughters
 //if (mcParticle.has_mothers()) {
//Check first mother
// auto const& mother = mcParticle.mothers_first_as<aod::McParticles>();
// LOGF(info, "First mother: %d has pdg code %d", mother.globalIndex(), mother.pdgCode());
 // Loop over all mothers (needed for some MCs with junctions etc.)
// for (auto& m : mcParticle.mothers_as<aod::McParticles>()) {
  //LOGF(debug, "M2 %d %d", mcParticle.globalIndex(), m.globalIndex());
  // }
// }
// if (mcParticle.has_daughters()) {
// for (auto& d : mcParticle.daughters_as<aod::McParticles>()) {
//  LOGF(debug, "D2 %d %d", mcParticle.globalIndex(), d.globalIndex());
//       }
 //    }
  // }
    //   }
 // LOGF(info, "Primaries for this collision: %d", count);
  //  }
// };

// Loop over MCColisions and get corresponding collisions (there can be more than one)
// For each of them get the corresponding tracks
// Note the use of "SmallGroups" template, that allows to handle both Run 2, where
// we have exactly 1-to-1 correspondence between collisions and mc collisions, and
// Run 3, where we can have 0, 1, or more collisions for a given mc collision


struct LoopOverMcMatched {
  OutputObj<TH1F> PhiDiff{TH1F("Phi", "Phi_{MC} ", 1000, 0, 10)};
  OutputObj<TH1F> TrackPdg{TH1F("pdg","pdg",10000,-1000,1000)};
  OutputObj<TH1F>EtaDiff{TH1F("Eta","Eta_{MC} ",1000,-3,3)};
  OutputObj<TH1F>ptspec{TH1F("pt_s","pt_{MC}",1000,0,20)};
  OutputObj<TH1F> charge{TH1F("charge_prim", "charge_prim", 100, -5, 5)};
//OutputObj<TH1F> PhiDiff{TH1F("Phi", ";Phi_{MC} - Phi_{Rec}", 1000, -10, 10)};
 //   OutputObj<TH1F> TrackPdg{TH1F("pdg","pdg",10000,-20,20)};
//   OutputObj<TH1F>EtaDiff{TH1F("Eta","Eta_{MC} - Eta_{Rec}",1000,-3,3)};


  void process(aod::McCollision const& mcCollision, soa::SmallGroups<soa::Join<aod::McCollisionLabels, aod::Collisions>> const& collisions,
               soa::Join<aod::Tracks, aod::McTrackLabels> const& tracks, aod::McParticles const& mcParticles)
  {
    // access MC truth information with mcCollision() and mcParticle() methods
    LOGF(info, "MC collision at vtx-z = %f with %d mc particles and %d reconstructed collisions", mcCollision.posZ(), mcParticles.size(), collisions.size());
    for (auto& collision : collisions) {
      LOGF(info, "  Reconstructed collision at vtx-z = %f", collision.posZ());

      // NOTE this will be replaced by a improved grouping in the future
      
      auto groupedTracks = tracks.sliceBy(aod::track::collisionId, collision.globalIndex());
      LOGF(info, "  which has %d tracks", groupedTracks.size());
      for (auto& track : groupedTracks) {
        if (!track.has_mcParticle()) {
          LOGF(warning, "No MC particle for track, skip...");
          continue;

        }
      auto trackpdg = track.mcParticle().pdgCode();
      TrackPdg->Fill(trackpdg);
      if(trackpdg==13 || trackpdg==-13 ||trackpdg==11 || trackpdg==-11){
      EtaDiff->Fill(track.mcParticle().eta());
      PhiDiff->Fill(track.mcParticle().phi());
      ptspec->Fill(track.mcParticle().pt());
    }
      }
    }
  }
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    adaptAnalysisTask<PhysicalPrimaryCharge>(cfgc),
    adaptAnalysisTask<LoopOverMcMatched>(cfgc)};
}
