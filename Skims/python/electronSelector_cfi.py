
import FWCore.ParameterSet.Config as cms

electronSelector = cms.EDFilter(
  "GoodElectronSelector",
  ElectronSource    = cms.InputTag('gsfElectrons'),
  ConversionsSource = cms.InputTag("allConversions"), #not used for WP VETO
  VertexSource      = cms.InputTag('goodVertices'),
  BeamSpotSource    = cms.InputTag("offlineBeamSpot"),
  RhoIsoSource      = cms.InputTag("kt6PFJets", "rho"),
  IsoValInputTags   = cms.VInputTag(cms.InputTag('elPFIsoValueCharged03PFIdPFIso'),
                                    cms.InputTag('elPFIsoValueGamma03PFIdPFIso'),
                                    cms.InputTag('elPFIsoValueNeutral03PFIdPFIso')),
  MinElePt       = cms.double(10),
  MaxEleEta      = cms.double(2.5),
  DoElectronVeto           = cms.bool(False),
  DoElectronID             = cms.bool(True),
  DoElectronVtxAssociation = cms.bool(True),
  DoElectronIsolation      = cms.bool(True),
  Debug                    = cms.bool(False)
)
