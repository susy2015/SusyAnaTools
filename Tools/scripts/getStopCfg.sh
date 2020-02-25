#!/bin/bash

# getStopCfg.sh
# Caleb J. Smith
# September 27, 2018

# Required to run this script:
# - release tag from StopCfg repo that you wish to checkout

GITHUB_SUSY2015_URL=https://github.com/susy2015
REPO_NAME=StopCfg
RELEASE_URL="$GITHUB_SUSY2015_URL/$REPO_NAME/releases"

STARTING_DIR=$PWD
CFG_DIRECTORY=$PWD
TAG=
USE_PRE=
NO_SOFTLINK=
OVERWRITE=
VERBOSE=

# A POSIX variable
OPTIND=1    # Reset in case getopts has been used previously in the shell.

SUPP_CFG=supplementaryFiles.cfg
SETS_CFG_2016=sampleSets_PostProcessed_2016.cfg
SETS_CFG_2017=sampleSets_PostProcessed_2017.cfg
SETS_CFG_2018=sampleSets_PostProcessed_2018.cfg
SETS_CFG_RUN2=sampleSets_PostProcessed_Run2.cfg
COLL_CFG_2016=sampleCollections_2016.cfg
COLL_CFG_2017=sampleCollections_2017.cfg
COLL_CFG_2018=sampleCollections_2018.cfg
COLL_CFG_RUN2=sampleCollections_Run2.cfg
SETS_LINK_NAME_2016=$SETS_CFG_2016
SETS_LINK_NAME_2017=$SETS_CFG_2017
SETS_LINK_NAME_2018=$SETS_CFG_2018
SETS_LINK_NAME_RUN2=$SETS_CFG_RUN2
COLL_LINK_NAME_2016=$COLL_CFG_2016
COLL_LINK_NAME_2017=$COLL_CFG_2017
COLL_LINK_NAME_2018=$COLL_CFG_2018
COLL_LINK_NAME_RUN2=$COLL_CFG_RUN2
SUPP_FILE_DIR=supplementaryFiles
TARBALL=SUPP_FILE_TARBALL.tar.gz

function print_help {
    echo ""
    echo "Usage:"
    echo "    getStopCfg.sh -t RELEASE_TAG [-d checkout_directory] [-f cfg_filename] [-o] [-n] [-v]"
    echo ""
    echo "Options:"
    echo "    -t RELEASE_TAG :         This is the github release tag to check out (required option)"
    echo "    -d checkout_directory :  This is the directory where the configuration files will be downloaded to (default: .)"
    echo "    -i SETS_LINK_NAME_2016 : Specify this option to name the softlink to \"$SETS_CFG_2016\" something other than \"$SETS_LINK_NAME_2016\""
    echo "    -j SETS_LINK_NAME_2017 : Specify this option to name the softlink to \"$SETS_CFG_2017\" something other than \"$SETS_LINK_NAME_2017\""
    echo "    -k SETS_LINK_NAME_2018 : Specify this option to name the softlink to \"$SETS_CFG_2018\" something other than \"$SETS_LINK_NAME_2018\""
    echo "    -l SETS_LINK_NAME_RUN2 : Specify this option to name the softlink to \"$SETS_CFG_RUN2\" something other than \"$SETS_LINK_NAME_RUN2\""
    echo "    -w COLL_LINK_NAME_2016 : Specify this option to name the softlink to \"$COLL_CFG_2016\" something other than \"$COLL_LINK_NAME_2016\""
    echo "    -x COLL_LINK_NAME_2017 : Specify this option to name the softlink to \"$COLL_CFG_2017\" something other than \"$COLL_LINK_NAME_2017\""
    echo "    -y COLL_LINK_NAME_2018 : Specify this option to name the softlink to \"$COLL_CFG_2018\" something other than \"$COLL_LINK_NAME_2018\""
    echo "    -z COLL_LINK_NAME_RUN2 : Specify this option to name the softlink to \"$COLL_CFG_RUN2\" something other than \"$COLL_LINK_NAME_RUN2\""
    echo "    -e :                     Softlink pre-processed config files."
    echo "    -n :                     Download files without producing softlinks"
    echo "    -o :                     Overwrite the softlinks if they already exist"
    echo "    -v :                     increase verbosity: print more stuff... for those who like stuff"
    echo ""
    echo "Description:"
    echo "    This script automatically downloads the Stop search configuration files"
    echo "    and produces softlinks to these files in your current directory. This script should"
    echo "    be run from the directory where the stop code will be run from. Stop search "
    echo "    configuration releases can be browsed at $RELEASE_URL"
    echo ""
}


# Initialize our own variables:

