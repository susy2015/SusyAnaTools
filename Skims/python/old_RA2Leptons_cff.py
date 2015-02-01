
from SusyAnaTools.Skims.muonSelector_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *

# muons selectors

patMuonsID = muonSelector.clone()
patMuonsID.MuonSource = cms.InputTag("patMuons")
patMuonsID.MaxMuRelIso = 0.20
patMuonsID.DoMuonIsolation = False

patMuonsIDIso = muonSelector.clone()
patMuonsIDIso.MaxMuRelIso = 0.20

patMuonsPFID = muonSelector.clone()
patMuonsPFID.MuonSource = cms.InputTag("patMuonsPF")
patMuonsPFID.MaxMuRelIso = 0.20
patMuonsPFID.DoMuonIsolation = False

patMuonsPFIDIso = muonSelector.clone()
patMuonsPFIDIso.MuonSource = cms.InputTag("patMuonsPF")
patMuonsPFIDIso.MaxMuRelIso = 0.20

# muon filters

countMuonsID = countPatMuons.clone()
countMuonsID.src = cms.InputTag('patMuonsID')
countMuonsID.minNumber = cms.uint32(1)

countMuonsIDIso = countPatMuons.clone()
countMuonsIDIso.src = cms.InputTag('patMuonsIDIso')
countMuonsIDIso.minNumber = cms.uint32(1)

countPFMuonsID = countPatMuons.clone()
countPFMuonsID.src = cms.InputTag('patMuonsPFID')
countPFMuonsID.minNumber = cms.uint32(1)

countPFMuonsIDIso = countPatMuons.clone()
countPFMuonsIDIso.src = cms.InputTag('patMuonsPFIDIso')
countPFMuonsIDIso.minNumber = cms.uint32(1)

# muon sequences

ra2Muons = cms.Sequence(
  patMuonsID *
  patMuonsIDIso
)
ra2PFMuons = cms.Sequence(
  patMuonsPFID *
  patMuonsPFIDIso
)
ra2MuonVeto = cms.Sequence(
  ~countMuonsIDIso
)
ra2PFMuonVeto = cms.Sequence(
  ~countPFMuonsIDIso
)

from SusyAnaTools.Skims.electronSelector_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi import *

from RecoJets.JetProducers.kt4PFJets_cfi import *
kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)

# electrons selectors
patElectronsID    = electronSelector.clone()
patElectronsID.DoElectronIsolation = False

patElectronsIDIso = electronSelector.clone()
patElectronsIDIso.DoElectronIsolation = True

countElectronsID = countPatElectrons.clone()
countElectronsID.src = cms.InputTag('patElectronsID')
countElectronsID.minNumber = cms.uint32(1)

countElectronsIDIso = countPatElectrons.clone()
countElectronsIDIso.src = cms.InputTag('patElectronsIDIso')
countElectronsIDIso.minNumber = cms.uint32(1)

ra2Electrons = cms.Sequence(
  kt6PFJetsForIsolation *
  patElectronsID *
  patElectronsIDIso
)

ra2ElectronVeto = cms.Sequence(
  ~countElectronsIDIso
)
