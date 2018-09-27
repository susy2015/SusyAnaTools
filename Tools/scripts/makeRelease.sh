#!/bin/bash
#
# makeRelease.sh
# Caleb J. Smith
# September 27, 2018
# 
# Required to run this script:
# - StopCfg repo
# - StopCfg branch to base release off
# - directory containing supplementary files (root files, etc)
# - config file containing one supplementary file name per line
# - git token: source a script that does "export GITHUB_TOKEN=<git_token_created_on_github>"
# - github-release command: softlink /uscms/home/caleb/bin/github-release -> /uscms/home/pastika/bin/github-release
# - put github-release in path: export PATH="$PATH:~caleb/bin"


GITHUB_SUSY2015_URL=https://github.com/susy2015
REPO_NAME=StopCfg
USER=susy2015

SUPP_FILE_CFG=supplementaryFiles.cfg
SUPP_FILE_DIR=supplementaryFiles
SUPP_FILES=
TARBALL=SUPP_FILE_TARBALL.tar.gz
TAG=
MESSAGE=
BRANCH=


echo "GITHUB_TOKEN: $GITHUB_TOKEN"

# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

STOP_CFG_NAME=sampleSets.cfg

function print_help {
    echo "Usage:"
    echo "makeRelease.sh -t RELEASE_TAG -b BRANCH [-d SUPP_FILE_DIR] [-m MESSAGE]"
    echo ""
    echo "Description: This script automatically makes a release of the SUSY Stop config files"
    echo ""
    echo "-t RELEASE_TAG :       This is the github release tag to check out (Required)"
    echo "-b BRANCH :            Git branch to base release off. This branch must exist. It will be pushed to github. (Required)"
    echo "-d SUPP_FILE_DIR :     The folder where the supplemental training file can be found (default $SUPP_FILE_DIR)"
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
    d) SUPP_FILE_DIR=$OPTARG
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


# make list of files to tar
while read -r line || [[ -n "$line" ]]
do
    #echo "$line"
    # -z: returns true if length of string is zero
    # this just makes sure there is not leading space
    if [[ -z $SUPP_FILES ]]
    then
        SUPP_FILES="$SUPP_FILE_DIR/$line"
    else
        SUPP_FILES="$SUPP_FILES $SUPP_FILE_DIR/$line"
    fi
done < $SUPP_FILE_CFG

echo "SUPP_FILES: $SUPP_FILES"

for FILE in $SUPP_FILES
do
    if [ ! -f $FILE ]
    then
        echo "ERROR: File not found: $FILE"
        exit 1
    fi
done

echo "Making tarball: $TARBALL"
tar cfz $TARBALL $SUPP_FILES

if [ -f $TARBALL ]
then
    # creating branch and release
    echo "Creating branch and release"
    git push origin $BRANCH
    github-release release -u $USER -r $REPO_NAME -t $TAG -n $TAG -c $BRANCH -d "$MESSAGE"

    #Upload tarball here
    echo "Uploading tarball"
    github-release upload -u $USER -r $REPO_NAME -t $TAG -n $TARBALL -f $TARBALL -l "Stop supplementary files"
else
    echo "ERROR: Tarball not found: $TARBALL"
    exit 1
fi


# exit at the end; next section should be commented out
exit 0


# From TopTagger version of makeRelease.sh script
# Just here for reference
#
# #create release from tag 
# git push origin $BRANCH
# github-release release -u $USER -r $REPO_NAME -t $TAG -n $TAG -c $BRANCH -d "$MESSAGE"
# 
# if [ -f $STOP_CFG_NAME ]
# then
#     MVAFILES=$(git show $BRANCH:$STOP_CFG_NAME | grep "modelFile" | sed 's/[^"]*"\([^"]*\)"/\1/')
#     if [[ ! -z ${MVAFILES// } ]]
#     then
# 
#         #make tarball out of model file to save space 
#         MVATARBALL=MVAFILES.tar.gz
#         cd $SUPP_FILE_DIR
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



