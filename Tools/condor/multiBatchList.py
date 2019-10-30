# multiBatchList.py

# run batchList.py on multiple directories for a config files

from samples import SampleSet
import argparse
import optparse 
import subprocess
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
    # options
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--sampleSetsFile", "-s",   default = "",                           help="Sample sets config file")
    parser.add_argument("--run",            "-r",   default = False, action = "store_true", help="run batchList.py on all directories")
    options         = parser.parse_args()
    sampleSetsFile  = options.sampleSetsFile
    run             = options.run 

    if not os.path.exists(sampleSetsFile):
        print "The sample sets config file \"{0}\" does not exist.".format(sampleSetsFile)
        exit(1)

    dirs = getDirs(sampleSetsFile, "/eos/uscms")
    for d in dirs:
        if run:
            command = "python batchList.py -d {0} -l -c".format(d)
            print command
            subprocess.call(command, shell=True)
        else:
            print d


    
