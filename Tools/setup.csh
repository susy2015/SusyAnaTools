#!/bin/tcsh

set called=($_)
set temp=(`getopt -s tcsh -o s:t:lo -- $argv:q`)
if ($? != 0) then 
  echo "Terminating..." >/dev/stderr
  exit 1
endif

set STOPCFGDIR=""
set TAGGERCFGDIR=""
set OPENCV_DIRECTORY="/uscms_data/d3/pastika/zinv/dev/CMSSW_7_4_8/src/opencv/"
set OVERWRITE=""

# Now we do the eval part. As the result is a list, we need braces. But they
# must be quoted, because they must be evaluated when the eval is called.
# The 'q` stops doing any silly substitutions.
eval set argv=\($temp:q\)

while (1)
    switch($1:q)
        case -s:
            set STOPCFGDIR=$2:q
            shift; shift
            breaksw;
        case -t:
            set TAGGERCFGDIR=$2:q
            shift; shift
            breaksw;
        case -l:
            echo "Option l, argument "\`$2:q\'
            shift
            breaksw;
        case -o:
            set OVERWRITE="-o"
            shift
            breaksw;
        case --:
            shift
            break
        default:
            echo "Internal error!" ; exit 1
    endsw
end

set SRC=""

if ! $?CMSSW_BASE then
    set rootdir=""
    if ( "$called" != "" ) then            # called by source 
        set rootdir=`dirname $called[2]`   # may be relative path
    else
        set rootdir=`dirname $0`           # may be relative path
    endif
    set rootdir=`cd $rootdir && pwd`/../.. # ensure absolute path
    echo "CMSSW_BASE not set, setting SRC variable to "$rootdir" assuming local compile"
    set SRC=$rootdir
else
    set SRC=${CMSSW_BASE}/src
endif

if ($TAGGERCFGDIR:q == "") then
    set TAGGERCFGDIR=$SRC/TopTaggerCfgs
endif

if ! $?LD_LIBRARY_PATH then
    setenv LD_LIBRARY_PATH ./:${OPENCV_DIRECTORY}/lib/:${SRC}/opencv/lib/:${SRC}/TopTagger/TopTagger/test/:${SRC}/SusyAnaTools/Tools/obj/
else
    setenv LD_LIBRARY_PATH ./:${OPENCV_DIRECTORY}/lib/:${SRC}/opencv/lib/:${SRC}/TopTagger/TopTagger/test/:${SRC}/SusyAnaTools/Tools/obj/:${LD_LIBRARY_PATH}
endif

## default STOPCFGDIR
if ($STOPCFGDIR:q == "") then
    set STOPCFGDIR=$SRC/myStopCfgs
endif

## default TAGGERCFGDIR
if ($TAGGERCFGDIR:q == "") then
    set TAGGERCFGDIR=$SRC/myTopTaggerCfgs
endif

# run taggerSetup.sh
tcsh ${SRC}/TopTagger/TopTagger/test/taggerSetup.csh

# set LD_LIBRARY_PATH
if ! $?LD_LIBRARY_PATH then
    setenv LD_LIBRARY_PATH ./:${SRC}/SusyAnaTools/Tools/obj/
else
    setenv LD_LIBRARY_PATH ./:${SRC}/SusyAnaTools/Tools/obj/:${LD_LIBRARY_PATH}
endif

##Checkout latest toptagger cfg file 
${SRC}/SusyAnaTools/Tools/scripts/getAllCfg.sh $OVERWRITE -c ${SRC} -s $STOPCFGDIR -t $TAGGERCFGDIR

