## Instructions

The following installation instructions assume the user wants to process Run2015B prompt-reco data or Spring15 MC.

```
cmsrel CMSSW_7_4_6_patch6
cd CMSSW_7_4_6_patch6/src/
cmsenv
git cms-merge-topic -u cms-met:METCorUnc74X
git clone -b TestMiniAOD git@github.com:susy2015/recipeAUX.git
git clone -b Ntp_74X_27Sep2015_v2.0 git@github.com:susy2015/SusyAnaTools.git
scram b -j9
```
