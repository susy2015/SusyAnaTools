import FWCore.ParameterSet.Config as cms

studyHadTauDecayInGenLevelFilter = cms.EDFilter('studyHadTauDecayInGenLevelFilter',

   genJetSrc           = cms.InputTag("ak5GenJets"),
   genParticleSrc      = cms.InputTag("genParticles"),

   WTauHadFilter       = cms.bool(False),
   WTauMuFilter        = cms.bool(False),
   WTauEleFilter       = cms.bool(False),

   OtherTauHadFilter   = cms.bool(False),
   OtherTauMuFilter    = cms.bool(False),
   OtherTauEleFilter   = cms.bool(False),

   doZfilter           = cms.bool(False),

   OtherWLepRejection = cms.int32(0),

   doFillTree          = cms.bool(False),
   vtxSrc              = cms.InputTag("goodVertices"),

   evtWeightInput = cms.InputTag("weightProducer:weight"),
)
