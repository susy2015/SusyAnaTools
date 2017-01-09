import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

import os
import sys
import re
import tarfile


## --------------------------
## -- Command line options --
## --------------------------

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('standard')

options.register('era', "Run2_25ns", VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "Run2_25ns or Run2_50ns")
options.register('ntpVersion', "Ntp_80X_12Jul2016_v8.0", VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "ntpVersion: to be same as the tag of the release. But can be used to produce 72X ntuple as well!")
options.register('GlobalTag', "80X_mcRun2_asymptotic_2016_miniAODv2", VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "74X PromptReco: 74X_dataRun2_Prompt_v0")
options.register('cmsswVersion', '80X', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific MC fix")
options.register('specialFix', 'JEC', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "special fixes ==>   JEC : use external JEC; IVF : fix IVF")
options.register('jecDBname', "Spring16_25nsV6_MC", VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "Summer15_25nsV6_DATA for data")
options.register('hltName', 'HLT', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "HLT menu to use for trigger matching")

options.register('mcInfo', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "process MonteCarlo data, default is data")

options.register('doPDFs', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "switch to enable the production of PDF weights for NNPDF3.0, CT10, MMHT2014, n.b. you need to do `scram setup lhapdf` before running (default=False)")

options.register('addJetsForZinv', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "switch to add top projected jets for Zinv")

options.register('externalFilterList', '', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "event list for filters")

options.register('jetCorrections', 'L2Relative', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Level of jet corrections to use: Note the factors are read from DB via GlobalTag")
options.jetCorrections.append('L3Absolute')

options.register('mcVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific MC fix")
options.register('dataVersion', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "'36X' for example. Used for specific DATA fix")

options.register('jetTypes', 'AK4PF', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional jet types that will be produced (AK4Calo and AK4PF, cross cleaned in PF2PAT, are included anyway)")
options.register('hltSelection', '*', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "hlTriggers (OR) used to filter events. for data: ''HLT_Mu9', 'HLT_IsoMu9', 'HLT_IsoMu13_v*''; for MC, HLT_Mu9")
options.register('addKeep', '', VarParsing.VarParsing.multiplicity.list, VarParsing.VarParsing.varType.string, "Additional keep and drop statements to trim the event content")

options.register('debug', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "switch on/off debug mode")
options.register('verbose', 0, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "verbose of debug")

options.register('test', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "switch on/off debug mode")

options.register('doPtHatWeighting', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "PtHat weighting for QCD flat samples, default is False")

options.register('fileslist', '', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "name of a file with input source list")

options.register('fastsim', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "fastsim sample or not, default is False")

options.register('doTopTagger', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "do top tagger or not, default is True")

options.register('usePhiCorrMET', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "use phi corrected MET or not, default is False")

options.register('reducedfilterTags', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "use phi corrected MET or not, default is True")

options.register('smsModel', 'T1tttt', VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "SMS model name")
options.register('smsMotherMass',  -1, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "SMS mother mass")
options.register('smsDaughterMass',  -1, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "SMS daughter mass")
options.register('selSMSpts', False, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "select model pobools")

options.register('pythia8', True, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.bool, "pythi8 or not, default True")

options.parseArguments()
options._tagOrder =[]

print options

## -------------------------
## -- Check CMSSW version --
## -------------------------

procCMSSWver = os.environ['CMSSW_RELEASE_BASE'].split("/")[-1]
print "procCMSSWver : ", procCMSSWver, "\n"

if not "CMSSW_8_0" in procCMSSWver:
   print "You should be using CMSSW 80X!! Please change your release area"
   sys.exit("ERROR: Not using 80X release")

if not options.cmsswVersion == "80X":
   print "You should be using CMSSW 80X as option!! Please change to be consistent with the release area"
   sys.exit("ERROR: Not using 80X option")


## ------------------------
## -- Define the process --
## ------------------------

process = cms.Process("SUSY")

if options.era == "Run2_25ns":
   process = cms.Process("SUSY", eras.Run2_25ns)
elif options.era == "Run2_50ns":
   process = cms.Process("SUSY", eras.Run2_50ns)

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

## -- MessageLogger --
process.MessageLogger.cerr.FwkReport.reportEvery = 100
if options.debug:
   process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.MessageLogger.suppressWarning = cms.untracked.vstring('ecalLaserCorrFilter','manystripclus53X','toomanystripclus53X')

## -- Options and Output Report --
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.options.allowUnscheduled = cms.untracked.bool(True)

## -- Maximal Number of Events --
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

if options.debug and options.verbose ==1:
   process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',
                                        ignoreTotal=cms.untracked.int32(0),
                                        oncePerEventMode = cms.untracked.bool(False)
                                        )
   process.Timing = cms.Service("Timing")

## -- Input Source --
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      '/store/relval/CMSSW_3_8_0_pre8/RelValTTbar/GEN-SIM-RECO/START38_V6-v1/0004/847D00B0-608E-DF11-A37D-003048678FA0.root'
    )
)

if options.files:
   process.source.fileNames = options.files
elif options.fileslist:
   inputfiles = cms.untracked.vstring()
   if os.path.exists(options.fileslist) == False or os.path.isfile(options.fileslist) == False:
      sys.exit(5)
   else:
      ifile = open(options.fileslist, 'r')
      for line in ifile.readlines():
         inputfiles.append(line)
   print "inputfiles : \n", inputfiles, "\n"
   process.source.fileNames = inputfiles
else:
   process.source.fileNames = [
        '/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/10000/7CE5EA6A-F132-E611-9E20-008CFA1660A8.root',
#       '/store/mc/RunIISpring16MiniAODv2/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/50000/041F3A63-431E-E611-9E1E-008CFA1112CC.root',
#       '/store/mc/RunIISpring16MiniAODv2/SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/004A27F0-5132-E611-A936-02163E016171.root',
#       '/store/mc/RunIISpring16MiniAODv2/SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/00775AA9-5132-E611-A4FE-001E675049F5.root',
#       '/store/mc/RunIISpring16MiniAODv2/SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/020ABCF8-5B32-E611-A85E-02163E017932.root',
#       '/store/mc/RunIISpring16MiniAODv2/SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/0246C91A-A131-E611-95E8-02163E00E646.root',

#       '/store/data/Run2016C/HTMHT/MINIAOD/PromptReco-v2/000/275/420/00000/4AD126B0-F539-E611-AD77-02163E013390.root',
#       '/store/data/Run2016C/HTMHT/MINIAOD/PromptReco-v2/000/275/476/00000/5C70C94E-0D3A-E611-AE2E-02163E01346C.root',
#       '/store/data/Run2016C/HTMHT/MINIAOD/PromptReco-v2/000/275/588/00000/BE1D644B-393A-E611-905F-02163E0124F5.root',
   ]

## ---------------------
## -- Calibration tag --
## ---------------------

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag

if options.GlobalTag:
   process.GlobalTag = GlobalTag(process.GlobalTag, options.GlobalTag, '')

if options.mcInfo == False: 
   options.jetCorrections.append('L2L3Residual')
options.jetCorrections.insert(0, 'L1FastJet')

## ---------------------------------------
## -- Create all needed jet collections --
## ---------------------------------------

from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection

## Filter out neutrinos from packed GenParticles
process.packedGenParticlesForJetsNoNu = cms.EDFilter("CandPtrSelector", 
                                                     src = cms.InputTag("packedGenParticles"), 
                                                     cut = cms.string("abs(pdgId) != 12 && abs(pdgId) != 14 && abs(pdgId) != 16"))
## Define GenJets
process.ak4GenJetsNoNu = ak4GenJets.clone(src = 'packedGenParticlesForJetsNoNu')

## -- do projections --
process.pfCHS = cms.EDFilter("CandPtrSelector", 
                             src = cms.InputTag("packedPFCandidates"), 
                             cut = cms.string("fromPV"))

process.myak4PFJetsCHS = ak4PFJets.clone(src = 'pfCHS', doAreaFastjet = True) # no idea while doArea is false by default, but it's True in RECO so we have to set it
process.myak4GenJets = ak4GenJets.clone(src = 'packedGenParticles', rParam = 0.4)

## get leptons
process.load("SusyAnaTools.SkimsAUX.prodMuons_cfi")
process.load("SusyAnaTools.SkimsAUX.prodElectrons_cfi")
process.pfNoMuonCHSNoMu =  cms.EDProducer("CandPtrProjector", 
                                          src = cms.InputTag("pfCHS"), 
                                          veto = cms.InputTag("prodMuons", "mu2Clean"))
process.pfNoElectronCHSNoEle = cms.EDProducer("CandPtrProjector", 
                                              src = cms.InputTag("pfNoMuonCHSNoMu"), 
                                              veto = cms.InputTag("prodElectrons", "ele2Clean"))
process.ak4PFJetsCHSNoLep = ak4PFJets.clone(src = 'pfNoElectronCHSNoEle', doAreaFastjet = True) # no idea while doArea is false by default, but it's True in RECO so we have to set it

## define the JECs
if options.cmsswVersion == "80X":
   jetCorrectionLevels = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None')
   jetCorrLevelLists = ['L1FastJet', 'L2Relative', 'L3Absolute']
   if options.mcInfo == False:
      jetCorrectionLevels = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']), 'None')
      jetCorrLevelLists = ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']

## -- Add the Q/G discriminator --
qgDatabaseVersion = 'v2b' # check https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion
## import Q/G database
from CondCore.DBCommon.CondDBSetup_cfi import *
QGPoolDBESSource = cms.ESSource("PoolDBESSource",
      CondDBSetup,
      toGet = cms.VPSet(),
      connect = cms.string('frontier://FrontierProd/CMS_COND_PAT_000'),
)
for type in ['AK4PFchs','AK4PFchs_antib']:
  QGPoolDBESSource.toGet.extend(cms.VPSet(cms.PSet(
    record = cms.string('QGLikelihoodRcd'),
    tag    = cms.string('QGLikelihoodObject_'+qgDatabaseVersion+'_'+type),
    label  = cms.untracked.string('QGL_'+type)
  )))

process.load('RecoJets.JetProducers.QGTagger_cfi')
process.QGTagger.jetsLabel = cms.string('QGL_AK4PFchs')  # Other options: see https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion
process.QGTagger.srcJets   = cms.InputTag('slimmedJets') # Could be reco::PFJetCollection or pat::JetCollection (both AOD and miniAOD)

# Update always for slimmedJets -> new collection name is QGAK4PFCHS
# This QGAK4PFCHS should be the same as slimmedJets except adding QGTagger for no JEC case,
# therefore only place need to use it is for prodJets.jetSrc
from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import updatedPatJets as patJetsUpdated

JetTagOut = cms.InputTag('QGAK4PFCHS')

patJetsAuxiliary = patJetsUpdated.clone(
   jetSource = cms.InputTag('slimmedJets'),
   addJetCorrFactors = cms.bool(False),
)
patJetsAuxiliary.userData.userFloats.src += ['QGTagger:qgLikelihood','QGTagger:ptD', 'QGTagger:axis2']
patJetsAuxiliary.userData.userInts.src += ['QGTagger:mult']
setattr(process,JetTagOut.value(),patJetsAuxiliary)

process.QGTaggerOther = process.QGTagger.clone()
process.QGTaggerOther.srcJets = cms.InputTag("myak4PFJetsCHS")

process.QGTaggerNoLep = process.QGTagger.clone()
process.QGTaggerNoLep.srcJets = cms.InputTag("ak4PFJetsCHSNoLep")

if options.cmsswVersion == "80X":
   addJetCollection(
      process,
      postfix = "",
      labelName = 'AK4PFCHS',
      jetSource = cms.InputTag('myak4PFJetsCHS'),
      pvSource = cms.InputTag('offlineSlimmedPrimaryVertices'),
      pfCandidates = cms.InputTag('packedPFCandidates'),
      svSource = cms.InputTag('slimmedSecondaryVertices'),
      elSource = cms.InputTag('slimmedElectrons'),
      muSource = cms.InputTag('slimmedMuons'),
      jetCorrections = jetCorrectionLevels,
      btagDiscriminators = [ 'pfCombinedInclusiveSecondaryVertexV2BJetTags' ],
      genJetCollection = cms.InputTag('ak4GenJetsNoNu'),
      genParticles = cms.InputTag('prunedGenParticles'),
      algo = 'AK', rParam = 0.4
   )
   process.patJetsAK4PFCHS.userData.userFloats.src += ['QGTaggerOther:qgLikelihood','QGTaggerOther:ptD', 'QGTaggerOther:axis2']
   process.patJetsAK4PFCHS.userData.userInts.src += ['QGTaggerOther:mult']
   
   addJetCollection(
      process,
      postfix = "",
      labelName = 'AK4PFCHSNoLep',
      jetSource = cms.InputTag('ak4PFJetsCHSNoLep'),
      pvSource = cms.InputTag('offlineSlimmedPrimaryVertices'),
      pfCandidates = cms.InputTag('packedPFCandidates'),
      svSource = cms.InputTag('slimmedSecondaryVertices'),
      elSource = cms.InputTag('slimmedElectrons'),
      muSource = cms.InputTag('slimmedMuons'),
      jetCorrections = jetCorrectionLevels,
      btagDiscriminators = [ 'pfCombinedInclusiveSecondaryVertexV2BJetTags' ],
      genJetCollection = cms.InputTag('ak4GenJetsNoNu'),
      genParticles = cms.InputTag('prunedGenParticles'),
      algo = 'AK', rParam = 0.4
   )
   process.patJetsAK4PFCHSNoLep.userData.userFloats.src += ['QGTaggerNoLep:qgLikelihood','QGTaggerNoLep:ptD', 'QGTaggerNoLep:axis2']
   process.patJetsAK4PFCHSNoLep.userData.userInts.src += ['QGTaggerNoLep:mult']

if options.specialFix == "JEC":
  print ("\nApplying fix to JEC issues in %s ...\n" %(options.cmsswVersion))
  # JEC can be downloaded from https://twiki.cern.ch/twiki/bin/viewauth/CMS/JECDataMC
  #inputDB = "sqlite_file:" + os.environ['CMSSW_BASE'] + "/src/SusyAnaTools/SkimsAUX/data/PY8_RunIISpring15DR74_bx25_MC.db"
  #print inputDB
  process.load("CondCore.DBCommon.CondDBCommon_cfi")
  from CondCore.DBCommon.CondDBSetup_cfi import *
  process.jec = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
      messageLevel = cms.untracked.int32(0)
    ),
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(
      cms.PSet(
        record = cms.string('JetCorrectionsRecord'),
        tag    = cms.string('JetCorrectorParametersCollection_'+options.jecDBname+"_AK4PFchs"),
        label  = cms.untracked.string('AK4PFchs')
      ),
      ## here you add as many jet types as you need
      ## note that the tag name is specific for the particular sqlite file 
    ),
    # from page 19 on slides https://indico.cern.ch/event/405326/contribution/2/attachments/811719/1112498/Pythia8.pdf
    # connect = cms.string('sqlite:PY8_RunIISpring15DR74_bx25_MC.db')
    connect = cms.string('sqlite:'+options.jecDBname+'.db')
    # connect = cms.string(inputDB)
    # uncomment above tag lines and this comment to use MC JEC
  )
  ## add an es_prefer statement to resolve a possible conflict from simultaneous connection to a global tag
  process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')
   

  if options.cmsswVersion == "80X":
    from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
    updateJetCollection(
      process,
      jetSource = cms.InputTag('slimmedJets'),
      postfix = 'UpdatedJEC',
      jetCorrections = ('AK4PFchs', jetCorrLevelLists, 'None')
    )
    process.updatedPatJetsUpdatedJEC.userData.userFloats.src += ['QGTagger:qgLikelihood','QGTagger:ptD', 'QGTagger:axis2']
    process.updatedPatJetsUpdatedJEC.userData.userInts.src += ['QGTagger:mult']
    # update the MET to account for the new JECs
    from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
    runMetCorAndUncFromMiniAOD(
      process,
      isData=not options.mcInfo, # controls gen met
      #jetCollUnskimmed="updatedPatJetsUpdatedJEC",
      #jetColl="updatedPatJetsUpdatedJEC",
      #postfix="Update"
    )
    process.fix80XJEC = cms.Sequence( process.patJetCorrFactorsUpdatedJEC + process.updatedPatJetsUpdatedJEC ) ## NS: What is this patJetCorrFactorsUpdatedJEC ??
    

