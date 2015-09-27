from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = REQUESTNAME

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'treeMaker_stopRA2.py'
config.JobType.allowUndistributedCMSSW = False
config.JobType.pyCfgParams = ['mcInfo=0', 'GlobalTag=74X_dataRun2_Prompt_v1', 'specialFix=JEC', 'jecDBname=Summer15_25nsV2_DATA']
config.JobType.inputFiles = ['Summer15_25nsV2_DATA.db']

config.section_("Data")
config.Data.inputDataset = DATASETNAME
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
#Example config for data
#config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-256869_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
config.Data.lumiMask = JSON
config.Data.splitting = 'LumiBased'
#config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = UNITPERJOB
config.Data.publication = False
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/'
config.Data.publishDataName = PUBDATANAME
#Use your own username instead of the "lhx". Keep branch tag in the directory name, e.g., Spring15_720_Dec23_2014.
#config.Data.outLFNDirBase = '/store/group/lpcsusyhad/Spring15_74X_July_2015_v1.1/TTJets_madgraphMLM-pythia8/'
config.Data.outLFNDirBase = OUTLFNDIRBASE

config.Data.ignoreLocality = False

config.section_("Site")
config.Site.storageSite = 'T3_US_FNALLPC'
