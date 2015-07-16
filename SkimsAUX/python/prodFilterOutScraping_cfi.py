
import FWCore.ParameterSet.Config as cms

prodFilterOutScraping = cms.EDFilter(
  "prodFilterOutScraping",
  applyfilter = cms.untracked.bool(False),
  debugOn = cms.untracked.bool(False),
  numtrack = cms.untracked.uint32(10),
  thresh = cms.untracked.double(0.25)
)
