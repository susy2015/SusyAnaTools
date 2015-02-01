
import FWCore.ParameterSet.Config as cms

# rho value specific for electron isolaton ...
# TODO: why?
from RecoJets.JetProducers.kt4PFJets_cfi import *
kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)

goldenElectronSelector = cms.EDFilter(
  "GoldenElectronSelector",
  ElectronSource = cms.InputTag('patElectronsPF'),
  VertexSource   = cms.InputTag('goodVertices'),
  MinElePt       = cms.double(10),
  MaxEleEta      = cms.double(2.5),
  MaxEleD0       = cms.double(0.2),
  MaxEleDz       = cms.double(0.5),
  MaxEleRelIso   = cms.double(0.20), # Dummy for now. WP has predefined isolation cuts!
  DoElectronVeto           = cms.bool(False),
  DoElectronID             = cms.bool(True),
  DoElectronVtxAssociation = cms.bool(True),
  DoElectronIsolation      = cms.bool(True),

  DoElectronProvInfo       = cms.bool(True),

  conversionsInput = cms.InputTag("allConversions"),
  beamSpotInputTag = cms.InputTag("offlineBeamSpot"),
  WorkingPoint     = cms.string("VETO"),

  eleIsoSel  = cms.int32(2),
  rhoIsoInputTag  = cms.InputTag("kt6PFJetsForIsolation", "rho"),
  isoValInputTags = cms.VInputTag(cms.InputTag('elPFIsoValueCharged03PFIdPF'),
                                  cms.InputTag('elPFIsoValueGamma03PFIdPF'),
                                  cms.InputTag('elPFIsoValueNeutral03PFIdPF')
                                 ),
  RadialIsoCalculator = cms.PSet(
    weightfac = cms.double(3.),
    IsoDepositChargedCollection = cms.InputTag("elPFIsoDepositChargedPF"),
    IsoDepositNeutralCollection = cms.InputTag("elPFIsoDepositNeutralPF"),
    IsoDepositGammaCollection   = cms.InputTag("muPFIsoDepositGammaPF")
  ),

  inputProductIDSrc = cms.InputTag(''),
  inputKeySrc = cms.InputTag(''),

)
