foreach mp ("`cat T2tt_mass_points_test.txt`")
	set mother = "`echo $mp | cut -d _ -f 1`"
	set daughter = "`echo $mp | cut -d _ -f 2`"
	./make_signalcards $mother $daughter Signal_fastsim_T2tt_mStop${mother}_mLSP${daughter} search_bin_highdm_h 
end
