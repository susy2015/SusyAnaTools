## Instructions

The following installation instructions assume the user wants to process Run2016 data or Spring16 MC.

```
# Similar for 80X release
cmsrel CMSSW_8_0_10
cd CMSSW_8_0_10/src/
cmsenv
git clone -b TestMiniAOD git@github.com:susy2015/recipeAUX.git
git clone -b Ana_Prod_merged_June16_2016 git@github.com:susy2015/SusyAnaTools.git
scram b -j9
```
