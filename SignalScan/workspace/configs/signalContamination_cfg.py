from SusyAnalysis.RA2SusyScanAnalysis.patTemplate_cfg import *
# Message Logger ------------------------------------------------------------
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
   limit = cms.untracked.int32(-1),
   reportEvery = cms.untracked.int32(1)
)

import os
import sys
import re

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('standard')

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

options.register('type', 'signalScan', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "key output type string")

options.register('doTPBEfilter', 1, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "apply Ecal dead cell TP||BE filter, default is True")

options.register('doPtHatWeighting', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "PtHat weighting for QCD flat samples, default is False")

options.register('puMCfile', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "pu MC file name")

options.register('puDatafile', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "pu Data file name")

options.register('reWeightPUmethod', 'weight3D', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "reWeight PU method")

options.register('susyScanType', 'mSUGRA', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "SUSY scan type")
options.register('susyScanTopology', 'T1', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "SUSY scan topology")

options.register('applyTemplateMethod', 0, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "default is ptDep")

options.register('doHLTeff', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "doHLTeff")

options.register('applySgnfFilter', 0, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "applySgnfFilter or not. Have to apply on QCD to avoid crash. We'd like sgnf always filled.")

options.register('fileslist', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "name of a file with input source list")

options.register('applyinconsistentMuons', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "apply inconsistentMuons filter")

options.register('skipEvents', 0, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "number of events to skip. for debug purpose and can only be enabled if debug set to be True")


options.parseArguments()
options._tagOrder =[]

print options
 
#-- Message Logger ------------------------------------------------------------
process.MessageLogger.cerr.FwkReport.reportEvery = 100
if options.debug:
   process.MessageLogger.cerr.FwkReport.reportEvery = 1
   process.source.skipEvents = cms.untracked.uint32(options.skipEvents) 

#-- Input Source --------------------------------------------------------------
inputfiles = cms.untracked.vstring()
if options.fileslist:
   if os.path.exists(options.fileslist) == False or os.path.isfile(options.fileslist) == False:
      print 'fileslist ',options.fileslist,' does not exist\n'
      sys.exit(5)
   else:
      ifile = open(options.fileslist, 'r')
      for line in ifile.readlines():
         inputfiles.append(line)

print "inputfiles : \n", inputfiles, "\n"

if options.fileslist:
   process.source.fileNames = inputfiles
elif options.files:
   process.source.fileNames = options.files
