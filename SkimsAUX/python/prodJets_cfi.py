
import FWCore.ParameterSet.Config as cms

prodJets = cms.EDFilter(
  "prodJets",
  jetSrc = cms.InputTag('slimmedJets'),
  jetOtherSrc = cms.InputTag('patJetsAK4PFCHS'),
  jetType = cms.string('AK4PFchs'),
  vtxSrc = cms.InputTag('goodVertices'),
#  metSrc = cms.InputTag('slimmedMETs'),
  bTagKeyString = cms.string('pfCombinedInclusiveSecondaryVertexV2BJetTags'),
  W_emuVec = cms.InputTag("prodGenInfo:WemuVec"),
  W_tauVec = cms.InputTag("prodGenInfo:WtauVec"),
  W_tau_emuVec = cms.InputTag("prodGenInfo:WtauemuVec"),
  W_tau_prongsVec = cms.InputTag("prodGenInfo:WtauprongsVec"),
  W_tau_nuVec = cms.InputTag("prodGenInfo:WtaunuVec"),
  genDecayLVec = cms.InputTag("prodGenInfo:genDecayLVec"),
  genDecayMomRefVec = cms.InputTag("prodGenInfo:genDecayMomRefVec"),
  eleLVec = cms.InputTag("prodElectronsNoIso:elesLVec"), 
  muLVec = cms.InputTag("prodMuonsNoIso:muonsLVec"), 
  trksForIsoVetoLVec = cms.InputTag("prodIsoTrks:trksForIsoVetoLVec"),
  looseisoTrksLVec = cms.InputTag("prodIsoTrks:looseisoTrksLVec"),
  debug  = cms.bool(False)
)
