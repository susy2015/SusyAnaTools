#!/bin/bash
# GetTreeVariables.sh

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

#echo $outputName
#echo $textFile
#echo $rootFile

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

#outputName=ntupleVariables/Summer16_80X_v2_NanAOD_PreProcessed_MC_GJets
outputFile="$outputName"_variables.txt
outputFileClean="$outputName"_variables_clean.txt
#rootFile="root://cmseos.fnal.gov//store/user/caleb/NanoAOD/ZJetsToNuNu/prod2017MC_NANO_1_Skim.root"
#rootFile="root://cmseos.fnal.gov//store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/PostProcessed_10Feb2019_v1//Data_SingleMuon_2016/Data_SingleMuon_2016_0.root"
#rootFile="root://cmseos.fnal.gov//eos/uscms/store/user/lpcsusyhad/Stop_production/Summer16_80X_v2_NanAOD_MC/PostProcess_v1//GJets_HT-100To200/GJets_HT-100To200_0.root"
#rootFile="root://cmseos.fnal.gov//eos/uscms/store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/PreProcessed_10Feb2019//SingleMuon/2016_Data_Run2016B-17Jul2018_ver2-v1/190211_053245/0000/prod2016Data_NANO_1-1.root"
#rootFile="root://cmseos.fnal.gov//store/user/lpcsusyhad/Stop_production/Summer16_80X_v2_NanAOD_MC//GJets_DR-0p4_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/2016_MC_RunIISummer16MiniAODv2-PUMoriond17_qcut19_80X_v6-v1/190212_044117/0000/prod2016MC_NANO_1-1.root"

# output tree variables to file
#echo "python GetTreeVariables.py -i $rootFile > $outputFile"
python GetTreeVariables.py -i $rootFile > $outputFile
# cleaned output (one line per variable) to file
grep Br $outputFile > $outputFileClean

# number of variables (branches) in the tree
numVars=$(wc -l $outputFileClean | cut -f1 -d " ")
echo "There are $numVars variables in the tree." 
echo "Please see $outputFileClean for a complete list."
