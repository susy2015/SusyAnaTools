import FWCore.ParameterSet.Config as cms

#Three options to define/calculate an event weight:
muonEffProducer = cms.EDProducer('muonEffProducer',

   jetSrc           = cms.InputTag("patJetsAK5PFPt30"),
   caloJetSrc       = cms.InputTag("patJetsAK5CaloPt30"),
   muonSrc          = cms.InputTag("myPatMuonsPFIDIso"),

   applyMethods     = cms.vint32(0, 0),
   pfJetSelection   = cms.string("pt>0"),
   caloJetSelection = cms.string("pt>30 && emEnergyFraction<0.9 && emEnergyFraction>0.05"), # All caloJet inputs have minimum pt>30 GeV cut
   
   effFileName      = cms.string("fit_Muon_MC_Comb.root"),
 
   isopfJetMinDR    = cms.double(0.05),
   isocaloJetMinDR  = cms.double(0.00),
 
   isoEffHistNames  = cms.vstring("muonEffs/ISO_DRJan_Pt/probe_drjJan_probe_pt_PLOT"),
   recoEffHistNames = cms.vstring("muonEffs/ID_pt_eta/probe_pt_probe_eta_PLOT"),

)
