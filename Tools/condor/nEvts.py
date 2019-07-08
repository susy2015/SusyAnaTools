import numpy as np
from pyxrootd import client
from functools import partial
import concurrent.futures

def getFiles(fileList):
    # If multiprocessing is used this must also be run in a seperate thread from the master thread
    try:
        with client.File() as fl:
            # Why is the client written like this????
            fl.open(fileList)
            files = [f.strip("\n") for f in fl]
    except ValueError:
        print "Filelist not found: ", fileList
        return None
    else:
        return files

executor = concurrent.futures.ThreadPoolExecutor(1)

try:
    raise ImportError
# Uproot implementation is currently rather slow, disable it for now
#    import uproot
#    print "using uproot"
#
#    def getNEvtsProcess(fileURL, legacy=False):
#        try:
#            with uproot.open(fileURL) as f:
#                if legacy:
#                    array = f["stopTreeMaker"]["AUX"]["stored_weight"].array(executor=executor)
#                else:
#                    array = f["Events"]["genWeight"].array(executor=executor)
#        except:
#            print "ERROR: unable to open fileURL"
#            return None
#        total = array.shape[0]
#        diff = array.sum()
#        totalNeg = (total - diff)/2
#        totalPos = (total + diff)/2
#        return np.array((totalPos, totalNeg))

except ImportError:

    # uproot is not found, fall back to using ROOT
    import ROOT
    # make sure ROOT.TFile.Open(fileURL) does not seg fault when $ is in sys.argv (e.g. $ passed in as argument)
    ROOT.PyConfig.IgnoreCommandLineOptions = True 

    def getNEvtsProcess(fileURL, legacy=False):
        try:
            f = ROOT.TFile.Open(fileURL)
        except:
            print "ERROR: unable to open fileURL"
            return None
        if legacy:
            tree = f.Get("stopTreeMaker/AUX")
            isData = not bool(tree.GetBranch("genLVec"))
        else:
            tree = f.Get("Events")
            isData = not bool(tree.GetBranch("GenPart_pt"))
        if not tree:
            print "ERROR: tree is empty"

        if isData:
            #this is data, just gen entries
            return np.array((tree.GetEntries(), 0))

        h = ROOT.TH1D("h", "h", 2, -100, 100)
        if legacy:
            tree.Draw("stored_weight>>h", "1", "goff")
        else:
            tree.Draw("genWeight>>h", "1", "goff")
        totalNeg = h.GetBinContent(0) + h.GetBinContent(1)
        totalPos = h.GetBinContent(2) + h.GetBinContent(3)
        f.Close()
        return np.array((totalPos, totalNeg))


def getNEvts(fileList, threads=4, legacy=False):
    files = getFiles(fileList)

    if files:
        results = list(getNEvtsProcess(f, legacy) for f in files)
        return sum(results)
    else:
        print "files do not exist: getNEvts() returning None"
        return None


if __name__ == "__main__":
    import optparse 
    from samples import SampleSet
    import re

    parser = optparse.OptionParser("usage: %prog [options]\n")

    parser.add_option ('-s', "--sampleSetCfg",    dest='sampleSetCfg',     type='string',          default = "../sampleSets.cfg",  help="SampleSet.cfg file to use")
    parser.add_option ('-d', "--dataSetPattern",  dest='dataSetPattern',   type='string',          default = ".*",                 help="Regexp defining sampleSets to check (Defaults to all)")
    parser.add_option ('-t', "--threads",         dest='threads',          type='int',             default = 4,                    help="Number of threads to use (default: 4)")
    parser.add_option ('-l', "--legacy",          dest='legacy',           action = 'store_true',  default = False,                help="Use legacy ntuples instead of Nano AOD ntuples (default: False)")

    options, args = parser.parse_args()

    ss = SampleSet(options.sampleSetCfg)
    samples = [(name, f.replace("/eos/uscms", "root://cmseos.fnal.gov/")) for name, f in ss.sampleSetList()]

    for name, f in samples:
        if re.search(options.dataSetPattern, name):
            try:
                nPos, nNeg = getNEvts(f, options.threads, options.legacy)
                #################################################################################################
                # WARNING: Do not change print statement unless you also update nEvts.C and updateSamples.py!!! #
                #################################################################################################
                print "%s, %s, Positive weights: %i, Negative weights: %i"%(name, f, nPos, nNeg)
            except TypeError:
                print "ERROR: TypeError in getNEvts()"
                pass
                
