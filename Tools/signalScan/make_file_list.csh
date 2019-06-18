#foreach mp ("`cat T1tttt_mass_points.txt`")
foreach mp ("`cat T2tt_mStop-400to1200.txt`")
	set mother = "`echo $mp | cut -d _ -f 1`"
	set daughter = "`echo $mp | cut -d _ -f 2`"
	set eosDir = "/eos/uscms/store/user/lpcsusyhad/Stop_production/Summer16_94X_v3/PostProcessed_11Apr2019_v2p7_fastsimv1/SMS_T2tt_mStop_400to1200_fastsim_2016"
	#echo root://cmseos.fnal.gov/`ls ${eosDir}T1tttt/Signal_fastsim_${mp}.root` > Signal_fastsim_T1tttt_mGluino${mother}_mLSP${daughter}.list
	echo root://cmseos.fnal.gov/`ls ${eosDir}/SMS_T2tt_mStop_400to1200_fastsim_2016_*_Skim_Mom${mother}_LSP${daughter}.root` > Signal_fastsim_T2tt_mStop${mother}_mLSP${daughter}.list
	#echo "Signal_fastsim_T1tttt_mGluino${mother}_mLSP${daughter}"    : \['',1\],
	#echo Arguments = $mother $daughter $fileList
end
