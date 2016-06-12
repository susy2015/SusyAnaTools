## Instructions

The following installation instructions assume the user wants to process Run2015B prompt-reco data or Spring15 MC.

```
cmsrel CMSSW_8_0_10
cd CMSSW_8_0_10/src/
cmsenv
git clone -b TestMiniAOD git@github.com:susy2015/recipeAUX.git
<<<<<<< HEAD
git clone -b prod_80X_Ntp git@github.com:susy2015/SusyAnaTools.git
=======
git clone -b Ana_74X_June08_2016_SearchBins_69_2016 git@github.com:susy2015/SusyAnaTools.git
>>>>>>> b0417c34b1dc4b4ad5ef5bdb9dfb957816debcdc
scram b -j9
```

To submit jobs:

1. cd SusyAnaTools/SkimsAUX/workdir/prod/80X_crab_example/

2. Modify the MultiCrab3.py file for the line:

selSubmitKey = 'TEST ALL'

to:

selSubmitKey = 'TTJets' for all the TTJets* samples or

selSubmitKey = 'WJetsToLNu_HT-100To200' for just the WJetsToLNu_HT-100To200 sample
