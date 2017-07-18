import FWCore.ParameterSet.Config as cms

import FWCore.ParameterSet.VarParsing as VarParsing
### parsing job options 
import sys

options = VarParsing.VarParsing()

options.register('inputScript','',VarParsing.VarParsing.multiplicity.singleton,VarParsing.VarParsing.varType.string,"input Script")
options.register('outputFile','output',VarParsing.VarParsing.multiplicity.singleton,VarParsing.VarParsing.varType.string,"output File (w/o .root)")
options.register('maxEvents',-1,VarParsing.VarParsing.multiplicity.singleton,VarParsing.VarParsing.varType.int,"maximum events")
options.register('skipEvents', 0, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "skip N events")
options.register('job', 0, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "job number")
options.register('nJobs', 1, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "total jobs")
options.register('release','8_0_1', VarParsing.VarParsing.multiplicity.singleton,VarParsing.VarParsing.varType.string,"release number (w/o CMSSW)")

print("Using release "+options.release)


if hasattr(sys, "argv"):
    options.parseArguments()


process = cms.Process("DNNFiller")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.options = cms.untracked.PSet(
   allowUnscheduled = cms.untracked.bool(True),  
   wantSummary=cms.untracked.bool(False)
)

from PhysicsTools.PatAlgos.patInputFiles_cff import filesRelValTTbarPileUpMINIAODSIM

process.source = cms.Source('PoolSource',
    fileNames=cms.untracked.vstring (['/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/10000/7CE5EA6A-F132-E611-9E20-008CFA1660A8.root',]),
)

if options.inputScript != '' and options.inputScript != 'DeepNTuples.DeepNtuplizer.samples.TEST':
    process.load(options.inputScript)

numberOfFiles = len(process.source.fileNames)
numberOfJobs = options.nJobs
jobNumber = options.job


process.source.fileNames = process.source.fileNames[jobNumber:numberOfFiles:numberOfJobs]
if options.nJobs > 1:
    print ("running over these files:")
    print (process.source.fileNames)
#process.source.fileNames = ['file:/uscms/home/verzetti/nobackup/CMSSW_8_0_25/src/DeepNTuples/copy_numEvent100.root']

process.source.skipEvents = cms.untracked.uint32(options.skipEvents)
process.maxEvents  = cms.untracked.PSet( 
    input = cms.untracked.int32 (options.maxEvents) 
)


if int(options.release.replace("_",""))>=840 :
 bTagInfos = [
        'pfImpactParameterTagInfos',
        'pfInclusiveSecondaryVertexFinderTagInfos',
        'pfDeepCSVTagInfos',
 ]
else :
 bTagInfos = [
        'pfImpactParameterTagInfos',
        'pfInclusiveSecondaryVertexFinderTagInfos',
        'deepNNTagInfos',
 ]


if int(options.release.replace("_",""))>=840 :
 bTagDiscriminators = [
     'softPFMuonBJetTags',
     'softPFElectronBJetTags',
         'pfJetBProbabilityBJetTags',
         'pfJetProbabilityBJetTags',
     'pfCombinedInclusiveSecondaryVertexV2BJetTags',
         'pfDeepCSVJetTags:probudsg', #to be fixed with new names
         'pfDeepCSVJetTags:probb',
         'pfDeepCSVJetTags:probc',
         'pfDeepCSVJetTags:probbb',
         'pfDeepCSVJetTags:probcc',
 ]
else :
  bTagDiscriminators = [
     'softPFMuonBJetTags',
     'softPFElectronBJetTags',
         'pfJetBProbabilityBJetTags',
         'pfJetProbabilityBJetTags',
     'pfCombinedInclusiveSecondaryVertexV2BJetTags',
         'deepFlavourJetTags:probudsg', #to be fixed with new names
         'deepFlavourJetTags:probb',
         'deepFlavourJetTags:probc',
         'deepFlavourJetTags:probbb',
         'deepFlavourJetTags:probcc',
 ]



jetCorrectionsAK4 = ('AK4PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None')

