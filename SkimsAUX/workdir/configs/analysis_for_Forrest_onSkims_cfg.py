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
   # Zinv
   # TTbarJets
'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/TTJets_TuneD6T_7TeV-madgraph-tauola/TTJets_TuneD6T-madgraph_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_1_1_nks.root',
'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/TTJets_TuneD6T_7TeV-madgraph-tauola/TTJets_TuneD6T-madgraph_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_2_1_zjI.root',
'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/TTJets_TuneD6T_7TeV-madgraph-tauola/TTJets_TuneD6T-madgraph_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_3_1_QeV.root',
'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/TTJets_TuneD6T_7TeV-madgraph-tauola/TTJets_TuneD6T-madgraph_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_4_1_hKA.root',
'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/TTJets_TuneD6T_7TeV-madgraph-tauola/TTJets_TuneD6T-madgraph_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_5_1_5ae.root',
'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/TTJets_TuneD6T_7TeV-madgraph-tauola/TTJets_TuneD6T-madgraph_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_6_1_e8p.root',
'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/TTJets_TuneD6T_7TeV-madgraph-tauola/TTJets_TuneD6T-madgraph_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_7_1_Z8u.root',
'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/TTJets_TuneD6T_7TeV-madgraph-tauola/TTJets_TuneD6T-madgraph_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_8_1_QwD.root',
'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/TTJets_TuneD6T_7TeV-madgraph-tauola/TTJets_TuneD6T-madgraph_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_9_1_5LC.root',
   # InclusiveMu15
   # WJets
   # ZJets
   # QCD
#'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/QCD_Flat_TuneZ2-pythia6_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_742_1_Tj1.root',
#'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/QCD_Flat_TuneZ2-pythia6_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_743_1_KD2.root',
#'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/QCD_Flat_TuneZ2-pythia6_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_744_1_sqX.root',
#'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/QCD_Flat_TuneZ2-pythia6_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_745_1_AlI.root',
#'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/QCD_Flat_TuneZ2-pythia6_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_746_1_Bht.root',
#'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/QCD_Flat_TuneZ2-pythia6_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_747_1_oMy.root',
#'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/QCD_Flat_TuneZ2-pythia6_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_748_1_gsu.root',
#'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/QCD_Flat_TuneZ2-pythia6_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_749_1_FKh.root',
#'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/QCD_Flat_TuneZ2-pythia6_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_750_1_3xC.root',
#'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/QCD_Flat_TuneZ2-pythia6_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_751_1_YaZ.root',
#'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/QCD_Flat_TuneZ2-pythia6_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_752_1_kGN.root',
#'dcache:/pnfs/cms/WAX/11/store/user/forrestp/2011RA2/Jun09/forrestp/QCD_Pt_15to3000_TuneZ2_Flat_7TeV_pythia6/QCD_Flat_TuneZ2-pythia6_Spring11_ra2StdCleaned_Inclusive2Jets_PROD/050ab895ba89241b70d54919e6845449/ra2SUSYPAT_753_1_vX5.root',
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

# For BE filter
process.load('UserCode.SkimsRA2.ra2BEonlyFilter_cfi')
process.ra2BEonlyFilter.ecalAnomalousFilterTag         = cms.InputTag("ecalAnomalousFilter","anomalousECALVariables")
process.ra2BEonlyFilter.maxBoundaryEnergy              = cms.double(10)
process.ra2BEonlyFilter.limitDeadCellToChannelStatusEB = cms.vint32(12,14)
process.ra2BEonlyFilter.limitDeadCellToChannelStatusEE = cms.vint32(12,14)
# For TP filter
process.load('JetMETAnalysis.ecalDeadCellTools.RA2TPfilter_cff')
process.ra2TPonlyFilter = process.ecalDeadCellTPfilter.clone()

process.ppf = cms.Sequence(
      process.ra2PostCleaning * process.ra2TPonlyFilter * process.ra2BEonlyFilter
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

# standard RA2
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

# main W+Jets analysis
process.load("UserCode.InvisibleZ.analysis_WJets_cfi")
process.analysisWJets.muonSource = cms.untracked.InputTag("myPatMuonsPFIDIso")
process.analysisWJets.jetSource = cms.untracked.InputTag("myPatJetsAK5PFPt30noOverlapLeptons")
process.analysisWJets.bTagJetSource = cms.InputTag("myPatJetsAK5PFPt50Eta25noOverlapLeptons")
process.analysisWJets.minbTagPur = cms.double(2.0)
process.analysisWJets.minbTagVeto = cms.double(1.74)

process.analysisWJets_stage1 = process.analysisWJets.clone()
process.analysisWJets_stage1.outputPath = cms.untracked.string("stage1" + "_" + options.type + "_ANA.root")

process.analysisWJets_stage5 = process.analysisWJets.clone()
process.analysisWJets_stage5.outputPath = cms.untracked.string("stage5" + "_" + options.type + "_ANA.root")
process.analysisWJets_stage5.debug = cms.bool(True)
process.analysisWJets_stage5.verbose = cms.int32(2)

process.analysisWJets_stage6 = process.analysisWJets.clone()
process.analysisWJets_stage6.outputPath = cms.untracked.string("stage6" + "_" + options.type + "_ANA.root")

# Combined analysis stage sequence
process.analysis_stage0 = cms.Sequence(process.stdRA2ana_stage0 + process.dataDrivenTTbar_stage0)

process.analysis_stage1 = cms.Sequence(process.stdRA2ana_stage1 + process.analysisWJets_stage1 + process.dataDrivenQCD_stage1 + process.dataDrivenTTbar_stage1)

process.analysis_stage5 = cms.Sequence(process.stdRA2ana_stage5 + process.analysisWJets_stage5 + process.dataDrivenQCD_stage5 + process.dataDrivenTTbar_stage5)

process.analysis_stage6 = cms.Sequence(process.stdRA2ana_stage6 + process.analysisWJets_stage6 + process.dataDrivenQCD_stage6 + process.dataDrivenTTbar_stage6)

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
# >= 3 PF jets (pt>50 && |eta|<2.5)
   process.count3JetsAK5PFPt50Eta25 *
# Analysis at stage3
# HT filter
   process.myhtPFFilter 
# Analysis at this stage
)

# Fix a bug if load PhysicsTools.PatAlgos.patTemplate_cfg, but no output path used
process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('*') )

process.mht150_baseline_path = cms.Path(process.comb_seq * process.myjetMHTDPhiFilter * process.analysis_stage5 * process.mymhtPFFilter * process.analysis_stage6)
process.mht250_path          = cms.Path(process.comb_seq * process.myjetMHTDPhiFilter * process.mymht250PFFilter)
process.ht500_path           = cms.Path(process.comb_seq * process.myjetMHTDPhiFilter * process.myht500PFFilter * process.mymhtPFFilter_HT500)

#process.schedule = cms.Schedule( process.mht150_baseline_path, process.mht250_path, process.ht500_path )
process.schedule = cms.Schedule( process.mht150_baseline_path )
