#!/bin/bash
echo ''
echo '-----------------------------------------'
echo 'OVERWRITING CGI'
cd ~/AquaPi/code/aquaRequest
sudo cp aquaRequest.cgi /usr/lib/cgi-bin/aquaRequest.cgi
sudo chown www-data:www-data /usr/lib/cgi-bin/aquaRequest.cgi
./deployHTTP.sh
