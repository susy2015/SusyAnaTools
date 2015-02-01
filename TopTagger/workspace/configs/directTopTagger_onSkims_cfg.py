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

options.register('orderingOpt', 'mass', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "topTagger ordering method --> commandline input with the format: orderingOpt=''mass','mass''")
options.orderingOpt.append('mass')

options.register('usePhiCorrMET', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "use phi corrected MET or not, default is False")

options.register('dofilterTags', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "produce the root files containing filter path tags, default is False")

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
      'file:susypat.root'
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

process.load('UserCode.SkimsRA2.mySusyRA2GenCuts_cff')

process.load('UserCode.SkimsRA2.RA2CleaningFilterResults_cff')

##____________________________________________________________________________||
process.dummyCounter = cms.EDProducer("EventCountProducer")

process.load('RecoMET.METFilters.jetIDFailureFilter_cfi')
process.jetIDFailure.JetSource = cms.InputTag("stopJetsPFchsPt30")
process.jetIDFailure.MinJetPt  = cms.double(30.0)
process.jetIDFailure.MaxJetEta = cms.double(999.0)

process.ra2AllCleaning = cms.Sequence(
      process.cleaningOnFilterResults * process.jetIDFailure * process.dummyCounter
   )

if options.fastsim:
   process.ra2AllCleaning.remove(process.RA2_HBHENoiseFilterRA2)
   process.ra2AllCleaning.remove(process.RA2_beamHaloFilter)

# This is a temp fix for the met significance crash
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMETSignificance
process.load('UserCode.SkimsRA2.badMETsgnfFilter_cfi')
if options.applySgnfFilter == 1:
   process.ra2AllCleaning.replace(process.dummyCounter, process.badMETsgnfFilter*process.dummyCounter)

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
   process.myhtGenPF.JetCollection = cms.InputTag("patJetsPF")
   process.mymhtGenPF.JetCollection = cms.InputTag("patJetsPF")

process.myPatMuonsPFIDnoIso = process.myPatMuonsPFIDIso.clone()
process.myPatMuonsPFIDnoIso.DoMuonIsolation = False
process.muonSel_seq = cms.Sequence(process.myPatMuonsPFIDIso*process.myPatMuonsPFIDnoIso)
process.myPatMuonsPFIDIso.MinMuPt = cms.double(20)
process.myPatMuonsPFIDIso.MaxMuEta = cms.double(2.1)

process.load("UserCode.SkimsRA2.nJetsForSkimsRA2_cfi")
process.nJetsForSkimsRA2DefaultRA2Inputs = process.nJetsForSkimsRA2.clone()
process.nJetsForSkimsRA2DefaultRA2Inputs.JetSource = cms.InputTag("patJetsAK5PFPt50Eta25")

process.nchsJetsForSkimsRA2DefaultRA2Inputs = process.nJetsForSkimsRA2.clone()
process.nchsJetsForSkimsRA2DefaultRA2Inputs.JetSource = cms.InputTag("patJetsPFchsPt50Eta25")

process.load("UserCode.SkimsRA2.jetMHTDPhiForSkimsRA2_cfi")
process.jetMHTDPhiForSkimsRA2DefaultRA2Inputs = process.jetMHTDPhiForSkimsRA2.clone()
process.jetMHTDPhiForSkimsRA2DefaultRA2Inputs.MHTSource = cms.InputTag("mhtPF")
process.jetMHTDPhiForSkimsRA2DefaultRA2Inputs.JetSource = cms.InputTag("patJetsAK5PFPt30")

process.chsjetMHTDPhiForSkimsRA2DefaultRA2Inputs = process.jetMHTDPhiForSkimsRA2.clone()
process.chsjetMHTDPhiForSkimsRA2DefaultRA2Inputs.MHTSource = cms.InputTag("mhtPFchs")
process.chsjetMHTDPhiForSkimsRA2DefaultRA2Inputs.JetSource = cms.InputTag("patJetsPFchsPt50Eta25")

process.load("UserCode.SkimsRA2.signifMHTProducer_cfi")
process.mymhtPFforSgnf.resolutionsAlgo = cms.string("AK5PF")
process.mymhtPFforSgnf.resolutionsEra  = cms.string("Spring10")

