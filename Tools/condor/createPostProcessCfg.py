#import batchList
#import nEvts
#import updateSamples
import argparse
import os


def my_process(args):
    #Currently just creates the new config file. The rest is still TODO, but would mostly be calling other scripts.

    #First create new text files for the relevant directory: Grab from preprocessed config file and make same pre->post + version number change from SubmitLPC.py
    

    #Next create new config file with same directory replacement and text file name replacement
    if (args.config == ""):
        raise Exception('No preprocessing config file given. Please specify a config file.')
    else:
      pre_config = os.path.abspath(args.config)
      post_config = os.path.abspath(args.config.replace("Pre","Post"))
      with open(post_config, "wt") as outfile:
        lines = open(pre_config).readlines()
        for line_ in lines:
            line = line_.strip()
            post_line = ""
            post_entry = []
            #Keep commented and empty lines for readability
            if(len(line) <= 0 or line[0] == '#'):
                post_line += line_
            #Strip and split the rest to make replacements, then undo that for readability
            else:
               entry = line.split(",")
               stripped_entry = [ i.strip() for i in entry ]
               for i, num_entries in enumerate(stripped_entry):
                   if i == 1:
                       replaced_outdir = stripped_entry[1].replace("Pre","Post")
                       post_entry.append(replaced_outdir + "_v" + args.version)
                   elif i == 2:
                       post_entry.append(stripped_entry[0] + ".txt")
                   else:
                       post_entry.append(stripped_entry[i])
               it = 0
               while it < (len(post_entry) - 1):
                   post_line += (post_entry[it] + ", ")
                   it += 1
               post_line += (post_entry[-1] + "\n")
            #Now write the new line
            outfile.write(post_line)


    #Then run nEvents on the new config file

    #Then run updateSamples.py

    #Print any lines in the update log that contain "different"


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Creating postprocessed config file from preprocessed.')
    parser.add_argument('-c','--config', default="", help = "Preprocessing config to create a postprocessing config file for")
    parser.add_argument('-v','--version', default="2", help = "Round of postprocessing. Match to version number in SubmitLPC.py; this is the number appended to the eos directory name.")
    parser.add_argument('-d','--delete_old_eos_txt', default=False, type=bool, help = "Use to delete old EOS text files at the postprocessing location. Note that batchList.py won't overwrite them.")

    args = parser.parse_args()
    my_process(args)
