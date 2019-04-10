#!/bin/bash
echo ''
echo '-----------------------------------------'
echo 'OVERWRITING WEB APPLICATION'
cd ~/AquarPi/code/htdocs
sudo cp -R jquery/scheduler/* /var/www/jquery/scheduler/
sudo cp -R jquery/* /var/www/jquery/
sudo cp -R js/* /var/www/js/
sudo cp -R css/* /var/www/css/
sudo cp -R images/* /var/www/images/
sudo cp -R ico/* /var/www/ico/
sudo cp schedule*.html /var/www/
sudo cp imageshow.txt /var/www/imageshow.txt
sudo cp index.php /var/www/index.php
sudo cp languages.xml /var/www/languages.xml
sudo chown -R www-data:www-data /var/www/
