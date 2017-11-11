#!/bin/bash

ADV_DIR='30_1adv_k15'

echo -e "IBM ILOG is working..."
for f in $ADV_DIR/s*.dat
do
	echo "Run IMB ILOG for scenario: $f..."
	oplrun src.mod $f > $f.log
done
	
