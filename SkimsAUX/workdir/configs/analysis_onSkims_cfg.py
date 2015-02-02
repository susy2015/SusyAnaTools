from UserCode.InvisibleZ.patTemplate_cfg import *

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('standard')

options.register('GlobalTag', "START41_V0::All", VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "GlobaTTag to use (otherwise default Pat GT is used)")
options.register('mcInfo', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "process MonteCarlo data, default is data")
options.register('jetCorrections', 'L2Relative', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Level of jet corrections to use: Note the factors are read from DB via GlobalTag")
options.jetCorrections.append('L3Absolute')

options.register('hltName', 'REDIGI311X', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "HLT menu to use for trigger matching") 
options.register('mcVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific MC fix")
options.register('jetTypes', 'AK5PF', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional jet types that will be produced (AK5Calo and AK5PF, cross cleaned in PF2PAT, are included anyway)")
options.register('hltSelection', '*', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "hlTriggers (OR) used to filter events. for data: ''HLT_Mu9', 'HLT_IsoMu9', 'HLT_IsoMu13_v*''; for MC, HLT_Mu9")
options.register('addKeep', '', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional keep and drop statements to trim the event content")

options.register('dataVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific DATA fix")

options.register('debug', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "switch on/off debug mode")

options.register('test', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "switch on/off debug mode")

options.register('type', 'Zinv', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "key output type string")

options.register('doTPBEfilter', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "apply Ecal dead cell TP||BE filter, default is True")

options.register('Spec42X', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "key output type string")

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
   # Data
   ]

process.source.inputCommands = cms.untracked.vstring( "keep *", "drop *_MEtoEDMConverter_*_*" )
process.maxEvents.input = options.maxEvents

#-- Calibration tag -----------------------------------------------------------
if options.GlobalTag:
   process.GlobalTag.globaltag = options.GlobalTag

# JEC
if options.mcInfo == False: options.jetCorrections.append('L2L3Residual')
options.jetCorrections.insert(0, 'L1FastJet')
if options.Spec42X == "noL2L3Residual":
   options.jetCorrections.remove('L2L3Residual')

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
process.load('SandBox.Skims.RA2Objects_cff')
process.load('SandBox.Skims.RA2Selection_cff')

process.ppf = cms.Sequence(
      process.ra2PostCleaning
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

process.noOverlapJets_seq = cms.Sequence(process.myPatJetsAK5PFPt50Eta25noOverlapLeptons*process.myPatJetsAK5PFPt30noOverlapLeptons*process.myPatJetsAK5PFnoOverlapLeptons*process.myGenJetsAK5Pt50Eta25noOverlapLeptons*process.myGenJetsAK5Pt30noOverlapLeptons)
process.prepareAnaPars_seq = cms.Sequence(process.myhtPF*process.mymhtPF*process.myhtGenPF*process.mymhtGenPF)

process.countJetsAK5PFPt50Eta25.src = cms.InputTag("myPatJetsAK5PFPt50Eta25noOverlapLeptons")

process.count1JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25.clone()
process.count1JetsAK5PFPt50Eta25.minNumber = cms.uint32(1)

process.count2JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25.clone()
process.count2JetsAK5PFPt50Eta25.minNumber = cms.uint32(2)

process.count3JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25.clone()
process.count3JetsAK5PFPt50Eta25.minNumber = cms.uint32(3)

process.mymht250PFFilter = process.mymhtPFFilter.clone()
process.mymht250PFFilter.MinMHT = cms.double(250)

process.mymhtPFFilter_HT500 = process.mymhtPFFilter.clone()

process.myht500PFFilter = process.myhtPFFilter.clone()
process.myht500PFFilter.MinHT = cms.double(500)

# Other sequence
process.comb_seq = cms.Sequence(
# All cleaning && all basic variables, e.g., mht, ht...
   process.ppf *
# hlt requirement
   process.hltFilter *
# event weight
   process.weightProducer *
# noOverlap cleaning (muonSel has only iso muon reconstructed, there is no selection cut!)
   process.muonSel_seq * process.noOverlapJets_seq *
# produce HT and MHT
   process.prepareAnaPars_seq *
# no any cuts (must after prepareAnaPars_seq)  
# Electron veto (all default settings by Steven && with patElectronsPF as input)
   process.leptonVetoPOGnoMu *
# >= 1 PF jets (pt>50 && |eta|<2.5)
   process.count1JetsAK5PFPt50Eta25 *
# Analysis at this stage 
# >= 2 PF jets (pt>50 && |eta|<2.5)
   process.count2JetsAK5PFPt50Eta25 *
# Analysis at this stage
# >= 3 PF jets (pt>50 && |eta|<2.5)
   process.count3JetsAK5PFPt50Eta25 *
# Analysis at stage3
# HT filter
   process.myhtPFFilter 
# Analysis at this stage
)

# Fix a bug if load PhysicsTools.PatAlgos.patTemplate_cfg, but no output path used
process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('*') )

process.mht150_baseline_path = cms.Path(process.comb_seq * process.myjetMHTDPhiFilter * process.mymhtPFFilter)
process.mht250_path          = cms.Path(process.comb_seq * process.myjetMHTDPhiFilter * process.mymht250PFFilter)
process.ht500_path           = cms.Path(process.comb_seq * process.myjetMHTDPhiFilter * process.myht500PFFilter * process.mymhtPFFilter_HT500)

#process.schedule = cms.Schedule( process.mht150_baseline_path, process.mht250_path, process.ht500_path )
process.schedule = cms.Schedule( process.mht150_baseline_path )
