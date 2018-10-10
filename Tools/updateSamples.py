# updateSamples.py
# Author: Caleb Smith
# Date: 16-Aug-2018

# update samples config files
# - input samples config file with paths already updated
# - input file that is output of nEvts script with updated positive and negative weights
# - output new samples config file with updated weights
import argparse
import re
from shutil import copyfile

# regex
# regular expression to get sample name and positive/negative weights
#
# Processing file(s): SMS-T2tt_fastsim_mStop-150to250 /eos/uscms/store/user/lpcsusyhad/Stop_production/Summer16_80X_Jan_2017_Ntp_v12X/SMS-T2tt_mStop-150to250_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt  Neg weigths = 0, Pos weights = 32283695
# (.*): (.*)\t/eos.*Neg weigths = (.*), Pos weights = (.*)
# Groups:
#  1: 'Processing file(s)'                  - beginning
#  2: 'SMS-T2tt_fastsim_mStop-150to250'     - dataset name   
#  3: '0'                                   - number of negative weights
#  4: '32283695'                            - number of positive weights

regex = re.compile('(.*): (.*)\t/eos.*Neg weigths = (.*), Pos weights = (.*)')

# from samples.cc
#                                                       dataset, path,   file,   tree,   cross section, nevents+, nevents-, kfactor 
# int nMatches = sscanf(buf, "%s %s %s %s %f %f %f %f", cDSname, cFPath, cfName, cTPath, &f1,           &f2,      &f3,      &f4);
#
# string,  string, string, string, float,         float,    float,    float
# dataset, path,   file,   tree,   cross section, nevents+, nevents-, kfactor 
# cDSname, cFPath, cfName, cTPath, &f1,           &f2,      &f3,      &f4
#

def getNewSample(sample, nevents_file):
    # be careful: strip() removes spaces and endlines
    sample_list = list(x.strip() for x in sample.split(','))
    name = sample_list[0] 
    #print "sample_list: {0}".format(sample_list)
    # values are floats, but we want integers
    newSample = ", ".join(sample_list) + "\n"
    if "Data" in sample or "data" in sample:
        print "Skipping data sample: {0}".format(name)
        return newSample
    try:
        old_neg_weights = int(float(sample_list[-2]))
    except:
        print "ERROR: sample_list format is not correct; {0} is not a number".format(sample_list[-2])
        print "ERROR: Skipping sample_list = {0}".format(sample_list)
        return newSample
    try:
        old_pos_weights = int(float(sample_list[-3]))
    except:
        print "ERROR: sample_list format is not correct; {0} is not a number".format(sample_list[-3])
        print "ERROR: Skipping sample_list = {0}".format(sample_list)
        return newSample
    nevents = open(nevents_file, 'r')
    num_matches = 0
    message = ""
    for nevent in nevents:
        match = regex.match(nevent)
        if not match:
            continue
        if name == match.group(2):
            num_matches += 1
            # values are floats, but we want integers
            new_neg_weights = int(float(match.group(3)))
            new_pos_weights = int(float(match.group(4)))
            message = ""
            # compare integers, not strings!
            if old_neg_weights == new_neg_weights and old_pos_weights == new_pos_weights:
                message += " old and new weights are the same"
            else:
                message += " old and new weights are different"
            sample_list[-2] = str(new_neg_weights)
            sample_list[-3] = str(new_pos_weights)
            # print at the end
            message = "old weights: ({0}, {1}) new weights: ({2}, {3}) --- {4}".format(old_pos_weights, old_neg_weights, new_pos_weights, new_neg_weights, message)
    print "{0} has {1} match(es) in nevents file: {2}".format(name, num_matches, message)
    if num_matches == 0:
        print "WARNING: no matches found; {0} has {1} matches".format(name, num_matches)
    elif num_matches > 1:
        print "WARNING: more than one match found; {0} has {1} matches".format(name, num_matches)
    # replace commas and a space removed by split()
    # replace endline that was removed by strip()
    newSample = ", ".join(sample_list) + "\n"
    nevents.close()
    return newSample

def main():
    print "producing new samples config files"

    # options
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--samples_file", "-s", default="sampleSets.cfg",    help="file containing sample sets")
    parser.add_argument("--nevents_file", "-e", default="nEvents.txt",       help="file containing number of events with weights")
    parser.add_argument("--output_file",  "-o", default="sampleSets_v2.cfg", help="output file with updated sample sets")
    options = parser.parse_args()
    samples_file = options.samples_file
    nevents_file = options.nevents_file
    output_file  = options.output_file

    print "samples file: {0}".format(samples_file)
    print "nevents file: {0}".format(nevents_file)
    print "output file: {0}".format(output_file)

    print "Copying {0} to sampleSets_v1.cfg".format(samples_file)
    copyfile(samples_file, "sampleSets_v1.cfg")

    samples = open(samples_file, 'r')
    newSamples = open(output_file, 'w')

    # for each sample, find the correct weights
    for sample in samples:
        newSample = sample
        # be careful: strip() removes spaces and endlines
        # we remove these for our conditions, but not for our new file
        sample = sample.strip()
        #print "sample: {0}".format(sample)
        # if line is empty (after being stripped), write original line (with endline) to file
        if not sample:
            newSamples.write(newSample)
            continue
        # if first non-space character is # (comment), write original line (with endline) to file
        if sample[0] == "#":
            newSamples.write(newSample)
            continue
        # otherwise, assume that it is a sample and get new sample to write to file
        newSample = getNewSample(sample, nevents_file)
        newSamples.write(newSample)

    samples.close()
    newSamples.close()


if __name__ == "__main__":
    main()

