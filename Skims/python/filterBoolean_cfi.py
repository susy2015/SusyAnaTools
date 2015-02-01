import FWCore.ParameterSet.Config as cms

booleanFilter = cms.EDFilter(
  "FilterBoolean",
  ResultSource = cms.InputTag("boolean"),
)
