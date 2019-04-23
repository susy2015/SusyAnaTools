#!/cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_8/external/slc6_amd64_gcc491/bin/python
####!${SRT_CMSSW_RELEASE_BASE_SCRAMRTDEL}/external/${SCRAM_ARCH}/bin/python

import sys
import os
from os import system, environ
sys.path = [environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/",] + sys.path

from samples import SampleSet
import optparse 
import subprocess
import datetime

# options
parser = optparse.OptionParser("usage: %prog [options]\n")

parser.add_option ('-c',  dest='noSubmit',         action='store_true', default = False,               help="Do not submit jobs. Only create condor_submit.txt.")
parser.add_option ('-s',  dest='sampleSetsFile',   type='string',       default = "sampleSets.cfg",    help="Sample sets config file")

options, args = parser.parse_args()
sampleSetsFile = options.sampleSetsFile

submitFile = ""
exeName = ""

#Here is the configuration for the Data/MC validation of the TopTagger 
filestoTransferTT  = [
                      environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/samples.py",
                      environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/../obj/samplesModule.so",
                      environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/nEvts.py",
                     ]

if sampleSetsFile: filestoTransferTT += [environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/" + sampleSetsFile]

#go make TTopTagger plots!
submitFileTT = """universe = vanilla
Executable = $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/condor/goNEvts.sh
Requirements = OpSys == "LINUX"&& (Arch != "DUMMY" )
Should_Transfer_Files = YES
WhenToTransferOutput = ON_EXIT
Transfer_Input_Files = $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/condor/goNEvts.sh,TT.tar.gz,$ENV(CMSSW_VERSION).tar.gz
notify_user = ${LOGNAME}@FNAL.GOV
x509userproxy = $ENV(X509_USER_PROXY)
"""

def makeExeAndFriendsTarrball(filestoTransfer, fname):
    if True:
        #WORLDSWORSESOLUTIONTOAPROBLEM
        system("mkdir -p WORLDSWORSESOLUTIONTOAPROBLEM")
        for fn in filestoTransfer:
            system("cd WORLDSWORSESOLUTIONTOAPROBLEM; ln -s %s" % fn)
        
        print "Create tarball {0}.tag.gz".format(fname)
        tarallinputs = "tar czvf %s.tar.gz WORLDSWORSESOLUTIONTOAPROBLEM --dereference" % fname
        print tarallinputs
        system(tarallinputs)
        system("rm -r WORLDSWORSESOLUTIONTOAPROBLEM")

submitFile = submitFileTT
fileParts = [submitFile]
sc = SampleSet(sampleSetsFile)

now = datetime.datetime.now()

dirName = "submission_%s"%now.strftime("%Y-%m-%d_%H-%M-%S")
os.system("mkdir %s"%dirName)
os.chdir(dirName)

if True:
    print "Create tarball ${CMSSW_VERSION}.tar.gz"
    system("tar --exclude-caches-all --exclude-vcs -zcf ${CMSSW_VERSION}.tar.gz -C ${CMSSW_BASE}/.. ${CMSSW_VERSION} --exclude=src --exclude=tmp")

# makeExeAndFriendsTarball() is necessary now to apply WORLDSWORSESOLUTIONTOAPROBLEM 

exeName = "nEvts"
makeExeAndFriendsTarrball(filestoTransferTT, "TT")

for ds in sc.sampleSetList():
    dsn = ds[0]

    fileParts.append("Arguments = %s $ENV(CMSSW_VERSION) %s\n"%(dsn, sampleSetsFile))
    fileParts.append("Output = logs/%s.stdout\n"%(dsn))
    fileParts.append("Error = logs/%s.stderr\n"%(dsn))
    fileParts.append("Log = logs/%s.log\n"%(dsn))
    fileParts.append("Queue\n\n")

fout = open("condor_submit.txt", "w")
fout.write(''.join(fileParts))
fout.close()

system('mkdir -p logs')
if not options.noSubmit: 
    #system('mkdir -p logs')
    system("echo 'condor_submit condor_submit.txt'")
    system('condor_submit condor_submit.txt')
