#!/cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_8/external/slc6_amd64_gcc491/bin/python
####!${SRT_CMSSW_RELEASE_BASE_SCRAMRTDEL}/external/${SCRAM_ARCH}/bin/python

import sys
import os
from os import system, environ
sys.path = [environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/",] + sys.path

from samples import SampleSet
from samples import SampleCollection
import optparse 
import subprocess
import datetime

parser = optparse.OptionParser("usage: %prog [options]\n")

parser.add_option ('-n',  dest='numfile',               type='int',          default = 5,     help="number of files per job")
parser.add_option ('-d',  dest='sampleSetCfg',          type='string',       default = 'sampleSet.cfg',    help="SampleSet.cfg file")
parser.add_option ('-s', dest='sampleCollectionsFile',     type='string',       default = 'sampleSet.cfg',    help="SampleSet.cfg")
parser.add_option ('-c',  dest='noSubmit',              action='store_true', default = False, help="Do not submit jobs.  Only create condor_submit.txt.")
parser.add_option ('-t',  dest='taggerCfg',                 type='string'       , default = "",  help="Top tagger cfg directory")

options, args = parser.parse_args()

submitFile = ""
exeName = ""

#Here is the configuration for the Data/MC validation of the TopTagger 
filestoTransferTT  = [environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/tTagEfficiencyCalc",
                      environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/" + options.sampleSetCfg,
                      environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/" + options.sampleCollectionsFile,
                      environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/obj/samplesModule.so",
                      environ["CMSSW_BASE"] + "/src/TopTagger/TopTagger/test/libTopTagger.so",
                      environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/%s/TopTagger.cfg"%options.taggerCfg,
                      environ["CMSSW_BASE"] + "/src/SusyAnaTools/Tools/condor/%s/tfModel_frozen_DNN1_deepCVS_GR_balanced.pb"%options.taggerCfg,
                      ]

#go make TTopTagger plots!
submitFileTT = """universe = vanilla
Executable = $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/condor/goTTagEff.sh
Requirements = OpSys == "LINUX"&& (Arch != "DUMMY" )
Should_Transfer_Files = YES
WhenToTransferOutput = ON_EXIT
Transfer_Input_Files = $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/condor/goTTagEff.sh,tTag.tar.gz,$ENV(CMSSW_VERSION).tar.gz
Output = logs/tTag_$(Process).stdout
Error = logs/tTag_$(Process).stderr
Log = logs/tTag_$(Process).log
notify_user = ${LOGNAME}@FNAL.GOV
x509userproxy = $ENV(X509_USER_PROXY)

"""

def makeExeAndFriendsTarrball(filestoTransfer, fname):
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
#sc = SampleCollection(options.sampleSetCfg , options.sampleCollectionsFile)
ss = SampleSet(options.sampleSetCfg)
datasets = []

now = datetime.datetime.now()

dirName = "tTagEff_%s"%now.strftime("%Y-%m-%d_%H-%M-%S")
os.system("mkdir %s"%dirName)
os.chdir(dirName)

print "Create tarball ${CMSSW_VERSION}.tar.gz"
system("tar --exclude-caches-all --exclude-vcs -zcf ${CMSSW_VERSION}.tar.gz -C ${CMSSW_BASE}/.. ${CMSSW_VERSION} --exclude=src --exclude=tmp")

# makeExeAndFriendsTarball() is necessary now to apply WORLDSWORSESOLUTIONTOAPROBLEM 

exeName = "tTagEfficiencyCalc"
makeExeAndFriendsTarrball(filestoTransferTT, "tTag")

nFilesPerJob = options.numfile

for n, s in ss.sampleSetList():
    if "Period" in n: continue
    if "Data" in n: continue

    print "\t%s"%s
    print "\t%s"%n
    #print "\t{0} {1} {2}".format(s, n, e)
    try:
        f = open(s)
    except IOError:
        fShort = s.split("/")[-1]
        if(os.path.isfile(fShort)):
            os.remove(fShort)
        system("xrdcp root://cmseos.fnal.gov/$(echo %s | sed 's|/eos/uscms||') ."%s)
        print "fShort = {0}".format(fShort)
        f = open(fShort)
    if not f == None:
        count = 0
        for l in f:
            if '.root' in l and not 'failed' in l:
                count = count + 1
        for startFileNum in xrange(0, count, nFilesPerJob):
            #print 
            fileParts.append("Arguments = %s $ENV(CMSSW_VERSION) %i %i %s %s %s \n"%(n, nFilesPerJob, startFileNum, options.taggerCfg, options.sampleSetCfg.split("/")[-1], s))
            
            fileParts.append("Output = logs/%s_%s_%i.stdout\n"%(exeName, n, startFileNum))
            fileParts.append("Error = logs/%s_%s_%i.stderr\n"%(exeName, n, startFileNum))
            fileParts.append("Log = logs/%s_%s_%i.log\n"%(exeName, n, startFileNum))
            fileParts.append("Queue\n\n")

        f.close()

fout = open("condor_submit.txt", "w")
fout.write(''.join(fileParts))
fout.close()

system('mkdir -p logs')
if not options.noSubmit: 
    system("echo 'condor_submit condor_submit.txt'")
    system('condor_submit condor_submit.txt')

