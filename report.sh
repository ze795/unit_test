lcov --rc lcov_branch_coverage=1 -c -d ./build  -b src/ -o ./report/main.info
genhtml --legend --branch-coverage ./report/main.info -o ./report/main_report
