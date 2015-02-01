
import FWCore.ParameterSet.Config as cms

singleEventFilter = cms.EDFilter(
  "SingleEventFilter",
  Run   = cms.uint32(0),
  Lumi  = cms.uint32(0),
  Event = cms.uint32(0)
)

vetoIncMuon7    =  singleEventFilter.clone(Run = 149291, Lumi =  759, Event = 766167308)
