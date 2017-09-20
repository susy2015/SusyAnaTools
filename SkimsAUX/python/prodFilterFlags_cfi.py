import FWCore.ParameterSet.Config as cms

filterDecisionProducer = cms.EDProducer('prodFilterFlags',
   trigTagSrc = cms.InputTag("TriggerResults", "", "HLT"),
   filterName    =   cms.string("Flag_METFilters")
)
