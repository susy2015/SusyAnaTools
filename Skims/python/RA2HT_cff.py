
from SusyAnaTools.Skims.htProducer_cfi import *

# HT using PFJets
htPF = ht.clone()
htPF.JetCollection = cms.InputTag("patJetsAK5PFPt50Eta25")

# HT using PFchsJets
htPFchs = ht.clone()
htPFchs.JetCollection = cms.InputTag("patJetsPFchsPt50Eta25")


from SusyAnaTools.Skims.htFilter_cfi import *

# filter on PFHT
htPFFilter = htFilter.clone()
htPFFilter.HTSource = cms.InputTag("htPF")

# filter on PFchsHT
htPFchsFilter = htFilter.clone()
htPFchsFilter.HTSource = cms.InputTag("htPFchs")