process.load("SusyAnaTools.SkimsAUX.simpleJetSelector_cfi")
process.selectedPatJetsRA2 = process.simpleJetSelector.clone()

process.load("PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi")

# PFJets (with CHS)
process.ak4patJetsPFchsPt10     = process.selectedPatJetsRA2.clone()
process.ak4patJetsPFchsPt10.jetSrc = cms.InputTag('slimmedJets')
process.ak4patJetsPFchsPt10.pfJetCut = cms.string('pt > 10')

process.ak4patJetsPFchsPt30     = process.selectedPatJetsRA2.clone()
process.ak4patJetsPFchsPt30.jetSrc = cms.InputTag('slimmedJets')
process.ak4patJetsPFchsPt30.pfJetCut = cms.string('pt > 30')

process.ak4patJetsPFchsPt50Eta25     = process.selectedPatJetsRA2.clone()
process.ak4patJetsPFchsPt50Eta25.jetSrc = cms.InputTag('slimmedJets')
process.ak4patJetsPFchsPt50Eta25.pfJetCut = cms.string('pt > 50 & abs(eta) < 2.5')

process.patJetsAK4PFCHSPt10 = process.selectedPatJetsRA2.clone()
process.patJetsAK4PFCHSPt10.jetSrc = cms.InputTag("patJetsAK4PFCHS")
process.patJetsAK4PFCHSPt10.pfJetCut = cms.string('pt >= 10')