while getopts "h?t:d:a:b:x:y:enov" opt; do
    case "$opt" in
    h|\?)
        print_help
        exit 0
        ;;
    t)  TAG=$OPTARG
        ;;
    d)  CFG_DIRECTORY=$OPTARG
        ;;
    i)  SETS_LINK_NAME_2016=$OPTARG
        ;;
    j)  SETS_LINK_NAME_2017=$OPTARG
        ;;
    k)  SETS_LINK_NAME_2018=$OPTARG
        ;;
    l)  SETS_LINK_NAME_RUN2=$OPTARG
        ;;
    w)  COLL_LINK_NAME_2016=$OPTARG
        ;;
    x)  COLL_LINK_NAME_2017=$OPTARG
        ;;
    y)  COLL_LINK_NAME_2018=$OPTARG
        ;;
    z)  COLL_LINK_NAME_RUN2=$OPTARG
        ;;
    e) USE_PRE=1
        ;;
    n) NO_SOFTLINK=1
        ;;
    o) OVERWRITE="-f"
        ;;
    v) VERBOSE=1
        ;;
    esac
done

shift $((OPTIND-1))

[ "$1" = "--" ] && shift

if [[ -z $TAG ]]
then
    print_help
    exit 0
fi

echo " - Running getStopCfg.sh"

# if USE_PRE is set, softlink pre-processed configs
if [[ ! -z $USE_PRE ]]
then
    SETS_CFG_2016=sampleSets_PreProcessed_2016.cfg
    SETS_CFG_2017=sampleSets_PreProcessed_2017.cfg
    SETS_CFG_2018=sampleSets_PreProcessed_2018.cfg
    SETS_CFG_RUN2=sampleSets_PreProcessed_Run2.cfg
    SETS_LINK_NAME_2016=$SETS_CFG_2016
    SETS_LINK_NAME_2017=$SETS_CFG_2017
    SETS_LINK_NAME_2018=$SETS_CFG_2018
    SETS_LINK_NAME_RUN2=$SETS_CFG_RUN2
fi

# get source directory of bash script
# used for "Easter Egg"...
SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

# check if OVERWRITE is set
# if OVERWRITE is set, ask user for confirmation before continuing
if [[ -z $OVERWRITE ]]
then
    # OVERWRITE is not set
    # continue
    echo "INFO: OVERWRITE is not set. Existing files and softlinks will not be replaced."
    echo "  To replace existing softlinks and files, use the OVERWRITE option -o."
else
    # OVERWRITE is set
    # ask user for confirmation before continuing
    echo   "INFO: OVERWRITE is set. Existing files and softlinks will be replaced."
    echo   "  Would you like to continue and replace existing files?"
    printf "  Enter (Y/y/yes/si/oui/ja/da) to continue, and anything else to quit: "
    read answer
    if [[ $answer == "ok" ]]
    then
        # "Easter Egg"...
        echo "  ______    __  ___" 
        echo " /  __  \\  |  |/  /" 
        echo "|  |  |  | |  '  / " 
        echo "|  |  |  | |    <  " 
        echo "|  \`--'  | |  .  \\ " 
        echo " \\______/  |__|\\__\\" 
        echo ""
        exit 0
    fi
    if [[ $answer == "Y" || $answer == "y" || $answer == "yes" || $answer == "si" || $answer == "oui" || $answer == "ja" || $answer == "da" ]]
    then
        echo " - Continuing..."
    else
        echo " - Quitting..."
        exit 0
    fi
fi

# Check that CFG_DIRECTORY is a directory
if [ ! -d $CFG_DIRECTORY ]
then
    echo "ERROR: $CFG_DIRECTORY is not a valid directory!"
    exit 1
fi

cd $CFG_DIRECTORY

# Download tag
if [ ! -d $REPO_NAME-$TAG ]
then
    echo " - Downloading this REPO-TAG: $REPO_NAME-$TAG"
    if [[ ! -z $VERBOSE ]] # True if VERBOSE is set
    then
        wget $GITHUB_SUSY2015_URL/$REPO_NAME/archive/$TAG.tar.gz
    else
        wget $GITHUB_SUSY2015_URL/$REPO_NAME/archive/$TAG.tar.gz &> /dev/null
    fi
    if [ -f $TAG.tar.gz ]
    then
        tar xzf $TAG.tar.gz
        rm $TAG.tar.gz
    else
        echo "ERROR: Failed to download $GITHUB_SUSY2015_URL/$REPO_NAME/archive/$TAG.tar.gz"
        echo "  Check that the REPO-TAG that you entered ($REPO_NAME-$TAG)"
        echo "  exists at $RELEASE_URL"
        echo "  Check your spelling... you may have a typo! Copy and paste are your friends."
        exit 1
    fi
