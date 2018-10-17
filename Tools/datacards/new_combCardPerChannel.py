#!/usr/bin/env python

#from os import listdir
#from os import sys
#from os import system
#from os import path
import os
import math
from optparse import OptionParser # Command line parsing
import numpy as np
import re

import ROOT as rt

def procline(inputline, procComment = False, adjZEROrate = False, adjVal = 0.00001):
   outline = {}
   tmpline = inputline.split()
   if len(tmpline) == 0: return outline
   isComments = False
   if "#" in tmpline[0]:
      isComments = True
      if not procComment and isComments:
         return outline
   start = False
   curr = ''
   for each in tmpline:
      if "#" == each[0]:
         continue
      if each == "=":
         start = True
         continue
      if not start:
         curr = each
         continue
      ori_each = each
      if curr == 'rate' and float(each) == 0 and adjZEROrate:
         each = str(adjVal)
      curr = re.sub(r'down', r'dn', curr) 
      if not outline:
         outline[curr] = [each]
         if curr == 'rate':
            outline['ori_rate'] = [ori_each]
      else:
         outline[curr].append(each)
         if curr == 'rate':
            outline['ori_rate'].append(ori_each)
   return outline

def procfile(inputfile, procComment = False, adjZEROrate = False, adjVal = 0.00001):
   output = {}
   with open(inputfile) as infile:
      for line in infile:
         outline = procline(line, procComment, adjZEROrate, adjVal)
         if outline:
            output.update(outline)
   infile.close()
   return output

def declHists(val_channels, rt_file_name = "searchBins.root"):
   rtfile = rt.TFile(rt_file_name, "RECREATE")

   output = {}

   h1_data = rt.TH1D("data", "data", val_channels, 0, val_channels)
   h1_data.Sumw2()
   output['data'] = h1_data
   h1_ttbarW = rt.TH1D("ttbarW", "ttbarW", val_channels, 0, val_channels)
   h1_ttbarW.Sumw2()
   output['ttbarW'] = h1_ttbarW

   h1_lostle = rt.TH1D("lostle", "lostle", val_channels, 0, val_channels)
   h1_lostle.Sumw2()
   output['lostle'] = h1_lostle
   h1_hadtau = rt.TH1D("hadtau", "hadtau", val_channels, 0, val_channels)
   h1_hadtau.Sumw2()
   output['hadtau'] = h1_hadtau
   h1_zinv = rt.TH1D("zinv", "zinv", val_channels, 0, val_channels)
   h1_zinv.Sumw2()
   output['zinv'] = h1_zinv
   h1_qcd = rt.TH1D("qcd", "qcd", val_channels, 0, val_channels)
   h1_qcd.Sumw2()
   output['qcd'] = h1_qcd
   h1_ttz = rt.TH1D("ttz", "ttz", val_channels, 0, val_channels)
   h1_ttz.Sumw2()
   output['ttz'] = h1_ttz
   h1_rare = rt.TH1D("rare", "rare", val_channels, 0, val_channels)
   h1_rare.Sumw2()
   output['rare'] = h1_rare

   h1_ttbarW_syst = rt.TH1D("ttbarW_syst", "ttbarW_syst", val_channels, 0, val_channels)
   h1_ttbarW_syst.Sumw2()
   output['ttbarW_syst'] = h1_ttbarW_syst

   h1_lostle_syst = rt.TH1D("lostle_syst", "lostle_syst", val_channels, 0, val_channels)
   h1_lostle_syst.Sumw2()
   output['lostle_syst'] = h1_lostle_syst
   h1_hadtau_syst = rt.TH1D("hadtau_syst", "hadtau_syst", val_channels, 0, val_channels)
   h1_hadtau_syst.Sumw2()
   output['hadtau_syst'] = h1_hadtau_syst
   h1_zinv_syst = rt.TH1D("zinv_syst", "zinv_syst", val_channels, 0, val_channels)
   h1_zinv_syst.Sumw2()
   output['zinv_syst'] = h1_zinv_syst
   h1_qcd_syst = rt.TH1D("qcd_syst", "qcd_syst", val_channels, 0, val_channels)
   h1_qcd_syst.Sumw2()
   output['qcd_syst'] = h1_qcd_syst
   h1_ttz_syst = rt.TH1D("ttz_syst", "ttz_syst", val_channels, 0, val_channels)
   h1_ttz_syst.Sumw2()
   output['ttz_syst'] = h1_ttz_syst
   h1_rare_syst = rt.TH1D("rare_syst", "rare_syst", val_channels, 0, val_channels)
   h1_rare_syst.Sumw2()
   output['rare_syst'] = h1_rare_syst

   return rtfile, output

def getHyphenFormats(iv_proc_name, proc='signal', item_width=14, offset=0):
   num_proc = len(iv_proc_name)
   idx_proc = iv_proc_name[proc]
   pre_hyphen = '{:<{width}s}  '.format('-', width=item_width)*(idx_proc-offset)
   aft_hyphen = '{:<{width}s}  '.format('-', width=item_width)*(num_proc-1-idx_proc-offset)
   return pre_hyphen, aft_hyphen

proc_name = {0:'signal', 1:'ttbarW', 2:'zinv', 3:'qcd', 4:'ttz', 5:'rare'}
num_proc = len(proc_name)
iv_proc_name = dict( (val, key) for key, val in proc_name.items())

def prodCardPerChn(signal_key, outputdir='', comb_comb_file = 'comb_comb.txt', comb_mu_file ='comb_mu.txt', comb_ele_file ='comb_ele.txt', zinv_file='zinv.txt', qcd_file='qcd.txt', ttz_file='ttz.txt', rare_file='rare.txt', data_file='data.txt', signal_file ='signal.txt'):

   doExpLimitOnly = False

   lnU_rate_for_ZERO = 0.005
   lnU_range_for_ZERO = 10.0
   lnU_range = 10.0

   outbase_filename = "comb_" + signal_key
   print '\noutbase_filename : %s\n' % (outbase_filename)

   prt_table_file = open("prt_table.txt", "w")

   all_output_mapper = {}

   all_output_mapper['data'] = procfile(data_file)
   # parsing data file first to get basic information
   global channels
   if all_output_mapper['data'].has_key('luminosity') : lumi = all_output_mapper['data']['luminosity'][0]
   if all_output_mapper['data'].has_key('channels') : channels = all_output_mapper['data']['channels'][0]

   val_channels = int(channels)

