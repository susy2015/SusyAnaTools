## Instructions

The following installation instructions assume the user wants to process Run2016 data or Spring16 MC.

```
# Similar for 80X release
cmsrel CMSSW_8_0_10
cd CMSSW_8_0_10/src/
cmsenv
git clone -b TestMiniAOD git@github.com:susy2015/recipeAUX.git
git clone -b Ana_Prod_merged_June17_2016_fix_top_projection_bug git@github.com:susy2015/SusyAnaTools.git
scram b -j9
```

To produce ntuples with crab submission:
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
# https://github.com/susy2015/SusyAnaTools/blob/master/SkimsAUX/workdir/prod/80X_crab_example/MultiCrab3.py#L271
# cmsRun treeMaker_stopRA2.py mcInfo=1 GlobalTag=80X_mcRun2_asymptotic_2016_miniAODv2 specialFix=JEC jecDBname=Spring16_25nsV1_MC maxEvents=1000
```
