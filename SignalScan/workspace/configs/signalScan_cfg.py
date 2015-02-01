from UserCode.SignalScan.patTemplate_cfg import *
# Message Logger ------------------------------------------------------------
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
   limit = cms.untracked.int32(-1),
   reportEvery = cms.untracked.int32(1)
)

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

options.register('type', 'muonCS', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "key output type string")

options.register('doTPBEfilter', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "apply Ecal dead cell TP||BE filter, default is True")

options.register('doPtHatWeighting', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "PtHat weighting for QCD flat samples, default is False")

options.register('puMCfile', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "pu MC file name")

options.register('puDatafile', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "pu Data file name")

options.register('reWeightPUmethod', 'weight3D', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "reWeight PU method")

options.register('susyScanType', 'SMS', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "SUSY scan type")
options.register('susyScanTopology', 'T1', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "SUSY scan topology")


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
      "/store/mc/Summer11/SMS-T1_Mgluino-100to1200_mLSP-50to1150_7TeV-Pythia6Z/AODSIM/PU_START42_V11_FastSim-v1/0000/FEE0CBF9-8BBF-E011-AF6B-00215E2216A4.root",
#      "/store/mc/Summer11/SMS-T2_Mgluino-100to1200_mLSP-50to1150_7TeV-Pythia6Z/AODSIM/PU_START42_V11_FastSim-v1/0000/FEB08D76-F0BF-E011-877F-E41F131816A8.root",
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

# Minimum jets requirement for standard RA2 and hadronic tau
process.count2JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25.clone()
process.count2JetsAK5PFPt50Eta25.minNumber = cms.uint32(2)

process.count4JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25.clone()
process.count4JetsAK5PFPt50Eta25.minNumber = cms.uint32(4)

process.count5JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25.clone()
process.count5JetsAK5PFPt50Eta25.minNumber = cms.uint32(5)

process.count6JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25.clone()
process.count6JetsAK5PFPt50Eta25.minNumber = cms.uint32(6)

process.count7JetsAK5PFPt50Eta25 = process.countJetsAK5PFPt50Eta25.clone()
process.count7JetsAK5PFPt50Eta25.minNumber = cms.uint32(7)

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

process.dummyCounter = cms.EDProducer("EventCountProducer")

process.load('SandBox.Skims.jetIDFailureFilter_cfi')
process.jetIDFailure.JetSource = cms.InputTag("patJetsAK5PFPt30")

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
process.weightProducer.reWeightPUmethod  = cms.untracked.string("")
process.weightProducer.reWeightPUmethod  = options.reWeightPUmethod

process.load("UserCode.SignalScan.RA2SignalScan_cfi")
process.RA2SignalScan.vtxSrc              = cms.InputTag("goodVerticesRA2")
process.RA2SignalScan.evtWeightInput      = cms.InputTag("weightProducer:weight")
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
   process.RA2SignalScan.xAxisBins           =  cms.vint32(   100   )
   process.RA2SignalScan.yAxisDivs           = cms.vdouble(  0, 1500)
   process.RA2SignalScan.yAxisBins           =  cms.vint32(   150   )
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

process.ra2AllCleaning_seq = cms.Sequence(
      process.ra2StdCleaning * process.ra2PostCleaning * process.jetIDFailure * process.dummyCounter
   )

if options.doTPBEfilter == 1:
   process.ra2AllCleaning.replace(process.dummyCounter, process.ra2TPonlyFilter*process.dummyCounter)
elif options.doTPBEfilter == 2:
   process.ra2AllCleaning.replace(process.dummyCounter, process.ra2BEonlyFilter*process.dummyCounter)
elif options.doTPBEfilter == 3:
   process.ra2AllCleaning.replace(process.dummyCounter, process.ra2TPonlyFilter*process.ra2BEonlyFilter*process.dummyCounter)

