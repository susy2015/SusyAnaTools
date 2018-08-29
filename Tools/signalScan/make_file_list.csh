foreach mp ("`cat T2tt_mass_points.txt`")
	set mother = "`echo $mp | cut -d _ -f 1`"
	set daughter = "`echo $mp | cut -d _ -f 2`"
	set eosDir = "/eos/uscms/store/user/lpcsusyhad/Stop_production/SoftBjet_PhotonNtuples/"
	echo root://cmseos.fnal.gov/`ls ${eosDir}T2tt/Signal_fastsim_${mp}.root` > Signal_fastsim_T2tt_mStop${mother}_mLSP${daughter}.list
	#echo Arguments = $mother $daughter $fileList
end
