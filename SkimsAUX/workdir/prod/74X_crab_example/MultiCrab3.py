#!/usr/bin/env python
# encoding: utf-8

# File        : MultiCrab3.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Apr 01
#
# Description :


import copy, os, time, sys

from CRABAPI.RawCommand import crabCommand
from WMCore.Configuration import saveConfigurationFile
from crab3Config import config as config

workArea = 'crabProdv3p0'
outDir =  '/store/group/lpcsusyhad/Spring15_74X_Nov_2015_Ntp_v3X'
Pubname = 'Spring15_74X_Nov_2015_Ntp_v3p0'
json_25ns = 'Cert_246908-259891_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
# Use the common keyword to select the samples you'd like to submit
# ALL: all of them; NONE: none of them; TEST: test printing out the crab3 config
# TTJets, WJetsToLNu, ZJetsToNuNu, DYJetsToLL, QCD, TTW, TTZ, ST_tW, SMS, HTMHT, SingleMuon, SingleElectron, DoubleMuon, DoubleEG
# Can be any of the combinations
selSubmitKey = 'TTJets'
#selSubmitKey = 'NONE'
doAutoMonitor = False

## Format: keyword : IsData, fulldatasetname, unitperjob
jobslist = {
    # TTbar
    'TTJets'                                 : [False, '/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'TTJets_SingleLeptFromT'                 : [False, '/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'TTJets_SingleLeptFromT_ext1'            : [False, '/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2_ext1-v1/MINIAODSIM', 5],
    'TTJets_SingleLeptFromTbar'              : [False, '/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'TTJets_SingleLeptFromTbar_ext1'         : [False, '/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2_ext1-v1/MINIAODSIM', 5],
    'TTJets_HT-600to800'                     : [False, '/TTJets_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'TTJets_HT-800to1200'                    : [False, '/TTJets_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'TTJets_HT-1200to2500'                   : [False, '/TTJets_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'TTJets_HT-2500toInf'                    : [False, '/TTJets_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'TTJets_DiLept'                          : [False, '/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'TTJets_DiLept_ext1'                     : [False, '/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2_ext1-v1/MINIAODSIM', 5],
    # WJets,
    'WJetsToLNu_HT-100To200'                 : [False, '/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'WJetsToLNu_HT-200To400'                 : [False, '/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'WJetsToLNu_HT-400To600'                 : [False, '/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'WJetsToLNu_HT-600To800'                 : [False, '/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'WJetsToLNu_HT-800To1200'                : [False, '/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'WJetsToLNu_HT-1200To2500'               : [False, '/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'WJetsToLNu_HT-2500ToInf'                : [False, '/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'WJetsToLNu_HT-600ToInf'                 : [False, '/WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    # Zinv,
    'ZJetsToNuNu_HT-100To200'                : [False, '/ZJetsToNuNu_HT-100To200_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'ZJetsToNuNu_HT-200To400'                : [False, '/ZJetsToNuNu_HT-200To400_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'ZJetsToNuNu_HT-400To600'                : [False, '/ZJetsToNuNu_HT-400To600_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'ZJetsToNuNu_HT-600ToInf'                : [False, '/ZJetsToNuNu_HT-600ToInf_13TeV-madgraph/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM', 5],
    # DYJets,
    'DYJetsToLL_M-50_HT-100to200'            : [False, '/DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'DYJetsToLL_M-50_HT-200to400'            : [False, '/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'DYJetsToLL_M-50_HT-400to600'            : [False, '/DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM', 5],
    'DYJetsToLL_M-50_HT-600toInf'            : [False, '/DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    # QCD,
    'QCD_HT100to200'                         : [False, '/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_HT200to300'                         : [False, '/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_HT300to500'                         : [False, '/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_HT500to700'                         : [False, '/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_HT700to1000'                        : [False, '/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_HT1000to1500'                       : [False, '/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_HT1500to2000'                       : [False, '/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_HT2000toInf'                        : [False, '/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_10to15'                          : [False, '/QCD_Pt_10to15_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_15to30'                          : [False, '/QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_30to50'                          : [False, '/QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_50to80'                          : [False, '/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_80to120'                         : [False, '/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_120to170'                        : [False, '/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_170to300'                        : [False, '/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_300to470'                        : [False, '/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_470to600'                        : [False, '/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_600to800'                        : [False, '/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_800to1000'                       : [False, '/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_1000to1400'                      : [False, '/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_1400to1800'                      : [False, '/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_1800to2400'                      : [False, '/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_2400to3200'                      : [False, '/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'QCD_Pt_3200toInf'                       : [False, '/QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    # OTHERS,
    'TTWJetsToQQ'                            : [False, '/TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'TTWJetsToLNu'                           : [False, '/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'TTZToQQ'                                : [False, '/TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'TTZToLLNuNu'                            : [False, '/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM', 5],
    'ST_tW_top_5f_inclusiveDecays'           : [False, '/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v2/MINIAODSIM', 5],
    'ST_tW_antitop_5f_inclusiveDecays'       : [False, '/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    # Signals
    'SMS-T1tttt_mGluino-1200_mLSP-800'       : [False, '/SMS-T1tttt_mGluino-1200_mLSP-800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'SMS-T1tttt_mGluino-1500_mLSP-100'       : [False, '/SMS-T1tttt_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'SMS-T1bbbb_mGluino-1000_mLSP-900'       : [False, '/SMS-T1bbbb_mGluino-1000_mLSP-900_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'SMS-T1bbbb_mGluino-1500_mLSP-100'       : [False, '/SMS-T1bbbb_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'SMS-T2tt_mStop-500_mLSP-325'            : [False, '/SMS-T2tt_mStop-500_mLSP-325_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    'SMS-T2tt_mStop-850_mLSP-100'            : [False, '/SMS-T2tt_mStop-850_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM', 5],
    # Data
#    'HTMHT-Run2015B-05Oct2015'               : [True, '/HTMHT/Run2015B-05Oct2015-v1/MINIAOD', 100],
    'HTMHT-Run2015C-25ns-05Oct2015'          : [True, '/HTMHT/Run2015C_25ns-05Oct2015-v1/MINIAOD', 100],
    'HTMHT-Run2015D-05Oct2015'               : [True, '/HTMHT/Run2015D-05Oct2015-v1/MINIAOD', 100],
    'HTMHT-Run2015D-PromptReco'              : [True, '/HTMHT/Run2015D-PromptReco-v4/MINIAOD', 100],

#    'MET-Run2015B-05Oct2015'                 : [True, '/MET/Run2015B-05Oct2015-v1/MINIAOD', 100],
    'MET-Run2015C-25ns-05Oct2015'            : [True, '/MET/Run2015C_25ns-05Oct2015-v1/MINIAOD', 100],
    'MET-Run2015D-05Oct2015'                 : [True, '/MET/Run2015D-05Oct2015-v1/MINIAOD', 100],
    'MET-Run2015D-PromptReco'                : [True, '/MET/Run2015D-PromptReco-v4/MINIAOD', 100],

#    'JetHT-Run2015B-05Oct2015'               : [True, '/JetHT/Run2015B-05Oct2015-v1/MINIAOD', 100],
    'JetHT-Run2015C-25ns-05Oct2015'          : [True, '/JetHT/Run2015C_25ns-05Oct2015-v1/MINIAOD', 100],
    'JetHT-Run2015D-05Oct2015'               : [True, '/JetHT/Run2015D-05Oct2015-v1/MINIAOD', 100],
    'JetHT-Run2015D-PromptReco'              : [True, '/JetHT/Run2015D-PromptReco-v4/MINIAOD', 100],

#    'SingleMuon-Run2015B-05Oct2015'          : [True, '/SingleMuon/Run2015B-05Oct2015-v1/MINIAOD', 100],
    'SingleMuon-Run2015C-25ns-05Oct2015'     : [True, '/SingleMuon/Run2015C_25ns-05Oct2015-v1/MINIAOD', 100],
    'SingleMuon-Run2015D-05Oct2015'          : [True, '/SingleMuon/Run2015D-05Oct2015-v1/MINIAOD', 100],
    'SingleMuon-Run2015D-PromptReco'         : [True, '/SingleMuon/Run2015D-PromptReco-v4/MINIAOD', 100],

#    'SingleElectron-Run2015B-05Oct2015'      : [True, '/SingleElectron/Run2015B-05Oct2015-v1/MINIAOD', 100],
    'SingleElectron-Run2015C-25ns-05Oct2015' : [True, '/SingleElectron/Run2015C_25ns-05Oct2015-v1/MINIAOD', 100],
    'SingleElectron-Run2015D-05Oct2015'      : [True, '/SingleElectron/Run2015D-05Oct2015-v1/MINIAOD', 100],
    'SingleElectron-Run2015D-PromptReco'     : [True, '/SingleElectron/Run2015D-PromptReco-v4/MINIAOD', 100],

#    'DoubleMuon-Run2015B-05Oct2015'          : [True, '/DoubleMuon/Run2015B-05Oct2015-v1/MINIAOD', 100],
    'DoubleMuon-Run2015C-25ns-05Oct2015'     : [True, '/DoubleMuon/Run2015C_25ns-05Oct2015-v1/MINIAOD', 100],
    'DoubleMuon-Run2015D-05Oct2015'          : [True, '/DoubleMuon/Run2015D-05Oct2015-v1/MINIAOD', 100],
    'DoubleMuon-Run2015D-PromptReco'         : [True, '/DoubleMuon/Run2015D-PromptReco-v4/MINIAOD', 100],

#    'DoubleEG-Run2015B-05Oct2015'            : [True, '/DoubleEG/Run2015B-05Oct2015-v1/MINIAOD', 100],
    'DoubleEG-Run2015C-25ns-05Oct2015'       : [True, '/DoubleEG/Run2015C_25ns-05Oct2015-v1/MINIAOD', 100],
    'DoubleEG-Run2015D-05Oct2015'            : [True, '/DoubleEG/Run2015D-05Oct2015-v1/MINIAOD', 100],
    'DoubleEG-Run2015D-PromptReco'           : [True, '/DoubleEG/Run2015D-PromptReco-v4/MINIAOD', 100],
}
tasklist = {}

def MonitoringJobs(tasklist):
    while True:
        for request, name in tasklist.items():
            dirname = './%s/crab_%s' % (workArea, name)
            fulldir = os.path.abspath(dirname)
            try:
                results = crabCommand('status', dir=fulldir)
                print "For task", request, "the job states are", results['jobsPerStatus']
            except:
                pass
            time.sleep(60)

def CreateMonitorList(tasklist):
    monList = open("monList_"+workArea+".txt", 'w')
    for key in sorted(tasklist):
       dirname = './%s/crab_%s' % (workArea, tasklist[key])
       fulldir = os.path.abspath(dirname)
       monList.write("crab status "+fulldir+"\n")
    monList.close()


if __name__ == "__main__":

    if not os.path.exists(workArea):
       os.makedirs(workArea)
       os.makedirs(workArea+"/test")

    doAll = False
    doTest = False

    allSelKeys = selSubmitKey.split()

    if selSubmitKey.find('NONE') != -1:
       print "Nothing to be done!" 
       sys.exit()
    if selSubmitKey.find('ALL') != -1:
       doAll = True
    if selSubmitKey.find('TEST') != -1:
       doTest = True 

    for key, value in jobslist.items():

        doThis = doAll

        if not doAll:
           for selKey in allSelKeys:
              if key.find(selKey) != -1:
                 doThis = True
                 break;

        if not doThis:
           continue

        tempconfig = copy.deepcopy(config)
        tempconfig.General.requestName = key
        tempconfig.General.workArea = workArea
        tempconfig.Data.outputDatasetTag = Pubname + "_" + key
        tempconfig.Data.outLFNDirBase = outDir

        if len(value) <3:
            print "Not enough argument for %s" % key
            raise  AssertionError()
        if value[0]: # Data
            if key.find('Run2015C') != -1:
               tempconfig.JobType.pyCfgParams = ['mcInfo=0', 'GlobalTag=74X_dataRun2_v4'] 
               tempconfig.JobType.inputFiles = [json_25ns]
               tempconfig.Data.splitting = 'LumiBased'
               tempconfig.Data.lumiMask = json_25ns
            elif key.find('Run2015D-05Oct2015') != -1:
               tempconfig.JobType.pyCfgParams = ['mcInfo=0', 'GlobalTag=74X_dataRun2_reMiniAOD_v0']
               tempconfig.JobType.inputFiles = [json_25ns]
               tempconfig.Data.splitting = 'LumiBased'
               tempconfig.Data.lumiMask = json_25ns
            elif key.find('Run2015D-PromptReco') != -1:
               tempconfig.JobType.pyCfgParams = ['mcInfo=0', 'GlobalTag=74X_dataRun2_Prompt_v4']
               tempconfig.JobType.inputFiles = [json_25ns]
               tempconfig.Data.splitting = 'LumiBased'
               tempconfig.Data.lumiMask = json_25ns
            else:
               pass
        else:
           tempconfig.JobType.pyCfgParams = ['mcInfo=1', 'GlobalTag=74X_mcRun2_asymptotic_v2']
           tempconfig.Data.splitting = 'FileBased'

        tempconfig.Data.inputDataset = value[1].strip()
        tempconfig.Data.unitsPerJob = value[2]

        if value[0] and len(value) > 3:
            tempconfig.Data.lumiMask = value[3]

        # Submitting jobs
        if doTest:
           saveConfigurationFile(tempconfig, workArea+"/test/"+key+"_test_cfg.py")
           tasklist["crab_"+key] = key
        else:
           results = crabCommand('submit', config = tempconfig)
           tasklist[results['uniquerequestname']] = key
           del tempconfig
    CreateMonitorList(tasklist)
    if not doTest and doAutoMonitor:     
       MonitoringJobs(tasklist)
