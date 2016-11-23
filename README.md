### Instructions

## Production Code

The following installation instructions assume the user wants to process Run2016 data or Spring16 MC.

```
cmsrel CMSSW_8_0_23
cd CMSSW_8_0_23/src/
cmsenv
git cms-init
git cms-merge-topic -u kpedro88:METfix8022
git cms-merge-topic -u cms-met:CMSSW_8_0_X-METFilterUpdate
git clone -b TestMiniAOD git@github.com:susy2015/recipeAUX.git
git clone git@github.com:cms-jet/JetToolbox.git JMEAnalysis/JetToolbox -b jetToolbox_80X_V2
git clone -b Moriond2017 git@github.com:susy2015/SusyAnaTools.git
scram b -j9
```

To produce ntuples with crab submission (google doc for production signup is https://docs.google.com/spreadsheets/d/17Hpp5S-UkiKvuugKxqbW0-3aLhiJrJP8MEEyHce_Lzw/edit#gid=0):
```
cd SusyAnaTools/SkimsAUX/workdir/prod/80X_crab_example
# Edit the line in MultiCrab3.py
# selSubmitKey = 'TEST ALL'
# to the sample keys you'd like to submit. Note we use space to seperate the sample keys.
# selSubmitKey = 'TTJets HTMHT'
# Will let you submit all the samples matching "*TTJets*" and "*HTMHT*"
# ----
# After submission, one can change the selSubmitKey to the following to monitor and re-submit failed jobs for you automatically
# selSubmitKey = 'TEST STATUS TTJets HTMHT'
```

To test the ntuple production interactively:
```
cd SusyAnaTools/SkimsAUX/workdir/prod/80X_crab_example
# do "cmsRun treeMaker_stopRA2.py" but with commandline options extracted from the MultiCrab3.py.
# For instance, for 2016 MC, do the following from
# https://github.com/susy2015/SusyAnaTools/blob/master/SkimsAUX/workdir/prod/80X_crab_example/MultiCrab3.py#L271:
# cmsRun treeMaker_stopRA2.py mcInfo=1 GlobalTag=80X_mcRun2_asymptotic_2016_miniAODv2 specialFix=JEC jecDBname=Spring16_25nsV1_MC maxEvents=1000
```


## TopTagger Code

To Checkout TopTagger Code:

```
## Checkout OpenCV
cd $CMSSW_BASE/src
git clone git@github.com:susy2015/opencv.git
cd opencv
git checkout 3.1.0_StopBugFix
cmake .
make -j 8

## Checkout Tagtagger
cd $CMSSW_BASE/src
git clone git@github.com:susy2015/TopTagger.git
scram b -j 8
cd TopTagger/TopTagger/test/
make -j 8 
```

You can then compile the SUSYAnaTools
```
cd $CMSSW_BASE/src/SusyAnaTools/Tools/
make
```

## Setup Tools Environment
Go to your working area, then do below
```
source $CMSSW_BASE/src/SusyAnaTools/Tools/setup.csh
```