process.patJetsAK4PFCHSPt10NoLep = process.selectedPatJetsRA2.clone()
process.patJetsAK4PFCHSPt10NoLep.jetSrc = cms.InputTag("patJetsAK4PFCHSNoLep")
process.patJetsAK4PFCHSPt10NoLep.pfJetCut = cms.string('pt >= 10')

# PFJets - filters
process.countak4JetsPFchsPt50Eta25           = process.countPatJets.clone()
process.countak4JetsPFchsPt50Eta25.src       = cms.InputTag('ak4patJetsPFchsPt50Eta25')
process.countak4JetsPFchsPt50Eta25.minNumber = cms.uint32(3)

process.ra2PFchsJets = cms.Sequence( process.ak4patJetsPFchsPt10 * process.ak4patJetsPFchsPt30 * process.ak4patJetsPFchsPt50Eta25 )

## -- Add AK8 PUPPI jet collection using Jet Toolbox --

from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox

jetToolbox( process, 'ak8', 'ak8JetSubs', 'out', 
  runOnMC = options.mcInfo, 
  PUMethod='Puppi', 
  addSoftDropSubjets = True, 
  addSoftDrop = True, 
  addNsub = True, 
  bTagDiscriminators = ['pfCombinedInclusiveSecondaryVertexV2BJetTags'], 
  addCMSTopTagger = False)

# To get the lepton cleaned collection
process.pfCandidatesNoMu =  cms.EDProducer("CandPtrProjector", 
                                          src = cms.InputTag("packedPFCandidates"), 
                                          veto = cms.InputTag("prodMuons", "mu2Clean"))
process.pfCandidatesNoEle = cms.EDProducer("CandPtrProjector", 
                                          src = cms.InputTag("pfCandidatesNoMu"), 
                                          veto = cms.InputTag("prodElectrons", "ele2Clean"))
jetToolbox( process, 'ak8', 'ak8JetSubsNoLep', 'out', 
            runOnMC = options.mcInfo, 
            PUMethod='Puppi', 
            newPFCollection=True,
            nameNewPFCollection='pfCandidatesNoEle',
            addSoftDropSubjets = True, 
            addSoftDrop = True, 
            addNsub = True, 
            bTagDiscriminators = ['pfCombinedInclusiveSecondaryVertexV2BJetTags'], 
            addCMSTopTagger = False,
            postFix="NoLep")


## -- Other Analysis related configuration --
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
if options.hltSelection:
   process.hltFilter = hlt.hltHighLevel.clone(
      TriggerResultsTag = cms.InputTag("TriggerResults","",options.hltName),
      HLTPaths = cms.vstring(options.hltSelection),
      throw = True, # Don't throw?!
      andOr = True
   )

# HT 
process.load("SusyAnaTools.Skims.htProducer_cfi")
process.load("SusyAnaTools.Skims.htFilter_cfi")
process.htPFchs = process.ht.clone()
process.htPFchs.JetCollection = cms.InputTag("ak4patJetsPFchsPt50Eta25")
process.htPFchsFilter = process.htFilter.clone()
process.htPFchsFilter.HTSource = cms.InputTag("htPFchs")

# MHT
process.load("SusyAnaTools.Skims.mhtProducer_cfi")
process.load("SusyAnaTools.Skims.mhtFilter_cfi")
process.mhtPFchs = process.mht.clone()
process.mhtPFchs.JetCollection = cms.InputTag("ak4patJetsPFchsPt30")
process.mhtPFchsFilter = process.mhtFilter.clone()
process.mhtPFchsFilter.MHTSource = cms.InputTag("mhtPFchs")

#MT2
process.load("SusyAnaTools.SkimsAUX.mt2Producer_cfi")
#process.load("SusyAnaTools.SkimsAUX.mt2Filter_cfi")
process.mt2PFchs = process.mt2.clone()
process.mt2PFchs.JetTag = cms.InputTag("ak4patJetsPFchsPt30")
process.mt2PFchs.METTag = cms.InputTag("slimmedMETs")


# Delta Phi
process.load("SusyAnaTools.Skims.jetMHTDPhiFilter_cfi")
process.ak4jetMHTPFchsDPhiFilter = process.jetMHTDPhiFilter.clone()
process.ak4jetMHTPFchsDPhiFilter.JetSource = cms.InputTag("ak4patJetsPFchsPt30")
process.ak4jetMHTPFchsDPhiFilter.MHTSource = cms.InputTag("mhtPFchs")

process.ra2Objects = cms.Sequence( 
                                   process.ra2PFchsJets *
                                   process.htPFchs *
                                   process.mhtPFchs
                                 )

process.load("PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi")
process.load("PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi")

############################# START SUSYPAT specifics ####################################
process.prefilterCounter        = cms.EDProducer("EventCountProducer")
process.postStdCleaningCounter  = cms.EDProducer("EventCountProducer")

# Standard Event cleaning 
process.load("SusyAnaTools.SkimsAUX.prodFilterOutScraping_cfi")
process.load("SusyAnaTools.SkimsAUX.prodGoodVertices_cfi")

# an example sequence to create skimmed susypat-tuples
process.cleanpatseq = cms.Sequence(
#                      process.ra2StdCleaning          *
                      process.postStdCleaningCounter  #*
                      )
############################# EDN SUSYPAT specifics ####################################

process.dummyCounter = cms.EDProducer("EventCountProducer")

process.load('SusyAnaTools.SkimsAUX.prodJetIDEventFilter_cfi')
process.prodJetIDEventFilter.JetSource = cms.InputTag("slimmedJets")
process.prodJetIDEventFilter.MinJetPt  = cms.double(30.0)
process.prodJetIDEventFilter.MaxJetEta = cms.double(999.0)

process.prodJetIDEventFilterNoLep = process.prodJetIDEventFilter.clone()
process.prodJetIDEventFilterNoLep.JetSource = cms.InputTag("patJetsAK4PFCHSPt10NoLep")

