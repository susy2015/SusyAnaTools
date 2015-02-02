
import FWCore.ParameterSet.Config as cms

mymhtPFforSgnf = cms.EDProducer("signifMHTProducer",
  JetCollection = cms.InputTag("patJetsAK5PFPt30"),
  MinJetPt      = cms.double(30),
  MaxJetEta     = cms.double(5),

  resolutionsAlgo = cms.string("AK5PF"),
  resolutionsEra  = cms.string("Spring10"),
)
