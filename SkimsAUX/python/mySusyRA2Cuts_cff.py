import FWCore.ParameterSet.Config as cms

# All PF leptons
from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi import *

from UserCode.SkimsRA2.goldenMuonSelector_cfi import goldenMuonSelector
from UserCode.SkimsRA2.goldenElectronSelector_cfi import goldenElectronSelector

from SusyAnaTools.Skims.electronSelector_cfi import *

# Standard veto leptons re-defined by the golden selectors
# They have to work on PATtuples!
# One can make them work on AOD easily by disabling the following lines
# Or put the provInfo producers in front of them.
goldenMuonSelector.inputProductIDSrc = cms.InputTag("provInfoMuons:muonPFbaseProductID")
goldenMuonSelector.inputKeySrc = cms.InputTag("provInfoMuons:muonPFbaseKey")

goldenElectronSelector.inputProductIDSrc = cms.InputTag("provInfoElectrons:elePFbaseProductID")
goldenElectronSelector.inputKeySrc = cms.InputTag("provInfoElectrons:elePFbaseKey")

patMuonsPFID = goldenMuonSelector.clone()
patMuonsPFID.MuonSource = cms.InputTag("patMuonsPF")
patMuonsPFID.MaxMuRelIso = 0.20
patMuonsPFID.DoMuonIsolation = False

patMuonsPFIDIso = goldenMuonSelector.clone()
patMuonsPFIDIso.MuonSource = cms.InputTag("patMuonsPF")
patMuonsPFIDIso.MaxMuRelIso = 0.20

countPFMuonsID = countPatMuons.clone()
countPFMuonsID.src = cms.InputTag('patMuonsPFID')
countPFMuonsID.minNumber = cms.uint32(1)

countPFMuonsIDIso = countPatMuons.clone()
countPFMuonsIDIso.src = cms.InputTag('patMuonsPFIDIso')
countPFMuonsIDIso.minNumber = cms.uint32(1)

patElectronsPFID = goldenElectronSelector.clone()
patElectronsPFID.ElectronSource = cms.InputTag("patElectronsPF")
patElectronsPFID.MaxEleRelIso = 0.20
patElectronsPFID.DoElectronIsolation = False

patElectronsPFIDIso = goldenElectronSelector.clone()
patElectronsPFIDIso.ElectronSource = cms.InputTag("patElectronsPF")
patElectronsPFIDIso.MaxEleRelIso = 0.20

countPFElectronsID = countPatElectrons.clone()
countPFElectronsID.src = cms.InputTag('patElectronsPFID')
countPFElectronsID.minNumber = cms.uint32(1)

countPFElectronsIDIso = countPatElectrons.clone()
countPFElectronsIDIso.src = cms.InputTag('patElectronsPFIDIso')
countPFElectronsIDIso.minNumber = cms.uint32(1)

from RecoJets.JetProducers.kt4PFJets_cfi import *
kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)

# electrons selectors
patElectronsIDIso = electronSelector.clone()

countElectronsIDIso = countPatElectrons.clone()
countElectronsIDIso.src = cms.InputTag('patElectronsIDIso')
countElectronsIDIso.minNumber = cms.uint32(1)

ra2PFMuons = cms.Sequence(
  patMuonsPFID *
  patMuonsPFIDIso
)
ra2PFMuonVeto = cms.Sequence(
  ~countPFMuonsIDIso
)

ra2PFElectrons = cms.Sequence(
  patElectronsPFID *
  patElectronsPFIDIso
)
ra2PFElectronVeto = cms.Sequence(
  ~countPFElectronsIDIso
)

ra2Electrons = cms.Sequence(
  kt6PFJetsForIsolation *
  patElectronsIDIso
)
ra2ElectronVeto = cms.Sequence(
  ~countElectronsIDIso
)

#Load lepton vetos
# XXX: the counter method is used
#leptonVetoPOGnoMu = cms.Sequence( ra2PFElectronVeto )
#leptonVetoPOGbothEleANDMu = cms.Sequence( ra2PFElectronVeto * ra2PFMuonVeto )
leptonVetoPOGnoMu = cms.Sequence( ra2ElectronVeto )
leptonVetoPOGbothEleANDMu = cms.Sequence( ra2ElectronVeto * ra2PFMuonVeto )