process.load('SusyAnaTools.SkimsAUX.weightProducer_cfi')
process.weightProducer.inputPUfileMC   = cms.untracked.string("")
process.weightProducer.inputPUfileData = cms.untracked.string("")
if options.doPtHatWeighting:
  process.weightProducer.Method     = cms.string("PtHat")
  process.weightProducer.Exponent   = cms.double(-4.5)
  process.weightProducer.XS         = cms.double(1.0)
  process.weightProducer.NumberEvts = cms.double(1.0)
  process.weightProducer.Lumi       = cms.double(1.0)
  process.weightProducer.weightWARNingUpThreshold  = cms.double(2.0)

from JetMETCorrections.Configuration.DefaultJEC_cff import *
process.ak4PFJetschsL1FastL2L3Residual = ak4PFJetsL1FastL2L3Residual.clone( algorithm = cms.string('AK4PFchs'), src = 'slimmedJets' )
process.ak4PFJetschsL1FastL2L3 = ak4PFJetsL1FastL2L3.clone( algorithm = cms.string('AK4PFchs'), src = 'slimmedJets' )

# Default is dR = 0.3, dz < 0.05, pt > 10, reliso < 0.1
process.load("SusyAnaTools.Skims.trackIsolationMaker_cfi")
process.trackIsolation = process.trackIsolationFilter.clone()
process.trackIsolation.pfCandidatesTag = cms.InputTag("packedPFCandidates")
process.trackIsolation.doTrkIsoVeto = cms.bool(False)

process.loosetrackIsolation = process.trackIsolation.clone()
process.loosetrackIsolation.minPt_PFCandidate = cms.double(5.0)
process.loosetrackIsolation.isoCut            = cms.double(0.5)

process.refalltrackIsolation = process.trackIsolation.clone()
process.refalltrackIsolation.mintPt_PFCandidate = cms.double (-1.0)
process.refalltrackIsolation.isoCut           = cms.double(9999.0)

process.load('SusyAnaTools.Skims.StopJets_drt_from_AOD_cff')
process.load("SusyAnaTools.SkimsAUX.nJetsForSkimsRA2_cfi")
process.load("SusyAnaTools.SkimsAUX.jetMHTDPhiForSkimsRA2_cfi")

# ak4 jets
process.ak4stopJetsPFchsPt30 = process.stopJetsPFchsPt30.clone(jetSrc = "slimmedJets")
process.ak4stopJetsPFchsPt50Eta24 = process.stopJetsPFchsPt50Eta24.clone(jetSrc = "slimmedJets")

process.ak4nJetsForSkimsStop = process.nJetsForSkimsRA2.clone()
process.ak4nJetsForSkimsStop.JetSource = cms.InputTag("ak4stopJetsPFchsPt30")
process.ak4jetMHTDPhiForSkimsStop = process.jetMHTDPhiForSkimsRA2.clone()
process.ak4jetMHTDPhiForSkimsStop.MHTSource = cms.InputTag("slimmedMETs")
process.ak4jetMHTDPhiForSkimsStop.JetSource = cms.InputTag("ak4stopJetsPFchsPt30")

process.ak4stophtPFchs = process.htPFchs.clone()
process.ak4stophtPFchs.JetCollection = cms.InputTag("ak4stopJetsPFchsPt50Eta24")

process.ak4stopmhtPFchs = process.mhtPFchs.clone()
process.ak4stopmhtPFchs.JetCollection = cms.InputTag("ak4stopJetsPFchsPt30")
#

process.prepareCutvars_seq = cms.Sequence( process.ak4stopJetsPFchsPt30 * process.ak4stopJetsPFchsPt50Eta24 * process.ak4nJetsForSkimsStop * process.ak4jetMHTDPhiForSkimsStop * process.ak4stophtPFchs * process.ak4stopmhtPFchs )

process.load("SusyAnaTools.Skims.StopBTagJets_cff")
process.stopBJets.JetSrc = cms.InputTag("stopJetsPFchsPt30")

# Other sequence
process.comb_seq = cms.Sequence(
  # All cleaning && all basic variables, e.g., mht, ht...     
  process.cleanpatseq *
  # hlt requirement
  process.hltFilter *
  process.prodMuons * process.prodElectrons * 
  process.QGTagger * process.QGTaggerOther * process.QGTaggerNoLep * process.QGAK4PFCHS *
  process.weightProducer *
  process.trackIsolation * process.loosetrackIsolation * process.refalltrackIsolation * 
  process.stopPFJets * process.stopBJets *
  process.ra2Objects *
  process.prepareCutvars_seq
)

process.load("SusyAnaTools.Skims.StopDPhiSelection_cff")
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

process.load("SusyAnaTools.Skims.StopType3TopTagger_cff")
if options.usePhiCorrMET == True:
   process.type3topTagger.metSrc = cms.InputTag("slimmedMETs")
else:
   process.type3topTagger.metSrc = cms.InputTag("slimmedMETs")
process.type3topTagger.taggingMode = cms.untracked.bool(True)
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
   fileName = cms.string('stopFlatNtuples.root')
)

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load("SusyAnaTools.SignalScan.genDecayStringMaker_cfi")
process.printDecay.src = cms.InputTag("prunedGenParticles")
process.printDecay.keyDecayStrs = cms.vstring("t", "tbar", "~chi_1+", "~chi_1-")
process.printDecay.printDecay = cms.untracked.bool(options.debug)

process.load("SusyAnaTools.SignalScan.genDecayStringMakerPythia8_cfi")
process.printDecayPythia8.src = cms.InputTag("prunedGenParticles")
process.printDecayPythia8.keyDecayStrs = cms.vstring("t", "tbar", "~chi_1+", "~chi_1-")
process.printDecayPythia8.printDecay = cms.untracked.bool(options.debug)

process.load("SusyAnaTools.SignalScan.smsModelFilter_cfi")
process.smsModelFilter.SusyScanTopology   = cms.string(options.smsModel)
process.smsModelFilter.SusyScanMotherMass = cms.double(options.smsMotherMass)
process.smsModelFilter.SusyScanLSPMass    = cms.double(options.smsDaughterMass)
process.smsModelFilter.SusyScanFracLSP    = cms.double(0.0)
process.smsModelFilter.Debug              = cms.bool(options.debug)

process.load("SusyAnaTools.TopTagger.groomProd_cfi")
process.groomProdak4 = process.groomProd.clone()
process.groomProdak4.jetSrc = cms.InputTag("ak4patJetsPFchsPt10")
process.groomProdak4.groomingOpt = cms.untracked.int32(1)
#process.groomProdak4.debug = cms.untracked.bool(options.debug)

process.load("SusyAnaTools.SkimsAUX.prodJets_cfi")
process.load("SusyAnaTools.SkimsAUX.prodGenJets_cfi")
process.load("SusyAnaTools.SkimsAUX.prodMET_cfi")
process.load("SusyAnaTools.SkimsAUX.prodGenInfo_cfi")
process.load("SusyAnaTools.SkimsAUX.prodIsoTrks_cfi")
process.load("SusyAnaTools.SkimsAUX.prodEventInfo_cfi")
process.load("SusyAnaTools.SkimsAUX.ISRJetProducer_cfi")

#Addition of Filter Decision Bits and Trigger Results
process.load("SusyAnaTools.SkimsAUX.prodTriggerResults_cfi")
process.load("SusyAnaTools.SkimsAUX.prodFilterFlags_cfi")

#rerun HBHE noise filter manually
process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
process.HBHENoiseFilterResultProducer.minZeros = cms.int32(99999)
process.HBHENoiseFilterResultProducer.IgnoreTS4TS5ifJetInLowBVRegion=cms.bool(False)
process.HBHENoiseFilterResultProducer.defaultDecision = cms.string("HBHENoiseFilterResultRun2Loose")

#New met filters for 2016
process.load('RecoMET.METFilters.BadChargedCandidateFilter_cfi')
process.load('RecoMET.METFilters.BadPFMuonFilter_cfi')
process.BadChargedCandidateFilter.taggingMode = cms.bool(True)
process.BadChargedCandidateFilter.muons = cms.InputTag("slimmedMuons")
process.BadChargedCandidateFilter.PFCandidates = cms.InputTag("packedPFCandidates")
process.BadPFMuonFilter.taggingMode = cms.bool(True)
process.BadPFMuonFilter.muons = cms.InputTag("slimmedMuons")
process.BadPFMuonFilter.PFCandidates = cms.InputTag("packedPFCandidates")

