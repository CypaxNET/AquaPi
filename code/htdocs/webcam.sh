#!/bin/bash
sudo fswebcam -r 1280x720 -D 5 -S 20 --no-banner /mnt/RAMDisk/webcam.jpg
sudo chown -R www-data:www-data /mnt/RAMDisk/webcam.jpg

