"""
A script to check what stations are contained within basin outlines.

First argument is the station file and the subsequent arguments are N basin file outlines.
"""

import argparse
import os

import matplotlib.path as mpltPath
import numpy as np

from qcore import formats

parser = argparse.ArgumentParser()
parser.add_argument("station_file")
parser.add_argument("outline", nargs="+")

args = parser.parse_args()

stat_file = formats.load_station_file(args.station_file)

paths = []
for outline_fp in args.outline:
    outline = np.loadtxt(outline_fp)

    path = mpltPath.Path(outline)

    for stat in stat_file.iterrows():
        if path.contains_point((stat[1].lon, stat[1].lat)):
            print(f"station: {stat[0], stat[1].lon, stat[1].lat} is contained in the basin {os.path.basename(outline_fp)}")