process.triggerProducer.trigTagSrc = cms.InputTag("TriggerResults","",options.hltName)
#process.METFilters = process.filterDecisionProducer.clone( filterName  =   cms.string("Flag_METFilters") )
process.CSCTightHaloFilter = process.filterDecisionProducer.clone( filterName  =   cms.string("Flag_CSCTightHaloFilter") )
process.globalTightHalo2016Filter = process.filterDecisionProducer.clone( filterName  =   cms.string("Flag_globalTightHalo2016Filter") )
process.goodVerticesFilter = process.filterDecisionProducer.clone( filterName  =   cms.string("Flag_goodVertices") )
process.eeBadScFilter = process.filterDecisionProducer.clone( filterName  =   cms.string("Flag_eeBadScFilter") )
#process.HBHENoiseFilter = process.filterDecisionProducer.clone( filterName  =   cms.string("Flag_HBHENoiseFilter") )
process.EcalDeadCellTriggerPrimitiveFilter = process.filterDecisionProducer.clone( filterName  =   cms.string("Flag_EcalDeadCellTriggerPrimitiveFilter") )

process.prodJets.bTagKeyString = cms.string('pfCombinedInclusiveSecondaryVertexV2BJetTags')

process.prodJets.debug = cms.bool(options.debug)
process.prodJets.jetSrc = cms.InputTag('QGAK4PFCHS')
process.prodJets.jetOtherSrc = cms.InputTag('patJetsAK4PFCHS')

process.prodJetsNoLep = process.prodJets.clone()
process.prodJetsNoLep.jetSrc = cms.InputTag('patJetsAK4PFCHSPt10NoLep')
process.prodJetsNoLep.jetOtherSrc = cms.InputTag('patJetsAK4PFCHSPt10NoLep')
process.prodJetsNoLep.qgTaggerKey = cms.string('QGTaggerNoLep')
process.prodJetsNoLep.puppiJetsSrc = cms.InputTag('selectedPatJetsAK8PFPuppiNoLep')
process.prodJetsNoLep.puppiSubJetsSrc = cms.InputTag('selectedPatJetsAK8PFPuppiNoLepSoftDropPacked')
process.prodJetsNoLep.NjettinessAK8Puppi_label = cms.string('NjettinessAK8PuppiNoLep')
process.prodJetsNoLep.ak8PFJetsPuppi_label = cms.string('ak8PFJetsPuppiNoLep')
#process.prodJetsNoLep.ak8JetsSrc = cms.string('slimmedJetsAK8NoLep')
#process.prodJetsNoLep.ak8SubJetsSrc = cms.string('slimmedJetsAK8PFCHSSoftDropPackedNoLep') #NS To be updated

process.prodMuonsNoIso = process.prodMuons.clone()
process.prodMuonsNoIso.DoMuonIsolation = cms.int32(0)

process.prodElectronsNoIso = process.prodElectrons.clone()
process.prodElectronsNoIso.DoElectronIsolation = cms.int32(0)

process.load("SusyAnaTools.StopTreeMaker.stopTreeMaker_cfi")
process.stopTreeMaker.debug = cms.bool(options.debug)
process.stopTreeMaker.TreeName = cms.string("AUX")

process.ntpVersion = cms.EDFilter(
   "prodNtupleVersionString", 
   inputStr = cms.vstring(options.ntpVersion, options.GlobalTag, options.cmsswVersion, options.specialFix, options.hltName, options.era, options.jecDBname, procCMSSWver)
)

process.stopTreeMaker.vectorString.append(cms.InputTag("ntpVersion"))

process.stopTreeMaker.vectorInt.append(cms.InputTag("triggerProducer", "PassTrigger"))
process.stopTreeMaker.vectorInt.append(cms.InputTag("triggerProducer", "TriggerPrescales"))
process.stopTreeMaker.vectorString.append(cms.InputTag("triggerProducer", "TriggerNames"))

# prodGoodVertices has the same as vtxSize in prodEventInfo...
#process.stopTreeMaker.varsInt.append(cms.InputTag("prodGoodVertices"))
#process.stopTreeMaker.varsInt.append(cms.InputTag("prodFilterOutScraping"))
process.stopTreeMaker.varsBool.append(cms.InputTag("prodJetIDEventFilter", "looseJetID"))
process.stopTreeMaker.varsBool.append(cms.InputTag("prodJetIDEventFilter", "tightJetID"))
process.stopTreeMaker.varsBool.append(cms.InputTag("prodJetIDEventFilter", "tightlepvetoJetID"))
process.stopTreeMaker.varsBool.append(cms.InputTag("prodJetIDEventFilterNoLep", "looseJetID"))
process.stopTreeMaker.varsBoolNamesInTree.append("prodJetIDEventFilterNoLep:looseJetID|looseJetID_NoLep")
process.stopTreeMaker.varsBool.append(cms.InputTag("prodJetIDEventFilterNoLep", "tightJetID"))
process.stopTreeMaker.varsBoolNamesInTree.append("prodJetIDEventFilterNoLep:tightJetID|tightJetID_NoLep")
process.stopTreeMaker.varsBool.append(cms.InputTag("prodJetIDEventFilterNoLep", "tightlepvetoJetID"))
process.stopTreeMaker.varsBoolNamesInTree.append("prodJetIDEventFilterNoLep:tightlepvetoJetID|tightlepvetoJetID_NoLep")
#process.stopTreeMaker.varsInt.append(cms.InputTag("METFilters"))
#process.stopTreeMaker.varsInt.append(cms.InputTag("CSCTightHaloFilter")) # 74X txt files are ready for the 2015 working point, use this and not the flag in miniAOD 
process.stopTreeMaker.varsInt.append(cms.InputTag("globalTightHalo2016Filter"))
process.stopTreeMaker.varsInt.append(cms.InputTag("goodVerticesFilter"))
process.stopTreeMaker.varsInt.append(cms.InputTag("eeBadScFilter"))
process.stopTreeMaker.varsInt.append(cms.InputTag("EcalDeadCellTriggerPrimitiveFilter"))
process.stopTreeMaker.varsBool.append(cms.InputTag("BadChargedCandidateFilter"))
process.stopTreeMaker.varsBool.append(cms.InputTag("BadPFMuonFilter"))

if options.fastsim == False:
   process.stopTreeMaker.varsBool.append(cms.InputTag("HBHENoiseFilterResultProducer", "HBHENoiseFilterResult"))
   process.stopTreeMaker.varsBoolNamesInTree.append("HBHENoiseFilterResultProducer:HBHENoiseFilterResult|HBHENoiseFilter")

   process.stopTreeMaker.varsBool.append(cms.InputTag("HBHENoiseFilterResultProducer", "HBHEIsoNoiseFilterResult"))
   process.stopTreeMaker.varsBoolNamesInTree.append("HBHENoiseFilterResultProducer:HBHEIsoNoiseFilterResult|HBHEIsoNoiseFilter")

process.stopTreeMaker.varsInt.append(cms.InputTag("prodMuons", "nMuons"))
process.stopTreeMaker.varsIntNamesInTree.append("prodMuons:nMuons|nMuons_CUT")
process.stopTreeMaker.varsInt.append(cms.InputTag("prodMuonsNoIso", "nMuons"))
process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodMuonsNoIso", "muonsLVec"))
process.stopTreeMaker.vectorDouble.extend([cms.InputTag("prodMuonsNoIso", "muonsCharge"), cms.InputTag("prodMuonsNoIso", "muonsMtw"), cms.InputTag("prodMuonsNoIso", "muonsRelIso"), cms.InputTag("prodMuonsNoIso", "muonsMiniIso"), cms.InputTag("prodMuonsNoIso", "muonspfActivity")])
process.stopTreeMaker.vectorInt.extend([cms.InputTag("prodMuonsNoIso", "muonsFlagMedium"), cms.InputTag("prodMuonsNoIso", "muonsFlagTight")])

