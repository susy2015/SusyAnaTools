
import FWCore.ParameterSet.Config as cms

prodGenInfo = cms.EDFilter(
  "prodGenInfo",
  genParticleSrc = cms.InputTag("prunedGenParticles"),
  genDecayStrVecSrc   = cms.InputTag("printDecayPythia8:decayStr"),
  genDecayChainPartIdxVecSrc = cms.InputTag("printDecayPythia8:decayChainPartIdxVec"),
  PFCandSource = cms.InputTag('packedPFCandidates'),
  debug  = cms.bool(False)
)
