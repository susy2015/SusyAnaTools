import os
import optparse
from glob import glob

def eoscp(filename, path, force = False, eosurl = "root://cmseos.fnal.gov"):
    stripedName = filename.split("/")[-1]
    if force:
        command = "xrdcp --force %s %s/%s"%(filename, eosurl, "/".join([path, filename]))
    else:
        command = "xrdcp %s %s/%s"%(filename, eosurl, "/".join([path, filename]))
    print command
    stdout = os.popen(command)
    for l in stdout:
        print l.strip("\n")

def eosls(path, option = "", eosurl = "root://cmseos.fnal.gov"):
    return os.popen("xrdfs %s ls %s %s"%(eosurl, option, path))

def recSearch(path, fout = None, eosurl = "root://cmseos.fnal.gov"):
    for l in eosls(path, "-l", eosurl = options.eosurl):
        if l.startswith("d"):
            endpath = l.split()[-1].split("/")[-1]
            recSearch("/".join([path, endpath]), fout)
        else:
            filename = "/".join([path, l.split()[-1].split("/")[-1]])
            if filename.endswith(".root") and not "failed" in filename:
                if fout == None:
                    print "".join(["%s/"%eosurl, filename])
                else:
                    #fout.write("".join(["%s/"%eosurl, filename, "\n"]))
                    fp = "".join(["%s/"%eosurl, filename, "\n"])
                    fpsplit = fp.split("/")
                    first = "/".join(fpsplit[:-3])
                    second = fpsplit[-3]
                    third = "/".join(fpsplit[-2:])
                    key = (first, third, )
                    if key in fout:
                        if(second > fout[key]):
                            fout[key] = second
                    else:
                        fout[key] = second


if __name__ == "__main__":

    parser = optparse.OptionParser("usage: %prog [options]\n")
    
    parser.add_option ('-d',      dest='directory',  type='string',   default = "",    help="file path to begin")
    parser.add_option ('-f',      dest='file',       type='string',   default = "",    help="File name to transfer")
    parser.add_option ('-e',      dest='eosurl',     type='string',   default = "root://cmseos.fnal.gov", help="eos url (Defaults to root://cmseos.fnal.gov)")
    parser.add_option ('-l',      dest='list',   action="store_true", default = False, help="Create file lists")
    parser.add_option ('-c',      dest='copy',   action="store_true", default = False, help="Copy a file to eos")
    parser.add_option ('--force', dest='force',  action="store_true", default = False, help="Force overwrite when doing xrdcp (use with caution!)")
    
    options, args = parser.parse_args()
    
    startPath = options.directory
    
    if options.list:
        for l in eosls(startPath, "-l", eosurl = options.eosurl):
            if l.startswith("d"):
                endpath = l.split()[-1].split("/")[-1]
                print "".join([endpath, ".txt"])
                fileName = "".join([endpath, ".txt"])
                f = open(fileName, "w")
                outDict = {}
                recSearch("/".join([startPath, endpath]), outDict, eosurl = options.eosurl)
                for key in outDict:
                    f.write("/".join([key[0], outDict[key], key[1]]))
                f.close()
        
                if options.copy:
                    eoscp(fileName, startPath, options.force, eosurl = options.eosurl)

    if options.copy and len(options.file):
        files = glob(options.file)
        if not(len(files)):
            print "No files to transfer with glob options.file"
        else:
            for f in files:
                eoscp(f, startPath, options.force, eosurl = options.eosurl)
            
