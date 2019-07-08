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

from samples import SampleSet
from nEvts import getNEvts

# colors for printing to terminal
class TextColor:
    red = "\033[91m"
    green = "\033[92m"
    yellow = "\033[93m"
    blue = "\033[94m"
    end = "\033[0m"

def updateXSection(sample, xsection_dict):
    # be careful: strip() removes spaces and endlines
    sample_list = list(x.strip() for x in sample.split(','))
    name = sample_list[0] 
    if name[0] == "#":
        name = name[1:].strip()
    
    new_xsection = -1.0
    newSample = ", ".join(sample_list) + "\n"
    
    try:
        new_xsection = xsection_dict[name]
    except KeyError:
        print "KeyError for name: {0}".format(name)
        return newSample

    sample_list[4] = new_xsection
    newSample = ", ".join(sample_list) + "\n"
    return newSample


def getNewSample(sample, weight_dict, neventsFile):
    # be careful: strip() removes spaces and endlines
    sample_list = list(x.strip() for x in sample.split(','))
    name = sample_list[0] 
    
    ###############################################################################
    # WARNING: Do not change regex unless you also update nEvts.C and nEvts.py!!! #
    ###############################################################################
    regex = re.compile(name+'.*Positive weights: (.*), Negative weights: (.*)')
    # values are floats, but we want integers
    newSample = ", ".join(sample_list) + "\n"
    if "Data" in sample:
        print "Skipping data sample: {0}".format(name)
        return newSample
    try:
        old_neg_weights = int(float(sample_list[-2]))
    except ValueError:
        print TextColor.red + "ERROR: sample_list format is not correct; {0} is not a number".format(sample_list[-2]) + TextColor.end
        print "       Skipping sample_list = {0}".format(sample_list)
        return newSample
    try:
        old_pos_weights = int(float(sample_list[-3]))
    except ValueError:
        print TextColor.red + "ERROR: sample_list format is not correct; {0} is not a number".format(sample_list[-3]) + TextColor.end
        print "       Skipping sample_list = {0}".format(sample_list)
        return newSample
    # only use nevents_file if it is provided (not empty string)
    if neventsFile:
        # return to the beginning of the file
        neventsFile.seek(0)
        num_matches = 0
        message = ""
        for line in neventsFile:
            match = regex.match(line)
            if not match:
                continue
            num_matches += 1
            # values are floats, but we want integers
            new_pos_weights = int(float(match.group(1)))
            new_neg_weights = int(float(match.group(2)))
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
        if num_matches == 0:
            print TextColor.yellow + "WARNING: no matches found in nEvents file; {0} has {1} matches".format(name, num_matches) + TextColor.end
        elif num_matches > 1:
            print TextColor.yellow + "WARNING: more than one match found in nEvents file; {0} has {1} matches".format(name, num_matches) + TextColor.end
        else:
            print "{0} has {1} match(es) in nEvents file: {2}".format(name, num_matches, message)
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

def main():
    # options
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--nevents_file",    "-e", default="",                             help="file containing number of events that will be put into output file")
    parser.add_argument("--xsection_file",   "-x", default="",                             help="config file with cross sections that will be put into output file")
    parser.add_argument("--input_file",      "-i", default="../sampleSets.cfg",            help="Existing SampleSets.cfg file")
    parser.add_argument("--output_file",     "-o", default="../sampleSets_v1.cfg",         help="New SampleSets.cfg file to create")
    parser.add_argument("--threads",         "-t", default=4,                              help="Number of threads to use")
    parser.add_argument("--dataset_pattern", "-d", default = ".*",                         help="Regexp defining sampleSets to check")
    parser.add_argument("--legacy",          "-l", default = False, action = "store_true", help="Use legacy ntuples instead of Nano AOD ntuples")
    parser.add_argument("--verbose",         "-v", default = False, action = "store_true", help="verbose flag to print more things")
    
    options          = parser.parse_args()
    nevents_file     = options.nevents_file
    xsection_file    = options.xsection_file
    input_file       = options.input_file
    output_file      = options.output_file
    threads          = int(options.threads)
    dataset_pattern  = options.dataset_pattern
    legacy           = options.legacy
    verbose          = options.verbose

    if verbose:
        print "Settings:"
        print "  nevents file: {0}".format(nevents_file)
        print "  xsection file: {0}".format(xsection_file)
        print "  input file: {0}".format(input_file)
        print "  output file: {0}".format(output_file)
        print "  use legacy ntuples: {0}".format(legacy)

    try:
        inputSamples  = open(input_file, 'r')
    except IOError:
        print TextColor.red + "ERROR opening input file {0}".format(input_file) + TextColor.end
        return
    try:
        outputSamples  = open(output_file, 'w')
    except IOError:
        print TextColor.red + "ERROR opening output file {0}".format(output_file) + TextColor.end
        return
    
    ss = SampleSet(input_file)
    samples = [(name, s_file.replace("/eos/uscms", "root://cmseos.fnal.gov/")) for name, s_file in ss.sampleSetList()]
    
    xsection_dict = {}
    weight_dict = {}

    # initialize files to false
    neventsFile = False
    xsectionFile = False

    # get nevents from file
    if nevents_file:
        # open nevents file if it is provided
        try:
            neventsFile = open(nevents_file, 'r')
        except IOError:
            print TextColor.red + "ERROR opening nevents file {0}".format(nevents_file) + TextColor.end
            return
    
    # get xsections from file
    elif xsection_file:
        # open xsection file if it is provided
        try:
            xsectionFile = open(xsection_file, 'r')
            for line in xsectionFile:
                # be careful: strip() removes spaces and endlines
                line = line.strip()
                if line:
                    #print line
                    sample_list = list(x.strip() for x in line.split(','))
                    if len(sample_list) > 5:
                        name = sample_list[0] 
                        if name[0] == "#":
                            name = name[1:].strip()
                        xsection_dict[name] = sample_list[4]
        except IOError:
            print TextColor.red + "ERROR opening xsection file {0}".format(xsection_file) + TextColor.end
            return

    # calculate nevents
    else:
        # only get weights using getNEvts() if nevents_file is not provided by user
        for name, s_file in samples:
            if re.search(dataset_pattern, name):
                try:
                    nPos, nNeg = getNEvts(s_file, threads, legacy)
                    weight_dict[name] = {}
                    weight_dict[name]["pos"] = int(nPos)
                    weight_dict[name]["neg"] = int(nNeg)
                    if verbose:
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
        # if xsection_file is provided, replace cross sections
        if xsection_file:
            newSample = updateXSection(sample, xsection_dict)
            outputSamples.write(newSample)
            continue

        # if first non-space character is # (comment), write original line (with endline) to file
        if sample[0] == "#":
            outputSamples.write(newSample)
            continue
        # if sample matches pattern, assume that it is a sample and get new sample to write to file
        if re.search(dataset_pattern, sample):
            newSample = getNewSample(sample, weight_dict, neventsFile)
            if verbose:
                print "newSample: {0}".format(newSample)
        outputSamples.write(newSample)

    inputSamples.close()
    outputSamples.close()
    if neventsFile: 
        neventsFile.close()
    if xsectionFile: 
        xsectionFile.close()
    print TextColor.green + "Config file updates are finished!" + TextColor.end


if __name__ == "__main__":
    main()

