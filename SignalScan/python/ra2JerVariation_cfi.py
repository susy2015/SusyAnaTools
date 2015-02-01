import FWCore.ParameterSet.Config as cms

from UserCode.SignalScan.jerVariation_cfi import *

# make RA2 sequences
from SandBox.Skims.RA2Objects_cff import *
from SandBox.Skims.RA2Selection_cff import *

# sequence nominal JER variation
jerJetsAK5PFPt30 = cms.EDFilter("PATJetSelector",
                                src = cms.InputTag("jerNominal"),
                                cut = cms.string("pt > 30")
                                )

jerJetsAK5PFPt50Eta25 = cms.EDFilter("PATJetSelector",
                                     src = cms.InputTag("jerNominal"),
                                     cut = cms.string('pt > 50 & abs(eta) < 2.5')
                                     )

htJerJets                = htPF.clone()
htJerJets.JetCollection  = cms.InputTag("jerJetsAK5PFPt50Eta25")

mhtJerJets               = mhtPF.clone()
mhtJerJets.JetCollection = cms.InputTag("jerJetsAK5PFPt30")

countJerJetsAK5PFPt50Eta25           = countPatJets.clone()
countJerJetsAK5PFPt50Eta25.src       = cms.InputTag('jerJetsAK5PFPt50Eta25')
countJerJetsAK5PFPt50Eta25.minNumber = cms.uint32(3)

jerJetMHTPFDPhiFilter = jetMHTPFDPhiFilter.clone()
jerJetMHTPFDPhiFilter.JetSource = cms.InputTag('jerJetsAK5PFPt30')
jerJetMHTPFDPhiFilter.MHTSource = cms.InputTag("mhtJerJets")

htJerPFFilter            = htFilter.clone()
htJerPFFilter.HTSource   = cms.InputTag("htJerJets")
htJerPFFilter.MinHT      = cms.double(350.0)

mhtJerFilter             = mhtPFFilter.clone()
mhtJerFilter.MHTSource    = cms.InputTag("mhtJerJets")
mhtJerFilter.MinMHT      = cms.double(200.0)

jerNominalSequence = cms.Sequence(jerNominal *
                                  jerJetsAK5PFPt30 *
                                  jerJetsAK5PFPt50Eta25 *
                                  htJerJets *
                                  mhtJerJets *
                                  htJerPFFilter *
                                  mhtJerFilter * 
                                  countJerJetsAK5PFPt50Eta25 *
                                  jerJetMHTPFDPhiFilter
                                  )


# sequence core up JER variation
jerUpJetsAK5PFPt30 = cms.EDFilter("PATJetSelector",
                                  src = cms.InputTag("jerUpJets"),
                                  cut = cms.string("pt > 30")
                                  )

jerUpJetsAK5PFPt50Eta25 = cms.EDFilter("PATJetSelector",
                                       src = cms.InputTag("jerUpJets"),
                                       cut = cms.string('pt > 50 & abs(eta) < 2.5')
                                       )

htJerUpJets                = htPF.clone()
htJerUpJets.JetCollection  = cms.InputTag("jerUpJetsAK5PFPt50Eta25")

mhtJerUpJets               = mhtPF.clone()
mhtJerUpJets.JetCollection = cms.InputTag("jerUpJetsAK5PFPt30")

countJerUpJetsAK5PFPt50Eta25           = countPatJets.clone()
countJerUpJetsAK5PFPt50Eta25.src       = cms.InputTag('jerUpJetsAK5PFPt50Eta25')
countJerUpJetsAK5PFPt50Eta25.minNumber = cms.uint32(3)

jerUpJetsMHTPFDPhiFilter = jetMHTPFDPhiFilter.clone()
jerUpJetsMHTPFDPhiFilter.JetSource = cms.InputTag('jerUpJetsAK5PFPt30')
jerUpJetsMHTPFDPhiFilter.MHTSource = cms.InputTag("mhtJerUpJets")

htJerUpPFFilter            = htFilter.clone()
htJerUpPFFilter.HTSource   = cms.InputTag("htJerUpJets")
htJerUpPFFilter.MinHT      = cms.double(350.0)

mhtJerUpFilter             = mhtPFFilter.clone()
mhtJerUpFilter.MHTSource   = cms.InputTag("mhtJerUpJets")
mhtJerUpFilter.MinMHT      = cms.double(200.0)

jerUpSequence              = cms.Sequence(jerUpJets *
                                          jerUpJetsAK5PFPt30 *
                                          jerUpJetsAK5PFPt50Eta25 *
                                          htJerUpJets *
                                          mhtJerUpJets *
                                          htJerUpPFFilter *
                                          mhtJerUpFilter *
                                          countJerUpJetsAK5PFPt50Eta25 *
                                          jerUpJetsMHTPFDPhiFilter 
                                          )

# sequence core up JER variation
jerDnJetsAK5PFPt30 = cms.EDFilter("PATJetSelector",
                                  src = cms.InputTag("jerDnJets"),
                                  cut = cms.string("pt > 30")
                                  )

jerDnJetsAK5PFPt50Eta25 = cms.EDFilter("PATJetSelector",
                                       src = cms.InputTag("jerDnJets"),
                                       cut = cms.string('pt > 50 & abs(eta) < 2.5')
                                       )

htJerDnJets                = htPF.clone()
htJerDnJets.JetCollection  = cms.InputTag("jerDnJetsAK5PFPt50Eta25")

mhtJerDnJets               = mhtPF.clone()
mhtJerDnJets.JetCollection = cms.InputTag("jerDnJetsAK5PFPt30")

countJerDnJetsAK5PFPt50Eta25           = countPatJets.clone()
countJerDnJetsAK5PFPt50Eta25.src       = cms.InputTag('jerDnJetsAK5PFPt50Eta25')
countJerDnJetsAK5PFPt50Eta25.minNumber = cms.uint32(3)

jerDnJetsMHTPFDPhiFilter = jetMHTPFDPhiFilter.clone()
jerDnJetsMHTPFDPhiFilter.JetSource = cms.InputTag('jerDnJetsAK5PFPt30')
jerDnJetsMHTPFDPhiFilter.MHTSource = cms.InputTag("mhtJerDnJets")

htJerDnPFFilter            = htFilter.clone()
htJerDnPFFilter.HTSource   = cms.InputTag("htJerDnJets")
htJerDnPFFilter.MinHT      = cms.double(350.0)

mhtJerDnFilter             = mhtPFFilter.clone()
mhtJerDnFilter.MHTSource   = cms.InputTag("mhtJerDnJets")
mhtJerDnFilter.MinMHT      = cms.double(200.0)

jerDnSequence              = cms.Sequence(jerDnJets *
                                          jerDnJetsAK5PFPt30 *
                                          jerDnJetsAK5PFPt50Eta25 *
                                          htJerDnJets *
                                          mhtJerDnJets *
                                          htJerDnPFFilter *
                                          mhtJerDnFilter *
                                          countJerDnJetsAK5PFPt50Eta25 *
                                          jerDnJetsMHTPFDPhiFilter 
                                          )
