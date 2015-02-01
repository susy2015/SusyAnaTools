
import FWCore.ParameterSet.Config as cms

from RecoMuon.MuonIsolation.muonPFIsolationDeposits_cff import *

goldenMuonSelector = cms.EDFilter(
  "GoldenMuonSelector",
  MuonSource   = cms.InputTag('patMuonsPF'),
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

  DoMuonProvInfo       = cms.bool(True),

  muonIsoSel  = cms.int32(1),
  rhoSrc      = cms.InputTag("kt6PFJetsCentralNeutral", "rho"),

  RadialIsoCalculator = cms.PSet(
    weightfac = cms.double(3.),
    IsoDepositChargedCollection = cms.InputTag("muPFIsoDepositChargedPF"),
    IsoDepositNeutralCollection = cms.InputTag("muPFIsoDepositNeutralPF"),
    IsoDepositGammaCollection   = cms.InputTag("muPFIsoDepositGammaPF"),
    deltaR = cms.double(0.3),
    skipDefaultVeto = cms.bool(True),
    vetos = cms.vstring('0.0001', 'Threshold(0.0)'),
    debug = cms.bool(False),
  ),

  inputProductIDSrc = cms.InputTag(''),
  inputKeySrc = cms.InputTag(''),

  jetSrc = cms.InputTag('patJetsPF'),

  doFillTree = cms.bool(False),
  studyMode  = cms.bool(False),
)
