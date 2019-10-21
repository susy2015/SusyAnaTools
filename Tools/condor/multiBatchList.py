# multiBatchList.py

# run batchList.py on multiple directories for a config files

from samples import SampleSet
import optparse 
import os
import sys
import re

# get list of unique directories
# strip off prefix and file name
def getDirs(sampleSetsFile, prefix):
    dirs = []
    ss = SampleSet(sampleSetsFile)
    for ds in ss.sampleSetList():
        path = ds[1]
        path = re.sub(prefix, "", path) 
        path = re.sub("/[A-Za-z0-9_.-]*\.txt$", "", path) 
        if path not in dirs:
            dirs.append(path)
            #print path
    return dirs

if __name__ == "__main__":

    parser = optparse.OptionParser("usage: %prog [options]\n")
    parser.add_option ('-s',      dest='sampleSetsFile',       type='string',    default = "",      help="Sample sets config file")
    options, args = parser.parse_args()
    sampleSetsFile = options.sampleSetsFile

    if not os.path.exists(sampleSetsFile):
        print "The sample sets config file \"{0}\" does not exist.".format(sampleSetsFile)
        exit(1)

    dirs = getDirs(sampleSetsFile, "/eos/uscms")
    for d in dirs:
        print d

    
