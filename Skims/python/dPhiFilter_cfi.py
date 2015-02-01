
import FWCore.ParameterSet.Config as cms

dPhiFilter = cms.EDFilter(
  "DPhiFilter",
  metSrc = cms.InputTag("patMETsPF"),
  jetSrc = cms.InputTag("patJetsAK5PFPt30"),
)
