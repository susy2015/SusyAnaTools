#!/bin/bash

export PATH=${PATH}:/cvmfs/cms.cern.ch/common
export CMS_PATH=/cvmfs/cms.cern.ch

tar -zxvf $2.tar.gz
cd $2/src
source /cvmfs/cms.cern.ch/cmsset_default.sh

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${_CONDOR_SCRATCH_DIR}/$2/src/SusyAnaTools/Tools/obj:${_CONDOR_SCRATCH_DIR}/$2/src/TopTagger/TopTagger/test:${_CONDOR_SCRATCH_DIR}/$2/src/opencv/lib:${_CONDOR_SCRATCH_DIR}/$2/lib/${SCRAM_ARCH}

eval `scramv1 runtime -sh`

cd ${_CONDOR_SCRATCH_DIR}

xrdcp root://cmseos.fnal.gov/$(echo $6 | sed 's|/eos/uscms||') .

./basicCheck $1 0 $3 $4 $7 "condor"
