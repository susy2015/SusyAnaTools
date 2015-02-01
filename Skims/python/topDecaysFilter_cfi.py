
import FWCore.ParameterSet.Config as cms

topDecaysFilter = cms.EDFilter(
  "topDecaysFilter",
 
  genJetSrc           = cms.InputTag("ak5GenJets"),
  genParticleSrc      = cms.InputTag("genParticles"),

  toPickTypeStrVec    = cms.vstring("all"),

  debug               = cms.bool(False),
)
