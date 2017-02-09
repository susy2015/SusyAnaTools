import ROOT as rt
import sys
import os
import re
import optparse
import array

def rebin1D(h, bins):
    """Rebin histo h to bins and recompute the errors."""
    new_h = rt.TH1D("%s"%(h.GetName()), "%s"%(h.GetName()),
                 len(bins)-1, array.array('d', bins))
    new_h.Sumw2()
    for i in xrange(h.GetNbinsX()):
        bin_to_fill = new_h.FindBin(h.GetBinCenter(i+1))
        new_h.SetBinContent(bin_to_fill, h.GetBinContent(i+1) + new_h.GetBinContent(bin_to_fill))
        new_h.SetBinError(bin_to_fill, rt.TMath.Sqrt(h.GetBinError(i+1)*h.GetBinError(i+1) + new_h.GetBinError(bin_to_fill)*new_h.GetBinError(bin_to_fill) ) )
    new_h.SetBinContent(new_h.GetNbinsX(), new_h.GetBinContent(new_h.GetNbinsX()) + new_h.GetBinContent(new_h.GetNbinsX()+1))
    return new_h

parser = optparse.OptionParser("usage: %prog [options]\n")

parser.add_option('-d', dest='dir', type='string', default = '', help="directory")
parser.add_option('-t', dest='type', type='string', default = 'bTagEff')

options, args = parser.parse_args()

if not os.path.exists(options.dir):
   print("{} does NOT exist!".format(options.dir))
   sys.exit(1)

hist_names = []

if options.type == 'bTagEff':
   hist_names.extend(['d_eff_b', 'd_eff_c', 'd_eff_udsg', 'n_eff_b', 'n_eff_c', 'n_eff_udsg'])
elif options.type == 'ISRJets':
   hist_names.extend(['NJetsISR'])

print("options.dir : {}".format(options.dir))
print("options.type : {}".format(options.type))
print("hist_names : {}".format(hist_names))

mapper = {}

# the root file should take the name format as:
# sample_str + '_' + options.type + '_' + num + ".root
for filename in os.listdir(options.dir):
   if not ('.root' in filename): continue
   my_regex = r"(\S*)_" + re.escape(options.type) + r"_(\d*).root"
   m = re.match(my_regex, filename)
#   if m.groups()[0] in mapper.keys:
   if m:
     if m.group(1) in mapper.keys():
       mapper[m.group(1)].append(filename)
     else:
       mapper[m.group(1)] = [filename]

for key, value in mapper.items():
   target_filename = key+"_"+options.type+".root"
   hadd_command = "hadd -f "+target_filename
   for perfile in value:
      hadd_command += " "+options.dir+"/"+perfile
#   print("{} : {}".format(key, hadd_command))
   os.system(hadd_command)
   rt_file = rt.TFile(target_filename)
   cache_hist_list = []
   for hn in hist_names:
      hist = rt_file.Get(hn)
      if options.type == 'ISRJets':
         bins = [0,1,2,3,4,5,6,7]
         hist = rebin1D(hist, bins)
      cache_hist_list.append(hist) 
   new_rt_file = rt.TFile(target_filename+".tmp.root", "RECREATE")
   for ih in range(len(cache_hist_list)):
      cache_hist_list[ih].SetName(hist_names[ih]+"_"+key)
      cache_hist_list[ih].Write()
   new_rt_file.Write()
   new_rt_file.Close()
   os.system('mv '+target_filename+".tmp.root " + target_filename)
