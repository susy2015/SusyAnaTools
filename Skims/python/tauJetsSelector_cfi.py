import FWCore.ParameterSet.Config as cms

tauJetsSelector = cms.EDFilter("TauJetsSelector",
  MetSrc    = cms.InputTag("patMETsPF"),
  JetSrc    = cms.InputTag("patJetsAK5PF"),
  JetPtMin  = cms.double(15.0),
  JetEtaMax = cms.double(2.4),
  bTagName  = cms.string("combinedSecondaryVertexBJetTags"),
  bTagValue = cms.double(0.898),
  DoTauVeto = cms.bool(False)
)
