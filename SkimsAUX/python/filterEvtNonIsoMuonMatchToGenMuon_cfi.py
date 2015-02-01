import FWCore.ParameterSet.Config as cms

filterEvtNonIsoMuonMatchToGenMuon = cms.EDFilter('evtNonIsoMuonMatchToGenMuon',

   muonSource          = cms.InputTag("patMuonsPF"),
   jetSource           = cms.InputTag("patJetsAK5PFPt50Eta25"),

   genJetSrc           = cms.InputTag("ak5GenJets"),
   genParticleSrc      = cms.InputTag("genParticles"),

   deltaR              = cms.double(0.3),
   maxMuRelIso         = cms.double(0.2),

   debug               = cms.bool(False),

   doFilter            = cms.bool(False),
)                               
