#!/bin/csh

echo "1. Create fileB"
./sparse fileA fileB

echo "2. gzip fileA & fileB"
gzip -k fileA
gzip -k fileB

echo "3. Unpack fileB to sparse"
gzip -cd fileB.gz | ./sparse fileC

echo "4. Create fileD"
./sparse -b 100 fileC fileD

echo "5. Stat files"
stat -L file*
