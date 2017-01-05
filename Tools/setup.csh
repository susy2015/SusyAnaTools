set called=($_)
set temp=(`getopt -s tcsh -o d:l -- $argv:q`)
if ($? != 0) then 
  echo "Terminating..." >/dev/stderr
  exit 1
endif

set TAGGERCFGDIR=""

# Now we do the eval part. As the result is a list, we need braces. But they
# must be quoted, because they must be evaluated when the eval is called.
# The 'q` stops doing any silly substitutions.
eval set argv=\($temp:q\)

while (1)
    switch($1:q)
        case -d:
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

if ($TAGGERCFGDIR:q == "") then
    set TAGGERCFGDIR=$SRC/TopTaggerCfgs
endif

if ! $?LD_LIBRARY_PATH then
    setenv LD_LIBRARY_PATH ./:${SRC}/opencv/lib/:${SRC}/TopTagger/TopTagger/test/:${SRC}/src/SusyAnaTools/Tools/obj/
else
    setenv LD_LIBRARY_PATH ./:${SRC}/opencv/lib/:${SRC}/TopTagger/TopTagger/test/:${SRC}/src/SusyAnaTools/Tools/obj/:${LD_LIBRARY_PATH}
endif

## Get the btagging file
if (! -f CSVv2_ichep.csv) then
  ln -s ${SRC}/SusyAnaTools/Tools/CSVv2_ichep.csv .
endif

if (! -f TTbarNoHad_bTagEff.root) then
  ln -s ${SRC}/SusyAnaTools/Tools/TTbarNoHad_bTagEff.root .
endif

## Pileup Reweighting
if (! -f PileupHistograms_Nov17.root) then
  ln -s ${SRC}/SusyAnaTools/Tools/PileupHistograms_Nov17.root .
endif

##Checkout latest toptagger cfg file 

mkdir -p $TAGGERCFGDIR
${SRC}/TopTagger/Tools/getTaggerCfg.sh -t MVAAK8_Tight_v1.1.1 -d $TAGGERCFGDIR
${SRC}/TopTagger/Tools/getTaggerCfg.sh -t Legacy_AK4Only_v0.1.0 -f Legacy_TopTagger.cfg -d $TAGGERCFGDIR
