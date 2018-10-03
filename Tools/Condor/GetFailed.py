#!/usr/bin/env python
# encoding: utf-8

# File        : GetFailed.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2017 Feb 14
#
# Description :

import subprocess
import glob
import os

tmpdir = '/uscmst1b_scratch/lpc1/lpctrig/benwu/CondorTempbenwu/Mar07DataMC_v1/'

RunExe = "%s/RunExe.csh" % tmpdir
outdir = ""


def GetOutRoot():
    global outdir
    with open(RunExe, 'r') as f:
        for l in f.readlines():
            if "set OUTPUT" in l.strip():
                outdir = l.split()[-1]

    run = "eos root://cmseos.fnal.gov ls %s" % outdir
    process = subprocess.Popen(run, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = process.communicate()

    rootset = set()
    for l in out.split():
        if "root" in l:
            rootset.add(l.split(".")[0])
    return rootset

def GetFileList():
    gg = glob.glob("%s/FileList/*list" % tmpdir)
    gg += glob.glob("%s/*list" % tmpdir)
    fileset = set()
    filemap = {}
    for l  in gg:
        if "list" in l:
            newname ="_".join(l.split("/")[-1].split(".")[:-1])
            filemap[newname] = l
            fileset.add(newname)
    return fileset, filemap


if __name__ == "__main__":
    rootset = GetOutRoot()
    fileset, filemap = GetFileList()
    if len(rootset - fileset)  != 0 :
        print "Extra root file?", rootset -fileset
    diff = fileset - rootset
    print "/eos/uscms/%s/testMain.cc" % outdir
    print "OutDir = %s" % outdir
    print "ProjectName = %s" % os.path.basename(os.path.normpath(tmpdir))[5:]
    for l in sorted(diff):
        misslist = filemap[l].split("/")[-1]
        missname = "_".join(misslist.split(".")[:-1])
        print '\'%s\' : [\'%s\', 1],' % (missname, filemap[l])

