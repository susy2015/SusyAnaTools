
import FWCore.ParameterSet.Config as cms

provInfoMuons = cms.EDFilter(
  "provInfoMuons",
  MuonSource   = cms.InputTag('patMuonsPF'),
)
