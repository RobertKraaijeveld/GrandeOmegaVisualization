#!/bin/bash
echo "Testing!"

cd ../lib/

rm Buildfiles/* 

make -f testmakefile

mv Tests Buildfiles/
cd Buildfiles/
./Tests
