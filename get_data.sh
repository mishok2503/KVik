make -j4

for test_num in {1..5}
do
    echo "Test $test_num: "
    time "./benchmarks" "artifacts/res$test_num" 0
    echo ""
done
