import FWCore.ParameterSet.Config as cms

prodGoodVertices = cms.EDFilter(
  "prodGoodVertices",
  vtxSrc = cms.InputTag("offlineSlimmedPrimaryVertices"),
  secvtxSrc = cms.InputTag("offlineSlimmedSecondaryVertices"),
  vtxCut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)

goodVertices = cms.EDFilter(
  "VertexSelector",
  filter = cms.bool(False),
  src = cms.InputTag("offlineSlimmedPrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)