# Note that the 'rate' numbers are adjusted specific to fit the data card formatting.
# While possible, a 'ori_rate' is kept for the original ones (this is automatcally done in the procfile() function)
# 'signal' : might need to be modified due to signal contamination (TODO)
# 'ttbarW' : this is used to setup the lep CS, in order to tell the combine tool the TF, the rate cannot be 0 (force to lnU_rate_for_ZERO)
# all others : a very small number (0.00001) is used to replace 0.0 since otherwise the combine tool will complain
   all_output_mapper['comb_mu'] = procfile(comb_mu_file, procComment=True, adjZEROrate=True)
   all_output_mapper['comb_ele'] = procfile(comb_ele_file, procComment=True, adjZEROrate=True)
   all_output_mapper['zinv'] = procfile(zinv_file, adjZEROrate=True)
   all_output_mapper['qcd'] = procfile(qcd_file)
   all_output_mapper['ttz'] = procfile(ttz_file, adjZEROrate=True)
   all_output_mapper['rare'] = procfile(rare_file, adjZEROrate=True)
   all_output_mapper['signal'] = procfile(signal_file, adjZEROrate=True)
# this ttbarW entry is only used for rate and limited place as the final 
# data cards have mu and electron channel seperated.
   all_output_mapper['ttbarW'] = procfile(comb_comb_file, procComment=True, adjZEROrate=True, adjVal=lnU_rate_for_ZERO)

## special treatment for qcd to get its rate
#   qcd_tfactor_cnt = 1
#   qcd_tfactor_dict = {}
#
#   qcd_nonclosure_cnt = 1
#   qcd_nonclosure_dict = {}
#
#   for i in range(val_channels):
#      qcd_cs = float(all_output_mapper['qcd']['QCD_Data_CS'][i])
#      qcd_contam_pred = float(all_output_mapper['qcd']['QCD_otherBG_CS'][i])
#
#      qcd_tfactor = float(all_output_mapper['qcd']['QCD_TFactor'][i])
#      qcd_tfactor_str = str(qcd_tfactor)
#      if not (qcd_tfactor_str in qcd_tfactor_dict.keys()):
#         qcd_tfactor_dict[qcd_tfactor_str] = qcd_tfactor_cnt
#         qcd_tfactor_cnt += 1 
#      
#      qcd_nonclosure_str = all_output_mapper['qcd']['QCD_NonClosure_relative_err'][i]
#      if not (qcd_nonclosure_str in qcd_nonclosure_dict.keys()):
#         qcd_nonclosure_dict[qcd_nonclosure_str] = qcd_nonclosure_cnt
#         qcd_nonclosure_cnt += 1 
#      
#      qcd_rate = qcd_cs*qcd_tfactor if qcd_cs !=0 else lnU_rate_for_ZERO*qcd_tfactor
#      qcd_corr_rate = (qcd_cs-qcd_contam_pred)*qcd_tfactor if (qcd_cs-qcd_contam_pred)*qcd_tfactor >=0 else 0
#      if i ==0:
#         all_output_mapper['qcd']['rate'] = [qcd_rate]
##         all_output_mapper['qcd']['corr_rate'] = [qcd_corr_rate]
#         all_output_mapper['qcd']['ori_rate'] = [qcd_corr_rate]
#      else:
#         all_output_mapper['qcd']['rate'].append(qcd_rate)
##         all_output_mapper['qcd']['corr_rate'].append(qcd_corr_rate)
#         all_output_mapper['qcd']['ori_rate'].append(qcd_corr_rate)

# special treatment for signal
#   for i in range(val_channels):
#      signal_cs_event = all_output_mapper['signal']['cs_event'][i]
#      signal_avg_weight = all_output_mapper['signal']['avg_weight'][i]
#      signal_contam = all_output_mapper['signal']['contam'][i]
#      all_output_mapper['signal']['rate'][i] = (signal_cs_event-signal_contam)*signal_avg_weight

   rt_file, decl_hists = declHists(val_channels)

   header_width = 38
   func_width = 12
   total_label_width = header_width + func_width
   item_width = 14

   for chn in range(1, val_channels+1):

      pred_tot_rate = 0
      pred_tot_stat = 0

      prt_stat_unc_up = {}
      prt_stat_unc_dn = {}

      prt_syst_unc_up = {}
      prt_syst_unc_dn = {}

      pred_rate = {}

      for key, val in proc_name.items():
         if val=='signal': continue
         per_rate = float(all_output_mapper[val]['ori_rate'][chn-1])
         pred_rate[val] = per_rate
         if ('ttbarW' in val) or ('comb' in val):
            stat_unc_avg = 0.5*(float(all_output_mapper[val]['stat_unc_abs_up'][chn-1]) + float(all_output_mapper[val]['stat_unc_abs_dn'][chn-1]))
            prt_stat_unc_up[val] = float(all_output_mapper[val]['stat_unc_abs_up'][chn-1])
            prt_stat_unc_dn[val] = float(all_output_mapper[val]['stat_unc_abs_dn'][chn-1])
         #elif ('zinv' in val) or ('ttz' in val) or ('rare' in val):
         elif ('zinv' in val) or ('ttz' in val) or ('rare' in val) or ('qcd' in val):
            cs_event = float(all_output_mapper[val]['cs_event'][chn-1]) 
            avg_weight = float(all_output_mapper[val]['avg_weight'][chn-1])
            stat_unc_avg = math.sqrt(cs_event) * avg_weight
            prt_stat_unc_up[val] = math.sqrt(cs_event) * avg_weight
            prt_stat_unc_dn[val] = math.sqrt(cs_event) * avg_weight
            if cs_event == 0:
               stat_unc_avg = 1.84 * avg_weight
               prt_stat_unc_up[val] = 1.84 * avg_weight
               prt_stat_unc_dn[val] = 0.0
