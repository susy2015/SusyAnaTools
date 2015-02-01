
from SusyAnaTools.Skims.basicJetSelector_drt_from_AOD_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi import *

# PFJets (default)
patJetsAK4PFPt30     = selectedPatJetsRA2.clone()
patJetsAK4PFPt30.src = cms.InputTag('slimmedJets')
patJetsAK4PFPt30.cut = cms.string('pt > 30')

patJetsAK4PFPt50Eta25     = selectedPatJetsRA2.clone()
patJetsAK4PFPt50Eta25.src = cms.InputTag('patJetsAK4PFPt30')
patJetsAK4PFPt50Eta25.cut = cms.string('pt > 50 & abs(eta) < 2.5')

# PFJets - filters
countJetsAK4PFPt50Eta25           = countPatJets.clone()
countJetsAK4PFPt50Eta25.src       = cms.InputTag('patJetsAK4PFPt50Eta25')
countJetsAK4PFPt50Eta25.minNumber = cms.uint32(3)

# PF sequences
ra2PFJets = cms.Sequence(
  patJetsAK4PFPt30 *
  patJetsAK4PFPt50Eta25
)


# PFJets (with CHS)
patJetsPFchsPt30     = selectedPatJetsRA2.clone()
patJetsPFchsPt30.src = cms.InputTag('slimmedJets')
patJetsPFchsPt30.cut = cms.string('pt > 30')

patJetsPFchsPt50Eta25     = selectedPatJetsRA2.clone()
patJetsPFchsPt50Eta25.src = cms.InputTag('patJetsPFchsPt30')
patJetsPFchsPt50Eta25.cut = cms.string('pt > 50 & abs(eta) < 2.5')

# PFJets - filters
countJetsPFchsPt50Eta25           = countPatJets.clone()
countJetsPFchsPt50Eta25.src       = cms.InputTag('patJetsPFchsPt50Eta25')
countJetsPFchsPt50Eta25.minNumber = cms.uint32(3)

# PF sequences
ra2PFchsJets = cms.Sequence(
  patJetsPFchsPt30 *
  patJetsPFchsPt50Eta25
)
