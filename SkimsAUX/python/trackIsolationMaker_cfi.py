import FWCore.ParameterSet.Config as cms

trackIsolationFilter = cms.EDFilter("TrackIsolationFilter",
                                     pfCandidatesTag     = cms.InputTag("packedPFCandidates"),
                                     vertexInputTag      = cms.InputTag("goodVertices"),
                                     dR_ConeSize         = cms.double(0.3),
                                     dz_CutValue         = cms.double(0.1),
                                     minPt_PFCandidate   = cms.double(5.0),
                                     isoCut              = cms.double(0.2),
                                     doTrkIsoVeto        = cms.bool(False),
                                     doFillTree          = cms.bool(False),
                                     evtWeightInput      = cms.InputTag("weightProducer:weight"),
)
