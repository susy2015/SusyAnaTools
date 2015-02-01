
import FWCore.ParameterSet.Config as cms

prodEventInfo = cms.EDFilter(
  "prodEventInfo",
  vtxSrc = cms.InputTag('goodVertices'),
  debug  = cms.bool(False)
)
