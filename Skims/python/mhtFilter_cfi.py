
import FWCore.ParameterSet.Config as cms

mhtFilter = cms.EDFilter(
  "MHTFilter",
  MHTSource = cms.InputTag("mht"),
  MinMHT = cms.double(200.0)
)
