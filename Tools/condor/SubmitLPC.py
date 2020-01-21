#!/usr/bin/env python
# coding: utf-8

import os
import re
import time
import subprocess
import glob
import tarfile
import shutil
import getpass
import argparse
from collections import defaultdict

#DelExe    = '../tupleReadNormEx'
#DelExe    = '../tupleReadElec'
DelExe    = '../SB_reader'
OutDir = '/store/user/%s/StopStudy' %  getpass.getuser()
tempdir = '/uscms_data/d3/%s/condor_temp/' % getpass.getuser()
#ShortProjectName = 'TTZNormEx'
#ShortProjectName = 'TTZElec'
ShortProjectName = 'VBSBv5_nolepvetoSF'
argument = "%s.$(Process).list %s_$(Process).root"
# argument = "--inputFiles=%s.$(Process).list --outputFile=%s_$(Process).root --jettype=L1PuppiJets"
defaultLperFile = 5
run_everything = False
if run_everything: defaultLperFile = 100
year = "2016"
#year = "2017"
#year = "2018"
#if args.era != "":
#    year = args.era

process_dict = {
##Rare would include TTZToQQ, Diboson, Triboson, Higgs. Also include DY?
#    "TTZToLLNuNu" : ["TTZToLLNuNu"],
#    "ttHToNonbb" : ["ttHToNonbb"],
#    "Normalization" : ["TTZToLLNuNu","ttHToNonbb","VHToNonbb","GluGluHToZZTo4L","ST_tWll","tZq_ll","TTWJetsToLNu","WZTo3LNu","ZZTo4L","WWZ","WZZ","ZZZ","WZG","WWG","WWW","TTTT","TTbarDiLep","DYJetsToLL_HT_70to100","DYJetsToLL_HT_100to200","DYJetsToLL_HT_200to400","DYJetsToLL_HT_400to600","DYJetsToLL_HT_600to800","DYJetsToLL_HT_800to1200","DYJetsToLL_HT_1200to2500","DYJetsToLL_HT_2500toInf"],
#	"TTbar" : ["TTbarSingleLepT", "TTbarSingleLepTbar", "TTbarDiLep"],
#	"WJets" : ["WJetsToLNu_HT_70to100", "WJetsToLNu_HT_100to200", "WJetsToLNu_HT_200to400", "WJetsToLNu_HT_400to600", "WJetsToLNu_HT_600to800", "WJetsToLNu_HT_800to1200", "WJetsToLNu_HT_1200to2500", "WJetsToLNu_HT_2500toInf"],
#	"ZJets" : ["ZJetsToNuNu_HT_100to200", "ZJetsToNuNu_HT_200to400", "ZJetsToNuNu_HT_400to600", "ZJetsToNuNu_HT_600to800", "ZJetsToNuNu_HT_800to1200", "ZJetsToNuNu_HT_1200to2500", "ZJetsToNuNu_HT_2500toInf"],
#	"QCD" : ["QCD_HT_100to200", "QCD_HT_200to300", "QCD_HT_300to500", "QCD_HT_500to700", "QCD_HT_700to1000", "QCD_HT_1000to1500", "QCD_HT_1500to2000", "QCD_HT_2000toInf"],
#	"TopAssoc" : ["ST_tW_top_incl", "ST_tW_antitop_incl", "ST_s_lep", "ST_t_top", "ST_t_antitop", "TTZToLLNuNu", "TTZToQQ"],
##	"TopAssoc" : ["ST_tW_top_incl", "ST_tW_antitop_incl", "ST_s_lep", "ST_t_top", "ST_t_antitop", "TTZToQQ"],
#	"TopAssoc_ext" : ["TTWJetsToLNu","TTWJetsToQQ","TTTT","TTGJets","ST_tWll","ST_tWnunu","tZq_ll"],
#    "resubmit" : ["ttHToNonbb","WWTo2L2Nu","WWTo4Q","VHToNonbb","WZ","ZZZ"],
#   "TTZToQQ" : ["TTZToQQ"],
#	"Diboson" : ["ZZTo2Q2Nu", "ZZTo2L2Nu", "WZ", "WWTo2L2Nu", "WWToLNuQQ"],
#	"Diboson_ext" : ["WWTo4Q","WZTo1L1Nu2Q","WZTo1L3Nu","WZTo3LNu","ZZTo2L2Q","ZZTo4Q","ZZTo4L"],
#	"Triboson" : ["WWW","WWZ","WZZ","ZZZ","WZG","WWG"],
#	"Higgs" : ["ttHTobb","ttHToNonbb","VHToNonbb","GluGluHToZZTo4L"],
#	"DY" : ["DYJetsToLL_HT_70to100","DYJetsToLL_HT_100to200","DYJetsToLL_HT_200to400","DYJetsToLL_HT_400to600","DYJetsToLL_HT_600to800","DYJetsToLL_HT_800to1200","DYJetsToLL_HT_1200to2500","DYJetsToLL_HT_2500toInf"],
#	"GJets" : ["GJets_HT_100To200","GJets_HT_200To400","GJets_HT_400To600","GJets_HT_600ToInf"],
##	"Signal_2017_fullsim" : ["SMS_T1tttt_mGluino2000_mLSP100_fullsim","SMS_T1tttt_mGluino1200_mLSP800_fullsim","SMS_T2tt_mStop_1200_mLSP_100_fullsim","SMS_T2tt_mStop_650_mLSP_350_fullsim","SMS_T2tt_mStop_1200_mLSP_100_TuneCP5_fullsim","SMS_T2tt_mStop_850_mLSP_100_TuneCP5_fullsim","SMS_T2tt_mStop_650_mLSP_350_TuneCP5_fullsim","SMS_T1bbbb_mGluino1000_mLSP900_fullsim","SMS_T1bbbb_mGluino1500_mLSP100_fullsim",],
#	"MET_2016" : ["Data_MET_2016_PeriodB", "Data_MET_2016_PeriodC", "Data_MET_2016_PeriodD", "Data_MET_2016_PeriodE", "Data_MET_2016_PeriodF", "Data_MET_2016_PeriodG", "Data_MET_2016_PeriodH"],
#	"SingleMuon_2016" : ["Data_SingleMuon_2016_PeriodB", "Data_SingleMuon_2016_PeriodC", "Data_SingleMuon_2016_PeriodD", "Data_SingleMuon_2016_PeriodE", "Data_SingleMuon_2016_PeriodF", "Data_SingleMuon_2016_PeriodG", "Data_SingleMuon_2016_PeriodH"],
#	"SingleMuon_2017_BtoE" : ["Data_SingleMuon_2017_PeriodB", "Data_SingleMuon_2017_PeriodC", "Data_SingleMuon_2017_PeriodD", "Data_SingleMuon_2017_PeriodE"],
#   "SingleMuon_2017_F" : ["Data_SingleMuon_2017_PeriodF"],
#	"SingleMuon_2018_PreHEM"  : ["Data_SingleMuon_2018_PeriodA", "Data_SingleMuon_2018_PeriodB_PreHEM"],
#	"SingleMuon_2018_AB"  : ["Data_SingleMuon_2018_PeriodA", "Data_SingleMuon_2018_PeriodB"],
#	"SingleMuon_2018_PostHEM" : ["Data_SingleMuon_2018_PeriodB_PostHEM", "Data_SingleMuon_2018_PeriodC", "Data_SingleMuon_2018_PeriodD"],
#	"SingleMuon_2018_CD"  : ["Data_SingleMuon_2018_PeriodC", "Data_SingleMuon_2018_PeriodD"],
#   "SingleElectron_2016" : ["Data_SingleElectron_2016_PeriodB","Data_SingleElectron_2016_PeriodC","Data_SingleElectron_2016_PeriodD","Data_SingleElectron_2016_PeriodE","Data_SingleElectron_2016_PeriodF","Data_SingleElectron_2016_PeriodG","Data_SingleElectron_2016_PeriodH"],
#   "SingleElectron_2017_BtoE" : ["Data_SingleElectron_2017_PeriodB","Data_SingleElectron_2017_PeriodC","Data_SingleElectron_2017_PeriodD","Data_SingleElectron_2017_PeriodE"],
#   "SingleElectron_2017_F" : ["Data_SingleElectron_2017_PeriodF"],
#    "SingleElectron_2018_PreHEM" : ["Data_EGamma_2018_PeriodA","Data_EGamma_2018_PeriodB_PreHEM"],
#    "SingleElectron_2018_PostHEM" : ["Data_EGamma_2018_PeriodB_PostHEM","Data_EGamma_2018_PeriodC","Data_EGamma_2018_PeriodD"],
    }
