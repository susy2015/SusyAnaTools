
import FWCore.ParameterSet.Config as cms

topTaggerType3 = cms.EDFilter(
  "TopTaggerType3",
  metSrc = cms.InputTag("patMETsPF"),
  jetSrc = cms.InputTag("patJetsAK5PFPt30"),
  evtWeightInput = cms.InputTag("weightProducer:weight"),
  vtxSrc = cms.InputTag("goodVertices"),

  genJetSrc           = cms.InputTag("ak5GenJets"),
  genParticleSrc      = cms.InputTag("genParticles"),
  genMETSrc           = cms.InputTag("genMetCalo"),

  gentopIdxVecSrc     = cms.InputTag("topDecaysFilter:topIdxVec"),
  gentopDausIdxVecSrc = cms.InputTag("topDecaysFilter:topDausIdxVec"),
  genWDausIdxVecSrc   = cms.InputTag("topDecaysFilter:WDausIdxVec"),
  gentauDausIdxVecSrc = cms.InputTag("topDecaysFilter:tauDausIdxVec"),
  decayTypeVecSrc     = cms.InputTag("topDecaysFilter:decayTypeVec"),
)
