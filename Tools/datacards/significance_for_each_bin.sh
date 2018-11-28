set i = 1
set last_bin = 239
while($i <= $last_bin)
	#combineCards.py comb__ch${i}.txt comb_ttbarW_eleCS__ch${i}.txt comb_ttbarW_muCS__ch${i}.txt > search_bin_${i}.txt
	#combine -M Significance search_bin_${i}.txt -t -1 --expectSignal=1 > expected_significance_ch${i}.lg
	#combine -M Significance search_bin_${i}.txt > observed_significance_ch${i}.lg
	combine -M Asymptotic search_bin_${i}.txt > limit_ch${i}.lg
	#echo "ch${i} `grep 'Significance:' observed_significance_ch${i}.lg | cut -d : -f 2`"
	echo "ch${i} `grep 'Observed Limit:' limit_ch${i}.lg | cut -d \< -f 2`"
	@ i++
	wait
end
