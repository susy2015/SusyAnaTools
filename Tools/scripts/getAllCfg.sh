#!/bin/bash

# getAllCfg.sh
# Caleb J. Smith
# October 12, 2018

# Required to run this script:
# - directory for StopCfg download
# - directory for TopTaggerCfg download

# Optional:
# - overwrite flag

# Initialize our own variables:

CMSSWSRCDIR=
STOPCFGDIR=
TAGGERCFGDIR=
OVERWRITE=

function print_help {
    echo ""
    echo "Usage:"
    echo "    getAllCfg.sh -c CMSSWSRCDIR -s STOPCFGDIR -t TAGGERCFGDIR [-o]"
    echo ""
    echo "Options:"
    echo "    -c CMSSWSRCDIR :   The CMSSW_BASE/src directory                   (required option)"
    echo "    -s STOPCFGDIR :    The directory for downloading StopCfg tag      (required option)"
    echo "    -t TAGGERCFGDIR :  The directory for downloading TopTaggerCfg tag (required option)"
    echo "    -o :               Overwrite the softlinks if they already exist  (optional)"
    echo ""
    echo "Description:"
    echo "    This script downloads the StopCfg and TopTaggerCfg files"
    echo "    and produces softlinks to this files in your current directory."
    echo "    This script should be run from the directory where the stop code will be run from."
    echo ""
}

while getopts "h?c:s:t:o" opt; do
    case "$opt" in
    h|\?)
        print_help
        exit 0
        ;;
    c)  CMSSWSRCDIR=$OPTARG
        ;;
    s)  STOPCFGDIR=$OPTARG
        ;;
    t)  TAGGERCFGDIR=$OPTARG
        ;;
    o) OVERWRITE="-o"
        ;;
    esac
done

shift $((OPTIND-1))

[ "$1" = "--" ] && shift

if [[ -z $CMSSWSRCDIR || -z $STOPCFGDIR || -z $TAGGERCFGDIR ]]
then
    print_help
    exit 0
fi

echo " - Running getAllCfg.sh"

##Checkout stop cfg files and supplementary files

# -p give no error if directory exists
mkdir -p $STOPCFGDIR
# set OVERWRITE = -o via options to overwrite softlinks if they exist
$CMSSWSRCDIR/SusyAnaTools/Tools/scripts/getStopCfg.sh $OVERWRITE -t CMSSW8028_2016_v1.0.2 -d $STOPCFGDIR

##Checkout latest toptagger cfg file 

TOPTAGGERFILE=TopTagger.cfg
SIMPLETOPTAGGERFILE=TopTagger_Simplified.cfg

# -p give no error if directory exists
mkdir -p $TAGGERCFGDIR
# set OVERWRITE = -o via options to overwrite softlinks if they exist
$CMSSWSRCDIR/TopTagger/TopTagger/scripts/getTaggerCfg.sh $OVERWRITE -t MVAAK8_Tight_v1.2.1 -f $TOPTAGGERFILE -d $TAGGERCFGDIR
$CMSSWSRCDIR/TopTagger/TopTagger/scripts/getTaggerCfg.sh $OVERWRITE -t MVAAK8_Tight_noQGL_binaryCSV_v1.0.2 -f $SIMPLETOPTAGGERFILE -d $TAGGERCFGDIR