else:
   process.source.fileNames = [
#      "/store/mc/Summer11/SMS-T1_Mgluino-100to1200_mLSP-50to1150_7TeV-Pythia6Z/AODSIM/PU_START42_V11_FastSim-v1/0000/FEE0CBF9-8BBF-E011-AF6B-00215E2216A4.root",
#      "/store/mc/Summer11/SMS-T2_Mgluino-100to1200_mLSP-50to1150_7TeV-Pythia6Z/AODSIM/PU_START42_V11_FastSim-v1/0000/FEB08D76-F0BF-E011-877F-E41F131816A8.root",
      "/store/results/susy/mSUGRA_m0-220to3000_m12-100to1000_tanb-10andA0-0_7TeV-Pythia6Z/StoreResults-PU_START42_V11_FastSim-v5/mSUGRA_m0-220to3000_m12-100to1000_tanb-10andA0-0_7TeV-Pythia6Z/USER/StoreResults-PU_START42_V11_FastSim-v5/0000/92A4CB7A-3231-E111-AD7B-002618943922.root",
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

# Load RA2 related configuration
process.load('SandBox.Skims.RA2Objects_cff')
process.load('SandBox.Skims.RA2Selection_cff')

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

if options.mcInfo == True:
   process.patMETsTypeIPF.corrector = cms.string('ak5PFL2L3')
else:
   process.patMETsTypeIPF.corrector = cms.string('ak5PFL2L3Residual')

# For BE filter
process.load('UserCode.SkimsRA2.ra2BEonlyFilter_cfi')
process.ra2BEonlyFilter.ecalAnomalousFilterTag         = cms.InputTag("ecalAnomalousFilter","anomalousECALVariables")
process.ra2BEonlyFilter.maxBoundaryEnergy              = cms.double(10)
process.ra2BEonlyFilter.limitDeadCellToChannelStatusEB = cms.vint32(12,14)
process.ra2BEonlyFilter.limitDeadCellToChannelStatusEE = cms.vint32(12,14)
# For TP filter
process.load('JetMETAnalysis.ecalDeadCellTools.RA2TPfilter_cff')
process.ra2TPonlyFilter = process.ecalDeadCellTPfilter.clone()

process.dummyCounterStdRA2 = cms.EDProducer("EventCountProducer")
process.dummyCounterHadTau = cms.EDProducer("EventCountProducer")

process.load('SandBox.Skims.jetIDFailureFilter_cfi')
process.jetIDFailure_hadTau = process.jetIDFailure.clone()
process.jetIDFailure_stdRA2 = process.jetIDFailure.clone()

process.jetIDFailure_hadTau.JetSource = cms.InputTag("myPatJetsAK5PFPt30noOverlapLeptons")
process.jetIDFailure_stdRA2.JetSource = cms.InputTag("patJetsAK5PFPt30")

# A filter to reject events non conserving 4-momentum beyond a user specified absolute tolerance
process.load("GeneratorInterface.GenFilters.TotalKinematicsFilter_cfi")

if options.applyinconsistentMuons == False:
   process.ra2PostCleaning.remove(process.inconsistentMuons)

process.ra2AllCleaning_stdRA2 = cms.Sequence( process.ra2PostCleaning * process.jetIDFailure_stdRA2 * process.dummyCounterStdRA2)
process.ra2AllCleaning_hadTau = cms.Sequence( process.ra2PostCleaning * process.jetIDFailure_hadTau * process.dummyCounterHadTau)

if options.mcInfo == True:
   process.ra2AllCleaning_stdRA2.replace(process.dummyCounterStdRA2, process.totalKinematicsFilter*process.dummyCounterStdRA2)
   process.ra2AllCleaning_hadTau.replace(process.dummyCounterHadTau, process.totalKinematicsFilter*process.dummyCounterHadTau)

if options.doTPBEfilter == 1:
   process.ra2AllCleaning_stdRA2.replace(process.dummyCounterStdRA2, process.ra2TPonlyFilter*process.dummyCounterStdRA2)
   process.ra2AllCleaning_hadTau.replace(process.dummyCounterHadTau, process.ra2TPonlyFilter*process.dummyCounterHadTau)
elif options.doTPBEfilter == 2:
   process.ra2AllCleaning_stdRA2.replace(process.dummyCounterStdRA2, process.ra2BEonlyFilter*process.dummyCounterStdRA2)
   process.ra2AllCleaning_hadTau.replace(process.dummyCounterHadTau, process.ra2BEonlyFilter*process.dummyCounterHadTau)
elif options.doTPBEfilter == 3:
   process.ra2AllCleaning_stdRA2.replace(process.dummyCounterStdRA2, process.ra2TPonlyFilter*process.ra2BEonlyFilter*process.dummyCounterStdRA2)
   process.ra2AllCleaning_hadTau.replace(process.dummyCounterHadTau, process.ra2TPonlyFilter*process.ra2BEonlyFilter*process.dummyCounterHadTau)

# This is a temp fix for the met significance crash
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMETSignificance
process.load('UserCode.SkimsRA2.badMETsgnfFilter_cfi')
if options.applySgnfFilter == 1:
   process.ra2AllCleaning_stdRA2.replace(process.dummyCounterStdRA2, process.badMETsgnfFilter*process.dummyCounterStdRA2)
   process.ra2AllCleaning_hadTau.replace(process.dummyCounterHadTau, process.badMETsgnfFilter*process.dummyCounterHadTau)

process.load('UserCode.SkimsRA2.weightProducer_cfi')
process.myweightProducer.inputPUfileMC   = cms.untracked.string("")
process.myweightProducer.inputPUfileMC   = options.puMCfile
process.myweightProducer.inputPUfileData = cms.untracked.string("")
process.myweightProducer.inputPUfileData = options.puDatafile
if options.doPtHatWeighting:
   process.myweightProducer.Method     = cms.string("PtHat")
   process.myweightProducer.Exponent   = cms.double(-4.5)
   process.myweightProducer.XS         = cms.double(1.0)
   process.myweightProducer.NumberEvts = cms.double(1.0)
   process.myweightProducer.Lumi       = cms.double(1.0)
   process.myweightProducer.weightWARNingUpThreshold  = cms.double(2.0)
process.myweightProducer.reWeightPUmethod  = cms.untracked.string("")
process.myweightProducer.reWeightPUmethod  = options.reWeightPUmethod

process.load('UserCode.InvisibleZ.mySusyRA2GenCuts_cff')
process.mymhtPF.JetCollection = cms.InputTag("myPatJetsAK5PFPt30noOverlapLeptons")
process.myhtPF.JetCollection = cms.InputTag("myPatJetsAK5PFPt50Eta25noOverlapLeptons")
process.myjetMHTDPhiFilter.JetSource = cms.InputTag("myPatJetsAK5PFPt30noOverlapLeptons")

if options.mcInfo == False:
   process.myGenJetsAK5Pt50Eta25noOverlapLeptons.doGenLevel = cms.bool(False)
   process.myGenJetsAK5Pt30noOverlapLeptons.doGenLevel = cms.bool(False)
   process.myGenJetsAK5Pt50Eta25noOverlapLeptons.doGenLevel = cms.bool(False)
   process.myGenJetsAK5Pt30noOverlapLeptons.doGenLevel = cms.bool(False)
   process.myhtGenPF.JetCollection = cms.InputTag("patJetsAK5PF")
   process.mymhtGenPF.JetCollection = cms.InputTag("patJetsAK5PF")

process.myPatMuonsPFIDnoIso = process.myPatMuonsPFIDIso.clone()
process.myPatMuonsPFIDnoIso.DoMuonIsolation = False
process.muonSel_seq = cms.Sequence(process.myPatMuonsPFIDIso*process.myPatMuonsPFIDnoIso)
process.myPatMuonsPFIDIso.MinMuPt = cms.double(20)
process.myPatMuonsPFIDIso.MaxMuEta = cms.double(2.1)

process.load("UserCode.SkimsRA2.signifMHTProducer_cfi")
process.mymhtPFforSgnf.resolutionsAlgo = cms.string("AK5PF")
process.mymhtPFforSgnf.resolutionsEra  = cms.string("Spring10")

process.noOverlapJets_seq = cms.Sequence(process.myPatJetsAK5PFPt50Eta25noOverlapLeptons + process.myPatJetsAK5PFPt30noOverlapLeptons + process.myPatJetsAK5PFnoOverlapLeptons + process.myGenJetsAK5Pt50Eta25noOverlapLeptons + process.myGenJetsAK5Pt30noOverlapLeptons)
process.prepareAnaPars_seq = cms.Sequence(process.myhtPF + process.mymhtPF + process.myhtGenPF + process.mymhtGenPF + process.mymhtPFforSgnf)

process.countJetsAK5PFPt50Eta25Cloned = process.countJetsAK5PFPt50Eta25.clone()
process.countJetsAK5PFPt50Eta25Cloned.src = cms.InputTag("myPatJetsAK5PFPt50Eta25noOverlapLeptons")

process.count1JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25Cloned.clone()
process.count1JetsAK5PFPt50Eta25.minNumber = cms.uint32(1)

process.count2JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25Cloned.clone()
process.count2JetsAK5PFPt50Eta25.minNumber = cms.uint32(2)

process.count3JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25Cloned.clone()
process.count3JetsAK5PFPt50Eta25.minNumber = cms.uint32(3)

process.load("UserCode.SkimsRA2.muonEffProducer_cfi")
process.muonEffProducer.jetSrc           = cms.InputTag("patJetsAK5PFPt30")
process.muonEffProducer.caloJetSrc        = cms.InputTag("ak5CaloJetsL2L3")
process.muonEffProducer.muonSrc          = cms.InputTag("myPatMuonsPFIDIso")
process.muonEffProducer.effFileName      = cms.string("fit_Muon_xx_Comb.root")
process.muonEffProducer.applyMethods     = cms.vint32(0, 0)
process.muonEffProducer.jetSelection     = cms.string("pt>0")
process.muonEffProducer.isoMinDR         = cms.double(0.05)
process.muonEffProducer.isoEffHistNames  = cms.vstring("muonEffs/ISO_DRJan_Pt/probe_drjJan_probe_pt_PLOT")
process.muonEffProducer.recoEffHistNames = cms.vstring("muonEffs/ID_pt_eta/probe_pt_probe_eta_PLOT")
process.muonEffProducer.hltEffHistNames  = cms.vstring("muonEffsHLT/HLTIsoMu17_pt_eta/probe_pt_probe_eta_PLOT")

process.muonEffProducerMC = process.muonEffProducer.clone()
process.muonEffProducerMC.effFileName      = cms.string("fit_Muon_MC_Comb.root")

process.muonEffProducerData = process.muonEffProducer.clone()
process.muonEffProducerData.effFileName      = cms.string("fit_Muon_Data_Comb.root")

if options.doHLTeff == True:
   process.muonEffProducerMC.doHLTeff      = cms.untracked.bool(True)
   process.muonEffProducerData.doHLTeff      = cms.untracked.bool(True)
else:
   process.muonEffProducerMC.doHLTeff      = cms.untracked.bool(False)
   process.muonEffProducerData.doHLTeff      = cms.untracked.bool(False)

process.prepareMuonEffs_seq = cms.Sequence(process.muonEffProducerMC + process.muonEffProducerData)

# Hadronic tau
process.load("UserCode.SignalScan.HadronicTauSignalContamination_cfi")
process.HadronicTauSignalContamination.muonSource = cms.InputTag("myPatMuonsPFIDIso")
process.HadronicTauSignalContamination.eleSource  = cms.InputTag("patElectronsPFIDIso")
process.HadronicTauSignalContamination.jetSource  = cms.InputTag("myPatJetsAK5PFnoOverlapLeptons")
process.HadronicTauSignalContamination.jetMHTcuts          = cms.vdouble(30, 5)
process.HadronicTauSignalContamination.jetHTcuts           = cms.vdouble(50, 2.5)
process.HadronicTauSignalContamination.cutJetMHTDPhivals   = cms.vdouble(0.5, 0.5, 0.3)
process.HadronicTauSignalContamination.templateRootName    = cms.string("tauTemplate.root")
process.HadronicTauSignalContamination.mhtInputTag         = cms.InputTag("mymhtPF")
process.HadronicTauSignalContamination.htInputTag          = cms.InputTag("myhtPF")
process.HadronicTauSignalContamination.metInputTag         = cms.InputTag("patMETsPF")
process.HadronicTauSignalContamination.Ntry                = cms.int32(100)
process.HadronicTauSignalContamination.evtWeightInput      = cms.InputTag("myweightProducer:weight")

if options.mcInfo == True:
   process.HadronicTauSignalContamination.effWeightsInput     = cms.InputTag("muonEffProducerMC:effWeights")
   process.HadronicTauSignalContamination.effWeightErr2sInput = cms.InputTag("muonEffProducerMC:effWeightErr2s")
else:
   process.HadronicTauSignalContamination.effWeightsInput     = cms.InputTag("muonEffProducerData:effWeights")
   process.HadronicTauSignalContamination.effWeightErr2sInput = cms.InputTag("muonEffProducerData:effWeightErr2s")

# MT cut filter
process.load("UserCode.SkimsRA2.MTFilter_cfi")
process.MTFilter.muonSource  = cms.InputTag("myPatMuonsPFIDIso")
process.MTFilter.metInputTag = cms.InputTag("patMETsPF")
process.MTFilter.cutMTvals   = cms.vdouble(0, 100)

# Lost-lepton
process.load("RA2.Selection.RA2Selection_cff")

process.RA2FinalSelection.selections.selectors.LeptonVeto.invertVeto = cms.bool(True)
process.RA2FinalSelection.selections.selectors.LeptonVeto.MTWcut = cms.bool(True)

process.load("RA2.SemiLepTTbarBkgProd.LeptonSelection_cff")
process.load("RA2.SemiLepTTbarBkgProd.JetSelection_cff")

#################################################################
##Leptonic Background Estimation Producer (ttbar & W+jets)
################################################################
process.load("RA2.SemiLepTTbarBkgProd.semilepttbarbkgprod_cfi")

process.SemiLepTTbarBkgProd.JetTag = 'cleanPatJetsAK5Calo'
process.SemiLepTTbarBkgProd.MuonTag = process.PatTagNames.muonTag
process.SemiLepTTbarBkgProd.ElectronTag = process.PatTagNames.electronTag

process.SemiLepTTbarBkgProd.EfficiencyInputFile = "Hongxuan3_NewISORECO_combAcc_DATA.root"
process.SemiLepTTbarBkgProd.FileName = "output.root"

process.load("RA2.WeightProducer.weightproducer_cfi")

process.LostLep_seq = cms.Sequence(
   process.weightProducer *
   process.RA2FinalSelection *
   process.SemiLepTTbarBkgProd
)

process.load("UserCode.SignalScan.RA2SignalScan_cfi")
process.RA2SignalScan.vtxSrc              = cms.InputTag("goodVerticesRA2")
process.RA2SignalScan.evtWeightInput      = cms.InputTag("myweightProducer:weight")
process.RA2SignalScan.nVtxPUcut           = cms.uint32(4)

process.RA2SignalScan.metSrc              = cms.InputTag("patMETsPF")
process.RA2SignalScan.mhtSrc              = cms.InputTag("mhtPF")
process.RA2SignalScan.mht_forSgnfSrc      = cms.InputTag("mhtPF")
process.RA2SignalScan.htSrc               = cms.InputTag("htPF")

process.RA2SignalScan.debug               = cms.bool(False)

process.RA2SignalScan.fillTotalOnly       = cms.bool(False)

process.RA2SignalScan.cutLoHTvals         = cms.vdouble( 350, 500, 800, 800, 500, 1200, 1000, 500, 500, 500, 500,  800,  800,  800,  800, 1000, 1000, 1000, 1200, 1200, 1400 )
process.RA2SignalScan.cutHiHTvals         = cms.vdouble(  -1,  -1,  -1,  -1,  -1,   -1,   -1, 800, 800, 800, 800, 1000, 1000, 1000, 1000, 1200, 1200, 1200, 1400, 1400,   -1 )

process.RA2SignalScan.cutLoMHTvals        = cms.vdouble( 200, 200, 200, 500, 350,  400,  600, 200, 350, 500, 600,  200,  350,  500,  600,  200,  350,  500,  200,  350,  200 )
process.RA2SignalScan.cutHiMHTvals        = cms.vdouble(  -1,  -1,  -1,  -1,  -1,   -1,   -1, 350, 500, 600,  -1,  350,  500,  600,   -1,  350,  500,   -1,  350,   -1,   -1 )

if options.susyScanType == "mSUGRA":
   process.RA2SignalScan.xAxisDivs           = cms.vdouble(  0, 5000)
   process.RA2SignalScan.xAxisBins           =  cms.vint32(   250   )
   process.RA2SignalScan.yAxisDivs           = cms.vdouble(  0, 1500)
   process.RA2SignalScan.yAxisBins           =  cms.vint32(    75   )
elif options.susyScanType == "SMS":
   process.RA2SignalScan.xAxisDivs           = cms.vdouble(  0, 1500)
   process.RA2SignalScan.xAxisBins           =  cms.vint32(    60   )
   process.RA2SignalScan.yAxisDivs           = cms.vdouble(  0, 1500)
   process.RA2SignalScan.yAxisBins           =  cms.vint32(    60   )

process.RA2SignalScan.susyScanType        = options.susyScanType
process.RA2SignalScan.susyScanTopology    = options.susyScanTopology

# RA2 sequences
process.ra2PFpat_seq = cms.Sequence(
                      process.susyPatDefaultSequence *
                      process.ra2Objects
                     )

# HBHENoiseFilter is not stored in FastSim samples
process.ra2PostCleaning.remove(process.HBHENoiseFilter)

process.aux_seq = cms.Sequence(
# hlt requirement
   process.hltFilter *
   process.ra2StdCleaning *
# event weight
   process.myweightProducer
)

process.aux_hadTau_seq = cms.Sequence(
   process.muonSel_seq * process.noOverlapJets_seq *
   process.prepareAnaPars_seq * process.prepareMuonEffs_seq
)

process.ra2LeptonVeto_seq = cms.Sequence(
   process.ra2PFMuonVeto * process.ra2PFElectronVeto
)

# nominal ra2 sequence for acceptance
process.ra2Selection  = cms.Sequence(
   process.htPFFilter *
   process.mhtPFFilter *
   process.countJetsAK5PFPt50Eta25 *
   process.jetMHTPFDPhiFilter
)


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('ra2AnalysisScan.root')
)

