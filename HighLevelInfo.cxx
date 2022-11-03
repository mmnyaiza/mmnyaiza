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


#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"
#include "CommonConstants/MathConstants.h"
#include "TDatabasePDG.h"

using namespace o2;
using namespace o2::framework;
using namespace constants::math;

//From old code
#include <iostream>
#include <cstdio>
#include <tuple>
#include <vector>
#include <stdlib.h>
#include "cmath"
#include <string>

struct LogProperties
{
    //Defining histograms to store properties
    OutputObj<TH1F> ptH{TH1F("pt", "pt", 100, 0., 10)};
    OutputObj<TH1F> phiH{TH1F("phi", "phi", 100, 0., TwoPI)};
    OutputObj<TH1F> etaH{TH1F("eta", "eta", 102, -2.01, 2.01)};

    void process(aod::McCollision const& mcCollision, aod::McParticles const& mcParticles)
    {
        //LOGF(info, "Number of Particles: %d", mcParticles.size());

        //Loop over particles in collision
        for (auto& part : mcParticles) {
            if(part.getGenStatusCode() > 0) //only record final state particles
            {   
                ptH->Fill(part.pt());
                phiH->Fill(part.phi());
                etaH->Fill(part.eta());
            }
        }   
    }
};

struct CountSpecies
{
    //PDG Codes
    Int_t pdgE = 11;
    Int_t pdgMu = 13;
    Int_t pdgPi = 211;
    Int_t pdgPi0 = 111;

    int countE_tot = 0;
    int countMu_tot = 0;
    int countPi_tot = 0;
    int countColl = 0;
    int corrCount = 0;

    int twoPi = 2.*M_PI;

    //Specific Histograms
    OutputObj<TH1F> ptE_H{TH1F("ptE", "ptE", 100, 0., 10)};
    OutputObj<TH1F> phiE_H{TH1F("phiE", "phiE", 100, 0., twoPi)};
    OutputObj<TH1F> etaE_H{TH1F("etaE", "etaE", 100, -2.01, 2.01)};

    OutputObj<TH1F> ptMu_H{TH1F("ptMu", "ptMu", 100, 0., 10)};
    OutputObj<TH1F> phiMu_H{TH1F("phiMu", "phiMu", 100, 0., twoPi)};
    OutputObj<TH1F> etaMu_H{TH1F("etaMu", "etaMu", 100, -2.01, 2.01)};

    OutputObj<TH1F> ptPi_H{TH1F("ptPi", "ptPi", 100, 0., 10)};
    OutputObj<TH1F> phiPi_H{TH1F("phiPi", "phiPi", 100, 0., twoPi)};
    OutputObj<TH1F> etaPi_H{TH1F("etaPi", "etaPi", 100, -2.01, 2.01)};

    OutputObj<TH1F> dPhi_H{TH1F("dPhi", "dPhi", 60, 0, twoPi)};

    //Storage Vectors
    std::vector<aod::McParticle> elecs;
    std::vector<aod::McParticle> muons;
    std::vector<aod::McParticle> pairs;
    std::vector<int> otherPDG;
    

    
    void process(aod::McCollision const& mcCollision, soa::SmallGroups<soa::Join<aod::McCollisionLabels, aod::Collisions>> const& collisions,
               soa::Join<aod::Tracks, aod::McTrackLabels> const& tracks, aod::McParticles const& mcParticles)
               
