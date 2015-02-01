
import FWCore.ParameterSet.Config as cms

nJetsForSkimsRA2 = cms.EDFilter(
  "nJetsForSkimsRA2",
  JetSource = cms.InputTag("patJets"),
  MinJetPt      = cms.double(50),
  MaxJetEta     = cms.double(2.5)
)
