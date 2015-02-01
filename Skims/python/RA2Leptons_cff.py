from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *

# muon filters
countPFMuonsID = countPatMuons.clone()
countPFMuonsID.src = cms.InputTag('selectedMuons')
countPFMuonsID.minNumber = cms.uint32(1)

countPFMuonsIDIso = countPatMuons.clone()
countPFMuonsIDIso.src = cms.InputTag('selectedMuons')
countPFMuonsIDIso.minNumber = cms.uint32(1)

# muon sequences

#ra2PFMuons = cms.Sequence(
#  patMuonsPFID *
#  patMuonsPFIDIso
#)
ra2PFMuonVeto = cms.Sequence(
  ~countPFMuonsIDIso
)

from PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi import *

countPFElectronsID = countPatElectrons.clone()
countPFElectronsID.src = cms.InputTag('selectedElectrons')
countPFElectronsID.minNumber = cms.uint32(1)

countPFElectronsIDIso = countPatElectrons.clone()
countPFElectronsIDIso.src = cms.InputTag('selectedElectrons')
countPFElectronsIDIso.minNumber = cms.uint32(1)

#ra2Electrons = cms.Sequence(
#  kt6PFJetsForIsolation *
#  patElectronsID *
#  patElectronsIDIso
#)

ra2ElectronVeto = cms.Sequence(
  ~countElectronsIDIso
)
