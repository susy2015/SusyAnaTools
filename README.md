## Instructions

The following installation instructions assume the user wants to process Run2016 data or Spring16 MC.

```
cmsrel CMSSW_8_0_23_patch1
cd CMSSW_8_0_23_patch1/src/
cmsenv
git cms-init
git cms-merge-topic -u kpedro88:METfix8022
git cms-merge-topic -u cms-met:CMSSW_8_0_X-METFilterUpdate
git clone -b TestMiniAOD git@github.com:susy2015/recipeAUX.git
git clone -b prodNtpV10_AK8_QGL_Oct20_2016 git@github.com:susy2015/SusyAnaTools.git
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

To Checkout TopTagger Code:

Inside CMSSW_8_0_23_patch1/src/
```
git clone -b master git@github.com:susy2015/TopTagger.git
```
Currently OpenCV package needed to compile toptagger code.

OpenCV provides a C++ interface to a random forest algorithm which can be integrated into our normal c++ workflow.

Installing openCV

In order to use sample weights we need the latest release of openCV 3.1.0 and a bug fix I have added to stop RTree training form crashing.  You may download the repository from github via my fork in susy2015.

To install from github do the following in your "CMSSW_8_0_23_patch1/src"

```
git clone git@github.com:susy2015/opencv.git
cd opencv
git checkout 3.1.0_StopBugFix
```

To compile openCV run the following commands in the "opencv" folder

```
cmake .
make -j6
```
Now compile full package:

Inside CMSSW_8_0_23_patch1/src/
```
scram b -j9
```

To use the toptagger or run toptagger Please check following link

https://github.com/susy2015/TopTagger#configuration-file