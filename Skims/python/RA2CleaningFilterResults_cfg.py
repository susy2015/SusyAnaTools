# Sequence to clean events based on boolean filter results stored on pattuples

from SusyAnaTools.Skims.filterBoolean_cfi import *

process.RA2_HBHENoiseFilterRA2    = process.booleanFilter.clone()
process.RA2_HBHENoiseFilterRA2.ResultSource = cms.InputTag("HBHENoiseFilterRA2","HBHENoiseFilterResult","PAT")

process.RA2_beamHaloFilter        = process.booleanFilter.clone()
process.RA2_beamHaloFilter.ResultSource = cms.InputTag("beamHaloFilter")

# Don't use this filter for now, needs to understand performance on physics events
process.RA2_eeNoiseFilter         = process.booleanFilter.clone()
process.RA2_eeNoiseFilter.ResultSource = cms.InputTag("eeNoiseFilter")

process.RA2_trackingFailureFilter = process.booleanFilter.clone()
process.RA2_trackingFailureFilter.ResultSource = cms.InputTag("trackingFailureFilter")

process.RA2_inconsistentMuons     = process.booleanFilter.clone()
process.RA2_inconsistentMuons.ResultSource = cms.InputTag("inconsistentMuons")

process.RA2_greedyMuons           = process.booleanFilter.clone()
process.RA2_greedyMuons.ResultSource = cms.InputTag("greedyMuons")

process.RA2_EcalTPFilter          = process.booleanFilter.clone()
process.RA2_EcalTPFilter.ResultSource = cms.InputTag("ra2EcalTPFilter")

process.RA2_EcalBEFilter          = process.booleanFilter.clone()
process.RA2_EcalBEFilter.ResultSource = cms.InputTag("ra2EcalBEFilter")

process.HcalLaserEventFilter      = process.booleanFilter.clone()
process.HcalLaserEventFilter.ResultSource = cms.InputTag("hcalLaserEventFilter")

process.EEBadScFilter             = process.booleanFilter.clone()
process.EEBadScFilter.ResultSource= cms.InputTag("eeBadScFilter")

process.EcalLaserFilter           = process.booleanFilter.clone()
process.EcalLaserFilter.ResultSource= cms.InputTag("ecalLaserCorrFilter")

process.RA2_PBNRFilter            = process.booleanFilter.clone()
process.RA2_PBNRFilter.ResultSource = cms.InputTag("ra2PBNR")

process.cleaningOnFilterResults  = cms.Sequence(
    process.RA2_HBHENoiseFilterRA2
  * process.RA2_beamHaloFilter
  * process.RA2_trackingFailureFilter
  * process.RA2_inconsistentMuons
  * process.RA2_greedyMuons
  * process.RA2_EcalTPFilter
  * process.RA2_EcalBEFilter
  * process.HcalLaserEventFilter
  * process.EEBadScFilter
  #* process.RA2_eeNoiseFilter
  * process.EcalLaserFilter
  #* process.RA2_PBNRFilter #(to be applied jet-by-jet)
)
