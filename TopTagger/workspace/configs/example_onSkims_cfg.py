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

options.register('doFillTree', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "fill the topTagger tree or not, default is False")

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

process.dummyCounter = cms.EDProducer("EventCountProducer")

##____________________________________________________________________________||
process.load('RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi')
process.EcalDeadCellTriggerPrimitiveFilter.debug = cms.bool(True)
#process.EcalDeadCellTriggerPrimitiveFilter.verbose = cms.int32(2);
process.EcalDeadCellTriggerPrimitiveFilter.tpDigiCollection = cms.InputTag("ecalTPSkimNA")

process.load('RecoMET.METFilters.trackingFailureFilter_cfi')
process.trackingFailureFilter.JetSource = cms.InputTag('patJetsAK5PF')

##____________________________________________________________________________||
process.load('RecoMET.METFilters.EcalDeadCellBoundaryEnergyFilter_cfi')

process.load('RecoMET.METFilters.jetIDFailureFilter_cfi')
process.jetIDFailure.JetSource = cms.InputTag("patJetsAK5PF")
process.jetIDFailure.MinJetPt  = cms.double(30.0)
process.jetIDFailure.MaxJetEta = cms.double(999.0)

print "WARNING... removing ra2EcalBEFilter (NOT YET CHECKED), eeNoiseFilter (BAD PERFORMANCE)"
process.ra2PostCleaning.remove(process.eeNoiseFilter)
process.ra2PostCleaning.remove(process.ra2EcalBEFilter)

process.ra2AllCleaning = cms.Sequence(
      process.ra2PostCleaning * process.jetIDFailure * process.dummyCounter
   )

if options.fastsim:
   process.ra2AllCleaning.remove(process.HBHENoiseFilter)
   process.ra2AllCleaning.remove(process.HBHENoiseFilterRA2)
   process.ra2AllCleaning.remove(process.beamHaloFilter)

if options.doTPBEfilter == 1:
   process.ra2AllCleaning.replace(process.dummyCounter, process.EcalDeadCellTriggerPrimitiveFilter*process.dummyCounter)
elif options.doTPBEfilter == 2:
   process.ra2AllCleaning.replace(process.dummyCounter, process.EcalDeadCellBoundaryEnergyFilter*process.dummyCounter)
elif options.doTPBEfilter == 3:
   process.ra2AllCleaning.replace(process.dummyCounter, process.EcalDeadCellTriggerPrimitiveFilter * process.EcalDeadCellBoundaryEnergyFilter*process.dummyCounter)

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

process.TFileService = cms.Service("TFileService", fileName = cms.string('directStop.root'))

process.count5JetsAK5PFPt30 = process.countPatJets.clone()
process.count5JetsAK5PFPt30.src = cms.InputTag('patJetsAK5PFPt30')
process.count5JetsAK5PFPt30.minNumber = cms.uint32(5)

process.load("UserCode.SkimsRA2.trackIsolationMaker_cfi")

process.load("UserCode.TopTagger.dPhiFilter_cfi")
process.load("UserCode.TopTagger.signifFilter_cfi")
process.load("UserCode.TopTagger.newHTFilter_cfi")
process.load("UserCode.TopTagger.matchTrigJetsFilter_cfi")

process.load("UserCode.TopTagger.topTagger_cfi")
if options.doFillTree:
   process.topTagger.doFillTree = cms.untracked.bool(True)
#   process.topTagger.doMTMT2cuts = cms.untracked.bool(False)

process.topTaggerAllVetoRA2topEleMuVeto = process.topTagger.clone()

process.load('UserCode.SkimsRA2.sakLooseMuonSelector_cfi')
process.load('UserCode.SkimsRA2.sakLooseElectronSelector_cfi')

process.comb_seq = cms.Sequence(
# All cleaning && all basic variables, e.g., mht, ht...     
# hlt requirement
   process.hltFilter *
# event weight
   process.weightProducer
# noOverlap cleaning (muonSel has only iso muon reconstructed, there is no selection cut!)
# produce HT and MHT
)

process.recoLevelRA2topEleMuVeto_TauVeto_path = cms.Path(
             process.comb_seq *
             process.sakLooseMuonSelector * process.sakLooseElectronSelector * process.ra2AllCleaning * process.trackIsolationFilter *
             process.count5JetsAK5PFPt30 * process.topTaggerAllVetoRA2topEleMuVeto
)

process.schedule = cms.Schedule( process.recoLevelRA2topEleMuVeto_TauVeto_path )