process.stopTreeMaker.varsInt.append(cms.InputTag("prodElectrons", "nElectrons"))
process.stopTreeMaker.varsIntNamesInTree.append("prodElectrons:nElectrons|nElectrons_CUT")
process.stopTreeMaker.varsInt.append(cms.InputTag("prodElectronsNoIso", "nElectrons"))
process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodElectronsNoIso", "elesLVec"))
process.stopTreeMaker.vectorDouble.extend([cms.InputTag("prodElectronsNoIso", "elesCharge"), cms.InputTag("prodElectronsNoIso", "elesMtw"), cms.InputTag("prodElectronsNoIso", "elesRelIso"), cms.InputTag("prodElectronsNoIso", "elesMiniIso"), cms.InputTag("prodElectronsNoIso", "elespfActivity")])
process.stopTreeMaker.vectorBool.extend([cms.InputTag("prodElectronsNoIso", "elesisEB")])
process.stopTreeMaker.vectorInt.extend([cms.InputTag("prodElectronsNoIso", "elesFlagMedium"), cms.InputTag("prodElectronsNoIso", "elesFlagVeto")])

process.stopTreeMaker.varsInt.append(cms.InputTag("prodJets", "nJets"))
process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodJets", "jetsLVec"))
#process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodGenJets", "genjetsLVec"))
process.stopTreeMaker.vectorInt.append(cms.InputTag("prodJets", "recoJetsFlavor"))

process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsJecUnc"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsJecScaleRawToFull"))

process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "qgLikelihood"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "qgPtD"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "qgAxis2"))
process.stopTreeMaker.vectorInt.append(cms.InputTag("prodJets", "qgMult"))

process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetschargedHadronEnergyFraction"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetschargedEmEnergyFraction"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsneutralEmEnergyFraction"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsmuonEnergyFraction"))

process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsBtag"))
process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJets:recoJetsBtag|recoJetsBtag_0")
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsCharge"))
process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJets:recoJetsCharge|recoJetsCharge_0")

process.stopTreeMaker.vectorInt.append(cms.InputTag("prodJets", "muMatchedJetIdx"))
process.stopTreeMaker.vectorInt.append(cms.InputTag("prodJets", "eleMatchedJetIdx"))
process.stopTreeMaker.vectorInt.append(cms.InputTag("prodJets", "looseisoTrksMatchedJetIdx"))
process.stopTreeMaker.vectorInt.append(cms.InputTag("prodJets", "trksForIsoVetoMatchedJetIdx"))

process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodJets", "puppiJetsLVec"))
process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodJets", "puppiSubJetsLVec"))
process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodJets", "ak8JetsLVec"))
process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodJets", "ak8SubJetsLVec"))

process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "tau1"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "tau2"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "tau3"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "softDropMass"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "ak8SubJetsBdisc"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "puppitau1"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "puppitau2"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "puppitau3"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "puppisoftDropMass"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "puppiSubJetsBdisc"))

if options.addJetsForZinv == True: 
   process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodJetsNoLep", "jetsLVec"))
   process.stopTreeMaker.vectorTLorentzVectorNamesInTree.append("prodJetsNoLep:jetsLVec|jetsLVecLepCleaned")

   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "recoJetsJecUnc"))
   process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJetsNoLep:recoJetsJecUnc|recoJetsJecUncLepCleaned")

   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "qgLikelihood"))
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "qgPtD"))
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "qgAxis2"))
   process.stopTreeMaker.vectorInt.append(cms.InputTag("prodJetsNoLep", "qgMult"))

   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "recoJetschargedHadronEnergyFraction"))
   process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJetsNoLep:recoJetschargedHadronEnergyFraction|recoJetschargedHadronEnergyFractionLepCleaned")
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "recoJetsneutralEmEnergyFraction"))
   process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJetsNoLep:recoJetsneutralEmEnergyFraction|recoJetsneutralEmEnergyFractionLepCleaned")
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "recoJetschargedEmEnergyFraction"))
   process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJetsNoLep:recoJetschargedEmEnergyFraction|recoJetschargedEmEnergyFractionLepCleaned")
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "recoJetsmuonEnergyFraction"))
   process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJetsNoLep:recoJetsmuonEnergyFraction|recoJetsmuonEnergyFractionLepCleaned")

   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "recoJetsBtag"))
   process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJetsNoLep:recoJetsBtag|recoJetsBtag_0_LepCleaned")
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "recoJetsCharge"))
   process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJetsNoLep:recoJetsCharge|recoJetsCharge_0_LepCleaned")

   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "recoJetsJecScaleRawToFull"))
   process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJetsNoLep:recoJetsJecScaleRawToFull|recoJetsJecScaleRawToFull_LepCleaned")

   process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodJetsNoLep", "puppiJetsLVec"))
   process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodJetsNoLep", "puppiSubJetsLVec"))
   #process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodJetsNoLep", "ak8JetsLVec"))

   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "tau1"))
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "tau2"))
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "tau3"))
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "puppisoftDropMass"))
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "puppitau1"))
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "puppitau2"))
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "puppitau3"))
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJetsNoLep", "puppiSubJetsBdisc"))

if options.mcInfo == True:
   process.prodGenInfo.debug = cms.bool(options.debug)
   process.stopTreeMaker.vectorString.append(cms.InputTag("prodGenInfo", "genDecayStrVec"))
   process.stopTreeMaker.vectorInt.extend([cms.InputTag("prodGenInfo", "genDecayIdxVec"), cms.InputTag("prodGenInfo", "genDecayPdgIdVec"), cms.InputTag("prodGenInfo", "genDecayMomIdxVec"), cms.InputTag("prodGenInfo", "genDecayMomRefVec"), cms.InputTag("prodGenInfo", "WemuVec"), cms.InputTag("prodGenInfo", "WtauVec"), cms.InputTag("prodGenInfo", "WtauemuVec"), cms.InputTag("prodGenInfo", "WtauprongsVec"), cms.InputTag("prodGenInfo", "WtaunuVec"),  cms.InputTag("prodGenInfo","selPDGid")])
   process.stopTreeMaker.vectorIntNamesInTree.extend(["prodGenInfo:WemuVec|W_emuVec", "prodGenInfo:WtauVec|W_tauVec", "prodGenInfo:WtauemuVec|W_tau_emuVec", "prodGenInfo:WtauprongsVec|W_tau_prongsVec", "prodGenInfo:WtaunuVec|W_tau_nuVec"])
   process.stopTreeMaker.vectorDouble.extend([cms.InputTag("prodGenInfo", "WemupfActivityVec"), cms.InputTag("prodGenInfo", "WtauemupfActivityVec"), cms.InputTag("prodGenInfo", "WtauprongspfActivityVec")])
   process.stopTreeMaker.vectorDoubleNamesInTree.extend(["prodGenInfo:WemupfActivityVec|W_emu_pfActivityVec", "prodGenInfo:WtauemupfActivityVec|W_tau_emu_pfActivityVec", "prodGenInfo:WtauprongspfActivityVec|W_tau_prongs_pfActivityVec"])
   process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodGenInfo", "genDecayLVec"))
   process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodGenInfo", "selGenParticle"))

   #isrJets
   process.ISRJetProducer.debug = cms.bool(options.debug)
   process.stopTreeMaker.varsInt.append(cms.InputTag("ISRJetProducer", "NJetsISR"))

   process.genHT = cms.EDProducer('GenHTProducer')
   process.stopTreeMaker.varsDouble.append(cms.InputTag("genHT", "genHT"))
   process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodGenJets", "genjetsLVec"))
   process.stopTreeMaker.varsDouble.extend([cms.InputTag("prodMET:genmet"), cms.InputTag("prodMET:genmetphi")])

   process.PDFWeights = cms.EDProducer('PDFWeightProducer')
   process.stopTreeMaker.varsDouble.append(cms.InputTag("PDFWeights", "x1"))
   process.stopTreeMaker.varsDouble.append(cms.InputTag("PDFWeights", "x2"))
   process.stopTreeMaker.varsDouble.append(cms.InputTag("PDFWeights", "q"))
   process.stopTreeMaker.varsInt.append(cms.InputTag("PDFWeights", "id1"))
   process.stopTreeMaker.varsInt.append(cms.InputTag("PDFWeights", "id2"))
   process.stopTreeMaker.vectorDouble.append(cms.InputTag("PDFWeights", "ScaleWeightsMiniAOD"))

   if options.doPDFs == True:
      process.stopTreeMaker.vectorDouble.append(cms.InputTag("PDFWeights", "PDFweights"))
      process.stopTreeMaker.vectorInt.append(cms.InputTag("PDFWeights", "PDFids"))
      process.stopTreeMaker.vectorDouble.append(cms.InputTag("PDFWeights", "PDFweightsMiniAOD"))
      process.stopTreeMaker.vectorInt.append(cms.InputTag("PDFWeights", "PDFidsMiniAOD"))

   if options.fastsim == True:
      process.load("SusyAnaTools.SkimsAUX.susyscan_cfi")
      process.SusyScanProducer.shouldScan = cms.bool(options.fastsim)
      process.stopTreeMaker.varsDouble.extend([cms.InputTag("SusyScanProducer", "SusyMotherMass"), cms.InputTag("SusyScanProducer", "SusyLSPMass")])

      process.prodJets.jetOtherSrc = cms.InputTag('slimmedJets')

