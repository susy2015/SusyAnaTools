#!/bin/bash

export PATH=${PATH}:/cvmfs/cms.cern.ch/common
export CMS_PATH=/cvmfs/cms.cern.ch

cd $1/src
eval `scramv1 runtime -sh`

cd ${_CONDOR_SCRATCH_DIR}

cmsRun treeMaker_stopRA2.py fileslist=$2 numfile=$3 startfile=$4

xrdcp stopFlatNtuples_$5.root root://cmseos.fnal.gov/$6/stopFlatNtuples_$5.root
