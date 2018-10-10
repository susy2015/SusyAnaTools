#!/bin/tcsh

set called=($_)
set temp=(`getopt -s tcsh -o s:t:lo -- $argv:q`)
if ($? != 0) then 
  echo "Terminating..." >/dev/stderr
  exit 1
endif

set STOPCFGDIR=""
set TAGGERCFGDIR=""
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
    if ( "$called" != "" ) then  ### called by source 
        set rootdir=`dirname $called[2]`       # may be relative path
    else
        set rootdir=`dirname $0`       # may be relative path
    endif
    set rootdir=`cd $rootdir && pwd`/../..    # ensure absolute path
    echo "CMSSW_BASE not set, setting SRC variable to "$rootdir" assuming local compile"
    set SRC=$rootdir
else
    set SRC=${CMSSW_BASE}/src
endif

## default STOPCFGDIR
if ($STOPCFGDIR:q == "") then
    set STOPCFGDIR=$SRC/myStopCfgs
endif

## default TAGGERCFGDIR
if ($TAGGERCFGDIR:q == "") then
    set TAGGERCFGDIR=$SRC/myTopTaggerCfgs
endif

if ! $?LD_LIBRARY_PATH then
    setenv LD_LIBRARY_PATH ./:${SRC}/opencv/lib/:${SRC}/TopTagger/TopTagger/test/:${SRC}/SusyAnaTools/Tools/obj/
else
    setenv LD_LIBRARY_PATH ./:${SRC}/opencv/lib/:${SRC}/TopTagger/TopTagger/test/:${SRC}/SusyAnaTools/Tools/obj/:${LD_LIBRARY_PATH}
endif

##Checkout stop cfg files and supplementary files

# -p give no error if directory exists
mkdir -p $STOPCFGDIR
# set OVERWRITE = -o via options to overwrite softlinks if they exist
${SRC}/SusyAnaTools/Tools/scripts/getStopCfg.sh $OVERWRITE -t CMSSW8028_2016_v1.0.2 -d $STOPCFGDIR

##Checkout latest toptagger cfg file 

set TOPTAGGERFILE=TopTagger.cfg
set SIMPLETOPTAGGERFILE=TopTagger_Simplified.cfg
set LEGTOPTAGGERFILE=Legacy_TopTagger.cfg

# -p give no error if directory exists
mkdir -p $TAGGERCFGDIR
# set OVERWRITE = -o via options to overwrite softlinks if they exist
${SRC}/TopTagger/TopTagger/scripts/getTaggerCfg.sh $OVERWRITE -t MVAAK8_Tight_v1.2.1 -f $TOPTAGGERFILE -d $TAGGERCFGDIR
${SRC}/TopTagger/TopTagger/scripts/getTaggerCfg.sh $OVERWRITE -t Legacy_AK4Only_v0.1.1 -f $LEGTOPTAGGERFILE -d $TAGGERCFGDIR
${SRC}/TopTagger/TopTagger/scripts/getTaggerCfg.sh $OVERWRITE -t MVAAK8_Tight_noQGL_binaryCSV_v1.0.2 -f $SIMPLETOPTAGGERFILE -d $TAGGERCFGDIR


