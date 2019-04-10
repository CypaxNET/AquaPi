#!/bin/bash
echo ''
echo '-----------------------------------'
echo 'Updating repository'
cd /home/pi/AquaPi
git pull

echo ''
echo '-----------------------------------'
echo 'Compiling aquaBackend'
sudo pkill aquaBackend
cd /home/pi/AquaPi/code/aquaBackend
qmake
make