Data = False
PeriodF = False
PostHEM = False
loose_cuts = False
isTTZ = False

process_list = []
for sub_list in process_dict.values():
    for item in sub_list:
        if "Data" in item:
            process_list.append(item)
            Data = True
        else:
            process_list.append(item + "_" + year)
        if "2017_PeriodF" in item:
            PeriodF = True
        if "PostHEM" in item:
            PostHEM = True
        if "TTZToLLNuNu" in item:
            isTTZ = True

ProjectName = ShortProjectName + "_" + year
LongProjectName = ShortProjectName + "_" + year
if PostHEM or PeriodF: argument = "%s.$(Process).list %s2_$(Process).root"
argument = argument + " --era " + year
if Data:
    LongProjectName = LongProjectName + "_Data"
    argument += " --isData"
if PostHEM:
    ProjectName = ProjectName + "_PostHEM"
    LongProjectName = LongProjectName + "_PostHEM"
    argument += " --PostHEM"
if PeriodF:
    ProjectName = ProjectName + "_PeriodF"
    LongProjectName = LongProjectName + "_PeriodF"
    argument += " --PeriodF"
if loose_cuts and (DelExe != '../SB_reader'):
    argument += " --loose_cuts"

if DelExe == '../SB_reader':
    if isTTZ:
        LongProjectName = LongProjectName + "_TTZ"
        argument += " --isTTZ"

