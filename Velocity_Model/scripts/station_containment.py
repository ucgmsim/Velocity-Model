"""
A script to check what stations are contained within basin outlines.

First argument is the station file and the subsequent arguments are N basin file outlines.
"""

import argparse
from pathlib import Path

import matplotlib.path as mpltPath
import numpy as np

from qcore import formats

parser = argparse.ArgumentParser()
parser.add_argument("station_file")
parser.add_argument("outline", nargs="+", type=Path)

args = parser.parse_args()

stat_file = formats.load_station_file(args.station_file)

paths = []
for outline_fp in args.outline:
    outline = np.loadtxt(outline_fp)

    path = mpltPath.Path(outline)

    for stat in stat_file.iterrows():
        if path.contains_point((stat[1].lon, stat[1].lat)):
            print(stat[1].lon, stat[1].lat, stat[0], True, outline_fp.name)
        else:
            print(stat[1].lon, stat[1].lat, stat[0], False, None)