from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
updateJetCollection(
        process,
        labelName = "DeepFlavour",
        jetSource = cms.InputTag('slimmedJets'),#'ak4Jets'
        jetCorrections = jetCorrectionsAK4,
        pfCandidates = cms.InputTag('packedPFCandidates'),
        pvSource = cms.InputTag("offlineSlimmedPrimaryVertices"),
        svSource = cms.InputTag('slimmedSecondaryVertices'),
        muSource = cms.InputTag('slimmedMuons'),
        elSource = cms.InputTag('slimmedElectrons'),
        btagInfos = bTagInfos,
        btagDiscriminators = bTagDiscriminators,
        explicitJTA = False
)

 
process.updatedPatJetsDeepFlavour.userData.userFloats.src += ['QGTagger:qgLikelihood','QGTagger:ptD', 'QGTagger:axis1', 'QGTagger:axis2']
process.updatedPatJetsDeepFlavour.userData.userInts.src += ['QGTagger:mult']

if hasattr(process,'updatedPatJetsTransientCorrectedDeepFlavour'):
	process.updatedPatJetsTransientCorrectedDeepFlavour.addTagInfos = cms.bool(True) 
	process.updatedPatJetsTransientCorrectedDeepFlavour.addBTagInfo = cms.bool(True)
else:
	raise ValueError('I could not find updatedPatJetsTransientCorrectedDeepFlavour to embed the tagInfos, please check the cfg')


# QGLikelihood
process.load("DeepNTuples.DeepNtuplizer.QGLikelihood_cfi")
process.es_prefer_jec = cms.ESPrefer("PoolDBESSource", "QGPoolDBESSource")
process.load('RecoJets.JetProducers.QGTagger_cfi')
process.QGTagger.srcJets   = cms.InputTag("slimmedJets")#selectedUpdatedPatJetsDeepFlavour")
process.QGTagger.jetsLabel = cms.string('QGL_AK4PFchs')


outFileName = options.outputFile + '_' + str(options.job) +  '.root'
print ('Using output file ' + outFileName)

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string(outFileName))

###############################################################################################################################

process.load("SusyAnaTools.SignalScan.genDecayStringMakerPythia8_cfi")
process.printDecayPythia8.src = cms.InputTag("prunedGenParticles")
process.printDecayPythia8.keyDecayStrs = cms.vstring("t", "tbar", "~chi_1+", "~chi_1-")
process.printDecayPythia8.printDecay = cms.untracked.bool(False)

###############################################################################################################################

process.load("SusyAnaTools.SkimsAUX.prodGenInfo_cfi")
process.load("SusyAnaTools.SkimsAUX.prodJets_cfi")

###############################################################################################################################

process.load("SusyAnaTools.SkimsAUX.prodJets_cfi")
process.prodJets.bTagKeyString = cms.string('pfCombinedInclusiveSecondaryVertexV2BJetTags')
process.prodJets.jetPBJetTags = cms.string('pfJetBProbabilityBJetTags')
process.prodJets.jetPNegBJetTags= cms.string('pfNegativeOnlyJetBProbabilityBJetTags')
process.prodJets.jetPPosBJetTags= cms.string('pfPositiveOnlyJetBProbabilityBJetTags')
process.prodJets.jetBPBJetTags= cms.string('jetBPBJetTags')
process.prodJets.jetBPNegBJetTags= cms.string('jetBPNegBJetTags')
process.prodJets.jetBPPosBJetTags= cms.string('jetBPPosBJetTags')
process.prodJets.debug = cms.bool(False)
process.prodJets.jetSrc = cms.InputTag('selectedUpdatedPatJetsDeepFlavour')
#process.prodJets.jetOtherSrc = cms.InputTag('selectedUpdatedPatJetsDeepFlavour')

###############################################################################################################################

process.load("SusyAnaTools.StopTreeMaker.stopTreeMaker_cfi")
process.stopTreeMaker.debug = cms.bool(False)
process.stopTreeMaker.TreeName = cms.string("AUX")

