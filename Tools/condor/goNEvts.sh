#!/bin/bash

_PWD=${PWD}
#_CONDOR_SCRATCH_DIR=${PBS_O_WORKDIR}

printenv

echo ""

ls 

echo ""

export PATH=${PATH}:/cvmfs/cms.cern.ch/common
export CMS_PATH=/cvmfs/cms.cern.ch

#move the files from /home to /var/tmp
if [ ! -f ${_CONDOR_SCRATCH_DIR}/$2.tar.gz ]; then
    mv ${_PWD}/$2.tar.gz ${_CONDOR_SCRATCH_DIR}
fi
if [ ! -f ${_CONDOR_SCRATCH_DIR}/TT.tar.gz ]; then
    mv ${_PWD}/TT.tar.gz ${_CONDOR_SCRATCH_DIR}
fi

cd ${_CONDOR_SCRATCH_DIR}

#get the release setup and in place
tar -xzf $2.tar.gz
cd $2/
mkdir -p src
cd src
scram b ProjectRename
eval `scramv1 runtime -sh`

#set up local code
tar -xzf ${_CONDOR_SCRATCH_DIR}/TT.tar.gz
cd WORLDSWORSESOLUTIONTOAPROBLEM

mkdir -p ../obj
mv samplesModule.so ../obj

pwd
ls -lhrt

python nEvts.py -s $3 -d "^$1$" > output_$1.txt
ls -lhrt

mv output_$1.txt ${_CONDOR_SCRATCH_DIR}

rm -r ${_CONDOR_SCRATCH_DIR}/$2
