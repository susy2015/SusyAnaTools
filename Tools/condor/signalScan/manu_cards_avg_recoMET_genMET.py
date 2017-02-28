#!/usr/bin/python

import os
import time
import math
import ROOT
from optparse import OptionParser # Command line parsing

# Remove the "=" part and anything after "#"
def procline(inputline):
   outline = []
   tmpline = inputline.split()
   isComments = False
   for each in tmpline:
      if each == "#":
         isComments = True
      if each == "=":
         continue
      if isComments == False :
         outline.append(each)
   return outline

def main():
   usage = "usage: %prog options"
   version = "%prog."
   parser = OptionParser(usage=usage,version=version)
   parser.add_option("-b", "--base", action="store", dest="basedir", type="string", default="", help="the original cards without fastsim filter")
   parser.add_option("-d", "--dest", action="store", dest="destdir", type="string", default="", help="the cards after fastsim filter")
   parser.add_option("-m", "--model", action="store", dest="model", type="string", default="T2tt", help="set SMS model type")
   
   (options, args) = parser.parse_args()

   print 'basedir :', options.basedir
   print 'destdir :', options.destdir

   channels = 84

   list_keywords_to_avg = ['rate', 'cs_event', 'contam', 'avg_weight', 'stat_unc_up', 'stat_unc_dn']
   list_keywords_to_skip = ['syst_metMag_up', 'syst_metMag_dn']

   for signal_name in os.listdir(options.destdir):
      if not ("signal" in signal_name): continue
      if not (".txt" in signal_name): continue
      if signal_name.startswith('.'): continue

      splitsignalinput = signal_name.split("/")
      stripDirSignalInput = splitsignalinput[-1]

      if "signal_" in stripDirSignalInput: tmp_signal_key = stripDirSignalInput.replace("signal_", "")
      elif "signal" in stripDirSignalInput: tmp_signal_key = stripDirSignalInput.replace("signal", "")
      tmp_signal_key = tmp_signal_key.replace(".txt", "")

      tmp_signal_name = signal_name
      print "processing signal with name %s" % signal_name

      full_dest_name = os.path.join(options.destdir, signal_name)
      dest_file = open(full_dest_name)

      full_base_name = os.path.join(options.basedir, signal_name)
      base_file = open(full_base_name)

      out_file = open(signal_name, "w")

      cached_syst_for_avg = []
      cached_cs = []
      cached_rate = []

      for base_line in base_file:
         split_base_line = procline(base_line)
#         if split_base_line[0] == "channels" : channels = split_base_line[1]

         toskip = False
         for line_skip in list_keywords_to_skip:
            if line_skip in base_line:
               toskip = True
               
         if toskip: continue

         modified = False
         for line_modif in list_keywords_to_avg:
            if line_modif in base_line:
               modified = True
               dest_file.seek(0, 0)
               for dest_line in dest_file:
                  if line_modif in dest_line:
                     split_dest_line = procline(dest_line)
                     out_line = "%11s = " % (line_modif)
                     for chn in range(1, int(channels)+1):
                        base_val = float(split_base_line[chn])
                        dest_val = float(split_dest_line[chn])
                        avg_val = 0.5*(base_val+dest_val)
                        if avg_val !=0:
                           cached_syst_for_avg.append( 0.5*abs(dest_val-base_val)/avg_val )
                        else:
                           cached_syst_for_avg.append( 0 )
                        if line_modif == "rate":
                           cached_rate.append(avg_val)
                           out_line = "%s%11.5f" % (out_line, avg_val)
                        elif line_modif == "cs_event":
                           avg_val = round(avg_val)
                           cached_cs.append(avg_val)
                           if cached_rate[chn-1] != 0 and avg_val ==0 :
                              print "ERROR ... cached_rate[chn-1] : %10.5f  avg_val : %10.0f!" % (cached_rate[chn-1], avg_val)
                           out_line = "%s%11.0f" % (out_line, avg_val)
                        elif line_modif == "contam":
                           if avg_val > cached_cs[chn-1]:
                              print "WARNING ... contam : %10.5f >= cs : %10.0f!" % (avg_val, cached_cs[chn-1])
                              avg_val = cached_cs[chn-1]
                           out_line = "%s%11.5f" % (out_line, avg_val)
                        elif line_modif == "avg_weight":
                           if cached_cs[chn-1] !=0:
                              out_line = "%s%11.7f" % (out_line, cached_rate[chn-1]/cached_cs[chn-1]) 
                           else:
                              out_line = "%s%11.7f" % (out_line, base_val)
                        elif "stat_unc_" in line_modif:
                           if cached_cs[chn-1] !=0:
                              out_line = "%s%11.3f" % (out_line, math.sqrt(cached_cs[chn-1])/cached_cs[chn-1])
                           else:
                              out_line = "%s%11.3f" % (out_line, 0)
                     out_file.write(out_line+"\n")
         if not modified:
            out_file.write(base_line)

      out_file.write("%11s = " % ("syst_metMag_up"))
      for chn in range(1, int(channels)+1):
         out_file.write("%11.7f" % cached_syst_for_avg[chn-1])
      out_file.write("\n")
 
      out_file.write("%11s = " % ("syst_metMag_dn"))
      for chn in range(1, int(channels)+1):
         out_file.write("%11.7f" % cached_syst_for_avg[chn-1])
      out_file.write("\n")
 
      out_file.close()

      base_file.close()
      dest_file.close()

if __name__ == "__main__":
   main()
