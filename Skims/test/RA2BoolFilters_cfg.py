import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

#===================== Message Logger =============================
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
    limit = cms.untracked.int32(10),
    reportEvery = cms.untracked.int32(1)
    )
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
    )
process.MessageLogger.cerr.FwkReport.reportEvery = 1

#================= configure poolsource module ===================
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring("/store/user/lpcsusyhad/53X_ntuples/kasmi/TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2_NOCUTS_12Oct2012V3_corrTag/b9d339f81100b66394e7e5c0a998fe80/susypat_11_1_xDE.root"))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source.skipEvents = cms.untracked.uint32(0)

#========================= global Tag =============================

#runningOnMC = True 
runningOnMC = False

#process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "START52_V9B::All"
if runningOnMC == False:
    process.GlobalTag.globaltag = "GR_R_52_V7::All"

##============ configure output module configuration ========================
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('test.root')
)

#=============== configure analysis sequence ================================

process.load("SandBox.Skims.filterBoolean_cfi")
process.RA2_HBHENoiseFilterRA2    = process.booleanFilter.clone()
process.RA2_HBHENoiseFilterRA2.ResultSource = cms.InputTag("HBHENoiseFilterRA2","HBHENoiseFilterResult","PAT")
process.RA2_beamHaloFilter        = process.booleanFilter.clone()
process.RA2_beamHaloFilter.ResultSource = cms.InputTag("beamHaloFilter")
process.RA2_eeNoiseFilter         = process.booleanFilter.clone()
process.RA2_eeNoiseFilter.ResultSource = cms.InputTag("eeNoiseFilter")
process.RA2_trackingFailureFilter = process.booleanFilter.clone()
process.RA2_trackingFailureFilter.ResultSource = cms.InputTag("trackingFailureFilter")
process.RA2_inconsistentMuons     = process.booleanFilter.clone()
process.RA2_inconsistentMuons.ResultSource = cms.InputTag("inconsistentMuons")
process.RA2_greedyMuons           = process.booleanFilter.clone()
process.RA2_greedyMuons.ResultSource = cms.InputTag("greedyMuons")
process.RA2_EcalTPFilter          = process.booleanFilter.clone()
process.RA2_EcalTPFilter.ResultSource = cms.InputTag("ra2EcalTPFilter")
process.RA2_EcalBEFilter          = process.booleanFilter.clone()
process.RA2_EcalBEFilter.ResultSource = cms.InputTag("ra2EcalBEFilter")
process.HcalLaserEventFilter      = process.booleanFilter.clone()
process.HcalLaserEventFilter.ResultSource = cms.InputTag("hcalLaserEventFilter")
process.EEBadScFilter             = process.booleanFilter.clone()
process.EEBadScFilter.ResultSource = cms.InputTag("eeBadScFilter")
process.EcalLaserFilter           = process.booleanFilter.clone()
process.EcalLaserFilter.ResultSource= cms.InputTag("ecalLaserCorrFilter")
process.RA2_PBNRFilter            = process.booleanFilter.clone()
process.RA2_PBNRFilter.ResultSource = cms.InputTag("ra2PBNR")

process.cleaningSeq  = cms.Sequence(
    process.RA2_HBHENoiseFilterRA2
  * process.RA2_beamHaloFilter
  #* process.RA2_eeNoiseFilter #don't use blindly
  * process.RA2_trackingFailureFilter
  * process.RA2_inconsistentMuons
  * process.RA2_greedyMuons
  * process.RA2_EcalTPFilter
  * process.RA2_EcalBEFilter
  * process.HcalLaserEventFilter
  * process.EEBadScFilter
  #* process.RA2_eeNoiseFilter
  * process.EcalLaserFilter
  #* process.RA2_PBNRFilter #(to be applied jet-by-jet)
)

process.load("SandBox.Skims.RA2Leptons_cff")


#process.p = cms.Path(process.cleaningSeq)
process.p = cms.Path(process.patElectronsID * process.patElectronsIDIso)


###-- Dump config ------------------------------------------------------------
##file = open('SusyPAT_RA2414_cfg.py','w')
##file.write(str(process.dumpPython()))
##file.close()