# Tigher muons using goldenMuonSelector
myPatMuonsPFIDIso = goldenMuonSelector.clone()
myPatMuonsPFIDIso.MuonSource = cms.InputTag('patMuonsPF')
myPatMuonsPFIDIso.DoMuonIsolation = True
myPatMuonsPFIDIso.DoMuonID = True 
myPatMuonsPFIDIso.DoMuonVtxAssociation = True 
myPatMuonsPFIDIso.MinMuPt = cms.double(20) # pt cut
myPatMuonsPFIDIso.MaxMuEta = cms.double(2.1) # eta cut
myPatMuonsPFIDIso.MaxMuD0 = cms.double(0.2) # d_xy cut
myPatMuonsPFIDIso.MaxMuDz = cms.double(0.5)
myPatMuonsPFIDIso.MinMuNumHit = cms.double(11)
myPatMuonsPFIDIso.MaxMuRelIso = cms.double(0.20) # iso cut
myPatMuonsPFIDIso.DoMuonProvInfo = False

countMyPatMuonsPFIDIsoFilter = countPatMuons.clone()
countMyPatMuonsPFIDIsoFilter.src = cms.InputTag('myPatMuonsPFIDIso')
#require exact one ID'd and Isolated muon
countMyPatMuonsPFIDIsoFilter.minNumber = cms.uint32(1)
countMyPatMuonsPFIDIsoFilter.maxNumber = cms.uint32(1)

count2MyPatMuonsPFIDIsoFilter = countPatMuons.clone()
count2MyPatMuonsPFIDIsoFilter.src = cms.InputTag('myPatMuonsPFIDIso')
#require exact one ID'd and Isolated muon
count2MyPatMuonsPFIDIsoFilter.minNumber = cms.uint32(2)
count2MyPatMuonsPFIDIsoFilter.maxNumber = cms.uint32(2)

#Jet Filter
#from SusyAnaTools.Skims.RA2Jets_cff import * 
from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi import *

from UserCode.SkimsRA2.simpleJetSelector_cfi import *
patJetsPFchsPt30 = simpleJetSelector.clone() 
patJetsPFchsPt30.jetSrc = cms.InputTag("patJetsPF")
patJetsPFchsPt30.pfJetCut = cms.string("pt>30")

patJetsPFchsPt70 = simpleJetSelector.clone() 
patJetsPFchsPt70.jetSrc = cms.InputTag("patJetsPF")
patJetsPFchsPt70.pfJetCut = cms.string("pt>70")

patJetsPFchsPt30Eta25 = simpleJetSelector.clone() 
patJetsPFchsPt30Eta25.jetSrc = cms.InputTag("patJetsPF")
patJetsPFchsPt30Eta25.pfJetCut = cms.string("pt>30 && abs(eta)<2.5")

patJetsPFchsPt70Eta25 = simpleJetSelector.clone() 
patJetsPFchsPt70Eta25.jetSrc = cms.InputTag("patJetsPF")
patJetsPFchsPt70Eta25.pfJetCut = cms.string("pt>70 && abs(eta)<2.5")

patJetsPFchsPt50Eta25 = simpleJetSelector.clone()
patJetsPFchsPt50Eta25.jetSrc = cms.InputTag("patJetsPF")
patJetsPFchsPt50Eta25.pfJetCut = cms.string("pt>50 && abs(eta)<2.5")

myPatJetsAK5PFPt30Eta25 = selectedPatJets.clone()
myPatJetsAK5PFPt30Eta25.src = cms.InputTag('patJetsAK5PFPt30')
myPatJetsAK5PFPt30Eta25.cut = cms.string('abs(eta) < 2.5')

myPatJetsPFchsPt30Eta25 = simpleJetSelector.clone()
myPatJetsPFchsPt30Eta25.src = cms.InputTag('patJetsPFchsPt30')
myPatJetsPFchsPt30Eta25.cut = cms.string('abs(eta) < 2.5')

