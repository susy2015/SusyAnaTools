rm -f datacards/signal_temp/*

#foreach mp ("`cat T2tt_mStop1200to2000.txt`")
foreach mp ("`cat T2tt_mass_points.txt`")
#foreach mp ("`cat T1tttt_mass_points.txt`")
#foreach mp ("`cat T2fbd_mass_points.txt`")
	set mother = "`echo $mp | cut -d _ -f 1`"
	set daughter = "`echo $mp | cut -d _ -f 2`"
	#../make_signalcards $mother $daughter SMS_T2tt_mStop${mother}_mLSP${daughter}_fastsim_2016_0 search_bin_v2_highdm_h 
	../make_signalcards $mother $daughter SMS_T2tt_mStop${mother}_mLSP${daughter}_fastsim search_bin_v4_h search_bin_v4_singleMuCR_h search_bin_v4_singleElCR_h 53 
	#../make_signalcards $mother $daughter SMS_T1tttt_mGluino${mother}_mLSP${daughter}_fastsim search_bin_v4_h search_bin_v4_singleMuCR_h search_bin_v4_singleElCR_h 53
	#../make_signalcards $mother $daughter SMS_T2fbd_mStop${mother}_mLSP${daughter}_fastsim_2016_0 search_bin_v2_lowdm_h 999 
	#../make_signalcards $mother $daughter SMS_T2fbd_mStop${mother}_mLSP${daughter}_fastsim_2016_0 search_bin_v2_lowdm_more_MET_h 999 
end