#         elif 'qcd' in val:
#            qcd_cs = float(all_output_mapper[val]['QCD_Data_CS'][chn-1])
#            qcd_tfactor = float(all_output_mapper[val]['QCD_TFactor'][chn-1])
#            stat_unc_avg = math.sqrt(qcd_cs) * qcd_tfactor
#            prt_stat_unc_up[val] = math.sqrt(qcd_cs) * qcd_tfactor
#            prt_stat_unc_dn[val] = math.sqrt(qcd_cs) * qcd_tfactor
#            if qcd_cs ==0 :
#               stat_unc_avg = 1.84 * qcd_tfactor
#               prt_stat_unc_up[val] = 1.84 * qcd_tfactor
#               prt_stat_unc_dn[val] = 0.0
         pred_tot_rate += per_rate
         decl_hists[val].SetBinContent(chn, per_rate)
         decl_hists[val].SetBinError(chn, stat_unc_avg)
         decl_hists[val+'_syst'].SetBinContent(chn, per_rate)

      if len(outputdir) !=0:
         outfile_perchn = open(outputdir + "/" + outbase_filename + "_ch" + str(chn) + ".txt", "w")
      else:
         outfile_perchn = open(outbase_filename + "_ch" + str(chn) + ".txt", "w")
      outfile_perchn.write("""imax 1 # number of channels\n
jmax {} # number of backgrounds\n
kmax * nuissance\n
shapes * * FAKE\n
----------------\n
bin         bin{:d}\n""".format(num_proc-1, chn))

      data_rate = all_output_mapper['data']['rate'][chn-1]
      outfile_perchn.write("observation {:s}\n".format(str(data_rate)))

      decl_hists['data'].SetBinContent(chn, float(data_rate))
      decl_hists['data'].SetBinError(chn, math.sqrt(float(data_rate)))

      outfile_perchn.write('{:<{width}s}'.format('bin', width=total_label_width)+'{:<{width}s}'.format('bin'+str(chn), width = item_width+2)*num_proc+'\n')
      outfile_perchn.write('{:<{width}s}'.format('process', width=total_label_width)+'  '.join('{:<{width}s}'.format(val, width=item_width) for key, val in proc_name.items()) + '\n')
      outfile_perchn.write('{:<{width}s}'.format('process', width=total_label_width)+'  '.join('{:<{width}s}'.format(str(key), width=item_width) for key, val in proc_name.items()) + '\n')
      outfile_perchn.write('{:<{width}s}'.format('rate', width=total_label_width)+'  '.join('{:<{width}.5f}'.format(float(all_output_mapper[val]['rate'][chn-1]), width=item_width) for key, val in proc_name.items()) + '\n')

      outfile_perchn.write("---------------------------------------\n")
# Now processing signals -- we usually do NOT need correlate its unc to backgrounds...
# Many of the syst are in pair of (up, dn), so only look for 'up' in the loop, then direct get 'dn' if found 'up'
      idx_signal = iv_proc_name['signal']
      pre_hyphen, aft_hyphen = getHyphenFormats(iv_proc_name, 'signal', item_width)

      signal_mapper = all_output_mapper['signal']
      for key, val in signal_mapper.items():
         if re.match(r'^stat_.*up.*', key):
            key = re.sub(r'_up', '', key)
            signal_cs_event = float(signal_mapper['cs_event'][chn-1])
            signal_contam = float(signal_mapper['contam'][chn-1])
            signal_avg_weight = float(signal_mapper['avg_weight'][chn-1])
            header_str = 'signal_'+key+'_chn'+str(chn)
            func_str = 'gmN  {:0.0f}  '.format(signal_cs_event-signal_contam)
            outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen+'{:<{width}.5f}'.format(signal_avg_weight, width=item_width)+'  '+aft_hyphen+'\n')
         if re.match(r'^syst_.*up.*', key):
            if 'pdfUnc' in key:
               continue
            dn_key = re.sub(r'up', r'dn', key)
            up_val = float(val[chn-1])
            dn_val = float(signal_mapper[dn_key][chn-1])
            key_key = re.sub(r'_up', '', key)
# Down unc cannot be over 100%!
            if dn_val>=1:
               dn_val = 1.0 - 0.001
            if up_val<=-1:
               up_val = -1.0 + 0.001
            header_str = 'signal_'+key_key
            func_str = '  lnN  '
            outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen+'{:>{width1}.4f}/{:<{width2}.4f}'.format(1-dn_val, 1+up_val, width1=int((item_width-2)/2), width2=int((item_width-2)/2) )+'   '+aft_hyphen+'\n')

# Now processing ttbarW
      pre_hyphen, aft_hyphen = getHyphenFormats(iv_proc_name, 'ttbarW', item_width)

      ttbarW_mapper = all_output_mapper['ttbarW']
      comb_mu_mapper = all_output_mapper['comb_mu']
      comb_ele_mapper = all_output_mapper['comb_ele']

      ttbarW_rate = float(ttbarW_mapper['rate'][chn-1])

      header_str = 'ttbarW_CS_ch'+str(chn)
      func_str = '  lnU  '
      to_set_lnU_range_ttbarW = lnU_range
      if float(ttbarW_mapper['ori_rate'][chn-1]) == 0: to_set_lnU_range_ttbarW = lnU_range_for_ZERO
      outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen+'{:<{width}.5f}'.format(to_set_lnU_range_ttbarW, width=item_width)+'  '+aft_hyphen + '\n')

      prt_syst_unc_up['ttbarW'] = 0.0
      prt_syst_unc_dn['ttbarW'] = 0.0
      for key, val in ttbarW_mapper.items():
         if re.match(r'^syst_.*up.*', key):
            dn_key = re.sub(r'up', r'dn', key)
            up_val = float(val[chn-1])
            dn_val = float(ttbarW_mapper[dn_key][chn-1])
            prt_syst_unc_up['ttbarW'] += (up_val*up_val)
            prt_syst_unc_dn['ttbarW'] += (dn_val*dn_val)
            key_key = re.sub(r'_up', '', key)
