import FWCore.ParameterSet.Config as cms

ra2BEonlyFilter = cms.EDFilter('applyBEfilter',

  debug = cms.untracked.bool( False ),

# Ulla's BE filter
  ecalAnomalousFilterTag = cms.InputTag("ecalAnomalousFilter","anomalousECALVariables"),

  minDeadClusterSize = cms.double(24),
  maxBoundaryEnergy = cms.double(10),

  limitDeadCellToChannelStatusEB = cms.vint32(12,14),
  limitDeadCellToChannelStatusEE = cms.vint32(12,14),

)
