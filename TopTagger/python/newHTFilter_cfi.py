
import FWCore.ParameterSet.Config as cms

newHTFilter = cms.EDFilter(
  "NewHTFilter",
  jetSrc = cms.InputTag("patJetsAK5PFPt30"),
)
