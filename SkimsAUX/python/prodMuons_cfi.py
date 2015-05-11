
import FWCore.ParameterSet.Config as cms

prodMuons = cms.EDFilter(
  "prodMuons",
  MuonSource   = cms.InputTag('slimmedMuons'),
  VertexSource = cms.InputTag('goodVertices'),
  metSource    = cms.InputTag('slimmedMETs'),
  PFCandSource = cms.InputTag('packedPFCandidates'),
  MinMuPt      = cms.double(5),
  MaxMuEta     = cms.double(2.4),
  MaxMuD0      = cms.double(0.2),
  MaxMuDz      = cms.double(0.5),
  MaxMuRelIso  = cms.double(0.20),
  MinMuNumHit  = cms.double(11),
  DoMuonVeto           = cms.bool(False),
  DoMuonID             = cms.bool(True),
  DoMuonVtxAssociation = cms.bool(True),
  DoMuonIsolation      = cms.bool(True),
  Debug                = cms.bool(False)
)
