#!/bin/bash

for ((i=0;i<100000;i++))
do
nohup ../cmake-build-debug/clientDemo > $i.txt 2>&1 &
done



