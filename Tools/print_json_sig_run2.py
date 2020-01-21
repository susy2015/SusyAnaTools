import ROOT
import json
import os
import argparse
import re

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
        unc.update({k : [1 + sign*val[0], 1 - sign*val[1]]})
    return unc

def myprocess(sample,mass_point,bintype):
    jsonFile = "dc_BkgPred_BinMaps_master.json" #softlink from Matthew's repository to keep it updated
    with open(jsonFile, "r") as inputFile:
        unitMap = json.load(inputFile)
        binmapname = ""
        if bintype == "sb":
            binmapname = "binNum"
            binMap = {int(v): k for k, v in unitMap[binmapname].iteritems()}
        elif bintype == "ub":
            binmapname = "unitCRNum"
            binMap = {int(v): k for k, v in unitMap[binmapname]["lepcr"].iteritems()}
        nBins = len(binMap)

    #infile = ROOT.TFile("TTZ_VB_2016.root")
    #infile = ROOT.TFile("TTZ_VB.root")
    #infilename = "results/" + sample + "/" + mass_point + ".root"
    #infilename = "results/fastsim/{0}/{1}.root".format(sample,mass_point)
    #infilename = "results/VBSB/{0}.root".format(mass_point)
    infilename = "results/v4/VBSB/{0}.root".format(mass_point)
    infile = ROOT.TFile(infilename)
    h_b  = infile.Get("h_{0}".format(bintype))
    #h_sb_avgwtsq = infile.Get("h_sb_avg_weight_sq")

    nbin = 0;
    if bintype == "vb": nbin = 43
    elif bintype == "sb": nbin = 183
    elif bintype == "ub": nbin = 112

    extensions = ["_bsf","_trig_eff","_puWeight","_PFWeight","_pdfWeight","_JES","_METUnClust","_ivfunc","_eff_e","_err_mu","_eff_tau","_ISRWeight","_fastSF","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag"]
    leptonvetos = ["_eff_e","_err_mu","_eff_tau"]
    topvetos = ["_eff_wtag","_eff_toptag","_eff_restoptag"]
    ext_names = {   "_bsf" : "b",
                    "_trig_eff" : "trigger_err",
                    "_puWeight" : "PU_Weight",
                    "_PFWeight" : "Prefire_Weight",
                    "_pdfWeight" : "PDF_Weight",
                    "_JES" : "JES",
                    "_METUnClust" : "metres",
                    "_ivfunc" : "ivfunc",
                    "_eff_e" : "eff_e",
                    "_err_mu" : "err_mu",
                    "_eff_tau" : "eff_tau",
                    "_eff_wtag" : "eff_wtag",
                    "_eff_toptag" : "eff_toptag",
                    "_eff_restoptag" : "eff_restoptag",
                    "_ISRWeight" : "ISR_Weight",
                    "_fastSF" : "Scale_Unc",
                    "_METunc" : "MET_Unc",
                }

    #rate
    dict_b = {i : [h_b.GetBinContent(i+1),h_b.GetBinError(i+1)] for i in range(nbin)}
    #Ensure non-negative
    for k,v in dict_b.iteritems():
        if dict_b[k][0] <= 0: dict_b[k] = [1e-06,1e-06]

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

    predMap = {binMap.get(k,k) : v for k,v in dict_b.iteritems()}
    #predMap_round = {binMap.get(k,k) : v for k,v in dict_sb_round.iteritems()}

    #j = {}
    #j[mass_point] = predMap
    #j["yieldsMap"] = predMap_round

    #systematic uncertainties
    sys_b = {}
    sys_updown = {}
    syst_unc = {}
    syst_unc_map = {}
    syst_updown = {}
    if bintype == "sb":
        lowdm = [0,53]
        highdm = [53,183]
    elif bintype == "ub":
        lowdm = [0,53]
        highdm = [53,529]
    for ext in extensions:
        #print ext
        sys_b[ext] = [infile.Get("h_" + bintype + ext + "_up"),infile.Get("h_" + bintype + ext + "_down")]
        for i in range(nbin):
            #then divide by nominal
            if dict_b[i][0] > 0:
                if sys_b[ext][0].GetBinContent(i+1)/dict_b[i][0] > sys_b[ext][1].GetBinContent(i+1)/dict_b[i][0]:
                    syst_updown.update({i : [sys_b[ext][0].GetBinContent(i+1)/dict_b[i][0],sys_b[ext][1].GetBinContent(i+1)/dict_b[i][0]]})
                else: syst_updown.update({i : [sys_b[ext][1].GetBinContent(i+1)/dict_b[i][0],sys_b[ext][0].GetBinContent(i+1)/dict_b[i][0]]})
            else: syst_updown.update({i : [0.0,0.0]})
        #syst_unc[ext] = combineupdown(syst_updown)
        syst_unc[ext] = simupdown(syst_updown)
        for i in range(nbin):
            if dict_b[i][0] == 1e-06:
                syst_unc[ext][i] = [2,0.001]
        if (ext in leptonvetos) or (ext in topvetos):
            for i in range(lowdm[1]):
                syst_unc[ext][i] = [1,1]
        if ext in leptonvetos:
            for i in range(highdm[0],highdm[1]):
                syst_unc[ext][i] = [1,1]
        if ext == "_ivfunc":
            for i in range(highdm[0],highdm[1]):
                syst_unc[ext][i] = [1,1]
        
        syst_unc_map[ext] = {binMap.get(k,k) : v for k,v in syst_unc[ext].iteritems()}

        #if bintype == "sb":
        #    testbin = 142
        #    print "Bin ", testbin, ext
        #    print "dict_b", dict_b[testbin]
        #    print "sys_b", sys_b[ext][0].GetBinContent(testbin+1), sys_b[ext][1].GetBinContent(testbin+1)
        #    print "syst_updown", syst_updown[testbin]
        #    print "syst_unc", syst_unc[ext][testbin]
        #    print "syst_unc_map", syst_unc_map[ext]["bin_hm_nbeq2_highmtb_nt1_nrt1_nw0_htgt1300_MET_pt450toinf"]
        #j["syst_unc" + ext] = syst_unc_map[ext]
    #turning off rest of top/w vetos
    for k,v in syst_unc_map["_eff_wtag"].iteritems():
        if "nw0" in k:
            syst_unc_map["_eff_wtag"][k] = [1,1]
    for k,v in syst_unc_map["_eff_toptag"].iteritems():
        if "nt0" in k:
            syst_unc_map["_eff_toptag"][k] = [1,1]
    for k,v in syst_unc_map["_eff_restoptag"].iteritems():
        if "nrt0" in k:
            syst_unc_map["_eff_restoptag"][k] = [1,1]

    #print "Creating file fastsim_results/{0}/{1}_pred.json".format(sample,mass_point)
    #outfile = "fastsim_results/{0}/{1}_pred.json".format(sample,mass_point)
    #with open(outfile, "w") as outputFile:
    #    json.dump(j,outputFile,sort_keys=True,indent=4,separators=(',', ': '))

    #syst_outfile = "fastsim_results/{0}/{1}_{2}_syst.conf".format(sample,mass_point,bintype)
    #print "Creating file {0}".format(syst_outfile)
    #with open(syst_outfile,"w") as outputFile:
    #    for ext in extensions:
    #        for k,v in syst_unc_map[ext].iteritems():
    #            outputFile.write("{0}\t{1}_up\t{2}\t{3}\n".format(k,ext_names[ext],sample,v[0]))
    #            outputFile.write("{0}\t{1}_down\t{2}\t{3}\n".format(k,ext_names[ext],sample,v[1]))

    #return j
    return predMap,syst_unc_map,syst_unc