process.noOverlapJets_seq = cms.Sequence(process.myPatJetsAK5PFPt50Eta25noOverlapLeptons + process.myPatJetsAK5PFPt30noOverlapLeptons + process.myPatJetsAK5PFnoOverlapLeptons + process.myPatJetsAK5PFPt30noOverlapLeptonsForJetIDFailure + process.myGenJetsAK5Pt50Eta25noOverlapLeptons + process.myGenJetsAK5Pt30noOverlapLeptons)
process.prepareAnaPars_seq = cms.Sequence(process.myhtPF + process.mymhtPF + process.myhtGenPF + process.mymhtGenPF + process.mymhtPFforSgnf)
process.prepareCutVars_seq = cms.Sequence( process.nJetsForSkimsRA2DefaultRA2Inputs + process.jetMHTDPhiForSkimsRA2DefaultRA2Inputs + process.nchsJetsForSkimsRA2DefaultRA2Inputs + process.chsjetMHTDPhiForSkimsRA2DefaultRA2Inputs )

process.TFileService = cms.Service("TFileService", fileName = cms.string('directStop.root'))

# Define CHS correctors
from JetMETCorrections.Configuration.DefaultJEC_cff import *
process.ak5PFchsL1Fastjet  = ak5PFL1Fastjet.clone ( algorithm = cms.string('AK5PFchs') )
process.ak5PFchsL2Relative = ak5PFL2Relative.clone( algorithm = cms.string('AK5PFchs') )
process.ak5PFchsL3Absolute = ak5PFL3Absolute.clone( algorithm = cms.string('AK5PFchs') )
process.ak5PFchsResidual   = ak5PFResidual.clone  ( algorithm = cms.string('AK5PFchs') )

process.ak5PFchsL1FastL2L3         = ak5PFL1FastL2L3Residual.clone( correctors = cms.vstring('ak5PFchsL1Fastjet', 'ak5PFchsL2Relative', 'ak5PFchsL3Absolute') )
process.ak5PFchsL1FastL2L3Residual = ak5PFL1FastL2L3Residual.clone( correctors = cms.vstring('ak5PFchsL1Fastjet', 'ak5PFchsL2Relative', 'ak5PFchsL3Absolute', 'ak5PFchsResidual') )
process.ak5PFchsL2L3               = ak5PFL1FastL2L3Residual.clone( correctors = cms.vstring('ak5PFchsL2Relative', 'ak5PFchsL3Absolute') )

process.load("SandBox.Stop.StopLeptons_cff")

process.load('SandBox.Stop.StopJets_cff')

process.load("SandBox.Stop.StopMETPhiCorr_cff")

if options.mcInfo == False: 
   process.newMETwPhiCorr = process.newMETwPhiCorrData.clone()
else:
   process.newMETwPhiCorr = process.newMETwPhiCorrMC.clone()

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

process.load("SandBox.Stop.StopDPhiSelection_cff")
process.jetsMETDPhiFilter.jetSrc = cms.InputTag("stopJetsPFchsPt30")
if options.usePhiCorrMET == True:
   process.jetsMETDPhiFilter.metSrc = cms.InputTag("newMETwPhiCorr")
else:
   process.jetsMETDPhiFilter.metSrc = cms.InputTag("patMETsPF")
process.jetsMETDPhiFilter.dPhiCuts = cms.untracked.vdouble(0.5, 0.5, 0.3)

process.load("SandBox.Stop.StopBTagJets_cff")
process.stopBJets.src = cms.InputTag("stopJetsPFchsPt30")

process.load("SandBox.Stop.StopType3TopTagger_cff")
if options.usePhiCorrMET == True:
   process.type3topTagger.metSrc = cms.InputTag("newMETwPhiCorr")
else:
   process.type3topTagger.metSrc = cms.InputTag("patMETsPF")
process.type3topTagger.taggingMode = cms.untracked.bool(False)
process.type3topTagger.jetSrc = cms.InputTag("stopJetsPFchsPt30")
process.type3topTagger.orderingOptArr = cms.untracked.vstring(options.orderingOpt)

process.topTaggerSingleMuSel = process.type3topTagger.clone()
process.topTaggerSingleMuSel.jetSrc = cms.InputTag("myPatJetsAK5PFPt30noOverlapLeptons")

