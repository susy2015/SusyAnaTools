#!/bin/csh

set maxNum = $1
set chunk = $2

#set configDir=${CMSSW_BASE}/src/UserCode/jetMETanalyzer/config/
#set template=${configDir}/cutList.txt

set currDir = `pwd`
set template = ${currDir}'/cutList.txt'
set head=cutList

#--- service variables
#set chunk = 100000
set     j = 0


set i=0
while ( ${i} < ${maxNum} )

if( ${i} < ${maxNum} ) then 
  echo "i :"  ${i}
   @ j =  ${i}
   @ j =  $j * $chunk
   echo "j :" ${j}
   if( ${chunk} != 0 ) then 
      cat ${template} | sed -e 's/\(initEntry\s*\)-1/\1'${j}'/' | sed -e 's/\(nevents\s*\) -1/\1'${chunk}'/' > ${head}_${i}.txt
   else 
      cat ${template} > ${head}_${i}.txt
   endif
endif

@ i = ${i} + "1"
end
