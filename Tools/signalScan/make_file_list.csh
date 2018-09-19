foreach mp ("`cat T1tttt_mass_points.txt`")
	set mother = "`echo $mp | cut -d _ -f 1`"
	set daughter = "`echo $mp | cut -d _ -f 2`"
	set eosDir = "/eos/uscms/store/user/lpcsusyhad/Stop_production/SoftBjet_PhotonNtuples/"
	echo root://cmseos.fnal.gov/`ls ${eosDir}T1tttt/Signal_fastsim_${mp}.root` > Signal_fastsim_T1tttt_mGluino${mother}_mLSP${daughter}.list
	echo "Signal_fastsim_T1tttt_mGluino${mother}_mLSP${daughter}"    : \['',1\],
	#echo Arguments = $mother $daughter $fileList
end