#=================== No cuts total statistics =======================================
# just get the total no. of events witout any cuts
process.RA2SignalScan_total = process.RA2SignalScan.clone()
process.RA2SignalScan_total.fillTotalOnly = cms.bool(True)
process.RA2SignalScan_total.printINFO     = cms.untracked.bool(True)

#=================== Nominal RA2 ====================================================
# analysis after nominal RA2 selection & nominal JES
process.RA2SignalScan_stdRA2 = process.RA2SignalScan.clone()

process.RA2HadTau_SC = process.RA2SignalScan.clone()
process.RA2HadTau_SC.doSignalSC = cms.int32(1)
process.RA2HadTau_SC.hadTauSCweightInputTag = cms.InputTag("HadronicTauSignalContamination:predictions")

process.RA2LostLep_SC = process.RA2SignalScan.clone()
process.RA2LostLep_SC.doSignalSC = cms.int32(2)
process.RA2LostLep_SC.lostLepSCweightInputTag = cms.InputTag("SemiLepTTbarBkgProd:LeptonicBgkWeightMuCS")

process.RA2SignalScan_muonCS = process.RA2SignalScan.clone()
process.RA2SignalScan_muonCS.mhtSrc          = cms.InputTag("mymhtPF")
process.RA2SignalScan_muonCS.mht_forSgnfSrc  = cms.InputTag("mymhtPF")
process.RA2SignalScan_muonCS.htSrc           = cms.InputTag("myhtPF")

