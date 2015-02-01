#!/bin/bash

export configSpecStr=$1

export currDir=`pwd`

export outputDir=${currDir}'/output/'

echo $configSpecStr
#echo $configDir
echo $outputDir
echo $currDir

export inputlist=$currDir'/inputList_'$configSpecStr'.txt'
export cutlist=$currDir'/cutList_'$configSpecStr'.txt'
export outputroot=$outputDir'jetMETana_'$configSpecStr
export outputcuteff=$outputDir'cutEff_'$configSpecStr

echo $inputlist
echo $cutlist
echo $outputroot
echo $outputcuteff

export filename='run_'$configSpecStr'.sh'
if [ -e $filename ]; then
  rm $filename
fi

#----------
cat >> $filename <<EOF
#!/bin/bash

#---

universe = vanilla
Executable = ${CMSSW_BASE}/src/UserCode/jetMETanalyzer/jetMETanalyzer
Requirements = Memory >= 199 &&OpSys == "LINUX"&& (Arch != "DUMMY" )
Should_Transfer_Files = YES
WhenToTransferOutput = ON_EXIT
getenv = true
initialdir = ${outputDir}
Input = badChannels_SUSY2010_TFileService.root
Output = anaJetMET_${configSpecStr}_\$(Cluster)_\$(Process).stdout
Error = anaJetMET_${configSpecStr}_\$(Cluster)_\$(Process).stderr
Log = anaJetMET_${configSpecStr}_\$(Cluster)_\$(Process).log
Arguments = $inputlist $cutlist $outputroot $outputcuteff 
Queue 

#exit
EOF
#----------

chmod 755 $filename

#condor_submit xxx.sh
#condor_q -submitter lhx
#condor_rm -name cmslpc15 jobID

exit
