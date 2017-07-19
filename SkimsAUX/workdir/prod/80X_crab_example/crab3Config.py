from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'Spring15DR74_Asympt25ns_TTJets'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'run_topNtuplizer.py'
config.JobType.allowUndistributedCMSSW = False
config.JobType.inputFiles = ["QGL_cmssw8020_v2.db",]
#config.JobType.pyCfgParams = ['GlobalTag=GR_P_V56', 'mcInfo=0']

config.section_("Data")
config.Data.inputDataset = '/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM'
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
#Example config for data
#config.Data.lumiMask = 'json_DCSONLY_Run2015B.txt'
##config.Data.runRange = '193093-193999'
#config.Data.splitting = 'LumiBased'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 5
config.Data.publication = False
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/'
config.Data.outputDatasetTag = 'Spring15DR74_Asympt25ns_V1.1'
#Use your own username instead of the "lhx". Keep branch tag in the directory name, e.g., Spring15_720_Dec23_2014.
config.Data.outLFNDirBase = '/store/group/lpcsusyhad/Spring15_74X_July_2015_v1.1/lhx/TTJets_madgraphMLM-pythia8/'

config.Data.ignoreLocality = False

config.section_("Site")
config.Site.storageSite = 'T3_US_FNALLPC'
