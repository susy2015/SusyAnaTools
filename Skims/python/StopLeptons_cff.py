from SusyAnaTools.Skims.muonSelector_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *

from SusyAnaTools.Skims.electronSelector_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi import *

from SusyAnaTools.Skims.sakLooseMuonSelector_cfi import *
from SusyAnaTools.Skims.sakLooseElectronSelector_cfi import *

stoppatMuonsPFID = muonSelector.clone()
stoppatMuonsPFID.MuonSource = cms.InputTag("patMuonsPF")
stoppatMuonsPFID.MaxMuRelIso = 0.20
stoppatMuonsPFID.DoMuonIsolation = False
stoppatMuonsPFID.MinMuPt = 5

stopsakLooseMuonSelector = sakLooseMuonSelector.clone()
stopsakLooseMuonSelector.MuonSource = cms.InputTag("stoppatMuonsPFID")
stopsakLooseMuonSelector.DoMuonVeto = cms.bool(False)
stopsakLooseMuonSelector.DoMuonIso  = cms.bool(True)
stopsakLooseMuonSelector.DoMuonID   = cms.bool(False)

#from RecoJets.JetProducers.kt4PFJets_cfi import *
#kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
#kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)

stoppatElectronsID = electronSelector.clone()
stoppatElectronsID.DoElectronIsolation = False
stoppatElectronsID.MinElePt = 5

stopsakLooseElectronSelector = sakLooseElectronSelector.clone()
stopsakLooseElectronSelector.ElectronSource = cms.InputTag("stoppatElectronsID")
stopsakLooseElectronSelector.DoElectronVeto = cms.bool(False)
stopsakLooseElectronSelector.DoElectronIso  = cms.bool(True)
stopsakLooseElectronSelector.DoElectronID   = cms.bool(False)

stopPFMuons = cms.Sequence(
   stoppatMuonsPFID * stopsakLooseMuonSelector
)

stopElectrons = cms.Sequence(
   stoppatElectronsID * stopsakLooseElectronSelector
)

stopcountPFMuonsIDIso = countPatMuons.clone()
stopcountPFMuonsIDIso.src = cms.InputTag('stopsakLooseMuonSelector')
stopcountPFMuonsIDIso.minNumber = cms.uint32(1)

stopcountElectronsIDIso = countPatElectrons.clone()
stopcountElectronsIDIso.src = cms.InputTag('stopsakLooseElectronSelector')
stopcountElectronsIDIso.minNumber = cms.uint32(1)

stopPFMuonVeto = cms.Sequence(
  ~stopcountPFMuonsIDIso
)

stopElectronVeto = cms.Sequence(
  ~stopcountElectronsIDIso
)
