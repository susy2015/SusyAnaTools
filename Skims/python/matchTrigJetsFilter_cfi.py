
import FWCore.ParameterSet.Config as cms

matchTrigJetsFilter = cms.EDFilter(
  "MatchTrigJetsFilter",
  jetSrc = cms.InputTag("patJetsAK5PFPt30"),
)