process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodJets", "jetsLVec"))
#process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodGenJets", "genjetsLVec"))
process.stopTreeMaker.vectorInt.append(cms.InputTag("prodJets", "recoJetsFlavor"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsJecUnc"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsJecScaleRawToFull"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "qgLikelihood"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "qgPtD"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "qgAxis1"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "qgAxis2"))
process.stopTreeMaker.vectorInt.append(cms.InputTag("prodJets", "qgMult"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetschargedHadronEnergyFraction"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetschargedEmEnergyFraction"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsneutralEmEnergyFraction"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsmuonEnergyFraction"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsHFHadronEnergyFraction"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsHFEMEnergyFraction"));
#process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodGenJets", "genjetsLVec"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsneutralEnergyFraction"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "PhotonEnergyFraction"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "ElectronEnergyFraction"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "ChargedHadronMultiplicity"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "NeutralHadronMultiplicity"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "PhotonMultiplicity"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "ElectronMultiplicity"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "MuonMultiplicity"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVb"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVc"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVl"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVbb"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVcc"))
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVbN"))
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVcN"))
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVlN"))
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVbbN"))
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVccN"))
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVbP"))
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVcP"))
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVlP"))
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVbbP"))
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "DeepCSVccP"))
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","CombinedSvtx"));
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","CombinedSvtxN"));
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","CombinedSvtxP"));
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","Svtx"));
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","SvtxN"));
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","SvtxHP"));
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","SvtxNHP"));
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","SoftM"));
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","SoftMN"));
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","SoftMP"));
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","SoftE"));
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","SoftEN"));
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","SoftEP"));
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","DoubleSV"));
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","cMVA"));
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","cMVAv2"));
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","cMVAv2Neg"));
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets","cMVAv2Pos"));
process.stopTreeMaker.vectorInt.append(cms.InputTag("prodJets","nTracks"));
process.stopTreeMaker.vectorInt.append(cms.InputTag("prodJets","nSVs"));
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "JetProba"))
process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJets:JetProba|JetProba_0")
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "JetProbaN"))
#process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJets:JetProbaN|JetProbaN_0")
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "JetProbaP"))
#process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJets:JetProbaP|JetProbaP_0")
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "JetBprob"))
process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJets:JetProb|JetProb_0")
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "JetBprobN"))
#process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJets:JetProbN|JetProbN_0")
#process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "JetBprobP"))
#process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJets:JetProbP|JetProbP_0")
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsBtag"))
process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJets:recoJetsBtag|recoJetsBtag_0")
process.stopTreeMaker.vectorDouble.append(cms.InputTag("prodJets", "recoJetsCharge"))
process.stopTreeMaker.vectorDoubleNamesInTree.append("prodJets:recoJetsCharge|recoJetsCharge_0")

process.stopTreeMaker.vectorString.append(cms.InputTag("prodGenInfo", "genDecayStrVec"))
process.stopTreeMaker.vectorInt.extend([cms.InputTag("prodGenInfo", "genDecayIdxVec"), cms.InputTag("prodGenInfo", "genDecayPdgIdVec"), cms.InputTag("prodGenInfo", "genDecayMomIdxVec"), cms.InputTag("prodGenInfo", "genDecayMomRefVec"), cms.InputTag("prodGenInfo", "WemuVec"), cms.InputTag("prodGenInfo", "WtauVec"), cms.InputTag("prodGenInfo", "WtauemuVec"), cms.InputTag("prodGenInfo", "WtauprongsVec"), cms.InputTag("prodGenInfo", "WtaunuVec"),  cms.InputTag("prodGenInfo","selPDGid")])
process.stopTreeMaker.vectorIntNamesInTree.extend(["prodGenInfo:WemuVec|W_emuVec", "prodGenInfo:WtauVec|W_tauVec", "prodGenInfo:WtauemuVec|W_tau_emuVec", "prodGenInfo:WtauprongsVec|W_tau_prongsVec", "prodGenInfo:WtaunuVec|W_tau_nuVec"])
process.stopTreeMaker.vectorDouble.extend([cms.InputTag("prodGenInfo", "WemupfActivityVec"), cms.InputTag("prodGenInfo", "WtauemupfActivityVec"), cms.InputTag("prodGenInfo", "WtauprongspfActivityVec")])
process.stopTreeMaker.vectorDoubleNamesInTree.extend(["prodGenInfo:WemupfActivityVec|W_emu_pfActivityVec", "prodGenInfo:WtauemupfActivityVec|W_tau_emu_pfActivityVec", "prodGenInfo:WtauprongspfActivityVec|W_tau_prongs_pfActivityVec"])
process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodGenInfo", "genDecayLVec"))
process.stopTreeMaker.vectorTLorentzVector.append(cms.InputTag("prodGenInfo", "selGenParticle"))

###############################################################################################################################

process.p = cms.Path(process.QGTagger * process.printDecayPythia8 * process.prodGenInfo * process.prodJets * process.stopTreeMaker)
