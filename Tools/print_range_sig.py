import ROOT
import argparse
import numpy as np
import pandas as pd

configs = [ "condor/fastsim_configs/SMS_T1ttbb_fastsim_2016.cfg",
            "condor/fastsim_configs/SMS_T1tttt_fastsim_2016.cfg",
            "condor/fastsim_configs/SMS_T2bWC_fastsim_2016.cfg",
            "condor/fastsim_configs/SMS_T2bW_fastsim_2016.cfg",
            "condor/fastsim_configs/SMS_T2cc_fastsim_2016.cfg",
            "condor/fastsim_configs/SMS_T2fbd_fastsim_2016.cfg",
            "condor/fastsim_configs/SMS_T2tb_fastsim_2016.cfg",
            "condor/fastsim_configs/SMS_T2tt_fastsim_2017.cfg",
            "condor/fastsim_configs/SMS_T5ttcc_fastsim_2018.cfg",
            ]
#testconfigs = ["temp/SMS_T2tt_fastsim_2016.cfg"]
testconfigs = ["condor/fastsim_configs/SMS_T2tt_fastsim_2017.cfg"]

extensions = ["_bsf_light","_bsf_heavy","_bsf_light_fast","_bsf_heavy_fast","_trig_eff","_puWeight","_PFWeight","_JES","_METUnClust","_ivfunc","_ivfunc_fast","_eff_e","_err_mu","_eff_tau","_ISRWeight","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag","_eff_wtag_fast","_eff_toptag_fast","_eff_restoptag_fast","_ak8veto","_ak8veto_fast","_eff_densetoptag"]
full_extensions = ["_bsf_light","_bsf_heavy","_bsf_light_fast","_bsf_heavy_fast","_trig_eff","_puWeight","_PFWeight","_JES","_METUnClust","_ivfunc","_ivfunc_fast","_eff_e","_err_mu","_eff_tau","_ISRWeight","_METunc","_eff_wtag","_eff_toptag","_eff_restoptag","_eff_wtag_fast","_eff_toptag_fast","_eff_restoptag_fast","_ak8veto","_ak8veto_fast","_eff_densetoptag","LHEScaleWeight"]
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
                "LHEScaleWeight" : "LHESigScale",
            }
LHE_extensions = ["_LHEScaleWeight_0","_LHEScaleWeight_1","_LHEScaleWeight_3","_LHEScaleWeight_4","_LHEScaleWeight_5","_LHEScaleWeight_7","_LHEScaleWeight_8"]

def percdiff(updown):
    unc = {}
    for k,v in updown.iteritems():
        unc.update({k : [1+abs(1-v[0]),1+abs(1-v[1])]})
    return unc

def percdiffratios(ratios):
    percdiffs = []
    for v in ratios:
        percdiff = [abs(1-v[0]),abs(1-v[1])]
        if percdiff[0] == 0.99: percdiff[0] = 1.0
        if percdiff[1] == 0.99: percdiff[1] = 1.0
        percdiffs.append(percdiff)
    return percdiffs

def bad_systematic(ratios):
    new_ratios = []
    for v in ratios:
        if v[0] > 2.0: v[0] = 2.0
        if v[1] > 2.0: v[1] = 2.0
        if v[0] <= 0.0: v[0] = 0.01
        if v[1] <= 0.0: v[1] = 0.01
        log_ratio_difference = abs(np.log(v[0])) - abs(np.log(v[1]))
        bad_syst = abs(log_ratio_difference) > 0.35
        if bad_syst:
            if log_ratio_difference > 0:
                # UpRatio is larger, so use DownRatio
                v[0] = 1/v[1]
            else:
                # DownRatio is larger, so use UpRatio
                v[1] = 1/v[0]
        if v[0] > 2.0: v[0] = 2.0
        if v[1] > 2.0: v[1] = 2.0
        if v[0] < 0.01: v[0] = 0.01
        if v[1] < 0.01: v[1] = 0.01
        new_ratios.append(v)
    return new_ratios

def one_sided_systematic(ratios):
    new_ratios = []
    for v in ratios:
        one_sided_syst = ((v[0] > 1) and (v[1] > 1)) or ((v[0] < 1) and (v[1] < 1))
        if one_sided_syst:
            geomean = np.sqrt(v[0] * v[1])
            v[0] /= geomean
            v[1] /= geomean
        new_ratios.append(v)
    return new_ratios

