
from RecoMET.METFilters.jetIDFailureFilter_cfi import *

ra2PBNR = jetIDFailure.clone()
ra2PBNR.JetSource         = cms.InputTag("patJetsPFchsPt30")
ra2PBNR.MinJetPt          = 30.0
ra2PBNR.MaxJetEta         = 999.0
ra2PBNR.MaxNeutralHadFrac = 0.90
ra2PBNR.MaxNeutralEMFrac  = 0.95
