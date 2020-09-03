import ROOT
import json
import os
import argparse
import re
import numpy as np

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
        '''
        sign1 = 1
        sign2 = 1
        if v[0] < 1: sign1 = -1
        if v[1] < 1: sign2 = -1
        val = [abs(v[0]-1),abs(v[1]-1)]
        unc.update({k : [1 + sign1*val[0], 1 - sign1*val[1]]})
        #unc.update({k : [1 + sign1*val[0], 1 + sign2*val[1]]})
        '''
        #Now letting Matt handle same-side uncertainties
        unc.update({k : [v[0],v[1]]})
    return unc
def percdiff(updown):
    unc = {}
    for k,v in updown.iteritems():
        unc.update({k : [1+abs(1-v[0]),1+abs(1-v[1])]})
    return unc


def myprocess(sample,mass_point,bintype):
    jsonFile = "dc_BkgPred_BinMaps_master.json" #softlink from Matthew's repository to keep it updated
    with open(jsonFile, "r") as inputFile:
        unitMap = json.load(inputFile)
        binmapname = ""
        if bintype == "sb":
            binmapname = "binNum"
            binMap = {int(v): k for k, v in unitMap[binmapname].iteritems()}
            #print "binMap[134]", binMap[134]
        elif bintype == "ub":
            binmapname = "unitCRNum"
            binMap = {int(v): k for k, v in unitMap[binmapname]["lepcr"].iteritems()}
        elif bintype == "vb":
            binMap = {k : str(k) for k in range(0,43)}
        elif bintype == "qb":
            binmapname = "unitCRNum"
            binMap = {int(v): k for k, v in unitMap[binmapname]["qcdcr"].iteritems()}
        nBins = len(binMap)
    ##infilename = "results/" + sample + "/" + mass_point + ".root"
    ##infilename = "results/VBSB/{0}/{1}_2016.root".format(sample,mass_point)
    infilename = "results/VBSB/{0}/merged/{1}.root".format(sample,mass_point)
    ##infilename = "results/VBSB_update/T2fbd/{0}.root".format(mass_point)
    ##infilename = "results/VBSB/{0}/{1}.root".format(sample,mass_point)
    #infilename = "fastsimstuff/v6p5newstat/{0}.root".format(mass_point)
    #infilename = "fastsimstuff/v6p5T2bW/{0}/merged/{1}.root".format(sample,mass_point)
    #infilename = "fastsimstuff/v6p5newstat/{0}_2016.root".format(mass_point)
    #print infilename
    infile = ROOT.TFile(infilename)
    h_b  = infile.Get("h_{0}".format(bintype))
    if bintype == "sb": h_raw = infile.Get("h_{0}_raw".format(bintype))
    #h_b_fast  = infile.Get("h_{0}_fast".format(bintype))
    h_b_full  = infile.Get("h_{0}_full".format(bintype))
    #h_sb_avgwtsq = infile.Get("h_sb_avg_weight_sq")

    #extensions = ["_bsf","_trig_eff","_puWeight","_PFWeight","_pdfWeight","_JES","_METUnClust","_ivfunc","_eff_e","_err_mu","_eff_tau","_ISRWeight","_fastSF","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag"]
    #extensions = ["_bsf","_trig_eff","_puWeight","_PFWeight","_JES","_METUnClust","_ivfunc","_eff_e","_err_mu","_eff_tau","_ISRWeight","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag"]
    #extensions = ["_bsf_light","_bsf_heavy","_bsf_light_fast","_bsf_heavy_fast","_trig_eff","_puWeight","_PFWeight","_JES","_METUnClust","_ivfunc","_ivfunc_fast","_eff_e","_err_mu","_eff_tau","_ISRWeight","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag","_eff_wtag_fast","_eff_toptag_fast","_eff_restoptag_fast","_ak8veto","_ak8veto_fast","_eff_densetoptag"]
    #extensions = ["_bsf","_bsf_fast","_trig_eff","_puWeight","_PFWeight","_JES","_METUnClust","_ivfunc","_ivfunc_fast","_eff_e","_err_mu","_eff_tau","_ISRWeight","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag","_eff_wtag_fast","_eff_toptag_fast","_eff_restoptag_fast","_ak8veto","_ak8veto_fast","_eff_densetoptag"]
    extensions = ["_bsf","_bsf_fast","_bsf_light","_bsf_heavy","_bsf_light_fast","_bsf_heavy_fast","_trig_eff","_puWeight","_PFWeight","_JES","_METUnClust","_ivfunc","_ivfunc_fast","_eff_e","_err_mu","_eff_tau","_ISRWeight","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag","_eff_wtag_fast","_eff_toptag_fast","_eff_restoptag_fast","_ak8veto","_ak8veto_fast","_eff_densetoptag"]
    leptonvetos = ["_eff_e","_err_mu","_eff_tau"]
    topvetos = ["_eff_wtag","_eff_toptag","_eff_restoptag","_eff_wtag_fast","_eff_toptag_fast","_eff_restoptag_fast"]
    ak8vetos = ["_eff_wtag","_eff_toptag","_eff_wtag_fast","_eff_toptag_fast"]
    fast_tagger = ["_eff_wtag_fast","_eff_toptag_fast","_ak8veto_fast"]
    full_tagger = ["_eff_wtag","_eff_toptag","_ak8veto","_eff_densetoptag"]
    ext_names = {   "_bsf" : "b",
                    "_bsf_light" : "b_light",
                    "_bsf_heavy" : "b_heavy",
                    "_bsf_light_fast" : "b_light_fast",
                    "_bsf_heavy_fast" : "b_heavy_fast",
                    "_bsf_fast" : "b_fast",
                    "_trig_eff" : "trigger_err",
                    "_puWeight" : "PU_Weight",
                    "_PFWeight" : "Prefire_Weight",
                    #"_pdfWeight" : "PDF_Weight",
                    "_JES" : "JES",
                    "_METUnClust" : "metres",
                    "_ivfunc" : "ivfunc",
                    "_ivfunc_fast" : "ivfunc_fast",
                    "_eff_e" : "eff_e",
                    "_err_mu" : "err_mu",
                    "_eff_tau" : "eff_tau",
                    "_eff_wtag" : "eff_wtag",
                    "_eff_toptag" : "eff_toptag",
                    "_eff_restoptag" : "eff_restoptag",
                    "_eff_wtag_fast" : "eff_wtag_fast",
                    "_eff_toptag_fast" : "eff_toptag_fast",
                    "_eff_restoptag_fast" : "eff_restoptag_fast",
                    "_ak8veto" : "eff_fatjet_veto",
                    "_ak8veto_fast" : "eff_fatjet_veto_fast",
                    "_ISRWeight" : "ISR_Weight",
                    #"_fastSF" : "Scale_Unc",
                    "_METunc" : "MET_Unc",
                    "_eff_densetoptag" : "eff_densetoptag",
                    "_lumi" : "lumi",
                }

    #rate
    #if bintype == "sb": print "Bin 183:", h_b.GetBinContent(183)
    dict_b = {i : [h_b.GetBinContent(i+1),h_b.GetBinError(i+1)] for i in range(nBins)}
    if bintype == "sb": raw_b = {i : [h_raw.GetBinContent(i+1),h_raw.GetBinError(i+1)] for i in range(nBins)}
    #dict_b_fast = {i : [h_b_fast.GetBinContent(i+1),h_b_fast.GetBinError(i+1)] for i in range(nBins)}
    dict_b_full = {i : [h_b_full.GetBinContent(i+1),h_b_full.GetBinError(i+1)] for i in range(nBins)}
    #if bintype == "sb": print "dict_b 182:", dict_b[182]
    #Ensure non-negative; no need to remove 0 signal (as opposed to 0 background, which does cause problems)
    for k,v in dict_b.iteritems():
        if dict_b[k][0] < 0: dict_b[k] = [0,0]
        #if dict_b_fast[k][0] < 0: dict_b_fast[k] = [0,0]
        if dict_b_full[k][0] < 0: dict_b_full[k] = [0,0]
        if (dict_b[k][1] > dict_b[k][0]) and ((dict_b[k][1] - dict_b[k][0]) > 0.001):
            print mass_point, k, dict_b[k][0], dict_b[k][1]

    #if bintype == "sb": print "dict_b 182:", dict_b[182]

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
    if bintype == "sb": rawMap = {binMap.get(k,k) : v for k,v in raw_b.iteritems()}
    else: rawMap = []
    #test
    '''
    for k,v in predMap.iteritems():
        if (predMap[k][1] > predMap[k][0]) and ((predMap[k][1] - predMap[k][0]) > 0.001):
            print mass_point, k, predMap[k][0], predMap[k][1]
        if (k == "bin_hm_nb1_highmtb_nt0_nrtgeq1_nw0_htgt1500_MET_pt550to650") and (mass_point == "SMS_T2bW_mStop250_mLSP75_fastsim"):
            print v
    '''
    #if bintype == "sb": print "predMap bin_hm_nb3_highmtb_nrtntnwgeq3_MET_pt250toinf:", predMap["bin_hm_nb3_highmtb_nrtntnwgeq3_MET_pt250toinf"]
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
    perc_diff_map = {}
    if bintype == "sb":
        lowdm = [0,53]
        highdm = [53,183]
    elif bintype == "ub":
        lowdm = [0,53]
        highdm = [53,112]
    elif bintype == "vb":
        lowdm = [0,19]
        highdm = [19,43]
    elif bintype == "qb":
        lowdm = [0,32]
        highdm = [33,92]
    for ext in extensions:
        #print ext, infilename
        sys_b[ext] = [infile.Get("h_" + bintype + ext + "_up"),infile.Get("h_" + bintype + ext + "_down")]
        for i in range(nBins):
            #if ext not in fast_tagger:
            if ext not in full_tagger:
                #then divide by nominal
                if dict_b[i][0] > 0:
                    #if sys_b[ext][0].GetBinContent(i+1)/dict_b[i][0] > sys_b[ext][1].GetBinContent(i+1)/dict_b[i][0]:
                    #    syst_updown.update({i : [sys_b[ext][0].GetBinContent(i+1)/dict_b[i][0],sys_b[ext][1].GetBinContent(i+1)/dict_b[i][0]]})
                    #else: syst_updown.update({i : [sys_b[ext][1].GetBinContent(i+1)/dict_b[i][0],sys_b[ext][0].GetBinContent(i+1)/dict_b[i][0]]})
                    syst_updown.update({i : [sys_b[ext][0].GetBinContent(i+1)/dict_b[i][0],sys_b[ext][1].GetBinContent(i+1)/dict_b[i][0]]})
                else: syst_updown.update({i : [2.0,0.01]})
            else:
                if dict_b_full[i][0] > 0:
                    syst_updown.update({i : [sys_b[ext][0].GetBinContent(i+1)/dict_b_full[i][0],sys_b[ext][1].GetBinContent(i+1)/dict_b_full[i][0]]})
                else: syst_updown.update({i : [2.0,0.01]})
        #syst_unc[ext] = combineupdown(syst_updown)
        syst_unc[ext] = simupdown(syst_updown)
        for i in range(nBins):
            if dict_b[i][0] == 0:
                syst_unc[ext][i] = [2.0,0.01]
        #if ((ext in leptonvetos) and (bintype == "qb")) or (ext in topvetos):
        if ((ext in leptonvetos) and (bintype == "qb")):
            for i in range(lowdm[1]):
                syst_unc[ext][i] = [1,1]
        if (ext in leptonvetos) and (bintype == "qb"):
            for i in range(highdm[0],highdm[1]):
                syst_unc[ext][i] = [1,1]
        if (ext == "_ivfunc") or (ext == "_ivfunc_fast"):
            for i in range(highdm[0],highdm[1]):
                syst_unc[ext][i] = [1,1]
        for i in range(nBins):
            if syst_unc[ext][i][0] > 2.0: syst_unc[ext][i][0] = 2.0
            if syst_unc[ext][i][0] < 0.01: syst_unc[ext][i][0] = 2.0
            if syst_unc[ext][i][1] > 2.0: syst_unc[ext][i][1] = 0.01
            if syst_unc[ext][i][1] < 0.01: syst_unc[ext][i][1] = 0.01
        
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
    LHEScaleWeight = {}
    nums = [0,1,3,4,5,7,8]
    for n in nums:
        LHEScaleWeight[n] = infile.Get("h_" + bintype + "_LHEScaleWeight_" + str(n))
        LHEScaleWeight[n].Divide(h_b)
    for i in range(nBins):
        if dict_b[i][0] > 0:
            temp_min = LHEScaleWeight[0].GetBinContent(i+1)
            temp_max = LHEScaleWeight[0].GetBinContent(i+1)
            for n in nums:
                temp_min = min(temp_min,LHEScaleWeight[n].GetBinContent(i+1))
                temp_max = max(temp_max,LHEScaleWeight[n].GetBinContent(i+1))
            syst_updown.update({i : [temp_max, temp_min]})
        else: syst_updown.update({i : [2.0,0.01]})
    syst_unc["LHEScale"] = simupdown(syst_updown)
    for i in range(nBins):
        if syst_unc["LHEScale"][i][0] > 2.0: syst_unc["LHEScale"][i][0] = 2.0
        if syst_unc["LHEScale"][i][0] < 0.01: syst_unc["LHEScale"][i][0] = 2.0
        if syst_unc["LHEScale"][i][1] > 2.0: syst_unc["LHEScale"][i][1] = 0.01
        if syst_unc["LHEScale"][i][1] < 0.01: syst_unc["LHEScale"][i][1] = 0.01
    syst_unc_map["LHEScale"] = {binMap.get(k,k) : v for k,v in syst_unc["LHEScale"].iteritems()}
    syst_unc["_lumi"] = {}
    for i in range(nBins):
        syst_unc["_lumi"][i] = [1.018,0.982]
    syst_unc_map["_lumi"] = {binMap.get(k,k) : v for k,v in syst_unc["_lumi"].iteritems()}
    ##Replacement?
    #for ext in ak8vetos:
    #    for k,v in syst_unc_map[ext].iteritems():
    #        if "lm" in k:
    #            syst_unc_map[ext][k] = syst_unc_map["_ak8veto"][k]
    #        if ("nw0" in k) and (ext == "_eff_wtag"):
    #            syst_unc_map[ext][k] = syst_unc_map["_ak8veto"][k]
    #        if ("nt0" in k) and (ext == "_eff_toptag"):
    #            syst_unc_map[ext][k] = syst_unc_map["_ak8veto"][k]
    #turning off rest of top/w vetos
    #for k,v in syst_unc_map["_eff_wtag"].iteritems():
    #    if "nw0" in k:
    #        syst_unc_map["_eff_wtag"][k] = [1,1]
    #for k,v in syst_unc_map["_eff_toptag"].iteritems():
    #    if "nt0" in k:
    #        syst_unc_map["_eff_toptag"][k] = [1,1]
    #for k,v in syst_unc_map["_eff_restoptag"].iteritems():
    #    if "nrt0" in k:
    #        syst_unc_map["_eff_restoptag"][k] = [1,1]

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
    #return predMap,syst_unc_map,syst_unc
    for ext in extensions:
        perc_diff_map[ext] = percdiff(syst_unc_map[ext])
    return dict_b,predMap,syst_unc_map,syst_unc,perc_diff_map,rawMap

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
    fastsimdir = os.path.abspath("fastsim_results/v6p5_Jul31/{0}".format(samplename))
    #fastsimdir = os.path.abspath("fastsimstuff/v6p5newstat/{0}".format(samplename))
    if not os.path.exists(fastsimdir):
        os.makedirs(fastsimdir)
    #extensions = ["_bsf","_trig_eff","_puWeight","_PFWeight","_pdfWeight","_JES","_METUnClust","_ivfunc","_eff_e","_err_mu","_eff_tau","_ISRWeight","_fastSF","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag"]
    #extensions = ["_bsf","_trig_eff","_puWeight","_PFWeight","_JES","_METUnClust","_ivfunc","_eff_e","_err_mu","_eff_tau","_ISRWeight","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag"]
    #extensions = ["_bsf","_bsf_fast","_trig_eff","_puWeight","_PFWeight","_JES","_METUnClust","_ivfunc","_ivfunc_fast","_eff_e","_err_mu","_eff_tau","_ISRWeight","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag","_eff_wtag_fast","_eff_toptag_fast","_eff_restoptag_fast","_ak8veto","_ak8veto_fast","_eff_densetoptag","_lumi"]
    extensions = ["_bsf_light","_bsf_heavy","_bsf_light_fast","_bsf_heavy_fast","_trig_eff","_puWeight","_PFWeight","_JES","_METUnClust","_ivfunc","_ivfunc_fast","_eff_e","_err_mu","_eff_tau","_ISRWeight","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag","_eff_wtag_fast","_eff_toptag_fast","_eff_restoptag_fast","_ak8veto","_ak8veto_fast","_eff_densetoptag"]
    #extensions = ["_bsf","_bsf_fast","_trig_eff","_puWeight","_PFWeight","_JES","_METUnClust","_ivfunc","_ivfunc_fast","_eff_e","_err_mu","_eff_tau","_ISRWeight","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag","_eff_wtag_fast","_eff_toptag_fast","_eff_restoptag_fast","_ak8veto","_ak8veto_fast","_eff_densetoptag"]
    leptonvetos = ["_eff_e","_err_mu","_eff_tau"]
    topvetos = ["_eff_wtag","_eff_toptag","_eff_restoptag","_eff_wtag_fast","_eff_toptag_fast","_eff_restoptag_fast"]
    ak8vetos = ["_eff_wtag","_eff_toptag","_eff_wtag_fast","_eff_toptag_fast"]
    ext_names = {   "_bsf" : "b",
                    "_bsf_light" : "b_light",
                    "_bsf_heavy" : "b_heavy",
                    "_bsf_light_fast" : "b_light_fast",
                    "_bsf_heavy_fast" : "b_heavy_fast",
                    "_bsf_fast" : "b_fast",
                    "_trig_eff" : "trigger_err",
                    "_puWeight" : "PU_Weight",
                    "_PFWeight" : "Prefire_Weight",
                    #"_pdfWeight" : "PDF_Weight",
                    "_JES" : "JES",
                    "_METUnClust" : "metres",
                    "_ivfunc" : "ivfunc",
                    "_ivfunc_fast" : "ivfunc_fast",
                    "_eff_e" : "eff_e",
                    "_err_mu" : "err_mu",
                    "_eff_tau" : "eff_tau",
                    "_eff_wtag" : "eff_wtag",
                    "_eff_toptag" : "eff_toptag",
                    "_eff_restoptag" : "eff_restoptag",
                    "_eff_wtag_fast" : "eff_wtag_fast",
                    "_eff_toptag_fast" : "eff_toptag_fast",
                    "_eff_restoptag_fast" : "eff_restoptag_fast",
                    "_ak8veto" : "eff_fatjet_veto",
                    "_ak8veto_fast" : "eff_fatjet_veto_fast",
                    "_ISRWeight" : "ISR_Weight",
                    #"_fastSF" : "Scale_Unc",
                    "_METunc" : "MET_Unc",
                    "_eff_densetoptag" : "eff_densetoptag",
                    "_lumi" : "lumi",
                    "LHEScale" : "LHESigScale",
                }
    mass_point_list = []
    yieldsmap = {}
    yields = {}
    raw_yieldsmap = {}
    vb_raw_yieldsmap = {}
    systmap = {}
    syst = {}
    vb_yields = {}
    vb_yieldsmap = {}
    vb_systmap = {}
    vb_syst = {}
    endperc_diff = {}
    vb_endperc_diff = {}
    with open(args.config, "r") as infile:
        for line in infile:
            entries = line.split(", ")
            mass_point = entries[0][:-5]
            if "#" in mass_point: continue
            splitmasspoint = mass_point.split("_")
            numbers = re.findall(r'\d+',mass_point) #0th will be 1 or 2 from T1, T2
            shortmasspoint = "{0}_{1}_{2}".format(splitmasspoint[1],numbers[1],numbers[2])
            mass_point_list.append(shortmasspoint)
            print samplename, mass_point
            #Run myprocess(sample) for each mass point.
            yields[shortmasspoint],yieldsmap[shortmasspoint],systmap[shortmasspoint],syst[shortmasspoint],endperc_diff[shortmasspoint],raw_yieldsmap[shortmasspoint] = myprocess(samplename,mass_point,"sb")
            yields["lepcr_{0}".format(shortmasspoint)],yieldsmap["lepcr_{0}".format(shortmasspoint)],systmap["lepcr_{0}".format(shortmasspoint)],syst["lepcr_{0}".format(shortmasspoint)],endperc_diff["lepcr_{0}".format(shortmasspoint)],raw_yieldsmap["lepcr_{0}".format(shortmasspoint)] = myprocess(samplename,mass_point,"ub")
            yields["qcdcr_{0}".format(shortmasspoint)],yieldsmap["qcdcr_{0}".format(shortmasspoint)],systmap["qcdcr_{0}".format(shortmasspoint)],syst["qcdcr_{0}".format(shortmasspoint)],endperc_diff["qcdcr_{0}".format(shortmasspoint)],raw_yieldsmap["qcdcr_{0}".format(shortmasspoint)] = myprocess(samplename,mass_point,"qb")
            vb_yields[shortmasspoint],vb_yieldsmap[shortmasspoint],vb_systmap[shortmasspoint],vb_syst[shortmasspoint],vb_endperc_diff[shortmasspoint],vb_raw_yieldsmap[shortmasspoint] = myprocess(samplename,mass_point,"vb")

            '''
            # here for yield plots
            mycanvas = ROOT.TCanvas(shortmasspoint, shortmasspoint, 600, 600)
            ROOT.gStyle.SetOptStat(0)
            ROOT.gPad.SetLogy()
            hist = ROOT.TH1F(shortmasspoint, shortmasspoint, 183,0,183)
            for i in range(183):
                hist.SetBinContent(i+1,yields[shortmasspoint][i][0])
            hist.SetLineWidth(2)
            hist.GetYaxis().SetRangeUser(0.001,1000)
            hist.Draw()
            #mycanvas.SaveAs("VBSB_plots/v5p1/{0}_logy_min.png".format(shortmasspoint))
            #mycanvas.SaveAs("fastsimstuff/v6p5corridoryields/{0}_logy_min.png".format(shortmasspoint))
            mycanvas.SaveAs("VBSB_plots/v6p5_systematics/signal/{0}.png".format(shortmasspoint))
            '''
            '''
            #generally here for plotting
            mycanvas = ROOT.TCanvas(shortmasspoint, shortmasspoint, 600, 600)
            ROOT.gStyle.SetOptStat(0)
            #testext = ["_bsf_light","_bsf_heavy","_bsf_light_fast","_bsf_heavy_fast"]
            testext = ["_bsf","_bsf_fast"]
            #testext = ["_eff_wtag","_eff_toptag","_eff_restoptag","_eff_wtag_fast","_eff_toptag_fast","_eff_restoptag_fast","_ak8veto","_ak8veto_fast"]
            small = ["_trig_eff","_PFWeight","_eff_e","_err_mu","_eff_tau","_fastSF","_ISRWeight"]
            for ext in testext:
                leg = ROOT.TLegend(0.6,0.8,0.9,0.89)
                leg.SetBorderSize(0)
                leg.SetTextSize(0.04)
                #ci = 1756
                #newcolor = ROOT.TColor(ci, 0.1, 0.2, 0.3, "", 0.5)
                #leg.SetFillColor(newcolor)
                #leg.IsTransparent()
                hist_up = ROOT.TH1F(shortmasspoint + ext + "_up", shortmasspoint + " " + ext_names[ext],183,0,183)
                hist_down = ROOT.TH1F(shortmasspoint + ext + "_down", shortmasspoint + " " + ext_names[ext] + " Down",183,0,183)
                hist_up_lowdm = ROOT.TH1F(shortmasspoint + ext + "_up_lowdm", shortmasspoint + " " + ext_names[ext] + " LowDM",53,0,53)
                hist_down_lowdm = ROOT.TH1F(shortmasspoint + ext + "_down_lowdm", shortmasspoint + " " + ext_names[ext] + " Down LowDM",53,0,53)
                hist_up_highdm = ROOT.TH1F(shortmasspoint + ext + "_up_highdm", shortmasspoint + " " + ext_names[ext] + " HighDM",130,53,183)
                hist_down_highdm = ROOT.TH1F(shortmasspoint + ext + "_down_highdm", shortmasspoint + " " + ext_names[ext] + " Down HighDM",130,53,183)
                sum_up = 0
                sum_dn = 0
                for i in range(183):
                    hist_up.SetBinContent(i+1,syst[shortmasspoint][ext][i][0])
                    hist_down.SetBinContent(i+1,syst[shortmasspoint][ext][i][1])
                    sum_up += syst[shortmasspoint][ext][i][0]
                    sum_dn += syst[shortmasspoint][ext][i][1]
                for i in range(53):
                    hist_up_lowdm.SetBinContent(i+1,syst[shortmasspoint][ext][i][0])
                    hist_down_lowdm.SetBinContent(i+1,syst[shortmasspoint][ext][i][1])
                for i in range(130):
                    hist_up_highdm.SetBinContent(i+1,syst[shortmasspoint][ext][i+53][0])
                    hist_down_highdm.SetBinContent(i+1,syst[shortmasspoint][ext][i+53][1])
                print sum_up/183.0, sum_dn/183.0
                hist_up.SetLineColor(ROOT.kRed)
                hist_up.SetLineWidth(2)
                hist_up.GetYaxis().SetRangeUser(0,2)
                hist_up.GetXaxis().SetTitle("Search bins")
                hist_up.GetYaxis().SetTitle("Systematic Up/Down")
                if ext in small: hist_up.GetYaxis().SetRangeUser(0.75,1.25)
                if ext in testext: hist_up.GetYaxis().SetRangeUser(0,2)
                hist_up.Draw()
                hist_down.SetLineColor(ROOT.kBlue)
                hist_down.SetLineWidth(2)
                hist_down.Draw("same")
                leg.AddEntry(hist_up, "Up","l")
                leg.AddEntry(hist_down, "Down","l")
                leg.Draw("same")
                #mycanvas.SaveAs("VBSB_plots/v6p5_systematics/signal/{0}{1}.png".format(shortmasspoint,ext))
                mycanvas.SaveAs("results/TTbarDMJets/{0}{1}.png".format(shortmasspoint,ext))
                #mycanvas.SaveAs("fastsimstuff/v6p5newstat/{0}_2016/{1}{2}.png".format(samplename,shortmasspoint,ext))
                #c = raw_input("Press Enter to continue")
            '''
            '''
            mycanvas = ROOT.TCanvas(shortmasspoint, shortmasspoint, 600, 600)
            ROOT.gStyle.SetOptStat(0)
            #testext = ["_eff_wtag_fast"]
            testext = ["_eff_wtag","_eff_toptag","_eff_restoptag","_eff_wtag_fast","_eff_toptag_fast","_eff_restoptag_fast","_ak8veto","_ak8veto_fast"]
            small = ["_trig_eff","_PFWeight","_eff_e","_err_mu","_eff_tau","_fastSF","_ISRWeight"]
            for ext in testext:
                leg = ROOT.TLegend(0.6,0.8,0.9,0.89)
                leg.SetBorderSize(0)
                leg.SetTextSize(0.04)
                hist_up = ROOT.TH1F(shortmasspoint + ext + "_up", shortmasspoint + " " + ext_names[ext] + " lepCR",112,0,112)
                hist_down = ROOT.TH1F(shortmasspoint + ext + "_down", shortmasspoint + " " + ext_names[ext] + " Down lepCR",112,0,112)
                hist_up_lowdm = ROOT.TH1F(shortmasspoint + ext + "_up_lowdm", shortmasspoint + " " + ext_names[ext] + " LowDM lepCR",53,0,53)
                hist_down_lowdm = ROOT.TH1F(shortmasspoint + ext + "_down_lowdm", shortmasspoint + " " + ext_names[ext] + " Down LowDM lepCR",53,0,53)
                hist_up_highdm = ROOT.TH1F(shortmasspoint + ext + "_up_highdm", shortmasspoint + " " + ext_names[ext] + " HighDM lepCR",59,53,112)
                hist_down_highdm = ROOT.TH1F(shortmasspoint + ext + "_down_highdm", shortmasspoint + " " + ext_names[ext] + " Down HighDM lepCR",59,53,112)
                for i in range(112):
                    hist_up.SetBinContent(i+1,syst["lepcr_{0}".format(shortmasspoint)][ext][i][0])
                    hist_down.SetBinContent(i+1,syst["lepcr_{0}".format(shortmasspoint)][ext][i][1])
                for i in range(53):
                    hist_up_lowdm.SetBinContent(i+1,syst["lepcr_{0}".format(shortmasspoint)][ext][i][0])
                    hist_down_lowdm.SetBinContent(i+1,syst["lepcr_{0}".format(shortmasspoint)][ext][i][1])
                for i in range(59):
                    hist_up_highdm.SetBinContent(i+1,syst["lepcr_{0}".format(shortmasspoint)][ext][i+53][0])
                    hist_down_highdm.SetBinContent(i+1,syst["lepcr_{0}".format(shortmasspoint)][ext][i+53][1])
                hist_up.SetLineColor(ROOT.kRed)
                hist_up.SetLineWidth(2)
                hist_up.GetYaxis().SetRangeUser(0,2)
                hist_up.GetXaxis().SetTitle("LL CR unit bins")
                hist_up.GetYaxis().SetTitle("Systematic Up/Down")
                if ext in small: hist_up.GetYaxis().SetRangeUser(0.75,1.25)
                if ext in testext: hist_up.GetYaxis().SetRangeUser(0.75,1.25)
                hist_up.Draw()
                hist_down.SetLineColor(ROOT.kBlue)
                hist_down.SetLineWidth(2)
                hist_down.Draw("same")
                leg.AddEntry(hist_up, "Up","l")
                leg.AddEntry(hist_down, "Down","l")
                leg.Draw("same")
                #mycanvas.SaveAs("VBSB_plots/v5signal_nofastSF/{0}{1}.png".format(shortmasspoint,ext))
                mycanvas.SaveAs("fastsimstuff/v6p5newstat/{0}{1}_lepCR.png".format(shortmasspoint,ext))
                #c = raw_input("Press Enter to continue")
            '''
            '''
            mycanvas = ROOT.TCanvas(shortmasspoint, shortmasspoint, 600, 600)
            ROOT.gStyle.SetOptStat(0)
            testext = ["_eff_wtag_fast"]
            #testext = ["_bsf","_trig_eff"]
            small = ["_trig_eff","_PFWeight","_eff_e","_err_mu","_eff_tau","_fastSF","_eff_wtag_fast"]
            for ext in testext:
                leg = ROOT.TLegend(0.4,0.8,0.9,0.89)
                leg.SetBorderSize(0)
                leg.SetTextSize(0.04)
                hist_up = ROOT.TH1F(shortmasspoint + ext + "_vb_up", shortmasspoint + ext + " VB Up",43,0,43)
                hist_down = ROOT.TH1F(shortmasspoint + ext + "_vb_down", shortmasspoint + ext + " VB Down",43,0,43)
                hist_up_lowdm = ROOT.TH1F(shortmasspoint + ext + "_vb_up_lowdm", shortmasspoint + ext + " VB Up LowDM",19,0,19)
                hist_down_lowdm = ROOT.TH1F(shortmasspoint + ext + "_vb_down_lowdm", shortmasspoint + ext + " VB Down LowDM",19,0,19)
                hist_up_highdm = ROOT.TH1F(shortmasspoint + ext + "_vb_up_highdm", shortmasspoint + ext + " VB Up HighDM",24,19,43)
                hist_down_highdm = ROOT.TH1F(shortmasspoint + ext + "_vb_down_highdm", shortmasspoint + ext + " VB Down HighDM",24,19,43)
                for i in range(43):
                    hist_up.SetBinContent(i+1,vb_syst[shortmasspoint][ext][i][0])
                    hist_down.SetBinContent(i+1,vb_syst[shortmasspoint][ext][i][1])
                for i in range(19):
                    hist_up_lowdm.SetBinContent(i+1,vb_syst[shortmasspoint][ext][i][0])
                    hist_down_lowdm.SetBinContent(i+1,vb_syst[shortmasspoint][ext][i][1])
                for i in range(24):
                    hist_up_highdm.SetBinContent(i+1,vb_syst[shortmasspoint][ext][i+19][0])
                    hist_down_highdm.SetBinContent(i+1,vb_syst[shortmasspoint][ext][i+19][1])
                hist_up_highdm.SetLineColor(ROOT.kRed)
                hist_up_highdm.SetLineWidth(2)
                hist_up_highdm.GetYaxis().SetRangeUser(0,2)
                if ext in small: hist_up_highdm.GetYaxis().SetRangeUser(0.75,1.25)
                hist_up_highdm.Draw()
                hist_down_highdm.SetLineColor(ROOT.kBlue)
                hist_down_highdm.SetLineWidth(2)
                hist_down_highdm.Draw("same")
                leg.AddEntry(hist_up_highdm, shortmasspoint + ext + " Up HighDM","l")
                leg.AddEntry(hist_down_highdm, shortmasspoint + ext + " Down HighDM","l")
                leg.Draw("same")
                #mycanvas.SaveAs("VBSB_plots/v5signal/{0}{1}_vb_highdm.png".format(shortmasspoint,ext))
                mycanvas.SaveAs("fastsimstuff/v6p5newstat/{0}{1}_vb_highdm.png".format(shortmasspoint,ext))
                '''
    find_splitb_fullunc = False
    if find_splitb_fullunc:
        extlist = ["_bsf_light","_bsf_heavy","_bsf_light_fast","_bsf_heavy_fast"]
        for point in mass_point_list:
            #print point, systmap[point]["_bsf_light"]["bin_hm_nbeq2_highmtb_nt1_nrt0_nw1_MET_pt250to550"][0], yieldsmap[point]["bin_hm_nbeq2_highmtb_nt1_nrt0_nw1_MET_pt250to550"][0]
            for ext in extlist:
                for k,v in systmap[point][ext].iteritems():
                    if((v[0] == 2.0 or v[1] == 0.01 or v[0] == 0.01 or v[1] == 2.0) and (yieldsmap[point][k][0] != 0.0)):
                        print point, k, ext
                for k,v in systmap["lepcr_{0}".format(point)][ext].iteritems():
                    if((v[0] == 2.0 or v[1] == 0.01 or v[0] == 0.01 or v[1] == 2.0) and (yieldsmap["lepcr_{0}".format(point)][k][0] != 0.0)):
                        print point, k, ext
                for k,v in systmap["qcdcr_{0}".format(point)][ext].iteritems():
                    if((v[0] == 2.0 or v[1] == 0.01 or v[0] == 0.01 or v[1] == 2.0) and (yieldsmap["qcdcr_{0}".format(point)][k][0] != 0.0)):
                        print point, k, ext

    make_outfiles = False
    if make_outfiles:
        j = {}
        j["signals"] = mass_point_list
        j["yieldsMap"] = yieldsmap
        for point in mass_point_list:
            json_outfile = "fastsim_results/v6p5_Jul31/{0}/{1}.json".format(samplename,point)
            #json_outfile = "fastsimstuff/v6p5newstat/{0}/{1}.json".format(samplename,point)
            print "Creating file {0}".format(json_outfile)
            with open(json_outfile,"w") as outfile:
                y = {}
                y["signals"] = [point,]
                y["yieldsMap"] = {}
                y["yieldsMap"][point] = yieldsmap[point]
                y["yieldsMap"]["lepcr_{0}".format(point)] = yieldsmap["lepcr_{0}".format(point)]
                y["yieldsMap"]["qcdcr_{0}".format(point)] = yieldsmap["qcdcr_{0}".format(point)]
                json.dump(y,outfile,sort_keys=True,indent=4,separators=(',',': '))
            syst_outfile = "fastsim_results/v6p5_Jul31/{0}/{1}_syst.conf".format(samplename,point)
            #syst_outfile = "fastsimstuff/v6p5newstat/{0}/{1}_syst.conf".format(samplename,point)
            print "Creating file {0}".format(syst_outfile)
            with open(syst_outfile,"w") as outfile:
                for ext in extensions:
                    for k,v in systmap[point][ext].iteritems():
                        if (v[0] != 1 or v[1] != 1) or (ext not in leptonvetos and ext not in topvetos and (ext != "_ivfunc" and ext != "_ivfunc_fast")):
                            outfile.write("{0}    {1}_Up    {2}    {3}\n".format(k,ext_names[ext],point,v[0]))
                            outfile.write("{0}    {1}_Down    {2}    {3}\n".format(k,ext_names[ext],point,v[1]))
                for k,v in systmap[point]["LHEScale"].iteritems():
                    if v[0] != 1 or v[1] != 1:
                        outfile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHESigScale_Up",point,v[0]))
                        outfile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHESigScale_Down",point,v[1]))
                for ext in extensions:
                    for k,v in systmap["lepcr_{0}".format(point)][ext].iteritems():
                        if (v[0] != 1 or v[1] != 1) or (ext not in leptonvetos and ext not in topvetos and (ext != "_ivfunc" and ext != "_ivfunc_fast")):
                            outfile.write("{0}    {1}_Up    {2}    {3}\n".format(k,ext_names[ext],point,v[0]))
                            outfile.write("{0}    {1}_Down    {2}    {3}\n".format(k,ext_names[ext],point,v[1]))
                for k,v in systmap["lepcr_{0}".format(point)]["LHEScale"].iteritems():
                    if v[0] != 1 or v[1] != 1:
                        outfile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHESigScale_Up",point,v[0]))
                        outfile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHESigScale_Down",point,v[1]))
                for ext in extensions:
                    for k,v in systmap["qcdcr_{0}".format(point)][ext].iteritems():
                        if (v[0] != 1 or v[1] != 1) or (ext not in leptonvetos and ext not in topvetos and (ext != "_ivfunc" and ext != "_ivfunc_fast")):
                            outfile.write("{0}    {1}_Up    {2}    {3}\n".format(k,ext_names[ext],point,v[0]))
                            outfile.write("{0}    {1}_Down    {2}    {3}\n".format(k,ext_names[ext],point,v[1]))
                for k,v in systmap["qcdcr_{0}".format(point)]["LHEScale"].iteritems():
                    if v[0] != 1 or v[1] != 1:
                        outfile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHESigScale_Up",point,v[0]))
                        outfile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHESigScale_Down",point,v[1]))
    
    print_range_Jon = False
    if print_range_Jon:
        extensions.append("LHEScale")
        for point in mass_point_list:
            for ext in extensions:
                #ups = [v[0] for k,v in systmap[point][ext].iteritems()]
                #downs = [v[1] for k,v in systmap[point][ext].iteritems()]
                #ups = np.concatenate([ups[ups>1],1/ups[ups<1]])
                #downs = np.concatenate([downs[downs>1],1/downs[downs<1]])
                #ups = np.log(ups)
                #downs = np.log(downs)
                ups = []
                downs = []
                for k,v in systmap[point][ext].iteritems():
                    if yieldsmap[point][k][0] != 0.0:
                        ups.append(v[0])
                        downs.append(v[1])
                while 1.0 in ups:
                    ups.remove(1.0)
                while 1.0 in downs:
                    downs.remove(1.0)
                for i,v in enumerate(ups):
                    if v < 1: v = 1/v
                    ups[i] = np.log(v-1)
                for i,v in enumerate(downs):
                    if v < 1: v = 1/v
                    downs[i] = np.log(v-1)
                if ups and downs:
                    mean_up = np.mean(np.array(ups))
                    std_up = np.std(np.array(ups))
                    #median_up = np.median(np.array(ups))
                    mean_down = np.mean(np.array(downs))
                    std_down = np.std(np.array(downs))
                    #median_down = np.median(np.array(downs))
                    print point, ext_names[ext]
                    #print "medians:", "up:", np.exp(median_up), "down:", np.exp(median_down)
                    print "up range: {0:.2f}% -- {1:.2f}%".format(100*np.exp(mean_up - std_up), 100*np.exp(mean_up + std_up))
                    print "dn range: {0:.2f}% -- {1:.2f}%".format(100*np.exp(mean_down - std_down), 100*np.exp(mean_down + std_down))

    print_range = True
    if print_range:
        #extensions.append("LHEScale")
        for point in mass_point_list:
            for ext in extensions:
                ups = []
                downs = []
                for k,v in endperc_diff[point][ext].iteritems():
                    if yieldsmap[point][k][0] != 0.0:
                        if (2.0 in v) or (1.99 in v) or (0.01 in v):
                            if raw_yieldsmap[point][k][0] > 10:
                                ups.append(v[0])
                                downs.append(v[1])
                        else:
                            ups.append(v[0])
                            downs.append(v[1])
                            #if ext_names[ext] == "eff_wtag": print v[0], v[1]
                while 1.0 in ups:
                    ups.remove(1.0)
                while 1.0 in downs:
                    downs.remove(1.0)
                if ups and downs:
                    ups.sort()
                    downs.sort()
                    print point, ext_names[ext]
                    max_up = ups[-1]
                    #if max_up == 2.0 or max_up == 1.99: max_up = ups[-2]
                    #if max_up == 2.0 or max_up == 1.99: max_up = ups[-3]
                    #if max_up == 2.0 or max_up == 1.99: max_up = ups[-4]
                    #if max_up == 2.0 or max_up == 1.99: max_up = ups[-5]
                    max_down = downs[-1]
                    #if max_down == 2.0 or max_down == 1.99: max_down = downs[-2]
                    #if max_down == 2.0 or max_down == 1.99: max_down = downs[-3]
                    #if max_down == 2.0 or max_down == 1.99: max_down = downs[-4]
                    #if max_down == 2.0 or max_down == 1.99: max_down = downs[-5]
                    if max_down == 1.99: max_down = 2.0
                    print "up: {0:.2f}% -- {1:.2f}%".format(100*abs(1-ups[0]), 100*abs(1-max_up))
                    print "down: {0:.2f}% -- {1:.2f}%".format(100*abs(1-downs[0]), 100*abs(1-max_down))
                    
    find_large_syst = False
    if find_large_syst:
        for point in mass_point_list:
            for ext in extensions:
                for k,v in systmap[point][ext].iteritems():
                    if yieldsmap[point][k][0] != 0.0:
                        if (2.0 in v) or (0.01 in v):
                            print point, ext_names[ext], raw_yieldsmap[point][k][0], k

