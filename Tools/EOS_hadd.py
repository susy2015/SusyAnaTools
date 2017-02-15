import glob, os, sys
import optparse
import math
import re

parser = optparse.OptionParser("usage: %prog [options]\n")

parser.add_option('-d', dest='dir', type='string', default = '', help="directory")
parser.add_option('-k', dest='key', type='string', default = 'bTagEff')
parser.add_option('-n', dest='split', type='int', default = 5)
parser.add_option('-s', dest='sel', type='string', default ='')

options, args = parser.parse_args()

d_hadd_source = "/store/group/lpcsusyhad/lhx/Skimmed_2016Dec29"

MGM = "root://cmseos.fnal.gov/"

cmd = 'xrdfs root://cmseos.fnal.gov ls -u ' + options.dir
if options.key:
   cmd += ' | grep -E ' + options.key

cmd_output = os.popen(cmd).read()

split_output = cmd_output.split()

mapper = {}

split_sel = options.sel.split()

for each in split_output:
   my_regex = r"(\S*)" + re.escape(options.key) + r"_(\S*)_stopFlatNtuples_(\d*).root"
   m = re.match(my_regex, each)
   if m:
     found = False if options.sel else True
     for sel in split_sel:
        if sel in m.group(2):
           found = True
     if not found: continue 
     if m.group(2) in mapper.keys():
        mapper[m.group(2)].append(each)
     else:
        mapper[m.group(2)] = [each]

for key, val in mapper.items():
   par_itvl = int(math.ceil(len(val)*1.0/options.split))
   if par_itvl * options.split < len(val):
      print("ERROR in splitting\n")
      sys.exit(1) 
   print("\nkey : {}  -->  size : {}  par : {}".format(key, len(val), par_itvl) )
   for ip in range(options.split):
      hadd_cmd = "hadd "+options.key+"_"+key+"_"+str(ip)+".root"
      low_idx = ip * par_itvl
      high_idx = (ip+1) * par_itvl
      if low_idx > len(val): break
      if high_idx > len(val): high_idx = len(val)
      val_split = val[low_idx:high_idx]
#      print("  --> ip : {}  val_split : {}".format(ip, val_split))
      for iv in val_split:
         hadd_cmd += " " + iv
#      print(hadd_cmd) 
      os.system(hadd_cmd) 
