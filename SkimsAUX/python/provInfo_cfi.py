import FWCore.ParameterSet.Config as cms

provInfo = cms.EDProducer('provInfo',
   muonSource          = cms.InputTag("patMuonsPFIDIso"),
   eleSource           = cms.InputTag("patElectronsPFIDIso"),
   debug               = cms.bool(False),
)                               