# Down unc cannot be over 100%!
            if dn_val>=1:
               dn_val = 1.0 - 0.001
            header_str = 'ttbarW_'+key_key
            func_str = '  lnN  '
            outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen+'{:>{width1}.4f}/{:<{width2}.4f}'.format(1-dn_val, 1+up_val, width1=int((item_width-2)/2), width2=int((item_width-2)/2) )+'   '+aft_hyphen+'\n')
      prt_syst_unc_up['ttbarW'] = math.sqrt(prt_syst_unc_up['ttbarW']) * pred_rate['ttbarW']
      prt_syst_unc_dn['ttbarW'] = math.sqrt(prt_syst_unc_dn['ttbarW']) * pred_rate['ttbarW']
      if prt_syst_unc_dn['ttbarW'] > pred_rate['ttbarW']: prt_syst_unc_dn['ttbarW'] = pred_rate['ttbarW']

# muCS
      ttbarW_muCS_proc_name = {0:'signal', 1:'ttbarW_muCS'}
      num_ttbarW_muCS_proc = len(ttbarW_muCS_proc_name)
      iv_ttbarW_muCS_proc_name = {val:key for key, val in ttbarW_muCS_proc_name.items()}
      pre_hyphen_muCS, aft_hyphen_muCS = getHyphenFormats(iv_ttbarW_muCS_proc_name, 'ttbarW_muCS', item_width*2)

      if len(outputdir) !=0:
         ttbarW_muCS_perchn = open(outputdir + "/comb_ttbarW_muCS_" + signal_key + "_ch" + str(chn) + ".txt", "w")
      else:
         ttbarW_muCS_perchn = open("comb_ttbarW_muCS_" + signal_key + "_ch" + str(chn) + ".txt", "w")
      ttbarW_muCS_perchn.write("""imax 1 # number of channels\n
jmax {} # number of backgrounds\n
kmax * nuissance\n
shapes * * FAKE\n
----------------\n
bin         bin_ttbarW_muCS_ch{:d}\n""".format(num_ttbarW_muCS_proc-1, chn))
      muCS_rate = float(comb_mu_mapper['rate'][chn-1])
      muCS_tf = float(comb_mu_mapper['fin_TF_to_mu'][chn-1])
      muCS_num = int(muCS_rate/muCS_tf)
      muCS_signal_rate = 0.0001

      ttbarW_muCS_perchn.write('observation {:d}\n'.format(muCS_num))
      ttbarW_muCS_perchn.write('{:<{width}s}'.format('bin', width=total_label_width)+'{:<{width}s}'.format('bin_ttbarW_muCS_ch'+str(chn), width = item_width*2+2)*num_ttbarW_muCS_proc+'\n')
      ttbarW_muCS_perchn.write('{:<{width}s}'.format('process', width=total_label_width)+'  '.join('{:<{width}s}'.format(val, width=item_width*2) for key, val in ttbarW_muCS_proc_name.items()) + '\n')
      ttbarW_muCS_perchn.write('{:<{width}s}'.format('process', width=total_label_width)+'  '.join('{:<{width}s}'.format(str(key), width=item_width*2) for key, val in ttbarW_muCS_proc_name.items()) + '\n')
      ttbarW_muCS_perchn.write('{:<{width}s}'.format('rate', width=total_label_width)+'{:<{width}.5f}'.format(muCS_signal_rate, width=item_width*2)+'  {:<{width}.5f}'.format(ttbarW_rate/muCS_tf, width=item_width*2) + '\n')
      ttbarW_muCS_perchn.write("---------------------------------------\n")
      header_str = 'ttbarW_CS_ch'+str(chn)
      func_str = '  lnU  '
      ttbarW_muCS_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen_muCS+'{:<{width}.5f}'.format(to_set_lnU_range_ttbarW, width=item_width*2)+'  '+aft_hyphen_muCS + '\n')

      for key, val in comb_mu_mapper.items():
         if re.match(r'^syst_.*up.*', key):
            dn_key = re.sub(r'up', r'dn', key)
            up_val = float(val[chn-1])
            dn_val = float(comb_mu_mapper[dn_key][chn-1])
            key_key = re.sub(r'_up', '', key)
# Down unc cannot be over 100%!
            if dn_val>=1:
               dn_val = 1.0 - 0.001
            header_str = 'ttbarW_'+key_key
            func_str = '  lnN  '
#            ttbarW_muCS_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen_muCS+'{:>{width1}.4f}/{:<{width2}.4f}'.format(1-dn_val, 1+up_val, width1=int((item_width-2)/2), width2=int((item_width-2)/2) )+'   '+aft_hyphen_muCS+'\n')

