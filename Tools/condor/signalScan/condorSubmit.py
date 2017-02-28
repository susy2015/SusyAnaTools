#!/cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_8/external/slc6_amd64_gcc491/bin/python
####!${SRT_CMSSW_RELEASE_BASE_SCRAMRTDEL}/external/${SCRAM_ARCH}/bin/python

from samples import SampleCollection
from os import system, environ
import re
import optparse 

submitFile = """universe = vanilla
Executable = $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/condor/signalScan/goScan.sh
Requirements = OpSys == "LINUX"&& (Arch != "DUMMY" )
Should_Transfer_Files = YES
WhenToTransferOutput = ON_EXIT
Transfer_Input_Files = $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/signalScan, $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/condor/signalScan/goScan.sh, $ENV(CMSSW_BASE)/lib/$ENV(SCRAM_ARCH)/librecipeAUXOxbridgeMT2.so, $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/data/signalScan_fastsim_T2tt_bTagEff_ISR.root, $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/data/signalScan_fastsim_T1tttt_bTagEff_ISR.root, $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/CSVFiles/CSVv2_Moriond17_B_H.csv, $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/CSVFiles/fastsim_csvv2_ttbar_26_1_2017.csv, $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/ISR_Root_Files/ISRWeights.root, $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/LeptonSF_Root_Files/allINone_leptonSF_Moriond17.root, $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/TopTagger.cfg, $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/TrainingOutput_dR20_pt30_depth12_500tree_2017_Feb16.model, $ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/data/puppiCorr.root, TRANS_TAR_BALL_CMSSW
notify_user = ${LOGNAME}@FNAL.GOV
x509userproxy = $ENV(X509_USER_PROXY)

"""

parser = optparse.OptionParser("usage: %prog [options]\n")

parser.add_option ('-n', dest='numfile', type='int', default = 5, help="number of files per job")
parser.add_option ('-d', dest='datasets', type='string', default = '', help="List of datasets 'ZJetsToNuNu,DYJetsToLL'")
parser.add_option ('-l', dest='dataCollections', action='store_true', default = False, help="List all datacollections")
parser.add_option ('-r', dest='refLumi', type='string', default = None, help="Data collection to define lumi (uses default lumi if no reference data collection is defined)")
parser.add_option ('-c', dest='noSubmit', action='store_true', default = False, help="Do not submit jobs.  Only create condor_submit.txt.")

rel_base = environ['CMSSW_BASE']
rel_name = rel_base.split('/')[-1]
tar_file_name = rel_name+'.tar.gz'

cache_all_dir = rel_base+'/src/SusyAnaTools/Tools/cache_all.sh'
system('sh '+cache_all_dir)

tar_command = 'tar --exclude-caches-all -czf '+tar_file_name + ' -C '+rel_base+'/.. ' + rel_name
system(tar_command)
system('mv '+rel_name+'.tar.gz '+rel_base+'/src/SusyAnaTools/Tools/condor/')

submitFile = re.sub(r'TRANS_TAR_BALL_CMSSW', '$ENV(CMSSW_BASE)/src/SusyAnaTools/Tools/condor/'+tar_file_name, submitFile)

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
                fileParts.append("Arguments = %s %s %i %i %f %s %s\n"%(n, rel_name, nFilesPerJob, startFileNum, lumi, s, ds))
                fileParts.append("Output = logs/basicCheck_%s_%i.stdout\n"%(n, startFileNum/nFilesPerJob))
                fileParts.append("Error = logs/basicCheck_%s_%i.stderr\n"%(n, startFileNum/nFilesPerJob))
                fileParts.append("Log = logs/basicCheck_%s_%i.log\n"%(n, startFileNum/nFilesPerJob))
                fileParts.append("Queue\n\n")

            f.close()

fout = open("condor_submit.txt", "w")
fout.write(''.join(fileParts))
fout.close()

system('mkdir -p logs')
system("echo 'condor_submit condor_submit.txt'")
system('condor_submit condor_submit.txt')
