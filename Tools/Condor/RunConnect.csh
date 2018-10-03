#!/bin/csh -v

set CMSSW = DELDIR
set EXE   = DELEXE
set OUTPUT = OUTDIR

#============================================================================#
#-----------------------------   Setup the env   ----------------------------#
#============================================================================#
echo "============  Running on" $HOST " ============"
source /cvmfs/cms.cern.ch/cmsset_default.csh
setenv SCRAM_ARCH  DELARCH
cmsrel ${CMSSW}
cd ${CMSSW}/src
eval `scramv1 runtime -csh`
cd -

foreach tarfile (`ls *gz FileList/*gz`)
  echo $tarfile
  tar -xzf $tarfile 
end

if ! $?LD_LIBRARY_PATH then
    setenv LD_LIBRARY_PATH ./
else
    setenv LD_LIBRARY_PATH ./:${LD_LIBRARY_PATH}
endif

#============================================================================#
#--------------------------   To Run the Process   --------------------------#
#============================================================================#
echo $EXE $argv
time ./$EXE $argv

if ($? == 0) then
  foreach tarfile (`ls *gz FileList/*gz`)
    tar -tf $tarfile  | xargs rm -r
  end
  foreach outfile (`ls *root`)
    echo "Copying ${outfile} to ${OUTPUT}"
    xrdcp $outfile "root://cmseos.fnal.gov/${OUTPUT}/${outfile}"
    if ($? == 0) then
      rm $outfile
    endif
  end
endif
