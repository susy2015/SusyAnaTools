import glob

folder_name = "SMS_T2tt_mStop_350to400_fastsim_2017"
folder_path = "/eos/uscms/store/user/lpcsusyhad/Stop_production/Fall17_94X_v2_NanAOD_MC/PostProcessed_18Apr2019_v2p7p1_fastsimv2/"
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
	mother_mass = item.split("_")[0]
	LSP_mass = item.split("_")[1]
	target_name = "SMS_T2tt_mStop" + mother_mass + "_mLSP" + LSP_mass + "_fastsim_2017.root"
	#source_name = folder_name + "_*_Skim_Mom" + mother_mass + "_LSP" + LSP_mass + ".root"
	source_name = "Skim_Mom" + mother_mass + "_LSP" + LSP_mass
	#print "python haddnano.py " + folder_path + folder_name + "/" + target_name + " " + folder_path + folder_name + "/" + source_name
	print target_name, folder_path.split("/eos/uscms")[1], source_name