process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodIsoTrks:trksForIsoVetoLVec"))

process.stopTreeMaker.vectorDouble.extend([cms.InputTag("prodIsoTrks:trksForIsoVetocharge"), cms.InputTag("prodIsoTrks:trksForIsoVetodz"), cms.InputTag("prodIsoTrks:trksForIsoVetoiso"), cms.InputTag("prodIsoTrks:trksForIsoVetopfActivity"), cms.InputTag("prodIsoTrks:looseisoTrkscharge"), cms.InputTag("prodIsoTrks:looseisoTrksdz"), cms.InputTag("prodIsoTrks:looseisoTrksiso"), cms.InputTag("prodIsoTrks:looseisoTrksmtw"), cms.InputTag("prodIsoTrks:looseisoTrkspfActivity")])
process.stopTreeMaker.vectorDoubleNamesInTree.extend(["prodIsoTrks:trksForIsoVetocharge|trksForIsoVeto_charge", "prodIsoTrks:trksForIsoVetodz|trksForIsoVeto_dz", "prodIsoTrks:trksForIsoVetoiso|trksForIsoVeto_iso", "prodIsoTrks:trksForIsoVetopfActivity|trksForIsoVeto_pfActivity", "prodIsoTrks:looseisoTrkscharge|loose_isoTrks_charge", "prodIsoTrks:looseisoTrksdz|loose_isoTrks_dz", "prodIsoTrks:looseisoTrksiso|loose_isoTrks_iso", "prodIsoTrks:looseisoTrksmtw|loose_isoTrks_mtw", "prodIsoTrks:looseisoTrkspfActivity|loose_isoTrks_pfActivity"])

process.stopTreeMaker.vectorInt.extend([cms.InputTag("prodIsoTrks:trksForIsoVetopdgId"), cms.InputTag("prodIsoTrks:trksForIsoVetoidx"), cms.InputTag("prodIsoTrks:looseisoTrkspdgId"), cms.InputTag("prodIsoTrks:looseisoTrksidx"), cms.InputTag("prodIsoTrks:forVetoIsoTrksidx")])
process.stopTreeMaker.vectorIntNamesInTree.extend(["prodIsoTrks:trksForIsoVetopdgId|trksForIsoVeto_pdgId", "prodIsoTrks:trksForIsoVetoidx|trksForIsoVeto_idx", "prodIsoTrks:looseisoTrkspdgId|loose_isoTrks_pdgId", "prodIsoTrks:looseisoTrksidx|loose_isoTrks_idx"])

process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodIsoTrks:looseisoTrksLVec"))
process.stopTreeMaker.vectorTLorentzVectorNamesInTree.append("prodIsoTrks:looseisoTrksLVec|loose_isoTrksLVec")

process.stopTreeMaker.varsInt.extend([cms.InputTag("prodIsoTrks:loosenIsoTrks"), cms.InputTag("prodIsoTrks:nIsoTrksForVeto")])
process.stopTreeMaker.varsIntNamesInTree.extend(["prodIsoTrks:loosenIsoTrks|loose_nIsoTrks", "prodIsoTrks:nIsoTrksForVeto|nIsoTrks_CUT"])

process.stopTreeMaker.varsDouble.extend([cms.InputTag("ak4stopmhtPFchs:mht"), cms.InputTag("ak4stopmhtPFchs:mhtphi")])

process.stopTreeMaker.varsDouble.append(cms.InputTag("mt2PFchs:mt2"))

process.stopTreeMaker.varsDouble.append(cms.InputTag("ak4stophtPFchs"))
process.stopTreeMaker.varsDoubleNamesInTree.append("ak4stophtPFchs|ht")

process.stopTreeMaker.varsInt.append(cms.InputTag("ak4nJetsForSkimsStop:nJets"))
process.stopTreeMaker.varsIntNamesInTree.append("ak4nJetsForSkimsStop:nJets|nJets_CUT")

process.stopTreeMaker.varsDouble.extend([cms.InputTag("prodMET:met"), cms.InputTag("prodMET:metphi")])
process.stopTreeMaker.varsDouble.extend([cms.InputTag("prodMET:calomet"), cms.InputTag("prodMET:calometphi")])
process.stopTreeMaker.vectorDouble.extend([cms.InputTag("prodMET:metMagUp"), cms.InputTag("prodMET:metMagDown"), cms.InputTag("prodMET:metPhiUp"), cms.InputTag("prodMET:metPhiDown")])

process.stopTreeMaker.varsDouble.extend([cms.InputTag("ak4jetMHTDPhiForSkimsStop:dPhi0"), cms.InputTag("ak4jetMHTDPhiForSkimsStop:dPhi1"), cms.InputTag("ak4jetMHTDPhiForSkimsStop:dPhi2")])
process.stopTreeMaker.varsDoubleNamesInTree.extend(["ak4jetMHTDPhiForSkimsStop:dPhi0|dPhi0_CUT", "ak4jetMHTDPhiForSkimsStop:dPhi1|dPhi1_CUT", "ak4jetMHTDPhiForSkimsStop:dPhi2|dPhi2_CUT"])

process.stopTreeMaker.varsInt.extend([cms.InputTag("prodEventInfo:vtxSize"), cms.InputTag("prodEventInfo:npv"), cms.InputTag("prodEventInfo:nm1"), cms.InputTag("prodEventInfo:n0"), cms.InputTag("prodEventInfo:np1")])
process.stopTreeMaker.varsDouble.extend([cms.InputTag("prodEventInfo:trunpv"), cms.InputTag("prodEventInfo:avgnpv"), cms.InputTag("prodEventInfo:storedWeight")])
process.stopTreeMaker.varsDoubleNamesInTree.extend(["prodEventInfo:trunpv|tru_npv", "prodEventInfo:avgnpv|avg_npv", "prodEventInfo:storedWeight|stored_weight"])

if options.doTopTagger == True:
   process.stopTreeMaker.varsInt.extend([cms.InputTag("type3topTagger:bestTopJetIdx"), cms.InputTag("type3topTagger:pickedRemainingCombfatJetIdx")])
   process.stopTreeMaker.varsDouble.extend([cms.InputTag("type3topTagger:bestTopJetMass"), cms.InputTag("type3topTagger:MT2"), cms.InputTag("type3topTagger:mTbestTopJet"), cms.InputTag("type3topTagger:mTbJet"), cms.InputTag("type3topTagger:linearCombmTbJetPlusmTbestTopJet"), cms.InputTag("type3topTagger:mTbestWJet"), cms.InputTag("type3topTagger:mTbestbJet"), cms.InputTag("type3topTagger:mTremainingTopJet")])
   process.stopTreeMaker.varsBool.append(cms.InputTag("type3topTagger:remainPassCSVS"))

