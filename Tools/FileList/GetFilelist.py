#!/usr/bin/env python
# encoding: utf-8

# File        : GetFilelist.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Jun 11
#
# Description :

from __future__ import print_function
import glob
import fnmatch
import os
import re

hostname = os.uname()[1]
onLPC = False
if hostname == 'macbook':
    NtupleDir = "/Users/benwu/Data/Dataset/PHY14_Ntuple/PHYS14_720_Mar14_2014_v2/"
elif hostname == 'nbay04.fnal.gov':
    NtupleDir = "/data/nbay04/c/benwu/PHY14_Ntuple/PHYS14_720_Mar14_2014_v2/"
elif re.match(r'cmslpc\d+\.fnal\.gov', hostname) != None:
    #NtupleDir = "/eos/uscms/store/user/lpcsusyhad/Spring15_74X_Oct_2015_Ntp_v2X"
    NtupleDir = "/eos/uscms/store/user/lpcsusyhad/Stop_production/SoftBjet_PhotonNtuples/"
    onLPC = True

NameDir = {
    "TTbar"                                      : "PU20bx25_TTJets_MSDecaysCKM_madgraph-tauola",
    "WJetsToLNu_HT_600toInf"                     : "PU20bx25_WJetsToLNu_HT-600toInf_madgraph-tauola",
    "WJetsToLNu_HT_400to600"                     : "PU20bx25_WJetsToLNu_HT-400to600_madgraph-tauola" ,
    "WJetsToLNu_HT_200to400"                     : "PU20bx25_WJetsToLNu_HT-200to400_madgraph-tauola" ,
    "WJetsToLNu_HT_100to200"                     : "PU20bx25_WJetsToLNu_HT-100to200_madgraph-tauola",
    "ZJetsToNuNu_HT_600toInf"                    : "PU20bx25_ZJetsToNuNu_HT-600toInf_madgraph-tauola",
    "ZJetsToNuNu_HT_400to600"                    : "PU20bx25_ZJetsToNuNu_HT-400to600_madgraph-tauola",
    "ZJetsToNuNu_HT_200to400"                    : "PU20bx25_ZJetsToNuNu_HT-200to400_madgraph-tauola",
    "ZJetsToNuNu_HT_100to200"                    : "PU20bx25_ZJetsToNuNu_HT-100to200_madgraph-tauola",
    "DYJetsToLL_HT_600toInf"                     : "DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola",
    "DYJetsToLL_HT_400to600"                     : "DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola",
    "DYJetsToLL_HT_200to400"                     : "DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola",
    "DYJetsToLL_HT_100to200"                     : "DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola",
    #"QCD_HT_1000toInf",
    #"QCD_HT_500to1000",
    #"QCD_HT_250to500",
    "T_tW"                                       : "PU20bx25_T_tW-channel-DR_powheg-tauola",
    "Tbar_tW"                                    : "PU20bx25_Tbar_tW-channel-DR_powheg-tauola",
    "TTZ"                                        : "PU20bx25_TTZJets_madgraph-tauola",
    "Signal_T1tttt_mGluino1200_mLSP800"          : "PU20bx25_T1tttt_2J_mGl-1200_mLSP-800_madgraph-tauola",
    "Signal_T1tttt_mGluino1500_mLSP100"          : "PU20bx25_T1tttt_2J_mGl-1500_mLSP-100_madgraph-tauola",
    "Signal_T5tttt_mGluino1300_mStop300_mChi280" : "T5ttttDeg_mGo1300_mStop300_mChi280_4bodydec_v2",
    "Signal_T5tttt_mGluino1300_mStop300_mCh285"  : "T5ttttDeg_mGo1300_mStop300_mCh285_mChi280_23bodydec_v2",
    "Signal_T1bbbb_mGluino1000_mLSP900"          : "PU20bx25_T1bbbb_2J_mGl-1000_mLSP-900_madgraph-tauola",
    "Signal_T1bbbb_mGluino1500_mLSP100"          : "PU20bx25_T1bbbb_2J_mGl-1500_mLSP-100_madgraph-tauola",
    "Signal_T2tt_mStop425_mLSP325"               : "PU20bx25_T2tt_2J_mStop-425_mLSP-325_madgraph-tauola",
    "Signal_T2tt_mStop500_mLSP325"               : "PU20bx25_T2tt_2J_mStop-500_mLSP-325_madgraph-tauola",
    "Signal_T2tt_mStop650_mLSP325"               : "PU20bx25_T2tt_2J_mStop-650_mLSP-325_madgraph-tauola",
    "Signal_T2tt_mStop850_mLSP100"               : "PU20bx25_T2tt_2J_mStop-850_mLSP-100_madgraph-tauola",
    "Signal_T2bb_mSbottom600_mLSP580"            : "PU20bx25_T2bb_2J_mStop-600_mLSP-580_madgraph-tauola",
    "Signal_T2bb_mSbottom900_mLSP100"            : "PU20bx25_T2bb_2J_mStop-900_mLSP-100_madgraph-tauola",

    ## Unmatched ntuples
    #"DYJetsToLL_M-50_13TeV-madgraph-pythia8"
    #"PU20bx25_GJets_HT-100to200_madgraph-tauola"
    #"PU20bx25_GJets_HT-200to400_madgraph-tauola"
    #"PU20bx25_GJets_HT-400to600_madgraph-tauola"
    #"PU20bx25_GJets_HT-600toInf_madgraph-tauola"
    #"PU20bx25_QCD_Pt-1000to1400_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-120to170_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-1400to1800_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-15to30_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-170to300_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-1800toInf_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-300to470_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-30to50_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-470to600_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-50to80_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-600to800_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-800to1000_Tune4C_13TeV_pythia8"
#"PU20bx25_QCD_Pt-80to120_Tune4C_13TeV_pythia8"
#"PU20bx25_TTWJets_madgraph-tauola"
#"PU20bx25_WJetsToLNu_madgraph-tauola"
#"TTDMDMJets_M1000GeV"
#"TTDMDMJets_M600GeV"
#"ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola"
#"ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola"
#"ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola"
#"ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola"
}