# eleCS
      ttbarW_eleCS_proc_name = {0:'signal', 1:'ttbarW_eleCS'}
      num_ttbarW_eleCS_proc = len(ttbarW_eleCS_proc_name)
      iv_ttbarW_eleCS_proc_name = {val:key for key, val in ttbarW_eleCS_proc_name.items()}
      pre_hyphen_eleCS, aft_hyphen_eleCS = getHyphenFormats(iv_ttbarW_eleCS_proc_name, 'ttbarW_eleCS', item_width*2)

      if len(outputdir) !=0:
         ttbarW_eleCS_perchn = open(outputdir + "/comb_ttbarW_eleCS_" + signal_key + "_ch" + str(chn) + ".txt", "w")
      else:
         ttbarW_eleCS_perchn = open("comb_ttbarW_eleCS_" + signal_key + "_ch" + str(chn) + ".txt", "w")
      ttbarW_eleCS_perchn.write("""imax 1 # number of channels\n
jmax {} # number of backgrounds\n
kmax * nuissance\n
shapes * * FAKE\n
----------------\n
bin         bin_ttbarW_eleCS_ch{:d}\n""".format(num_ttbarW_eleCS_proc-1, chn))
      eleCS_rate = float(comb_ele_mapper['rate'][chn-1])
      eleCS_tf = float(comb_ele_mapper['fin_TF_to_ele'][chn-1])
      eleCS_num = int(eleCS_rate/eleCS_tf)
      eleCS_signal_rate = 0.0001

      ttbarW_eleCS_perchn.write('observation {:d}\n'.format(eleCS_num))
      ttbarW_eleCS_perchn.write('{:<{width}s}'.format('bin', width=total_label_width)+'{:<{width}s}'.format('bin_ttbarW_eleCS_ch'+str(chn), width = item_width*2+2)*num_ttbarW_eleCS_proc+'\n')
      ttbarW_eleCS_perchn.write('{:<{width}s}'.format('process', width=total_label_width)+'  '.join('{:<{width}s}'.format(val, width=item_width*2) for key, val in ttbarW_eleCS_proc_name.items()) + '\n')
      ttbarW_eleCS_perchn.write('{:<{width}s}'.format('process', width=total_label_width)+'  '.join('{:<{width}s}'.format(str(key), width=item_width*2) for key, val in ttbarW_eleCS_proc_name.items()) + '\n')
      ttbarW_eleCS_perchn.write('{:<{width}s}'.format('rate', width=total_label_width)+'{:<{width}.5f}'.format(eleCS_signal_rate, width=item_width*2)+'  {:<{width}.5f}'.format(ttbarW_rate/eleCS_tf, width=item_width*2) + '\n')
      ttbarW_eleCS_perchn.write("---------------------------------------\n")
      header_str = 'ttbarW_CS_ch'+str(chn)
      func_str = '  lnU  '
      ttbarW_eleCS_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen_eleCS+'{:<{width}.5f}'.format(to_set_lnU_range_ttbarW, width=item_width*2)+'  '+aft_hyphen_eleCS + '\n')

      for key, val in comb_ele_mapper.items():
         if re.match(r'^syst_.*up.*', key):
            dn_key = re.sub(r'up', r'dn', key)
            up_val = float(val[chn-1])
            dn_val = float(comb_ele_mapper[dn_key][chn-1])
            key_key = re.sub(r'_up', '', key)
# Down unc cannot be over 100%!
            if dn_val>=1:
               dn_val = 1.0 - 0.001
            header_str = 'ttbarW_'+key_key
            func_str = '  lnN  '
#            ttbarW_eleCS_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen_eleCS+'{:>{width1}.4f}/{:<{width2}.4f}'.format(1-dn_val, 1+up_val, width1=int((item_width-2)/2), width2=int((item_width-2)/2) )+'   '+aft_hyphen_eleCS+'\n')

# Now processing zinv
      idx_zinv = iv_proc_name['zinv']
      pre_hyphen, aft_hyphen = getHyphenFormats(iv_proc_name, 'zinv', item_width)

      zinv_mapper = all_output_mapper['zinv']
      prt_syst_unc_up['zinv'] = 0.0
      prt_syst_unc_dn['zinv'] = 0.0
      for key, val in zinv_mapper.items():
         if re.match(r'^cs_event', key):
            zinv_cs_event = float(zinv_mapper['cs_event'][chn-1])
            zinv_avg_weight = float(zinv_mapper['avg_weight'][chn-1])
            header_str = 'zinv_stat_unc_chn'+str(chn)
            func_str = 'gmN  {:0.0f}  '.format(zinv_cs_event)
            outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen+'{:<{width}.5f}'.format(zinv_avg_weight, width=item_width)+'  '+aft_hyphen+'\n')
         if re.match(r'^syst_.*up.*', key):
            dn_key = re.sub(r'up', r'dn', key)
            up_val = float(val[chn-1])
            dn_val = float(zinv_mapper[dn_key][chn-1])
            prt_syst_unc_up['zinv'] += (up_val*up_val)
            prt_syst_unc_dn['zinv'] += (dn_val*dn_val)
            key_key = re.sub(r'_up', '', key)
# Down unc cannot be over 100%!
            if dn_val>=1:
               dn_val = 1.0 - 0.001
            header_str = 'zinv_'+key_key
            func_str = '  lnN  '
            outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen+'{:>{width1}.4f}/{:<{width2}.4f}'.format(1-dn_val, 1+up_val, width1=int((item_width-2)/2), width2=int((item_width-2)/2) )+'   '+aft_hyphen+'\n')
      prt_syst_unc_up['zinv'] = math.sqrt(prt_syst_unc_up['zinv']) * pred_rate['zinv']
      prt_syst_unc_dn['zinv'] = math.sqrt(prt_syst_unc_dn['zinv']) * pred_rate['zinv']
      if prt_syst_unc_dn['zinv'] > pred_rate['zinv']: prt_syst_unc_dn['zinv'] = pred_rate['zinv']

# Now processing QCD like zinv
      idx_qcd = iv_proc_name['qcd']
      pre_hyphen, aft_hyphen = getHyphenFormats(iv_proc_name, 'qcd', item_width)

      qcd_mapper = all_output_mapper['qcd']
      prt_syst_unc_up['qcd'] = 0.0
      prt_syst_unc_dn['qcd'] = 0.0
      for key, val in qcd_mapper.items():
         if re.match(r'^cs_event', key):
            qcd_cs_event = float(qcd_mapper['cs_event'][chn-1])
            qcd_avg_weight = float(qcd_mapper['avg_weight'][chn-1])
            header_str = 'qcd_stat_unc_chn'+str(chn)
            func_str = 'gmN  {:0.0f}  '.format(qcd_cs_event)
            outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen+'{:<{width}.5f}'.format(qcd_avg_weight, width=item_width)+'  '+aft_hyphen+'\n')
         if re.match(r'^syst_.*up.*', key):
            dn_key = re.sub(r'up', r'dn', key)
            up_val = float(val[chn-1])
            dn_val = float(qcd_mapper[dn_key][chn-1])
            prt_syst_unc_up['qcd'] += (up_val*up_val)
            prt_syst_unc_dn['qcd'] += (dn_val*dn_val)
            key_key = re.sub(r'_up', '', key)
