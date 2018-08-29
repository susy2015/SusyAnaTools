echo "Asymptotic Limits (CLs):"
grep "Observed Limit" log_all_ch.lg
grep "Expected 16.0%" log_all_ch.lg
grep "Expected 50.0%" log_all_ch.lg
grep "Expected 84.0%" log_all_ch.lg
echo "\nExpected significance:"
grep "Significance:" expected_significance.lg
