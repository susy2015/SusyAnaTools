
from SusyAnaTools.Skims.dPhiFilter_cfi import *

jetsMETDPhiFilter = dPhiFilter.clone()
jetsMETDPhiFilter.jetSrc = cms.InputTag("patJetsPF")
jetsMETDPhiFilter.metSrc = cms.InputTag("patMETsPF")
