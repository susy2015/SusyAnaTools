# Sequence to clean events based on boolean filter results stored on pattuples

from SusyAnaTools.Skims.filterBoolean_cfi import *

RA2_HBHENoiseFilterRA2    = booleanFilter.clone()
RA2_HBHENoiseFilterRA2.ResultSource = cms.InputTag("HBHENoiseFilterRA2","HBHENoiseFilterResult","PAT")

RA2_beamHaloFilter        = booleanFilter.clone()
RA2_beamHaloFilter.ResultSource = cms.InputTag("beamHaloFilter")

# Don't use this filter for now, needs to understand performance on physics events
RA2_eeNoiseFilter         = booleanFilter.clone()
RA2_eeNoiseFilter.ResultSource = cms.InputTag("eeNoiseFilter")

RA2_trackingFailureFilter = booleanFilter.clone()
RA2_trackingFailureFilter.ResultSource = cms.InputTag("trackingFailureFilter")

RA2_inconsistentMuons     = booleanFilter.clone()
RA2_inconsistentMuons.ResultSource = cms.InputTag("inconsistentMuons")

RA2_greedyMuons           = booleanFilter.clone()
RA2_greedyMuons.ResultSource = cms.InputTag("greedyMuons")

RA2_EcalTPFilter          = booleanFilter.clone()
RA2_EcalTPFilter.ResultSource = cms.InputTag("ra2EcalTPFilter")

RA2_EcalBEFilter          = booleanFilter.clone()
RA2_EcalBEFilter.ResultSource = cms.InputTag("ra2EcalBEFilter")

HcalLaserEventFilter      = booleanFilter.clone()
HcalLaserEventFilter.ResultSource = cms.InputTag("hcalLaserEventFilter")

EEBadScFilter             = booleanFilter.clone()
EEBadScFilter.ResultSource= cms.InputTag("eeBadScFilter")

EcalLaserFilter           = booleanFilter.clone()
EcalLaserFilter.ResultSource= cms.InputTag("ecalLaserCorrFilter")

RA2_PBNRFilter            = booleanFilter.clone()
RA2_PBNRFilter.ResultSource = cms.InputTag("ra2PBNR")

cleaningOnFilterResults  = cms.Sequence(
    RA2_HBHENoiseFilterRA2
  * RA2_beamHaloFilter
  * RA2_trackingFailureFilter
  * RA2_inconsistentMuons
  * RA2_greedyMuons
  * RA2_EcalTPFilter
  * RA2_EcalBEFilter
  * HcalLaserEventFilter
  * EEBadScFilter
  #* RA2_eeNoiseFilter
#  * EcalLaserFilter
  #* RA2_PBNRFilter #(to be applied jet-by-jet)
)