    {
        int countE = 0;
        int countMu = 0;
        int countPi = 0;
        countColl++;
        //LOGF(info, "Collision %d", countColl);

        //for (auto& collision : collisions)
        //{
            //auto groupedTracks = tracks.sliceBy(aod::track::collisionId, collision.globalIndex());

            
            for (auto& part : mcParticles)
            {
                    if(part.getGenStatusCode() > 0)
                    {
                        if(abs(part.pdgCode()) == pdgE) 
                        {
                            countE++;
                            countE_tot++;

                            ptE_H->Fill(part.pt());
                            phiE_H->Fill(part.phi());
                            etaE_H->Fill(part.eta());

                            elecs.push_back(part);


                            //Attempt 1, testing the get methods
                            //auto& allM = part.mothersIds();
                            //LOGF(info, part.fIndexArray_Mothers());


                            
                        } 
                        else if(abs(part.pdgCode()) == pdgMu) 
                        {
                            countMu++;
                            countMu_tot++;

                            ptMu_H->Fill(part.pt());
                            phiMu_H->Fill(part.phi());
                            etaMu_H->Fill(part.eta());

                            muons.push_back(part);
                        } 

                        else if(abs(part.pdgCode()) == pdgPi or abs(part.pdgCode()) == pdgPi0) 
                        {
                            countPi++;
                            countPi_tot++;

                            ptPi_H->Fill(part.pt());
                            phiPi_H->Fill(part.phi());
                            etaPi_H->Fill(part.eta());
                        }

                        else
                        {
                            int code = part.pdgCode();
                            otherPDG.push_back(code);
                            //LOGF(info, "PDG Code: %d", code);
                        }

                    }
                //}
            //}
            }
            /*if (countE > 0 and countMu >0)
            {
                LOGF(info,"possible correlations");
                int count = 0;
                for (int i=0; i<countMu; i++)
                {
                    Int_t mSign = muons[i].pdgCode();

                    for (int j=0; j<countE; j++)
                    {
                        Int_t eSign = elecs[j].pdgCode();
                        LOGF(info,"Electron: %d",j);
                        LOGF(info,"PDG: %d",eSign);
                        LOGF(info,"Muon: %d",i);
                        LOGF(info,"PDG: %d",mSign);

                        if(mSign * eSign < 0) //correlation occurs when opposite sign pair is found
                        {
                            //LOGF(info,"correlation found");
                            //LOGF(info,"");
                            corrCount++;
                            count++;

                            pairs.push_back(elecs[j]);
                            pairs.push_back(muons[i]);

                            //LOGF(info,"phi: %d",(elecs[j]).phi());
                            //LOGF(info,"testing");

                            double dPhi = elecs[j].phi() - muons[i].phi();
                            //LOGF(info, dPhi);

                            if(dPhi<0) {dPhi = dPhi + 2.*M_PI;}
                            else if(dPhi>2*M_PI) {dPhi = dPhi - 2.*M_PI;}

                            dPhi_H->Fill(dPhi);
                            //dPhi_H->Fill(elecs[j].phi() - muons[i].phi());
                            //LOGF(info,"Shifted dPhi: %d",dPhi);
                            //LOGF(info,"fin");

                        }
                    }
                }
            }*/

            //LOGF(info, "Number of Electrons: %d", countE);
            //LOGF(info, "Number of Muons: %d", countMu);
            //LOGF(info, "Number of Pions: %d", countPi);

            //LOGF(info, "Total Number of Electrons: %d", countE_tot);
            //LOGF(info, "Total Number of Muons: %d", countMu_tot);
            //LOGF(info, "Total Number of Pions: %d", countPi_tot);

            //LOGF(info, "Counted Correlations: %d", corrCount);
        //}
        
    }

    void finalCount(){LOGF(info, "Counted Correlations: %d", "corrCount");}
};

struct Parents
{
    //PDG Codes
    Int_t pdgE = 11;
    Int_t pdgMu = 13;
    Int_t countColl = 0;
    int countCP = 0;

    void process(aod::McCollision const& mcCollision, aod::McParticles const& mcParticles)
    {
        countColl++;
        
        for (auto& part : mcParticles)
        {
            if(part.getGenStatusCode() > 0)
            {
                if(abs(part.pdgCode()) == pdgE)
                {
                    auto* mothers = part.mothers_as<aod::McParticles>();
                    int* numM = mothers->size();
                    LOGF(info,"Number of parents: %d", numM);
                    
                    auto* mother = part.mothers_first_as<aod::McParticles>();
                    int mPDG = mother->pdgCode();
                    
                    int m2PDG = -1; //initializing variable

                    if(numM==2)
                    {
                        auto* mother2 = part.mothers_last_as<aod::McParticles>();
                        m2PDG = mother2->pdgCode();
                    }           

                    int count = 0; //safety net for infinite loops considering pointer error      
                    
                    while(mPDG!=2212 and count<10)
                    {
                        count++;

                        if(numM==2) 
                            if(abs(m2PDG)==4)
                            {
                                LOGF(info,"Quark Parent Found! (m2)");
                                break;
                            }
                        }


                        if(abs(mPDG)==4)
                        {
                            LOGF(info,"Quark Parent Found! (m1)");
                            break;
                        }
                        
                        LOGF(info,"Nothing found, continuing search");
                        mothers = mother->mothers_as<aod::McParticles>();
                        numM = mothers->size();
                        LOGF(info,"Number of parents: %d", numM);
                        
                        if(numM==2)
                        {
                            mother2 = mother->mothers_last_as<aod::McParticles>();
                            m2PDG = mother2->pdgCode();
                        }      

                        mother = mother->mothers_first_as<aod::McParticles>();
                        mPDG = mother->pdgCode();
                }
            }
        }    
    }

};

struct dPhiMC
{
    Int_t pdgE = 11;
    Int_t pdgMu = 13;
    Int_t pdgPi = 211;
    Int_t pdgPi0 = 111;
    int countColl = 0;
    int corrCount = 0;

    int twoPi = 2.*M_PI;

