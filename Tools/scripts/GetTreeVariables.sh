#!/bin/bash
# GetTreeVariables.sh

# input text file (listing root files) or root file
# output text files containting all variables from tree

outputName=
textFile=
rootFile=

# all options require values, hence we have a colon after each
while getopts o:t:r: option
do
    case "${option}"
    in
    o) outputName=${OPTARG};;
    t) textFile=${OPTARG};;
    r) rootFile=${OPTARG};;
    esac
done

# check for outpuut file
if [[ -z "$outputName" ]]
then
    echo "Please provide a output file name (wihtout .txt extension) using the -o option."
    exit 1
fi

# check for text or root file
if [[ -z "$textFile" && -z "$rootFile" ]]
then
    echo "Please provide either text file using the -t option or a root file using the -r option."
    exit 1
fi

# you cannot give both a text and a root file
if [[ !(-z "$textFile") && !(-z "$rootFile") ]]
then
    echo "Please do not provide a text file and a root file; you must use one or the other."
    exit 1
fi

if [[ !(-z "$textFile") ]]
then
    rootFile=$(head $textFile -n 1)
fi

outputFile="$outputName"_variables.txt
outputFileClean="$outputName"_variables_clean.txt

# output tree variables to file
python $CMSSW_BASE/src/SusyAnaTools/Tools/scripts/GetTreeVariables.py -i $rootFile > $outputFile
# cleaned output (one line per variable) to file
grep Br $outputFile > $outputFileClean

# number of variables (branches) in the tree
numVars=$(wc -l $outputFileClean | cut -f1 -d " ")
echo "There are $numVars variables in the tree." 
echo "Please see $outputFileClean for a complete list."
