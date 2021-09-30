#!/usr/bin/zsh

# we should exit on error
set -e

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

mkdir -p data
mkdir -p build
echo -e "$cpu_model" "\n" "$compiler_version" "\n" "---" >data/tuplet-apply-times.csv
echo -e "$cpu_model" "\n" "$compiler_version" "\n" "---" >data/std-apply-times.csv

# Create print_ans.o
$compiler -c print_ans.cpp -o build/print_ans.o

for n in {$SKIP..$MAX..$SKIP}; do
    # Create a list of n values from 1..n
    values="$(echo {1..$n},)"
    # This indicates that GNU time should be used, rather than the built-in shell time command
    echo -n "tuplet::tuple: "
    \time -f "$n, %e, %M" $compiler apply.cpp build/print_ans.o \
        -std=c++20 \
        -o build/tuplet-apply \
        -I../include \
        -DVALUES="$values" 2>&1 | tee -a data/tuplet-apply-times.csv

    echo -n "std::tuple:    "
    \time -f "$n, %e, %M" $compiler apply.cpp build/print_ans.o \
        -std=c++20 \
        -o build/std-apply \
        -DVALUES="$values" \
        -DUSE_STD_TUPLE 2>&1 | tee -a data/std-apply-times.csv

    # Here, we check to make sure that the program actually produces the correct
    # Output. This should be the sum of the numbers from 1..n.
    # Calculate the expected sum:
    sum="$(( ($n * $n + $n) / 2 ))"
    if [ "$(build/tuplet-apply)" != "$sum" ]; then
        echo ERROR: tuplet-apply broken.
        break
    fi
    if [ "$(build/std-apply)" != "$sum" ]; then
        echo ERROR: std-apply broken.
        break
    fi
done
