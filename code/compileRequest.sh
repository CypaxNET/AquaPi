#!/bin/bash
echo ''
echo '-----------------------------------'
echo 'Compiling aquaRequest'
sudo pkill aquaRequest
cd /home/pi/AquaPi/code/aquaRequest
qmake
make
