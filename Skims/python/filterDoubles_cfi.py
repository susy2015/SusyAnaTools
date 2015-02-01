import FWCore.ParameterSet.Config as cms

doublesFilter = cms.EDFilter(
  "FilterDoubles",
  ResultSource = cms.InputTag("topTagger"),
)
