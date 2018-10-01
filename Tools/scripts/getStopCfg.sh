#!/bin/bash

# getStopCfg.sh
# Caleb J. Smith
# September 27, 2018

# Required to run this script:
# - release tag from StopCfg repo that you wish to checkout

GITHUB_SUSY2015_URL=https://github.com/susy2015
REPO_NAME=StopCfg

STARTING_DIR=$PWD
CFG_DIRECTORY=$PWD
TAG=
NO_SOFTLINK=
OVERWRITE=

# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

SUPP_CFG=supplementaryFiles.cfg
SETS_CFG=sampleSets.cfg
COLL_CFG=sampleCollections.cfg
SETS_LINK_NAME=$SETS_CFG
COLL_LINK_NAME=$COLL_CFG
SUPP_FILE_DIR=supplementaryFiles
TARBALL=SUPP_FILE_TARBALL.tar.gz

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
    echo "    This script automatically downloads the Stop search configuration files"
    echo "    and produces a softlink to this files in your corrent directory. This script should"
    echo "    be run from the directory where the stop code will be run from. Stop search "
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

#cd $REPO_NAME-$TAG
#DOWNLOAD_DIR=$PWD

# From previous TopTagger version of this script
# here just for reference
#
# MVAFILES=
# 
# if [ -f sampleSets.cfg ]
# then
#     MVAFILES=$(grep "modelFile" sampleSets.cfg | sed 's/[^"]*"\([^"]*\)"/\1/')
#     MISSING=
#     if [[ ! -z ${MVAFILES// } ]]
#     then
#         for MVAFILE in $MVAFILES; do
#             if [ ! -f $MVAFILE ]
#             then
#                 MISSING="yes"
#                 break
#             fi
#         done
#         if [[ ! -z ${MISSING// } ]]
#         then
#             MVATARBALL=MVAFILES.tar.gz
#             wget $GITHUB_SUSY2015_URL/$REPO_NAME/releases/download/$TAG/$MVATARBALL
#             if [ ! -f $MVATARBALL ]
#             then
#                 echo "MVA tarball "$MVATARBALL" not found!!!"
#                 MVATARBALL=${MVAFILES%.*}.tar.gz
#                 echo "trying "$MVATARBALL
#                 wget $GITHUB_SUSY2015_URL/$REPO_NAME/releases/download/$TAG/$MVATARBALL
#                 if [ ! -f $MVATARBALL ]
#                 then
#                     echo "MVA tarball "$MVATARBALL" not found!!!"
#                     exit 0
#                 fi
#             fi
#             tar xzf $MVATARBALL
#             rm $MVATARBALL
#         fi
#     fi
# fi

cd $REPO_NAME-$TAG
DOWNLOAD_DIR=$PWD

# note: we don't want this once SUPP_CFG is in the repo
#cd $STARTING_DIR

if [[ -f $SUPP_CFG ]]
then
    # note: we don't want this once SUPP_CFG is in the repo
    #cd $DOWNLOAD_DIR
    if [[ -d $SUPP_FILE_DIR ]] 
    then
        echo "The directory $DOWNLOAD_DIR/$SUPP_FILE_DIR already exists"
    else
        if [[ ! -f $TARBALL ]]
        then
            echo "Downloading supplementary files"
            wget $GITHUB_SUSY2015_URL/$REPO_NAME/releases/download/$TAG/$TARBALL
            if [ ! -f $TARBALL ]
            then
                echo "ERROR: Tarball $DOWNLOAD_DIR/$TARBALL not found after attempted download"
                exit 1
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

echo "STARTING_DIR: $STARTING_DIR"
echo "DOWNLOAD_DIR: $DOWNLOAD_DIR"


# [[ -z STRING ]] : True of the length if "STRING" is zero.
if [[ -z $NO_SOFTLINK ]]
then
    # create softlinks
    echo "Creating softlinks to config files"
    ln $OVERWRITE -s $DOWNLOAD_DIR/$SETS_CFG $SETS_LINK_NAME
    ln $OVERWRITE -s $DOWNLOAD_DIR/$COLL_CFG $COLL_LINK_NAME
    # make list of files to tar
    if [[ -f $DOWNLOAD_DIR/$SUPP_CFG ]]
    then
        echo "Creating softlinks to supplementary files"
        while read -r line || [[ -n "$line" ]]
        do
            SUPP_FILE="$line"
            ln $OVERWRITE -s $DOWNLOAD_DIR/$SUPP_FILE_DIR/$SUPP_FILE $SUPP_FILE
        done < $DOWNLOAD_DIR/$SUPP_CFG
    else
        echo "Config file $DOWNLOAD_DIR/$SUPP_CFG not found"
    fi
fi




