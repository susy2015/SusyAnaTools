
from SusyAnaTools.Skims.btagJetSelector_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi import *

#select b-tag jets
stopBJets        = btagJetsSelector.clone()
stopBJets.JetSrc = cms.InputTag('stopJetsPFchsPt30')

#count jets
stopCountBJets           = countPatJets.clone()
stopCountBJets.src       = cms.InputTag('stopBJets')
stopCountBJets.minNumber = cms.uint32(1)

