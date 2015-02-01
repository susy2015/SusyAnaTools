
import FWCore.ParameterSet.Config as cms

provInfoElectrons = cms.EDFilter(
  "provInfoElectrons",
  ElectronSource = cms.InputTag('patElectronsPF'),
)
