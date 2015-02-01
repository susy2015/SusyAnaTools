import FWCore.ParameterSet.Config as cms

btagJetsSelector = cms.EDFilter("BJetsSelector",
  JetSrc       = cms.InputTag("patJetsPF"),
  JetPtMin     = cms.double(30.0),
  JetEtaMax    = cms.double(2.4),
  bTagName     = cms.string("combinedSecondaryVertexBJetTags"),
  bTagValue    = cms.double(0.679),
  SelecOnBJets = cms.bool(False)
)