else
    echo " - Skipping the download of the requested REPO-TAG because the directory "$REPO_NAME-$TAG" is already present"
fi


cd $REPO_NAME-$TAG
DOWNLOAD_DIR=$PWD

if [[ ! -z $VERBOSE ]] # True if VERBOSE is set
then
    echo "INFO: DOWNLOAD_DIR is $DOWNLOAD_DIR"
fi

if [[ -f $SUPP_CFG ]]
then
    if [[ -d $SUPP_FILE_DIR ]] 
    then
        echo " - Skipping the download of the supplementary files because the directory $DOWNLOAD_DIR/$SUPP_FILE_DIR already exists"
    else
        if [[ ! -f $TARBALL ]]
        then
            echo " - Downloading supplementary files"
            if [[ ! -z $VERBOSE ]] # True if VERBOSE is set
            then
                wget $GITHUB_SUSY2015_URL/$REPO_NAME/releases/download/$TAG/$TARBALL
            else
                wget $GITHUB_SUSY2015_URL/$REPO_NAME/releases/download/$TAG/$TARBALL &> /dev/null
            fi
            if [ ! -f $TARBALL ]
            then
                echo "ERROR: Tarball $DOWNLOAD_DIR/$TARBALL not found after attempted download"
                # we don't use supplementary files now
                #exit 1
            fi
        else
            echo "The tarball $DOWNLOAD_DIR/$TARBALL already exists"
        fi
        tar xzf $TARBALL
        rm $TARBALL
    fi
else
    echo "ERROR: The supplementary file config \"$DOWNLOAD_DIR/$SUPP_CFG\" does not exist"
fi


cd $STARTING_DIR

if [[ ! -z $VERBOSE ]] # True if VERBOSE is set
then
    echo "INFO: STARTING_DIR is $STARTING_DIR"
fi

# If OVERWRITE is set, make solftlinks (using ln) with -f
# If OVERWRITE is not set, make solftlinks (using ln)
# Pipe output to /dev/null

# Note: "> /dev/null 2>&1" does this:
# stdin  ==> fd 0      (default fd 0)
# stdout ==> /dev/null (default fd 1)
# stderr ==> stdout    (default fd 2)

# [[ -z STRING ]] : True if the length of "STRING" is zero, False if "STRING" has nonzero length
if [[ -z $NO_SOFTLINK ]]
then
    # create softlinks
    echo " - Creating softlinks to $REPO_NAME config files"
    ln $OVERWRITE -s $DOWNLOAD_DIR/$SETS_CFG_2016 $SETS_LINK_NAME_2016 > /dev/null 2>&1 
    ln $OVERWRITE -s $DOWNLOAD_DIR/$SETS_CFG_2017 $SETS_LINK_NAME_2017 > /dev/null 2>&1 
    ln $OVERWRITE -s $DOWNLOAD_DIR/$SETS_CFG_2018 $SETS_LINK_NAME_2018 > /dev/null 2>&1 
    ln $OVERWRITE -s $DOWNLOAD_DIR/$SETS_CFG_RUN2 $SETS_LINK_NAME_RUN2 > /dev/null 2>&1 
    ln $OVERWRITE -s $DOWNLOAD_DIR/$COLL_CFG_2016 $COLL_LINK_NAME_2016 > /dev/null 2>&1
    ln $OVERWRITE -s $DOWNLOAD_DIR/$COLL_CFG_2017 $COLL_LINK_NAME_2017 > /dev/null 2>&1
    ln $OVERWRITE -s $DOWNLOAD_DIR/$COLL_CFG_2018 $COLL_LINK_NAME_2018 > /dev/null 2>&1
    ln $OVERWRITE -s $DOWNLOAD_DIR/$COLL_CFG_RUN2 $COLL_LINK_NAME_RUN2 > /dev/null 2>&1
    # make list of files to tar
    if [[ -f $DOWNLOAD_DIR/$SUPP_CFG ]]
    then
        echo " - Creating softlinks to $REPO_NAME supplementary files"
        while read -r line || [[ -n "$line" ]]
        do
            SUPP_FILE="$line"
            if [ -f $DOWNLOAD_DIR/$SUPP_FILE_DIR/$SUPP_FILE ]
            then
                ln $OVERWRITE -s $DOWNLOAD_DIR/$SUPP_FILE_DIR/$SUPP_FILE $SUPP_FILE > /dev/null 2>&1
            fi
        done < $DOWNLOAD_DIR/$SUPP_CFG
    else
        echo "ERROR: Config file $DOWNLOAD_DIR/$SUPP_CFG not found"
    fi
fi


