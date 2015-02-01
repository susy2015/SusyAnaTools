
import FWCore.ParameterSet.Config as cms

jetMHTDPhiFilter = cms.EDFilter(
  "JetMHTDPhiFilter",
  MHTSource = cms.InputTag("mht"),
  JetSource = cms.InputTag("patJets")
)