#print process_list

def tar_cmssw():
    print("Tarring up CMSSW, ignoring file larger than 100MB")
    cmsswdir = os.environ['CMSSW_BASE']
    cmsswtar = os.path.abspath('%s/CMSSW.tar.gz' % tempdir)
    if os.path.exists(cmsswtar):
        ans = raw_input('CMSSW tarball %s already exists, remove? [yn] ' % cmsswtar)
        if ans.lower()[0] == 'y':
            os.remove(cmsswtar)
        else:
            return cmsswtar

    def exclude(tarinfo):
        if tarinfo.size > 100*1024*1024:
            tarinfo = None
            return tarinfo
        exclude_patterns = ['/.git/', '/tmp/', '/jobs.*/', '/logs/', '/.SCRAM/', '.pyc', '/datacards/', '/results.*/', '/plots.*/','/fastsim_results.*/']
        for pattern in exclude_patterns:
            if re.search(pattern, tarinfo.name):
                # print('ignoring %s in the tarball', tarinfo.name)
                tarinfo = None
                break
        return tarinfo

    with tarfile.open(cmsswtar, "w:gz",dereference=True) as tar:
        tar.add(cmsswdir, arcname=os.path.basename(cmsswdir), filter=exclude)
    return cmsswtar

def Condor_Sub(condor_file):
    curdir = os.path.abspath(os.path.curdir)
    os.chdir(os.path.dirname(condor_file))
    print "Submitting to condor: " + condor_file
    os.system("condor_submit " + condor_file)
    os.chdir(curdir)


def SplitPro(key, file, lineperfile=20):
    splitedfiles = []
    filelistdir = tempdir + '/' + "FileList"
    try:
        os.makedirs(filelistdir)
    except OSError:
        pass

    filename = os.path.abspath(file)

    f = open(filename, 'r')
    lines = f.readlines()

    if len(lines) <= lineperfile:
        shutil.copy2(os.path.abspath(filename), "%s/%s.0.list" % (filelistdir, key))
        splitedfiles.append(os.path.abspath("%s/%s.0.list" % (filelistdir, key)))
        return splitedfiles

    fraction = len(lines) / lineperfile
    if len(lines) % lineperfile > 0:
        fraction += 1

    for i in range(0, fraction):
        wlines = []
        if i == fraction - 1 :
            wlines = lines[lineperfile*i :]
        else:
            wlines = lines[lineperfile*i : lineperfile*(i+1)]
        if len(wlines) > 0:
            outf = open("%s/%s.%d.list" % (filelistdir, key, i), 'w')
            outf.writelines(wlines)
            splitedfiles.append(os.path.abspath("%s/%s.%d.list" % (filelistdir, key, i)))
        outf.close()

    return splitedfiles

