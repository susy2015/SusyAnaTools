#!/bin/bash

GITHUB_SUSY2015_URL=https://github.com/susy2015
REPO_NAME=StopCfg

CFG_DIRECTORY=$PWD
TAG=
NO_SOFTLINK=
OVERWRITE=

# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

SETS_CFG=sampleSets.cfg
COLL_CFG=sampleCollections.cfg
SETS_LINK_NAME=$SETS_CFG
COLL_LINK_NAME=$COLL_CFG

function print_help {
    echo ""
    echo "Usage:"
    echo "    getStopCfg.sh -t RELEASE_TAG [-d checkout_directory] [-f cfg_filename] [-n]"
    echo ""
    echo "Options:"
    echo "    -t RELEASE_TAG :         This is the github release tag to check out (required option)"
    echo "    -d checkout_directory :  This is the directory where the configuration files will be downloaded to (default: .)"
    echo "    -s SETS_LINK_NAME :      Specify this option to name the softlink to \"$SETS_CFG\" something other than \"$SETS_LINK_NAME\""
    echo "    -c COLL_LINK_NAME :      Specify this option to name the softlink to \"$COLL_CFG\" something other than \"$COLL_LINK_NAME\""
    echo "    -o :                     Overwrite the softlinks if they already exist"
    echo "    -n :                     Download files without producing softlinks"
    echo ""
    echo "Description:"
    echo "    This script automatically downloads the Stop SUSY configuration files"
    echo "    and produces a softlink to this files in your corrent directory. This script should"
    echo "    be run from the directory where the tagger code will be run from. Stop SUSY "
    echo "    configuration releases can be browsed at https://github.com/susy2015/StopCfg/releases."
    echo ""
}


# Initialize our own variables:

while getopts "h?t:d:s:c:on" opt; do
    case "$opt" in
    h|\?)
        print_help
        exit 0
        ;;
    t)  TAG=$OPTARG
        ;;
    d)  CFG_DIRECTORY=$OPTARG
        ;;
    s)  SETS_LINK_NAME=$OPTARG
        ;;
    c)  COLL_LINK_NAME=$OPTARG
        ;;
    o) OVERWRITE="-f"
        ;;
    n) NO_SOFTLINK=NO
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

STARTING_DIR=$PWD

if [ ! -d $CFG_DIRECTORY ]
then
    echo "ERROR: $CFG_DIRECTORY is not a valid directory!"
    exit 1
fi

cd $CFG_DIRECTORY

if [ ! -d $REPO_NAME-$TAG ]
then
    echo "Checking out tag: " $TAG
    wget $GITHUB_SUSY2015_URL/$REPO_NAME/archive/$TAG.tar.gz
    if [ -f $TAG.tar.gz ]
    then
        tar xzf $TAG.tar.gz
        rm $TAG.tar.gz
    else
        echo "Failed to get " $TAG.tar.gz
        exit 0
    fi
else
    echo "Directory "$REPO_NAME-$TAG" already present"
fi

cd $REPO_NAME-$TAG
DOWNLOAD_DIR=$PWD

MVAFILES=

if [ -f sampleSets.cfg ]
then
    MVAFILES=$(grep "modelFile" sampleSets.cfg | sed 's/[^"]*"\([^"]*\)"/\1/')
    MISSING=
    if [[ ! -z ${MVAFILES// } ]]
    then
        for MVAFILE in $MVAFILES; do
            if [ ! -f $MVAFILE ]
            then
                MISSING="yes"
                break
            fi
        done
        if [[ ! -z ${MISSING// } ]]
        then
            MVATARBALL=MVAFILES.tar.gz
            wget $GITHUB_SUSY2015_URL/$REPO_NAME/releases/download/$TAG/$MVATARBALL
            if [ ! -f $MVATARBALL ]
            then
                echo "MVA tarball "$MVATARBALL" not found!!!"
                MVATARBALL=${MVAFILES%.*}.tar.gz
                echo "trying "$MVATARBALL
                wget $GITHUB_SUSY2015_URL/$REPO_NAME/releases/download/$TAG/$MVATARBALL
                if [ ! -f $MVATARBALL ]
                then
                    echo "MVA tarball "$MVATARBALL" not found!!!"
                    exit 0
                fi
            fi
            tar xzf $MVATARBALL
            rm $MVATARBALL
        fi
    fi
fi

cd $STARTING_DIR

# [[ -z STRING ]] : True of the length if "STRING" is zero.
if [[ -z $NO_SOFTLINK ]]
then
    ln $OVERWRITE -s $DOWNLOAD_DIR/$SETS_CFG $SETS_LINK_NAME
    ln $OVERWRITE -s $DOWNLOAD_DIR/$COLL_CFG $COLL_LINK_NAME
    if [[ ! -z ${MVAFILES// } ]] 
    then
        for MVAFILE in $MVAFILES; do
            ln $OVERWRITE -s $DOWNLOAD_DIR/$MVAFILE $MVAFILE
        done
    fi
fi