process.metPFchsFilter = process.mhtPFchsFilter.clone()
if options.usePhiCorrMET == True:
   process.metPFchsFilter.MHTSource = cms.InputTag("newMETwPhiCorr")
else:
   process.metPFchsFilter.MHTSource = cms.InputTag("patMETsPF")

process.metPFchsFilter.MinMHT = cms.double(175)

process.met350PFchsFilter = process.mhtPFchsFilter.clone()
if options.usePhiCorrMET == True:
   process.met350PFchsFilter.MHTSource = cms.InputTag("newMETwPhiCorr")
else:
   process.met350PFchsFilter.MHTSource = cms.InputTag("patMETsPF")
process.met350PFchsFilter.MinMHT = cms.double(350)

process.load('SandBox.Stop.topDecaysFilter_cfi')

process.count1JetsPFchsPt70Eta24Std = process.count2JetsPFchsPt70Eta24Std.clone()
process.count1JetsPFchsPt70Eta24Std.minNumber = cms.uint32(1)

process.count3JetsPFchsPt50Eta24Std = process.count4JetsPFchsPt50Eta24Std.clone()
process.count3JetsPFchsPt50Eta24Std.minNumber = cms.uint32(3)

process.count4JetsPFchsPt30Eta24Std = process.count5JetsPFchsPt30Eta24Std.clone()
process.count4JetsPFchsPt30Eta24Std.minNumber = cms.uint32(4)

process.load("UserCode.SkimsRA2.histAndTree_cfi")

if options.storeSgnf:
   process.histAndTree.doSgnf = cms.bool(True)
else:
   process.histAndTree.doSgnf = cms.bool(False)

if options.mcInfo:
   process.histAndTree.doFillGenTopInfo = cms.bool(True)
# Follow the vetos in the path. Don't reject events here!
process.histAndTree.nLeptonsSels     = cms.vint32(-1, -1)

process.histAndTree.metSrc = cms.InputTag("newMETwPhiCorr")

process.histAndTree.muonSrc            = cms.InputTag("stopsakLooseMuonSelector")
process.histAndTree.forVetoMuonSrc     = cms.InputTag("stopsakLooseMuonSelector")
process.histAndTree.eleSrc             = cms.InputTag("stopsakLooseElectronSelector")
process.histAndTree.forVetoElectronSrc = cms.InputTag("stopsakLooseElectronSelector")
process.histAndTree.jetSrc             = cms.InputTag("stopJetsPFchsPt30")
process.histAndTree.mhtSrc             = cms.InputTag("mhtPFchs")
process.histAndTree.htSrc              = cms.InputTag("htPFchs")

process.histAndTree_recoLevelEleMuTauVeto = process.histAndTree.clone()
process.histAndTree_recoLevelEleMuTauVeto.dPhis_CUT_vec_Src  = cms.InputTag("jetMHTDPhiForSkimsRA2DefaultRA2Inputs:jetMHTDPhiVec")
process.histAndTree_recoLevelEleMuTauVeto.nJets_CUT_Src      = cms.InputTag("nJetsForSkimsRA2DefaultRA2Inputs:nJets")
process.histAndTree_recoLevelEleMuTauVeto.doFillTree         = cms.bool(True)

process.histAndTree_genLevelEleMuTauVeto = process.histAndTree_recoLevelEleMuTauVeto.clone()

#
process.histAndTree_recoLevelSingleMuSel = process.histAndTree.clone()
process.histAndTree_recoLevelSingleMuSel.dPhis_CUT_vec_Src  = cms.InputTag("jetMHTDPhiForSkimsRA2DefaultRA2Inputs:jetMHTDPhiVec")
process.histAndTree_recoLevelSingleMuSel.nJets_CUT_Src      = cms.InputTag("nJetsForSkimsRA2DefaultRA2Inputs:nJets")
process.histAndTree_recoLevelSingleMuSel.doFillTree         = cms.bool(True)

process.ra2LeptonVeto_seq = cms.Sequence(
   process.ra2PFMuonVeto * process.ra2PFElectronVeto
)

