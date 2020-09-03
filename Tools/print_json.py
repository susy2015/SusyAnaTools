import ROOT
import json
import os
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
        sign = 1
        othersign = 1
        if v[0] < 1: sign = -1
        if v[1] < 1: othersign = -1
        if sign != othersign:
            val = [abs(v[0]-1),abs(v[1]-1)]
            unc.update({k : [1 + sign*val[0],1 - sign*val[1]]})
        else:
            val = 0.5 * (abs(v[0]-1) + abs(1-v[1])) #average of absolute difference
            if abs(v[0]) > abs(v[1]): unc.update({k: [1 + sign*val,1 - sign*val]})
            else: unc.update({k: [1 - sign*val,1 + sign*val]})
        '''
        #Now letting Matt handle same-side uncertainties
        unc.update({k : [v[0],v[1]]})
    return unc

def bad_systematic(v):
    log_ratio_difference = abs(np.log(v[0])) - abs(np.log(v[1]))
    bad_syst = abs(log_ratio_difference) > 0.35
    if bad_syst:
        if log_ratio_difference > 0:
            # UpRatio is larger, so use DownRatio
            v[0] = 1/v[1]
        else:
            # DownRatio is larger, so use UpRatio
            v[1] = 1/v[0]
    return v

def one_sided_systematic(v):
    one_sided_syst = ((v[0] > 1) and (v[1] > 1)) or ((v[0] < 1) and (v[1] < 1))
    if one_sided_syst:
        geomean = np.sqrt(v[0] * v[1])
        v[0] /= geomean
        v[1] /= geomean
    return v

def percdiff(updown,ext):
    unc = {}
    for k,v in updown.iteritems():
        if ext == "_JES" and k == "bin_lm_nb1_nivf1_lowmtb_medptisr_lowptb_MET_pt400to500": print v
        v = bad_systematic(v)
        if ext == "_JES" and k == "bin_lm_nb1_nivf1_lowmtb_medptisr_lowptb_MET_pt400to500": print v
        v = one_sided_systematic(v)
        if ext == "_JES" and k == "bin_lm_nb1_nivf1_lowmtb_medptisr_lowptb_MET_pt400to500": print v
        #kill_systematic is taken care of later
        #if 0.99*yields[k][0] < yields[k][1]: v = [1.0,1.0]
        unc.update({k : [abs(1-v[0]),abs(1-v[1])]})
    return unc


def myprocess(sample):
    jsonFile = "dc_BkgPred_BinMaps_master.json"
    with open(jsonFile, "r") as inputFile:
        unitMap = json.load(inputFile)
        binMap = {}
        for k,v in unitMap["binNum"].iteritems():
            binMap.update({int(v) : k})

        #binMap = {int(v): k for k, v in unitMap["binNum"].iteritems()}
    #print binMap[0]

    #infile = ROOT.TFile("TTZ_VB_2016.root")
    #infile = ROOT.TFile("TTZ_VB_1617.root")
    infile = ROOT.TFile("TTZ_VB.root")
    h_sb_low  = infile.Get(sample + "_sb_low")
    h_sb_high = infile.Get(sample + "_sb_high")
    #h_sb_low_avgwtsq = infile.Get(sample + "_sb_low_avg_weight_sq")
    #h_sb_high_avgwtsq = infile.Get(sample + "_sb_high_avg_weight_sq")

    extensions = ["_bsf_light","_bsf_heavy","_trig_eff","_puWeight","_PFWeight","_pdfWeight","_JES","_METUnClust","_ivfunc","_eff_e","_err_mu","_eff_tau","_eff_wtag","_eff_toptag","_eff_restoptag","_ak8veto","_eff_densetoptag"]
    #extensions = ["_bsf","_trig_eff","_puWeight","_PFWeight","_pdfWeight","_JES","_METUnClust","_ivfunc","_eff_e","_err_mu","_eff_tau","_eff_wtag","_eff_toptag","_eff_restoptag","_ak8veto","_eff_densetoptag"]
    longextensions = ["_bsf_light","_bsf_heavy","_trig_eff","_puWeight","_PFWeight","_pdfWeight","_JES","_METUnClust","_ivfunc","_eff_e","_err_mu","_eff_tau","_eff_wtag","_eff_toptag","_eff_restoptag","_ak8veto","_eff_densetoptag","LHEScale"]
    leptonvetos = ["_eff_e","_err_mu","_eff_tau"]
    topvetos = ["_eff_wtag","_eff_toptag","_eff_restoptag"]
    ext_names = {   "_bsf" : "b",
                    "_bsf_light" : "b_light",
                    "_bsf_heavy" : "b_heavy",
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
                    "_ak8veto" : "eff_fatjet_veto",
                    "_eff_densetoptag" : "eff_densetoptag",
                    "_lumi" : "lumi",
                    "LHEScale" : "LHEScale",
                }
    if sample == "TTZ":
        extensions.append("_SF")
        ext_names.update({"_SF" : "TTZ_SF"})

    #rate
    #dict_sb = {i : [h_sb_low.GetBinContent(i+1),h_sb_low.GetBinError(i+1)] for i in range(53)}
    dict_sb = {}
    for i in range(53):
        dict_sb.update({i : [h_sb_low.GetBinContent(i+1),h_sb_low.GetBinError(i+1)]})
    #dict_sb.update({i+53 : [h_sb_high.GetBinContent(i+1),h_sb_high.GetBinError(i+1)] for i in range(130)})
    for i in range(130):
        dict_sb.update({i+53 : [h_sb_high.GetBinContent(i+1),h_sb_high.GetBinError(i+1)]})
    #Ensure non-negative
    for k,v in dict_sb.iteritems():
        if dict_sb[k][0] <= 0: dict_sb[k] = [1e-06,1e-06]
    #if sample == "TTZ":
    #    for k in dict_sb:
    #        dict_sb.update({k : [dict_sb[k][0] * 1.214/1.14066, dict_sb[k][1] * 1.214/1.14066]})

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

    #predMap = {binMap.get(k,k) : v for k,v in dict_sb.iteritems()}
    predMap = {}
    for k,v in dict_sb.iteritems():
        #print k,v
        predMap.update({binMap.get(k,k) : v})
    #predMap_round = {binMap.get(k,k) : v for k,v in dict_sb_round.iteritems()}

    j = {}
    j["yieldsMap"] = predMap
    #j["yieldsMap"] = predMap_round
    statuncs = []
    for k,v in predMap.iteritems():
        statuncs.append(abs(1-(v[1]/v[0])))
    statuncs.sort()
    minstat = statuncs[0]
    maxstat = statuncs[-1]   
    ninefivemaxstat = statuncs[int(0.95*len(statuncs))]
    print sample,"stats:",100*minstat,100*maxstat,100*ninefivemaxstat

    #systematic uncertainties
    sys_sb_low = {}
    sys_sb_high = {}
    sys_updown = {}
    syst_unc = {}
    syst_unc_map = {}
    syst_updown = {}
    perc_diff = {}
    perc_diff_map = {}
    for ext in extensions:
        #print ext
        sys_sb_low[ext] = [infile.Get(sample + "_sb_low" + ext + "_up"),infile.Get(sample + "_sb_low" + ext + "_down")]
        sys_sb_high[ext] = [infile.Get(sample + "_sb_high" + ext + "_up"),infile.Get(sample + "_sb_high" + ext + "_down")]
        #if sample == "TTZ":
        #    #1.214/1.14066
        #    sys_sb_low[ext][0].Scale(1.214/1.14066)
        #    sys_sb_low[ext][1].Scale(1.214/1.14066)
        #    sys_sb_high[ext][0].Scale(1.214/1.14066)
        #    sys_sb_high[ext][1].Scale(1.214/1.14066)
        for i in range(53):
            #then divide by nominal
            if dict_sb[i][0] > 0:
                #if sys_sb_low[ext][0].GetBinContent(i+1)/dict_sb[i][0] > sys_sb_low[ext][1].GetBinContent(i+1)/dict_sb[i][0]:
                #    syst_updown.update({i : [sys_sb_low[ext][0].GetBinContent(i+1)/dict_sb[i][0],sys_sb_low[ext][1].GetBinContent(i+1)/dict_sb[i][0]]})
                #else: syst_updown.update({i : [sys_sb_low[ext][1].GetBinContent(i+1)/dict_sb[i][0],sys_sb_low[ext][0].GetBinContent(i+1)/dict_sb[i][0]]})
                syst_updown.update({i : [sys_sb_low[ext][0].GetBinContent(i+1)/dict_sb[i][0],sys_sb_low[ext][1].GetBinContent(i+1)/dict_sb[i][0]]})
            else: syst_updown.update({i : [2.0,0.0]})
        for i in range(130):
            if dict_sb[i+53][0] > 0:
                #if sys_sb_high[ext][0].GetBinContent(i+1)/dict_sb[i+53][0] > sys_sb_high[ext][1].GetBinContent(i+1)/dict_sb[i+53][0]:
                #    syst_updown.update({i+53 : [sys_sb_high[ext][0].GetBinContent(i+1)/dict_sb[i+53][0],sys_sb_high[ext][1].GetBinContent(i+1)/dict_sb[i+53][0]]})
                #else: syst_updown.update({i+53 : [sys_sb_high[ext][1].GetBinContent(i+1)/dict_sb[i+53][0],sys_sb_high[ext][0].GetBinContent(i+1)/dict_sb[i+53][0]]})
                syst_updown.update({i+53 : [sys_sb_high[ext][0].GetBinContent(i+1)/dict_sb[i+53][0],sys_sb_high[ext][1].GetBinContent(i+1)/dict_sb[i+53][0]]})
            else: syst_updown.update({i+53 : [2.0,0.0]})
        #syst_unc[ext] = combineupdown(syst_updown)
        syst_unc[ext] = simupdown(syst_updown)
        for i in range(183):
            if dict_sb[i][0] == 1e-06:
                syst_unc[ext][i] = [2,0.01]
                #syst_unc[ext][i] = [2,0.5]
        #if (ext in topvetos):
        #if (ext in leptonvetos) or (ext in topvetos):
        if (ext in leptonvetos):
            for i in range(53):
                syst_unc[ext][i] = [1,1]
        if ext in leptonvetos:
            for i in range(53,183):
                syst_unc[ext][i] = [1,1]
        if ext == "_ivfunc":
            for i in range(53,183):
                syst_unc[ext][i] = [1,1]
        if ext == "_SF":
            for i in range(183):
                syst_unc[ext][i] = [1.0821,1.-0.0821]
        for i in range(183):
            if syst_unc[ext][i][0] > 2.0: syst_unc[ext][i][0] = 2.0
            if syst_unc[ext][i][1] > 2.0: syst_unc[ext][i][1] = 2.0
            if syst_unc[ext][i][0] < 0.01: syst_unc[ext][i][0] = 0.01
            if syst_unc[ext][i][1] < 0.01: syst_unc[ext][i][1] = 0.01
            #if syst_unc[ext][i][0] < 0.5: syst_unc[ext][i][0] = 0.5
            #if syst_unc[ext][i][1] < 0.5: syst_unc[ext][i][1] = 0.5
        syst_unc_map[ext] = {binMap.get(k,k) : v for k,v in syst_unc[ext].iteritems()}
        if ext == "_JES": print "syst:",syst_unc_map[ext]["bin_lm_nb1_nivf1_lowmtb_medptisr_lowptb_MET_pt400to500"]
        #perc_diff_map[ext] = percdiff(syst_unc_map[ext])
        #for k,v in syst_unc[ext].iteritems():
        #    #print k
        #    syst_unc_map[ext].update({binMap.get(k,k) : v})
        #j["syst_unc" + ext] = syst_unc_map[ext]
    
    LHEScaleWeight_low = {}
    LHEScaleWeight_high = {}
    nums = [0,1,3,4,5,7,8]
    for n in nums:
        LHEScaleWeight_low[n] = infile.Get(sample + "_sb_low_LHEScaleWeight_" + str(n))
        LHEScaleWeight_high[n] = infile.Get(sample + "_sb_high_LHEScaleWeight_" + str(n))
        LHEScaleWeight_low[n].Divide(h_sb_low)
        LHEScaleWeight_high[n].Divide(h_sb_high)
    for i in range(53):
        if dict_sb[i][0] > 0:
            temp_low_min = LHEScaleWeight_low[0].GetBinContent(i+1)
            temp_low_max = LHEScaleWeight_low[0].GetBinContent(i+1)
            for n in nums:
                temp_low_min = min(temp_low_min,LHEScaleWeight_low[n].GetBinContent(i+1))
                temp_low_max = max(temp_low_max,LHEScaleWeight_low[n].GetBinContent(i+1))
            syst_updown.update({i : [temp_low_max, temp_low_min]})
        else: syst_updown.update({i : [2.0,0.01]})
        #else: syst_updown.update({i : [2.0,0.5]})
    for i in range(130):
        if dict_sb[i][0] > 0:
            temp_high_min = LHEScaleWeight_high[0].GetBinContent(i+1)
            temp_high_max = LHEScaleWeight_high[0].GetBinContent(i+1)
            for n in nums:
                temp_high_min = min(temp_high_min,LHEScaleWeight_high[n].GetBinContent(i+1))
                temp_high_max = max(temp_high_max,LHEScaleWeight_high[n].GetBinContent(i+1))
            syst_updown.update({i+53: [temp_high_max, temp_high_min]})
        else: syst_updown.update({i+53 : [2.0,0.01]})
        #else: syst_updown.update({i+53 : [2.0,0.5]})
    syst_unc["LHEScale"] = simupdown(syst_updown)
    for i in range(183):
        if syst_unc["LHEScale"][i][0] > 2.0: syst_unc["LHEScale"][i][0] = 2.0
        #if syst_unc["LHEScale"][i][0] < 0.5: syst_unc["LHEScale"][i][0] = 2.0
        if syst_unc["LHEScale"][i][0] < 0.01: syst_unc["LHEScale"][i][0] = 2.0
        if syst_unc["LHEScale"][i][1] > 2.0: syst_unc["LHEScale"][i][1] = 0.01
        #if syst_unc["LHEScale"][i][1] > 2.0: syst_unc["LHEScale"][i][1] = 0.5
        if syst_unc["LHEScale"][i][1] < 0.01: syst_unc["LHEScale"][i][1] = 0.01
        #if syst_unc["LHEScale"][i][1] < 0.5: syst_unc["LHEScale"][i][1] = 0.5
    syst_unc_map["LHEScale"] = {binMap.get(k,k) : v for k,v in syst_unc["LHEScale"].iteritems()}
    #perc_diff_map["LHEScale"] = percdiff(syst_unc_map["LHEScale"])
    '''
    LHEScale_low_Both_Up = infile.Get(sample + "_sb_low_LHEScaleWeight_8")
    LHEScale_low_Both_Down = infile.Get(sample + "_sb_low_LHEScaleWeight_0")
    LHEScale_low_Renorm_Up = infile.Get(sample + "_sb_low_LHEScaleWeight_7")
    LHEScale_low_Renorm_Down = infile.Get(sample + "_sb_low_LHEScaleWeight_1")
    LHEScale_low_Factor_Up = infile.Get(sample + "_sb_low_LHEScaleWeight_5")
    LHEScale_low_Factor_Down = infile.Get(sample + "_sb_low_LHEScaleWeight_3")
    LHEScale_high_Both_Up = infile.Get(sample + "_sb_high_LHEScaleWeight_8")
    LHEScale_high_Both_Down = infile.Get(sample + "_sb_high_LHEScaleWeight_0")
    LHEScale_high_Renorm_Up = infile.Get(sample + "_sb_high_LHEScaleWeight_7")
    LHEScale_high_Renorm_Down = infile.Get(sample + "_sb_high_LHEScaleWeight_1")
    LHEScale_high_Factor_Up = infile.Get(sample + "_sb_high_LHEScaleWeight_5")
    LHEScale_high_Factor_Down = infile.Get(sample + "_sb_high_LHEScaleWeight_3")
    Both_updown = {}
    Renorm_updown = {}
    Factor_updown = {}
    for i in range(53):
        if dict_sb[i][0] > 0:
            Both_updown.update({i : [LHEScale_low_Both_Up.GetBinContent(i+1)/dict_sb[i][0],LHEScale_low_Both_Down.GetBinContent(i+1)/dict_sb[i][0]]})
            Renorm_updown.update({i : [LHEScale_low_Renorm_Up.GetBinContent(i+1)/dict_sb[i][0],LHEScale_low_Renorm_Down.GetBinContent(i+1)/dict_sb[i][0]]})
            Factor_updown.update({i : [LHEScale_low_Factor_Up.GetBinContent(i+1)/dict_sb[i][0],LHEScale_low_Factor_Down.GetBinContent(i+1)/dict_sb[i][0]]})
        else:
            Both_updown.update({i : [2.0,0.01]})
            Renorm_updown.update({i : [2.0,0.01]})
            Factor_updown.update({i : [2.0,0.01]})
    for i in range(130):
        if dict_sb[i+53][0] > 0:
            Both_updown.update({i+53 : [LHEScale_low_Both_Up.GetBinContent(i+1)/dict_sb[i+53][0],LHEScale_low_Both_Down.GetBinContent(i+1)/dict_sb[i+53][0]]})
            Renorm_updown.update({i+53 : [LHEScale_low_Renorm_Up.GetBinContent(i+1)/dict_sb[i+53][0],LHEScale_low_Renorm_Down.GetBinContent(i+1)/dict_sb[i+53][0]]})
            Factor_updown.update({i+53 : [LHEScale_low_Factor_Up.GetBinContent(i+1)/dict_sb[i+53][0],LHEScale_low_Factor_Down.GetBinContent(i+1)/dict_sb[i+53][0]]})
        else:
            Both_updown.update({i+53 : [2.0,0.01]})
            Renorm_updown.update({i+53 : [2.0,0.01]})
            Factor_updown.update({i+53 : [2.0,0.01]})
    syst_unc["LHEScale_Both"] = simupdown(Both_updown)
    syst_unc["LHEScale_Renorm"] = simupdown(Renorm_updown)
    syst_unc["LHEScale_Factor"] = simupdown(Factor_updown)
    for i in range(183):
        if syst_unc["LHEScale_Both"][i][0] > 2.0: syst_unc["LHEScale_Both"][i][0] = 2.0
        if syst_unc["LHEScale_Both"][i][1] > 2.0: syst_unc["LHEScale_Both"][i][1] = 2.0
        if syst_unc["LHEScale_Both"][i][0] < 0.01: syst_unc["LHEScale_Both"][i][0] = 0.01
        if syst_unc["LHEScale_Both"][i][1] < 0.01: syst_unc["LHEScale_Both"][i][1] = 0.01
        if syst_unc["LHEScale_Renorm"][i][0] > 2.0: syst_unc["LHEScale_Renorm"][i][0] = 2.0
        if syst_unc["LHEScale_Renorm"][i][1] > 2.0: syst_unc["LHEScale_Renorm"][i][1] = 2.0
        if syst_unc["LHEScale_Renorm"][i][0] < 0.01: syst_unc["LHEScale_Renorm"][i][0] = 0.01
        if syst_unc["LHEScale_Renorm"][i][1] < 0.01: syst_unc["LHEScale_Renorm"][i][1] = 0.01
        if syst_unc["LHEScale_Factor"][i][0] > 2.0: syst_unc["LHEScale_Factor"][i][0] = 2.0
        if syst_unc["LHEScale_Factor"][i][1] > 2.0: syst_unc["LHEScale_Factor"][i][1] = 2.0
        if syst_unc["LHEScale_Factor"][i][0] < 0.01: syst_unc["LHEScale_Factor"][i][0] = 0.01
        if syst_unc["LHEScale_Factor"][i][1] < 0.01: syst_unc["LHEScale_Factor"][i][1] = 0.01
    syst_unc_map["LHEScale_Both"] = {binMap.get(k,k) : v for k,v in syst_unc["LHEScale_Both"].iteritems()}
    syst_unc_map["LHEScale_Renorm"] = {binMap.get(k,k) : v for k,v in syst_unc["LHEScale_Renorm"].iteritems()}
    syst_unc_map["LHEScale_Factor"] = {binMap.get(k,k) : v for k,v in syst_unc["LHEScale_Factor"].iteritems()}
    '''
    #for k,v in syst_unc_map["_eff_wtag"].iteritems():
    #    if "nw0" in k:
    #        syst_unc_map["_eff_wtag"][k] = [1,1]
    #for k,v in syst_unc_map["_eff_toptag"].iteritems():
    #    if "nt0" in k:
    #        syst_unc_map["_eff_toptag"][k] = [1,1]
    #for k,v in syst_unc_map["_eff_restoptag"].iteritems():
    #    if "nrt0" in k:
    #        syst_unc_map["_eff_restoptag"][k] = [1,1]
    syst_unc["_lumi"] = {}
    for i in range(183):
        syst_unc["_lumi"][i] = [1.018,0.982]
    syst_unc_map["_lumi"] = {binMap.get(k,k) : v for k,v in syst_unc["_lumi"].iteritems()}
    extensions.append("_lumi")
    for ext in longextensions:
        perc_diff_map[ext] = percdiff(syst_unc_map[ext],ext)
        if ext == "_JES": print "percdiff:", perc_diff_map[ext]["bin_lm_nb1_nivf1_lowmtb_medptisr_lowptb_MET_pt400to500"]
    
    plot_hists = False
    if plot_hists:
        outdir1 = os.path.abspath("VBSB_plots/v6p5_splitb")
        if not os.path.exists(outdir1):
            os.makedirs(outdir1)
        
        mycanvas = ROOT.TCanvas(sample, sample, 600, 600)
        ROOT.gStyle.SetOptStat(0)
        leg = ROOT.TLegend(0.4,0.8,0.9,0.89)
        leg.SetBorderSize(0)
        leg.SetTextSize(0.04)
        
        #ROOT.gPad.SetLogy()
        #hist_yield = ROOT.TH1F(sample + "_yield", sample + " Yield",183,0,183)
        #for i in range(183):
        #    hist_yield.SetBinContent(i+1,dict_sb[i][0])
        #    hist_yield.SetBinError(i+1,dict_sb[i][1])
        #hist_yield.SetLineWidth(2)
        #hist_yield.Draw()
        #leg.AddEntry(hist_yield, sample + " Yield")
        #mycanvas.SaveAs("VBSB_plots/v6/{0}_yield.png".format(sample))
        #raw_input('Press enter to continue')
        
        
        #testext = ["_bsf_light","_bsf_heavy"]
        testext = ["_bsf"]
        #testext = ["_JES","_METUnClust"]
        small = ["_trig_eff","_PFWeight","_eff_e","_err_mu","_eff_tau"]
        common_names =  {   "_JES" : "JES",
                            "_METUnClust" : "Unclustered MET",
                        }
        for ext in testext:
            leg = ROOT.TLegend(0.6,0.8,0.9,0.89)
            leg.SetBorderSize(0)
            leg.SetTextSize(0.04)
            hist_up = ROOT.TH1F(sample + ext + "_up", sample + " " + ext_names[ext],183,0,183)
            hist_down = ROOT.TH1F(sample + ext + "_down", sample + " " + ext_names[ext] + " Down",183,0,183)
            sum_up = 0
            sum_dn = 0
            for i in range(183):
                hist_up.SetBinContent(i+1,syst_unc[ext][i][0])
                hist_down.SetBinContent(i+1,syst_unc[ext][i][1])
                sum_up += syst_unc[ext][i][0]
                sum_dn += syst_unc[ext][i][1]
            print sum_up/183.0, sum_dn/183.0
            hist_up.SetLineColor(ROOT.kBlue)
            hist_up.SetLineWidth(2)
            hist_up.GetYaxis().SetRangeUser(0,2)
            hist_up.GetXaxis().SetTitle("Search Bins")
            hist_up.GetYaxis().SetTitle("Systematic Up/Down")
            if ext in small: hist_up.GetYaxis().SetRangeUser(0.8,1.2)
            hist_up.Draw()
            hist_down.SetLineColor(ROOT.kRed)
            hist_down.SetLineWidth(2)
            #hist_down.Draw("same")
            leg.AddEntry(hist_up, "Up","l")
            #leg.AddEntry(hist_down, "Down","l")
            leg.Draw("same")
            mycanvas.SaveAs("VBSB_plots/v6p5_splitb/{0}{1}.png".format(sample,ext))
            #c = raw_input('Press enter to continue')
    
    make_outfiles = False
    if make_outfiles:
        outdir2 = os.path.abspath("VBSB_results/v7/")
        if not os.path.exists(outdir2):
            os.makedirs(outdir2)
        
        outfile = "VBSB_results/v7/{0}_pred.json".format(sample)
        #outfile = "fastsim_results/v7/TTZRare/{0}_pred.json".format(sample)
        #outfile = "results/TTZ_VBSB_nogenlepveto/{0}_pred.json".format(sample)
        print "Creating file {0}".format(outfile)
        with open(outfile, "w") as outputFile:
            json.dump(j,outputFile,sort_keys=True,indent=4,separators=(',', ': '))
        
        syst_outfile = "VBSB_results/v7/{0}_syst.conf".format(sample)
        #syst_outfile = "fastsim_results/v7/TTZRare/{0}_syst.conf".format(sample)
        #syst_outfile = "results/TTZ_VBSB_nogenlepveto/{0}_syst.conf".format(sample)
        print "Creating file {0}".format(syst_outfile)
        with open(syst_outfile,"w") as outputFile:
            for ext in extensions:
                for k,v in syst_unc_map[ext].iteritems():
                    if v[0] != 1 or v[1] != 1:
                        outputFile.write("{0}\t{1}\t{2}\t{3}\n".format(k,ext_names[ext] + "_Up",sample,v[0]))
                        outputFile.write("{0}\t{1}\t{2}\t{3}\n".format(k,ext_names[ext] + "_Down",sample,v[1]))
            for k,v in syst_unc_map["LHEScale"].iteritems():
                if v[0] != 1 or v[1] != 1:
                    outputFile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHEScale_Up",sample,v[0]))
                    outputFile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHEScale_Down",sample,v[1]))

            #for k,v in syst_unc_map["LHEScale_Both"].iteritems():
            #    if v[0] != 1 and v[1] != 1:
            #        outputFile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHEScale_Both_Up",sample,v[0]))
            #        outputFile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHEScale_Both_Down",sample,v[1]))
            #for k,v in syst_unc_map["LHEScale_Renorm"].iteritems():
            #    if v[0] != 1 and v[1] != 1:
            #        outputFile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHEScale_Renorm_Up",sample,v[0]))
            #        outputFile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHEScale_Renorm_Down",sample,v[1]))
            #for k,v in syst_unc_map["LHEScale_Factor"].iteritems():
            #    if v[0] != 1 and v[1] != 1:
            #        outputFile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHEScale_Factor_Up",sample,v[0]))
            #        outputFile.write("{0}\t{1}\t{2}\t{3}\n".format(k,"LHEScale_Factor_Down",sample,v[1]))
    
    #print syst_unc_map["_JES"]["bin_hm_nb1_highmtb_nt0_nrtgeq1_nw0_htgt1500_MET_pt350to450"]    
    print_range_Jon = False
    if print_range_Jon:
        for ext in extensions:
            ups = [v[0] for k,v in syst_unc_map[ext].iteritems()]
            downs = [v[1] for k,v in syst_unc_map[ext].iteritems()]
            ups = np.concatenate([ups[ups>1],1/ups[ups<1]])
            downs = np.concatenate([downs[downs>1],1/downs[downs<1]])
            ups = np.log(ups)
            downs = np.log(downs)
            #while 1.0 in ups:
            #    ups.remove(1.0)
            #while 1.0 in downs:
            #    downs.remove(1.0)
            #for i,v in enumerate(ups):
            #    if v < 1: v = 1/v
            #    ups[i] = np.log(v-1)
            #for i,v in enumerate(downs):
            #    if v < 1: v = 1/v
            #    downs[i] = np.log(v-1)
            if ups and downs:
                mean_up = np.mean(np.array(ups))
                std_up = np.std(np.array(ups))
                median_up = np.median(np.array(ups))
                mean_down = np.mean(np.array(downs))
                std_down = np.std(np.array(downs))
                median_down = np.median(np.array(downs))
                print sample, ext_names[ext]
                print "medians:", "up:", np.exp(median_up), "down:", np.exp(median_down)
                print "up range:", np.exp(mean_up - std_up), np.exp(mean_up + std_up)
                print "dn range:", np.exp(mean_down - std_down), np.exp(mean_down + std_down)
    print_range = True
    if print_range:
        
        for ext in longextensions:
            ups = []
            downs = []
            for k,v in perc_diff_map[ext].iteritems():
                #if (0.99*predMap[k][0] >= predMap[k][1]): #no change from below
                if (predMap[k][0] > 1e-06) and (0.99*predMap[k][0] >= predMap[k][1]):
                    ups.append(v[0])
                    downs.append(v[1])
            while 0.0 in ups:
                ups.remove(0.0)
            while 0.0 in downs:
                downs.remove(0.0)
            '''
            for i,v in enumerate(ups):
                if v < 1: ups[i] = 2 - v
            for i,v in enumerate(downs):
                if v < 1: downs[i] = 2 - v
            '''
            if ups and downs:
                ups.sort()
                downs.sort()
                totals = []
                for up in ups: totals.append(up)
                for dn in downs: totals.append(dn)
                totals.sort()
                #print sample, ext_names[ext]
                #print "up: ", np.amin(np.array(ups)), np.amax(np.array(ups))
                #print "down: ", np.amin(np.array(downs)), np.amax(np.array(downs))
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
                if max_down == 0.99: max_down = 1.0
                #print "up: {0}% -- {1}%".format(int(100*abs(1-ups[0])), int(100*abs(1-max_up)))
                #print "down: {0}% -- {1}%".format(int(100*abs(1-downs[0])), int(100*abs(1-max_down)))
                #max_idx = int(0.95*len(totals))
                max_idx = -1
                maximum = totals[max_idx]
                #if maximum == 0.99: maximum = 1.0
                print sample,ext_names[ext],"{0}%--{1}%".format(int(round(100*abs(totals[0]))),int(round(100*abs(maximum))))

if __name__ == "__main__":
    myprocess("TTZ")
    myprocess("Rare")
    myprocess("Comb")

