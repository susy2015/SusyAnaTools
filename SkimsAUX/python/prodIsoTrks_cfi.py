
import FWCore.ParameterSet.Config as cms

prodIsoTrks = cms.EDFilter(
  "prodIsoTrks",
  vtxSrc = cms.InputTag('offlineSlimmedPrimaryVertices'),#prodGoodVertices'),#goodVertices'),
  metSrc = cms.InputTag('slimmedMETs'),
  forVetoIsoTrkSrc = cms.InputTag("trackIsolation"),
  pfCandSrc = cms.InputTag("packedPFCandidates"),
  isotrk_dR = cms.double(0.5),
  isotrk_dz = cms.double(0.15),
  loose_isoTrkSrc = cms.InputTag("loosetrackIsolation"),
  loose_isotrk_isoVecSrc = cms.InputTag("loosetrackIsolation:pfcandstrkiso"),
  loose_isotrk_dzpvVecSrc = cms.InputTag("loosetrackIsolation:pfcandsdzpv"),
  W_emuVec = cms.InputTag("prodGenInfo:WemuVec"),
  W_tau_emuVec = cms.InputTag("prodGenInfo:WtauemuVec"),
  W_tau_prongsVec = cms.InputTag("prodGenInfo:WtauprongsVec"),
  genDecayLVec = cms.InputTag("prodGenInfo:genDecayLVec"),
  debug  = cms.bool(False),
  ref_all_isoTrkSrc = cms.InputTag("refalltrackIsolation"),
  ref_all_isoTrk_isoVecSrc = cms.InputTag("refalltrackIsolation:pfcandstrkiso"),
)
