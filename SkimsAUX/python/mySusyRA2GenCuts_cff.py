import FWCore.ParameterSet.Config as cms

from UserCode.SkimsRA2.mySusyRA2Cuts_cff import *

myGenJetsAK5Pt50Eta25noOverlapMuons = overlappingCleanedJets.clone()
myGenJetsAK5Pt50Eta25noOverlapMuons.muonSource = cms.InputTag("patMuonsPFIDIso")
myGenJetsAK5Pt50Eta25noOverlapMuons.eleSource = cms.InputTag("patElectronsIDIso")
myGenJetsAK5Pt50Eta25noOverlapMuons.jetSource = cms.InputTag("patJetsPFchsPt50Eta25")
myGenJetsAK5Pt50Eta25noOverlapMuons.genJetSrc = cms.InputTag("ak5GenJets")
myGenJetsAK5Pt50Eta25noOverlapMuons.genParticleSrc = cms.InputTag("genParticles")
myGenJetsAK5Pt50Eta25noOverlapMuons.doGenLevel = cms.bool(True)
myGenJetsAK5Pt50Eta25noOverlapMuons.genJetMinPt = cms.double(50)
myGenJetsAK5Pt50Eta25noOverlapMuons.genJetMaxEta = cms.double(2.5)
myGenJetsAK5Pt50Eta25noOverlapMuons.deltaR = cms.double(0.5)

myGenJetsAK5Pt30noOverlapMuons = overlappingCleanedJets.clone()
myGenJetsAK5Pt30noOverlapMuons.muonSource = cms.InputTag("patMuonsPFIDIso")
myGenJetsAK5Pt30noOverlapMuons.eleSource = cms.InputTag("patElectronsIDIso")
myGenJetsAK5Pt30noOverlapMuons.jetSource = cms.InputTag("patJetsPFchsPt30")
myGenJetsAK5Pt30noOverlapMuons.genJetSrc = cms.InputTag("ak5GenJets")
myGenJetsAK5Pt30noOverlapMuons.genParticleSrc = cms.InputTag("genParticles")
myGenJetsAK5Pt30noOverlapMuons.doGenLevel = cms.bool(True)
myGenJetsAK5Pt30noOverlapMuons.genJetMinPt = cms.double(30)
myGenJetsAK5Pt30noOverlapMuons.genJetMaxEta = cms.double(999999)
myGenJetsAK5Pt30noOverlapMuons.deltaR = cms.double(0.5)

myGenJetsAK5Pt50Eta25noOverlapLeptons = overlappingCleanedJets.clone()
myGenJetsAK5Pt50Eta25noOverlapLeptons.muonSource = cms.InputTag("patMuonsPFIDIso")
myGenJetsAK5Pt50Eta25noOverlapLeptons.eleSource = cms.InputTag("patElectronsIDIso")
myGenJetsAK5Pt50Eta25noOverlapLeptons.jetSource = cms.InputTag("patJetsPF")
myGenJetsAK5Pt50Eta25noOverlapLeptons.genJetSrc = cms.InputTag("ak5GenJets")
myGenJetsAK5Pt50Eta25noOverlapLeptons.genParticleSrc = cms.InputTag("genParticles")
myGenJetsAK5Pt50Eta25noOverlapLeptons.doGenLevel = cms.bool(True)
myGenJetsAK5Pt50Eta25noOverlapLeptons.genJetMinPt = cms.double(50)
myGenJetsAK5Pt50Eta25noOverlapLeptons.genJetMaxEta = cms.double(2.5)
myGenJetsAK5Pt50Eta25noOverlapLeptons.deltaR = cms.double(0.5)
myGenJetsAK5Pt50Eta25noOverlapLeptons.useEle = cms.bool(True)
myGenJetsAK5Pt50Eta25noOverlapLeptons.recoJetMinPt = cms.double(50)
myGenJetsAK5Pt50Eta25noOverlapLeptons.recoJetMaxEta = cms.double(2.5)

myGenJetsAK5Pt30noOverlapLeptons = overlappingCleanedJets.clone()
myGenJetsAK5Pt30noOverlapLeptons.muonSource = cms.InputTag("patMuonsPFIDIso")
myGenJetsAK5Pt30noOverlapLeptons.eleSource = cms.InputTag("patElectronsIDIso")
myGenJetsAK5Pt30noOverlapLeptons.jetSource = cms.InputTag("patJetsPF")
myGenJetsAK5Pt30noOverlapLeptons.genJetSrc = cms.InputTag("ak5GenJets")
myGenJetsAK5Pt30noOverlapLeptons.genParticleSrc = cms.InputTag("genParticles")
myGenJetsAK5Pt30noOverlapLeptons.doGenLevel = cms.bool(True)
myGenJetsAK5Pt30noOverlapLeptons.genJetMinPt = cms.double(30)
myGenJetsAK5Pt30noOverlapLeptons.genJetMaxEta = cms.double(999999)
myGenJetsAK5Pt30noOverlapLeptons.deltaR = cms.double(0.5)
myGenJetsAK5Pt30noOverlapLeptons.useEle = cms.bool(True)
myGenJetsAK5Pt30noOverlapLeptons.recoJetMinPt = cms.double(30)
myGenJetsAK5Pt30noOverlapLeptons.recoJetMaxEta = cms.double(-1)

from UserCode.SkimsRA2.genJetCounterFilter_cfi import *

count1GenJets = genJetCounterFilter.clone()
count1GenJets.genJetSrc = cms.InputTag("myGenJetsAK5Pt50Eta25noOverlapLeptons:cleanGenJets")
count1GenJets.minJetNum = cms.int32(1)

count2GenJets = genJetCounterFilter.clone()
count2GenJets.genJetSrc = cms.InputTag("myGenJetsAK5Pt50Eta25noOverlapLeptons:cleanGenJets")
count2GenJets.minJetNum = cms.int32(2)

count3GenJets = genJetCounterFilter.clone()
count3GenJets.genJetSrc = cms.InputTag("myGenJetsAK5Pt50Eta25noOverlapLeptons:cleanGenJets")
count3GenJets.minJetNum = cms.int32(3)

#Calculate MHT
mymhtGenPF = mhtPF.clone()
mymhtGenPF.JetCollection = cms.InputTag("myGenJetsAK5Pt30noOverlapLeptons:cleanGenJets")
mymhtGenPFFilter = mhtPFFilter.clone()
mymhtGenPFFilter.MHTSource = cms.InputTag("mymhtGenPF")
#mymhtGenPFFilter.MinMHT = cms.double(150)

#Calculate HT
myhtGenPF = htPF.clone()
myhtGenPF.JetCollection = cms.InputTag("myGenJetsAK5Pt50Eta25noOverlapLeptons:cleanGenJets")
myhtGenPFFilter = htPFFilter.clone()
myhtGenPFFilter.HTSource = cms.InputTag("myhtGenPF")
#myhtGenPFFilter.MinHT = cms.double(300)

#Load Dphi filter
myjetMHTDPhiGenFilter = jetMHTDPhiFilter.clone()
myjetMHTDPhiGenFilter.MHTSource = cms.InputTag("mymhtGenPF")
myjetMHTDPhiGenFilter.JetSource = cms.InputTag("myGenJetsAK5Pt30noOverlapLeptons:cleanGenJets")
