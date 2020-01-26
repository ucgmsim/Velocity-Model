#!/usr/env/python3
#   Author: Alex Hobson
#   Date: 22/01/2020
#
#   Application to generate cross section diagrams of New Zealand Velocity Model (NZVM)
#

import argparse
import math
import os

import numpy as np
from matplotlib import pyplot as plt
import matplotlib.ticker as ticker

KM_TO_M = 1000

def load_data(file, nx, ny, nz):
    """
    Loads the velocity model into memory as a 3d numpy array
    Params:
        - file: file path to velocity model
        - nx: number of samples in x axis
        - ny: number of samples in y axis
        - nz: number of samples in z axis
    """
    array = np.fromfile(file, dtype=np.float32, count=-1, sep='', offset=0)
    length = len(array)
    array = array.reshape((ny, nz, nx))
    print("Loaded",length,"data points")
    return (length, array)


def display_data_matplot(data, num_x, num_y, max_color, hh):
    """
    Prepares a figure in matplotlib related to the NZVM to be displayed
    Params:
        - data: numpy 2d array of data to plot
        - num_x: number of samples in x axis (of data, not of VM)
        - num_y: number of samples in y axis (of data, not of VM)
        - max_color: maximum velocity (m/s) the chart should go up to (-1 for auto)
        - hh: cell size (resolution) of VM in m
    """
    fig = plt.figure(1)
    
    plot = plt.matshow(data, 1, interpolation='none')

    plot.set_cmap("jet")

    if hh != 0:
        plt.ylabel("Depth (metres below surface)")
        ticks_y = ticker.FuncFormatter(lambda y, pos: '{:.0f}'.format(y*hh))
        plot.axes.yaxis.set_major_formatter(ticks_y)

        plt.xlabel("Horizontal distance (metres from origin)")
        ticks_x = ticker.FuncFormatter(lambda x, pos: '{:.0f}'.format(x*hh))
        plot.axes.xaxis.set_major_formatter(ticks_x)
    else:
        plt.ylabel("Depth (samples below surface)")
        plt.xlabel("Horizontal distance (no. samples)")

    

    if max_color != -1:
        plt.clim(0, max_color)
    else:
        plt.clim(0, int(math.ceil(np.amax(data)/1000.0)*1000.0)) #round up to nearest 1000
    
    legend = plt.colorbar()
    legend.ax.get_yaxis().labelpad = 15
    legend.ax.set_ylabel("Wave velocity (m/sec)", rotation=270)

def main():
    """
    Main method of the program
    """
    parser = argparse.ArgumentParser(description='Generate a cross section of the NZVM.')

    parser.add_argument("vm", help="file path to the velocity model")
    parser.add_argument("nx", help="data number of samples in x direction", type=int)
    parser.add_argument("ny", help="data number of samples in x direction", type=int)
    parser.add_argument("nz", help="data number of samples in x direction", type=int)
    parser.add_argument("-hh", "--hh", type=int, default=0, \
            help="The resolution of the data (in metres). e.g. 400 means each change in nx, ny, nz is 400m")
    parser.add_argument("-x", "--origin_x", help="x axis offset from model where plot should begin (default 0)", type=int, default=0)
    parser.add_argument("-y", "--origin_y", help="y axis offset from model where plot should begin (default 0)", type=int, default=0)
    parser.add_argument("axis", help="which axis to plot along (x (west to east) or y (north to south)) (default x)")
    parser.add_argument("delta", help="how many samples along the specified axis the plot should go for", type=int)
    parser.add_argument("-max", "--maximum", type=int, default=2500, \
            help="The upper bound for the color scale (in metres/second). Anything faster than this would appear as maximum colour. Use -1 for automatic calculation of maximum (default 2500)")
    parser.add_argument("-o", "--output", type=str, default="", \
            help="If specified, the output file the plot would be saved to, otherwise opens the plot in a new window")
    
    args = parser.parse_args()

    if not os.path.exists(args.vm):
        print("Error: VM file does not exist")
        quit()

    nx = args.nx
    ny = args.ny
    nz = args.nz
    hh = args.hh

    data = []
    
    start_point = (args.origin_x, args.origin_y)

    axis = args.axis

    if axis != 'x' and axis != 'y':
        axis = 'x'

    if start_point[0] >= nx or start_point[0] < 0 or start_point[1] >= ny or start_point[1] < 0:
        print("Error: Origin extends beyond boundaries of given data file")
        quit()

    max_delta = ny - start_point[1] - 1
    if axis == 'x':
        max_delta = nx - start_point[0]
    
    delta = args.delta

    if delta <= 0 or delta > max_delta:
        print("Error: delta extends beyond boundaries of given data file")
        quit()

    length, array = load_data(args.vm, nx, ny, nz)

    if length != nx*ny*nz:
        print("Error: Inconsistent VM size. Check model and nx, ny, nz parameters and try again. Expected",nx*ny*nz,"values, got",len(array))
        quit()
    
    if axis == 'x':
        data = array[start_point[1],:,start_point[0]:(start_point[0]+delta+1)] * KM_TO_M
    else:
        data = array[start_point[1]:(start_point[1]+delta+1),:,start_point[0]] * KM_TO_M
        data = np.swapaxes(data,0,1)
        
    display_data_matplot(data, int(len(data)/nz), nz, args.maximum, hh)

    if len(args.output) > 0:
        plt.savefig(args.output)
    else:
        plt.show()

if __name__ == "__main__":
    main()