    //Specific Histograms
    OutputObj<TH1F> ptE_mcCorr{TH1F("ptE_mcCorr", "ptE_mcCorr", 100, 0., 10)};
    OutputObj<TH1F> phiE_mcCorr{TH1F("phiE_mcCorr", "phiE_mcCorr", 100, 0., twoPi)};
    OutputObj<TH1F> etaE_mcCorr{TH1F("etaE_mcCorr", "etaE_mcCorr", 100, -2.01, 2.01)};

    OutputObj<TH1F> ptMu_mcCorr{TH1F("ptMu_mcCorr", "ptMu_mcCorr", 100, 0., 10)};
    OutputObj<TH1F> phiMu_mcCorr{TH1F("phiMu_mcCorr", "phiMu_mcCorr", 100, 0., twoPi)};
    OutputObj<TH1F> etaMu_mcCorr{TH1F("etaMu_mcCorr", "etaMu_mcCorr", 100, -2.01, 2.01)};

    OutputObj<TH1F> dPhi_mc{TH1F("dPhi_mc", "dPhi_mc", 60, 0, twoPi)};

    std::vector<aod::McParticle> elecs;
    std::vector<aod::McParticle> muons;
    std::vector<aod::McParticle> pairs;

    void process(aod::McCollision const& mcCollision, soa::SmallGroups<soa::Join<aod::McCollisionLabels, aod::Collisions>> const& collisions,
               soa::Join<aod::Tracks, aod::McTrackLabels> const& tracks, aod::McParticles const& mcParticles)
               
    {
        int countE = 0;
        int countMu = 0;
        //int countPi = 0;
        countColl++;
        //LOGF(info, "Collision %d", countColl);

        for (auto& part : mcParticles)
        {
            if(part.getGenStatusCode() > 0)
            {
                if(abs(part.pdgCode()) == pdgE) 
                {
                    countE++;
                    elecs.push_back(part);
                    
                } 
                else if(abs(part.pdgCode()) == pdgMu) 
                {
                    countMu++;
                    muons.push_back(part);
                } 

                //will only be relevant once background is added
                //else if(abs(part.pdgCode()) == pdgPi or abs(part.pdgCode()) == pdgPi0) 
                //{
                //    countPi++;
                //}

            }
        }

        if (countE > 0 and countMu >0)
        {
            LOGF(info, "Collision %d", countColl);
            LOGF(info,"possible correlations");
            int count = 0;
            for (int i=0; i<countMu; i++)
            {
                Int_t mSign = muons[i].pdgCode();

                for (int j=0; j<countE; j++)
                {
                    Int_t eSign = elecs[j].pdgCode();
                    LOGF(info,"Electron: %d",j);
                    LOGF(info,"PDG: %d",eSign);
                    LOGF(info,"Muon: %d",i);
                    LOGF(info,"PDG: %d",mSign);

                    if(mSign * eSign < 0) //correlation occurs when opposite sign pair is found
                    {
                        //LOGF(info,"correlation found");
                        //LOGF(info,"");
                        corrCount++;
                        count++;

                        pairs.push_back(elecs[j]);
                        pairs.push_back(muons[i]);
                        double dPhi = elecs[j].phi() - muons[i].phi();

                        if(dPhi<0) {dPhi = dPhi + 2.*M_PI;}
                        else if(dPhi>2*M_PI) {dPhi = dPhi - 2.*M_PI;}

                        ptE_mcCorr->Fill(elecs[j].pt());
                        phiE_mcCorr->Fill(elecs[j].phi());
                        etaE_mcCorr->Fill(elecs[j].eta());

                        ptMu_mcCorr->Fill(muons[i].pt());
                        phiMu_mcCorr->Fill(muons[i].phi());
                        etaMu_mcCorr->Fill(muons[i].eta());

                        dPhi_mc->Fill(dPhi);

                    }
                }
            }
        }
    }
};

struct dPhiLinkTracks
{
    Int_t pdgE = 11;
    Int_t pdgMu = 13;
    Int_t pdgPi = 211;
    Int_t pdgPi0 = 111;
    int countColl = 0;
    int corrCount = 0;

    int twoPi = 2.*M_PI;

    //Specific Histograms
    OutputObj<TH1F> ptE_linkCorr{TH1F("ptE_linkCorr", "ptE_linkCorr", 100, 0., 10)};
    OutputObj<TH1F> phiE_linkCorr{TH1F("phiE_linkCorr", "phiE_linkCorr", 100, 0., twoPi)};
    OutputObj<TH1F> etaE_linkCorr{TH1F("etaE_linkCorr", "etaE_linkCorr", 100, -2.01, 2.01)};

    OutputObj<TH1F> ptMu_linkCorr{TH1F("ptMu_linkCorr", "ptMu_linkCorr", 100, 0., 10)};
    OutputObj<TH1F> phiMu_linkCorr{TH1F("phiMu_linkCorr", "phiMu_linkCorr", 100, 0., twoPi)};
    OutputObj<TH1F> etaMu_linkCorr{TH1F("etaMu_linkCorr", "etaMu_linkCorr", 100, -2.01, 2.01)};

