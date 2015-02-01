
import FWCore.ParameterSet.Config as cms

ht = cms.EDProducer("HTProducer",
  JetCollection = cms.InputTag("patJets"),
  MinJetPt      = cms.double(50),
  MaxJetEta     = cms.double(2.5)
)
