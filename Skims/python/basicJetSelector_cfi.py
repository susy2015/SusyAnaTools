import FWCore.ParameterSet.Config as cms

# module to select Jets
# See https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string
#
selectedPatJets = cms.EDFilter("RA2BasicJetSelector",
    src = cms.InputTag("slimmedJets"),
    cut = cms.string("")
)