def my_process(args):
    ## temp dir for submit
    global tempdir
    global ProjectName
    global Process
    ProjectName = time.strftime('%b%d') + ProjectName
    tempdir = tempdir + os.getlogin() + "/" + ProjectName +  "/"
    try:
        os.makedirs(tempdir)
    except OSError:
        pass

    ## Create the output directory
    outdir = OutDir +  "/" + ProjectName + "/"
    try:
        os.makedirs("/eos/uscms/%s" % outdir)
    except OSError:
        pass

    ## Update RunHT.csh with DelDir and pileups
    RunHTFile = tempdir + "/" + "RunExe.csh"
    with open(RunHTFile, "wt") as outfile:
        for line in open("RunExe.csh", "r"):
            #line = line.replace("DELDIR", os.environ['PWD'])
            line = line.replace("DELSCR", os.environ['SCRAM_ARCH'])
            line = line.replace("DELDIR", os.environ['CMSSW_VERSION'])
            line = line.replace("DELEXE", DelExe.split('/')[-1])
            line = line.replace("OUTDIR", outdir)
            outfile.write(line)

    ### Create Tarball
    NewNpro = {}
    Tarfiles = []
    if args.config != "":
        Process = ConfigList(os.path.abspath(args.config))
    elif args.era != "":
        Process = ConfigList(os.path.abspath("configs/sampleSets_PostProcessed_"+args.era+".cfg"))
    for key, value in Process.items():
	if (not run_everything) and (key not in process_list):
	    del Process[key]
	    continue
        if value[0] == "":
            value[0] = "../FileList/"+key+".list"
        if not os.path.isfile(value[0]):
            continue
        npro = GetProcess(key, value)
        Tarfiles+=npro
        NewNpro[key] = len(npro)
    Tarfiles+=glob.glob("../*py")

    tarballnames = []

    Tarfiles.append(os.path.abspath(DelExe))
    tarballname ="%s/%s.tar.gz" % (tempdir, LongProjectName)
    with tarfile.open(tarballname, "w:gz", dereference=True) as tar:
        [tar.add(f, arcname=f.split('/')[-1]) for f in Tarfiles]
        tar.close()
    tarballnames.append(tarballname)
    tarballnames.append(tar_cmssw())
    tarballnames.append("/uscms/home/benwu/python-packages.tgz")

    ### Update condor files
    for key, value in Process.items():
        print "==============================================="
        print key
        print "==============================================="
        #if args.loose_cuts: argument += " -l"
        arg = "\nArguments = " + argument % (key, key) + "\n Queue %d \n" % NewNpro[key]

        ## Prepare the condor file
        condorfile = tempdir + "/" + "condor_" + ProjectName +"_" + key
        with open(condorfile, "wt") as outfile:
            for line in open("condor_template", "r"):
                line = line.replace("EXECUTABLE", os.path.abspath(RunHTFile))
                line = line.replace("TARFILES", ", ".join(tarballnames))
                line = line.replace("TEMPDIR", tempdir)
                line = line.replace("PROJECTNAME", ProjectName)
                line = line.replace("PROCESSNAME", key)
                line = line.replace("ARGUMENTS", arg)
                outfile.write(line)

        Condor_Sub(condorfile)

def GetProcess(key, value):
    if len(value) == 1:
        return SplitPro(key, value[0], 1)
    else :
        return SplitPro(key, value[0], value[1])

def ConfigList(config):
    process = defaultdict(list)
    lines = open(config).readlines()
    for line_ in lines:
        line = line_.strip()
        if(len(line) <= 0 or line[0] == '#'):
            continue
        entry = line.split(",")
        stripped_entry = [ i.strip() for i in entry]
        process[stripped_entry[0]] = ["%s/%s" % (stripped_entry[1], stripped_entry[2]), defaultLperFile]
    return process

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='NanoAOD postprocessing.')
    parser.add_argument('-c', '--config',
        default = "",
        help = 'Path to the input config file.')
    parser.add_argument('-e', '--era',
        default = "",
        help = 'Era/Year')
    parser.add_argument('-l','--loose_cuts',
        default = False,
        action = "store_true",
        help = "Use loose cuts & dilepton trigger")
    args = parser.parse_args()
    my_process(args)



