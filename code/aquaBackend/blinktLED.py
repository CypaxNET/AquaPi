#!/usr/bin/env python
# coding=iso-8859-15

# IMPORTS #####################################################################
import sys
import types
import math
import colorsys
import time
import blinkt
import argparse               # required to parse commands
import logging

# CONSTANTS ###################################################################
# The following elements must not be altered during script execution
scriptVersion = '1.0.0.3'

def main():
  global scriptVersion

  logging.basicConfig(level=logging.DEBUG, format='  %(levelname)s: %(message)s')
  logging.info('Blinkt! HSV control')
  logging.info('Version ' + scriptVersion)

  #blinkt.set_clear_on_exit()
  blinkt.set_brightness(0.1)
  
  # construct the argument parser and parse the arguments
  thisArgumentParser = argparse.ArgumentParser()
  thisArgumentParser.add_argument("-hue", "--hue", required = False, help = "Hue",        default="0.55")
  thisArgumentParser.add_argument("-sat", "--sat", required = False, help = "Saturation", default="1.0")
  thisArgumentParser.add_argument("-val", "--val", required = False, help = "Value",      default="1.0")
  args = vars(thisArgumentParser.parse_args())

  HUE = args["hue"]
  SAT = args["sat"]
  VOL = args["val"]

  r, g, b = [int(c * 255.0) for c in colorsys.hsv_to_rgb(HUE, SAT, VOL)]
  for x in range(blinkt.NUM_PIXELS):
    blinkt.set_pixel(x, r, g, b)

  blinkt.show()
  
  return 0
  
if __name__ == "__main__":
  sys.exit(main())
  

