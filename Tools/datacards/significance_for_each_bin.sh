set i = 1
set last_bin = 124
while($i <= $last_bin)
	combineCards.py comb__ch${i}.txt comb_ttbarW_eleCS__ch${i}.txt comb_ttbarW_muCS__ch${i}.txt > search_bin_${i}.txt
	combine -M Significance search_bin_${i}.txt -t -1 --expectSignal=1 > expected_significance_ch${i}.lg &
	@ i++
	wait
end
