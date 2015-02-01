# $Id: RA2Cleaning_cff.py,v 1.16 2012/12/19 14:19:01 seema Exp $

# Standard Event cleaning 
from SusyAnaTools.Skims.noscraping_cfi      import *
from SusyAnaTools.Skims.vertex_cfi          import *
from SusyAnaTools.Skims.HBHENoiseFilter_cff import *

ra2StdCleaning = cms.Sequence(
                 noscraping    
               * oneGoodVertex
)

# RA2 detector noise cleaning
from RecoMET.METFilters.eeNoiseFilter_cfi         import *
from SusyAnaTools.Skims.beamHaloFilter_cfi             import *
from SusyAnaTools.Skims.HBHENoiseFilter_cff            import *
from RecoMET.METFilters.hcalLaserEventFilter_cfi  import *
from RecoMET.METFilters.eeBadScFilter_cfi         import *
from RecoMET.METFilters.trackingFailureFilter_cfi import *
hcalLaserEventFilter.vetoByRunEventNumber=cms.untracked.bool(False)
hcalLaserEventFilter.vetoByHBHEOccupancy=cms.untracked.bool(True)
from RecoMET.METFilters.ecalLaserCorrFilter_cfi   import *

ra2NoiseCleaning = cms.Sequence(
                   eeNoiseFilter
                 * trackingFailureFilter 
                 * beamHaloFilter
                 * HBHENoiseFilter     # rejects the event
                 * HBHENoiseFilterRA2  # produced a boolean & stores it
                 * hcalLaserEventFilter 
                 * eeBadScFilter
                 * ecalLaserCorrFilter
)    

## RA2 post reconstruction cleaning
# badly reconstructed muons
from SusyAnaTools.Skims.muonPFCandidateProducer_cfi                import *

from RecoMET.METFilters.greedyMuonPFCandidateFilter_cfi       import *
greedyMuons              = greedyMuonPFCandidateFilter.clone()
greedyMuons.PFCandidates = cms.InputTag('muonPFCandidateProducer')

from RecoMET.METFilters.inconsistentMuonPFCandidateFilter_cfi import *
inconsistentMuons              = inconsistentMuonPFCandidateFilter.clone()
inconsistentMuons.PFCandidates = cms.InputTag('muonPFCandidateProducer')

selectGoodPFEventsSequence = cms.Sequence(
                             muonPFCandidateProducer 
                           * inconsistentMuons       
                           * greedyMuons
)

# ECAL dead cell filters
from RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi import *
EcalDeadCellTriggerPrimitiveFilter.tpDigiCollection = cms.InputTag("ecalTPSkimNA")
ra2EcalTPFilter     = EcalDeadCellTriggerPrimitiveFilter.clone()

from RecoMET.METFilters.EcalDeadCellBoundaryEnergyFilter_cfi   import *
ra2EcalBEFilter           = EcalDeadCellBoundaryEnergyFilter.clone()
ra2EcalBEFilter.recHitsEB = "reducedEcalRecHitsEB"
ra2EcalBEFilter.recHitsEE = "reducedEcalRecHitsEE"

ra2EcalPostRecoCleaning = cms.Sequence(  
                          ra2EcalTPFilter
                        * ra2EcalBEFilter
)

# tracking-POG filters
from RecoMET.METFilters.trackingPOGFilters_cff import *
## NOTE: to make tagging mode of the tracking POG filters (three of them), please do:
## Also the stored boolean for the three filters is opposite to what we usually
## have for other filters, i.e., true means rejected bad events while false means 
## good events.
manystripclus53X.taggedMode         = cms.untracked.bool(True)
manystripclus53X.forcedValue        = cms.untracked.bool(False)
toomanystripclus53X.taggedMode      = cms.untracked.bool(True)
toomanystripclus53X.forcedValue     = cms.untracked.bool(False)
logErrorTooManyClusters.taggedMode  = cms.untracked.bool(True)
logErrorTooManyClusters.forcedValue = cms.untracked.bool(False)
trackingPOGCleaning = cms.Sequence(manystripclus53X * toomanystripclus53X * logErrorTooManyClusters)

# after all MET POG recommeded filters, also check JetID
from  SusyAnaTools.Skims.RA2JetIDFailureFilter_cfi import *

ra2PostCleaning = cms.Sequence(
                  ra2NoiseCleaning
                * selectGoodPFEventsSequence 
                * ra2EcalPostRecoCleaning
                #* trackingPOGCleaning # should be run on AODs
                #* ra2PBNR  #(to be jet-by-jet)
)
