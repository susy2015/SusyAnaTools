#!/cvmfs/cms.cern.ch/slc6_amd64_gcc491/cms/cmssw/CMSSW_7_4_8/external/slc6_amd64_gcc491/bin/python
####!${SRT_CMSSW_RELEASE_BASE_SCRAMRTDEL}/external/${SCRAM_ARCH}/bin/python
from os import system
import optparse 

parser = optparse.OptionParser("usage: %prog [options]\n")

parser.add_option ('-n', dest='numfile', type='int', default = 1, help="number of files per job")
parser.add_option ('-d', dest='datasets', type='string', default = '', help="List of datasets 'ZJetsToNuNu,DYJetsToLL'")
parser.add_option ('-l', dest='dataCollections', action='store_true', default = False, help="List all datacollections")
parser.add_option ('-r', dest='refLumi', type='string', default = None, help="Data collection to define lumi (uses default lumi if no reference data collection is defined)")
parser.add_option ('-c', dest='noSubmit', action='store_true', default = False, help="Do not submit jobs.  Only create condor_submit.txt.")
parser.add_option ('-f', dest='filelist', type='string', default = '', help="file list")
parser.add_option ('-o', dest='outputdir', type='string', default = '', help="output directory in eos space")

options, args = parser.parse_args()

submitFile = """universe = vanilla
Executable = $ENV(CMSSW_BASE)/src/SusyAnaTools/SkimsAUX/workdir/prod/80X_crab_example/condor_prod_fastsim/prodNtp.sh
Requirements = OpSys == "LINUX"&& (Arch != "DUMMY" )
Should_Transfer_Files = if_needed
Transfer_Input_Files = $ENV(CMSSW_BASE)/src/SusyAnaTools/SkimsAUX/workdir/prod/80X_crab_example/condor_prod_fastsim/prodNtp.sh, $ENV(CMSSW_BASE)/src/SusyAnaTools/SkimsAUX/workdir/prod/80X_crab_example/condor_prod_fastsim/treeMaker_stopRA2.py, $ENV(CMSSW_BASE)/src/SusyAnaTools/SkimsAUX/workdir/prod/80X_crab_example/condor_prod_fastsim/""" + options.filelist + """
Transfer_Output_Files = "" 
notify_user = ${LOGNAME}@FNAL.GOV
x509userproxy = $ENV(X509_USER_PROXY)

"""

nFilesPerJob = options.numfile

fileParts = [submitFile]

with open(options.filelist) as myfile:
   count = sum(1 for line in myfile if line.rstrip('\n'))

print "\nopen file : %s with %d number of lines\n"%(options.filelist,count)

for startFileNum in xrange(0, count, nFilesPerJob):
   fileParts.append("Arguments = $ENV(CMSSW_BASE) %s %i %i %i %s\n"%(options.filelist, nFilesPerJob, startFileNum, startFileNum/nFilesPerJob, options.outputdir))
   fileParts.append("Output = logs/prodNtpk_%s_%i.stdout\n"%(options.filelist, startFileNum/nFilesPerJob))
   fileParts.append("Error = logs/prodNtpk_%s_%i.stderr\n"%(options.filelist, startFileNum/nFilesPerJob))
   fileParts.append("Log = logs/prodNtpk_%s_%i.log\n"%(options.filelist, startFileNum/nFilesPerJob))
   fileParts.append("Queue\n\n")

fout = open("condor_submit.txt", "w")
fout.write(''.join(fileParts))
fout.close()

system('mkdir -p logs')
system("echo 'condor_submit condor_submit.txt'")
system('condor_submit condor_submit.txt')