from UserCode.SkimsRA2.overlappingCleanedJets_cfi import *
#Check for overlaps between jets and leptons (muons and electrons)
myPatJetsAK5PFPt50Eta25noOverlapLeptons = overlappingCleanedJets.clone()
myPatJetsAK5PFPt50Eta25noOverlapLeptons.muonSource = cms.InputTag("patMuonsPFIDIso")
myPatJetsAK5PFPt50Eta25noOverlapLeptons.eleSource = cms.InputTag("patElectronsPFIDIso")
myPatJetsAK5PFPt50Eta25noOverlapLeptons.jetSource = cms.InputTag("patJetsAK5PF")
myPatJetsAK5PFPt50Eta25noOverlapLeptons.recoJetMinPt = cms.double(50)
myPatJetsAK5PFPt50Eta25noOverlapLeptons.recoJetMaxEta = cms.double(2.5)

myPatJetsAK5PFPt30noOverlapLeptons = overlappingCleanedJets.clone()
myPatJetsAK5PFPt30noOverlapLeptons.muonSource = cms.InputTag("patMuonsPFIDIso")
myPatJetsAK5PFPt30noOverlapLeptons.eleSource = cms.InputTag("patElectronsPFIDIso")
myPatJetsAK5PFPt30noOverlapLeptons.jetSource = cms.InputTag("patJetsAK5PF")
myPatJetsAK5PFPt30noOverlapLeptons.recoJetMinPt = cms.double(30)
myPatJetsAK5PFPt30noOverlapLeptons.recoJetMaxEta = cms.double(-1)

myPatJetsAK5PFnoOverlapLeptons = overlappingCleanedJets.clone()
myPatJetsAK5PFnoOverlapLeptons.muonSource = cms.InputTag("patMuonsPFIDIso")
myPatJetsAK5PFnoOverlapLeptons.eleSource = cms.InputTag("patElectronsPFIDIso")
myPatJetsAK5PFnoOverlapLeptons.jetSource = cms.InputTag("patJetsAK5PF")
myPatJetsAK5PFnoOverlapLeptons.recoJetMinPt = cms.double(-1)
myPatJetsAK5PFnoOverlapLeptons.recoJetMaxEta = cms.double(-1)
myPatJetsAK5PFnoOverlapLeptons.verbose = cms.bool(True)

myPatJetsAK5PFPt30Eta25noOverlapLeptons = overlappingCleanedJets.clone()
myPatJetsAK5PFPt30Eta25noOverlapLeptons.muonSource = cms.InputTag("patMuonsPFIDIso")
myPatJetsAK5PFPt30Eta25noOverlapLeptons.eleSource = cms.InputTag("patElectronsPFIDIso")
myPatJetsAK5PFPt30Eta25noOverlapLeptons.jetSource = cms.InputTag("patJetsAK5PF")
myPatJetsAK5PFPt30Eta25noOverlapLeptons.recoJetMinPt = cms.double(30)
myPatJetsAK5PFPt30Eta25noOverlapLeptons.recoJetMaxEta = cms.double(2.5)

# Use chs jets
print "For chs jets, have to use deltaR cleaning method... temporarily!"
overlappingCleanedJets.useDeltaR = cms.bool(True)

myPatJetsPFchsPt50Eta25noOverlapLeptons = overlappingCleanedJets.clone()
myPatJetsPFchsPt50Eta25noOverlapLeptons.muonSource = cms.InputTag("patMuonsPFIDIso")
myPatJetsPFchsPt50Eta25noOverlapLeptons.eleSource = cms.InputTag("patElectronsIDIso")
myPatJetsPFchsPt50Eta25noOverlapLeptons.jetSource = cms.InputTag("patJetsPF")
myPatJetsPFchsPt50Eta25noOverlapLeptons.recoJetMinPt = cms.double(50)
myPatJetsPFchsPt50Eta25noOverlapLeptons.recoJetMaxEta = cms.double(2.5)

myPatJetsPFchsPt30noOverlapLeptons = overlappingCleanedJets.clone()
myPatJetsPFchsPt30noOverlapLeptons.muonSource = cms.InputTag("patMuonsPFIDIso")
myPatJetsPFchsPt30noOverlapLeptons.eleSource = cms.InputTag("patElectronsIDIso")
myPatJetsPFchsPt30noOverlapLeptons.jetSource = cms.InputTag("patJetsPF")
myPatJetsPFchsPt30noOverlapLeptons.recoJetMinPt = cms.double(30)
myPatJetsPFchsPt30noOverlapLeptons.recoJetMaxEta = cms.double(-1)

