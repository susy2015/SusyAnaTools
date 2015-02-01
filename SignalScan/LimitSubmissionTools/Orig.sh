#!/bin/bash
  exec > /dev/null 2>/dev/null
  WORKDIR=AUniquePlaceholderName
  #CONFIGDIR=ConfigDirPlaceholderName
  RELEASEDIR=ReleaseDirPlaceholderName
  
  ZERO="" #$SGE_TASK_ID
  if [ $SGE_TASK_ID -lt 1000 ]; then
    ZERO="0"
  fi
  if [ $SGE_TASK_ID -lt 100 ]; then
    ZERO="00"
  fi
  if [ $SGE_TASK_ID -lt 10 ]; then
    ZERO="000"
  fi
  JOB="$ZERO$SGE_TASK_ID"

  # change to scratch directory (local, not lustre in this example)
  #cd $TMPDIR
  # copy raw data to scratch directory (here from the afs)
  # DO NOT USE afscp UNTIL FURTHER NOTICE afscp /afs/naf.desy.de/group/vo/rawdata/bigfile bigfile
  # cp /afs/naf.desy.de/group/vo/rawdata/bigfile bigfile
  #sleep $SGE_TASK_ID
  
  #cp -r $RELEASEDIR .
  #RELEASEBASE=`basename $RELEASEDIR`
  cd $RELEASEDIR/src
  ini cmssw
  cmsenv
  #ini root530 
  cd $WORKDIR
  
  mkdir job$JOB/results
  mkdir job$JOB/log
  cd job$JOB

  pwd 
  echo "SGE_TASK_ID = $SGE_TASK_ID" 
  date
  for i in `cat config/config_*` 
  do
    echo $i
    cat ../../$i
    file="${i##*/}"
    echo "copying ../../$i to config/$file"
    cp ../../$i config/$file  
    echo "calling ../../limit config/$file"  
    ../../limit config/$file
  done
  du -h --max-depth=1
  date

  #cp results/* job$SGE_TASK_ID/results/.
  cp "$TMPDIR"/stdout.txt log/.
  cp "$TMPDIR"/stderr.txt log/.
  #cp -r job$SGE_TASK_ID $RESULTPATH/.

### qsub -t 1-3609 -l h_cpu=00:15:00 -l h_vmem=4000M job.sh
