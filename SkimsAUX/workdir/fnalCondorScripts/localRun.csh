#!/bin/csh

echo "`scram tool info python | grep PYTHON_BASE | sed 's/PYTHON_BASE=/setenv PYTHONHOME /'`\n"
eval `scram tool info python | grep PYTHON_BASE | sed 's/PYTHON_BASE=/setenv PYTHONHOME /'`

set index=$1

if (! -d manual_${index} ) then
   mkdir manual_${index}
endif
set currPath=`pwd`
cd manual_${index}
ln -s ../fit_Muon_Data_Comb.root .
ln -s ../fit_Muon_MC_Comb.root .
ln -s ../tauTemplate.root .
ln -s ../applyTauTemplate_onSkims_cfg.py .
ln -s ../inputList_${index}.txt .

cmsRun applyTauTemplate_onSkims_cfg.py fileslist=inputList_${index}.txt GlobalTag=GR_R_42_V19::All hltName=HLT hltSelection="HLT_IsoMu17_*DiCentral*Jet30_v*" mcInfo=0 doHLTeff=1 >& log_run.lg &

cd currPath
