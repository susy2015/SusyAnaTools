
from SusyAnaTools.Skims.jetMHTDPhiFilter_cfi import *

jetMHTPFDPhiFilter = jetMHTDPhiFilter.clone()
jetMHTPFDPhiFilter.JetSource = cms.InputTag("patJetsAK5PFPt30")
jetMHTPFDPhiFilter.MHTSource = cms.InputTag("mhtPF")

jetMHTPFchsDPhiFilter = jetMHTDPhiFilter.clone()
jetMHTPFchsDPhiFilter.JetSource = cms.InputTag("patJetsPFchsPt30")
jetMHTPFchsDPhiFilter.MHTSource = cms.InputTag("mhtPFchs")
