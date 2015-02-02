
import FWCore.ParameterSet.Config as cms

muonPFCandidateProducer = cms.EDProducer("MuonPFCandidateProducer",
  PATMuonCollection = cms.InputTag("patMuonsPF")
)
