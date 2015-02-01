import FWCore.ParameterSet.Config as cms

process = cms.Process("SUSY")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

## Source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      '/store/relval/CMSSW_3_8_0_pre8/RelValTTbar/GEN-SIM-RECO/START38_V6-v1/0004/847D00B0-608E-DF11-A37D-003048678FA0.root'
    )
)
## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

# The following is make patJets, but EI is done with the above
process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

import os
import sys
import re

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('standard')

options.register('GlobalTag', "START70_V6::All", VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "GlobaTTag to use (otherwise default Pat GT is used)")
options.register('mcInfo', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "process MonteCarlo data, default is data")
options.register('jetCorrections', 'L2Relative', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Level of jet corrections to use: Note the factors are read from DB via GlobalTag")
options.jetCorrections.append('L3Absolute')

options.register('hltName', 'HLT', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "HLT menu to use for trigger matching")
options.register('mcVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific MC fix")
options.register('jetTypes', 'AK5PF', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional jet types that will be produced (AK5Calo and AK5PF, cross cleaned in PF2PAT, are included anyway)")
options.register('hltSelection', '*', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "hlTriggers (OR) used to filter events. for data: ''HLT_Mu9', 'HLT_IsoMu9', 'HLT_IsoMu13_v*''; for MC, HLT_Mu9")
options.register('addKeep', '', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional keep and drop statements to trim the event content")

options.register('dataVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific DATA fix")

options.register('debug', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "switch on/off debug mode")

options.register('test', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "switch on/off debug mode")

options.register('doPtHatWeighting', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "PtHat weighting for QCD flat samples, default is False")

options.register('fileslist', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "name of a file with input source list")

options.register('fastsim', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "fastsim sample or not, default is False")

options.register('doTopTagger', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "do top tagger or not, default is True")

options.register('usePhiCorrMET', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "use phi corrected MET or not, default is False")

options.register('reducedfilterTags', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "use phi corrected MET or not, default is True")

options.register('smsModel', 'T2tt', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "SMS model name")
options.register('smsMotherMass',  -1, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "SMS mother mass")
options.register('smsDaughterMass',  -1, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "SMS daughter mass")
options.register('selSMSpts', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "select model points")

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
      'file:patTuple_mini.root'
   ]

process.maxEvents.input = options.maxEvents

#-- Calibration tag -----------------------------------------------------------
if options.GlobalTag:
   process.GlobalTag.globaltag = options.GlobalTag

if options.mcInfo == False: options.jetCorrections.append('L2L3Residual')
options.jetCorrections.insert(0, 'L1FastJet')

print "jetCorrections: "
print options.jetCorrections

from RecoJets.JetProducers.ak5PFJets_cfi import ak5PFJets
from RecoJets.JetProducers.ak5GenJets_cfi import ak5GenJets
from RecoMET.METProducers.PFMET_cfi import pfMet

#select isolated collections
process.selectedMuons = cms.EDFilter("CandPtrSelector", src = cms.InputTag("slimmedMuons"), cut = cms.string('''abs(eta)<2.5 && pt>10. &&
   (pfIsolationR04().sumChargedHadronPt+
    max(0.,pfIsolationR04().sumNeutralHadronEt+
    pfIsolationR04().sumPhotonEt-
    0.50*pfIsolationR04().sumPUPt))/pt < 0.20 && 
    (isPFMuon && (isGlobalMuon || isTrackerMuon) )'''))
process.selectedElectrons = cms.EDFilter("CandPtrSelector", src = cms.InputTag("slimmedElectrons"), cut = cms.string('''abs(eta)<2.5 && pt>20. &&
    gsfTrack.isAvailable() &&
    gsfTrack.trackerExpectedHitsInner.numberOfLostHits<2 &&
    (pfIsolationVariables().sumChargedHadronPt+
    max(0.,pfIsolationVariables().sumNeutralHadronEt+
    pfIsolationVariables().sumPhotonEt-
    0.5*pfIsolationVariables().sumPUPt))/pt < 0.15'''))

#do projections
process.pfCHS = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV"))
process.pfNoMuonCHS =  cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfCHS"), veto = cms.InputTag("selectedMuons"))
process.pfNoElectronsCHS = cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfNoMuonCHS"), veto =  cms.InputTag("selectedElectrons"))

process.pfNoMuon =  cms.EDProducer("CandPtrProjector", src = cms.InputTag("packedPFCandidates"), veto = cms.InputTag("selectedMuons"))
process.pfNoElectrons = cms.EDProducer("CandPtrProjector", src = cms.InputTag("pfNoMuon"), veto =  cms.InputTag("selectedElectrons"))

#process.ak5PFJets = ak5PFJets.clone(src = 'pfNoElectrons', doAreaFastjet = True) # no idea while doArea is false by default, but it's True in RECO so we have to set it
#process.ak5PFJetsCHS = ak5PFJets.clone(src = 'pfNoElectronsCHS', doAreaFastjet = True) # no idea while doArea is false by default, but it's True in RECO so we have to set it
process.ak5PFJets = ak5PFJets.clone(src = 'pfCHS', doAreaFastjet = True) # no idea while doArea is false by default, but it's True in RECO so we have to set it
process.ak5PFJetsCHS = ak5PFJets.clone(src = 'pfCHS', doAreaFastjet = True) # no idea while doArea is false by default, but it's True in RECO so we have to set it
process.ak5GenJets = ak5GenJets.clone(src = 'packedGenParticles')

process.pfMet = pfMet.clone(src = "packedPFCandidates")
process.pfMet.calculateSignificance = False # this can't be easily implemented on packed PF candidates at the moment

#from PhysicsTools.PatAlgos.producersLayer1.metProducer_cfi import patMETs
process.load('PhysicsTools.PatAlgos.producersLayer1.metProducer_cfi')
from JetMETCorrections.Type1MET.pfMETCorrections_cff import *
from PhysicsTools.PatUtils.patPFMETCorrections_cff import patPFJetMETtype1p2Corr

process.patMETs.addGenMET = cms.bool(False)

#process.patpfType1CorrectedMet = pfType1CorrectedMet.clone(
#
#)

#process.patPFMet = patMETs.clone(
#   metSource = cms.InputTag('pfType1CorrectedMet'),
#   addMuonCorrections = cms.bool(False),
#   addGenMET    = cms.bool(False),
#   genMETSource = cms.InputTag('genMetTrue')
#)

#process.patType1CorrectedPFMet = cms.EDProducer("CorrectedPATMETProducer",
#   src = cms.InputTag('patPFMet'),
#   applyType1Corrections = cms.bool(True),
#   srcType1Corrections = cms.VInputTag(
#      cms.InputTag('patPFJetMETtype1p2Corr', 'type1'),
      ##cms.InputTag('patPFMETtype0Corr')                    
#   ),
#   applyType2Corrections = cms.bool(False)
#)   

from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
addJetCollection(
   process,
   postfix   = "",
   labelName = 'AK5PFCHS',
   jetSource = cms.InputTag('ak5PFJetsCHS'),
   trackSource = cms.InputTag('unpackedTracksAndVertices'), 
   pvSource = cms.InputTag('unpackedTracksAndVertices'), 
   jetCorrections = ('AK5PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'Type-2'),
   btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
   )
addJetCollection(
   process,
   postfix   = "",
   labelName = 'AK5PF',
   jetSource = cms.InputTag('ak5PFJets'),
   trackSource = cms.InputTag('unpackedTracksAndVertices'),
   pvSource = cms.InputTag('unpackedTracksAndVertices'), 
   jetCorrections = ('AK5PF', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'Type-2'),
   btagDiscriminators = [      'combinedSecondaryVertexBJetTags'     ]
   )

#process.load('PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi')
#process.load('PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff')
#adjust MC matching
#process.patJetPartonMatchPatJetsAK5PFCHS = process.patJetPartonMatch.clone()
#process.patJetPartonMatchPatJetsAK5PF = process.patJetPartonMatch.clone()

#process.patJetPartonMatch.matched = "prunedGenParticles"

process.patJetPartonMatchAK5PFCHS.matched = "prunedGenParticles"
process.patJetPartonMatchAK5PF.matched = "prunedGenParticles"
#process.patJetPartons.src = "prunedGenParticles"
process.patJetPartons.particles = "prunedGenParticles"
process.patJetPartons.skipFirstN = cms.uint32(0) # do not skip first 6 particles, we already pruned some!
process.patJetPartons.acceptNoDaughters = cms.bool(True) # as we drop intermediate stuff, we need to accept quarks with no siblings

#adjust PV
#process.patJetCorrFactors.primaryVertices = "offlineSlimmedPrimaryVertices"

process.patJetCorrFactorsAK5PFCHS.primaryVertices = "offlineSlimmedPrimaryVertices"
process.patJetCorrFactorsAK5PF.primaryVertices = "offlineSlimmedPrimaryVertices"

#process.impactParameterTagInfos.primaryVertex = "unpackedTracksAndVertices"

#recreate tracks and pv for btagging
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.combinedSecondaryVertex.trackMultiplicityMin = 1 #silly sv, uses un filtered tracks.. i.e. any pt

process.MessageLogger.suppressWarning = cms.untracked.vstring('ecalLaserCorrFilter','manystripclus53X','toomanystripclus53X')
process.options.allowUnscheduled = cms.untracked.bool(True)

#Analysis related configuration
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
if options.hltSelection:
   process.hltFilter = hlt.hltHighLevel.clone(
      TriggerResultsTag = cms.InputTag("TriggerResults","",options.hltName),
      HLTPaths = cms.vstring(options.hltSelection),
      throw = True, # Don't throw?!
      andOr = True
   )

#process.patJetsPF = process.patJetsAK5PFCHS.clone()

process.load("RecoJets.Configuration.GenJetParticles_cff")
process.genParticlesForJets.src = cms.InputTag("prunedGenParticles")

process.load("AllHadronicSUSY.SkimsRA2.simpleJetSelector_cfi")
process.selectedPatJetsRA2 = process.simpleJetSelector.clone()

process.load("PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi")
# PFJets (with CHS)
process.patJetsPFchsPt30     = process.selectedPatJetsRA2.clone()
process.patJetsPFchsPt30.jetSrc = cms.InputTag('patJetsAK5PFCHS')
process.patJetsPFchsPt30.pfJetCut = cms.string('pt > 30')

process.patJetsPFchsPt50Eta25     = process.selectedPatJetsRA2.clone()
process.patJetsPFchsPt50Eta25.jetSrc = cms.InputTag('patJetsAK5PFCHS')
process.patJetsPFchsPt50Eta25.pfJetCut = cms.string('pt > 50 & abs(eta) < 2.5')

# PFJets - filters
process.countJetsPFchsPt50Eta25           = process.countPatJets.clone()
process.countJetsPFchsPt50Eta25.src       = cms.InputTag('patJetsPFchsPt50Eta25')
process.countJetsPFchsPt50Eta25.minNumber = cms.uint32(3)

process.ra2PFchsJets = cms.Sequence( process.patJetsPFchsPt30 * process.patJetsPFchsPt50Eta25 )

# HT 
process.load("AllHadronicSUSY.Skims.htProducer_cfi")
process.load("AllHadronicSUSY.Skims.htFilter_cfi")
process.htPFchs = process.ht.clone()
process.htPFchs.JetCollection = cms.InputTag("patJetsPFchsPt50Eta25")

process.htPFchsFilter = process.htFilter.clone()
process.htPFchsFilter.HTSource = cms.InputTag("htPFchs")

# MHT
process.load("AllHadronicSUSY.Skims.mhtProducer_cfi")
process.load("AllHadronicSUSY.Skims.mhtFilter_cfi")
process.mhtPFchs = process.mht.clone()
process.mhtPFchs.JetCollection = cms.InputTag("patJetsPFchsPt30")

process.mhtPFchsFilter = process.mhtFilter.clone()
process.mhtPFchsFilter.MHTSource = cms.InputTag("mhtPFchs")

# Delta Phi
process.load("AllHadronicSUSY.Skims.jetMHTDPhiFilter_cfi")

process.jetMHTPFchsDPhiFilter = process.jetMHTDPhiFilter.clone()
process.jetMHTPFchsDPhiFilter.JetSource = cms.InputTag("patJetsPFchsPt30")
process.jetMHTPFchsDPhiFilter.MHTSource = cms.InputTag("mhtPFchs")

process.ra2Objects = cms.Sequence( 
                                   process.ra2PFchsJets *
                                   process.htPFchs *
                                   process.mhtPFchs *
                                   process.selectedMuons *
                                   process.selectedElectrons
                                 )

process.load("PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi")
process.load("PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi")

process.countPFMuonsIDIso = process.countPatMuons.clone()
process.countPFMuonsIDIso.src = cms.InputTag('selectedMuons')
process.countPFMuonsIDIso.minNumber = cms.uint32(1)

process.countPFElectronsIDIso = process.countPatElectrons.clone()
process.countPFElectronsIDIso.src = cms.InputTag('selectedElectrons')
process.countPFElectronsIDIso.minNumber = cms.uint32(1)

process.ra2PFMuonVeto = cms.Sequence( ~process.countPFMuonsIDIso )
process.ra2PFElectronVeto = cms.Sequence( ~process.countPFElectronsIDIso )

############################# START SUSYPAT specifics ####################################
process.prefilterCounter        = cms.EDProducer("EventCountProducer")
process.postStdCleaningCounter  = cms.EDProducer("EventCountProducer")

# Standard Event cleaning 
process.load("AllHadronicSUSY.Skims.noscraping_cfi")
process.load("AllHadronicSUSY.Skims.vertex_cfi")

process.goodVertices = cms.EDFilter(
  "VertexSelector",
  filter = cms.bool(False),
  src = cms.InputTag("offlineSlimmedPrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)

process.ra2StdCleaning = cms.Sequence(
#                 process.noscraping *
                 process.oneGoodVertex
)

# an example sequence to create skimmed susypat-tuples
process.cleanpatseq = cms.Sequence(
                      process.ra2StdCleaning          *
                      process.postStdCleaningCounter  *
                      process.ra2Objects
                      )
############################# EDN SUSYPAT specifics ####################################

process.dummyCounter = cms.EDProducer("EventCountProducer")

process.load('RecoMET.METFilters.jetIDFailureFilter_cfi')
process.jetIDFailure.JetSource = cms.InputTag("patJetsPF")
process.jetIDFailure.MinJetPt  = cms.double(30.0)
process.jetIDFailure.MaxJetEta = cms.double(999.0)
# Adjust jets due to JEC in jetIDFailure filter
if options.mcInfo == True:
   process.jetIDFailure.JECLevel = cms.untracked.string('ak5PFchsL1FastL2L3')
else:
   process.jetIDFailure.JECLevel = cms.untracked.string('ak5PFchsL1FastL2L3Residual')
# End of the adjusting in the jetIDFailure filter

process.load('AllHadronicSUSY.SkimsRA2.weightProducer_cfi')
process.weightProducer.inputPUfileMC   = cms.untracked.string("")
process.weightProducer.inputPUfileData = cms.untracked.string("")
if options.doPtHatWeighting:
   process.weightProducer.Method     = cms.string("PtHat")
   process.weightProducer.Exponent   = cms.double(-4.5)
   process.weightProducer.XS         = cms.double(1.0)
   process.weightProducer.NumberEvts = cms.double(1.0)
   process.weightProducer.Lumi       = cms.double(1.0)
   process.weightProducer.weightWARNingUpThreshold  = cms.double(2.0)

# Define CHS correctors
from JetMETCorrections.Configuration.DefaultJEC_cff import *
process.ak5PFchsL1Fastjet  = ak5PFL1Fastjet.clone ( algorithm = cms.string('AK5PFchs') )
process.ak5PFchsL2Relative = ak5PFL2Relative.clone( algorithm = cms.string('AK5PFchs') )
process.ak5PFchsL3Absolute = ak5PFL3Absolute.clone( algorithm = cms.string('AK5PFchs') )
process.ak5PFchsResidual   = ak5PFResidual.clone  ( algorithm = cms.string('AK5PFchs') )

process.ak5PFchsL1FastL2L3         = ak5PFL1FastL2L3Residual.clone( correctors = cms.vstring('ak5PFchsL1Fastjet', 'ak5PFchsL2Relative', 'ak5PFchsL3Absolute') )
process.ak5PFchsL1FastL2L3Residual = ak5PFL1FastL2L3Residual.clone( correctors = cms.vstring('ak5PFchsL1Fastjet', 'ak5PFchsL2Relative', 'ak5PFchsL3Absolute', 'ak5PFchsResidual') )
process.ak5PFchsL2L3               = ak5PFL1FastL2L3Residual.clone( correctors = cms.vstring('ak5PFchsL2Relative', 'ak5PFchsL3Absolute') )

process.load("AllHadronicSUSY.Stop.trackIsolationMaker_cfi")
process.trackIsolation = process.trackIsolationFilter.clone()
process.trackIsolation.pfCandidatesTag = cms.InputTag("packedPFCandidates")
process.trackIsolation.doTrkIsoVeto = cms.bool(False)
process.trackIsolation.minPt_PFCandidate = cms.double(5.0)

process.load('AllHadronicSUSY.Stop.StopJets_drt_from_AOD_cff')

process.load("AllHadronicSUSY.SkimsRA2.nJetsForSkimsRA2_cfi")
process.nJetsForSkimsStop = process.nJetsForSkimsRA2.clone()
process.nJetsForSkimsStop.JetSource = cms.InputTag("stopJetsPFchsPt30")
process.load("AllHadronicSUSY.SkimsRA2.jetMHTDPhiForSkimsRA2_cfi")
process.jetMHTDPhiForSkimsStop = process.jetMHTDPhiForSkimsRA2.clone()
process.jetMHTDPhiForSkimsStop.MHTSource = cms.InputTag("slimmedMETs")
process.jetMHTDPhiForSkimsStop.JetSource = cms.InputTag("stopJetsPFchsPt30")

process.stophtPFchs = process.htPFchs.clone()
process.stophtPFchs.JetCollection = cms.InputTag("stopJetsPFchsPt50Eta24")

process.stopmhtPFchs = process.mhtPFchs.clone()
process.stopmhtPFchs.JetCollection = cms.InputTag("stopJetsPFchsPt30")

process.prepareCutVars_seq = cms.Sequence( process.nJetsForSkimsStop * process.jetMHTDPhiForSkimsStop * process.stophtPFchs * process.stopmhtPFchs )

process.load("AllHadronicSUSY.Stop.StopBTagJets_cff")
process.stopBJets.JetSrc = cms.InputTag("stopJetsPFchsPt30")

# Other sequence
process.comb_seq = cms.Sequence(
# All cleaning && all basic variables, e.g., mht, ht...     
   process.cleanpatseq *
# hlt requirement
   process.hltFilter *
   process.weightProducer *
#   process.trackIsolation *
   process.stopPFJets * process.stopBJets *
   process.prepareCutVars_seq
)

process.load("AllHadronicSUSY.Stop.StopDPhiSelection_cff")
process.jetsMETDPhiFilter.jetSrc = cms.InputTag("stopJetsPFchsPt30")
if options.usePhiCorrMET == True:
   process.jetsMETDPhiFilter.metSrc = cms.InputTag("slimmedMETs")
else:
   process.jetsMETDPhiFilter.metSrc = cms.InputTag("slimmedMETs")
process.jetsMETDPhiFilter.dPhiCuts = cms.untracked.vdouble(0.5, 0.5, 0.3)

process.stopCount1BJets = process.stopCountBJets.clone()
process.stopCount1BJets.minNumber = cms.uint32(1)

process.stopCount2BJets = process.stopCountBJets.clone()
process.stopCount2BJets.minNumber = cms.uint32(2)

process.load("AllHadronicSUSY.Stop.StopType3TopTagger_cff")
if options.usePhiCorrMET == True:
   process.type3topTagger.metSrc = cms.InputTag("slimmedMETs")
else:
   process.type3topTagger.metSrc = cms.InputTag("slimmedMETs")
process.type3topTagger.taggingMode = cms.untracked.bool(False)
process.type3topTagger.jetSrc = cms.InputTag("stopJetsPFchsPt30")

process.metPFchsFilter = process.mhtPFchsFilter.clone()
if options.usePhiCorrMET == True:
   process.metPFchsFilter.MHTSource = cms.InputTag("slimmedMETs")
else:
   process.metPFchsFilter.MHTSource = cms.InputTag("slimmedMETs")

process.met175PFchsFilter = process.metPFchsFilter.clone()
process.met175PFchsFilter.MinMHT = cms.double(175)

process.met200PFchsFilter = process.metPFchsFilter.clone()
process.met200PFchsFilter.MinMHT = cms.double(200)

process.met350PFchsFilter = process.metPFchsFilter.clone()
process.met350PFchsFilter.MinMHT = cms.double(350)

process.TFileService = cms.Service("TFileService",
   fileName = cms.string('stdStopHistAndTree.root')
)

process.load("AllHadronicSUSY.SkimsRA2.myGenJetsPartSelector_cfi")
process.myGenParticlesForJets.src = cms.InputTag("packedGenParticles")

process.myGenParticlesForJetsNoNu = process.myGenParticlesForJets.clone()
process.myGenParticlesForJetsNoNu.ignoreParticleIDs += cms.vuint32( 12,14,16)

process.myGenParticlesForJetsNoNuNoStopDecays = process.myGenParticlesForJetsNoNu.clone()
process.myGenParticlesForJetsNoNuNoStopDecays.ignoreParticleDecayIDs = cms.vint32(1000006, -1000006)

process.myGenParticlesForJetsNoNuOnlyStopDecays = process.myGenParticlesForJetsNoNu.clone()
process.myGenParticlesForJetsNoNuOnlyStopDecays.ignoreParticleDecayIDs = cms.vint32(1000006, -1000006)
process.myGenParticlesForJetsNoNuOnlyStopDecays.invertignoreParticleDecay = cms.bool(True)

process.load("RecoJets.JetProducers.ak5GenJets_cfi")

process.myak5GenJetsNoNu = process.ak5GenJets.clone( src = cms.InputTag("myGenParticlesForJetsNoNu") )
process.myak5GenJetsNoNuNoStopDecays = process.ak5GenJets.clone( src = cms.InputTag("myGenParticlesForJetsNoNuNoStopDecays") )
process.myak5GenJetsNoNuOnlyStopDecays = process.ak5GenJets.clone( src = cms.InputTag("myGenParticlesForJetsNoNuOnlyStopDecays") )

process.load("AllHadronicSUSY.SkimsRA2.histAndTree_cfi")
if options.doTopTagger == True:
   process.histAndTree.doTopTagger = cms.untracked.bool(True)
else:
   process.histAndTree.doTopTagger = cms.untracked.bool(False)

#if options.mcInfo == True:
#   process.histAndTree.storeGenDecayInfo = cms.untracked.bool(True)
#   process.histAndTree.genJetsInputTags = cms.VInputTag(
#                                                         cms.InputTag("myak5GenJetsNoNu"),
#                                                         cms.InputTag("myak5GenJetsNoNuNoStopDecays"),
#                                                         cms.InputTag("myak5GenJetsNoNuOnlyStopDecays"),
#                                                       )

if options.selSMSpts == True:
   process.histAndTree.storeSMSmodelInfo = cms.untracked.bool(True)
else:
   process.histAndTree.storeSMSmodelInfo = cms.untracked.bool(False)

# The top tagger calculation in the histAndTree uses the metSrc
process.histAndTree.type1metSrc = cms.InputTag("slimmedMETs")
process.histAndTree.metSrc      = cms.InputTag("slimmedMETs")

process.stdStop_histAndTree = process.histAndTree.clone()
process.stdStop_histAndTree.nLeptonsSels       = cms.vint32(-1, -1)
process.stdStop_histAndTree.muonSrc            = cms.InputTag("slimmedMuons")
process.stdStop_histAndTree.forVetoMuonSrc     = cms.InputTag("slimmedMuons")
process.stdStop_histAndTree.eleSrc             = cms.InputTag("slimmedElectrons")
process.stdStop_histAndTree.forVetoElectronSrc = cms.InputTag("slimmedElectrons")
process.stdStop_histAndTree.forVetoIsoTrkSrc   = cms.InputTag("")
process.stdStop_histAndTree.outRootName        = cms.string("StdStop.root")
process.stdStop_histAndTree.jetSrc             = cms.InputTag("stopJetsPFchsPt30")
process.stdStop_histAndTree.mhtSrc             = cms.InputTag("stopmhtPFchs")
process.stdStop_histAndTree.htSrc              = cms.InputTag("stophtPFchs")
process.stdStop_histAndTree.dPhis_CUT_vec_Src  = cms.InputTag("jetMHTDPhiForSkimsStop:jetMHTDPhiVec")
process.stdStop_histAndTree.nJets_CUT_Src      = cms.InputTag("nJetsForSkimsStop:nJets")
process.stdStop_histAndTree.mht_forSgnfSrc     = cms.InputTag("slimmedMETs")
process.stdStop_histAndTree.doFillTree         = cms.bool(True)

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("AllHadronicSUSY.SignalScan.genDecayStringMaker_cfi")
process.printDecay.keyDecayStrs = cms.vstring("t", "tbar", "~chi_1+", "~chi_1-")
process.printDecay.printDecay = cms.untracked.bool(options.debug)

process.load("AllHadronicSUSY.SignalScan.smsModelFilter_cfi")
process.smsModelFilter.SusyScanTopology   = cms.string(options.smsModel)
process.smsModelFilter.SusyScanMotherMass = cms.double(options.smsMotherMass)
process.smsModelFilter.SusyScanLSPMass    = cms.double(options.smsDaughterMass)
process.smsModelFilter.SusyScanFracLSP    = cms.double(0.0)
process.smsModelFilter.Debug              = cms.bool(options.debug)

process.StdStop_DirectionalLeptonVeto_Path = cms.Path(
                                   process.comb_seq *
                                   process.stdStop_histAndTree *
                                   process.ra2PFMuonVeto * process.ra2PFElectronVeto *
                                   process.count2JetsPFchsPt70Eta24Std *
                                   process.count4JetsPFchsPt50Eta24Std *
                                   process.count5JetsPFchsPt30Eta24Std *
                                   process.count5JetsPFchsPt30Std *
                                   process.jetsMETDPhiFilter *
                                   process.stopCount1BJets *
                                   process.type3topTagger *
                                   process.met200PFchsFilter *
                                   process.met350PFchsFilter
)

#if options.mcInfo == True:
#   process.StdStop_DirectionalLeptonVeto_Path.replace(process.stdStop_histAndTree, process.printDecay*process.stdStop_histAndTree)
#   process.StdStop_DirectionalLeptonVeto_Path.replace(process.stdStop_histAndTree, process.myGenParticlesForJetsNoNu * process.myGenParticlesForJetsNoNuNoStopDecays * process.myGenParticlesForJetsNoNuOnlyStopDecays * process.myak5GenJetsNoNu * process.myak5GenJetsNoNuNoStopDecays * process.myak5GenJetsNoNuOnlyStopDecays * process.stdStop_histAndTree)

if options.selSMSpts == True:
   process.StdStop_DirectionalLeptonVeto_Path.replace(process.hltFilter, process.hltFilter*process.smsModelFilter)

###-- Dump config ------------------------------------------------------------
file = open('allDump_cfg.py','w')
file.write(str(process.dumpPython()))
file.close()

#process.OUT = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string('test.root'),
##    outputCommands = cms.untracked.vstring(['drop *','keep patJets_patJetsAK5PF_*_*','keep patJets_patJetsAK5PFCHS_*_*','keep *_*_*_PAT'])
#    outputCommands = cms.untracked.vstring(['drop *','keep patJets_patJetsAK5PF_*_*','keep patJets_patJetsAK5PFCHS_*_*','keep *_*_*_PAT', 'keep *_pfMet_*_*', 'keep *_patMETs_*_*'])
##    outputCommands = cms.untracked.vstring(['keep *_*_*_*'])
#)
#process.endpath= cms.EndPath(process.OUT)
