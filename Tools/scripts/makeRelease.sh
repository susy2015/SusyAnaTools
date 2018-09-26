#!/bin/bash

GITHUB_SUSY2015_URL=https://github.com/susy2015
REPO_NAME=StopCfg
USER=susy2015

MVA_FILE_DIR=ModelFiles
TAG=
MESSAGE=
BRANCH=


# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

TOP_CFG_NAME=sampleSets.cfg

function print_help {
    echo "Usage:"
    echo "makeRelease.sh -t RELEASE_TAG -b BRANCH [-d MVA_FILE_DIR] [-m MESSAGE]"
    echo ""
    echo "Description: This script automatically makes a release of the SUSY Stop config files"
    echo ""
    echo "-t RELEASE_TAG :       This is the github release tag to check out (Required)"
    echo "-b BRANCH :            Git branch to base release off.  This must be pushed to github. (Required)"
    echo "-d MVA_FILE_DIR :      The folder where the MVA training file can be found (default ModelFiles)"
    echo "-m MESSAGE :           Commit message for tag (Default empty)"
}


# Initialize our own variables:

while getopts "h?t:d:m:b:" opt; do
    case "$opt" in
    h|\?)
        print_help
        exit 0
        ;;
    t) TAG=$OPTARG
        ;;
    d) MVA_FILE_DIR=$OPTARG
        ;;
    m) MESSAGE=$OPTARG
        ;;
    b) BRANCH=$OPTARG
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

if [[ -z $BRANCH ]]
then
    print_help
    exit 0
fi

exit 0

# #create release from tag 
# git push origin $BRANCH
# github-release release -u $USER -r $REPO_NAME -t $TAG -n $TAG -c $BRANCH -d "$MESSAGE"
# 
# if [ -f $TOP_CFG_NAME ]
# then
#     MVAFILES=$(git show $BRANCH:$TOP_CFG_NAME | grep "modelFile" | sed 's/[^"]*"\([^"]*\)"/\1/')
#     if [[ ! -z ${MVAFILES// } ]]
#     then
# 
#         #make tarball out of model file to save space 
#         MVATARBALL=MVAFILES.tar.gz
#         cd $MVA_FILE_DIR
#         for MVAFILE in $MVAFILES; do
#             if [ ! -f $MVAFILE ]
#             then
#                 echo "Model file "$MVAFILE" not found!"
#                 exit 0
#             fi
#         done
#         tar czf $MVATARBALL $MVAFILES
#         
#         #Upload tarball here
#         if [ -f $MVATARBALL ]
#         then
#             github-release upload -u $USER -r $REPO_NAME -t $TAG -n $MVATARBALL -f $MVATARBALL -l "MVA model files"
#         else
#             echo "Tarball "$MVATARBALL" not found!!!!!!!"
#         fi
#     fi
# fi
