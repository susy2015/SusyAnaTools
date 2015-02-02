
import FWCore.ParameterSet.Config as cms

mht = cms.EDProducer("MHTProducer",
  JetCollection = cms.InputTag("patJets"),
  MinJetPt      = cms.double(30),
  MaxJetEta     = cms.double(5)
)
