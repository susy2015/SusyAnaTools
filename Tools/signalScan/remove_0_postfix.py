import os
import glob

full_list = glob.glob('*_0.root')
for old_name in full_list:
	temp_name = old_name.split("_0.root")[0]
	new_name = temp_name + ".root"
	os.rename(old_name, new_name)
