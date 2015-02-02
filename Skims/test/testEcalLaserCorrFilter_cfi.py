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
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring([
  'file:/uscms/home/seema/3DayLifetime/pickevents_triggerissue.root'
] ))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source.skipEvents = cms.untracked.uint32(0)

# ############   Geometry & magnetic field ###############
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi");
process.load("Geometry.CaloEventSetup.CaloGeometry_cfi");
process.load("Geometry.CaloEventSetup.CaloTopology_cfi");
# ############ Database ##############
process.EcalLaserCorrectionService = cms.ESProducer("EcalLaserCorrectionService")

#========================= global Tag =============================
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'FT_R_53_V6::All'     #GT for Jul13 rereco


#=============== configure analysis sequence ================================

process.load("SandBox.Skims.ecalLaserCorrFilter_cfi")
process.ecalLaserCorrFilter.Debug = True

process.ppf = cms.Path( process.ecalLaserCorrFilter )

