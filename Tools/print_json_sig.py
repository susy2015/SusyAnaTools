import ROOT
import json
import os
import argparse

def combineupdown(updown):
    #takes a dict {bin : [up/nominal,down/nominal]}
    #outputs dict {bin : unc}
    unc = {}
    for k,v in updown.iteritems():
        sign = 1
        if v[0] < 1:
            sign = -1 #using up for sign
        val = 0.5 * (abs(v[0]-1) + abs(1-v[1])) #average of absolute difference
        unc.update({k : 1 + sign*val})
    return unc
def simupdown(updown):
    unc = {}
    for k,v in updown.iteritems():
        sign = 1
        if v[0] < 1: sign = -1
        val = [abs(v[0]-1),abs(v[1]-1)]
        unc.update({k : [1 + sign*val[0],1+sign*val[1]]})
    return unc

def myprocess(sample,mass_point):
    jsonFile = "dc_BkgPred_BinMaps_master.json" #softlink from Matthew's repository to keep it updated
    with open(jsonFile, "r") as inputFile:
        unitMap = json.load(inputFile)
        binMap = {int(v): k for k, v in unitMap["binNum"].iteritems()}
        nBins = len(binMap)

    #infile = ROOT.TFile("TTZ_VB_2016.root")
    #infile = ROOT.TFile("TTZ_VB.root")
    #infilename = "results/" + sample + "/" + mass_point + ".root"
    infilename = "results/fastsim/{0}/{1}.root".format(sample,mass_point)
    infile = ROOT.TFile(infilename)
    h_sb_low  = infile.Get("h_sb_low")
    h_sb_high = infile.Get("h_sb_high")
    h_sb_low_avgwtsq = infile.Get("h_sb_low_avg_weight_sq")
    h_sb_high_avgwtsq = infile.Get("h_sb_high_avg_weight_sq")

    extensions = ["_bsf","_trig_eff","_puWeight","_PFWeight","_pdfWeight","_JES","_METUnClust"]
    ext_names = {   "_bsf" : "b_SF",
                    "_trig_eff" : "trigger_eff",
                    "_puWeight" : "PU_Weight",
                    "_PFWeight" : "Prefire_Weight",
                    "_pdfWeight" : "PDF_Weight",
                    "_JES" : "JEC",
                    "_METUnClust" : "MET_UnClust"}
    if sample == "TTZ":
        extensions.append("_SF")
        ext_names.update({"_SF" : "TTZ_SF"})

    #rate
    dict_sb = {i : [h_sb_low.GetBinContent(i+1),h_sb_low.GetBinError(i+1)] for i in range(53)}
    dict_sb.update({i+53 : [h_sb_high.GetBinContent(i+1),h_sb_high.GetBinError(i+1)] for i in range(130)})
    #Ensure non-negative
    for k,v in dict_sb.iteritems():
        if dict_sb[k][0] < 0: dict_sb[k][0] = 0.0

    '''
    #cs_event
    rounded = []
    for i in range(53):
        if h_sb_low_avgwtsq.GetBinContent(i+1) > 0:
            rounded.append(round(h_sb_low.GetBinContent(i+1)/h_sb_low_avgwtsq.GetBinContent(i+1)))
        else: rounded.append(0)
    for i in range(130):
        if h_sb_high_avgwtsq.GetBinContent(i+1) > 0:
            rounded.append(round(h_sb_high.GetBinContent(i+1)/h_sb_high_avgwtsq.GetBinContent(i+1)))
        else: rounded.append(0)

    dict_sb_round = {i : [int(rounded[i]),h_sb_low.GetBinError(i+1)] for i in range(53)}
    dict_sb_round.update({i+53 : [int(rounded[i+53]),h_sb_high.GetBinError(i+1)] for i in range(130)})
    '''

    predMap = {binMap.get(k,k) : v for k,v in dict_sb.iteritems()}
    #predMap_round = {binMap.get(k,k) : v for k,v in dict_sb_round.iteritems()}

    j = {}
    j[mass_point] = predMap
    #j["yieldsMap"] = predMap_round

    #systematic uncertainties
    sys_sb_low = {}
    sys_sb_high = {}
    sys_updown = {}
    syst_unc = {}
    syst_unc_map = {}
    syst_updown = {}
    for ext in extensions:
        sys_sb_low[ext] = [infile.Get("h_sb_low" + ext + "_up"),infile.Get("h_sb_low" + ext + "_down")]
        sys_sb_high[ext] = [infile.Get("h_sb_high" + ext + "_up"),infile.Get("h_sb_high" + ext + "_down")]
        for i in range(53):
            #then divide by nominal
            if dict_sb[i][0] > 0:
                if sys_sb_low[ext][0].GetBinContent(i+1)/dict_sb[i][0] > sys_sb_low[ext][1].GetBinContent(i+1)/dict_sb[i][0]:
                    syst_updown.update({i : [sys_sb_low[ext][0].GetBinContent(i+1)/dict_sb[i][0],sys_sb_low[ext][1].GetBinContent(i+1)/dict_sb[i][0]]})
                else: syst_updown.update({i : [sys_sb_low[ext][1].GetBinContent(i+1)/dict_sb[i][0],sys_sb_low[ext][0].GetBinContent(i+1)/dict_sb[i][0]]})
            else: syst_updown.update({i : [0.0,0.0]})
        for i in range(130):
            if dict_sb[i+53][0] > 0:
                if sys_sb_high[ext][0].GetBinContent(i+1)/dict_sb[i+53][0] > sys_sb_high[ext][1].GetBinContent(i+1)/dict_sb[i+53][0]:
                    syst_updown.update({i+53 : [sys_sb_high[ext][0].GetBinContent(i+1)/dict_sb[i+53][0],sys_sb_high[ext][1].GetBinContent(i+1)/dict_sb[i+53][0]]})
                else: syst_updown.update({i+53 : [sys_sb_high[ext][1].GetBinContent(i+1)/dict_sb[i+53][0],sys_sb_high[ext][0].GetBinContent(i+1)/dict_sb[i+53][0]]})
            else: syst_updown.update({i+53 : [0.0,0.0]})
        #syst_unc[ext] = combineupdown(syst_updown)
        syst_unc[ext] = simupdown(syst_updown)
        syst_unc_map[ext] = {binMap.get(k,k) : v for k,v in syst_unc[ext].iteritems()}
        #j["syst_unc" + ext] = syst_unc_map[ext]

    #print "Creating file fastsim_results/{0}/{1}_pred.json".format(sample,mass_point)
    #outfile = "fastsim_results/{0}/{1}_pred.json".format(sample,mass_point)
    #with open(outfile, "w") as outputFile:
    #    json.dump(j,outputFile,sort_keys=True,indent=4,separators=(',', ': '))

    print "Creating file fastsim_results/{0}/{1}_syst.conf".format(sample,mass_point)
    syst_outfile = "fastsim_results/{0}/{1}_syst.conf".format(sample,mass_point)
    with open(syst_outfile,"w") as outputFile:
        for ext in extensions:
            for k,v in syst_unc_map[ext].iteritems():
                outputFile.write("{0}\t{1}\t{2}\t{3}\n".format(k,ext_names[ext],sample,v))
    #return j
    return predMap

