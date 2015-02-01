import FWCore.ParameterSet.Config as cms

jerNominal = cms.EDProducer("JERVariation",
                        JetSource      = cms.InputTag('ak5GenJets'),
                        ResponseHistFile = cms.FileInPath("UserCode/SignalScan/data/ScaledAndSmearedMCTruthResolution_PF_dR02_01_BE1214_110118.root"),
                        DirResponseName = cms.string("QCDfromSmearing"),
                        jetMaxEtaCut = cms.double(-1.0),
                        jetMinPtCut = cms.double(-1.0),
                        )

jerUpJets                 = jerNominal.clone()
jerUpJets.DirResponseName = cms.string("QCDfromSmearingCoreUP")

jerDnJets                 = jerNominal.clone()
jerDnJets.DirResponseName = cms.string("QCDfromSmearingCoreDN")

