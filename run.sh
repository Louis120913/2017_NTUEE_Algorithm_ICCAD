#!/bin/bash

binFile=./DPT_balance_color_dbg
inputDir=inputs/
outputDir=outputs/

for i in 1 2 3 4 5
do
    inputFile="${inputDir}iccad2015_input.case${i}"
    outputFile="${outputDir}case${i}.txt"
    echo "Doing ${inputFile} ..."
    ${binFile} ${inputFile} ${outputFile}
    python evaluate.py ${outputFile}
done