# FatTop Sample
# LPCNameDir = {
    # "QCD_HT_500to1000"             : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/QCD_HT-500To1000_13TeV-madgraph",
    # "Signal_T2tt_mStop425_mLSP325" : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/SMS-T2tt_2J_mStop-425_mLSP-325_Tune4C_13TeV-madgraph-tauola",
    # "Signal_T2tt_mStop500_mLSP325" : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/SMS-T2tt_2J_mStop-500_mLSP-325_Tune4C_13TeV-madgraph-tauola",
    # "Signal_T2tt_mStop650_mLSP325" : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/SMS-T2tt_2J_mStop-650_mLSP-325_Tune4C_13TeV-madgraph-tauola",
    # "Signal_T2tt_mStop850_mLSP100" : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola",
    # "TTbar"                        : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola",
    # "ZJetsToNuNu_HT_100to200"      : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola",
    # "ZJetsToNuNu_HT_200to400"      : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola",
    # "ZJetsToNuNu_HT_400to600"      : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola",
    # "ZJetsToNuNu_HT_600toInf"      : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola",
    # "WJetsToLNu_HT_100to200"       : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/WJetsToLNu_HT-100to200_Tune4C_13TeV-madgraph-tauola",
    # "WJetsToLNu_HT_200to400"       : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/WJetsToLNu_HT-200to400_Tune4C_13TeV-madgraph-tauola",
    # "WJetsToLNu_HT_400to600"       : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/WJetsToLNu_HT-400to600_Tune4C_13TeV-madgraph-tauola",
    # "WJetsToLNu_HT_600toInf"       : "/eos/uscms/store/user/lpcsusyhad/benwu/PHYS14_FatTop/TopTagTest7/WJetsToLNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola",
# }

