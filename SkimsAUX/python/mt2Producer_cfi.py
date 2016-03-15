import FWCore.ParameterSet.Config as cms

mt2 = cms.EDProducer('Mt2Producer',
JetTag               = cms.InputTag('JetTag_'),
METTag               = cms.InputTag('METTag_'),
)
