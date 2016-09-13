
import FWCore.ParameterSet.Config as cms

prodJets = cms.EDFilter(
  "prodJets",
  jetSrc = cms.InputTag('slimmedJets'),
  jetOtherSrc = cms.InputTag('patJetsAK4PFCHS'),
  jetType = cms.string('AK4PFchs'),
  qgTaggerKey = cms.string('QGTagger'),
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
  puppiJetsSrc = cms.InputTag("slimmedJetsPuppi"),
  ak8JetsSrc = cms.InputTag("slimmedJetsAK8"),
  debug  = cms.bool(False),
  #double tau21        = 99;
  #double tau32        = 99;
  #double puppi_tau21        = 99;
  #double puppi_tau32        = 99;
  #bool SoftDropTau32Tagged = false;
  properties = cms.vstring(
       "bDiscriminatorCSV",
  ),
  bDiscriminatorCSV = cms.vstring('pfCombinedInclusiveSecondaryVertexV2BJetTags'), 
)
