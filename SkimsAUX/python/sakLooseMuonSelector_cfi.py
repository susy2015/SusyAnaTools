
import FWCore.ParameterSet.Config as cms

sakLooseMuonSelector = cms.EDFilter(
  "SAKLooseMuonSelector",
  MuonSource   = cms.InputTag("patMuonsPF"),
  VertexSource = cms.InputTag("goodVertices"),
  PFSource     = cms.InputTag("particleFlow"),
  DoMuonVeto   = cms.bool(True),
)
