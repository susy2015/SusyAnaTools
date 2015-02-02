
import FWCore.ParameterSet.Config as cms

htFilter = cms.EDFilter(
  "HTFilter",
  HTSource = cms.InputTag("ht"),
  MinHT = cms.double(350.0)
)
