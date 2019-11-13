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
DelExe    = '../SB_reader_signal'
OutDir = '/store/user/%s/StopStudy' %  getpass.getuser()
tempdir = '/uscms_data/d3/%s/condor_temp/' % getpass.getuser()
#Sample name added in myprocess, which has access to args
ShortProjectName = 'VBSB'
argument = "%s.$(Process).list %s_$(Process).root"
# argument = "--inputFiles=%s.$(Process).list --outputFile=%s_$(Process).root --jettype=L1PuppiJets"
defaultLperFile = 5
run_everything = True
#if run_everything: defaultLperFile = 100
year = "2016"
#year = "2017"
#year = "2018"
#if args.era != "":
#    year = args.era


PostHEM = False
loose_cuts = False
PeriodF = False

process_list = []
#Using run_everything, so process_list unnecessary. Just point to the correct config.

ProjectName = ShortProjectName + "_" + year
LongProjectName = ShortProjectName + "_" + year
argument = argument + " --era " + year
if PostHEM:
    ProjectName = ProjectName + "_PostHEM"
    LongProjectName = LongProjectName + "_PostHEM"
    argument += " --PostHEM"
if loose_cuts and (DelExe != '../SB_reader'):
    argument += " --loose_cuts"

if DelExe == '../SB_reader':
    if PeriodF:
        ProjectName = ProjectName + "_PeriodF"
        LongProjectName = LongProjectName + "_PeriodF"
        argument += " --PeriodF"

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
        exclude_patterns = ['/.git/', '/tmp/', '/jobs.*/', '/logs/', '/.SCRAM/', '.pyc', '/datacards/', '/results.*/', '/plots.*/']
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
    #ProjectName = time.strftime('%b%d') + ProjectName + "_"
    ProjectName = time.strftime('%b%d') + ProjectName + "_" + args.config.split("/")[-1][:-4]
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
    else:
        print "Please specify a config."
        return 1
    #elif args.era != "":
    #    Process = ConfigList(os.path.abspath("configs/sampleSets_PostProcessed_"+args.era+".cfg"))
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



