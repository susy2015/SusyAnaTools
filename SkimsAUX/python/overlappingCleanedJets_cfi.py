import FWCore.ParameterSet.Config as cms

overlappingCleanedJets = cms.EDProducer('cleanOverlappingLeptonsJets',

   muonSource          = cms.InputTag("patMuonsPFIDIso"),
   eleSource           = cms.InputTag("patElectronsPFIDIso"),
   jetSource           = cms.InputTag("patJetsAK5PFPt50Eta25"),
   muonCSSource        = cms.InputTag("myPatMuonsPFIDIso"),

   muonProductIDSrc    = cms.InputTag("patMuonsPFIDIso:muonPFbaseProductID"),
   muonKeySrc          = cms.InputTag("patMuonsPFIDIso:muonPFbaseKey"),
   eleProductIDSrc     = cms.InputTag("patElectronsPFIDIso:elePFbaseProductID"),
   eleKeySrc           = cms.InputTag("patElectronsPFIDIso:elePFbaseKey"),

   doGenLevel          = cms.bool(False),
   genJetSrc           = cms.InputTag("ak5GenJets"),
   genParticleSrc      = cms.InputTag("genParticles"),
   genJetMinPt         = cms.double(0),
   genJetMaxEta        = cms.double(999999),
   deltaR              = cms.double(0.3),
   deltaRcon           = cms.double(0.01),
   deltaRtighterForEle = cms.double(0.1),

   recoJetMinPt        = cms.double(-1),
   recoJetMaxEta       = cms.double(-1),

   debug               = cms.bool(False),

   useEle              = cms.bool(False),

   verbose             = cms.bool(False),

   useDeltaR           = cms.bool(False),
)                               
