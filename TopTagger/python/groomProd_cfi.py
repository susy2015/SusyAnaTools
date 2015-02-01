
import FWCore.ParameterSet.Config as cms

groomProd = cms.EDFilter(
  "groomProd",
  jetSrc = cms.InputTag("patJetsPFchsPt10"),
)