process.stopTreeMaker.varsDouble.append(cms.InputTag("weightProducer:weight"))
process.stopTreeMaker.varsDoubleNamesInTree.append("weightProducer:weight|evtWeight")

if options.fastsim == False:
   process.trig_filter_seq = cms.Sequence( process.HBHENoiseFilterResultProducer * process.triggerProducer * process.CSCTightHaloFilter * process.globalTightHalo2016Filter * process.goodVerticesFilter * process.eeBadScFilter * process.EcalDeadCellTriggerPrimitiveFilter * process.BadChargedCandidateFilter * process.BadPFMuonFilter ) 
else:
   process.trig_filter_seq = cms.Sequence( process.triggerProducer * process.CSCTightHaloFilter * process.globalTightHalo2016Filter * process.goodVerticesFilter * process.eeBadScFilter * process.EcalDeadCellTriggerPrimitiveFilter * process.BadChargedCandidateFilter * process.BadPFMuonFilter ) 

if options.externalFilterList:
   process.load("SusyAnaTools.SkimsAUX.EventListFilter_cfi")
   for flist in options.externalFilterList:
      tfile = tarfile.open(flist, 'r:gz')
      tfile.extractall('.')
      flist = flist.replace(".txt.tar.gz", ".txt")
      if flist.find("ecalsc") != -1:
         process.eeBadScListFilter = process.EventListFilter.clone(inputFileList=flist)
         process.trig_filter_seq += process.eeBadScListFilter
         process.stopTreeMaker.varsBool.append(cms.InputTag("eeBadScListFilter"))
      elif flist.find("csc2015") != -1:
         process.CSCTightHaloListFilter = process.EventListFilter.clone(inputFileList=flist)
         process.trig_filter_seq += process.CSCTightHaloListFilter
         process.stopTreeMaker.varsBool.append(cms.InputTag("CSCTightHaloListFilter"))
      elif flist.find("badResolutionTrack") !=-1:
         process.badResolutionTrackListFilter = process.EventListFilter.clone(inputFileList=flist)
         process.trig_filter_seq += process.badResolutionTrackListFilter
         process.stopTreeMaker.varsBool.append(cms.InputTag("badResolutionTrackListFilter"))
      elif flist.find("muonBadTrack") != -1:
         process.muonBadTrackListFilter = process.EventListFilter.clone(inputFileList=flist)
         process.trig_filter_seq += process.muonBadTrackListFilter
         process.stopTreeMaker.varsBool.append(cms.InputTag("muonBadTrackListFilter"))
      else:
         print "Do NOT support externalFilterList with name : ", flist

if options.selSMSpts == True:
   process.stopTreeMaker.vectorString.extend([cms.InputTag("smsModelFilter:fileNameStr"), cms.InputTag("smsModelFilter:smsModelStr")])
   process.stopTreeMaker.varsDouble.extend([cms.InputTag("smsModelFilter:smsMotherMass"), cms.InputTag("smsModelFilter:smsDaughterMass")])

process.ak4Stop_Path = cms.Path(
                                   process.comb_seq * 
                                   process.printDecayPythia8 * process.prodGenInfo * 
                                   process.prodMuonsNoIso * process.prodElectronsNoIso * process.prodIsoTrks *  
                                   process.prodJets * process.prodMET * process.prodEventInfo * process.trig_filter_seq * 
                                   process.type3topTagger *
                                   process.stopTreeMaker
)

if options.doTopTagger == False:
   process.ak4Stop_Path.remove(process.type3topTagger)

if options.mcInfo == False:
   process.ak4Stop_Path.remove(process.prodGenInfo)
   process.ak4Stop_Path.remove(process.printDecayPythia8)

if options.selSMSpts == True:
   process.ak4Stop_Path.replace(process.hltFilter, process.hltFilter*process.smsModelFilter)

if options.specialFix == "JEC":
   if options.cmsswVersion == "80X":

      process.comb_seq.replace(process.weightProducer, process.fix80XJEC*process.weightProducer)
   
      process.updatedPatJetsUpdatedJECPt10 = process.selectedPatJetsRA2.clone()
      process.updatedPatJetsUpdatedJECPt10.jetSrc = cms.InputTag("updatedPatJetsUpdatedJEC")
      process.updatedPatJetsUpdatedJECPt10.pfJetCut = cms.string('pt >= 10')
   
      process.prodJets.jetSrc = cms.InputTag('updatedPatJetsUpdatedJECPt10')
      if options.fastsim == True:
         process.prodJets.jetOtherSrc = cms.InputTag('updatedPatJetsUpdatedJECPt10')
      
      process.ak4patJetsPFchsPt10.jetSrc = cms.InputTag('updatedPatJetsUpdatedJEC')
      process.ak4patJetsPFchsPt30.jetSrc = cms.InputTag('updatedPatJetsUpdatedJEC')
      process.ak4patJetsPFchsPt50Eta25.jetSrc = cms.InputTag('updatedPatJetsUpdatedJEC')
      process.prodJetIDEventFilter.JetSource = cms.InputTag("updatedPatJetsUpdatedJEC")
      process.ak4stopJetsPFchsPt30.jetSrc = cms.InputTag("updatedPatJetsUpdatedJEC")
      process.ak4stopJetsPFchsPt50Eta24.jetSrc = cms.InputTag("updatedPatJetsUpdatedJEC")
   
      process.stopJetsPFchsPt30.jetSrc = cms.InputTag("updatedPatJetsUpdatedJEC")
      process.stopJetsPFchsPt30Eta24.jetSrc = cms.InputTag("updatedPatJetsUpdatedJEC")
      process.stopJetsPFchsPt50Eta24.jetSrc = cms.InputTag("updatedPatJetsUpdatedJEC")
      process.stopJetsPFchsPt70Eta24.jetSrc = cms.InputTag("updatedPatJetsUpdatedJEC")
      process.stopJetsPFchsPt70eta2p5.jetSrc = cms.InputTag("updatedPatJetsUpdatedJEC")
   
      process.ak4jetMHTDPhiForSkimsStop.MHTSource = cms.InputTag("slimmedMETs", "", process.name_())
      process.jetsMETDPhiFilter.metSrc = cms.InputTag("slimmedMETs", "", process.name_())
      process.metPFchsFilter.MHTSource = cms.InputTag("slimmedMETs", "", process.name_())
   
      process.met175PFchsFilter.MHTSource = cms.InputTag("slimmedMETs", "", process.name_())
      process.met200PFchsFilter.MHTSource = cms.InputTag("slimmedMETs", "", process.name_())
   
      process.prodElectrons.metSource = cms.InputTag("slimmedMETs", "", process.name_())
      process.prodElectronsNoIso.metSource = cms.InputTag("slimmedMETs", "", process.name_())
   
      process.prodIsoTrks.metSrc = cms.InputTag("slimmedMETs", "", process.name_())
      process.prodMET.metSrc = cms.InputTag("slimmedMETs", "", process.name_())
   
      process.prodMuons.metSource = cms.InputTag("slimmedMETs", "", process.name_())
      process.prodMuonsNoIso.metSource = cms.InputTag("slimmedMETs", "", process.name_())
   
      process.type3topTagger.metSrc = cms.InputTag("slimmedMETs", "", process.name_())
   
      process.updatedPatJetsUpdatedJECPt30 = process.selectedPatJetsRA2.clone()
      process.updatedPatJetsUpdatedJECPt30.jetSrc = cms.InputTag("updatedPatJetsUpdatedJEC")
      process.updatedPatJetsUpdatedJECPt30.pfJetCut = cms.string('pt >= 30')
   
      process.mt2PFchs.JetTag = cms.InputTag("updatedPatJetsUpdatedJECPt30")
      process.mt2PFchs.METTag = cms.InputTag("slimmedMETs", "", process.name_())

###-- Dump config ------------------------------------------------------------
if options.debug:
   file = open('allDump_cfg.py','w')
   file.write(str(process.dumpPython()))
   file.close()
