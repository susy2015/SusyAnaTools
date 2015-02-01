
import FWCore.ParameterSet.Config as cms

hepTopTagger = cms.EDFilter(
  "hepTopTagger",
  metSrc = cms.InputTag("patMETsPF"),
  jetSrc = cms.InputTag("patJetsAK5PFPt30"),
  evtWeightInput = cms.InputTag(""),
  vtxSrc = cms.InputTag("goodVertices"),
  hepTaggerJetSrc = cms.InputTag("HEPTopTagJets"),
)
