import FWCore.ParameterSet.Config as cms

badMETsgnfFilter = cms.EDFilter('badMETsgnfFilter',

  metSrc = cms.InputTag("patMETsPF"),
 
  maxSigmaX2 = cms.double(1.e10),
  maxSigmaY2 = cms.double(1.e10),

)
