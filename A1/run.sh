#!/bin/bash

echo "**** intialize"

rm main.dat
rm test.dat

echo "**** compile code"

cc prototype.c -o prototype

echo "**** execute test - 5 to 10 minutes "

./prototype

echo "**** graph the data "

gnuplot plot1
gnuplot plot2
gnuplot plot3

echo "**** complete "
