from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'PHYS14_PU20bx25_TTJets'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'treeMaker_stopRA2.py'
config.JobType.allowNonProductionCMSSW = False

config.section_("Data")
config.Data.inputDataset = '/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 5
config.Data.publication = False
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/'
config.Data.publishDataName = 'PHYS14_PU20bx25_PHYS14_25_V1-FLAT'
#Use your own username instead of the "lhx". Keep branch tag in the directory name, e.g., PHYS14_720_Dec23_2014.
config.Data.outLFN = '/store/group/lpcsusyhad/PHYS14_720_Mar14_2014_v1/lhx/PU20bx25_TTJets_MSDecaysCKM_madgraph-tauola/'

config.Data.ignoreLocality = False

config.section_("Site")
config.Site.storageSite = 'T3_US_FNALLPC'
