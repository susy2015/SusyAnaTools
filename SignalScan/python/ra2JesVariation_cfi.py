import FWCore.ParameterSet.Config as cms

from UserCode.SignalScan.jesVariation_cfi import *

# make RA2 sequences
from SandBox.Skims.RA2Objects_cff import *
from SandBox.Skims.RA2Selection_cff import *

# sequence JES UP variation
jesUpJetsAK5PFPt30 = cms.EDFilter("PATJetSelector",
                                src = cms.InputTag("jesUpJets"),
                                cut = cms.string("pt > 30")
                                )

jesUpJetsAK5PFPt50Eta25 = cms.EDFilter("PATJetSelector",
                                     src = cms.InputTag("jesUpJets"),
                                     cut = cms.string('pt > 50 & abs(eta) < 2.5')
                                     )

htJesUpJets                = htPF.clone()
htJesUpJets.JetCollection  = cms.InputTag("jesUpJetsAK5PFPt50Eta25")

mhtJesUpJets               = mhtPF.clone()
mhtJesUpJets.JetCollection = cms.InputTag("jesUpJetsAK5PFPt30")

countJesUpJetsAK5PFPt50Eta25           = countPatJets.clone()
countJesUpJetsAK5PFPt50Eta25.src       = cms.InputTag('jesUpJetsAK5PFPt50Eta25')
countJesUpJetsAK5PFPt50Eta25.minNumber = cms.uint32(3)

jesUpJetsMHTPFDPhiFilter = jetMHTPFDPhiFilter.clone()
jesUpJetsMHTPFDPhiFilter.JetSource = cms.InputTag('jesUpJetsAK5PFPt30')
jesUpJetsMHTPFDPhiFilter.MHTSource = cms.InputTag("mhtJesUpJets")

htJesUpPFFilter            = htFilter.clone()
htJesUpPFFilter.HTSource   = cms.InputTag("htJesUpJets")
htJesUpPFFilter.MinHT      = cms.double(350.0)

mhtJesUpFilter             = mhtPFFilter.clone()
mhtJesUpFilter.MHTSource   = cms.InputTag("mhtJesUpJets")
mhtJesUpFilter.MinMHT      = cms.double(200.0)

jesUpSequence              = cms.Sequence(jesUpJets *
                                          jesUpJetsAK5PFPt30 *
                                          jesUpJetsAK5PFPt50Eta25 *
                                          htJesUpJets *
                                          mhtJesUpJets *
                                          htJesUpPFFilter *
                                          mhtJesUpFilter *
                                          countJesUpJetsAK5PFPt50Eta25 *
                                          jesUpJetsMHTPFDPhiFilter 
                                          )


# sequence JES DN variation
jesDnJetsAK5PFPt30 = cms.EDFilter("PATJetSelector",
                                src = cms.InputTag("jesDnJets"),
                                cut = cms.string("pt > 30")
                                )

jesDnJetsAK5PFPt50Eta25 = cms.EDFilter("PATJetSelector",
                                     src = cms.InputTag("jesDnJets"),
                                     cut = cms.string('pt > 50 & abs(eta) < 2.5')
                                     )

htJesDnJets                = htPF.clone()
htJesDnJets.JetCollection  = cms.InputTag("jesDnJetsAK5PFPt50Eta25")

mhtJesDnJets               = mhtPF.clone()
mhtJesDnJets.JetCollection = cms.InputTag("jesDnJetsAK5PFPt30")

countJesDnJetsAK5PFPt50Eta25           = countPatJets.clone()
countJesDnJetsAK5PFPt50Eta25.src       = cms.InputTag('jesDnJetsAK5PFPt50Eta25')
countJesDnJetsAK5PFPt50Eta25.minNumber = cms.uint32(3)

jesDnJetsMHTPFDPhiFilter = jetMHTPFDPhiFilter.clone()
jesDnJetsMHTPFDPhiFilter.JetSource = cms.InputTag('jesDnJetsAK5PFPt30')
jesDnJetsMHTPFDPhiFilter.MHTSource = cms.InputTag("mhtJesDnJets")

htJesDnPFFilter            = htFilter.clone()
htJesDnPFFilter.HTSource   = cms.InputTag("htJesDnJets")
htJesDnPFFilter.MinHT      = cms.double(350.0)

mhtJesDnFilter             = mhtPFFilter.clone()
mhtJesDnFilter.MHTSource   = cms.InputTag("mhtJesDnJets")
mhtJesDnFilter.MinMHT      = cms.double(200.0)

jesDnSequence              = cms.Sequence(jesDnJets *
                                          jesDnJetsAK5PFPt30 *
                                          jesDnJetsAK5PFPt50Eta25 *
                                          htJesDnJets *
                                          mhtJesDnJets *
                                          htJesDnPFFilter *
                                          mhtJesDnFilter *
                                          countJesDnJetsAK5PFPt50Eta25 *
                                          jesDnJetsMHTPFDPhiFilter 
                                          )

