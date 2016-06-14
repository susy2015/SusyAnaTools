## Instructions

The following installation instructions assume the user wants to process Run2016 data or Spring16 MC.

```
# Similar for 80X release
cmsrel CMSSW_8_0_10
cd CMSSW_8_0_10/src/
cmsenv
git clone -b TestMiniAOD git@github.com:susy2015/recipeAUX.git
# master branch is now for 80X analysis
git clone Ana_June14_2016_fix_event_filter_bugs git@github.com:susy2015/SusyAnaTools.git
scram b -j9
cd SusyAnaTools/Tools
# The samples.cc(h) are for 80X analysis
git checkout remotes/origin/FromKash samples.cc
git checkout remotes/origin/FromKash samples.h
```
