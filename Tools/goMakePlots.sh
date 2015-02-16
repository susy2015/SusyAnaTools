#!/bin/bash

export PATH=${PATH}:/cvmfs/cms.cern.ch/common
export CMS_PATH=/cvmfs/cms.cern.ch

cd $2/src
eval `scramv1 runtime -sh`

cd ${_CONDOR_SCRATCH_DIR}

xrdcp root://cmsxrootd-site.fnal.gov//store/user/lpcsusyhad/PHYS14_720_Dec23_2014/rootlist_$1.txt .

./makePlots --condor -D $1

rm rootlist_$1.txt
