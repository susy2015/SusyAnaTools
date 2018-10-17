../new_combCardPerChannel.py
echo "
combineCards.py comb__ch*.txt comb_ttbarW_eleCS__ch*.txt comb_ttbarW_muCS__ch*.txt > comb_all_ch.txt
combine -M Asymptotic comb_all_ch.txt > log_all_ch.lg &
combine -M Significance comb_all_ch.txt -t -1 --expectSignal=1 > expected_significance.lg &
source ../print_limit_and_significance.sh

source ../significance_for_each_bin.sh
grep \"Significance:\" expected_significance_ch*.lg
"
