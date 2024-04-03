export LCOV="lcov --rc lcov_branch_coverage=1"
export WEB_BROWSER="firefox"

mkdir -p report
$LCOV -c -d ./build  -b src/ -o ./report/main.info
$LCOV --remove ./report/main.info "*/googletest/*" -o ./report/main.info
$LCOV --remove ./report/main.info "*/ut_src/*" -o ./report/main.info
$LCOV --remove ./report/main.info "*/stub/*" -o ./report/main.info
$LCOV --remove ./report/main.info "*/report/*" -o ./report/main.info
$LCOV --remove ./report/main.info "*/build/*" -o ./report/main.info

genhtml --legend --branch-coverage ./report/main.info -o ./report/main_report
$WEB_BROWSER ./report/main_report/index.html
