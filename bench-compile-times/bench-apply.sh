#!/usr/bin/zsh

if [ -n "$CXX" ]; then
    compiler="$CXX"
else
    compiler="g++"
fi

cpu_model="$(grep -m 1 "model name" /proc/cpuinfo)"
compiler_version="$($compiler --version | head -n 1)"

echo "Using $compiler aka $compiler_version"

SKIP=$1
MAX=$2

echo -e "$cpu_model" "\n" "$compiler_version" "\n" "---" >data/tuplet-apply-times.csv
echo -e "$cpu_model" "\n" "$compiler_version" "\n" "---" >data/std-apply-times.csv

# Create print_ans.o
$compiler -c print_ans.cpp

for n in {$SKIP..$MAX..$SKIP}; do
    # Create a list of n values from 1..n
    values="$(echo {1..$n},)"
    # This indicates that GNU time should be used, rather than the built-in shell time command
    echo -n "tuplet::tuple: "
    \time -f "$n, %e, %M" $compiler apply.cpp print_ans.o \
        -std=c++20 \
        -I../include \
        -DVALUES="$values" 2>&1 | tee -a data/tuplet-apply-times.csv

    echo -n "std::tuple:    "
    \time -f "$n, %e, %M" $compiler apply.cpp print_ans.o \
        -std=c++20 \
        -DVALUES="$values" \
        -DUSE_STD_TUPLE 2>&1 | tee -a data/std-apply-times.csv
done