myPatJetsPFchsnoOverlapLeptons = overlappingCleanedJets.clone()
myPatJetsPFchsnoOverlapLeptons.muonSource = cms.InputTag("patMuonsPFIDIso")
myPatJetsPFchsnoOverlapLeptons.eleSource = cms.InputTag("patElectronsIDIso")
myPatJetsPFchsnoOverlapLeptons.jetSource = cms.InputTag("patJetsPF")
myPatJetsPFchsnoOverlapLeptons.recoJetMinPt = cms.double(-1)
myPatJetsPFchsnoOverlapLeptons.recoJetMaxEta = cms.double(-1)
myPatJetsPFchsnoOverlapLeptons.verbose = cms.bool(True)

myPatJetsPFchsPt30Eta25noOverlapLeptons = overlappingCleanedJets.clone()
myPatJetsPFchsPt30Eta25noOverlapLeptons.muonSource = cms.InputTag("patMuonsPFIDIso")
myPatJetsPFchsPt30Eta25noOverlapLeptons.eleSource = cms.InputTag("patElectronsIDIso")
myPatJetsPFchsPt30Eta25noOverlapLeptons.jetSource = cms.InputTag("patJetsPF")
myPatJetsPFchsPt30Eta25noOverlapLeptons.recoJetMinPt = cms.double(30)
myPatJetsPFchsPt30Eta25noOverlapLeptons.recoJetMaxEta = cms.double(2.5)


#filterEvtNonIsoMuonMatchToGenMuon
from UserCode.SkimsRA2.filterEvtNonIsoMuonMatchToGenMuon_cfi import *
myFilterEvtNonIsoMuonMatchToGenMuon = filterEvtNonIsoMuonMatchToGenMuon.clone()
myFilterEvtNonIsoMuonMatchToGenMuon.muonSource  = cms.InputTag("patMuonsPF")
myFilterEvtNonIsoMuonMatchToGenMuon.deltaR      = cms.double(0.3)
myFilterEvtNonIsoMuonMatchToGenMuon.maxMuRelIso = cms.double(0.20)
myFilterEvtNonIsoMuonMatchToGenMuon.doFilter    = cms.bool(False)

#Calculate MHT
from SusyAnaTools.Skims.RA2MHT_cff import *
mymhtPF = mhtPF.clone()
mymhtPFFilter = mhtPFFilter.clone()
mymhtPFFilter.MHTSource = cms.InputTag("mymhtPF")
#mymhtPFFilter.MinMHT = cms.double(150)

mymhtPFchs = mhtPFchs.clone()
mymhtPFchsFilter = mhtPFchsFilter.clone()
mymhtPFchsFilter.MHTSource = cms.InputTag("mymhtPFchs")

#Calculate HT
from SusyAnaTools.Skims.RA2HT_cff import *
myhtPF = htPF.clone()
myhtPFFilter = htPFFilter.clone()
myhtPFFilter.HTSource = cms.InputTag("myhtPF")
#myhtPFFilter.MinHT = cms.double(300)

myhtPFchs = htPFchs.clone()
myhtPFchsFilter = htPFchsFilter.clone()
myhtPFchsFilter.HTSource = cms.InputTag("myhtPFchs")

#Load Dphi filter
from SusyAnaTools.Skims.jetMHTDPhiFilter_cfi import *
myjetMHTDPhiFilter = jetMHTDPhiFilter.clone()
myjetMHTDPhiFilter.MHTSource = cms.InputTag("mymhtPF")
myjetMHTDPhiFilter.JetSource = cms.InputTag("patJetsAK5PFPt30")

myjetMHTchsDPhiFilter = jetMHTDPhiFilter.clone()
myjetMHTchsDPhiFilter.MHTSource = cms.InputTag("mymhtPFchs")
myjetMHTchsDPhiFilter.JetSource = cms.InputTag("patJetsPFchsPt30")
