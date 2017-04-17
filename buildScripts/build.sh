#!/bin/bash
systemArchitecture=$(uname -m) 

echo "Building!"

#temp for jenkins
sudo apt-get install libcppunit-dev

cd ../ext/

if [ "$systemArchitecture" = "x86_64" ]; then
 echo "64 bit system, using 64 bit makefile..."
 make -f makefile64bit 
else
 echo "32 bit system, using 32 bit makefile.."
 make -f makefile32bit 
fi

