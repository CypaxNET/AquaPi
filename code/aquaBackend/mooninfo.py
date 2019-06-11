#!/usr/bin/env python
# coding=iso-8859-15

# IMPORTS #####################################################################
import sys
import types
import datetime
import math
import time
import pylunar
import argparse               # required to parse commands
import logging
import socket

#https://www.timeanddate.de/mond/deutschland/freiburg

#Freiburg, Germany
DEFAULT_LAT = "47,59,41.38"
DEFAULT_LON = "7,50,59.57"

# Kerala, India
#DEFAULT_LAT = "22,51,0"
#DEFAULT_LON = "76,38,20.5"

# Faro, Spain
#DEFAULT_LAT = "37,1,31"
#DEFAULT_LON = "-7,56,45"


# CONSTANTS ###################################################################
# The following elements must not be altered during script execution
scriptVersion = '1.0.0.0'

def main():
  global scriptVersion

  doDebugPrint = False
  
  mooninfo = dict()

  logging.basicConfig(level=logging.DEBUG, format='  %(levelname)s: %(message)s')
  logging.info('Moon Info Script')
  logging.info('Version ' + scriptVersion)
  mooninfo["version"] = scriptVersion

  # construct the argument parser and parse the arguments
  thisArgumentParser = argparse.ArgumentParser()
  thisArgumentParser.add_argument("-n", "--lat",   required = False,    help = "Latitude",  default=DEFAULT_LAT)
  thisArgumentParser.add_argument("-e", "--lon",   required = False,    help = "Longitude", default=DEFAULT_LON)
  thisArgumentParser.add_argument("-d", "--debug", action='store_true', help = "Debug output")
  args = vars(thisArgumentParser.parse_args())
  doDebugPrint = args["debug"]
  latStr = args["lat"]
  lonStr = args["lon"]
  
  lat = tuple([float(x) for x in latStr.split(',')])
  lon = tuple([float(x) for x in lonStr.split(',')])

  now = datetime.datetime.utcnow()

  if doDebugPrint:
    print("Now: " + str(now) + " UTC")
  mooninfo["datetime"] = str(now)

  mi = pylunar.MoonInfo(lat, lon)
  mi.update((now.year, now.month, now.day, now.hour, now.minute, now.second))
  mooninfo["fraction"] = str(mi.fractional_phase())
  mooninfo["altitude"] = str(mi.fractional_phase())
  mooninfo["azimuth"] = str(mi.fractional_phase())
  if doDebugPrint:
    print("Fractional phase: %.4f" % mi.fractional_phase())
  if doDebugPrint:
    print("Altitude: %.4f°" % mi.altitude())
  if doDebugPrint:
    print("Azimuth %.4f°" % mi.azimuth())
  
  rise_set_times = mi.rise_set_times(time.tzname[time.daylight])
  moon_rise_time = rise_set_times[0][1]
  moon_set_time = rise_set_times[2][1]

  if (type(moon_rise_time) is str):
    if doDebugPrint:
      print("Moon does not rise today")
    moon_rise_time = now.replace(hour=0,minute=0,second=0,microsecond=0)
    mooninfo["doesrise"] = "False"
  else:
    moon_rise_time = now.replace(hour=int(moon_rise_time[3]),minute=int(moon_rise_time[4]),second=int(moon_rise_time[5]),microsecond=0)
    mooninfo["doesrise"] = "True"
  if (type(moon_set_time) is str):
    if doDebugPrint:
      print("Moon does not set today")
    mooninfo["doesset"] = "False"
    moon_set_time = now.replace(hour=23,minute=59,second=59,microsecond=999999)
  else:
    moon_set_time = now.replace(hour=int(moon_set_time[3]),minute=int(moon_set_time[4]),second=int(moon_set_time[5]),microsecond=0)
    mooninfo["doesset"] = "True"
  if doDebugPrint:
    print("Rise time: " + str(moon_rise_time) + " UTC")
  if doDebugPrint:
    print("Set time:  " + str(moon_set_time) + " UTC")
  mooninfo["rise"] = str(moon_rise_time)
  mooninfo["set"] = str(moon_set_time)

  moonIlluminationFactor = mi.fractional_phase()
  moonIlluminationFactor = max(0.0, moonIlluminationFactor)
  moonIlluminationFactor = min(1.0, moonIlluminationFactor)

  moonAltitudeFactor = mi.altitude()
  moonAltitudeFactor = max(0.0, moonAltitudeFactor)
  moonAltitudeFactor = min(90.0, moonAltitudeFactor)
  moonAltitudeFactor = (moonAltitudeFactor / 90.0) * 1.0

  if doDebugPrint:
    print("Altitude factor: %.4f" % moonAltitudeFactor)

  isMoonShine = False

  if (moon_set_time > moon_rise_time):
    if ((now >= moon_rise_time) and (now <= moon_set_time)):
      isMoonShine = True
  else:
    if ((now >= moon_rise_time) or (now <= moon_set_time)):
      isMoonShine = True

  if(isMoonShine):
    mooninfo["shining"] = "True"
    if doDebugPrint:
      print("Moon is shining (%.4f%%)" % (100.0 * moonIlluminationFactor * moonAltitudeFactor))
  else:
    mooninfo["shining"] = "False"
    mooninfo["value"] = "0.0"
    if doDebugPrint:
      print("Moon is NOT shining")

  if doDebugPrint:
    for keystr in mooninfo:
      print(keystr + " = " + str(mooninfo[keystr]))

  TCP_IP = 'localhost'
  TCP_PORT = 58237
  BUFFER_SIZE = 1000
  MESSAGE = str(mooninfo)

  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  s.connect((TCP_IP, TCP_PORT))
  s.send(MESSAGE.encode())
  s.close()

  return 0
  
if __name__ == "__main__":
  sys.exit(main())