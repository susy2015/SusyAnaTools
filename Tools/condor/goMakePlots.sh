#!/bin/bash

export PATH=${PATH}:/cvmfs/cms.cern.ch/common
export CMS_PATH=/cvmfs/cms.cern.ch
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$2/src/SusyAnaTools/Tools/obj:$2/src/TopTagger/TopTagger/test:$2/src/opencv/lib

cd $2/src
eval `scramv1 runtime -sh`

cd ${_CONDOR_SCRATCH_DIR}

xrdcp root://cmseos.fnal.gov/$(echo $6 | sed 's|/eos/uscms||') .

./basicCheck $1 0 $3 $4 $7 "condor"
