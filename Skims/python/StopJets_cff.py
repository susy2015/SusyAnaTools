
from SusyAnaTools.Skims.basicJetSelector_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi import *

# PFJets (with CHS)
stopJetsPFchsPt30     = selectedPatJets.clone()
stopJetsPFchsPt30.jetSrc = cms.InputTag('patJetsAK5PFCHS')
stopJetsPFchsPt30.cut = cms.string('pt > 30')

stopJetsPFchsPt70Eta24 = selectedPatJets.clone()
stopJetsPFchsPt70Eta24.jetSrc = cms.InputTag("patJetsAK5PFCHS")
stopJetsPFchsPt70Eta24.cut = cms.string("pt>70 && abs(eta)<2.4")

stopJetsPFchsPt50Eta24 = selectedPatJets.clone()
stopJetsPFchsPt50Eta24.jetSrc = cms.InputTag("patJetsAK5PFCHS")
stopJetsPFchsPt50Eta24.cut = cms.string("pt>50 && abs(eta)<2.4")

stopJetsPFchsPt30Eta24 = selectedPatJets.clone()
stopJetsPFchsPt30Eta24.jetSrc = cms.InputTag("patJetsAK5PFCHS")
stopJetsPFchsPt30Eta24.cut = cms.string("pt>30 && abs(eta)<2.4")

stopJetsPFchsPt70eta2p5 = selectedPatJets.clone()
stopJetsPFchsPt70eta2p5.jetSrc = cms.InputTag('patJetsAK5PFCHS')
stopJetsPFchsPt70eta2p5.cut = cms.string('pt > 70 & abs(eta) < 2.5')

#count jets
stopCountPFchsJetsPt30           = countPatJets.clone()
stopCountPFchsJetsPt30.src       = cms.InputTag('stopJetsPFchsPt30')
stopCountPFchsJetsPt30.minNumber = cms.uint32(6)

count2JetsPFchsPt70Eta24Std = countPatJets.clone()
count2JetsPFchsPt70Eta24Std.src = cms.InputTag("stopJetsPFchsPt70Eta24")
count2JetsPFchsPt70Eta24Std.minNumber = cms.uint32(2)

count4JetsPFchsPt50Eta24Std = countPatJets.clone()
count4JetsPFchsPt50Eta24Std.src = cms.InputTag("stopJetsPFchsPt50Eta24")
count4JetsPFchsPt50Eta24Std.minNumber = cms.uint32(4)

count5JetsPFchsPt30Eta24Std = countPatJets.clone()
count5JetsPFchsPt30Eta24Std.src = cms.InputTag("stopJetsPFchsPt30Eta24")
count5JetsPFchsPt30Eta24Std.minNumber = cms.uint32(5)

count5JetsPFchsPt30Std = countPatJets.clone()
count5JetsPFchsPt30Std.src = cms.InputTag("stopJetsPFchsPt30")
count5JetsPFchsPt30Std.minNumber = cms.uint32(5)

stopCountPFchsJetsPt70eta2p5           = countPatJets.clone()
stopCountPFchsJetsPt70eta2p5.src       = cms.InputTag('stopJetsPFchsPt70eta2p5')
stopCountPFchsJetsPt70eta2p5.minNumber = cms.uint32(2)


# need to have counter for leading 2 jets with pT>70 GeV & |eta|<2.5


# should we have a counter for b-jets too ?


stopPFJets = cms.Sequence(
  stopJetsPFchsPt30 * stopJetsPFchsPt30Eta24 * stopJetsPFchsPt50Eta24 * stopJetsPFchsPt70Eta24 *
  stopJetsPFchsPt70eta2p5
)
