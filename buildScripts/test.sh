#!/bin/bash
echo "Testing!"

cd ../ext/

rm Buildfiles/* 

make -f testmakefile

mv Tests Buildfiles/
cd Buildfiles/
./Tests
