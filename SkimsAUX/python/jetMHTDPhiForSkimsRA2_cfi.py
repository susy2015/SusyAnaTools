
import FWCore.ParameterSet.Config as cms

jetMHTDPhiForSkimsRA2 = cms.EDFilter(
  "JetMHTDPhiForSkimsRA2",
  MHTSource = cms.InputTag("mht"),
  JetSource = cms.InputTag("patJets")
)