LPCNameDir = {
    "TTbarInc"                          : "TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "TTbar_HT-600to800"                 : "TTJets_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "TTbar_HT-800to1200"                : "TTJets_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "TTbar_HT-1200to2500"               : "TTJets_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "TTbar_HT-2500toInf"                : "TTJets_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "TTbarDiLep"                        : "TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "TTbarSingleLepT"                   : "TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "TTbarSingleLepTbar"                : "TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "WJetsToLNu_HT_100to200"            : "WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "WJetsToLNu_HT_200to400"            : "WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "WJetsToLNu_HT_400to600"            : "WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "WJetsToLNu_HT_600to800"            : "WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "WJetsToLNu_HT_800to1200"           : "WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "WJetsToLNu_HT_1200to2500"          : "WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "WJetsToLNu_HT_2500toInf"           : "WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "WJetsToLNu_HT_600toInf"            : "WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "ZJetsToNuNu_HT_600toInf"           : "addJetsForZinv/ZJetsToNuNu_HT-600ToInf_13TeV-madgraph.txt",
    "ZJetsToNuNu_HT_400to600"           : "addJetsForZinv/ZJetsToNuNu_HT-400To600_13TeV-madgraph.txt",
    "ZJetsToNuNu_HT_200to400"           : "addJetsForZinv/ZJetsToNuNu_HT-200To400_13TeV-madgraph.txt",
    "ZJetsToNuNu_HT_100to200"           : "addJetsForZinv/ZJetsToNuNu_HT-100To200_13TeV-madgraph.txt",
    "DYJetsToLL_HT_600toInf"            : "DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "DYJetsToLL_HT_400to600"            : "DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "DYJetsToLL_HT_200to400"            : "DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "DYJetsToLL_HT_100to200"            : "DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "IncDYSampleSet"                    : "DYJetsToLL_M-50_13TeV-madgraph-pythia8.txt",
    "QCD_HT100to200"                    : "QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "QCD_HT200to300"                    : "QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "QCD_HT300to500"                    : "QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "QCD_HT500to700"                    : "QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "QCD_HT700to1000"                   : "QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "QCD_HT1000to1500"                  : "QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "QCD_HT1500to2000"                  : "QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "QCD_HT2000toInf"                   : "QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    "QCD_Pt_10to15"                     : "QCD_Pt_10to15_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_15to30"                     : "QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_30to50"                     : "QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_50to80"                     : "QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_80to120"                    : "QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_120to170"                   : "QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_170to300"                   : "QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_300to470"                   : "QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_470to600"                   : "QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_600to800"                   : "QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_800to1000"                  : "QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_1000to1400"                 : "QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_1400to1800"                 : "QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_1800to2400"                 : "QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_2400to3200"                 : "QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8.txt",
    "QCD_Pt_3200toInf"                  : "QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8.txt",
    "tW_top"                            : "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.txt",
    "tW_antitop"                        : "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.txt",
    "TTZToLLNuNu"                       : "TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt",
    "TTZToQQ"                           : "TTZToQQ_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt",
    "TTWJetsToLNu"                      : "TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt",
    "TTWJetsToQQ"                       : "TTWJetsToQQ_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8.txt",
    "Data_SingleElectron_2015B"         : "SingleElectron/Spring15_74X_Oct_2015_Ntp_v2p1_SingleElectron-Run2015B-PromptReco.txt",
    "Data_SingleElectron_2015C"         : "SingleElectron/Spring15_74X_Oct_2015_Ntp_v2p1_SingleElectron-Run2015C-PromptReco.txt",
    "Data_SingleElectron_2015D"         : "SingleElectron/Spring15_74X_Oct_2015_Ntp_v2p1_SingleElectron-Run2015D-PromptReco.txt",
    "Data_DoubleMuon_2015B"             : "DoubleMuon/Spring15_74X_Oct_2015_Ntp_v2p1_DoubleMuon-Run2015B-PromptReco.txt",
    "Data_DoubleMuon_2015C"             : "DoubleMuon/Spring15_74X_Oct_2015_Ntp_v2p1_DoubleMuon-Run2015C-PromptReco.txt",
    "Data_DoubleMuon_2015D"             : "DoubleMuon/Spring15_74X_Oct_2015_Ntp_v2p1_DoubleMuon-Run2015D-PromptReco.txt",
    "Data_DoubleEG_2015B"               : "DoubleEG/Spring15_74X_Oct_2015_Ntp_v2p1_DoubleEG-Run2015B-PromptReco.txt",
    "Data_DoubleEG_2015C"               : "DoubleEG/Spring15_74X_Oct_2015_Ntp_v2p1_DoubleEG-Run2015C-PromptReco.txt",
    "Data_DoubleEG_2015D"               : "DoubleEG/Spring15_74X_Oct_2015_Ntp_v2p1_DoubleEG-Run2015D-PromptReco.txt",
    "Data_HTMHT_2015B"                  : "HTMHT/Spring15_74X_Oct_2015_Ntp_v2p1_HTMHT-Run2015B-PromptReco.txt",
    "Data_HTMHT_2015C"                  : "HTMHT/Spring15_74X_Oct_2015_Ntp_v2p1_HTMHT-Run2015C-PromptReco.txt",
    "Data_HTMHT_2015D"                  : "HTMHT/Spring15_74X_Oct_2015_Ntp_v2p1_HTMHT-Run2015D-PromptReco_FIX.txt",
    "Signal_T1tttt_mGluino1200_mLSP800" : "T1tttt_2J_mGl-1200_mLSP-800_madgraph-tauola.txt",
    "Signal_T1tttt_mGluino1500_mLSP100" : "T1tttt_2J_mGl-1500_mLSP-100_madgraph-tauola.txt",
    "Signal_T1bbbb_mGluino1000_mLSP900" : "T1bbbb_2J_mGl-1000_mLSP-900_madgraph-tauola.txt",
    "Signal_T1bbbb_mGluino1500_mLSP100" : "T1bbbb_2J_mGl-1500_mLSP-100_madgraph-tauola.txt",
    "Signal_T2tt_mStop425_mLSP325"      : "T2tt_2J_mStop-425_mLSP-325_madgraph-tauola.txt",
    "Signal_T2tt_mStop500_mLSP325"      : "T2tt_2J_mStop-500_mLSP-325_madgraph-tauola.txt",
    "Signal_T2tt_mStop650_mLSP325"      : "T2tt_2J_mStop-650_mLSP-325_madgraph-tauola.txt",
    "Signal_T2tt_mStop850_mLSP100"      : "T2tt_2J_mStop-850_mLSP-100_madgraph-tauola.txt",
    "Signal_T2bb_mSbottom600_mLSP580"   : "T2bb_2J_mStop-600_mLSP-580_madgraph-tauola.txt",
    "Signal_T2bb_mSbottom900_mLSP100"   : "T2bb_2J_mStop-900_mLSP-100_madgraph-tauola.txt",
    "Signal_TTDMDMJets_M600GeV"         : "TTDMDMJets_M600GeV.txt",
    "Signal_TTDMDMJets_M1000GeV"        : "TTDMDMJets_M1000GeV.txt",


    #============================================================================#
    #---------------------------     Top Projected     --------------------------#
    #============================================================================#
    #"DYJetsToLL_HT_600toInf"            : "DYJetsToLL_M-50_HT-600toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    #"DYJetsToLL_HT_400to600"            : "DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    #"DYJetsToLL_HT_200to400"            : "DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    #"DYJetsToLL_HT_100to200"            : "DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    #"tW_antitop"                        : "addJetsForZinv/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.txt",
    #"tW_top"                            : "addJetsForZinv/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1.txt",
    #"TTbarDiLep"                        : "addJetsForZinv/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt",
    #"TTZToLLNuNu"                       : "addJetsForZinv/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8.txt",
    #"ZJetsToNuNu_HT_100to200"           : "addJetsForZinv/ZJetsToNuNu_HT-100To200_13TeV-madgraph.txt",
    #"ZJetsToNuNu_HT_200to400"           : "addJetsForZinv/ZJetsToNuNu_HT-200To400_13TeV-madgraph.txt",
    #"ZJetsToNuNu_HT_400to600"           : "addJetsForZinv/ZJetsToNuNu_HT-400To600_13TeV-madgraph.txt",
    #"ZJetsToNuNu_HT_600toInf"           : "addJetsForZinv/ZJetsToNuNu_HT-600ToInf_13TeV-madgraph.txt",

}


def GetList():
    for key, value in NameDir.items():
        matches = []
        file = open ("%s.list" % key, "w")
        for root, dirnames, filenames in os.walk( "%s/%s/" % (NtupleDir, value) ):
            for filename in fnmatch.filter(filenames, '*.root'):
                file.write("%s\n" % os.path.join(root, filename))
        file.close()


def GetLPClist():
    if os.environ['CMSSW_BASE'] == "":
        return False

    import sys
    import shutil
    sys.path.append("%s/src/SusyAnaTools/Tools/condor/" % os.environ["CMSSW_BASE"])
    from samples import SampleCollection
    sc = SampleCollection()

    for ds in sc.sampleCollectionList():
        ds = ds.strip()
        print(ds)

        for s, n in sc.sampleList(ds):
            if not os.path.isfile(s):
                continue
            shutil.copyfile(s, n+".list")


if __name__ == "__main__":
    if not onLPC:
        GetList()
    else:
        GetLPClist()

