### Instructions

## Updating a git repo to a new branch 

Commit all outstanding change to a new branch.
```
git fetch
git checkout NEW_TAG_NAME
```

## Production Code

The following installation instructions assume the user wants to process Run2016 data or Spring16 MC.

```
cmsrel CMSSW_9_2_6
cd CMSSW_9_2_6/src/
cmsenv
git cms-init
git clone -b TestMiniAOD git@github.com:susy2015/recipeAUX.git
git clone git@github.com:cms-jet/JetToolbox.git JMEAnalysis/JetToolbox -b jetToolbox_91X_v1 (https://github.com/cms-jet/JetToolbox/tree/jetToolbox_91X)
git clone -b CMSSW926 git@github.com:susy2015/SusyAnaTools.git
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
git checkout 3.1.1_BugFix
cmake .
make -j 8

## Checkout Tagtagger
cd $CMSSW_BASE/src
git clone -b HadStopAnaDevel_v8_Moriond2017_Mar27_2017 git@github.com:susy2015/TopTagger.git
cd TopTagger
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

## Check out tagger configuration 
By default SusyAnaTools/Tools/setup.csh will check out the default working point of the tagger configuration. To checkout another working point go to your working area where the code will be run from and run the following command 
```
$CMSSW_BASE/src/TopTagger/Tools/getTaggerCfg.sh -t DESIRED_TAG -d PATH_TO_DOWNLOAD_TO
```
If the legacy AK4 only version of the tagger is also needed run the following to checkout this configuration file
```
$CMSSW_BASE/src/TopTagger/Tools/getTaggerCfg.sh -t Legacy_AK4Only_v0.1.0 -f Legacy_TopTagger.cfg -d PATH_TO_DOWNLOAD_TO
```

Please note that this script will download the deired tagger configuration information in a seperate folder and softlink the necessary files into your corrent directory.  If you have multiple configuration files you work with in the same working directory use the "-f name" option to specify the name of the softlink to the tagger file (the default is TopTagger.cfg).  If you do not want this directory in your working directory you may add the "-d PATH_TO_DOWNLOAD_TO" option to specify where it should download these files.  This is particularly useful if you have several working directories as the script will then only check out each working point once and place softlinks as needed (the model files are rather large so this may save some space if you have many downloaded).  However, "-d" can be omitted and the files will be downloaded in your working directory.  Other configuration file releases can be found here https://github.com/susy2015/TopTaggerCfg/releases
