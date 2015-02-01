from SusyAnaTools.Skims.tauJetsSelector_cfi import *

tauJets = tauJetsSelector.clone()
tauJets.MetSrc    = cms.InputTag("patMETsPF")
tauJets.JetSrc    = cms.InputTag("patJetsPF")
tauJets.DoTauVeto = cms.bool(False)

tauJetsVeto = tauJets.clone()
tauJetsVeto.DoTauVeto = cms.bool(True)

stopTauJets = cms.Sequence(
   tauJets 
)    

stopTauJetVeto  = cms.Sequence(
  tauJetsVeto
)