if __name__ == "__main__":
    #Input arg: config. Read in list of mass points from config.
    parser = argparse.ArgumentParser(description='NanoAOD postprocessing.')
    parser.add_argument('-c', '--config',
        default = "",
        help = 'Path to the input config file. Used only for list of mass points.')
    args = parser.parse_args()
    #Read sample name from cfg. One mass point per line.
    samplename = args.config.split("/")[-1].split(".")[0][:-5]
    #Make directory based on sample name (if it doesn't already exist).
    fastsimdir = os.path.abspath("fastsim_results/{0}".format(samplename))
    if not os.path.exists(fastsimdir):
        os.makedirs(fastsimdir)
    extensions = ["_bsf","_trig_eff","_puWeight","_PFWeight","_pdfWeight","_JES","_METUnClust","_ivfunc","_eff_e","_err_mu","_eff_tau","_ISRWeight","_fastSF","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag"]
    ext_names = {   "_bsf" : "b",
                    "_trig_eff" : "trigger_err",
                    "_puWeight" : "PU_Weight",
                    "_PFWeight" : "Prefire_Weight",
                    "_pdfWeight" : "PDF_Weight",
                    "_JES" : "JES",
                    "_METUnClust" : "metres",
                    "_ivfunc" : "ivfunc",
                    "_eff_e" : "eff_e",
                    "_err_mu" : "err_mu",
                    "_eff_tau" : "eff_tau",
                    "_eff_wtag" : "eff_wtag",
                    "_eff_toptag" : "eff_toptag",
                    "_eff_restoptag" : "eff_restoptag",
                    "_ISRWeight" : "ISR_Weight",
                    "_fastSF" : "Scale_Unc",
                    "_METunc" : "MET_Unc",
                }
    mass_point_list = []
    yieldsmap = {}
    systmap = {}
    syst = {}
    with open(args.config, "r") as infile:
        for line in infile:
            entries = line.split(", ")
            mass_point = entries[0][:-5]
            splitmasspoint = mass_point.split("_")
            numbers = re.findall(r'\d+',mass_point) #0th will be 1 or 2 from T1, T2
            shortmasspoint = "{0}_{1}_{2}".format(splitmasspoint[1],numbers[1],numbers[2])
            mass_point_list.append(shortmasspoint)
            #Run myprocess(sample) for each mass point.
            #print mass_point
            yieldsmap[shortmasspoint],systmap[shortmasspoint],syst[shortmasspoint] = myprocess(samplename,mass_point,"sb")
            yieldsmap["lepcr_{0}".format(shortmasspoint)],systmap["lepcr_{0}".format(shortmasspoint)],syst["lepcr_{0}".format(shortmasspoint)] = myprocess(samplename,mass_point,"ub")

            '''
            mycanvas = ROOT.TCanvas(shortmasspoint, shortmasspoint, 600, 600)
            ROOT.gStyle.SetOptStat(0)
            #testext = ["_bsf"]
            #testext = ["_bsf","_trig_eff"]
            small = ["_trig_eff","_PFWeight","_eff_e","_err_mu","_eff_tau","_fastSF"]
            for ext in extensions:
                leg = ROOT.TLegend(0.4,0.8,0.9,0.89)
                leg.SetBorderSize(0)
                leg.SetTextSize(0.04)
                hist_up = ROOT.TH1F(shortmasspoint + ext + "_up", shortmasspoint + ext + " Up",183,0,183)
                hist_down = ROOT.TH1F(shortmasspoint + ext + "_down", shortmasspoint + ext + " Down",183,0,183)
                for i in range(183):
                    hist_up.SetBinContent(i+1,syst[shortmasspoint][ext][i][0])
                    hist_down.SetBinContent(i+1,syst[shortmasspoint][ext][i][1])
                hist_up.SetLineColor(ROOT.kBlue)
                hist_up.SetLineWidth(2)
                hist_up.GetYaxis().SetRangeUser(0,2)
                if ext in small: hist_up.GetYaxis().SetRangeUser(0.8,1.2)
                hist_up.Draw()
                hist_down.SetLineColor(ROOT.kRed)
                hist_down.SetLineWidth(2)
                hist_down.Draw("same")
                leg.AddEntry(hist_up, shortmasspoint + ext + " Up","l")
                leg.AddEntry(hist_down, shortmasspoint + ext + " Down","l")
                leg.Draw("same")
                mycanvas.SaveAs("VBSB_plots/{0}{1}.png".format(shortmasspoint,ext))
            '''
    j = {}
    j["signals"] = mass_point_list
    j["yieldsMap"] = yieldsmap
    
    for point in mass_point_list:
        json_outfile = "fastsim_results/{0}/{1}.json".format(samplename,point)
        print "Creating file {0}".format(json_outfile)
        with open(json_outfile,"w") as outfile:
            y = {}
            y["signals"] = [point,]
            y["yieldsMap"] = {}
            y["yieldsMap"][point] = yieldsmap[point]
            y["yieldsMap"]["lepcr_{0}".format(point)] = yieldsmap["lepcr_{0}".format(point)]
            json.dump(y,outfile,sort_keys=True,indent=4,separators=(',',': '))
        syst_outfile = "fastsim_results/{0}/{1}_syst.conf".format(samplename,point)
        print "Creating file {0}".format(syst_outfile)
        with open(syst_outfile,"w") as outfile:
            for ext in extensions:
                for k,v in systmap[point][ext].iteritems():
                    outfile.write("{0}    {1}_up    {2}    {3}\n".format(k,ext_names[ext],point,v[0]))
                    outfile.write("{0}    {1}_down    {2}    {3}\n".format(k,ext_names[ext],point,v[1]))
                for k,v in systmap["lepcr_{0}".format(point)][ext].iteritems():
                    outfile.write("{0}    {1}_up    {2}    {3}\n".format(k,ext_names[ext],point,v[0]))
                    outfile.write("{0}    {1}_down    {2}    {3}\n".format(k,ext_names[ext],point,v[1]))
    
    
    #outfilename = "fastsim_results/{0}/{0}_sb.json".format(samplename)
    #print "Creating file {0}".format(outfilename)
    #with open(outfilename,"w") as outfile:
    #    json.dump(j,outfile,sort_keys=True,indent=4,separators=(',',': '))


