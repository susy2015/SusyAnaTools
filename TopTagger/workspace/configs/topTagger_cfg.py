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
options.register('doValidation', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Include the validation histograms from SusyDQM (needs extra tags)")
options.register('doExtensiveMatching', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Matching to simtracks (needs extra tags)")
options.register('doSusyTopProjection', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "Apply Susy selection in PF2PAT to obtain lepton cleaned jets (needs validation)")
options.register('addKeep', '', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional keep and drop statements to trim the event content")

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

# The postfix used in the SUSYPAT. This is set fixed in the SUSYPAT, so I cannot tune it and
# have to extract it out.
postfix = "PF"

############################# START SUSYPAT specifics ####################################
from PhysicsTools.Configuration.SUSY_pattuple_cff import addDefaultSUSYPAT, getSUSY_pattuple_outputCommands
#Apply SUSYPAT
addDefaultSUSYPAT(process,options.mcInfo,options.hltName,options.jetCorrections,options.mcVersion,options.jetTypes,options.doValidation,options.doExtensiveMatching,options.doSusyTopProjection)
SUSY_pattuple_outputCommands = getSUSY_pattuple_outputCommands( process )
############################## END SUSYPAT specifics ####################################

#from PhysicsTools.PatAlgos.tools.pfTools import useGsfElectrons
#useGsfElectrons(process,postfix)

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

process.primaryVertexFilter = cms.EDFilter(
   "VertexSelector",
   src = cms.InputTag("offlinePrimaryVertices"),
   cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
   filter = cms.bool(True),
)  

##____________________________________________________________________________||
process.noscraping = cms.EDFilter(
  "FilterOutScraping",
  applyfilter = cms.untracked.bool(True),
  debugOn = cms.untracked.bool(False),
  numtrack = cms.untracked.uint32(10),
  thresh = cms.untracked.double(0.25)
) 

##____________________________________________________________________________||
process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')

##____________________________________________________________________________||
process.load("RecoMET.METFilters.hcalLaserEventFilter_cfi")
process.hcalLaserEventFilter.vetoByRunEventNumber=cms.untracked.bool(False)
process.hcalLaserEventFilter.vetoByHBHEOccupancy=cms.untracked.bool(True)

##____________________________________________________________________________||
process.load('RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi')
process.EcalDeadCellTriggerPrimitiveFilter.debug = cms.bool(True)
#process.EcalDeadCellTriggerPrimitiveFilter.verbose = cms.int32(2);
process.EcalDeadCellTriggerPrimitiveFilter.tpDigiCollection = cms.InputTag("ecalTPSkimNA")

##____________________________________________________________________________||
process.load('RecoMET.METFilters.EcalDeadCellBoundaryEnergyFilter_cfi')

process.load('RecoMET.METFilters.jetIDFailureFilter_cfi')
process.jetIDFailure.JetSource = cms.InputTag('patJetsAK5PF')
process.jetIDFailure.MinJetPt  = cms.double(30.0)
process.jetIDFailure.MaxJetEta = cms.double(999.0)

process.goodVertices = cms.EDFilter(
  "VertexSelector",
  filter = cms.bool(False),
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)

process.load('RecoMET.METFilters.trackingFailureFilter_cfi')

process.load('RecoMET.METFilters.inconsistentMuonPFCandidateFilter_cfi')

process.load('RecoMET.METFilters.greedyMuonPFCandidateFilter_cfi')

process.load('RecoMET.METFilters.eeNoiseFilter_cfi')

process.load('RecoMET.METFilters.eeBadScFilter_cfi')

# This is a temp fix for the met significance crash
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMETSignificance
process.load('UserCode.SkimsRA2.badMETsgnfFilter_cfi')

#if options.mcInfo == True:
#   process.patMETsTypeIPF.corrector = cms.string('ak5PFL2L3')
#else:
#   process.patMETsTypeIPF.corrector = cms.string('ak5PFL2L3Residual')

process.dummyCounter = cms.EDProducer("EventCountProducer")

process.ra2AllCleaning = cms.Sequence(
                          process.goodVertices * process.primaryVertexFilter * process.noscraping * 
                          process.HBHENoiseFilter * process.beamHaloFilter *
                          process.hcalLaserEventFilter * process.eeBadScFilter * process.trackingFailureFilter * 
                          process.inconsistentMuonPFCandidateFilter * process.greedyMuonPFCandidateFilter *
                          process.dummyCounter
                       )

if options.fastsim:
   process.ra2AllCleaning.remove(process.HBHENoiseFilter)
#   process.ra2AllCleaning.remove(process.HBHENoiseFilter2)
   process.ra2AllCleaning.remove(process.beamHaloFilter)

if options.doTPBEfilter == 1:
   process.ra2AllCleaning.replace(process.dummyCounter, process.EcalDeadCellTriggerPrimitiveFilter*process.dummyCounter)
elif options.doTPBEfilter == 2:
   process.ra2AllCleaning.replace(process.dummyCounter, process.EcalDeadCellBoundaryEnergyFilter*process.dummyCounter)
elif options.doTPBEfilter == 3:
   process.ra2AllCleaning.replace(process.dummyCounter, process.EcalDeadCellTriggerPrimitiveFilter * process.EcalDeadCellBoundaryEnergyFilter*process.dummyCounter)

if options.applySgnfFilter == 1:
   process.ra2AllCleaning.replace(process.dummyCounter, process.badMETsgnfFilter*process.dummyCounter)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string('directStop.root')
)

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

process.prepareCutVars_seq = cms.Sequence( process.nJetsForSkimsRA2DefaultRA2Inputs + process.jetMHTDPhiForSkimsRA2DefaultRA2Inputs )
process.prepareAnaPars_seq = cms.Sequence( process.mymhtPFforSgnf )

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

from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *
from PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi import *

from UserCode.SkimsRA2.goldenMuonSelector_cfi import goldenMuonSelector
from UserCode.SkimsRA2.goldenElectronSelector_cfi import goldenElectronSelector

process.patElectronsPFIDIso = goldenElectronSelector.clone()
process.patElectronsPFIDIso.ElectronSource = cms.InputTag("patElectronsPF")
process.patElectronsPFIDIso.MaxEleRelIso = 0.20

process.countPFElectronsIDIso = countPatElectrons.clone()
process.countPFElectronsIDIso.src = cms.InputTag('patElectronsPFIDIso')
process.countPFElectronsIDIso.minNumber = cms.uint32(1)

process.ra2PFElectronVeto = cms.Sequence(
  ~process.countPFElectronsIDIso
)

process.ra2LeptonVeto_seq = cms.Sequence(
   process.ra2PFMuonVeto * process.ra2PFElectronVeto
)

process.ra2Objects.remove(process.ra2Muons)
process.ra2Objects.remove(process.ra2Electrons)

# Most of RA2 sequence here
process.load("UserCode.SkimsRA2.provInfoMuons_cfi")
process.load("UserCode.SkimsRA2.provInfoElectrons_cfi")

process.patMuonsPFID.DoMuonProvInfo = cms.bool(False)
process.patMuonsPFIDIso.DoMuonProvInfo = cms.bool(False)

process.patElectronsPFIDIso.DoElectronProvInfo = cms.bool(False)

process.count5JetsAK5PFPt30 = process.countPatJets.clone()
process.count5JetsAK5PFPt30.src = cms.InputTag('patJetsAK5PFPt30')
process.count5JetsAK5PFPt30.minNumber = cms.uint32(5)

process.load("UserCode.SkimsRA2.trackIsolationMaker_cfi")

process.susypat_seq = cms.Sequence(
                      process.prefilterCounter *
                      process.ra2AllCleaning * process.weightProducer *
                      process.postStdCleaningCounter *
                      process.susyPatDefaultSequence * process.kt6PFJetsForIsolation *
                      process.ra2Objects * process.patElectronsPFIDIso * process.provInfoMuons * process.provInfoElectrons *
                      process.jetIDFailure *
                      process.prepareCutVars_seq * process.prepareAnaPars_seq
                     )

process.genLevelEleMuTauSel_path = cms.Path(
             process.leptonDecaysFilterWEleMuTau *
             process.susypat_seq * process.count5JetsAK5PFPt30 * 
             process.histAndTree_genLevelEleMuTauSel
)

process.recoLevelEleMuVeto_path = cms.Path(
             process.susypat_seq * process.count5JetsAK5PFPt30 *
             process.ra2LeptonVeto_seq * 
             process.histAndTree_recoLevelEleMuVeto
)

process.recoLevelEleMuTauVeto_path = cms.Path(
             process.susypat_seq * process.count5JetsAK5PFPt30 *
             process.ra2LeptonVeto_seq * process.trackIsolationFilter *
             process.histAndTree_recoLevelEleMuTauVeto
)

process.genLevelEleMuTauVeto_path = cms.Path(
           ~ process.leptonDecaysFilterWEleMuTau *
             process.susypat_seq * process.count5JetsAK5PFPt30 *
             process.histAndTree_genLevelEleMuTauVeto
)

process.schedule = cms.Schedule( process.genLevelEleMuTauSel_path, process.genLevelEleMuTauVeto_path, process.recoLevelEleMuVeto_path, process.recoLevelEleMuTauVeto_path )
