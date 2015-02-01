
import FWCore.ParameterSet.Config as cms

from RecoMuon.MuonIsolation.muonPFIsolationDeposits_cff import *

studyMuonIsolation = cms.EDFilter(
  "StudyMuonIsolation",
  MuonSource   = cms.InputTag('patMuonsPF'),
  vtxSrc       = cms.InputTag("goodVertices"),
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

  matchDR     = cms.double(0.3),

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

  doFillTree = cms.bool(True),
  studyMode  = cms.bool(True),

  jetSrc              = cms.InputTag("patJetsAK5PFPt30"),
  nVtxPUcut           = cms.uint32(9),

  metSrc              = cms.InputTag("patMETsPF"),
  mhtSrc              = cms.InputTag("mhtPF"),
  htSrc               = cms.InputTag("htPF"),

  dPhis_CUT_vec_Src   = cms.InputTag("jetMHTDPhiForSkimsRA2:jetMHTDPhiVec"),
  nJets_CUT_Src       = cms.InputTag("nJetsForSkimsRA2:nJets"),

  externalBitToTree_Src = cms.InputTag(""),

)
