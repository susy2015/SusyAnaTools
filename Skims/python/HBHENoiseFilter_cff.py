
# configure HBHEFilter according to recommendation on thi HN post
# https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/1196.html
## uncomment to switch-off the isolation based filter
##HBHENoiseFilter.minIsolatedNoiseSumE = cms.double(999999.)
##HBHENoiseFilter.minNumIsolatedNoiseChannels = cms.int32(999999)
##HBHENoiseFilter.minIsolatedNoiseSumEt = cms.double(999999.)

# include this one in path to filter the event
from CommonTools.RecoAlgos.HBHENoiseFilter_cfi import *

# incelude this one to store the information without filtering the event
from CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi import HBHENoiseFilterResultProducer
HBHENoiseFilterRA2 = HBHENoiseFilterResultProducer.clone()

