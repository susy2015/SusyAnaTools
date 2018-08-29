#!/bin/bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc530
eval `scramv1 project CMSSW CMSSW_9_2_6`
cd CMSSW_9_2_6/src/
eval `scramv1 runtime -sh` # cmsenv is an alias not on the workers
echo "CMSSW: "$CMSSW_BASE
cd ${_CONDOR_SCRATCH_DIR}
ls
cat ${3}
./stopNTuple_skim ${1} ${2} ${3} Signal_fastsim_${1}_${2}.root
xrdcp Signal_fastsim_${1}_${2}.root root://cmseos.fnal.gov//store/user/lpcsusyhad/Stop_production/SoftBjet_PhotonNtuples/T2tt_mStop-350to400/
rm Signal_fastsim_${1}_${2}.root