# Down unc cannot be over 100%!
            if dn_val>=1:
               dn_val = 1.0 - 0.001
            header_str = 'qcd_'+key_key
            func_str = '  lnN  '
            outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen+'{:>{width1}.4f}/{:<{width2}.4f}'.format(1-dn_val, 1+up_val, width1=int((item_width-2)/2), width2=int((item_width-2)/2) )+'   '+aft_hyphen+'\n')
      prt_syst_unc_up['qcd'] = math.sqrt(prt_syst_unc_up['qcd']) * pred_rate['qcd']
      prt_syst_unc_dn['qcd'] = math.sqrt(prt_syst_unc_dn['qcd']) * pred_rate['qcd']
      if prt_syst_unc_dn['qcd'] > pred_rate['qcd']: prt_syst_unc_dn['qcd'] = pred_rate['qcd']


## Now processing qcd
#      idx_qcd = iv_proc_name['qcd']
#      pre_hyphen, aft_hyphen = getHyphenFormats(iv_proc_name, 'qcd', item_width)
#
#      qcd_mapper = all_output_mapper['qcd']
#
#      pre_hyphen, aft_hyphen = getHyphenFormats(iv_proc_name, 'qcd', item_width)
#      header_str = 'invertDphi_ch'+str(chn)
#      func_str = '  lnU  '
#      to_set_lnU_range_qcd = lnU_range
#      if float(qcd_mapper['QCD_Data_CS'][chn-1])==0: to_set_lnU_range_qcd = lnU_range_for_ZERO
#      outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen+'{:<{width}.5f}'.format(to_set_lnU_range_qcd, width=item_width)+'  '+aft_hyphen + '\n')
#
#      prt_syst_unc_up['qcd'] = 0.0
#      prt_syst_unc_dn['qcd'] = 0.0
#      for key, val in qcd_mapper.items():
#         if re.match(r'^QCD_TFactor_relative', key):
#            qcd_tfactor = qcd_mapper['QCD_TFactor'][chn-1]
#            up_val = float(val[chn-1])
#            dn_val = up_val
#            prt_syst_unc_up['qcd'] += (up_val*up_val)
#            prt_syst_unc_dn['qcd'] += (dn_val*dn_val)
#            if dn_val>=1:
#               dn_val = 1.0 - 0.001
#            header_str = 'qcd_tfactor_chn{:d}'.format(qcd_tfactor_dict[qcd_tfactor])
#            func_str = '  lnN  '
#            outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen+'{:>{width1}.4f}/{:<{width2}.4f}'.format(1-dn_val, 1+up_val, width1=int((item_width-2)/2), width2=int((item_width-2)/2) )+'   '+aft_hyphen+'\n')
#         if re.match(r'QCD_NonClosure_relative', key):
#            up_val = float(val[chn-1])
#            dn_val = up_val
#            prt_syst_unc_up['qcd'] += (up_val*up_val)
#            prt_syst_unc_dn['qcd'] += (dn_val*dn_val)
#            if dn_val >=1:
#               dn_val = 1.0 - 0.001
#            header_str = 'qcd_nonclosure_chn{:d}'.format(qcd_nonclosure_dict[val[chn-1]])
#            func_str = '  lnN  '
#            outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen+'{:>{width1}.4f}/{:<{width2}.4f}'.format(1-dn_val, 1+up_val, width1=int((item_width-2)/2), width2=int((item_width-2)/2) )+'   '+aft_hyphen+'\n')
#      prt_syst_unc_up['qcd'] = math.sqrt(prt_syst_unc_up['qcd']) * pred_rate['qcd']
#      prt_syst_unc_dn['qcd'] = math.sqrt(prt_syst_unc_dn['qcd']) * pred_rate['qcd']
#      if prt_syst_unc_dn['qcd'] > pred_rate['qcd']: prt_syst_unc_dn['qcd'] = pred_rate['qcd']
#
## qcd CS
#      invertDphi_proc_name = {0:'signal', 1:'qcd', 2:'contam'}
#      num_invertDphi_proc = len(invertDphi_proc_name)
#      ivs_invertDphi_proc_name = {val:key for key, val in invertDphi_proc_name.items()}
#
#      if len(outputdir) !=0:
#         qcd_outfile_perchn = open(outputdir + "/comb_invertDphi_"+ signal_key + "_ch" + str(chn) + ".txt", "w")
#      else:
#         qcd_outfile_perchn = open("comb_invertDphi_"+ signal_key + "_ch" + str(chn) + ".txt", "w")
#      
#      qcd_outfile_perchn.write("""imax 1 # number of channels\n
#jmax {} # number of backgrounds\n
#kmax * nuissance\n"
#shapes * * FAKE\n
#----------------\n
#bin         bin_invertDphi_ch{:d}\n""".format(num_invertDphi_proc-1, chn))
#
#      qcd_cs = float(qcd_mapper['QCD_Data_CS'][chn-1])
#      qcd_signal_rate = 0.0001
## In the main card, we have:
## qcd_rate = qcd_cs*qcd_tfactor if qcd_cs !=0 else qcd_tfactor 
## this means we force qcd_cs to lnU_rate_for_ZERO when it's 0
#      qcd_pseudo_rate = qcd_cs if qcd_cs !=0 else lnU_rate_for_ZERO
#      qcd_contam_pred = float(all_output_mapper['qcd']['QCD_otherBG_CS'][chn-1])
#
#      qcd_outfile_perchn.write("observation {:0.0f}\n".format(qcd_cs))
#      qcd_outfile_perchn.write('{:<{width}s}'.format('bin', width=total_label_width)+'{:<{width}s}'.format('bin_invertDphi_ch'+str(chn), width = item_width*2+2)*num_invertDphi_proc+'\n')
#      qcd_outfile_perchn.write('{:<{width}s}'.format('process', width=total_label_width)+'  '.join('{:<{width}s}'.format(val, width=item_width*2) for key, val in invertDphi_proc_name.items()) + '\n')
#      qcd_outfile_perchn.write('{:<{width}s}'.format('process', width=total_label_width)+'  '.join('{:<{width}s}'.format(str(key), width=item_width*2) for key, val in invertDphi_proc_name.items()) + '\n')
#      qcd_outfile_perchn.write('{:<{width}s}'.format('rate', width=total_label_width)+'{:<{width}.5f}'.format(qcd_signal_rate, width=item_width*2)+'  {:<{width}.5f}'.format(qcd_pseudo_rate, width=item_width*2)
#                        +'  {:<{width}.5f}'.format(qcd_contam_pred, width=item_width*2) + '\n')
#      qcd_outfile_perchn.write("---------------------------------------\n")
#
#      pre_hyphen_invertDphi, aft_hyphen_invertDphi = getHyphenFormats(ivs_invertDphi_proc_name, 'qcd', item_width*2)
#      header_str = 'invertDphi_ch'+str(chn)
#      func_str = '  lnU  '
#      qcd_outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen_invertDphi+'{:<{width}.5f}'.format(to_set_lnU_range_qcd, width=item_width*2)+'  '+aft_hyphen_invertDphi + '\n')
#
#      pre_hyphen_invertDphi, aft_hyphen_invertDphi = getHyphenFormats(ivs_invertDphi_proc_name, 'contam', item_width*2)
#      up_val = float(qcd_mapper['QCD_otherBG_CS_relative_errup'][chn-1])
#      dn_val = float(qcd_mapper['QCD_otherBG_CS_relative_errdn'][chn-1])
#      if dn_val>=1:
#         dn_val = 1.0 - 0.001
#      header_str = 'qcd_contamUnc_chn{:d}'.format(chn)
#      func_str = '  lnN  '
#      qcd_outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_hyphen_invertDphi + '{:>{width1}.4f}/{:<{width2}.4f}'.format(1-dn_val, 1+up_val, width1=int((item_width-2)/2), width2=int((item_width-2)/2) )+'   '+aft_hyphen_invertDphi +'\n')
#
# Now processing ttz + rare
      ttz_mapper = all_output_mapper['ttz']
      rare_mapper = all_output_mapper['rare']
      pre_ttz_hyphen, aft_ttz_hyphen = getHyphenFormats(iv_proc_name, 'ttz', item_width)
      pre_rare_hyphen, aft_rare_hyphen = getHyphenFormats(iv_proc_name, 'rare', item_width)