process.aux_seq = cms.Sequence(
# hlt requirement
   process.hltFilter *
# event weight
   process.weightProducer
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

#=================== Nominal RA2 ====================================================
# analysis after nominal RA2 selection & nominal JES
process.RA2SignalScan_stdRA2 = process.RA2SignalScan.clone()


#=================== JES Variation ==================================================
# jes up & jes down variations
process.load('UserCode.SignalScan.ra2JesVariation_cfi')

# analysis after smearing GenJets with core Up resolution
process.RA2SignalScan_JesUP = process.RA2SignalScan.clone()
process.RA2SignalScan_JesUP.mhtSrc = cms.InputTag("mhtJesUpJets")
process.RA2SignalScan_JesUP.htSrc  = cms.InputTag("htJesUpJets")

# analysis after smearing GenJets with core Down resolution
process.RA2SignalScan_JesDN = process.RA2SignalScan.clone()
process.RA2SignalScan_JesDN.mhtSrc = cms.InputTag("mhtJesDnJets")
process.RA2SignalScan_JesDN.htSrc  = cms.InputTag("htJesDnJets")


#=================== JER Variation ==================================================
# nominal, jer up & jer down variations
process.load('UserCode.SignalScan.ra2JerVariation_cfi')

# analysis after smearing GenJets with nominal resolution
process.RA2SignalScan_JerNominal        = process.RA2SignalScan.clone()
process.RA2SignalScan_JerNominal.mhtSrc = cms.InputTag("mhtJerJets")
process.RA2SignalScan_JerNominal.htSrc  = cms.InputTag("htJerJets")

# analysis after smearing GenJets with core Up resolution
process.RA2SignalScan_JerUP             = process.RA2SignalScan.clone()
process.RA2SignalScan_JerUP.mhtSrc      = cms.InputTag("mhtJerUpJets")
process.RA2SignalScan_JerUP.htSrc       = cms.InputTag("htJerUpJets")

# analysis after smearing GenJets with core Down resolution
process.RA2SignalScan_JerDN             = process.RA2SignalScan.clone()
process.RA2SignalScan_JerDN.mhtSrc      = cms.InputTag("mhtJerDnJets")
process.RA2SignalScan_JerDN.htSrc       = cms.InputTag("htJerDnJets")

#=================== ISR/FSR ========================================================
process.lastParticles   = cms.EDProducer("ISRProducer")
process.myProducerLabel = cms.EDProducer('ISRHistoWeightProducer',
                                         src       = cms.InputTag('lastParticles'),
                                         inputFile = cms.untracked.FileInPath("UserCode/SignalScan/data/ISRWeight_PtDistributions_T1.root"),
                                         SusyScanTopology = cms.string("T1"),
                                         Debug            = cms.bool(False),
                                         reweightVariable=cms.untracked.string('pt')
                                         )
process.myProducerLabel.SusyScanTopology = options.susyScanTopology
if options.susyScanTopology == "T1":
   process.myProducerLabel.inputFile = cms.untracked.FileInPath("UserCode/SignalScan/data/ISRWeight_PtDistributions_T1.root")
elif options.susyScanTopology == "T2":
   process.myProducerLabel.inputFile = cms.untracked.FileInPath("UserCode/SignalScan/data/ISRWeight_PtDistributions_T2.root")

process.lastPt= cms.EDAnalyzer("CandViewHistoAnalyzer",
                               src = cms.InputTag("lastParticles"),
                               # weights = cms.untracked.InputTag("myProducerLabel"),
                               histograms = cms.VPSet(
            cms.PSet(
            min = cms.untracked.double(0.0),
            max = cms.untracked.double(1000.0),
            nbins = cms.untracked.int32(100),
            name = cms.untracked.string("last_pT"),
            description = cms.untracked.string("pT [GeV/c]"),
            plotquantity = cms.untracked.string("pt")
            )
            )
)
process.lastPtReWeight= cms.EDAnalyzer("CandViewHistoAnalyzer",
                               src = cms.InputTag("lastParticles"),
                               weights = cms.untracked.InputTag("myProducerLabel"),
                               histograms = cms.VPSet(
            cms.PSet(
            min = cms.untracked.double(0.0),
            max = cms.untracked.double(1000.0),
            nbins = cms.untracked.int32(100),
            name = cms.untracked.string("last_pT"),
            description = cms.untracked.string("pT [GeV/c]"),
            plotquantity = cms.untracked.string("pt")
            )
            )
)

process.isrWeight = cms.Sequence(process.lastParticles * process.myProducerLabel * process.lastPt * process.lastPtReWeight)

process.RA2SignalScan_TotalEventsISRReweighted                = process.RA2SignalScan.clone()
process.RA2SignalScan_TotalEventsISRReweighted.fillTotalOnly  = cms.bool(True)
process.RA2SignalScan_TotalEventsISRReweighted.doISRweight    = cms.untracked.bool(True)
process.RA2SignalScan_TotalEventsISRReweighted.ISRweightSrc   = cms.InputTag("myProducerLabel")

process.RA2SignalScan_AnalysisISRReweighted                = process.RA2SignalScan.clone()
process.RA2SignalScan_AnalysisISRReweighted.doISRweight    = cms.untracked.bool(True)
process.RA2SignalScan_AnalysisISRReweighted.ISRweightSrc   = cms.InputTag("myProducerLabel")


#=================== PDF Systematics ================================================
#----systematics
#--pdf systematics
#pdfCTEQ66, pdfMRST2006nnlo, pdfMRST2007lomod
process.pdfWeight = cms.EDProducer("PdfWeightProducer",
  PdfInfoTag = cms.untracked.InputTag("generator"),
  PdfSetNames = cms.untracked.vstring("cteq66.LHgrid","MSTW2008nlo68cl.LHgrid","NNPDF20_100.LHgrid")
)

process.RA2SignalScan_PDFSysTotal                  = process.RA2SignalScan.clone()
process.RA2SignalScan_PDFSysTotal.fillTotalOnly    = cms.bool(True)
process.RA2SignalScan_PDFSysTotal.doPDFSys         = cms.untracked.bool(True)

process.RA2SignalScan_PDFSys                  = process.RA2SignalScan.clone()
process.RA2SignalScan_PDFSys.doPDFSys         = cms.untracked.bool(True)


# Add whatever process we want manually!
process.total_path = cms.Path( process.weightProducer * process.goodVerticesRA2 * process.RA2SignalScan_total )

process.scan_stdRA2_path = cms.Path(
                              process.aux_seq * process.ra2PFpat_seq * process.ra2AllCleaning_seq * process.ra2LeptonVeto_seq * 
                              process.ra2Selection * process.RA2SignalScan_stdRA2 
)

process.scan_jesUP_path = cms.Path(
                              process.aux_seq * process.ra2PFpat_seq * process.ra2AllCleaning_seq * process.ra2LeptonVeto_seq *
                              process.jesUpSequence * process.RA2SignalScan_JesUP
)
process.scan_jesDN_path = cms.Path(
                              process.aux_seq * process.ra2PFpat_seq * process.ra2AllCleaning_seq * process.ra2LeptonVeto_seq *
                              process.jesDnSequence * process.RA2SignalScan_JesDN
)

process.scan_jer_path   = cms.Path(
                              process.aux_seq * process.ra2PFpat_seq * process.ra2AllCleaning_seq * process.ra2LeptonVeto_seq *
                              process.jerNominalSequence * process.RA2SignalScan_JerNominal
)
process.scan_jerUP_path = cms.Path(
                              process.aux_seq * process.ra2PFpat_seq * process.ra2AllCleaning_seq * process.ra2LeptonVeto_seq *
                              process.jerUpSequence * process.RA2SignalScan_JerUP
)
process.scan_jerDN_path = cms.Path(
                              process.aux_seq * process.ra2PFpat_seq * process.ra2AllCleaning_seq * process.ra2LeptonVeto_seq *
                              process.jerDnSequence * process.RA2SignalScan_JerDN
)

process.scan_total_isrfsr_path = cms.Path(
                                     process.weightProducer * process.goodVerticesRA2 * process.isrWeight * process.RA2SignalScan_TotalEventsISRReweighted
)
process.scan_isrfsr_path       = cms.Path(
                                     process.aux_seq * process.ra2PFpat_seq * process.ra2AllCleaning_seq * process.ra2LeptonVeto_seq *
                                     process.ra2Selection * process.RA2SignalScan_AnalysisISRReweighted
)

#process.scan_total_pdf_path = cms.Path(
#                                  process.weightProducer * process.goodVerticesRA2 * process.pdfWeight * process.RA2SignalScan_PDFSysTotal
#)
#process.scan_pdf_path       = cms.Path(
#                                  process.aux_seq * process.ra2PFpat_seq * process.ra2AllCleaning_seq * process.ra2LeptonVeto_seq *
#                                  process.ra2Selection * process.RA2SignalScan_PDFSys
#)
