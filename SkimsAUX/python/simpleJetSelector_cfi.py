
import FWCore.ParameterSet.Config as cms

simpleJetSelector = cms.EDFilter(
  "simpleJetSelector",
  jetSrc = cms.InputTag("patJetsAK5PFPt30"),
)
