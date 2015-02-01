from PhysicsTools.PatAlgos.patTemplate_cfg import *
#-- Message Logger ------------------------------------------------------------
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
    limit = cms.untracked.int32(-1),
    reportEvery = cms.untracked.int32(1)
    )

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('standard')

options.output = "ra2SUSYPAT.root"

options.register('GlobalTag', "START311_V2::All", VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "GlobaTTag to use (otherwise default Pat GT is used)")
options.register('mcInfo', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "process MonteCarlo data, default is data")
options.register('jetCorrections', 'L2Relative', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Level of jet corrections to use: Note the factors are read from DB via GlobalTag")
options.jetCorrections.append('L3Absolute')

options.register('hltName', 'REDIGI311X', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "HLT menu to use for trigger matching, e.g., HLT, REDIGI311X") 
options.register('mcVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific MC fix")
options.register('jetTypes', 'AK5PF', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional jet types that will be produced (AK5Calo and AK5PF, cross cleaned in PF2PAT, are included anyway)")
#options.jetTypes.append('AK5Calo')
options.register('hltSelection', '*', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "hlTriggers (OR) used to filter events. for data: ''HLT_Mu9', 'HLT_IsoMu9', 'HLT_IsoMu13_v*''; for MC, HLT_Mu9")
options.register('doValidation', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Include the validation histograms from SusyDQM (needs extra tags)")
options.register('doExtensiveMatching', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Matching to simtracks (needs extra tags)")
options.register('doSusyTopProjection', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Apply Susy selection in PF2PAT to obtain lepton cleaned jets (needs validation)")
options.register('addKeep', '', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional keep and drop statements to trim the event content")

options.register('dataVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific DATA fix")

options.register('debug', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "switch on/off debug mode")

options.register('doTPBEfilter', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "apply Ecal dead cell TP||BE filter, default is True")

options.parseArguments()
options._tagOrder =[]

print options
 
#-- Message Logger ------------------------------------------------------------
process.MessageLogger.cerr.FwkReport.reportEvery = 100
if options.debug:
   process.MessageLogger.cerr.FwkReport.reportEvery = 1

#-- Input Source --------------------------------------------------------------
if options.files:
   process.source.fileNames = options.files
else:
   process.source.fileNames = [
   # TTbarJets
   # InclusiveMu15
   # WJets
   # ZJets
        '/store/mc/Spring11/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0006/EA939248-D04E-E011-B095-E0CB4E553677.root',
   # QCD
        '/store/mc/Spring11/QCD_Pt_1400to1800_TuneZ2_7TeV_pythia6/AODSIM/PU_S1_START311_V1G1-v1/0044/F41F971A-9059-E011-8BCC-0015178C66B0.root',
        '/store/mc/Spring11/QCD_Pt_1400to1800_TuneZ2_7TeV_pythia6/AODSIM/PU_S1_START311_V1G1-v1/0044/EACD1979-8D59-E011-AABA-0015178C484C.root',
        '/store/mc/Spring11/QCD_Pt_1400to1800_TuneZ2_7TeV_pythia6/AODSIM/PU_S1_START311_V1G1-v1/0044/E4F48BBD-8D59-E011-8F49-00A0D1EE8A20.root',
        '/store/mc/Spring11/QCD_Pt_1400to1800_TuneZ2_7TeV_pythia6/AODSIM/PU_S1_START311_V1G1-v1/0044/DCEC5C6D-9059-E011-BE5A-0015178C4DB8.root',
        '/store/mc/Spring11/QCD_Pt_1400to1800_TuneZ2_7TeV_pythia6/AODSIM/PU_S1_START311_V1G1-v1/0044/DC2CC5AB-9059-E011-8E63-00151796D48C.root',
        '/store/mc/Spring11/QCD_Pt_1400to1800_TuneZ2_7TeV_pythia6/AODSIM/PU_S1_START311_V1G1-v1/0044/D8F58BB1-9059-E011-B540-001D0967DD0A.root',
        '/store/mc/Spring11/QCD_Pt_1400to1800_TuneZ2_7TeV_pythia6/AODSIM/PU_S1_START311_V1G1-v1/0044/D85FEC69-9059-E011-B0E2-0015178C6B90.root',
        '/store/mc/Spring11/QCD_Pt_1400to1800_TuneZ2_7TeV_pythia6/AODSIM/PU_S1_START311_V1G1-v1/0044/D0F2C974-9059-E011-BD03-0015179EDC6C.root',
#        '/store/mc/Spring11/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/GEN-SIM-RECODEBUG/PU_S1_START311_V1G1-v1/0002/FEDBE393-F64D-E011-A9A7-00215E21D906.root',
#        '/store/mc/Spring11/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/GEN-SIM-RECODEBUG/PU_S1_START311_V1G1-v1/0002/FE43E6AB-964E-E011-AA75-E41F13181D5C.root',
#        '/store/mc/Spring11/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/GEN-SIM-RECODEBUG/PU_S1_START311_V1G1-v1/0002/FE0EE3BC-E04E-E011-A5BD-00215E93EDA4.root',
#        '/store/mc/Spring11/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/GEN-SIM-RECODEBUG/PU_S1_START311_V1G1-v1/0002/FC7F9012-ED4D-E011-9009-E41F131816A0.root',
#        '/store/mc/Spring11/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/GEN-SIM-RECODEBUG/PU_S1_START311_V1G1-v1/0002/FC5ACFBD-774E-E011-AB6B-00215E21D690.root',
   # Data
     # HT-Run2011A-PromptReco/RECO
#       '/store/data/Run2011A/HT/RECO/PromptReco-v1/000/160/578/46066FCF-7351-E011-8E05-000423D94908.root' # for trigger HLT_HT160_v2
     # HT-Run2010A-PromptReco/AOD
#       '/store/data/Run2011A/HT/AOD/PromptReco-v1/000/161/016/E85EDEDB-EE55-E011-98AF-003048F11CF0.root',

   ]

process.source.inputCommands = cms.untracked.vstring( "keep *", "drop *_MEtoEDMConverter_*_*" )
process.maxEvents.input = options.maxEvents

#-- Calibration tag -----------------------------------------------------------
if options.GlobalTag:
   process.GlobalTag.globaltag = options.GlobalTag

if options.mcInfo == False: options.jetCorrections.append('L2L3Residual')
options.jetCorrections.insert(0, 'L1FastJet')

print "jetCorrections: "
print options.jetCorrections

############################# START SUSYPAT specifics ####################################
from PhysicsTools.Configuration.SUSY_pattuple_cff import addDefaultSUSYPAT, getSUSY_pattuple_outputCommands
#Apply SUSYPAT
addDefaultSUSYPAT(process,options.mcInfo,options.hltName,options.jetCorrections,options.mcVersion,options.jetTypes,options.doValidation,options.doExtensiveMatching,options.doSusyTopProjection)
SUSY_pattuple_outputCommands = getSUSY_pattuple_outputCommands( process )
############################## END SUSYPAT specifics ####################################

# Switch off some embedding
# Essential for particleflow based overlap cleaning
#process.patJetsAK5Calo.embedCaloTowers = False
#process.patJetsAK5Calo.embedPFCandidates = False
process.patJetsAK5PF.embedCaloTowers = False
process.patJetsAK5PF.embedPFCandidates = False

# PFClusterJet/MET
# FIXME: ONLY work in 42x release
#process.load('RecoJets.JetProducers.PFClustersForJets_cff')
#process.load('RecoMET.METProducers.PFClusterMET_cfi')
#process.pfClusterMET_seq = cms.Sequence(process.pfClusterRefsForJets*process.pfClusterMet)

# Default hltFilter with path "*"
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
if options.hltSelection:
   process.hltFilter = hlt.hltHighLevel.clone(
      TriggerResultsTag = cms.InputTag("TriggerResults","",options.hltName),
      HLTPaths = cms.vstring(options.hltSelection),
      throw = False, # Don't throw?!
      andOr = True
   )
   process.susyPatDefaultSequence.replace(process.eventCountProducer, process.eventCountProducer * process.hltFilter)

### standard sequences and paths for calo and pf
process.prefilterCounter        = cms.EDProducer("EventCountProducer")
process.postStdCleaningCounter  = cms.EDProducer("EventCountProducer")
process.postPFCleaningCounter   = cms.EDProducer("EventCountProducer")
process.postPFJetsCounter       = cms.EDProducer("EventCountProducer")
process.postPFHTCounter         = cms.EDProducer("EventCountProducer")

process.load('SandBox.Skims.RA2Objects_cff')
process.load('SandBox.Skims.RA2Jets_cff')
process.load('SandBox.Skims.RA2Leptons_cff')
process.load('SandBox.Skims.RA2HT_cff')
process.load('SandBox.Skims.RA2MHT_cff')
process.load('SandBox.Skims.RA2Photons_cff')

# One extra jet collection that I have used ...
process.patJetsAK5PFPt30Eta25 = process.selectedPatJets.clone()
process.patJetsAK5PFPt30Eta25.src = cms.InputTag('patJetsAK5PFPt30')
process.patJetsAK5PFPt30Eta25.cut = cms.string('abs(eta) < 2.5')

# Extra provenance info needed for cleaning jets overlapping with leptons
process.load('UserCode.SkimsRA2.provInfo_cfi')
process.provInfo.muonSource = cms.InputTag("patMuonsPF")
process.provInfo.eleSource = cms.InputTag("patElectronsPF")

if options.doTPBEfilter == False:
   process.dummyCounter1        = cms.EDProducer("EventCountProducer")
   process.dummyCounter2        = cms.EDProducer("EventCountProducer")
   process.dummyCounter3        = cms.EDProducer("EventCountProducer")
   process.ra2PFCleaningProducts.replace(process.ecalAnomalousFilter, process.dummyCounter1)
   process.ra2PostCleaning.replace(process.ecalDeadCellTPonlyFilter, process.dummyCounter2)
   process.ra2PostCleaning.replace(process.ecalDeadCellBEonlyFilter, process.dummyCounter3)
elif options.mcInfo == True:
   process.dummyCounter2        = cms.EDProducer("EventCountProducer")
   process.ra2PostCleaning.replace(process.ecalDeadCellTPonlyFilter, process.dummyCounter2)

## only difference with Steven's recipe is to remove calocleaning products.
process.ra2Objects = cms.Sequence(
    process.goodVertices *
    process.ra2PFCleaningProducts *
    process.ra2PFJets * process.patJetsAK5PFPt30Eta25 *
    process.ra2PFMuons *
    process.ra2PFElectrons *
    process.ra2Photons *
    process.htPF *
    process.mhtPF *
    process.provInfo
    )

process.cleanpatseq = cms.Sequence(
                      process.patMETCorrections *
                      process.susyPatDefaultSequence *
#                      process.pfClusterMET_seq *
                      process.prefilterCounter *
                      process.ra2Objects *
                      process.ra2StdCleaning *
                      process.postStdCleaningCounter
                      )

process.ppf = cms.Path(
        process.cleanpatseq *
        process.ra2PFCleaning *
        process.postPFCleaningCounter *
        process.countJetsAK5PFPt50Eta25 *
        process.postPFJetsCounter *
        process.htPFFilter *
        process.postPFHTCounter
      )

#-- Output module configuration -----------------------------------------------
process.out.fileName = options.output

# Custom settings
process.out.splitLevel = cms.untracked.int32(99)  # Turn on split level (smaller files???)
process.out.overrideInputFileSplitLevels = cms.untracked.bool(True)
#process.out.dropMetaData = cms.untracked.string('DROPPED')   # Get rid of metadata related to dropped collections
process.out.dropMetaData = cms.untracked.string('ALL')   # Get rid of metadata related to dropped collections
process.out.outputCommands = cms.untracked.vstring('drop *', *SUSY_pattuple_outputCommands)
if options.addKeep:
    process.out.outputCommands.extend(options.addKeep)

# Custom RA2 related contents
from SandBox.Skims.RA2Content_cff import getRA2PATOutput
process.out.outputCommands = getRA2PATOutput(process)

# My own customization
process.out.outputCommands.extend([
   'keep *_patTaus_*_*',
#   'keep *_patJets_*_*',
#   'keep *_TriggerResults_*_*',
   'keep edmTriggerResults_TriggerResults__REDIGI*',
#   'keep *_pf*PF_*_*',
   'keep *_*provInfo*_*_*',
   'drop *_patJetsIC5*_*_*',
   'drop *_*htCalo_*_*',
   'drop *_maskedECALProducer_*_*',
   'drop *_ecalAnomalousFilter_*_*'
])

process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('ppf') )

#process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.outpath = cms.EndPath(
      process.out
#      * process.dump 
      )