process.load("SandBox.Stop.trackIsolationMaker_cfi")
process.trackIsolation = process.trackIsolationFilter.clone()
process.trackIsolation.doTrkIsoVeto = cms.bool(False)
process.trackIsolation.minPt_PFCandidate = cms.double(5.0)

process.comb_seq = cms.Sequence(
# All cleaning && all basic variables, e.g., mht, ht...     
# hlt requirement
   process.hltFilter *
# event weight
   process.weightProducer * 
# All the muon, electron and isolated tracking producers
   process.patMuonsPFIDIso * process.patElectronsPFIDIso * process.stopPFMuons * process.stopElectrons * process.trackIsolation *
# noOverlap cleaning (muonSel has only iso muon reconstructed, there is no selection cut!)
   process.muonSel_seq * process.noOverlapJets_seq *
# chs jets
   process.patJetsPFchsPt30 * process.patJetsPFchsPt50Eta25 * process.htPFchs * process.mhtPFchs *
# produce HT and MHT
   process.prepareAnaPars_seq * process.prepareCutVars_seq *
   process.metphi_seq * process.newMETwPhiCorr *
   process.stopPFJets *
# topDecaysFilter
   process.topDecaysFilter
)

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.printTree = cms.EDAnalyzer("ParticleTreeDrawer",
                                   src = cms.InputTag("genParticles"),
                                   printP4 = cms.untracked.bool(False),
                                   printPtEtaPhi = cms.untracked.bool(False),
                                   printVertex = cms.untracked.bool(False),
                                   printStatus = cms.untracked.bool(True),
                                   printIndex = cms.untracked.bool(True),
#                                   status = cms.untracked.vint32(1, 2, 3)
                                   status = cms.untracked.vint32(1, 2, 3)
                                  )

process.printDecay = cms.EDAnalyzer( "ParticleDecayDrawer",
   src = cms.InputTag( "genParticles" ),
#    untracked bool printP4 = true
#    untracked bool printPtEtaPhi = true
   status = cms.untracked.vint32( 3 )
)

process.recoLevelEleMuVeto_TauVeto_path = cms.Path(
   process.comb_seq * 
   process.ra2AllCleaning * #process.printTree *
# Looser jet cuts
   process.stopPFMuonVeto * process.stopElectronVeto *
   process.count1JetsPFchsPt70Eta24Std * process.count3JetsPFchsPt50Eta24Std * process.count4JetsPFchsPt30Eta24Std *
# Store the events at this stage
   process.histAndTree_recoLevelEleMuTauVeto * 
# Apply full stop search cuts -- for each verification and other purposes
   process.count2JetsPFchsPt70Eta24Std *
   process.count4JetsPFchsPt50Eta24Std *
   process.count5JetsPFchsPt30Eta24Std *
   process.count5JetsPFchsPt30Std *
   process.jetsMETDPhiFilter *
   process.stopBJets * process.stopCountBJets *
   process.metPFchsFilter *
   process.type3topTagger * process.met350PFchsFilter
)

#process.out.fileName = options.output

#process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('recoLevelEleMuTauVeto_path') )

#process.outpath = cms.EndPath( process.out )

if options.dofilterTags == True:
   process.out.fileName = cms.untracked.string('filterTags.root')
   
   ##____________________________________________________________________________||
   process.hltTriggerSummaryAOD = cms.EDProducer(
       "TriggerSummaryProducerAOD",
       processName = cms.string( "@" )
       )
   
   ##____________________________________________________________________________||
   from PhysicsTools.PatAlgos.tools.trigTools import *
   switchOnTrigger(process)
   
   ##____________________________________________________________________________||
   process.patTriggerFilter = process.patTrigger.clone()
   process.patTriggerFilter.processName = cms.string('SUSY')
   process.MessageLogger.suppressWarning += ['patTriggerFilter']
   
   process.outpath = cms.EndPath(
       process.patTrigger *
       process.hltTriggerSummaryAOD *
       process.patTriggerFilter *
       process.out
   )
   
   ##____________________________________________________________________________||
   from PhysicsTools.PatAlgos.patEventContent_cff import *
   process.out.outputCommands = cms.untracked.vstring(
       'drop *',
       'keep patTriggerPaths_patTriggerFilter_*_*',
   )
   process.out.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('*', '!*'))
