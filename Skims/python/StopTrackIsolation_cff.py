from SusyAnaTools.Skims.trackIsolationMaker_cfi import *

# make collection of isolated tracks
isolatedTracks              = trackIsolationFilter.clone()
isolatedTracks.doTrkIsoVeto = False

# count isolated tracks
countIsoTracks           = trackIsolationCounter.clone()
countIsoTracks.src       = cms.InputTag("isolatedTracks")
countIsoTracks.minNumber = 1

stopIsoTracks = cms.Sequence(
  isolatedTracks
)    

stopIsoTrackVeto  = cms.Sequence(
  ~countIsoTracks
)
