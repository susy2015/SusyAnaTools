#!/bin/csh

set maxNum=$1
set chunk=$2
set perNum=$3

./fnalMakeConfigs.csh ${maxNum} ${chunk}
./fnalMakeSplitInputs.pl ${perNum}

set i=0

while ( ${i} < ${maxNum} )
./fnalCondorPrep.sh ${i}
@ i = ${i} + "1"
end

sleep 1s

set i=0

while ( ${i} < ${maxNum} )
condor_submit run_${i}.sh   
@ i = ${i} + "1"
end
