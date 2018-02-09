import FWCore.ParameterSet.Config as cms

my_id_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Spring16_V2p2_cff']

goodPhotons = cms.EDProducer("PhotonIDisoProducer",
        photonCollection       = cms.untracked.InputTag("slimmedPhotons"),
        electronCollection     = cms.untracked.InputTag("slimmedElectrons"),
#       conversionCollection   = cms.untracked.InputTag("reducedEgamma","reducedConversions",tagname),

        #Andres
        loosePhotonID                = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-loose"),
        mediumPhotonID               = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-medium"),
        tightPhotonID                = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-tight"),
        gammaSrc               = cms.InputTag("slimmedPhotons"),
        conversionCollection   = cms.untracked.InputTag("reducedEgamma","reducedConversions"),
        beamspotCollection     = cms.untracked.InputTag("offlineBeamSpot"),
        ecalRecHitsInputTag_EE = cms.InputTag("reducedEgamma","reducedEERecHits"),
        ecalRecHitsInputTag_EB = cms.InputTag("reducedEgamma","reducedEBRecHits"),
        rhoCollection          = cms.untracked.InputTag("fixedGridRhoFastjetAll"),
        genParCollection = cms.untracked.InputTag("prunedGenParticles"), 
        debug                  = cms.untracked.bool(False)
    )
