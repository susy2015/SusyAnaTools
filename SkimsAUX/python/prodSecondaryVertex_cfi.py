import FWCore.ParameterSet.Config as cms

prodSecondaryVertex = cms.EDFilter(
  "prodSecondaryVertex",
  vtxSrc = cms.InputTag("offlineSlimmedPrimaryVertices"),
  secvtxSrc = cms.InputTag("slimmedSecondaryVertices"),#"offlineSlimmedSecondaryVertices"),
  jetSrc = cms.InputTag('slimmedJets')
)
