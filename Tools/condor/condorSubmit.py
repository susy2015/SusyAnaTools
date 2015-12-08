#!/cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_8/external/slc6_amd64_gcc491/bin/python
####!${SRT_CMSSW_RELEASE_BASE_SCRAMRTDEL}/external/${SCRAM_ARCH}/bin/python

from samples import SampleCollection
from os import system
import optparse 

submitFile = """universe = vanilla
Executable = $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/condor/goMakePlots.sh
Requirements = OpSys == "LINUX"&& (Arch != "DUMMY" )
Should_Transfer_Files = YES
WhenToTransferOutput = ON_EXIT
Transfer_Input_Files = $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/basicCheck, $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/condor/goMakePlots.sh, $ENV(CMSSW_BASE)/lib/$ENV(SCRAM_ARCH)/librecipeAUXOxbridgeMT2.so
Output = logs/basicCheck_$(Process).stdout
Error = logs/basicCheck_$(Process).stderr
Log = logs/basicCheck_$(Process).log
notify_user = ${LOGNAME}@FNAL.GOV
x509userproxy = $ENV(X509_USER_PROXY)

"""

parser = optparse.OptionParser("usage: %prog [options]\n")

parser.add_option ('-n', dest='numfile', type='int', default = 5, help="number of files per job")
parser.add_option ('-d', dest='datasets', type='string', default = '', help="List of datasets 'ZJetsToNuNu,DYJetsToLL'")
parser.add_option ('-l', dest='dataCollections', action='store_true', default = False, help="List all datacollections")
parser.add_option ('-r', dest='refLumi', type='string', default = None, help="Data collection to define lumi (uses default lumi if no reference data collection is defined)")
parser.add_option ('-c', dest='noSubmit', action='store_true', default = False, help="Do not submit jobs.  Only create condor_submit.txt.")

options, args = parser.parse_args()

nFilesPerJob = options.numfile

fileParts = [submitFile]
sc = SampleCollection()
datasets = []

if options.dataCollections:
    print sc.sampleCollectionList()
    exit(0)

if options.datasets:
    datasets = options.datasets.split(',')
else:
    print "No dataset pecified"
    exit(0)

lumis = sc.sampleCollectionLumiList()
lumi = sc.getFixedLumi()
if options.refLumi != None:
    lumi = lumis[options.refLumi]
    print "Normalizing to %s pb-1" % (lumi)

for ds in datasets:
    ds = ds.strip()

    for s, n in sc.sampleList(ds):
        print n
        f = open(s)
        if not f == None:
            count = 0
            for l in f:
                if '.root' in l and not 'failed' in l:
                    count = count + 1
            for startFileNum in xrange(0, count, nFilesPerJob):
                fileParts.append("Arguments = %s $ENV(CMSSW_BASE) %i %i %f %s\nQueue\n\n"%(n, nFilesPerJob, startFileNum, lumi, s))
            f.close()

fout = open("condor_submit.txt", "w")
fout.write(''.join(fileParts))
fout.close()

system('mkdir -p logs')
system("echo 'condor_submit condor_submit.txt'")
system('condor_submit condor_submit.txt')
