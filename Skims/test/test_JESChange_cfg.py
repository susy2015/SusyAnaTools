import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

#===================== Message Logger =============================
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(
            limit = cms.untracked.int32(0),
            reportEvery = cms.untracked.int32(10000)
)
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
process.MessageLogger.cerr.FwkReport.reportEvery = 1


# Check for ny duplicates
#process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

runningOnMC = True
#runningOnMC = False
#process.GlobalTag.globaltag = "START53_V7F::All" # original GT used for skims, replace by new
process.GlobalTag.globaltag = "CheckGTonTwiki" 
if runningOnMC == False:
    #process.GlobalTag.globaltag = "GR_P_V41_AN2::All" # original GT used for skims, replace by new
    process.GlobalTag.globaltag = "CheckGTonTwiki"

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
     '/store/user/lpcsusyhad/53X_ntuples/kasmi/TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2_NOCUTS_12Oct2012V3_corrTag/b9d339f81100b66394e7e5c0a998fe80/susypat_2129_1_XRW.root'
     ,'/store/user/lpcsusyhad/53X_ntuples/kasmi/TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2_NOCUTS_12Oct2012V3_corrTag/b9d339f81100b66394e7e5c0a998fe80/susypat_1198_1_qcL.root'
     ,'/store/user/lpcsusyhad/53X_ntuples/kasmi/TTJets_SemiLeptMGDecays_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v2_NOCUTS_12Oct2012V3_corrTag/b9d339f81100b66394e7e5c0a998fe80/susypat_1199_1_LGz.root'
     )
)
if runningOnMC == False:
    process.source.fileNames = cms.untracked.vstring(
        '/store/user/lpcsusyhad/53X_ntuples/HTMHT_Run2012C_PromptReco_v2_lpc1/seema/HTMHT/HTMHT_Run2012C-PromptReco-v2_NoHepTopTagger_NOCUTS_HLTPFHTInc_12Oct2012V3/993e6ea01263feda15a347a7daafedb6/susypat_999_1_thS.root'
        ,'/store/user/lpcsusyhad/53X_ntuples/HTMHT_Run2012C_PromptReco_v2_lpc1/seema/HTMHT/HTMHT_Run2012C-PromptReco-v2_NoHepTopTagger_NOCUTS_HLTPFHTInc_12Oct2012V3/993e6ea01263feda15a347a7daafedb6/susypat_99_1_dz4.root'
        ,'/store/user/lpcsusyhad/53X_ntuples/HTMHT_Run2012C_PromptReco_v2_lpc1/seema/HTMHT/HTMHT_Run2012C-PromptReco-v2_NoHepTopTagger_NOCUTS_HLTPFHTInc_12Oct2012V3/993e6ea01263feda15a347a7daafedb6/susypat_9_1_8nC.root'
        )


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5) )

process.source.skipEvents = cms.untracked.uint32(0)  

# create CHS jets and met collections with new jEC
process.load("SandBox.Skims.jesChange_cfi")
process.newJetsMET.Debug = True
if runningOnMC == True :
    process.newJetsMET.JECLevel = cms.string('ak5PFchsL1FastL2L3')
    process.patMETPhiCorr.parameter = process.pfMEtSysShiftCorrParameters_2012runABCvsNvtx_mc
else :
    process.newJetsMET.JECLevel = cms.string('ak5PFchsL1FastL2L3Residual')
    process.patMETPhiCorr.parameter = process.pfMEtSysShiftCorrParameters_2012runABCvsNvtx_data
    
process.ppf = cms.Path(process.newra2PFchsJets)

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('aa.root'),
                               # save only events passing the full path
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('ppf') ),
                               # save PAT Layer 1 output; you need a '*' to
                               # unpack the list of commands 'patEventContent'
                               outputCommands = cms.untracked.vstring('keep *' )
                               )
process.outpath = cms.EndPath(process.out)
