import os
import optparse
from glob import glob
import re

def copy(filename, path, force = False):
    if force:
        command = "cp --force %s %s"%(filename, "/".join([path, filename]))
    else:
        command = "cp %s %s"%(filename, "/".join([path, filename]))
    print command
    with os.popen(command) as stdout:
        for l in stdout:
            print l.strip("\n")

def eoscp(filename, path, force = False, eosurl = "root://cmseos.fnal.gov"):
    if force:
        command = "xrdcp --force %s %s/%s"%(filename, eosurl, "/".join([path, filename]))
    else:
        command = "xrdcp %s %s/%s"%(filename, eosurl, "/".join([path, filename]))
    print command
    with os.popen(command) as stdout:
        for l in stdout:
            print l.strip("\n")

def eosls(path, option = "", eosurl = "root://cmseos.fnal.gov"):
    return os.popen("xrdfs %s ls %s %s"%(eosurl, option, path))

def recSearch(path, outDict = None, eosurl = "root://cmseos.fnal.gov", match=".*\.root$", reject=".*failed.*", crab = True):
    with eosls(path, "-l", eosurl = options.eosurl) as stdout:
        for l in stdout:
            #if this is a directory dive in to continue searching
            if l.startswith("d"):
                endpath = l.split()[-1].split("/")[-1]
                recSearch("/".join([path, endpath]), outDict, eosurl=eosurl, match=match, reject=reject, crab=crab)
            else:
                filename = "/".join([path, l.split()[-1].split("/")[-1]])
                #if filename.endswith(".root") and not "failed" in filename:
                if re.match(match, filename) and not re.match(reject, filename):
                    if outDict == None:
                        print "".join(["%s/"%eosurl, filename])
                    elif crab:
                        fp = "".join(["%s/"%eosurl, filename, "\n"])
                        fpsplit = fp.split("/")
                        #get part of file list before data folder
                        first = "/".join(fpsplit[:-3])
                        #get date folder
                        second = fpsplit[-3]
                        #get portion of path after date folder
                        third = "/".join(fpsplit[-2:])
                        #construct key which is unique to each file (but not date)
                        key = (first, third, )
                        #for each file only keep the results from the latest dated folder
                        if key in outDict:
                            if(second > outDict[key]):
                                outDict[key] = second
                        else:
                            outDict[key] = second
                    else:
                        outDict[filename] = ""


if __name__ == "__main__":

    parser = optparse.OptionParser("usage: %prog [options]\n")
    
    parser.add_option ('-p',      dest='path',       type='string',    default = "",                           help="File path to copy text files")
    parser.add_option ('-d',      dest='directory',  type='string',    default = "",                           help="File path to begin")
    parser.add_option ('-f',      dest='file',       type='string',    default = "",                           help="File name to transfer")
    parser.add_option ('-e',      dest='eosurl',     type='string',    default = "root://cmseos.fnal.gov",     help="Eos url (Defaults to root://cmseos.fnal.gov)")
    parser.add_option ('-m',      dest='match',      type='string',    default = ".*\.root$",                  help="File paths must match this regular expression")
    parser.add_option ('-r',      dest='reject',     type='string',    default = ".*failed.*",                 help="File paths must NOT match this regular expression")
    parser.add_option ('-t',      dest='startDirs',  type='string',    default = ".*",                         help="Start directory paths must match this regular expression")
    parser.add_option ('-l',      dest='list',   action="store_true",  default = False,                        help="Create file lists")
    parser.add_option ('-c',      dest='copy',   action="store_true",  default = False,                        help="Copy a file to eos (when used with -l this option automatically transfers filelists to eos, otherwise use -f to specify file to transfer)")
    parser.add_option ('-s',      dest='crab',   action="store_false", default = True,                         help="Will not assume standard crab file structure (Will not filter on latest copy of each file)")
    parser.add_option ('--force', dest='force',  action="store_true",  default = False,                        help="Force overwrite when doing xrdcp (use with caution!)")
    
    options, args = parser.parse_args()
    
    startPath = options.directory
    useEoscp = True
    if options.path:
        copyPath = options.path
        useEoscp = False
    else:
        copyPath = startPath
    
    if options.list:
        with eosls(startPath, "-l", eosurl = options.eosurl) as stdout:
            for l in stdout:
                #if this is a directory, begin recursive search for files
                if l.startswith("d"):
                    endpath = l.split()[-1].split("/")[-1]
                    #skip directorys not matching the required regexp
                    if not re.match(options.startDirs, endpath):
                        continue
                    print "".join([endpath, ".txt"])
                    fileName = "".join([endpath, ".txt"])
                    outDict = {}
                    recSearch("/".join([startPath, endpath]), outDict, eosurl = options.eosurl, match=options.match, reject=options.reject, crab=options.crab)
                    outList = []
                    for key in outDict:
                        if isinstance(key, tuple):
                            outList.append("/".join([key[0], outDict[key], key[1]]).strip("\n"))
                        else:
                            outList.append("%s"%key)
                    f = open(fileName, "w")
                    outList = sorted(outList)
                    f.write("\n".join(outList))
                    f.close()
            
                    if options.copy:
                        if useEoscp:
                            eoscp(fileName, startPath, options.force, eosurl = options.eosurl)
                        else:
                            copy(fileName, copyPath, options.force)

    if options.copy and len(options.file) and not options.list:
        files = glob(options.file)
        if not(len(files)):
            print "No files to transfer with glob options.file"
        else:
            for f in files:
                if useEoscp:
                    eoscp(f, startPath, options.force, eosurl = options.eosurl)
                else:
                    copy(f, copyPath, options.force)
           

