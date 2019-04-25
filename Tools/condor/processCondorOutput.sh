#!/bin/bash
# processCondorOutput.sh

# process test files from condor

directory=

# all options require values, hence we have a colon after each
while getopts d: option
do
    case "${option}"
    in
    d) directory=${OPTARG};;
    esac
done

# check for outpuut file
if [[ -z "$directory" ]]
then
    echo "Please provide a directory containing files returned by condor using the -d option."
    exit 1
fi

if [ ! -d "$directory" ]
then
    echo "The directory $directory does not exist."
    exit 1
fi

cd $directory
mkdir output
mv *.txt output
mv output/condor_submit.txt .
numFiles=$(ls output/*.txt | wc -l | cut -f1 -d " ")

echo "Number of files returned from condor: $numFiles"

cat output/*.txt > nEvents.txt

