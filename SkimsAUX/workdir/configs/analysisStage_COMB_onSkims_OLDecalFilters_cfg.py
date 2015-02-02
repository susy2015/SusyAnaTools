from UserCode.InvisibleZ.patTemplate_cfg import *

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('standard')

options.register('GlobalTag', "START311_V2::All", VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "GlobaTTag to use (otherwise default Pat GT is used)")
options.register('mcInfo', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "process MonteCarlo data, default is data")
options.register('jetCorrections', 'L2Relative', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Level of jet corrections to use: Note the factors are read from DB via GlobalTag")
options.jetCorrections.append('L3Absolute')

options.register('hltName', 'REDIGI311X', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "HLT menu to use for trigger matching") 
options.register('mcVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific MC fix")
options.register('jetTypes', 'AK5PF', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional jet types that will be produced (AK5Calo and AK5PF, cross cleaned in PF2PAT, are included anyway)")
options.register('hltSelection', '*', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "hlTriggers (OR) used to filter events. for data: ''HLT_Mu9', 'HLT_IsoMu9', 'HLT_IsoMu13_v*''; for MC, HLT_Mu9")
options.register('doValidation', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Include the validation histograms from SusyDQM (needs extra tags)")
options.register('doExtensiveMatching', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Matching to simtracks (needs extra tags)")
options.register('doSusyTopProjection', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Apply Susy selection in PF2PAT to obtain lepton cleaned jets (needs validation)")
options.register('addKeep', '', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional keep and drop statements to trim the event content")

options.register('dataVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific DATA fix")

options.register('debug', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "switch on/off debug mode")

options.register('test', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "switch on/off debug mode")

options.register('type', 'Zinv', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "key output type string")

options.register('WJetsSpecEvtFilter', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "specific option for WJets signal -- currently filter nonIsoMuonMatchToGenMuon or not")

options.register('doTPBEfilter', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "apply Ecal dead cell TP||BE filter, default is True")
options.register('doSpecialTPBEpaths', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "append special TP||BE filter paths")

options.register('doPtHatWeighting', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "PtHat weighting for QCD flat samples, default is False")

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
   # QCD
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_9_1_FxV.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_8_1_MZX.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_7_1_3tT.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_6_1_8WH.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_5_1_2uw.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_4_1_tA9.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_3_1_xRp.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_39_1_hid.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_38_1_Q3A.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_37_1_JRb.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_36_1_qmU.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_35_1_9kp.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_34_1_vX4.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_33_1_1ob.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_32_1_bXX.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_31_1_2cg.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_30_1_LLt.root',
        '/store/user/lhx/2011RA2/May10/lhx/QCD_Pt_1800_TuneZ2_7TeV_pythia6/QCD_Pt_1800_TuneZ2_pythia6_Spring11_ra2Cleaned_Inclusive3Jets_HT300/33d90992b6a26befe289e6bd5bcf20fb/ra2SUSYPAT_2_1_w5h.root',
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

# Default setting for test mode
if options.test:
   # This one should be corresponding to the process.source we use!
   process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange('160578:6-160578:53','160578:274-160578:356')
   ## Options and Output Report
   # process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
   options.hltSelection.pop()
   options.hltSelection.append('HLT_HT160_v2')
   process.maxEvents.input = 1000

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
#   process.susyPatDefaultSequence.replace(process.eventCountProducer, process.eventCountProducer * process.hltFilter)

### standard sequences and paths for calo and pf
process.prefilterCounter        = cms.EDProducer("EventCountProducer")
process.postStdCleaningCounter  = cms.EDProducer("EventCountProducer")
process.postCaloCleaningCounter = cms.EDProducer("EventCountProducer")
process.postPFCleaningCounter   = cms.EDProducer("EventCountProducer")
process.postCaloJetsCounter     = cms.EDProducer("EventCountProducer")
process.postPFJetsCounter       = cms.EDProducer("EventCountProducer")
process.postCaloHTCounter       = cms.EDProducer("EventCountProducer")
process.postPFHTCounter         = cms.EDProducer("EventCountProducer")

process.load('SandBox.Skims.RA2Objects_cff')
process.load('SandBox.Skims.RA2Jets_cff')
process.load('SandBox.Skims.RA2Leptons_cff')
process.load('SandBox.Skims.RA2HT_cff')
process.load('SandBox.Skims.RA2MHT_cff')
process.load('SandBox.Skims.RA2Photons_cff')

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
    process.ra2PFJets *
    process.ra2PFMuons *
    process.ra2PFElectrons *
    process.ra2Photons *
    process.htPF *
    process.mhtPF
    )

process.cleanpatseq = cms.Sequence(
#                      process.patMETCorrections *
#                      process.susyPatDefaultSequence *
#                      process.pfClusterMET_seq *
                       process.prefilterCounter *
                       process.ra2Objects *
                       process.ra2StdCleaning *
                       process.postStdCleaningCounter
                      )

process.ppf = cms.Sequence(
        process.cleanpatseq *
        process.ra2PFCleaning *
        process.postPFCleaningCounter
      )

process.load('UserCode.InvisibleZ.weightProducer_cfi')
if options.doPtHatWeighting:
   process.weightProducer.Method     = cms.string("PtHat")
   process.weightProducer.Exponent   = cms.double(-4.5)
   process.weightProducer.XS         = cms.double(1.0)
   process.weightProducer.NumberEvts = cms.double(1.0)
   process.weightProducer.Lumi       = cms.double(1.0)
   process.weightProducer.weightWARNingUpThreshold  = cms.double(2.0)

process.load('UserCode.InvisibleZ.mySusyRA2GenCuts_cff')
process.mymhtPF.JetCollection = cms.InputTag("myPatJetsAK5PFPt30noOverlapLeptons")
process.myhtPF.JetCollection = cms.InputTag("myPatJetsAK5PFPt50Eta25noOverlapLeptons")
process.myjetMHTDPhiFilter.JetSource = cms.InputTag("myPatJetsAK5PFPt30noOverlapLeptons")
process.bTagging.jetSource = cms.InputTag("myPatJetsAK5PFPt30noOverlapLeptons")

process.load("UserCode.InvisibleZ.stdRA2ana_cfi")
process.stdRA2ana.mhtInputTag = cms.InputTag("mymhtPF")
process.stdRA2ana.htInputTag = cms.InputTag("myhtPF")
process.stdRA2ana.jetSource = cms.untracked.InputTag("myPatJetsAK5PFPt30noOverlapLeptons")
process.stdRA2ana.genMHTInputTag = cms.InputTag("mymhtGenPF")
process.stdRA2ana.genHTInputTag = cms.InputTag("myhtGenPF")
process.stdRA2ana.genJetSource = cms.untracked.InputTag("myGenJetsAK5Pt30noOverlapLeptons:cleanGenJets")
#process.stdRA2ana.nGoodLeptonsInput = cms.InputTag("leptonVetoPOGnoMu:nGoodLeptons")

process.stdRA2ana_stage0 = process.stdRA2ana.clone()
process.stdRA2ana_stage0.outputPath = cms.untracked.string("stage0" + "_" + options.type + "_stdRA2.root")

process.stdRA2ana_stage1 = process.stdRA2ana.clone()
process.stdRA2ana_stage1.outputPath = cms.untracked.string("stage1" + "_" + options.type + "_stdRA2.root")

process.stdRA2ana_stage2 = process.stdRA2ana.clone()
process.stdRA2ana_stage2.outputPath = cms.untracked.string("stage2" + "_" + options.type + "_stdRA2.root")

process.stdRA2ana_stage3 = process.stdRA2ana.clone()
process.stdRA2ana_stage3.outputPath = cms.untracked.string("stage3" + "_" + options.type + "_stdRA2.root")

process.stdRA2ana_stage4 = process.stdRA2ana.clone();
process.stdRA2ana_stage4.outputPath = cms.untracked.string("stage4" + "_" + options.type + "_stdRA2.root")

process.stdRA2ana_stage5 = process.stdRA2ana.clone();
process.stdRA2ana_stage5.outputPath = cms.untracked.string("stage5" + "_" + options.type + "_stdRA2.root")
process.stdRA2ana_stage5.debug = cms.bool(True)
process.stdRA2ana_stage5.verbose = cms.int32(2)

process.stdRA2ana_stage6 = process.stdRA2ana.clone();
process.stdRA2ana_stage6.outputPath = cms.untracked.string("stage6" + "_" + options.type + "_stdRA2.root")

# data-driven QCD
process.load("UserCode.InvisibleZ.dataDrivenQCD_cfi")

process.myPatMuonsPFIDnoIso = process.myPatMuonsPFIDIso.clone()
process.myPatMuonsPFIDnoIso.DoMuonIsolation = False

process.dataDrivenQCD.mhtInputTag = cms.InputTag("mymhtPF")
process.dataDrivenQCD.htInputTag = cms.InputTag("myhtPF")
process.dataDrivenQCD.metSource = cms.untracked.InputTag("patMETsPF")
process.dataDrivenQCD.muonSource = cms.untracked.InputTag("myPatMuonsPFIDnoIso")
process.dataDrivenQCD.muonIsoVecInput = cms.InputTag("myPatMuonsPFIDnoIso:muonIso")
process.dataDrivenQCD.muonIsoCutInput = cms.double(0.20)
process.dataDrivenQCD.jetSource = cms.untracked.InputTag("myPatJetsAK5PFPt30noOverlapLeptons")
process.dataDrivenQCD.bTagJetSource = cms.InputTag("myPatJetsAK5PFPt50Eta25noOverlapLeptons")
process.dataDrivenQCD.minbTagPur = cms.double(2.0)
process.dataDrivenQCD.minbTagVeto = cms.double(1.74)

process.dataDrivenQCD_stage1 = process.dataDrivenQCD.clone()
process.dataDrivenQCD_stage1.outputPath = cms.untracked.string("stage1" + "_" + options.type + "_QCD.root")

process.dataDrivenQCD_stage2 = process.dataDrivenQCD.clone()
process.dataDrivenQCD_stage2.outputPath = cms.untracked.string("stage2" + "_" + options.type + "_QCD.root")

process.dataDrivenQCD_stage3 = process.dataDrivenQCD.clone()
process.dataDrivenQCD_stage3.outputPath = cms.untracked.string("stage3" + "_" + options.type + "_QCD.root")

process.dataDrivenQCD_stage4 = process.dataDrivenQCD.clone()
process.dataDrivenQCD_stage4.outputPath = cms.untracked.string("stage4" + "_" + options.type + "_QCD.root")

process.dataDrivenQCD_stage5 = process.dataDrivenQCD.clone()
process.dataDrivenQCD_stage5.outputPath = cms.untracked.string("stage5" + "_" + options.type + "_QCD.root")

process.dataDrivenQCD_stage6 = process.dataDrivenQCD.clone()
process.dataDrivenQCD_stage6.outputPath = cms.untracked.string("stage6" + "_" + options.type + "_QCD.root")

# data-driven TTbar
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load ("RecoBTag.PerformanceDB.PoolBTagPerformanceDBMC36X")
process.load ("RecoBTag.PerformanceDB.BTagPerformanceDBMC36X")
process.load ("RecoBTag.PerformanceDB.PoolBTagPerformanceDB1101")
process.load ("RecoBTag.PerformanceDB.BTagPerformanceDB1101")

process.load("UserCode.InvisibleZ.dataDrivenTTbar_cfi")
process.dataDrivenTTbar.mhtInputTag = cms.InputTag("mymhtPF")
process.dataDrivenTTbar.htInputTag = cms.InputTag("myhtPF")
process.dataDrivenTTbar.metSource = cms.untracked.InputTag("patMETsPF")
#process.dataDrivenTTbar.jetSource = cms.untracked.InputTag("myPatJetsAK5PFPt30noOverlapLeptons")
process.dataDrivenTTbar.jetSource = cms.untracked.InputTag("myPatJetsAK5PFPt50Eta25noOverlapLeptons")
#process.dataDrivenTTbar.jetSource = cms.untracked.InputTag("myPatJetsAK5PFPt30Eta25noOverlapLeptons")
process.dataDrivenTTbar.allJetSource = cms.InputTag("myPatJetsAK5PFnoOverlapLeptons")
process.dataDrivenTTbar.minbTagPur = cms.double(2.0)
process.dataDrivenTTbar.minbTagVeto = cms.double(1.74)
process.dataDrivenTTbar.muonSource = cms.untracked.InputTag("myPatMuonsPFIDIso")

process.dataDrivenTTbar_stage0 = process.dataDrivenTTbar.clone()
process.dataDrivenTTbar_stage0.outputPath = cms.untracked.string("stage0" + "_" + options.type + "_TTbar.root")
process.dataDrivenTTbar_stage0.effSSVHEMinput            = cms.double(0.64371)
process.dataDrivenTTbar_stage0.cQuark_misRateSSVHEMinput = cms.double(0.17570)
process.dataDrivenTTbar_stage0.lQuark_misRateSSVHEMinput = cms.double(0.01500)
process.dataDrivenTTbar_stage0.effSSVHPTinput            = cms.double(0.48759)
process.dataDrivenTTbar_stage0.cQuark_misRateSSVHPTinput = cms.double(0.06719)
process.dataDrivenTTbar_stage0.lQuark_misRateSSVHPTinput = cms.double(0.00192)
process.dataDrivenTTbar_stage0.bQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage0.cQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage0.lQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage0.bQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage0.cQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage0.lQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage0.effErrSSVHEMinput            = cms.double(0.00104)
process.dataDrivenTTbar_stage0.cQuark_misRateErrSSVHEMinput = cms.double(0.00199)
process.dataDrivenTTbar_stage0.lQuark_misRateErrSSVHEMinput = cms.double(0.00031)
process.dataDrivenTTbar_stage0.effErrSSVHPTinput            = cms.double(0.00108)
process.dataDrivenTTbar_stage0.cQuark_misRateErrSSVHPTinput = cms.double(0.00131)
process.dataDrivenTTbar_stage0.lQuark_misRateErrSSVHPTinput = cms.double(0.00011)
process.dataDrivenTTbar_stage0.bQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage0.cQuark_sfErrSSVHEMinput      = cms.double(0.270)
process.dataDrivenTTbar_stage0.lQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage0.bQuark_sfErrSSVHPTinput      = cms.double(0.135)
process.dataDrivenTTbar_stage0.cQuark_sfErrSSVHPTinput      = cms.double(0.270)
process.dataDrivenTTbar_stage0.lQuark_sfErrSSVHPTinput      = cms.double(0.135)


process.dataDrivenTTbar_stage1 = process.dataDrivenTTbar.clone()
process.dataDrivenTTbar_stage1.outputPath = cms.untracked.string("stage1" + "_" + options.type + "_TTbar.root")
process.dataDrivenTTbar_stage1.effSSVHEMinput            = cms.double(0.64371)
process.dataDrivenTTbar_stage1.cQuark_misRateSSVHEMinput = cms.double(0.17570)
process.dataDrivenTTbar_stage1.lQuark_misRateSSVHEMinput = cms.double(0.01500)
process.dataDrivenTTbar_stage1.effSSVHPTinput            = cms.double(0.48759)
process.dataDrivenTTbar_stage1.cQuark_misRateSSVHPTinput = cms.double(0.06719)
process.dataDrivenTTbar_stage1.lQuark_misRateSSVHPTinput = cms.double(0.00192)
process.dataDrivenTTbar_stage1.bQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage1.cQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage1.lQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage1.bQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage1.cQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage1.lQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage1.effErrSSVHEMinput            = cms.double(0.00104)
process.dataDrivenTTbar_stage1.cQuark_misRateErrSSVHEMinput = cms.double(0.00199)
process.dataDrivenTTbar_stage1.lQuark_misRateErrSSVHEMinput = cms.double(0.00031)
process.dataDrivenTTbar_stage1.effErrSSVHPTinput            = cms.double(0.00108)
process.dataDrivenTTbar_stage1.cQuark_misRateErrSSVHPTinput = cms.double(0.00131)
process.dataDrivenTTbar_stage1.lQuark_misRateErrSSVHPTinput = cms.double(0.00011)
process.dataDrivenTTbar_stage1.bQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage1.cQuark_sfErrSSVHEMinput      = cms.double(0.270)
process.dataDrivenTTbar_stage1.lQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage1.bQuark_sfErrSSVHPTinput      = cms.double(0.135)
process.dataDrivenTTbar_stage1.cQuark_sfErrSSVHPTinput      = cms.double(0.270)
process.dataDrivenTTbar_stage1.lQuark_sfErrSSVHPTinput      = cms.double(0.135)


process.dataDrivenTTbar_stage2 = process.dataDrivenTTbar.clone()
process.dataDrivenTTbar_stage2.outputPath = cms.untracked.string("stage2" + "_" + options.type + "_TTbar.root")
process.dataDrivenTTbar_stage2.effSSVHEMinput            = cms.double(0.64502)
process.dataDrivenTTbar_stage2.cQuark_misRateSSVHEMinput = cms.double(0.17536)
process.dataDrivenTTbar_stage2.lQuark_misRateSSVHEMinput = cms.double(0.01493)
process.dataDrivenTTbar_stage2.effSSVHPTinput            = cms.double(0.48936)
process.dataDrivenTTbar_stage2.cQuark_misRateSSVHPTinput = cms.double(0.06699)
process.dataDrivenTTbar_stage2.lQuark_misRateSSVHPTinput = cms.double(0.00186)
process.dataDrivenTTbar_stage2.bQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage2.cQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage2.lQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage2.bQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage2.cQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage2.lQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage2.effErrSSVHEMinput            = cms.double(0.00111)
process.dataDrivenTTbar_stage2.cQuark_misRateErrSSVHEMinput = cms.double(0.00210)
process.dataDrivenTTbar_stage2.lQuark_misRateErrSSVHEMinput = cms.double(0.00032)
process.dataDrivenTTbar_stage2.effErrSSVHPTinput            = cms.double(0.00116)
process.dataDrivenTTbar_stage2.cQuark_misRateErrSSVHPTinput = cms.double(0.00138)
process.dataDrivenTTbar_stage2.lQuark_misRateErrSSVHPTinput = cms.double(0.00011)
process.dataDrivenTTbar_stage2.bQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage2.cQuark_sfErrSSVHEMinput      = cms.double(0.270)
process.dataDrivenTTbar_stage2.lQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage2.bQuark_sfErrSSVHPTinput      = cms.double(0.135)
process.dataDrivenTTbar_stage2.cQuark_sfErrSSVHPTinput      = cms.double(0.270)
process.dataDrivenTTbar_stage2.lQuark_sfErrSSVHPTinput      = cms.double(0.135)


process.dataDrivenTTbar_stage3 = process.dataDrivenTTbar.clone()
process.dataDrivenTTbar_stage3.outputPath = cms.untracked.string("stage3" + "_" + options.type + "_TTbar.root")
process.dataDrivenTTbar_stage3.effSSVHEMinput            = cms.double(0.64698)
process.dataDrivenTTbar_stage3.cQuark_misRateSSVHEMinput = cms.double(0.17674)
process.dataDrivenTTbar_stage3.lQuark_misRateSSVHEMinput = cms.double(0.01509)
process.dataDrivenTTbar_stage3.effSSVHPTinput            = cms.double(0.49226)
process.dataDrivenTTbar_stage3.cQuark_misRateSSVHPTinput = cms.double(0.06940)
process.dataDrivenTTbar_stage3.lQuark_misRateSSVHPTinput = cms.double(0.00178)
process.dataDrivenTTbar_stage3.bQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage3.cQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage3.lQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage3.bQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage3.cQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage3.lQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage3.effErrSSVHEMinput            = cms.double(0.00150)
process.dataDrivenTTbar_stage3.cQuark_misRateErrSSVHEMinput = cms.double(0.00269)
process.dataDrivenTTbar_stage3.lQuark_misRateErrSSVHEMinput = cms.double(0.00039)
process.dataDrivenTTbar_stage3.effErrSSVHPTinput            = cms.double(0.00157)
process.dataDrivenTTbar_stage3.cQuark_misRateErrSSVHPTinput = cms.double(0.00179)
process.dataDrivenTTbar_stage3.lQuark_misRateErrSSVHPTinput = cms.double(0.00014)
process.dataDrivenTTbar_stage3.bQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage3.cQuark_sfErrSSVHEMinput      = cms.double(0.270)
process.dataDrivenTTbar_stage3.lQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage3.bQuark_sfErrSSVHPTinput      = cms.double(0.135)
process.dataDrivenTTbar_stage3.cQuark_sfErrSSVHPTinput      = cms.double(0.270)
process.dataDrivenTTbar_stage3.lQuark_sfErrSSVHPTinput      = cms.double(0.135)


process.dataDrivenTTbar_stage4 = process.dataDrivenTTbar.clone()
process.dataDrivenTTbar_stage4.outputPath = cms.untracked.string("stage4" + "_" + options.type + "_TTbar.root")
process.dataDrivenTTbar_stage4.effSSVHEMinput            = cms.double(0.65326)
process.dataDrivenTTbar_stage4.cQuark_misRateSSVHEMinput = cms.double(0.18287)
process.dataDrivenTTbar_stage4.lQuark_misRateSSVHEMinput = cms.double(0.01691)
process.dataDrivenTTbar_stage4.effSSVHPTinput            = cms.double(0.50175)
process.dataDrivenTTbar_stage4.cQuark_misRateSSVHPTinput = cms.double(0.07008)
process.dataDrivenTTbar_stage4.lQuark_misRateSSVHPTinput = cms.double(0.00204)
process.dataDrivenTTbar_stage4.bQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage4.cQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage4.lQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage4.bQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage4.cQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage4.lQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage4.effErrSSVHEMinput            = cms.double(0.00226)
process.dataDrivenTTbar_stage4.cQuark_misRateErrSSVHEMinput = cms.double(0.00400)
process.dataDrivenTTbar_stage4.lQuark_misRateErrSSVHEMinput = cms.double(0.00058)
process.dataDrivenTTbar_stage4.effErrSSVHPTinput            = cms.double(0.00237)
process.dataDrivenTTbar_stage4.cQuark_misRateErrSSVHPTinput = cms.double(0.00264)
process.dataDrivenTTbar_stage4.lQuark_misRateErrSSVHPTinput = cms.double(0.00020)
process.dataDrivenTTbar_stage4.bQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage4.cQuark_sfErrSSVHEMinput      = cms.double(0.270)
process.dataDrivenTTbar_stage4.lQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage4.bQuark_sfErrSSVHPTinput      = cms.double(0.135)
process.dataDrivenTTbar_stage4.cQuark_sfErrSSVHPTinput      = cms.double(0.270)
process.dataDrivenTTbar_stage4.lQuark_sfErrSSVHPTinput      = cms.double(0.135)


process.dataDrivenTTbar_stage5 = process.dataDrivenTTbar.clone()
process.dataDrivenTTbar_stage5.outputPath = cms.untracked.string("stage5" + "_" + options.type + "_TTbar.root")
process.dataDrivenTTbar_stage5.debug = cms.bool(True)
process.dataDrivenTTbar_stage5.verbose = cms.int32(2)
process.dataDrivenTTbar_stage5.effSSVHEMinput            = cms.double(0.65645)
process.dataDrivenTTbar_stage5.cQuark_misRateSSVHEMinput = cms.double(0.18266)
process.dataDrivenTTbar_stage5.lQuark_misRateSSVHEMinput = cms.double(0.01668)
process.dataDrivenTTbar_stage5.effSSVHPTinput            = cms.double(0.50496)
process.dataDrivenTTbar_stage5.cQuark_misRateSSVHPTinput = cms.double(0.07166)
process.dataDrivenTTbar_stage5.lQuark_misRateSSVHPTinput = cms.double(0.00196)
process.dataDrivenTTbar_stage5.bQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage5.cQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage5.lQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage5.bQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage5.cQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage5.lQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage5.effErrSSVHEMinput            = cms.double(0.00264)
process.dataDrivenTTbar_stage5.cQuark_misRateErrSSVHEMinput = cms.double(0.00467)
process.dataDrivenTTbar_stage5.lQuark_misRateErrSSVHEMinput = cms.double(0.00067)
process.dataDrivenTTbar_stage5.effErrSSVHPTinput            = cms.double(0.00278)
process.dataDrivenTTbar_stage5.cQuark_misRateErrSSVHPTinput = cms.double(0.00312)
process.dataDrivenTTbar_stage5.lQuark_misRateErrSSVHPTinput = cms.double(0.00023)
process.dataDrivenTTbar_stage5.bQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage5.cQuark_sfErrSSVHEMinput      = cms.double(0.270)
process.dataDrivenTTbar_stage5.lQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage5.bQuark_sfErrSSVHPTinput      = cms.double(0.135)
process.dataDrivenTTbar_stage5.cQuark_sfErrSSVHPTinput      = cms.double(0.270)
process.dataDrivenTTbar_stage5.lQuark_sfErrSSVHPTinput      = cms.double(0.135)


process.dataDrivenTTbar_stage6 = process.dataDrivenTTbar.clone()
process.dataDrivenTTbar_stage6.outputPath = cms.untracked.string("stage6" + "_" + options.type + "_TTbar.root")
process.dataDrivenTTbar_stage6.effSSVHEMinput            = cms.double(0.65257)
process.dataDrivenTTbar_stage6.cQuark_misRateSSVHEMinput = cms.double(0.19064)
process.dataDrivenTTbar_stage6.lQuark_misRateSSVHEMinput = cms.double(0.01768)
process.dataDrivenTTbar_stage6.effSSVHPTinput            = cms.double(0.49489)
process.dataDrivenTTbar_stage6.cQuark_misRateSSVHPTinput = cms.double(0.07509)
process.dataDrivenTTbar_stage6.lQuark_misRateSSVHPTinput = cms.double(0.00227)
process.dataDrivenTTbar_stage6.bQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage6.cQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage6.lQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage6.bQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage6.cQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage6.lQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage6.effErrSSVHEMinput            = cms.double(0.00513)
process.dataDrivenTTbar_stage6.cQuark_misRateErrSSVHEMinput = cms.double(0.00867)
process.dataDrivenTTbar_stage6.lQuark_misRateErrSSVHEMinput = cms.double(0.00126)
process.dataDrivenTTbar_stage6.effErrSSVHPTinput            = cms.double(0.00539)
process.dataDrivenTTbar_stage6.cQuark_misRateErrSSVHPTinput = cms.double(0.00582)
process.dataDrivenTTbar_stage6.lQuark_misRateErrSSVHPTinput = cms.double(0.00045)
process.dataDrivenTTbar_stage6.bQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage6.cQuark_sfErrSSVHEMinput      = cms.double(0.270)
process.dataDrivenTTbar_stage6.lQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage6.bQuark_sfErrSSVHPTinput      = cms.double(0.135)
process.dataDrivenTTbar_stage6.cQuark_sfErrSSVHPTinput      = cms.double(0.270)
process.dataDrivenTTbar_stage6.lQuark_sfErrSSVHPTinput      = cms.double(0.135)


# analysis stage
process.load("UserCode.InvisibleZ.analysis_WJets_cfi")
process.analysisWJets.muonSource = cms.untracked.InputTag("myPatMuonsPFIDIso")
process.analysisWJets.jetSource = cms.untracked.InputTag("myPatJetsAK5PFPt30noOverlapLeptons")
process.analysisWJets.bTagJetSource = cms.InputTag("myPatJetsAK5PFPt50Eta25noOverlapLeptons")
process.analysisWJets.minbTagPur = cms.double(2.0)
process.analysisWJets.minbTagVeto = cms.double(1.74)

process.analysisWJets_stage1 = process.analysisWJets.clone()
process.analysisWJets_stage1.outputPath = cms.untracked.string("stage1" + "_" + options.type + "_ANA.root")

process.analysisWJets_stage2 = process.analysisWJets.clone()
process.analysisWJets_stage2.outputPath = cms.untracked.string("stage2" + "_" + options.type + "_ANA.root")

process.analysisWJets_stage3 = process.analysisWJets.clone()
process.analysisWJets_stage3.outputPath = cms.untracked.string("stage3" + "_" + options.type + "_ANA.root")

process.analysisWJets_stage4 = process.analysisWJets.clone()
process.analysisWJets_stage4.outputPath = cms.untracked.string("stage4" + "_" + options.type + "_ANA.root")

process.analysisWJets_stage5 = process.analysisWJets.clone()
process.analysisWJets_stage5.outputPath = cms.untracked.string("stage5" + "_" + options.type + "_ANA.root")
process.analysisWJets_stage5.debug = cms.bool(True)
process.analysisWJets_stage5.verbose = cms.int32(2)

process.analysisWJets_stage6 = process.analysisWJets.clone()
process.analysisWJets_stage6.outputPath = cms.untracked.string("stage6" + "_" + options.type + "_ANA.root")

# Combined analysis stage sequence
process.analysis_stage0 = cms.Sequence(process.stdRA2ana_stage0 + process.dataDrivenTTbar_stage0)

process.analysis_stage1 = cms.Sequence(process.stdRA2ana_stage1 + process.analysisWJets_stage1 + process.dataDrivenQCD_stage1 + process.dataDrivenTTbar_stage1)

process.analysis_stage2 = cms.Sequence(process.stdRA2ana_stage2 + process.analysisWJets_stage2 + process.dataDrivenQCD_stage2 + process.dataDrivenTTbar_stage2)

process.analysis_stage3 = cms.Sequence(process.stdRA2ana_stage3 + process.analysisWJets_stage3 + process.dataDrivenQCD_stage3 + process.dataDrivenTTbar_stage3)

process.analysis_stage4 = cms.Sequence(process.stdRA2ana_stage4 + process.analysisWJets_stage4 + process.dataDrivenQCD_stage4 + process.dataDrivenTTbar_stage4)

process.analysis_stage5 = cms.Sequence(process.stdRA2ana_stage5 + process.analysisWJets_stage5 + process.dataDrivenQCD_stage5 + process.dataDrivenTTbar_stage5)

process.analysis_stage6 = cms.Sequence(process.stdRA2ana_stage6 + process.analysisWJets_stage6 + process.dataDrivenQCD_stage6 + process.dataDrivenTTbar_stage6)

# XXX: Do NOT apply the muon counter filter!!
# We cannot require only one muon for muons without isolation requirement,
# since it's obvious that we are going to have more muons without isolation...
# XXX: analysis_WJets has muon counters built-in
process.muonSel_seq = cms.Sequence(process.myPatMuonsPFIDIso*process.myPatMuonsPFIDnoIso)

if options.mcInfo == False: 
   process.myGenJetsAK5Pt50Eta25noOverlapLeptons.doGenLevel = cms.bool(False)
   process.myGenJetsAK5Pt30noOverlapLeptons.doGenLevel = cms.bool(False)
   process.myGenJetsAK5Pt50Eta25noOverlapLeptons.doGenLevel = cms.bool(False)
   process.myGenJetsAK5Pt30noOverlapLeptons.doGenLevel = cms.bool(False)
   process.myhtGenPF.JetCollection = cms.InputTag("patJetsAK5PF")
   process.mymhtGenPF.JetCollection = cms.InputTag("patJetsAK5PF")
   
#process.noOverlapJets_seq = cms.Sequence(process.myPatJetsAK5PFPt30Eta25*process.myPatJetsAK5PFPt30Eta25noOverlapLeptons*process.myPatJetsAK5PFPt50Eta25noOverlapLeptons*process.myPatJetsAK5PFPt30noOverlapLeptons*process.myPatJetsAK5PFnoOverlapLeptons*process.myGenJetsAK5Pt50Eta25noOverlapLeptons*process.myGenJetsAK5Pt30noOverlapLeptons)
process.noOverlapJets_seq = cms.Sequence(process.myPatJetsAK5PFPt50Eta25noOverlapLeptons*process.myPatJetsAK5PFPt30noOverlapLeptons*process.myPatJetsAK5PFnoOverlapLeptons*process.myGenJetsAK5Pt50Eta25noOverlapLeptons*process.myGenJetsAK5Pt30noOverlapLeptons)
process.prepareAnaPars_seq = cms.Sequence(process.myhtPF*process.mymhtPF*process.myhtGenPF*process.mymhtGenPF)

process.countJetsAK5PFPt50Eta25.src = cms.InputTag("myPatJetsAK5PFPt50Eta25noOverlapLeptons")

process.count1JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25.clone()
process.count1JetsAK5PFPt50Eta25.minNumber = cms.uint32(1)

process.count2JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25.clone()
process.count2JetsAK5PFPt50Eta25.minNumber = cms.uint32(2)

process.count3JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25.clone()
process.count3JetsAK5PFPt50Eta25.minNumber = cms.uint32(3)

process.dataDrivenQCD_stage5_MHT250 = process.dataDrivenQCD.clone()
process.dataDrivenQCD_stage5_MHT250.outputPath = cms.untracked.string("stage5" + "_MHT250_" + options.type + "_QCD.root")
process.dataDrivenTTbar_stage5_MHT250 = process.dataDrivenTTbar.clone()
process.dataDrivenTTbar_stage5_MHT250.outputPath = cms.untracked.string("stage5" + "_MHT250_" + options.type + "_TTbar.root")
process.analysisWJets_stage5_MHT250 = process.analysisWJets.clone()
process.analysisWJets_stage5_MHT250.outputPath = cms.untracked.string("stage5" + "_MHT250_" + options.type + "_ANA.root")
process.stdRA2ana_stage5_MHT250 = process.stdRA2ana.clone();
process.stdRA2ana_stage5_MHT250.outputPath = cms.untracked.string("stage5" + "_MHT250_" + options.type + "_stdRA2.root")
process.analysis_stage5_MHT250 = cms.Sequence(process.stdRA2ana_stage5_MHT250 + process.analysisWJets_stage5_MHT250 + process.dataDrivenQCD_stage5_MHT250 + process.dataDrivenTTbar_stage5_MHT250)

process.dataDrivenQCD_stage5_HT500 = process.dataDrivenQCD.clone()
process.dataDrivenQCD_stage5_HT500.outputPath = cms.untracked.string("stage5" + "_HT500_" + options.type + "_QCD.root")
process.dataDrivenTTbar_stage5_HT500 = process.dataDrivenTTbar.clone()
process.dataDrivenTTbar_stage5_HT500.outputPath = cms.untracked.string("stage5" + "_HT500_" + options.type + "_TTbar.root")
process.analysisWJets_stage5_HT500 = process.analysisWJets.clone()
process.analysisWJets_stage5_HT500.outputPath = cms.untracked.string("stage5" + "_HT500_" + options.type + "_ANA.root")
process.stdRA2ana_stage5_HT500 = process.stdRA2ana.clone();
process.stdRA2ana_stage5_HT500.outputPath = cms.untracked.string("stage5" + "_HT500_" + options.type + "_stdRA2.root")
process.analysis_stage5_HT500 = cms.Sequence(process.stdRA2ana_stage5_HT500 + process.analysisWJets_stage5_HT500 + process.dataDrivenQCD_stage5_HT500 + process.dataDrivenTTbar_stage5_HT500)


process.mymht250PFFilter = process.mymhtPFFilter.clone()
process.mymht250PFFilter.MinMHT = cms.double(250)

process.dataDrivenQCD_stage7_MHT250 = process.dataDrivenQCD.clone()
process.dataDrivenQCD_stage7_MHT250.outputPath = cms.untracked.string("stage7" + "_MHT250_" + options.type + "_QCD.root")
process.dataDrivenTTbar_stage7_MHT250 = process.dataDrivenTTbar.clone()
process.dataDrivenTTbar_stage7_MHT250.outputPath = cms.untracked.string("stage7" + "_MHT250_" + options.type + "_TTbar.root")
process.dataDrivenTTbar_stage7_MHT250.effSSVHEMinput            = cms.double(0.62597)
process.dataDrivenTTbar_stage7_MHT250.cQuark_misRateSSVHEMinput = cms.double(0.18219)
process.dataDrivenTTbar_stage7_MHT250.lQuark_misRateSSVHEMinput = cms.double(0.01915)
process.dataDrivenTTbar_stage7_MHT250.effSSVHPTinput            = cms.double(0.47052)
process.dataDrivenTTbar_stage7_MHT250.cQuark_misRateSSVHPTinput = cms.double(0.05797)
process.dataDrivenTTbar_stage7_MHT250.lQuark_misRateSSVHPTinput = cms.double(0.00234)
process.dataDrivenTTbar_stage7_MHT250.bQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage7_MHT250.cQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage7_MHT250.lQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage7_MHT250.bQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage7_MHT250.cQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage7_MHT250.lQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage7_MHT250.effErrSSVHEMinput            = cms.double(0.01181)
process.dataDrivenTTbar_stage7_MHT250.cQuark_misRateErrSSVHEMinput = cms.double(0.01756)
process.dataDrivenTTbar_stage7_MHT250.lQuark_misRateErrSSVHEMinput = cms.double(0.00271)
process.dataDrivenTTbar_stage7_MHT250.effErrSSVHPTinput            = cms.double(0.01218)
process.dataDrivenTTbar_stage7_MHT250.cQuark_misRateErrSSVHPTinput = cms.double(0.01063)
process.dataDrivenTTbar_stage7_MHT250.lQuark_misRateErrSSVHPTinput = cms.double(0.00096)
process.dataDrivenTTbar_stage7_MHT250.bQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage7_MHT250.cQuark_sfErrSSVHEMinput      = cms.double(0.270)
process.dataDrivenTTbar_stage7_MHT250.lQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage7_MHT250.bQuark_sfErrSSVHPTinput      = cms.double(0.135)
process.dataDrivenTTbar_stage7_MHT250.cQuark_sfErrSSVHPTinput      = cms.double(0.270)
process.dataDrivenTTbar_stage7_MHT250.lQuark_sfErrSSVHPTinput      = cms.double(0.135)

process.analysisWJets_stage7_MHT250 = process.analysisWJets.clone()
process.analysisWJets_stage7_MHT250.outputPath = cms.untracked.string("stage7" + "_MHT250_" + options.type + "_ANA.root")
process.stdRA2ana_stage7_MHT250 = process.stdRA2ana.clone();
process.stdRA2ana_stage7_MHT250.outputPath = cms.untracked.string("stage7" + "_MHT250_" + options.type + "_stdRA2.root")
process.analysis_stage7_MHT250 = cms.Sequence(process.stdRA2ana_stage7_MHT250 + process.analysisWJets_stage7_MHT250 + process.dataDrivenQCD_stage7_MHT250 + process.dataDrivenTTbar_stage7_MHT250)

process.mymhtPFFilter_HT500 = process.mymhtPFFilter.clone()

process.myht500PFFilter = process.myhtPFFilter.clone()
process.myht500PFFilter.MinHT = cms.double(500)

process.dataDrivenQCD_stage7_HT500 = process.dataDrivenQCD.clone()
process.dataDrivenQCD_stage7_HT500.outputPath = cms.untracked.string("stage7" + "_HT500_" + options.type + "_QCD.root")
process.dataDrivenTTbar_stage7_HT500 = process.dataDrivenTTbar.clone()
process.dataDrivenTTbar_stage7_HT500.outputPath = cms.untracked.string("stage7" + "_HT500_" + options.type + "_TTbar.root")
process.dataDrivenTTbar_stage7_HT500.effSSVHEMinput            = cms.double(0.63428)
process.dataDrivenTTbar_stage7_HT500.cQuark_misRateSSVHEMinput = cms.double(0.20815)
process.dataDrivenTTbar_stage7_HT500.lQuark_misRateSSVHEMinput = cms.double(0.02239)
process.dataDrivenTTbar_stage7_HT500.effSSVHPTinput            = cms.double(0.48372)
process.dataDrivenTTbar_stage7_HT500.cQuark_misRateSSVHPTinput = cms.double(0.09227)
process.dataDrivenTTbar_stage7_HT500.lQuark_misRateSSVHPTinput = cms.double(0.00352)
process.dataDrivenTTbar_stage7_HT500.bQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage7_HT500.cQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage7_HT500.lQuark_sfSSVHEMinput      = cms.double(0.90)
process.dataDrivenTTbar_stage7_HT500.bQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage7_HT500.cQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage7_HT500.lQuark_sfSSVHPTinput      = cms.double(0.90)
process.dataDrivenTTbar_stage7_HT500.effErrSSVHEMinput            = cms.double(0.01086)
process.dataDrivenTTbar_stage7_HT500.cQuark_misRateErrSSVHEMinput = cms.double(0.01881)
process.dataDrivenTTbar_stage7_HT500.lQuark_misRateErrSSVHEMinput = cms.double(0.00265)
process.dataDrivenTTbar_stage7_HT500.effErrSSVHPTinput            = cms.double(0.01127)
process.dataDrivenTTbar_stage7_HT500.cQuark_misRateErrSSVHPTinput = cms.double(0.01341)
process.dataDrivenTTbar_stage7_HT500.lQuark_misRateErrSSVHPTinput = cms.double(0.00106)
process.dataDrivenTTbar_stage7_HT500.bQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage7_HT500.cQuark_sfErrSSVHEMinput      = cms.double(0.270)
process.dataDrivenTTbar_stage7_HT500.lQuark_sfErrSSVHEMinput      = cms.double(0.135)
process.dataDrivenTTbar_stage7_HT500.bQuark_sfErrSSVHPTinput      = cms.double(0.135)
process.dataDrivenTTbar_stage7_HT500.cQuark_sfErrSSVHPTinput      = cms.double(0.270)
process.dataDrivenTTbar_stage7_HT500.lQuark_sfErrSSVHPTinput      = cms.double(0.135)

process.analysisWJets_stage7_HT500 = process.analysisWJets.clone()
process.analysisWJets_stage7_HT500.outputPath = cms.untracked.string("stage7" + "_HT500_" + options.type + "_ANA.root")
process.stdRA2ana_stage7_HT500 = process.stdRA2ana.clone();
process.stdRA2ana_stage7_HT500.outputPath = cms.untracked.string("stage7" + "_HT500_" + options.type + "_stdRA2.root")
process.analysis_stage7_HT500 = cms.Sequence(process.stdRA2ana_stage7_HT500 + process.analysisWJets_stage7_HT500 + process.dataDrivenQCD_stage7_HT500 + process.dataDrivenTTbar_stage7_HT500)

# Other sequence
process.comb_seq = cms.Sequence(
# All cleaning && all basic variables, e.g., mht, ht...
#   process.ppf *
# hlt requirement
   process.hltFilter *
# event weight
   process.weightProducer *
# noOverlap cleaning (muonSel has only iso muon reconstructed, there is no selection cut!)
   process.muonSel_seq * process.noOverlapJets_seq *
# produce HT and MHT
   process.prepareAnaPars_seq *
# no any cuts (must after prepareAnaPars_seq)  
   process.analysis_stage0 *
# Electron veto (all default settings by Steven && with patElectronsPF as input)
   process.leptonVetoPOGnoMu *
# >= 1 PF jets (pt>50 && |eta|<2.5)
   process.count1JetsAK5PFPt50Eta25 *
# Analysis at this stage 
   process.analysis_stage1 *
# >= 2 PF jets (pt>50 && |eta|<2.5)
   process.count2JetsAK5PFPt50Eta25 *
# Analysis at this stage
   process.analysis_stage2 *
# >= 3 PF jets (pt>50 && |eta|<2.5)
   process.count3JetsAK5PFPt50Eta25 *
# Analysis at stage3
   process.analysis_stage3 *
# HT filter
   process.myhtPFFilter *
# Analysis at this stage
   process.analysis_stage4 #*
# dPhi filter
#   process.myjetMHTDPhiFilter
)

# Fix a bug if load PhysicsTools.PatAlgos.patTemplate_cfg, but no output path used
process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('*') )

process.mht150_baseline_path = cms.Path(process.comb_seq * process.myjetMHTDPhiFilter * process.analysis_stage5 * process.mymhtPFFilter * process.analysis_stage6)
process.mht250_path          = cms.Path(process.comb_seq * process.myjetMHTDPhiFilter * process.analysis_stage5_MHT250 * process.mymht250PFFilter * process.analysis_stage7_MHT250)
process.ht500_path           = cms.Path(process.comb_seq * process.myjetMHTDPhiFilter * process.myht500PFFilter * process.analysis_stage5_HT500 * process.mymhtPFFilter_HT500 * process.analysis_stage7_HT500)

# For JetMET

process.myjetMHTDPhiFilter_BEonly = process.myjetMHTDPhiFilter.clone()
process.myjetMHTDPhiFilter_TPonly = process.myjetMHTDPhiFilter.clone()
process.myjetMHTDPhiFilter_BEandTP = process.myjetMHTDPhiFilter.clone()

process.stdRA2ana_stage4_BEonly = process.stdRA2ana.clone()
process.stdRA2ana_stage4_BEonly.outputPath = cms.untracked.string("stage4" + "_BEonly_" + options.type + "_stdRA2.root")

process.stdRA2ana_stage4_TPonly = process.stdRA2ana.clone()
process.stdRA2ana_stage4_TPonly.outputPath = cms.untracked.string("stage4" + "_TPonly_" + options.type + "_stdRA2.root")

process.stdRA2ana_stage4_BEandTP = process.stdRA2ana.clone()
process.stdRA2ana_stage4_BEandTP.outputPath = cms.untracked.string("stage4" + "_BEandTP_" + options.type + "_stdRA2.root")

process.stdRA2ana_stage5_BEonly = process.stdRA2ana.clone()
process.stdRA2ana_stage5_BEonly.outputPath = cms.untracked.string("stage5" + "_BEonly_" + options.type + "_stdRA2.root")

process.stdRA2ana_stage5_TPonly = process.stdRA2ana.clone()
process.stdRA2ana_stage5_TPonly.outputPath = cms.untracked.string("stage5" + "_TPonly_" + options.type + "_stdRA2.root")

process.stdRA2ana_stage5_BEandTP = process.stdRA2ana.clone()
process.stdRA2ana_stage5_BEandTP.outputPath = cms.untracked.string("stage5" + "_BEandTP_" + options.type + "_stdRA2.root")

#process.BEonly_path  = cms.Path(process.comb_seq * process.ecalAnomalousFilter * process.ecalDeadCellBEonlyFilter * process.stdRA2ana_stage4_BEonly * process.myjetMHTDPhiFilter_BEonly * process.stdRA2ana_stage5_BEonly)
#process.TPonly_path  = cms.Path(process.comb_seq * process.ecalAnomalousFilter * process.ecalDeadCellTPonlyFilter * process.stdRA2ana_stage4_TPonly * process.myjetMHTDPhiFilter_TPonly * process.stdRA2ana_stage5_TPonly)

#process.ecalDeadCellBEandTPfilter = process.ecalDeadCellFilter.clone()
#process.BEandTP_path = cms.Path(process.comb_seq * process.ecalAnomalousFilter * process.ecalDeadCellBEandTPfilter * process.stdRA2ana_stage4_BEandTP * process.myjetMHTDPhiFilter_BEandTP * process.stdRA2ana_stage5_BEandTP)

##process.analysis_path = cms.Path(process.comb_seq)

process.schedule = cms.Schedule( process.mht150_baseline_path, process.mht250_path, process.ht500_path )
