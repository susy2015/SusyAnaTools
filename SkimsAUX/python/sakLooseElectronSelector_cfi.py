
import FWCore.ParameterSet.Config as cms

sakLooseElectronSelector = cms.EDFilter(
  "SAKLooseElectronSelector",
  ElectronSource   = cms.InputTag("patElectrons"),
  VertexSource = cms.InputTag("goodVertices"),
  PFSource     = cms.InputTag("particleFlow"),
  DoElectronVeto   = cms.bool(True),
)
