#!/bin/bash
echo ''
echo '-----------------------------------'
echo 'Compiling aquaBackend'
sudo pkill aquaBackend
cd /home/pi/AquaPi/code/aquaBackend
qmake
make
