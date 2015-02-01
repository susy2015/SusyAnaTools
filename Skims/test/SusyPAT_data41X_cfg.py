#
#  SUSY-PAT configuration file adapted for RA2 workflow
#
#  PAT configuration for the SUSY group - 41X series
#  More information here:
#  https://twiki.cern.ch/twiki/bin/view/CMS/SusyPatLayer1DefV10
#

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

runningOnMC = True #False
#runningOnMC = False

#-- Message Logger ------------------------------------------------------------
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
    limit = cms.untracked.int32(10),
    reportEvery = cms.untracked.int32(1)
    )
process.MessageLogger.cerr.FwkReport.reportEvery = 1


#-- Input Source --------------------------------------------------------------
process.maxEvents.input = 100
# Due to problem in production of LM samples: same event number appears multiple times
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

############################# START SUSYPAT specifics ####################################
from PhysicsTools.Configuration.SUSY_pattuple_cff import addDefaultSUSYPAT, getSUSY_pattuple_outputCommands

hltMenu = 'HLT'

theJetColls = ['AK5PF']

jetMetCorr = ['L1FastJet', 'L2Relative', 'L3Absolute']
if runningOnMC == False: jetMetCorr.append('L2L3Residual')

process.GlobalTag.globaltag = "START41_V0::All"
if runningOnMC == False:
    process.GlobalTag.globaltag = "GR_R_41_V0::All"

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring('/store/mc/Spring11/QCD_TuneD6T_HT-500To1000_7TeV-madgraph/AODSIM/PU_S1_START311_V1G1-v1/0022/FEDF12BD-7856-E011-BADE-003048678F6C.root')
)
if runningOnMC == False:
    process.source = cms.Source("PoolSource",
       fileNames = cms.untracked.vstring('/store/data/Run2011A/HT/RECO/PromptReco-v1/000/161/311/1287EA04-D957-E011-80F8-0030487A18F2.root')
    )

# Due to problem in production of LM samples: same event number appears multiple times
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

addDefaultSUSYPAT(process,mcInfo=runningOnMC,HLTMenu=hltMenu,jetMetCorrections=jetMetCorr,mcVersion='',theJetNames=theJetColls, doSusyTopProjection=True)

#SUSY_pattuple_outputCommands = getSUSY_pattuple_outputCommands( process )

# overwrite default output content
from SandBox.Skims.RA2Content_cff import getRA2PATOutput
process.out.outputCommands = getRA2PATOutput(process)
process.out.dropMetaData = cms.untracked.string('DROPPED')
############################## END SUSYPAT specifics ####################################

#-- HLT selection ------------------------------------------------------------
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt
hltSelection=''
#if options.hltSelection:
if hltSelection:
    process.hltFilter = hlt.hltHighLevel.clone(
        HLTPaths = cms.vstring(hltSelection),
        TriggerResultsTag = cms.InputTag("TriggerResults","",hltMenu),
        throw = False
    )
    process.susyPatDefaultSequence.replace(process.eventCountProducer, process.eventCountProducer * process.hltFilter)


#-- check RA2 recipe here ------------------------------------------------------------
process.prefilterCounter        = cms.EDProducer("EventCountProducer")
process.postStdCleaningCounter  = cms.EDProducer("EventCountProducer")

#-- Output module configuration -----------------------------------------------
process.load('SandBox.Skims.RA2Objects_cff')
process.load('SandBox.Skims.RA2Selection_cff')

process.cleanpatseq = cms.Sequence(
                      process.prefilterCounter *
                      process.ra2StdCleaning *
                      process.postStdCleaningCounter *
                      process.susyPatDefaultSequence *
                      process.ra2Objects *
                      process.ra2FullPFSelectionNoMHT *
                      process.ra2PostCleaning
                      )

#please include
#process.load('PhysicsTools.EcalAnomalousEventFilter.BEFilter_XXX.py')
#process.load('PhysicsTools.EcalAnomalousEventFilter.TPFilter_XXX.py')
#and include the list of events to be rejceted corresponding to your
#dataset in path.

process.ppf = cms.Path(
        process.cleanpatseq
      )

#-- Output module configuration ---------------------------------------
process.out.fileName = cms.untracked.string('susypat.root')

process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('ppf') )
#process.out.outputCommands = cms.untracked.vstring('keep *')
process.outpath = cms.EndPath( process.out )

###-- Dump config ------------------------------------------------------------
##file = open('SusyPAT_RA2414_cfg.py','w')
##file.write(str(process.dumpPython()))
##file.close()

