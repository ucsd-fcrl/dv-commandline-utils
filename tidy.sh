#!/usr/bin/env bash

FILES=$(find . -name "*.hxx" -o -name "*.h" -o -name "*.cxx")

for f in ${FILES[*]};
do

clang-format -style=Mozilla -i ${f}

done
