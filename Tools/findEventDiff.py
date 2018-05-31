import re

with open("samples.cc") as f1:
    storedDS = {}
    for line in f1:
        if("lumi" in line and "addSample" in line): 
            line1 = line.split(",")
            line2 = line1[1].split("\"")
            dataSetSamples=line2[1]
            nEventsSamples=line1[5]
            storedDS[dataSetSamples] = nEventsSamples
            #print dataSetSamples + "  " + nEventsSamples

with open("numberOfEvents_V8.log") as f2:
    newDS = {}    
    for line in f2:
        if("Processing" in line):        
            line3 = line.split("/")
            line4 = line3[-1].split()
            dataSet = line4[0]
            if line4[4] == '0,':
                nEvents = " " + line4[8]
            else:
                nEvents = line4[8] + " - " + line4[4]
                nEvents = " " + nEvents[:-1]
            newDS[dataSet] = nEvents 
            #print dataSet + "  " + nEvents

numberNewDS = 0
numberStoredDS = 0
notInStoredDS = 0
notInNewDS = 0
for key in newDS:
    numberNewDS += 1 
    if key in storedDS.keys():
        continue
    else:        
        print "Data set not in samples.cc: %-80s nEvents =%s" % (key,newDS[key])
        notInStoredDS += 1

print "------------------------------------------------------------------------------------------------------------------------------------------------------------------------"

for key in storedDS:
    numberStoredDS += 1 
    if key in newDS.keys():
        continue
    else:
        print "Data set not in new nTuple: " + key
        notInNewDS += 1

print "------------------------------------------------------------------------------------------------------------------------------------------------------------------------"

print "Total Stored DS = " + str(numberStoredDS) + "  Total New DS = " + str(numberNewDS)
print "Total DS not in stored DS but in new = " + str(notInStoredDS)
print "Total DS not in new DS but in old =    " + str(notInNewDS) 

print "------------------------------------------------------------------------------------------------------------------------------------------------------------------------"

passed = 0 
failed = 0
for key1 in storedDS:
    for key2 in newDS:
        if key1 == key2:
            if str(newDS[key1]) == str(storedDS[key1]):
                passed += 1
                continue
            else:
                #print "%i / %i" % (len(str(storedDS[key1])), len(str(newDS[key1])))
                print "Number of events Old/New | %20s | %20s | Data Set | %s" % (storedDS[key1],newDS[key1],key1) 
                failed += 1
                continue

print "------------------------------------------------------------------------------------------------------------------------------------------------------------------------"

print "Total passed = " + str(passed) + " Total failed = " + str(failed) + " Total checked = " + str(passed + failed)
print "Total checked + Total DS not in stored = " + str(passed + failed + notInStoredDS)
print "Total checked + Total DS not in new =    " + str(passed + failed + notInNewDS)
