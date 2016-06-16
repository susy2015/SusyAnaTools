
import FWCore.ParameterSet.Config as cms

prodMuons = cms.EDFilter(
  "prodMuons",
  MuonSource    = cms.InputTag('slimmedMuons'),
  VertexSource  = cms.InputTag('goodVertices'),
  metSource     = cms.InputTag('slimmedMETs'),
  PFCandSource  = cms.InputTag('packedPFCandidates'),
  RhoSource     = cms.InputTag('fixedGridRhoFastjetCentralNeutral'),
  MinMuPt       = cms.double(5),
  MaxMuEta      = cms.double(2.4),
  MaxMuD0       = cms.double(0.2),
  MaxMuDz       = cms.double(0.5),
  MaxMuRelIso   = cms.double(0.20),
  MaxMuMiniIso  = cms.double(0.20),
  MinMuNumHit   = cms.double(11),
  DoMuonVeto           = cms.bool(False),
  DoMuonID             = cms.bool(True),
  DoMuonVtxAssociation = cms.bool(True),
  DoMuonIsolation      = cms.int32(2), # 1 for relIso; 2 for miniIso; 0 for nothing
  Debug                = cms.bool(False),
)
