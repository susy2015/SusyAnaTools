
import FWCore.ParameterSet.Config as cms

muonSelector = cms.EDFilter(
  "GoodMuonSelector",
  MuonSource   = cms.InputTag('patMuons'),
  VertexSource = cms.InputTag('goodVertices'),
  MinMuPt      = cms.double(10),
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
