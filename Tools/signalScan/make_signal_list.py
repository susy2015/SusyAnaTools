import glob
import re

#folder_name = "SMS_T2tt_mStop_1200to2000_fastsim_2016"
#folder_name = "SMS_T2fbd_fastsim_2016"
folder_name = "SMS_T1tttt_fastsim_2016"
folder_path = "/eos/uscms/store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/PostProcessed_11Apr2019_v2p7_fastsimv1/"
full_list = glob.glob(folder_path + folder_name + '/*.root')
mass_point_list = []
for file_name in full_list:
	last_name = file_name.split("Mom")[1]
	mother_mass = last_name.split("_")[0]
	LSP_mass = last_name.split("LSP")[1].split(".")[0]
	mass_point = mother_mass + "_" + LSP_mass
	if mass_point not in mass_point_list: mass_point_list.append(mass_point)
mass_point_list.sort()
for item in mass_point_list:
	print item

'''
cfg_file = open(folder_name + ".cfg","w+")
out_file_list = []
for item in mass_point_list:
    #if item == "500_490":
	print item
	mother_mass = item.split("_")[0]
	LSP_mass = item.split("_")[1]
	#example name: SMS_T2tt_mStop_400to1200_fastsim_2016_8_Skim_Mom1000_LSP300.root
	reg_name = ".*" + folder_name + "_.*_Skim_Mom" + mother_mass + "_LSP" + LSP_mass + ".root"
	#reg_name = ".*" + "SMS_T2tt_mStop_1200to2000_fastSim_2016" + "_.*_Skim_Mom" + mother_mass + "_LSP" + LSP_mass + ".root"
	out_name = "SMS_T1tttt_mGluino" + mother_mass + "_mLSP" + LSP_mass + "_fastsim"
	out_file_list.append(out_name)
	out_name = out_name + "_2016"
	cfg_file.write(out_name + ", " + folder_path + folder_name + ", " + out_name + ".txt, Events, 1.0, 1000000, 0, 1.0\n")
	#out_file = open(out_name + ".txt","w+")
	out_file = open(folder_path + folder_name + "/" + out_name + ".txt","w+")
	for file_name in full_list:
		#print "reg_name", reg_name
		#print "file_name", file_name
		if re.match(reg_name, file_name) != None: out_file.write("root://cmseos.fnal.gov/" + file_name + "\n")
	out_file.close()

cfg_file.close()
print out_file_list
'''
