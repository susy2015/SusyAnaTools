import FWCore.ParameterSet.Config as cms

genJetCounterFilter = cms.EDFilter('genJetCounterFilter',

   genJetSrc           = cms.InputTag("ak5GenJets"),
   minJetNum           = cms.int32(0),
   maxJetNum           = cms.int32(99999),

)                               
