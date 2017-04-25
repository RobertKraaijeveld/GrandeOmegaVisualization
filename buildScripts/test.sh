#!/bin/bash
echo "Testing!"

cd ../lib/

rm Buildfiles/* 

make -f testmakefile

mv Tests Buildfiles/
cd Buildfiles/
./Tests

echo "Running CppCheck statistical analysis..."
cd ..
cppcheck DataProcesser/ --enable=all
