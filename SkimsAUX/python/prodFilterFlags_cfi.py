import FWCore.ParameterSet.Config as cms

filterDecisionProducer = cms.EDProducer('prodFilterFlags',
   trigTagSrc = cms.InputTag("TriggerResults"),
   filterName    =   cms.string("Flag_METFilters")
)