# ttz
      ttz_cs_event = float(ttz_mapper['cs_event'][chn-1])
      ttz_avg_weight = float(ttz_mapper['avg_weight'][chn-1])
#      if ttz_cs_event ==0: ttz_avg_weight = 0.00001
      header_str = 'ttz_stat_unc_chn'+str(chn)
      func_str = 'gmN  {:0.0f}  '.format(ttz_cs_event)
      outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_ttz_hyphen+'{:<{width}.5f}'.format(ttz_avg_weight, width=item_width)+'  '+aft_ttz_hyphen+'\n')

      ttz_rate = float(ttz_mapper['ori_rate'][chn-1])
      prt_syst_unc_up['ttz'] = 0.0
      prt_syst_unc_dn['ttz'] = 0.0
      for key, val in ttz_mapper.items():
         if re.match('^syst_.*up.*', key):
            dn_key = re.sub(r'up', r'dn', key)
            up_val = float(val[chn-1])
            dn_val = float(ttz_mapper[dn_key][chn-1])
            up_val = 0 if ttz_rate ==0 else up_val/ttz_rate
            dn_val = 0 if ttz_rate ==0 else dn_val/ttz_rate
            prt_syst_unc_up['ttz'] += (up_val*up_val)
            prt_syst_unc_dn['ttz'] += (dn_val*dn_val)
            key_key = re.sub(r'_up', '', key)
            if dn_val >=1:
               dn_val = 1.0 - 0.001
            header_str = 'ttz_'+key_key
            func_str = '  lnN  '
            outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_ttz_hyphen+'{:>{width1}.4f}/{:<{width2}.4f}'.format(1-dn_val, 1+up_val, width1=int((item_width-2)/2), width2=int((item_width-2)/2) )+'   '+aft_ttz_hyphen+'\n')
      prt_syst_unc_up['ttz'] = math.sqrt(prt_syst_unc_up['ttz']) * pred_rate['ttz']
      prt_syst_unc_dn['ttz'] = math.sqrt(prt_syst_unc_dn['ttz']) * pred_rate['ttz']
      if prt_syst_unc_dn['ttz'] > pred_rate['ttz']: prt_syst_unc_dn['ttz'] = pred_rate['ttz']
 
# rare
      rare_cs_event = float(rare_mapper['cs_event'][chn-1])
      rare_avg_weight = float(rare_mapper['avg_weight'][chn-1])
