import numpy as np
from multiprocessing import Pool
from pyxrootd import client

def getFiles(fileList):
    #If multiprocessing is used this must also be run in a seperate thread from the master thread
    try:
        with client.File() as fl:
            #Why is the client written like this????
            fl.open(fileList)
            files = [f.strip("\n") for f in fl]
    except ValueError:
        print "Filelist not found: ", fileList
        return None
    else:
        return files


try:
    import uproot

    def getNEvtsProcess(fileURL):
        try:
            with uproot.open(fileURL) as f:
                array = f["stopTreeMaker"]["AUX"]["stored_weight"].array()
        except:
            print "ERROR: unable to open fileURL"
            return None
        totalEvents = array.shape[0]
        totalNeg = (array[:] < 0).sum()
        totalPos = totalEvents - totalNeg
        return np.array((totalPos, totalNeg))

except ImportError:

    #uproot is not found, fall back to using ROOT
    import ROOT

    def getNEvtsProcess(fileURL):
        try:
            f = ROOT.TFile.Open(fileURL)
        except:
            print "ERROR: unable to open fileURL"
            return None
        tree = f.Get("stopTreeMaker/AUX")
        if not tree:
            print "tree is empty"
        h = ROOT.TH1D("h", "h", 2, -100, 100)
        tree.Draw("stored_weight>>h", "1", "goff")
        totalNeg = h.GetBinContent(0) + h.GetBinContent(1)
        totalPos = h.GetBinContent(2) + h.GetBinContent(3)
        f.Close()
        return np.array((totalPos, totalNeg))


def getNEvts(fileList, threads=4):
    if threads > 0:
        p = Pool(threads)
        files = p.map(getFiles, [fileList])[0]
    else:
        files = getFiles(fileList)
    #return None # for testing only

    if files:
        if threads > 0:
            results = p.map(getNEvtsProcess, files)
        else:
            results = list(getNEvtsProcess(f) for f in files)
        return sum(results)
    else:
        print "files do not exist: getNEvts() returning None"
        return None


if __name__ == "__main__":
    import optparse 
    from samples import SampleSet
    import re

    parser = optparse.OptionParser("usage: %prog [options]\n")

    parser.add_option ('-s', "--sampleSetCfg",        dest='sampleSetCfg',     type='string', default = "../sampleSets.cfg",  help="SampleSet.cfg file to use")
    parser.add_option ('-d', "--dataSetPattern",      dest='dataSetPattern',   type='string', default = ".*",                 help="Regexp defining sampleSets to check (Defaults to all)")
    parser.add_option ('-t', "--threads",             dest='threads',          type='int',    default = 4,                    help="Number of threads to use (default: 4)")

    options, args = parser.parse_args()

    ss = SampleSet(options.sampleSetCfg)
    samples = [(name, file.replace("/eos/uscms", "root://cmseos.fnal.gov/")) for name, file in ss.sampleSetList()]

    for name, file in samples:
        if re.search(options.dataSetPattern, name):
            print "name: {0} file: {1}".format(name, file)
            try:
                nPos, nNeg = getNEvts(file, options.threads)
                print "%s, %s, Positive weights: %i, Negative weights: %i"%(name, file, nPos, nNeg)
            except TypeError:
                pass
                
