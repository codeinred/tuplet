#!/usr/bin/env zsh

cd "$(dirname $0)"

mkdir -p coverage
cd coverage

gcov -o ../build/CMakeFiles/**/*.o ../test2/all.cpp -m

lcov -c -d ../build/CMakeFiles/ -o name.info

genhtml name.info
