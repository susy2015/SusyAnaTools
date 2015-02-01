
import FWCore.ParameterSet.Config as cms

signifFilter = cms.EDFilter(
  "SignifFilter",
  metSrc = cms.InputTag("patMETsPF"),
  jetSrc = cms.InputTag("patJetsAK5PFPt30"),
)
