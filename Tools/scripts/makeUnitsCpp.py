#!/usr/bin/env python
# coding: utf-8
import json
import sys

variables = [
("Pass_Baseline",   "bool"),
("Pass_QCDCR",      "bool"),
("Pass_LepCR",      "bool"),
("Pass_PhoCR",      "bool"),
("HighDM",          "bool"),
("LowDM",           "bool"),
("nb",              "int"),
("mtb",             "float"),
("ptb",             "float"),
("MET",             "float"),
("nSoftB",          "int"),
("njets",           "int"),
("ISRpt",           "float"),
("HT",              "float"),
("nres",            "int"),
("ntop",            "int"), 
("nw",              "int")
]

cut_defs = {
    "qcdcr" : ["Pass_QCDCR"],
    "lepcr" : ["Pass_LepCR"],
    "phocr" : ["Pass_PhoCR"],
    "hm" : ["HighDM"],
    "lm" : ["LowDM"],
    "nb0"    : ["nb == 0"],
    "nb1"    : ["nb == 1"],
    "nbgeq1" : ["nb >= 1"],
    "nb2"    : ["nb >= 2"],
    "nbeq2"  : ["nb == 2"],
    "nb3"    : ["nb >= 3"],
    "nivf0" : ["nSoftB == 0"],
    "nivf1" : ["nSoftB >= 1"],
    "lowptisr"  : ["ISRpt >= 300", "ISRpt < 500"],
    "medptisr"  : ["ISRpt >= 300"],
    "highptisr" : ["ISRpt >= 500"],
    "nj2to5" : ["njets >= 2", "njets <= 5"],
    "nj6"    : ["njets >= 6"],
    "nj7"    : ["njets >= 7"],
    "lowmtb"  : ["mtb < 175"],
    "highmtb" : ["mtb >= 175"],
    "lowptb"    : ["ptb < 40"],
    "medptb"    : ["ptb >= 40", "ptb < 70"],
    "highptb"   : ["ptb >= 70"],
    "lowptb12"  : ["ptb < 80"],
    "medptb12"  : ["ptb >= 80", "ptb < 140"],
    "highptb12" : ["ptb >= 140"],
    "nt0"    : ["ntop == 0"],
    "nt1"    : ["ntop == 1"],
    "nt2"    : ["ntop == 2"],
    "ntgeq1" : ["ntop >= 1"],
    "nw0"    : ["nw == 0"],
    "nw1"    : ["nw == 1"],
    "nw2"    : ["nw == 2"],
    "nwgeq1" : ["nw >= 1"],
    "nrt0"    : ["nres == 0"],
    "nrt1"    : ["nres == 1"],
    "nrt2"    : ["nres == 2"],
    "nrtgeq1" : ["nres >= 1"],
    "nrtntnwgeq2" : ["(ntop + nw + nres) >= 2"],
    "nrtntnwgeq3" : ["(ntop + nw + nres) >= 3"],
    "htlt1000"     : ["HT < 1000"],
    "htgt1000"     : ["HT >= 1000"],
    "ht1000to1500" : ["HT >= 1000", "HT < 1500"],
    "htgt1500"     : ["HT >= 1500"],
    "htlt1300"     : ["HT < 1300"],
    "htgt1300"     : ["HT >= 1300"],
    "ht1000to1300" : ["HT >= 1000", "HT < 1300"],
    "ht1300to1500" : ["HT >= 1300", "HT < 1500"],
    "MET" : [],    # bins have "MET_pt" with an underscore, so if we split by underscores, we'll get "MET" alone
    "pt250to300" : ["MET >= 250", "MET < 300"],
    "pt250to350" : ["MET >= 250", "MET < 350"],
    "pt250to450" : ["MET >= 250", "MET < 450"],
    "pt250to550" : ["MET >= 250", "MET < 550"],
    "pt250toinf" : ["MET >= 250"],
    "pt300to400" : ["MET >= 300", "MET < 400"],
    "pt300toinf" : ["MET >= 300"],
    "pt350to450" : ["MET >= 350", "MET < 450"],
    "pt350to550" : ["MET >= 350", "MET < 550"],
    "pt350toinf" : ["MET >= 350"],
    "pt400to500" : ["MET >= 400", "MET < 500"],
    "pt400toinf" : ["MET >= 400"],
    "pt450to550" : ["MET >= 450", "MET < 550"],
    "pt450toinf" : ["MET >= 450"],
    "pt500to600" : ["MET >= 500", "MET < 600"],
    "pt500toinf" : ["MET >= 500"],
    "pt550to650" : ["MET >= 550", "MET < 650"],
    "pt550toinf" : ["MET >= 550"],
    "pt600toinf" : ["MET >= 600"],
    "pt650to750" : ["MET >= 650", "MET < 750"],
    "pt650toinf" : ["MET >= 650"],
    "pt750toinf" : ["MET >= 750"],
}

inputJsonFile = sys.argv[1]

with open(inputJsonFile) as f:
    JSON = json.load(f)

def make_cpp(units, func_name, outfile=sys.stdout, header=False):
    varstring = ", ".join([" ".join(a[::-1]) for a in variables])
    print("int %s(%s)"%(func_name, varstring), end="", file=outfile)
    if(header):
        print(";\n", end="", file=outfile)
    else:
        units = [(b,a) for a, b in units.items()]
        units.sort(key=lambda x: int(x[0]))
        print("\n{\n", end="", file=outfile)
#        print("\tif(!Pass_Baseline) return -1;\n", end="", file=outfile)
        print("\t", end="", file=outfile)
        for bin_index, (unit_num, bin_name) in enumerate(units):
            cuts = bin_name.split('_')[1:]
            conditionals = []
            if "cr_" not in bin_name:
                conditionals.append("Pass_Baseline")
            for cut in cuts:
                conditionals.extend(cut_defs[cut])
            cutstring = "(" + ") && (".join(conditionals) + ")"
            print("if (%s) return %d;\n\telse "%(cutstring, int(unit_num)), end="", file=outfile)
        print("return -1;\n}", file=outfile)
        print(file=outfile)

with open("units.cc", "w") as outfile:
    make_cpp(JSON["binNum"], "SRbin", outfile)
    make_cpp(JSON["unitSRNum"], "SRunit", outfile)
    make_cpp(JSON["unitCRNum"]["qcdcr"], "QCDCRunit", outfile)
    make_cpp(JSON["unitCRNum"]["lepcr"], "lepCRunit", outfile)
    make_cpp(JSON["unitCRNum"]["phocr"], "phoCRunit", outfile)

with open("units.hh", "w") as outfile:
    print("#ifndef SEARCHUNITDEF_HH", file=outfile)
    print("#define SEARCHUNITDEF_HH", file=outfile)
    make_cpp(JSON["binNum"],             "SRbin",     outfile, True)
    make_cpp(JSON["unitSRNum"],          "SRunit",    outfile, True)
    make_cpp(JSON["unitCRNum"]["qcdcr"], "QCDCRunit", outfile, True)
    make_cpp(JSON["unitCRNum"]["lepcr"], "lepCRunit", outfile, True)
    make_cpp(JSON["unitCRNum"]["phocr"], "phoCRunit", outfile, True)
    print("#endif", file=outfile)