#      if rare_cs_event ==0: rare_avg_weight = 0.00001
      header_str = 'rare_stat_unc_chn'+str(chn)
      func_str = 'gmN  {:0.0f}  '.format(rare_cs_event)
      outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_rare_hyphen+'{:<{width}.5f}'.format(rare_avg_weight, width=item_width)+'  '+aft_rare_hyphen+'\n')

      rare_rate = float(rare_mapper['ori_rate'][chn-1])
      prt_syst_unc_up['rare'] = 0.0
      prt_syst_unc_dn['rare'] = 0.0
      for key, val in rare_mapper.items():
         if re.match('^syst_.*up.*', key):
            dn_key = re.sub(r'up', r'dn', key)
            up_val = float(val[chn-1])
            dn_val = float(rare_mapper[dn_key][chn-1])
            up_val = 0 if rare_rate ==0 else up_val/rare_rate
            dn_val = 0 if rare_rate ==0 else dn_val/rare_rate
            prt_syst_unc_up['rare'] += (up_val*up_val)
            prt_syst_unc_dn['rare'] += (dn_val*dn_val)
            key_key = re.sub(r'_up', '', key)
            if dn_val >=1:
               dn_val = 1.0 - 0.001
            header_str = 'rare_'+key_key
            func_str = '  lnN  '
            outfile_perchn.write('{:<{width1}s}{:>{width2}s}'.format(header_str, func_str, width1=header_width, width2=func_width)+pre_rare_hyphen+'{:>{width1}.4f}/{:<{width2}.4f}'.format(1-dn_val, 1+up_val, width1=int((item_width-2)/2), width2=int((item_width-2)/2) )+'   '+aft_rare_hyphen+'\n')
      prt_syst_unc_up['rare'] = math.sqrt(prt_syst_unc_up['rare']) * pred_rate['rare']
      prt_syst_unc_dn['rare'] = math.sqrt(prt_syst_unc_dn['rare']) * pred_rate['rare']
      if prt_syst_unc_dn['rare'] > pred_rate['rare']: prt_syst_unc_dn['rare'] = pred_rate['rare']

      prt_pred_tot_rate = pred_tot_rate
      prt_pred_tot_stat_up = 0
      prt_pred_tot_stat_dn = 0
      prt_pred_tot_syst_up = 0
      prt_pred_tot_syst_dn = 0
      for key, val in proc_name.items():
         if val=='signal': continue
         prt_pred_tot_stat_up += prt_stat_unc_up[val]*prt_stat_unc_up[val]         
         prt_pred_tot_stat_dn += prt_stat_unc_dn[val]*prt_stat_unc_dn[val]         
         prt_pred_tot_syst_up += prt_syst_unc_up[val]*prt_syst_unc_up[val]         
         prt_pred_tot_syst_dn += prt_syst_unc_dn[val]*prt_syst_unc_dn[val]
      prt_pred_tot_stat_up = math.sqrt(prt_pred_tot_stat_up)
      prt_pred_tot_stat_dn = math.sqrt(prt_pred_tot_stat_dn)
      prt_pred_tot_syst_up = math.sqrt(prt_pred_tot_syst_up)
      prt_pred_tot_syst_dn = math.sqrt(prt_pred_tot_syst_dn)

      prt_table_file.write('{:d}  {:s} {:.4f}  {:.4f} {:.4f} {:.4f} {:.4f} {:.4f}'.format(chn-1,  str(data_rate), math.sqrt(float(data_rate)),  prt_pred_tot_rate, prt_pred_tot_stat_up, prt_pred_tot_stat_dn, prt_pred_tot_syst_up, prt_pred_tot_syst_dn))
      for key, val in proc_name.items():
         if val == 'signal': continue
         prt_table_file.write('  {:.4f} {:.4f} {:.4f} {:.4f} {:.4f}'.format(pred_rate[val], prt_stat_unc_up[val], prt_stat_unc_dn[val], prt_syst_unc_up[val], prt_syst_unc_dn[val]))
      prt_table_file.write('\n')

   rt_file.Write()
   rt_file.Close()
          
def main():
   usage = "usage: %prog options"
   version = "%prog."
   parser = OptionParser(usage=usage,version=version)
   parser.add_option("-m", "--mu", action="store", dest="ttbarW_mu", type="string", default="comb_mu.txt", help="ttbarW muon channel")
   parser.add_option("-e", "--ele", action="store", dest="ttbarW_ele", type="string", default="comb_ele.txt", help="ttbarW electron channel")
   parser.add_option("-c", "--comb", action="store", dest="ttbarW_comb", type="string", default="comb_comb.txt", help="ttbarW average comb channel")
   parser.add_option("-z", "--zinv", action="store", dest="zinv", type="string", default="zinv.txt", help="set zinv data card name")
   parser.add_option("-q", "--qcd", action="store", dest="qcd", type="string", default="qcd.txt", help="set qcd data card name")
   parser.add_option("-r", "--ttz", action="store", dest="ttz", type="string", default="ttz.txt", help="set ttz data card name")
   parser.add_option("-b", "--rare", action="store", dest="rare", type="string", default="rare.txt", help="set rare data card name")
   parser.add_option("-d", "--data", action="store", dest="data", type="string", default="data.txt", help="set data data card name")
   parser.add_option("-s", "--signal", action="store", dest="signal", type="string", default="signal.txt", help="set signal data card name")
   parser.add_option("-o", "--outputdir", action="store", dest="outputdir", type="string", default="", help="set combined card output directory")
   
   (options, args) = parser.parse_args()
   
   print 'ttbarW_comb :', options.ttbarW_comb
   print 'ttbarW_mu :', options.ttbarW_mu
   print 'ttbarW_ele :', options.ttbarW_ele
   print 'zinv :', options.zinv
   print 'qcd :', options.qcd
   print 'ttz :', options.ttz
   print 'rare :', options.rare
   print 'data :', options.data
   print 'signal : ', options.signal
   print 'outputdir : ', options.outputdir

   if len(options.outputdir) !=0:
      if not os.path.exists(options.outputdir): os.mkdir(options.outputdir) 

   splitsignalinput = options.signal.split("/")
   stripDirSignalInput = splitsignalinput[-1]

   if "signal_" in stripDirSignalInput: tmp_signal_key = stripDirSignalInput.replace("signal_", "")
   elif "signal" in stripDirSignalInput: tmp_signal_key = stripDirSignalInput.replace("signal", "")
   tmp_signal_key = tmp_signal_key.replace(".txt", "")
   
   prodCardPerChn(tmp_signal_key, options.outputdir, options.ttbarW_comb, options.ttbarW_mu, options.ttbarW_ele, options.zinv, options.qcd, options.ttz, options.rare, options.data, options.signal)

if __name__ == "__main__":
   main()
