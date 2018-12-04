# updateSamples.py
# Author: Caleb Smith
# Date: 16-Aug-2018

# update samples config files
# - input samples config file with paths already updated
# - input file that is output of nEvts script with updated positive and negative weights
# - output new samples config file with updated weights
import argparse
import optparse 
import re
from shutil import copyfile

from samples import SampleSet
from nEvts import getNEvts

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

regex1 = re.compile('(.*): (.*)\t/eos.*Neg weigths = (.*), Pos weights = (.*)')
regex2 = re.compile('(.*),.*/eos.*')

# from samples.cc
#                                                       dataset, path,   file,   tree,   cross section, nevents+, nevents-, kfactor 
# int nMatches = sscanf(buf, "%s %s %s %s %f %f %f %f", cDSname, cFPath, cfName, cTPath, &f1,           &f2,      &f3,      &f4);
#
# string,  string, string, string, float,         float,    float,    float
# dataset, path,   file,   tree,   cross section, nevents+, nevents-, kfactor 
# cDSname, cFPath, cfName, cTPath, &f1,           &f2,      &f3,      &f4
#

def getNewSample(sample, sample_dict, threads, nevents_file):
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
    # only use nevents_file if it is provided (not empty string)
    if nevents_file:
        nevents = open(nevents_file, 'r')
        num_matches = 0
        message = ""
        for nevent in nevents:
            match = regex1.match(nevent)
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
    # calculate events directly without nevents_file
    else:
        message = ""
        try:
            sample_file = sample_dict[name]
        except KeyError:
            print "KeyError for name: {0}".format(name)
            return newSample
        #print "sample_file: {0}".format(sample_file)
        #print "threads: {0}".format(threads)
        try:
            new_pos_weights, new_neg_weights = getNEvts(sample_file, threads)
        except TypeError:
            print "TypeError for sample_file: {0}".format(sample_file)
            return newSample
        # compare integers, not strings!
        if old_neg_weights == new_neg_weights and old_pos_weights == new_pos_weights:
            message += " old and new weights are the same"
        else:
            message += " old and new weights are different"
        sample_list[-2] = str(new_neg_weights)
        sample_list[-3] = str(new_pos_weights)
        # print at the end
        message = "old weights: ({0}, {1}) new weights: ({2}, {3}) --- {4}".format(old_pos_weights, old_neg_weights, new_pos_weights, new_neg_weights, message)
        print "{0}: {1}".format(name, message)
        # replace commas and a space removed by split()
        # replace endline that was removed by strip()
        newSample = ", ".join(sample_list) + "\n"
    return newSample

def main1():
    # options
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--nevents_file", "-e", default="",                     help="file containing number of events with weights")
    parser.add_argument("--samples_file", "-s", default="../sampleSets.cfg",    help="Existing SampleSets.cfg file")
    parser.add_argument("--output_file",  "-o", default="../sampleSets_v1.cfg", help="New SampleSets.cfg file to create")
    parser.add_argument("--threads",      "-t", default=0,                      help="Number of threads to use (default: 0)")
    
    options = parser.parse_args()
    samples_file = options.samples_file
    nevents_file = options.nevents_file
    output_file  = options.output_file
    threads      = int(options.threads)

    print "samples file: {0}".format(samples_file)
    print "nevents file: {0}".format(nevents_file)
    print "output file: {0}".format(output_file)

    inputSamples  = open(samples_file, 'r')
    outputSamples = open(output_file, 'w')
    
    ss = SampleSet(options.samples_file)
    samples = [(name, s_file.replace("/eos/uscms", "root://cmseos.fnal.gov/")) for name, s_file in ss.sampleSetList()]
    sample_dict = {}
    for key, val in samples:
        sample_dict[key] = val

    # for each sample, find the correct weights
    for sample in inputSamples:
        newSample = sample
        # be careful: strip() removes spaces and endlines
        # we remove these for our conditions, but not for our new file
        sample = sample.strip()
        #print "sample: {0}".format(sample)
        # if line is empty (after being stripped), write original line (with endline) to file
        if not sample:
            outputSamples.write(newSample)
            continue
        # if first non-space character is # (comment), write original line (with endline) to file
        if sample[0] == "#":
            outputSamples.write(newSample)
            continue
        # otherwise, assume that it is a sample and get new sample to write to file
        newSample = getNewSample(sample, sample_dict, threads, nevents_file)
        outputSamples.write(newSample)

    inputSamples.close()
    outputSamples.close()

def main2():

    parser = optparse.OptionParser("usage: %prog [options]\n")

    parser.add_option ('-s', "--sampleSetCfg",        dest='sampleSetCfg',     type='string', default = "../sampleSets.cfg",    help="Existing SampleSet.cfg file to use")
    parser.add_option ('-n', "--newSampleSetCfg",     dest='newSampleSetCfg',  type='string', default = "../sampleSets_v1.cfg", help="New SampleSet.cfg file to create")
    parser.add_option ('-e', "--nevents_file",        default="",              help="file containing number of events with weights")
    parser.add_option ('-d', "--dataSetPattern",      dest='dataSetPattern',   type='string', default = ".*",                   help="Regexp defining sampleSets to check (Defaults to all)")
    parser.add_option ('-t', "--threads",             dest='threads',          type='int',    default = 4,                      help="Number of threads to use (default: 4)")

    options, args = parser.parse_args()

    ss = SampleSet(options.sampleSetCfg)
    samples = [(name, file.replace("/eos/uscms", "root://cmseos.fnal.gov/")) for name, file in ss.sampleSetList()]
    sample_dict = {}
    for key, val in samples:
        sample_dict[key] = val

    for name, file in samples:
        if re.search(options.dataSetPattern, name):
            try:
                nPos, nNeg = getNEvts(file, options.threads)
                print "%s, %s, Positive weights: %i, Negative weights: %i"%(name, file, nPos, nNeg)
            except TypeError:
                pass

if __name__ == "__main__":
    main1()

