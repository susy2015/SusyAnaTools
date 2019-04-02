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

#Here is the configuration for the Data/MC validation of the TopTagger 
filestoTransferTT  = [environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/sampleSets.cfg",
                      environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/samples.py",
                      environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/../obj/samplesModule.so",
                      environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/nEvts.py",
                      ]

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

parser = optparse.OptionParser("usage: %prog [options]\n")

parser.add_option ('-c',  dest='noSubmit',              action='store_true', default = False, help="Do not submit jobs.  Only create condor_submit.txt.")

options, args = parser.parse_args()

submitFile = ""
exeName = ""

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
sc = SampleSet("sampleSets.cfg")

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

    fileParts.append("Arguments = %s $ENV(CMSSW_VERSION) \n"%(dsn))
    fileParts.append("Output = logs/%s.stdout\n"%(dsn))
    fileParts.append("Error = logs/%s.stderr\n"%(dsn))
    fileParts.append("Log = logs/%s.log\n"%(dsn))
    fileParts.append("Queue\n\n")

fout = open("condor_submit.txt", "w")
fout.write(''.join(fileParts))
fout.close()

if not options.noSubmit: 
    system('mkdir -p logs')
    system("echo 'condor_submit condor_submit.txt'")
    system('condor_submit condor_submit.txt')

