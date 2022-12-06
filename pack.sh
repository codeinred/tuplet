#!/usr/bin/env zsh

cat include/tuplet/tuple.hpp include/tuplet/format.hpp test/util/*.hpp test/*.cpp \
    | sed 's/#include <tuplet\/tuple.hpp>//g' \
    | sed 's/#include "tuplet\/tuple.hpp"//g' \
    | sed 's/#include <tuplet\/format.hpp>//g' \
    | sed 's/#include "tuplet\/format.hpp"//g' \
    | sed 's/#include "util\/printing.hpp"//g' \
    | sed 's/#include "util\/reflection.hpp"//g' > all.cpp
