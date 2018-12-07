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

# colors for printing to terminal
class TextColor:
    red = "\033[91m"
    green = "\033[92m"
    blue = "\033[94m"
    end = "\033[0m"


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

def getNewSample(sample, weight_dict, neventsFile):
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
    if neventsFile:
        # return to the beginning of the file
        neventsFile.seek(0)
        num_matches = 0
        message = ""
        for nevent in neventsFile:
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
    # calculate events directly without nevents_file
    else:
        message = ""
        try:
            weights = weight_dict[name]
        except KeyError:
            print "KeyError for name: {0}".format(name)
            return newSample
        #print "sample_file: {0}".format(sample_file)
        new_pos_weights = weights["pos"]
        new_neg_weights = weights["neg"]
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
    parser.add_argument("--nevents_file",    "-e", default="",                     help="file containing number of events with weights")
    parser.add_argument("--input_file",      "-i", default="../sampleSets.cfg",    help="Existing SampleSets.cfg file")
    parser.add_argument("--output_file",     "-o", default="../sampleSets_v1.cfg", help="New SampleSets.cfg file to create")
    parser.add_argument("--threads",         "-t", default=4,                      help="Number of threads to use (default: 0)")
    parser.add_argument("--dataset_pattern", "-d", default = ".*",                 help="Regexp defining sampleSets to check (Defaults to all)")
    
    options = parser.parse_args()
    input_file = options.input_file
    nevents_file = options.nevents_file
    output_file  = options.output_file
    threads      = int(options.threads)
    dataset_pattern = options.dataset_pattern

    print "  samples file: {0}".format(input_file)
    print "  nevents file: {0}".format(nevents_file)
    print "  output file: {0}".format(output_file)

    try:
        inputSamples  = open(input_file, 'r')
    except:
        print TextColor.red + "ERROR opening input file {0}".format(input_file) + TextColor.end
        return
    try:
        outputSamples  = open(output_file, 'w')
    except:
        print TextColor.red + "ERROR opening output file {0}".format(output_file) + TextColor.end
        return
    
    ss = SampleSet(input_file)
    samples = [(name, s_file.replace("/eos/uscms", "root://cmseos.fnal.gov/")) for name, s_file in ss.sampleSetList()]
    
    weight_dict = {}
    
    neventsFile = False
    if nevents_file:
        # open nevents file if it is provided
        try:
            neventsFile = open(nevents_file, 'r')
        except:
            print TextColor.red + "ERROR opening nevents file {0}".format(nevents_file) + TextColor.end
            return
    else:
        # only get weights using getNEvts() if nevents_file is not provided by user
        for name, s_file in samples:
            if re.search(dataset_pattern, name):
                try:
                    nPos, nNeg = getNEvts(s_file, threads)
                    weight_dict[name] = {}
                    weight_dict[name]["pos"] = int(nPos)
                    weight_dict[name]["neg"] = int(nNeg)
                    print "{0}, {1}, Positive weights: {2}, Negative weights: {3}".format(name, s_file, weight_dict[name]["pos"], weight_dict[name]["neg"])
                except TypeError:
                    print "TypeError: name = {0}, s_file = {1}".format(name, s_file)
                    pass

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
        # if sample matches pattern, assume that it is a sample and get new sample to write to file
        if re.search(dataset_pattern, sample):
            newSample = getNewSample(sample, weight_dict, neventsFile)
            #print "newSample: {0}".format(newSample)
        outputSamples.write(newSample)

    inputSamples.close()
    outputSamples.close()
    if neventsFile: 
        neventsFile.close()

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