def valsFromFile(infilename,extrema,stats):
    #print infilename
    infile = ROOT.TFile(infilename)
    h_sb = infile.Get("h_sb")
    h_sb_raw = infile.Get("h_sb_raw")
    sb_vals = [h_sb.GetBinContent(i+1) for i in range(183)]
    sb_stat = [h_sb.GetBinError(i+1) for i in range(183)]
    sb_raw_vals = [h_sb_raw.GetBinContent(i+1) for i in range(183)]
    for v in sb_vals:
        if v < 0.0: v = 0.0
    for i in range(183):
        if sb_vals[i] > 0.0:
            stats.append(abs(1-(sb_stat[i]/sb_vals[i])))
        else:
            stats.append(0.0)
    for ext in extensions:
        h_ext_up = infile.Get("h_sb{0}_up".format(ext))
        h_ext_down = infile.Get("h_sb{0}_down".format(ext))
        ratios = []
        for i in range(183):
            if (sb_vals[i] > 0.0) and (0.99*sb_vals[i] > sb_stat[i]):
                ratios.append([h_ext_up.GetBinContent(i+1)/sb_vals[i],h_ext_down.GetBinContent(i+1)/sb_vals[i]])
            else:
                ratios.append([1.0,1.0])
                #if sb_vals[i] < sb_stat[i]: print "stat greater than val",sb_vals[i],sb_stat[i]
        ratios = bad_systematic(ratios)
        ratios = one_sided_systematic(ratios)
        #done elsewhere
        #ratios = kill_systematic(ratios,sb_vals,sb_stat)
        updownvals = percdiffratios(ratios)
        '''
        for i in range(183):
            if sb_raw_vals[i] < 15:
                if updownvals[i][0] > 0.9: updownvals[i][0] = 0.0
                if updownvals[i][1] > 0.9: updownvals[i][1] = 0.0
        '''
        vals = []
        for i in range(183):
            if sb_vals[i] > 0.0:
                vals.append(updownvals[i][0])
                vals.append(updownvals[i][1])
        while 0.0 in vals:
            vals.remove(0.0)
        '''
        vals.sort()
        if vals:
            vals2 = [v for v in vals if v > 0.01]
            vals2.sort()
            if vals2:
                extrema[ext].append(vals2[0])
                extrema[ext].append(vals2[-1])
        '''
        for v in vals:
            extrema[ext].append(v)
        
    h_LHEext = []
    upLHEvals = []
    downLHEvals = []
    LHEvals = []
    for LHEext in LHE_extensions:
        h_LHEext.append(infile.Get("h_sb{0}".format(LHEext)))
    for i in range(183):
        ns = []
        d = sb_vals[i]
        for h in h_LHEext:
            n = h.GetBinContent(i+1)
            if n < 0.0: ns.append(0.0)
            else: ns.append(n)
        ns.sort()
        n_up = ns[-1]
        n_down = ns[0]
        #if i == 0: print "ns:", ns, "d:", d, "range:", abs(1-(n_up/d)), abs(1-(n_down/d))
        if d > 0.0:
            if (n_up/d > 2.0): upLHEvals.append(1.0)
            elif (n_up/d < 0.01): upLHEvals.append(1.0)
            else: upLHEvals.append(abs(1-(n_up/d)))
        else: upLHEvals.append(1.0)
        if d > 0.0:
            if (n_down/d > 2.0): downLHEvals.append(1.0)
            elif (n_down/d < 0.01): downLHEvals.append(1.0)
            else: downLHEvals.append(abs(1-(n_down/d)))
        else: downLHEvals.append(1.0)
    for i in range(183):
        if sb_vals[i] > 0.0:
            #if (upLHEvals[i] < 0.9) or (sb_raw_vals[i] > 15): LHEvals.append(upLHEvals[i])
            #if (downLHEvals[i] < 0.9) or (sb_raw_vals[i] > 15): LHEvals.append(downLHEvals[i])
            LHEvals.append(upLHEvals[i])
            LHEvals.append(downLHEvals[i])
    while 0.0 in LHEvals:
        LHEvals.remove(0.0)
    '''
    LHEvals.sort()
    #print LHEvals
    extrema["LHEScaleWeight"].append(LHEvals[0])
    extrema["LHEScaleWeight"].append(LHEvals[-1])
    '''
    for v in LHEvals:
        extrema["LHEScaleWeight"].append(v)

def signal_extrema(config):
    samplename = config.split("/")[-1].split(".")[0][:-5]
    extrema = {}
    d = {}
    for ext in extensions:
        extrema[ext] = []
    extrema["LHEScaleWeight"] = []
    stats = []
    with open(config, "r") as cfgfile:
        for line in cfgfile:
            entries = line.split(", ")
            mass_point = entries[0][:-5]
            if "#" in mass_point: continue
            #splitmasspoint = mass_point.split("_")
            #numbers = re.findall(r'\d+',mass_point) #0th will be 1 or 2 from T1, T2
            #shortmasspoint = "{0}_{1}_{2}".format(splitmasspoint[1],numbers[1],numbers[2])
            #mass_point_list.append(shortmasspoint)
            filename = "results/VBSB/{0}/merged/{1}.root".format(samplename,mass_point)
            valsFromFile(filename,extrema,stats)
    stats.sort()
    max_stat_idx = int(0.95*len(stats))
    print samplename,int(round(100*stats[0])),int(round(100*stats[max_stat_idx])),int(round(100*stats[-1]))
    final = {}
    max_idx = {}
    for ext in full_extensions:
        final[ext] = []
        for v in extrema[ext]:
            if v == v: final[ext].append(v)
        final[ext].sort()
        if not final[ext]:
            final[ext].append(0.0)
            final[ext].append(0.0)
        #print ext_names[ext], "{0:.2f}% -- {1:.2f}%".format(100*extrema[ext][0],100*extrema[ext][-1])
        #print ext_names[ext], "{0}% -- {1}%".format(int(100*extrema[ext][0]),int(100*extrema[ext][-1]))
        #if ext == "_bsf_light": print final[ext]
        max_idx[ext] = int(0.95*len(final[ext]))
        #if ext == "LHEScaleWeight": print final[ext], max_idx[ext]
    #l = ["{0}--{1}%".format(int(100*extrema[ext][0]),int(100*extrema[ext][-1])) for ext in extensions]
    #print "extrema:", extrema["LHEScaleWeight"], extrema["_bsf"]
    l = ["{0}--{1}%".format(int(100*final[ext][0]),int(100*final[ext][max_idx[ext]])) for ext in full_extensions]
    return samplename, l

if __name__ == "__main__":
    d = {}
    for config in configs:
        samplename, l = signal_extrema(config)
        d[samplename] = l
    df = pd.DataFrame(data=d,index=[ext_names[ext] for ext in full_extensions])
    #print df
    print df.to_latex()