# Add whatever process we want manually!
process.total_path = cms.Path( process.myweightProducer * process.goodVerticesRA2 * process.RA2SignalScan_total )

process.scan_stdRA2_path = cms.Path(
                              process.aux_seq * process.ra2PFpat_seq * process.ra2AllCleaning_stdRA2 * process.ra2Selection * 
                              process.ra2LeptonVeto_seq * process.RA2SignalScan_stdRA2 
)

process.count3JetsAK5PFPt50Eta25_muonCS = process.count3JetsAK5PFPt50Eta25.clone()
process.myjetMHTDPhiFilter_muonCS = process.myjetMHTDPhiFilter.clone()
process.myhtPFFilter_muonCS = process.myhtPFFilter.clone()
process.mymhtPFFilter_muonCS = process.mymhtPFFilter.clone()
process.scan_muonCS_path = cms.Path(
                              process.aux_seq * process.ra2PFpat_seq * 
                              process.aux_hadTau_seq * process.ra2AllCleaning_hadTau * 
                              process.count2JetsAK5PFPt50Eta25 * process.leptonVetoPOGnoMu * process.countMyPatMuonsPFIDIsoFilter * process.MTFilter * 
                              process.count3JetsAK5PFPt50Eta25_muonCS * process.myjetMHTDPhiFilter_muonCS * process.myhtPFFilter_muonCS * process.mymhtPFFilter_muonCS *
                              process.RA2SignalScan_muonCS
)

process.scan_lostLepSC_path = cms.Path(
                              process.aux_seq * process.ra2PFpat_seq * process.ra2AllCleaning_stdRA2 * process.ra2Selection * 
                              process.LostLep_seq * process.RA2LostLep_SC
)

process.scan_hadTauSC_path = cms.Path(
                              process.aux_seq * process.ra2PFpat_seq * 
                              process.aux_hadTau_seq * process.ra2AllCleaning_hadTau * 
                              process.count2JetsAK5PFPt50Eta25 * process.leptonVetoPOGnoMu * process.countMyPatMuonsPFIDIsoFilter * 
                              process.HadronicTauSignalContamination * process.RA2HadTau_SC
)
