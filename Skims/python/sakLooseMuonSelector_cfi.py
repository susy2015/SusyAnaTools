
import FWCore.ParameterSet.Config as cms

sakLooseMuonSelector = cms.EDFilter(
  "SAKLooseMuonSelector",
  MuonSource   = cms.InputTag("patMuonsPF"),
  VertexSource = cms.InputTag("goodVertices"),
  PFSource     = cms.InputTag("pfNoPileUpIsoPF"),
  DoMuonVeto   = cms.bool(True),
  DoMuonIso    = cms.bool(True),
  DoMuonID     = cms.bool(True),
)
