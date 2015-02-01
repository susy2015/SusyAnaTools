
import FWCore.ParameterSet.Config as cms

topTagger = cms.EDFilter(
  "TopTagger",
  metSrc = cms.InputTag("patMETsPF"),
  jetSrc = cms.InputTag("patJetsAK5PFPt30"),
  evtWeightInput = cms.InputTag(""),
  vtxSrc = cms.InputTag("goodVertices"),
)
