#!/bin/bash

_PWD=${PWD}
#_CONDOR_SCRATCH_DIR=${PBS_O_WORKDIR}

printenv

export PATH=${PATH}:/cvmfs/cms.cern.ch/common
export CMS_PATH=/cvmfs/cms.cern.ch

#move the files from /home to /var/tmp
ls -alh

mv ${_PWD}/$2.tar.gz ${_CONDOR_SCRATCH_DIR}
mv ${_PWD}/tTag.tar.gz ${_CONDOR_SCRATCH_DIR}

cd ${_CONDOR_SCRATCH_DIR}

#get the release setup and in place
tar -xzf $2.tar.gz
cd $2/
mkdir -p src
cd src
scram b ProjectRename
eval `scramv1 runtime -sh`

#set up local code
tar -xzf ${_CONDOR_SCRATCH_DIR}/tTag.tar.gz
cd WORLDSWORSESOLUTIONTOAPROBLEM

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PWD}

echo "\n ============================================ \n"

printenv

bool isCondor=true
#echo LD_LIBRARY_PATH
echo "xrdcp root://cmseos.fnal.gov/$(echo $7 | sed 's|/eos/uscms||') ."
xrdcp root://cmseos.fnal.gov/$(echo $7 | sed 's|/eos/uscms||') .

#if [ ! -f ${6##*/} ]; then
#    echo "File not found! Try xrdcp again"
#    export X509_USER_PROXY=/cms/data/$USER/.x509_user_proxy
#    xrdcp root://cmseos.fnal.gov/$(echo $6 | sed 's|/eos/uscms||') .
#fi

pwd
ls -lhrt

#export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${_CONDOR_SCRATCH_DIR}

echo $1
echo $3
echo $4
echo $5
echo $6


for i in `seq $4 $(($4 + $3))`; do

    ./tTagEfficiencyCalc $1 -1 1 $i $5 $6 1 

done

ls -lhrt

mv *_tTagEff_*.root ${_CONDOR_SCRATCH_DIR}
#mv topStudyOutput_* /cms/data/pastika/condorOutput

rm $(echo $6 | sed 's|.*/||')
rm -r ${_CONDOR_SCRATCH_DIR}/$2
