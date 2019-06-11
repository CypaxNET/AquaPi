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
DEFAULT_LAT = "48,00,0"
DEFAULT_LON = "7,51,0"

# Kerala, India
#DEFAULT_LAT = "22,51,0"
#DEFAULT_LON = "76,38,20.5"

# Faro, Spain
#DEFAULT_LAT = "37,1,31"
#DEFAULT_LON = "-7,56,45"


# CONSTANTS ###################################################################
# The following elements must not be altered during script execution
scriptVersion = '1.0.0.2'

def main():
  global scriptVersion

  doDebugPrint = False
  
  mooninfoDict = dict()

  logging.basicConfig(level=logging.DEBUG, format='  %(levelname)s: %(message)s')
  logging.info('Moon Info Script')
  logging.info('Version ' + scriptVersion)
  mooninfoDict["version"] = scriptVersion

  # construct the argument parser and parse the arguments
  thisArgumentParser = argparse.ArgumentParser()
  thisArgumentParser.add_argument("-n", "--lat",   required = False,    help = "Latitude",  default=DEFAULT_LAT)
  thisArgumentParser.add_argument("-e", "--lon",   required = False,    help = "Longitude", default=DEFAULT_LON)
  thisArgumentParser.add_argument("-d", "--debug", action='store_true')
  args = vars(thisArgumentParser.parse_args())
  doDebugPrint = args["debug"]
  latStr = args["lat"]
  lonStr = args["lon"]
  
  lat = tuple([float(x) for x in latStr.split(',')])
  lon = tuple([float(x) for x in lonStr.split(',')])

  now = datetime.datetime.utcnow()

  if doDebugPrint:
    print("Now: " + str(now) + " UTC")
  mooninfoDict["datetime"] = str(now)

  mi = pylunar.MoonInfo(lat, lon)
  mi.update((now.year, now.month, now.day, now.hour, now.minute, now.second))
  mooninfoDict["fraction"] = str(mi.fractional_phase())
  mooninfoDict["altitude"] = str(mi.altitude())
  mooninfoDict["azimuth"] = str(mi.azimuth())
  if doDebugPrint:
    print("Fractional phase: %.4f" % mi.fractional_phase())
    print("Altitude: %.4f°" % mi.altitude())
    print("Azimuth %.4f°" % mi.azimuth())
  
  moon_rise_time = now.replace(hour=0,minute=0,second=0,microsecond=0)
  moon_set_time = now.replace(hour=23,minute=59,second=59,microsecond=999999)
  
  rise_set_times = mi.rise_set_times(time.tzname[time.daylight])
  for timeInfo in rise_set_times:
    timeID = timeInfo[0]
    if ("rise" == timeID):
      moon_rise_time = timeInfo[1]
    elif ("set" == timeID):
      moon_set_time = timeInfo[1]

  if (type(moon_rise_time) is str):
    if doDebugPrint:
      print("Moon does not rise today")
    mooninfoDict["doesrise"] = "False"
  else:
    moon_rise_time = now.replace(hour=int(moon_rise_time[3]),minute=int(moon_rise_time[4]),second=int(moon_rise_time[5]),microsecond=0)
    mooninfoDict["doesrise"] = "True"
  
  if (type(moon_set_time) is str):
    if doDebugPrint:
      print("Moon does not set today")
    mooninfoDict["doesset"] = "False"
  else:
    moon_set_time = now.replace(hour=int(moon_set_time[3]),minute=int(moon_set_time[4]),second=int(moon_set_time[5]),microsecond=0)
    mooninfoDict["doesset"] = "True"
  
  if doDebugPrint:
    print("Rise time: " + str(moon_rise_time) + " UTC")
    print("Set time:  " + str(moon_set_time) + " UTC")

  mooninfoDict["rise"] = str(moon_rise_time)
  mooninfoDict["set"] = str(moon_set_time)

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
    mooninfoDict["shining"] = "True"
    if doDebugPrint:
      print("Moon is shining (%.4f%%)" % (100.0 * moonIlluminationFactor * moonAltitudeFactor))
  else:
    mooninfoDict["shining"] = "False"
    mooninfoDict["value"] = "0.0"
    if doDebugPrint:
      print("Moon is NOT shining")

  if doDebugPrint:
    for keystr in mooninfoDict:
      print(keystr + " = " + str(mooninfoDict[keystr]))

  TCP_IP = 'localhost'
  TCP_PORT = 58237
  BUFFER_SIZE = 1000
  MESSAGE = str(mooninfoDict)

  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  s.connect((TCP_IP, TCP_PORT))
  s.send(MESSAGE.encode())
  s.close()

  return 0
  
if __name__ == "__main__":
  sys.exit(main())