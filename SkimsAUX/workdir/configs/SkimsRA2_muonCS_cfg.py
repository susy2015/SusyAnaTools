from PhysicsTools.PatAlgos.patTemplate_cfg import *
# Message Logger ------------------------------------------------------------
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
   limit = cms.untracked.int32(-1),
   reportEvery = cms.untracked.int32(1)
)

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('standard')

options.output = "ra2SUSYPAT.root"

options.register('GlobalTag', "START42_V13::All", VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "GlobaTTag to use (otherwise default Pat GT is used)")
options.register('mcInfo', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "process MonteCarlo data, default is data")
options.register('jetCorrections', 'L2Relative', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Level of jet corrections to use: Note the factors are read from DB via GlobalTag")
options.jetCorrections.append('L3Absolute')

options.register('hltName', 'HLT', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "HLT menu to use for trigger matching, e.g., HLT, REDIGI311X") 
options.register('mcVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific MC fix")
options.register('jetTypes', 'AK5PF', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional jet types that will be produced (AK5Calo and AK5PF, cross cleaned in PF2PAT, are included anyway)")
#options.jetTypes.append('AK5Calo')
options.register('hltSelection', '*', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "hlTriggers (OR) used to filter events. for data: ''HLT_Mu9', 'HLT_IsoMu9', 'HLT_IsoMu13_v*''; for MC, HLT_Mu9")
options.register('doValidation', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Include the validation histograms from SusyDQM (needs extra tags)")
options.register('doExtensiveMatching', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Matching to simtracks (needs extra tags)")
options.register('doSusyTopProjection', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Apply Susy selection in PF2PAT to obtain lepton cleaned jets (needs validation)")
options.register('addKeep', '', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional keep and drop statements to trim the event content")

options.register('dataVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific DATA fix")

options.register('debug', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "switch on/off debug mode")

options.register('Spec42X', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "key output type string")

options.register('type', 'muonCS', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "key output type string")

options.register('doTPBEfilter', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "apply Ecal dead cell TP||BE filter, default is True")

options.register('dataTier', 'AOD', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "data tier string, e.g., AOD, RECO")

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
   # Assumming 1.5 GB each file OR 5 hours per job
   # TTbarJets --> 1286491 events; testing 2000 events: 364939424 B; 19:24:50-18:55:33 ~ 1757 s ==> 8000 events/job ~ 161 jobs ~ 1.95 hours/job ==> ~242 GB
        '/store/mc/Spring11/TTJets_TuneD6T_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0047/B6247956-0E5B-E011-9610-0024E8767D52.root',
        '/store/mc/Spring11/TTJets_TuneD6T_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0047/AE8174E0-385B-E011-BFA0-0024E87683AA.root',
        '/store/mc/Spring11/TTJets_TuneD6T_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0047/A66DEE86-D05A-E011-9694-0026B94E2899.root',
        '/store/mc/Spring11/TTJets_TuneD6T_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0047/7AD44420-D35A-E011-B71E-00151796D630.root',
   # InclusiveMu15 --> 29434562 events; testing 2000 events: 57705406 B; 19:12:55-18:52:51 ~ 124 s ==> 52000 events/job ~ 566 jobs ~ 0.90 hours/job ==> ~850 GB
        '/store/mc/Spring11/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0073/6EC83738-1657-E011-8A62-00261894393C.root',
        '/store/mc/Spring11/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0071/F2F974BA-5755-E011-930B-00261894380B.root',
        '/store/mc/Spring11/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0071/EC2CA2BA-5755-E011-A6F5-0026189438E8.root',
        '/store/mc/Spring11/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0071/C24F55BE-5755-E011-AF81-00261894393B.root',
   # WJets --> 15110974 events; testing 2000 events: 14843234 B; 19:19:10-18:54:05 ~ 1505 s ==> 24000 events/job ~ 630 jobs ~5 hours/job ~ 178 MB/job ==> ~112 GB
        '/store/mc/Spring11/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0019/E8406A8D-5B56-E011-9346-001A92810ADE.root',
        '/store/mc/Spring11/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0019/96961183-5B56-E011-BF4B-002618943809.root',
        '/store/mc/Spring11/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0014/FC12C45A-1C56-E011-B7AE-002354EF3BE2.root',
        '/store/mc/Spring11/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0014/AEA5985F-1C56-E011-8BEE-0018F3D09658.root',
   # DYJets --> 2400920 events; testing 2000 events: 32569994 B; 19:18:21-18:52:32 ~ 1549 s ==> 24000 events/job ~ 100 jobs ~5.16 hours/job ~ 391 MB/job ==> ~40 GB
        '/store/mc/Spring11/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0006/EA939248-D04E-E011-B095-E0CB4E553677.root',
        '/store/mc/Spring11/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0006/D82A8747-D04E-E011-9E94-90E6BA19A1FE.root',
        '/store/mc/Spring11/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0006/86E4BF47-D04E-E011-8EA2-485B39800BB3.root',
        '/store/mc/Spring11/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0005/AA882F9F-CD4E-E011-A99C-E0CB4E29C507.root',
   # QCD --> 10404020 events; testing 2000 events: 220498866 B; 19:32:02-18:56:27 ~ 2135 s ==> 14000 events/job ~ 743 jobs ~ 4.15 hours/job ==> ~1115 GB
        '/store/mc/Spring11/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/GEN-SIM-RECODEBUG/PU_S1_START311_V1G1-v1/0002/FEDBE393-F64D-E011-A9A7-00215E21D906.root',
        '/store/mc/Spring11/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/GEN-SIM-RECODEBUG/PU_S1_START311_V1G1-v1/0002/FE43E6AB-964E-E011-AA75-E41F13181D5C.root',
        '/store/mc/Spring11/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/GEN-SIM-RECODEBUG/PU_S1_START311_V1G1-v1/0002/FE0EE3BC-E04E-E011-A5BD-00215E93EDA4.root',
        '/store/mc/Spring11/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/GEN-SIM-RECODEBUG/PU_S1_START311_V1G1-v1/0002/FC7F9012-ED4D-E011-9009-E41F131816A0.root',
   # ZinvisibleJets --> 2165002 events; testing 2000 events: 8149072 B; 19:22:53-18:57:33 ~ 1520 s ==> 24000 events/job ~ 90 jobs ~ 5 hours/job ~ 98 MB/job ==> ~9 GB
        '/store/mc/Spring11/ZinvisibleJets_7TeV-madgraph/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0048/FA1EE905-515B-E011-8C82-00151796D5C4.root',
        '/store/mc/Spring11/ZinvisibleJets_7TeV-madgraph/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0048/F4CD344B-C45D-E011-9F4F-00151796D678.root',
        '/store/mc/Spring11/ZinvisibleJets_7TeV-madgraph/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0048/EAA9642A-635B-E011-99D4-0024E8768874.root',
        '/store/mc/Spring11/ZinvisibleJets_7TeV-madgraph/GEN-SIM-RECO/PU_S1_START311_V1G1-v1/0048/E8F31CB0-555B-E011-9FF3-0024E876820A.root',
   # Data
     # HT-Run2011A-PromptReco/RECO
#       '/store/data/Run2011A/HT/RECO/PromptReco-v1/000/160/578/46066FCF-7351-E011-8E05-000423D94908.root' # for trigger HLT_HT160_v2
     # HT-Run2010A-PromptReco/AOD
#       '/store/data/Run2011A/HT/AOD/PromptReco-v1/000/161/016/E85EDEDB-EE55-E011-98AF-003048F11CF0.root',

   ]

process.source.inputCommands = cms.untracked.vstring( "keep *", "drop *_MEtoEDMConverter_*_*" )
process.maxEvents.input = options.maxEvents

# Calibration tag -----------------------------------------------------------
if options.GlobalTag:
   process.GlobalTag.globaltag = options.GlobalTag

# JEC
if options.mcInfo == False: options.jetCorrections.append('L2L3Residual')
options.jetCorrections.insert(0, 'L1FastJet')
if options.Spec42X == "noL2L3Residual":
   options.jetCorrections.remove('L2L3Residual')

print "jetCorrections: "
print options.jetCorrections

############################# START SUSYPAT specifics ####################################
from PhysicsTools.Configuration.SUSY_pattuple_cff import addDefaultSUSYPAT, getSUSY_pattuple_outputCommands
#Apply SUSYPAT
addDefaultSUSYPAT(process,options.mcInfo,options.hltName,options.jetCorrections,options.mcVersion,options.jetTypes,options.doValidation,options.doExtensiveMatching,options.doSusyTopProjection)
SUSY_pattuple_outputCommands = getSUSY_pattuple_outputCommands( process )
############################## END SUSYPAT specifics ####################################

# Remove the PAT cleaning and filtering sequences
process.patDefaultSequence.remove(process.selectedPatCandidates)
process.patDefaultSequence.remove(process.cleanPatCandidates)
process.patDefaultSequence.remove(process.countPatCandidates)

# Disable embedment so that lepton cleaning method works
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
      throw = True, # Don't throw?!
      andOr = True
   )
   process.susyPatDefaultSequence.replace(process.eventCountProducer, process.eventCountProducer * process.hltFilter)

# Standard sequences and paths for calo and pf
process.prefilterCounter        = cms.EDProducer("EventCountProducer")
process.postStdCleaningCounter  = cms.EDProducer("EventCountProducer")
process.postPFCleaningCounter   = cms.EDProducer("EventCountProducer")
process.postPFJetsCounter       = cms.EDProducer("EventCountProducer")
process.postPFHTCounter         = cms.EDProducer("EventCountProducer")

# Load RA2 related configuration
process.load('SandBox.Skims.RA2Objects_cff')
process.load('SandBox.Skims.RA2Selection_cff')

# One extra jet collection that I have used ...
process.patJetsAK5PFPt30Eta25 = process.selectedPatJets.clone()
process.patJetsAK5PFPt30Eta25.src = cms.InputTag('patJetsAK5PFPt30')
process.patJetsAK5PFPt30Eta25.cut = cms.string('abs(eta) < 2.5')

# Extra provenance info needed for cleaning jets overlapping with leptons
process.load('UserCode.SkimsRA2.provInfo_cfi')
process.provInfo.muonSource = cms.InputTag("patMuonsPF")
process.provInfo.eleSource = cms.InputTag("patElectronsPF")

# For BE filter
process.load('PhysicsTools.EcalAnomalousEventFilter.ecalanomalouseventfilter_cfi')
process.ecalAnomalousFilter = process.EcalAnomalousEventFilter.clone()
process.ecalAnomalousFilter.FilterAlgo= cms.untracked.string("TuningMode")
process.ecalAnomalousFilter.cutBoundEnergyDeadCellsEB=cms.untracked.double(10)
process.ecalAnomalousFilter.cutBoundEnergyDeadCellsEE=cms.untracked.double(10)
process.ecalAnomalousFilter.cutBoundEnergyGapEB=cms.untracked.double(100)
process.ecalAnomalousFilter.cutBoundEnergyGapEE=cms.untracked.double(100)
process.ecalAnomalousFilter.enableGap=cms.untracked.bool(False)
process.ecalAnomalousFilter.limitDeadCellToChannelStatusEB = cms.vint32(12,14)
process.ecalAnomalousFilter.limitDeadCellToChannelStatusEE = cms.vint32(12,14)

# Minimum jets requirement for standard RA2 and hadronic tau
process.count2JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25.clone()
process.count2JetsAK5PFPt50Eta25.minNumber = cms.uint32(2)

if options.mcInfo == True:
   process.patMETsTypeIPF.corrector = cms.string('ak5PFL2L3')
else:
   process.patMETsTypeIPF.corrector = cms.string('ak5PFL2L3Residual')

# Most of RA2 sequence here
process.cleanpatseq = cms.Sequence(
                      process.prefilterCounter *
                      process.ra2StdCleaning *
                      process.postStdCleaningCounter *
                      process.susyPatDefaultSequence *
                      process.ra2Objects * process.patJetsAK5PFPt30Eta25 * process.ecalAnomalousFilter #*
#                      process.ra2PostCleaning
                     )

if options.dataTier == "AOD":
   process.cleanpatseq.remove(process.ecalAnomalousFilter)

# Our traditional ppf sequence
process.ppf = cms.Path(
        process.cleanpatseq *
        process.count2JetsAK5PFPt50Eta25
      )

# Output module configuration -----------------------------------------------
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
   'keep LHEEventProduct_*_*_*',
   'keep *_patTaus_*_*',
   'keep edmTriggerResults_TriggerResults__REDIGI*',
   'keep *_*provInfo*_*_*',
   'keep *_ecalTPSkim_*_*',
   'keep *_reducedEcalRecHits*_*_*',
   'keep *_ecalAnomalousFilter_*_*',
   'keep *_addPileupInfo_*_*',
   'keep recoTracks_generalTracks*_*_*',
   'keep recoCaloJets_ak5CaloJets_*_*',
   'keep recoCaloMETs_met_*_*',
   'keep recoMETs_tcMet_*_*',
   'drop *_patJetsIC5*_*_*',
   'drop *_*htCalo_*_*',
   'drop *_maskedECALProducer_*_*',
])

process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('ppf') )

#process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.outpath = cms.EndPath(
      process.out
#      * process.dump 
      )
