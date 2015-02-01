import FWCore.ParameterSet.Config as cms

subtractMuons = cms.EDProducer(
   "subtractMuons",
   oriMuonSrc = cms.InputTag('patMuonsPF'),
   subSetMuonSrc = cms.InputTag('patMuonsPF'),
   deltaR = cms.double(0.01),
)
