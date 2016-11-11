import FWCore.ParameterSet.Config as cms

prodJetIDEventFilter = cms.EDFilter("prodJetIDEventFilter",
        JetSource = cms.InputTag('slimmedJets'),
        MinJetPt      = cms.double(0.0),
        MaxJetEta     = cms.double(999.0),
        debug         = cms.bool(False),
        taggingMode   = cms.bool(True),
)
