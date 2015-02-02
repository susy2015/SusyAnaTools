import FWCore.ParameterSet.Config as cms

jesVariation = cms.EDProducer("JESVariation",
                              JetSource      = cms.InputTag('patJets'),
                              UncertaintyTag = cms.string('Uncertainty'),
                              PayloadName    = cms.string('AK5PF'),
                              JECUpDown      = cms.string("None"),
                              jetMaxEtaCut   = cms.double(-1.0),
                              jetMinPtCut    = cms.double(-1.0),
)

jesUpJets = jesVariation.clone()
jesUpJets.JECUpDown = "UP"

jesDnJets = jesVariation.clone()
jesDnJets.JECUpDown = "DOWN"
