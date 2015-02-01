
import FWCore.ParameterSet.Config as cms

sakLooseElectronSelector = cms.EDFilter(
  "SAKLooseElectronSelector",
  ElectronSource   = cms.InputTag("patElectrons"),
  VertexSource = cms.InputTag("goodVertices"),
  PFSource     = cms.InputTag("pfNoPileUpIsoPF"),
  DoElectronVeto   = cms.bool(True),
  DoElectronIso    = cms.bool(True),
  DoElectronID     = cms.bool(True),
)
