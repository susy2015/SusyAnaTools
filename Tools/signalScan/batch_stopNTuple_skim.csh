foreach mp ("`cat T2tt_mass_points_test.txt`")
	set mother = "`echo $mp | cut -d _ -f 1`"
	set daughter = "`echo $mp | cut -d _ -f 2`"
	#set fileList = SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt
	set fileList = SMS-T1tttt_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt
	echo Arguments = $mother $daughter $fileList
	echo Queue
	#../stopNTuple_skim $mother $daughter ../FileList/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt /eos/uscms/store/user/lpcsusyhad/Stop_production/SoftBjet_PhotonNtuples/T2tt_mStop-400to1200/Signal_fastsim_T2tt_mStop${mother}_mLSP${daughter}.root > stopNTuple_skim_${mother}_${daughter}.log &
	#sleep 10m
	#./stopNTuple_skim $mother $daughter FileList/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt Signal_fastsim_T2tt_mStop${mother}_mLSP${daughter}.root > stopNTuple_skim_${mother}_${daughter}.log &
	#wait
end
