import FWCore.ParameterSet.Config as cms

leptonDecaysFilter = cms.EDFilter('leptonDecaysFilter',

   genJetSrc           = cms.InputTag("ak5GenJets"),
   genParticleSrc      = cms.InputTag("genParticles"),

   WTauHadFilter       = cms.bool(False),
   WTauMuFilter        = cms.bool(False),
   WTauEleFilter       = cms.bool(False),

   OtherTauHadFilter   = cms.bool(False),
   OtherTauMuFilter    = cms.bool(False),
   OtherTauEleFilter   = cms.bool(False),

   doZfilter           = cms.bool(False),

   OtherWLepRejection  = cms.bool(False),

   WEleMuTauFilter     = cms.bool(False),
   WEleMuTaulepFilter  = cms.bool(False),

   WMuFilter       = cms.bool(False),
   WEleFilter      = cms.bool(False),

   OtherMuFilter   = cms.bool(False),
   OtherEleFilter  = cms.bool(False),

   MuAndEleFilter  = cms.bool(False),

   allOthersFilter = cms.bool(False),

)
