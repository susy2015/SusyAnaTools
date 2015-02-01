#
#  SUSY-PAT configuration file adapted for RA2 workflow
#
#  PAT configuration for the SUSY group - 41X series
#  More information here:
#  https://twiki.cern.ch/twiki/bin/view/CMS/SusyPatLayer1DefV10
#

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

#runningOnMC = True 
runningOnMC = False

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

process.GlobalTag.globaltag = "START42_V13::All"
if runningOnMC == False:
    process.GlobalTag.globaltag = "GR_R_42_V19::All"

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring('/store/mc/Summer11/QCD_Pt-600to800_TuneZ2_7TeV_pythia6/AODSIM/PU_S3_START42_V11-v2/0000/FEF7A5D8-EE7D-E011-B566-0024E8768439.root')
)
if runningOnMC == False:
    process.source = cms.Source("PoolSource",
       fileNames = cms.untracked.vstring('/store/data/Run2011A/HT/AOD/May10ReReco-v1/0000/FEF00BB3-617C-E011-8961-002618FDA21D.root')
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
                      process.ra2PostCleaning *
                      process.ra2EcalTPFilter 
                      *process.vetoBEFilterHTRun2011AMay10ReRecov1
                      # choose among the following as on June02,2011
                      #*process.vetoBEFilterHTRun2011AMay10ReRecov1
                      #*process.vetoBEFilterHTRun2011Av1
                      #*process.vetoBEFilterHTRun2011Av2
                      #*process.vetoBEFilterHTRun2011Av4
                      #*process.vetoBEFilterQCD_Pt15to3000TuneZ2Flat
                      #*process.vetoBEFilterQCDTuneD6THT1000
                      #*process.vetoBEFilterQCDTuneD6THT100To250
                      #*process.vetoBEFilterQCDTuneD6THT250To500
                      #*process.vetoBEFilterQCDTuneD6THT500To1000
                      #*process.vetoBEFilterQCDTuneD6THT50To100
                      )

process.ppf = cms.Path(
        process.cleanpatseq
      )

#-- Output module configuration ---------------------------------------
process.out.fileName = cms.untracked.string('susypat.root')

process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('ppf') )
process.outpath = cms.EndPath( process.out )

###-- Dump config ------------------------------------------------------------
##file = open('SusyPAT_RA2414_cfg.py','w')
##file.write(str(process.dumpPython()))
##file.close()

