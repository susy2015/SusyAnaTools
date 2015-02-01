
import FWCore.ParameterSet.Config as cms

prodJets = cms.EDFilter(
  "prodJets",
  jetSrc = cms.InputTag('ak4patJetsPFchsPt10'),
  vtxSrc = cms.InputTag('goodVertices'),
  metSrc = cms.InputTag('slimmedMETs'),
  bTagKeyString = cms.string('combinedInclusiveSecondaryVertexV2BJetTags'),
  debug  = cms.bool(False)
)
