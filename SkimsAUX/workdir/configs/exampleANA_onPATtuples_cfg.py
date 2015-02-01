from UserCode.SkimsRA2.patTemplate_cfg import *

# Message Logger ------------------------------------------------------------
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
   limit = cms.untracked.int32(-1),
   reportEvery = cms.untracked.int32(1)
)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

import os
import sys
import re

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('standard')

options.output = "SUSYPAT.root"

options.register('GlobalTag', "START52_V11C::All", VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "GlobaTTag to use (otherwise default Pat GT is used)")
options.register('mcInfo', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "process MonteCarlo data, default is data")
options.register('jetCorrections', 'L2Relative', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Level of jet corrections to use: Note the factors are read from DB via GlobalTag")
options.jetCorrections.append('L3Absolute')

options.register('hltName', 'HLT', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "HLT menu to use for trigger matching, e.g., HLT, REDIGI311X") 
options.register('mcVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific MC fix")
options.register('jetTypes', 'AK5PF', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional jet types that will be produced (AK5Calo and AK5PF, cross cleaned in PF2PAT, are included anyway)")
#options.jetTypes.append('AK5Calo')
options.register('hltSelection', '*', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "hlTriggers (OR) used to filter events. for data: ''HLT_Mu9', 'HLT_IsoMu9', 'HLT_IsoMu13_v*''; for MC, HLT_Mu9")

options.register('dataVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific DATA fix")

options.register('debug', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "switch on/off debug mode")

options.register('type', 'muonCS', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "key output type string")

options.register('doTPBEfilter', 1, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "apply Ecal dead cell TP||BE filter. 1 : TP only  2 : BE only  3 : TP||BE")

options.register('doPtHatWeighting', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "PtHat weighting for QCD flat samples, default is False")

options.register('puMCfile', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "pu MC file name")

options.register('puDatafile', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "pu Data file name")

options.register('dataTier', 'AOD', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "data tier string, e.g., AOD, RECO")

options.register('applySgnfFilter', 0, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "applySgnfFilter or not. Have to apply on QCD to avoid crash. We'd like sgnf always filled.")
options.register('storeSgnf', 1, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "store significance or not. In case we don't wanna apply the significance filter and also don't want the code aborted.")

options.register('fileslist', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "name of a file with input source list")

options.register('doSignalScan', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "do signalScan, default is False")
options.register('susyScanType', 'SMS', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "SUSY scan type")
options.register('susyScanTopology', 'T2tt', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "SUSY scan topology")

options.register('fastsim', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "fastsim sample or not, default is False")

options.register('signalFastScan', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "signalFastScan sample or not, default is False")

options.parseArguments()
options._tagOrder =[]

print options
 
#-- Message Logger ------------------------------------------------------------
process.MessageLogger.cerr.FwkReport.reportEvery = 100
if options.debug:
   process.MessageLogger.cerr.FwkReport.reportEvery = 1

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

if options.files:
   process.source.fileNames = options.files
elif options.fileslist:
   process.source.fileNames = inputfiles
else:
   process.source.fileNames = [
   ]

#process.source.skipEvents = cms.untracked.uint32(165)
process.source.inputCommands = cms.untracked.vstring( "keep *", "drop *_MEtoEDMConverter_*_*" )
process.maxEvents.input = options.maxEvents

# Calibration tag -----------------------------------------------------------
if options.GlobalTag:
   process.GlobalTag.globaltag = options.GlobalTag

# JEC
if options.mcInfo == False: options.jetCorrections.append('L2L3Residual')
options.jetCorrections.insert(0, 'L1FastJet')

print "jetCorrections: "
print options.jetCorrections

# Default hltFilter with path "*"
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
if options.hltSelection:
   process.hltFilter = hlt.hltHighLevel.clone(
      TriggerResultsTag = cms.InputTag("TriggerResults","",options.hltName),
      HLTPaths = cms.vstring(options.hltSelection),
      throw = True, # Don't throw?!
      andOr = True
   )

# Load RA2 related configuration
process.load('SandBox.Skims.RA2Cleaning_cff')
process.load('SandBox.Skims.RA2Jets_cff')
process.load('SandBox.Skims.RA2HT_cff')
process.load('SandBox.Skims.RA2MHT_cff')
process.load('SandBox.Skims.RA2DeltaR_cff')

# My own customization of RA2 selections
process.load('UserCode.SkimsRA2.mySusyRA2GenCuts_cff')

process.dummyCounter = cms.EDProducer("EventCountProducer")

# ECAL TP MET filter
process.load('RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi')
process.EcalDeadCellTriggerPrimitiveFilter.debug = cms.bool(True)
#process.EcalDeadCellTriggerPrimitiveFilter.verbose = cms.int32(2);
process.EcalDeadCellTriggerPrimitiveFilter.tpDigiCollection = cms.InputTag("ecalTPSkimNA")

# Tracking failure MET filter
process.load('RecoMET.METFilters.trackingFailureFilter_cfi')
process.trackingFailureFilter.JetSource = cms.InputTag('patJetsAK5PF')

# ECAL BE filter (NOT USED)
process.load('RecoMET.METFilters.EcalDeadCellBoundaryEnergyFilter_cfi')

# Loose Jet ID
# The myPatJetsAK5PFPt30noOverlapLeptonsForJetIDFailure is a jet collection removing
# overlap between jets and well-identified muons and electrons. 
# Because if PF, all particles are reconstructed into jets.
process.load('RecoMET.METFilters.jetIDFailureFilter_cfi')
process.jetIDFailure.JetSource = cms.InputTag("myPatJetsAK5PFPt30noOverlapLeptonsForJetIDFailure")
process.jetIDFailure.MinJetPt  = cms.double(30.0)
process.jetIDFailure.MaxJetEta = cms.double(999.0)

print "WARNING... removing ra2EcalBEFilter (NOT YET CHECKED), eeNoiseFilter (BAD PERFORMANCE)"
process.ra2PostCleaning.remove(process.eeNoiseFilter)
process.ra2PostCleaning.remove(process.ra2EcalBEFilter)

process.ra2AllCleaning = cms.Sequence(
      process.ra2PostCleaning * process.jetIDFailure * process.dummyCounter
   )

# Cannot apply the HBHENoiseFilter and beamHaloFilter on fastsim samples
if options.fastsim:
   process.ra2AllCleaning.remove(process.HBHENoiseFilter)
   process.ra2AllCleaning.remove(process.HBHENoiseFilterRA2)
   process.ra2AllCleaning.remove(process.beamHaloFilter)

# Only use ECAL TP filter
if options.doTPBEfilter == 1:
   process.ra2AllCleaning.replace(process.dummyCounter, process.EcalDeadCellTriggerPrimitiveFilter*process.dummyCounter)
elif options.doTPBEfilter == 2:
   process.ra2AllCleaning.replace(process.dummyCounter, process.EcalDeadCellBoundaryEnergyFilter*process.dummyCounter)
elif options.doTPBEfilter == 3:
   process.ra2AllCleaning.replace(process.dummyCounter, process.EcalDeadCellTriggerPrimitiveFilter * process.EcalDeadCellBoundaryEnergyFilter*process.dummyCounter)

# This is a temp fix for the met significance crash
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMETSignificance
process.load('UserCode.SkimsRA2.badMETsgnfFilter_cfi')
if options.applySgnfFilter == 1:
   process.ra2AllCleaning.replace(process.dummyCounter, process.badMETsgnfFilter*process.dummyCounter)

# A weight producer for event weight, e.g., the weight for QCD flat sample.
# The PU weighting cannot be used for Summer12 MCs, it has not been updated yet.
process.load('UserCode.SkimsRA2.weightProducer_cfi')
process.weightProducer.inputPUfileMC   = cms.untracked.string("")
process.weightProducer.inputPUfileMC   = options.puMCfile
process.weightProducer.inputPUfileData = cms.untracked.string("")
process.weightProducer.inputPUfileData = options.puDatafile
if options.doPtHatWeighting:
   process.weightProducer.Method     = cms.string("PtHat")
   process.weightProducer.Exponent   = cms.double(-4.5)
   process.weightProducer.XS         = cms.double(1.0)
   process.weightProducer.NumberEvts = cms.double(1.0)
   process.weightProducer.Lumi       = cms.double(1.0)
   process.weightProducer.weightWARNingUpThreshold  = cms.double(2.0)

process.myPatJetsAK5PFPt50Eta25noOverlapLeptons.noRemovalOfBigJetMuonPtDiff = cms.untracked.bool(True)
process.myPatJetsAK5PFPt30noOverlapLeptons.noRemovalOfBigJetMuonPtDiff      = cms.untracked.bool(True)
process.myPatJetsAK5PFnoOverlapLeptons.noRemovalOfBigJetMuonPtDiff          = cms.untracked.bool(True)

process.myPatJetsAK5PFPt30noOverlapLeptonsForJetIDFailure = process.myPatJetsAK5PFPt30noOverlapLeptons.clone()
process.myPatJetsAK5PFPt30noOverlapLeptonsForJetIDFailure.noRemovalOfBigJetMuonPtDiff      = cms.untracked.bool(False)

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

process.load("UserCode.SkimsRA2.nJetsForSkimsRA2_cfi")
process.nJetsForSkimsRA2DefaultRA2Inputs = process.nJetsForSkimsRA2.clone()
process.nJetsForSkimsRA2DefaultRA2Inputs.JetSource = cms.InputTag("patJetsAK5PFPt50Eta25")

process.load("UserCode.SkimsRA2.jetMHTDPhiForSkimsRA2_cfi")
process.jetMHTDPhiForSkimsRA2DefaultRA2Inputs = process.jetMHTDPhiForSkimsRA2.clone()
process.jetMHTDPhiForSkimsRA2DefaultRA2Inputs.MHTSource = cms.InputTag("mhtPF")
process.jetMHTDPhiForSkimsRA2DefaultRA2Inputs.JetSource = cms.InputTag("patJetsAK5PFPt30")

process.load("UserCode.SkimsRA2.signifMHTProducer_cfi")
process.mymhtPFforSgnf.resolutionsAlgo = cms.string("AK5PF")
process.mymhtPFforSgnf.resolutionsEra  = cms.string("Spring10")

process.noOverlapJets_seq = cms.Sequence(process.myPatJetsAK5PFPt50Eta25noOverlapLeptons + process.myPatJetsAK5PFPt30noOverlapLeptons + process.myPatJetsAK5PFnoOverlapLeptons + process.myPatJetsAK5PFPt30noOverlapLeptonsForJetIDFailure + process.myGenJetsAK5Pt50Eta25noOverlapLeptons + process.myGenJetsAK5Pt30noOverlapLeptons)
process.prepareAnaPars_seq = cms.Sequence(process.myhtPF + process.mymhtPF + process.myhtGenPF + process.mymhtGenPF + process.mymhtPFforSgnf)
process.prepareCutVars_seq = cms.Sequence( process.nJetsForSkimsRA2DefaultRA2Inputs + process.jetMHTDPhiForSkimsRA2DefaultRA2Inputs )

process.TFileService = cms.Service("TFileService", fileName = cms.string('directStop.root'))

# Generator level filters to pick events containing leptons
process.load("UserCode.SkimsRA2.leptonDecaysFilter_cfi")
process.leptonDecaysFilterWEleMuTau = process.leptonDecaysFilter.clone()
process.leptonDecaysFilterWEleMuTau.WEleMuTauFilter = cms.bool(True)
process.leptonDecaysFilterWEleMuTau.doZfilter       = cms.bool(False)

process.leptonDecaysFilterWEleMuTaulep = process.leptonDecaysFilter.clone()
process.leptonDecaysFilterWEleMuTaulep.WEleMuTaulepFilter = cms.bool(True)
process.leptonDecaysFilterWEleMuTaulep.doZfilter          = cms.bool(False)

process.load("UserCode.HadronicTau.decayToMuFilter_cfi")
process.decayToMuFilterAnyMu = process.decayToMuFilter.clone()
process.decayToMuFilterAnyMu.WTauMuFilter        = cms.bool(True)
process.decayToMuFilterAnyMu.WOMuFilter          = cms.bool(True)
process.decayToMuFilterAnyMu.OTauMuFilter        = cms.bool(True)
process.decayToMuFilterAnyMu.OOMuFilter          = cms.bool(True)

# A general purpose module to store basic event variables, e.g., jets, met and so on.
process.load("UserCode.SkimsRA2.histAndTree_cfi")

if options.storeSgnf:
   process.histAndTree.doSgnf = cms.bool(True)
else:
   process.histAndTree.doSgnf = cms.bool(False)

process.histAndTree_genLevelEleMuTauVeto = process.histAndTree.clone()
process.histAndTree_genLevelEleMuTauVeto.nLeptonsSels       = cms.vint32(-1, -1)
process.histAndTree_genLevelEleMuTauVeto.muonSrc            = cms.InputTag("patMuonsPFIDIso")
process.histAndTree_genLevelEleMuTauVeto.eleSrc             = cms.InputTag("patElectronsPFIDIso")
process.histAndTree_genLevelEleMuTauVeto.jetSrc             = cms.InputTag("patJetsAK5PFPt30")
process.histAndTree_genLevelEleMuTauVeto.forVetoMuonSrc     = cms.InputTag("patMuonsPFIDIso")
process.histAndTree_genLevelEleMuTauVeto.forVetoElectronSrc = cms.InputTag("patElectronsPFIDIso")
process.histAndTree_genLevelEleMuTauVeto.doFillTree         = cms.bool(True)
process.histAndTree_genLevelEleMuTauVeto.mhtSrc             = cms.InputTag("mhtPF")
process.histAndTree_genLevelEleMuTauVeto.htSrc              = cms.InputTag("htPF")
process.histAndTree_genLevelEleMuTauVeto.dPhis_CUT_vec_Src  = cms.InputTag("jetMHTDPhiForSkimsRA2DefaultRA2Inputs:jetMHTDPhiVec")
process.histAndTree_genLevelEleMuTauVeto.nJets_CUT_Src      = cms.InputTag("nJetsForSkimsRA2DefaultRA2Inputs:nJets")

process.histAndTree_genLevelEleMuTauSel = process.histAndTree.clone()
process.histAndTree_genLevelEleMuTauSel.nLeptonsSels       = cms.vint32(-1, -1)
process.histAndTree_genLevelEleMuTauSel.muonSrc            = cms.InputTag("patMuonsPFIDIso")
process.histAndTree_genLevelEleMuTauSel.eleSrc             = cms.InputTag("patElectronsPFIDIso")
process.histAndTree_genLevelEleMuTauSel.jetSrc             = cms.InputTag("patJetsAK5PFPt30")
process.histAndTree_genLevelEleMuTauSel.forVetoMuonSrc     = cms.InputTag("patMuonsPFIDIso")
process.histAndTree_genLevelEleMuTauSel.forVetoElectronSrc = cms.InputTag("patElectronsPFIDIso")
process.histAndTree_genLevelEleMuTauSel.doFillTree         = cms.bool(True)
process.histAndTree_genLevelEleMuTauSel.mhtSrc             = cms.InputTag("mhtPF")
process.histAndTree_genLevelEleMuTauSel.htSrc              = cms.InputTag("htPF")
process.histAndTree_genLevelEleMuTauSel.dPhis_CUT_vec_Src  = cms.InputTag("jetMHTDPhiForSkimsRA2DefaultRA2Inputs:jetMHTDPhiVec")
process.histAndTree_genLevelEleMuTauSel.nJets_CUT_Src      = cms.InputTag("nJetsForSkimsRA2DefaultRA2Inputs:nJets")

process.histAndTree_recoLevelEleMuVeto = process.histAndTree.clone()
process.histAndTree_recoLevelEleMuVeto.nLeptonsSels       = cms.vint32(0, 0)
process.histAndTree_recoLevelEleMuVeto.muonSrc            = cms.InputTag("patMuonsPFIDIso")
process.histAndTree_recoLevelEleMuVeto.eleSrc             = cms.InputTag("patElectronsPFIDIso")
process.histAndTree_recoLevelEleMuVeto.jetSrc             = cms.InputTag("patJetsAK5PFPt30")
process.histAndTree_recoLevelEleMuVeto.forVetoMuonSrc     = cms.InputTag("patMuonsPFIDIso")
process.histAndTree_recoLevelEleMuVeto.forVetoElectronSrc = cms.InputTag("patElectronsPFIDIso")
process.histAndTree_recoLevelEleMuVeto.doFillTree         = cms.bool(True)
process.histAndTree_recoLevelEleMuVeto.mhtSrc             = cms.InputTag("mhtPF")
process.histAndTree_recoLevelEleMuVeto.htSrc              = cms.InputTag("htPF")
process.histAndTree_recoLevelEleMuVeto.dPhis_CUT_vec_Src  = cms.InputTag("jetMHTDPhiForSkimsRA2DefaultRA2Inputs:jetMHTDPhiVec")
process.histAndTree_recoLevelEleMuVeto.nJets_CUT_Src      = cms.InputTag("nJetsForSkimsRA2DefaultRA2Inputs:nJets")

process.histAndTree_recoLevelEleMuTauVeto = process.histAndTree.clone()
process.histAndTree_recoLevelEleMuTauVeto.nLeptonsSels       = cms.vint32(0, 0)
process.histAndTree_recoLevelEleMuTauVeto.muonSrc            = cms.InputTag("patMuonsPFIDIso")
process.histAndTree_recoLevelEleMuTauVeto.eleSrc             = cms.InputTag("patElectronsPFIDIso")
process.histAndTree_recoLevelEleMuTauVeto.jetSrc             = cms.InputTag("patJetsAK5PFPt30")
process.histAndTree_recoLevelEleMuTauVeto.forVetoMuonSrc     = cms.InputTag("patMuonsPFIDIso")
process.histAndTree_recoLevelEleMuTauVeto.forVetoElectronSrc = cms.InputTag("patElectronsPFIDIso")
process.histAndTree_recoLevelEleMuTauVeto.doFillTree         = cms.bool(True)
process.histAndTree_recoLevelEleMuTauVeto.mhtSrc             = cms.InputTag("mhtPF")
process.histAndTree_recoLevelEleMuTauVeto.htSrc              = cms.InputTag("htPF")
process.histAndTree_recoLevelEleMuTauVeto.dPhis_CUT_vec_Src  = cms.InputTag("jetMHTDPhiForSkimsRA2DefaultRA2Inputs:jetMHTDPhiVec")
process.histAndTree_recoLevelEleMuTauVeto.nJets_CUT_Src      = cms.InputTag("nJetsForSkimsRA2DefaultRA2Inputs:nJets")

# Veto of muons and electrons -- RA2 definition
process.ra2LeptonVeto_seq = cms.Sequence(
   process.ra2PFMuonVeto * process.ra2PFElectronVeto
)

process.count5JetsAK5PFPt30 = process.countPatJets.clone()
process.count5JetsAK5PFPt30.src = cms.InputTag('patJetsAK5PFPt30')
process.count5JetsAK5PFPt30.minNumber = cms.uint32(5)

process.load("UserCode.SkimsRA2.trackIsolationMaker_cfi")

process.comb_seq = cms.Sequence(
# All cleaning && all basic variables, e.g., mht, ht...     
# hlt requirement
   process.hltFilter *
# event weight
   process.weightProducer * process.patMuonsPFIDIso * process.patElectronsPFIDIso *
# noOverlap cleaning (muonSel has only iso muon reconstructed, there is no selection cut!)
   process.muonSel_seq * process.noOverlapJets_seq *
# produce HT and MHT
   process.prepareAnaPars_seq * process.prepareCutVars_seq 
)

# A path to select electron, muon and tau in generator level
process.genLevelEleMuTauSel_path = cms.Path(
             process.leptonDecaysFilterWEleMuTau * process.count5JetsAK5PFPt30 *
             process.comb_seq * process.ra2AllCleaning *
             process.histAndTree_genLevelEleMuTauSel
)

# A reco level electron & muon veto
process.recoLevelEleMuVeto_path = cms.Path(
             process.count5JetsAK5PFPt30 *
             process.comb_seq * process.ra2LeptonVeto_seq * process.ra2AllCleaning *
             process.histAndTree_recoLevelEleMuVeto
)

# A reco level electron, muon and tau veto
process.recoLevelEleMuTauVeto_path = cms.Path(
             process.count5JetsAK5PFPt30 *
# trackIsolationFilter is the tau veto developed by the leptonic SUSY group
             process.comb_seq * process.ra2LeptonVeto_seq * process.ra2AllCleaning * process.trackIsolationFilter *
             process.histAndTree_recoLevelEleMuTauVeto
)

# A generator level electron, muon and tau veto
process.genLevelEleMuTauVeto_path = cms.Path(
           ~ process.leptonDecaysFilterWEleMuTau * process.count5JetsAK5PFPt30 *
             process.comb_seq * process.ra2AllCleaning *
             process.histAndTree_genLevelEleMuTauVeto
)

##process.schedule = cms.Schedule( process.genLevelEleMuTauSel_path, process.genLevelEleMuTauVeto_path, process.recoLevelEleMuVeto_path, process.recoLevelEleMuTauVeto_path )
#process.schedule = cms.Schedule( process.recoLevelEleMuTauVeto_path )
process.schedule = cms.Schedule( process.recoLevelEleMuTauVeto_path )