if __name__ == "__main__":
    #Input arg: config. Read in list of mass points from config.
    parser = argparse.ArgumentParser(description='NanoAOD postprocessing.')
    parser.add_argument('-c', '--config',
        default = "",
        help = 'Path to the input config file. Used only for list of mass points.')
    args = parser.parse_args()
    #Read sample name from cfg. One mass point per line.
    samplename = args.config.split("/")[-1].split(".")[0]
    #Make directory based on sample name (if it doesn't already exist).
    fastsimdir = os.path.abspath("fastsim_results/{0}".format(samplename))
    if not os.path.exists(fastsimdir):
        os.makedirs(fastsimdir)
    mass_point_list = []
    yieldsmap = {}
    with open(args.config, "r") as infile:
        for line in infile:
            entries = line.split(", ")
            mass_point = entries[0]
            mass_point_list.append(mass_point)
            #Run myprocess(sample) for each mass point.
            yieldsmap[mass_point] = myprocess(samplename,mass_point)
    j = {}
    j["signals"] = mass_point_list
    j["yieldsMap"] = yieldsmap
    outfilename = "fastsim_results/{0}/{0}.json".format(samplename)
    print "Creating file fastsim_results/{0}/{0}.json".format(samplename)
    with open(outfilename,"w") as outfile:
        json.dump(j,outfile,sort_keys=True,indent=4,separators=(',',': '))


