import FWCore.ParameterSet.Config as cms

prodMET = cms.EDFilter(
  "prodMET",
  metSrc = cms.InputTag('slimmedMETsMuEGClean'),
  metSrcMuCleanOnly = cms.InputTag('slimmedMETs'),
  metSrcEGCleanOnly = cms.InputTag('slimmedMETsEGClean'),
  metSrcNoClean = cms.InputTag('slimmedMETsUncorrected'),
  debug  = cms.bool(False)
)
