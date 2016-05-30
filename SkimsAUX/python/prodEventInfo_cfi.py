import FWCore.ParameterSet.Config as cms

prodEventInfo = cms.EDFilter(
  "prodEventInfo",
  vtxSrc = cms.InputTag('goodVertices'),
  puppiSrc = cms.InputTag("slimmedAddPileupInfo"),
  genSrc= cms.InputTag("generator"),
  debug  = cms.bool(False)
)
