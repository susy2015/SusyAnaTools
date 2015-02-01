
import FWCore.ParameterSet.Config as cms

beamHaloFilter = cms.EDFilter("BeamHaloFilter",
                              taggingMode = cms.bool(False)
)
