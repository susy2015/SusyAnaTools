rm -f datacards/signal_temp/*

#foreach mp ("`cat T2tt_mass_points.txt`")
foreach mp ("`cat T1tttt_mass_points.txt`")
	set mother = "`echo $mp | cut -d _ -f 1`"
	set daughter = "`echo $mp | cut -d _ -f 2`"
	#./make_signalcards $mother $daughter Signal_fastsim_T2tt_mStop${mother}_mLSP${daughter} search_bin_low_and_highdm_h 
	#./make_signalcards $mother $daughter Signal_fastsim_T2tt_mStop${mother}_mLSP${daughter} search_bin_more_HT_bins_h
	#./make_signalcards $mother $daughter Signal_fastsim_T2tt_mStop${mother}_mLSP${daughter} search_bin_more_HT_bins_merge_h
	#./make_signalcards $mother $daughter Signal_fastsim_T1tttt_mGluino${mother}_mLSP${daughter} search_bin_low_and_highdm_h 
	./make_signalcards $mother $daughter Signal_fastsim_T1tttt_mGluino${mother}_mLSP${daughter} search_bin_more_HT_bins_h 
	#./make_signalcards $mother $daughter Signal_fastsim_T1tttt_mGluino${mother}_mLSP${daughter} search_bin_more_HT_bins_merge_h 
end
