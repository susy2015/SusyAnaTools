import argparse
import os

#USAGE: python createPostProcessCfg.py -c <config> -v <number>
#Example: python createPostProcessCfg.py -c sampleSets_PreProcessed_2016.cfg -v 2p6
#This will output a file named sampleSets_PostProcessed_2016.cfg

def my_process(args):
   #Create new config file with pre->post + version number directory replacement and text file name replacement
    if not args.input:
        raise Exception('No preprocessing config file given. Please specify a config file.')
    else:
        pre_config = os.path.abspath(args.input)
        # set output config name if specified
        if args.output:
            post_config = args.output
        else:
            post_config = os.path.abspath(args.input.replace("Pre","Post"))
      
    with open(post_config, "wt") as outfile:
        lines = open(pre_config).readlines()
        for line_ in lines:
            line = line_.strip()
            post_line = ""
            post_entry = []
            #Keep commented and empty lines for readability. This will mean that incomplete (commented out) datasets in the preprocessed config will be copied over as-is.
            if(len(line) <= 0 or line[0] == '#'):
                post_line += line_
            #Strip and split the rest to make replacements, then undo that for readability
            else:
               entry = line.split(",")
               stripped_entry = [ i.strip() for i in entry ]
               for i, num_entries in enumerate(stripped_entry):
                   if i == 1: #Update directory location
                       replaced_outdir = stripped_entry[1].replace("Pre","Post")
                       #update from Joe's addition to SubmitLPC.py. Cut off any subdirectories:
                       replaced_outdir = replaced_outdir.split("/")
                       nCut = 0
                       for n, s in enumerate(replaced_outdir):
                           if "PostProcessed" in s:
                               nCut = n
                               break
                       replaced_outdir = "/".join(replaced_outdir[:nCut + 1])
                       post_entry.append(replaced_outdir + "_v" + args.version)
                   elif i == 2: #Update text file name
                       post_entry.append(stripped_entry[0] + ".txt")
                   else: #Keep everything else the same
                       post_entry.append(stripped_entry[i])
               it = 0
               #When creating the new line, want to treat the last entry differently; it gets a newline rather than ", "
               while it < (len(post_entry) - 1):
                   post_line += (post_entry[it] + ", ")
                   it += 1
               post_line += (post_entry[-1] + "\n")
            #Now write the new line in the config
            outfile.write(post_line)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Creating postprocessed config file from preprocessed.')
    parser.add_argument('-i','--input',   default="",  help = "Input (typically preprocessing) config")
    parser.add_argument('-o','--output',  default="",  help = "Output (typically postprocessing) [default: use input filename, but replace Pre with Post]")
    parser.add_argument('-v','--version', default="2", help = "Round of postprocessing. Match to version number in SubmitLPC.py; this is the number appended to the eos directory name.")

    args = parser.parse_args()
    my_process(args)

