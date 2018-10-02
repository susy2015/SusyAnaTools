set called=($_)
set temp=(`getopt -s tcsh -o d:l -- $argv:q`)
if ($? != 0) then 
  echo "Terminating..." >/dev/stderr
  exit 1
endif

set TAGGERCFGDIR=""
set STOPCFGDIR=""

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
    set TAGGERCFGDIR=$SRC/myTopTaggerCfgs
endif

if ($STOPCFGDIR:q == "") then
    set STOPCFGDIR=$SRC/myStopCfgs
endif

if ! $?LD_LIBRARY_PATH then
    setenv LD_LIBRARY_PATH ./:${SRC}/opencv/lib/:${SRC}/TopTagger/TopTagger/test/:${SRC}/SusyAnaTools/Tools/obj/
else
    setenv LD_LIBRARY_PATH ./:${SRC}/opencv/lib/:${SRC}/TopTagger/TopTagger/test/:${SRC}/SusyAnaTools/Tools/obj/:${LD_LIBRARY_PATH}
endif

# previous version of softlinks
# no longer needed
#
# ## Get the btagging file
# if (! -f CSVv2_Moriond17_B_H.csv) then
#   ln -s ${SRC}/SusyAnaTools/Tools/CSVFiles/CSVv2_Moriond17_B_H.csv .
# endif
# 
# if (! -f allINone_bTagEff.root) then
#   ln -s ${SRC}/SusyAnaTools/Tools/data/allINone_bTagEff.root .
# endif
# 
# if (! -f allINone_ISRJets.root) then
#   ln -s ${SRC}/SusyAnaTools/Tools/ISR_Root_Files/allINone_ISRJets.root .
# endif
# 
# if (! -f ISRWeights.root) then
#   ln -s ${SRC}/SusyAnaTools/Tools/ISR_Root_Files/ISRWeights.root .
# endif
# 
# ## Pileup Reweighting
# if (! -f PileupHistograms_0121_69p2mb_pm4p6.root) then
#   ln -s ${SRC}/SusyAnaTools/Tools/data/PileupHistograms_0121_69p2mb_pm4p6.root .
# endif
# 
# ## W softdrop mass correction 
# if (! -f puppiCorr.root) then
#   ln -s ${SRC}/SusyAnaTools/Tools/data/puppiCorr.root .
# endif
# 
# if (! -f allINone_leptonSF_Moriond17.root) then
#   ln -s ${SRC}/SusyAnaTools/Tools/LeptonSF_Root_Files/allINone_leptonSF_Moriond17.root .
# endif

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
mkdir -p $STOPCFGDIR
# -o for overwrite softlinks if they exist
${SRC}/SusyAnaTools/Tools/scripts/getStopCfg.sh -o -t supp_cfg_tag -d $STOPCFGDIR

# -p give no error if directory exists
mkdir -p $TAGGERCFGDIR
# -o for overwrite softlinks if they exist
${SRC}/TopTagger/TopTagger/scripts/getTaggerCfg.sh -o -t MVAAK8_Tight_v1.2.1 -d $TAGGERCFGDIR
${SRC}/TopTagger/TopTagger/scripts/getTaggerCfg.sh -o -t Legacy_AK4Only_v0.1.1 -f $LEGTOPTAGGERFILE -d $TAGGERCFGDIR
${SRC}/TopTagger/TopTagger/scripts/getTaggerCfg.sh -o -t MVAAK8_Tight_noQGL_binaryCSV_v1.0.2 -f $SIMPLETOPTAGGERFILE -d $TAGGERCFGDIR




