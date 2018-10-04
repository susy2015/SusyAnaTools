#!/bin/tcsh

set called=($_)
set temp=(`getopt -s tcsh -o s:t:l -- $argv:q`)
if ($? != 0) then 
  echo "Terminating..." >/dev/stderr
  exit 1
endif

set STOPCFGDIR=""
set TAGGERCFGDIR=""

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

#it is safer to clear our old cfg files, but as a safety only if they are softlinks
set STOPSETCFG=sampleSets.cfg
if ( -f $STOPSETCFG && -l $STOPSETCFG ) then
    rm $STOPSETCFG
endif
set STOPCOLCFG=sampleCollections.cfg
if ( -f $STOPCOLCFG && -l $STOPCOLCFG ) then
    rm $STOPCOLCFG
endif

# -p give no error if directory exists
mkdir -p $STOPCFGDIR
# -o for overwrite softlinks if they exist
${SRC}/SusyAnaTools/Tools/scripts/getStopCfg.sh -o -t CMSSW8028_2016_v1.0.1 -d $STOPCFGDIR

##Checkout latest toptagger cfg file 

#it is safer to clear our old cfg files, but as a safety only if they are softlinks
set TOPTAGGERFILE=TopTagger.cfg
if ( -f $TOPTAGGERFILE && -l $TOPTAGGERFILE ) then
    rm $TOPTAGGERFILE
endif
set SIMPLETOPTAGGERFILE=TopTagger_Simplified.cfg
if ( -f $SIMPLETOPTAGGERFILE && -l $SIMPLETOPTAGGERFILE ) then
    rm $SIMPLETOPTAGGERFILE
endif
set LEGTOPTAGGERFILE=Legacy_TopTagger.cfg
if ( -f $LEGTOPTAGGERFILE && -l $LEGTOPTAGGERFILE ) then
    rm $LEGTOPTAGGERFILE
endif

# -p give no error if directory exists
mkdir -p $TAGGERCFGDIR
# -o for overwrite softlinks if they exist
${SRC}/TopTagger/TopTagger/scripts/getTaggerCfg.sh -o -t MVAAK8_Tight_v1.2.1 -d $TAGGERCFGDIR
${SRC}/TopTagger/TopTagger/scripts/getTaggerCfg.sh -o -t Legacy_AK4Only_v0.1.1 -f $LEGTOPTAGGERFILE -d $TAGGERCFGDIR
${SRC}/TopTagger/TopTagger/scripts/getTaggerCfg.sh -o -t MVAAK8_Tight_noQGL_binaryCSV_v1.0.2 -f $SIMPLETOPTAGGERFILE -d $TAGGERCFGDIR