    OutputObj<TH1F> dPhi_link{TH1F("dPhi_link", "dPhi_link", 60, 0, twoPi)};

    std::vector<aod::McParticle> elecs;
    std::vector<aod::McParticle> muons;
    std::vector<aod::McParticle> pairs;

    void process(aod::McCollision const& mcCollision, soa::SmallGroups<soa::Join<aod::McCollisionLabels, aod::Collisions>> const& collisions,
            soa::Join<aod::Tracks, aod::McTrackLabels> const& tracks, aod::McParticles const& mcParticles)
               
    {
        int countE = 0;
        int countMu = 0;
        //int countPi = 0;
        countColl++;
       // LOGF(info, "Collision %d", countColl);

        for (auto& collision : collisions)
        {
            auto groupedTracks = tracks.sliceBy(aod::track::collisionId, collision.globalIndex());

            for (auto& track : groupedTracks) {

                if (!track.has_mcParticle()) 
                {
                    LOGF(warning, "No MC particle for track, skip...");
                    continue;
                }

                else
                {
                    auto part = track.mcParticle();
                    if(part.getGenStatusCode() > 0)
                    {
                        if(abs(part.pdgCode()) == pdgE) 
                        {
                            countE++;
                            elecs.push_back(part);

                        } 
                        else if(abs(part.pdgCode()) == pdgMu) 
                        {
                            countMu++;
                            muons.push_back(part);
                        } 

                        //else if(abs(part.pdgCode()) == pdgPi or abs(part.pdgCode()) == pdgPi0) 
                        //{
                        //    countPi++;
                        //}

                    }
                }
            }
        }

        if (countE > 0 and countMu >0)
        {
            LOGF(info, "Collision %d", countColl);
            LOGF(info,"possible correlations");
            int count = 0;
            for (int i=0; i<countMu; i++)
            {
                Int_t mSign = muons[i].pdgCode();

                for (int j=0; j<countE; j++)
                {
                    Int_t eSign = elecs[j].pdgCode();
                    LOGF(info,"Electron: %d",j);
                    LOGF(info,"PDG: %d",eSign);
                    LOGF(info,"Muon: %d",i);
                    LOGF(info,"PDG: %d",mSign);

                    if(mSign * eSign < 0) //correlation occurs when opposite sign pair is found
                    {
                        //LOGF(info,"correlation found");
                        //LOGF(info,"");
                        corrCount++;
                        count++;

                        pairs.push_back(elecs[j]);
                        pairs.push_back(muons[i]);
                        double dPhi = elecs[j].phi() - muons[i].phi();

                        if(dPhi<0) {dPhi = dPhi + 2.*M_PI;}
                        else if(dPhi>2*M_PI) {dPhi = dPhi - 2.*M_PI;}

                        ptE_linkCorr->Fill(elecs[j].pt());
                        phiE_linkCorr->Fill(elecs[j].phi());
                        etaE_linkCorr->Fill(elecs[j].eta());

                        ptMu_linkCorr->Fill(muons[i].pt());
                        phiMu_linkCorr->Fill(muons[i].phi());
                        etaMu_linkCorr->Fill(muons[i].eta());

                        dPhi_link->Fill(dPhi);

                    }
                }
            }
        }


        
    }

};

struct NumParticles 
{   
    //OutputObj<TH1F> chargeH{TH1F("charge_prim", "charge_prim", 100, -5, 5)};
    OutputObj<TH1F> chargeH{TH1F("charge_prim", "charge_prim", 80, -3, 3)};
    Service<TDatabasePDG> pdgDB;
    int primCount = 0;

    void process(aod::McCollision const& mcCollision, aod::McParticles const& mcParticles)
    {
        for (auto& part : mcParticles) 
        {
            
            if (part.isPhysicalPrimary())
            {
                primCount++;

                auto code = pdgDB->GetParticle(part.pdgCode());
                if (!code) {continue;}

                chargeH->Fill(code->Charge() / 3.); //charge comes in units of 1/3
            }

            
        }

        //LOGF(info, "Counted Primaries: %d", primCount);
    }
    void finalCount(){LOGF(info, "Counted Primaries: %d", "primCount");}

};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    adaptAnalysisTask<LogProperties>(cfgc),
    adaptAnalysisTask<CountSpecies>(cfgc),
    adaptAnalysisTask<Parents>(cfgc),
    //adaptAnalysisTask<dPhiMC>(cfgc),
    //adaptAnalysisTask<dPhiLinkTracks>(cfgc),
    adaptAnalysisTask<NumParticles>(cfgc)};
}