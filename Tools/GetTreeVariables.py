# GetTreeVariables.py
# Caleb J. Smith
# February 7, 2019

# print TTree

import argparse
import ROOT

def main():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--input_file",  "-i", required=True,     help="input root file")
    parser.add_argument("--tree",       "-t", default="Events",   help="tree in root file that you wish to print")
    
    options          = parser.parse_args()
    input_file       = options.input_file
    tree             = options.tree
    tFile = ROOT.TFile.Open(input_file)
    tTree = tFile.Get(tree)
    output = tTree.Print()

if __name__ == "__main__":
    main()
