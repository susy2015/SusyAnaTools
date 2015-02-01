
import FWCore.ParameterSet.Config as cms

prodElectrons = cms.EDFilter(
  "prodElectrons",
  ElectronSource    = cms.InputTag('slimmedElectrons'),
  ConversionsSource = cms.InputTag("reducedEgamma", "reducedConversions"), #not used for WP VETO
  VertexSource      = cms.InputTag('goodVertices'),
  metSource         = cms.InputTag('slimmedMETs'),
  BeamSpotSource    = cms.InputTag("offlineBeamSpot"),
  MinElePt       = cms.double(5),
  MaxEleEta      = cms.double(2.5),
  DoElectronVeto           = cms.bool(False),
  DoElectronID             = cms.bool(True),
  DoElectronVtxAssociation = cms.bool(True),
  DoElectronIsolation      = cms.bool(True),
  Debug                    = cms.bool(False)
)
