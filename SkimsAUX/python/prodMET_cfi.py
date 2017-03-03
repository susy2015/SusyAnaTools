
import FWCore.ParameterSet.Config as cms

prodMET = cms.EDFilter(
  "prodMET",
  metSrc = cms.InputTag('slimmedMETs'),
  debug  = cms.bool(False),
  addcalomet = cms.bool(True)
)
