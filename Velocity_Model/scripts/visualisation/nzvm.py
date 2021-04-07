#!/usr/env/python3
#   Author: Alex Hobson
#   Date: 22/01/2020
#
#   Application to generate cross section diagrams of New Zealand Velocity Model (NZVM)
#

import argparse
import math
import os

from matplotlib import animation
from matplotlib import pyplot as plt
from matplotlib import ticker
import numpy as np
from qcore import geo

KM_TO_M = 1000

def load_data(file, nx, ny, nz):
    """
    Loads the velocity model into memory, then return a 3d numpy array
    file: file path to velocity model
    nx: number of samples in x axis
    ny: number of samples in y axis
    nz: number of samples in z axis
    """
    array = np.fromfile(file, dtype=np.float32, count=-1, sep='', offset=0)
    length = len(array)
    array = array.reshape((ny, nz, nx))
    return (length, array)


def display_data_matplot(data, max_color, hh, subplot, title, start_lat, start_lon, end_lat, end_lon, axis, rotation):
    """
    Prepares a figure in matplotlib related to the NZVM to be displayed
    data: numpy 2d array of data to plot
    max_color: maximum velocity (m/s) the chart should go up to (-1 for auto)
    hh: cell size (resolution) of VM (metres) (or 0 for no size)
    subplot: matplotlib subplot instance to plot on
    title: title of the graph
    start_lat: latitude of top left point of cross section (or 361 for no latitude)
    start_lon: longitude of top left point of cross section (or 361 for no longitude)
    end_lat: latitude of bottom right point of cross section (or 361 for no latitude)
    end_lon: longitude of bottom right point of cross section (or 361 for no longitude)
    axis: 'x' or 'y' for which axis the cross section is on
    rotation: degrees of rotation the model is, where 0 means the x axis follows the same latitude
    """

    new_subplot = subplot.get_title() != title
    
    if hh == 0:
        if new_subplot:
            subplot.set_ylabel("Depth (samples below surface)")
            subplot.set_xlabel("Horizontal distance (no. samples)")
        plot = subplot.imshow(data, interpolation='nearest', cmap='jet')
    elif not start_lat or not start_lon:
        if new_subplot:
            subplot.set_ylabel("Depth (metres below surface)")
            subplot.set_xlabel("Horizontal distance (metres from origin)")
        plot = subplot.imshow(data, interpolation='nearest', cmap='jet', extent=[0, data.shape[1] * hh, data.shape[0] * hh, 0])
    else:
        if new_subplot:
            subplot.set_ylabel("Depth (metres below surface)")
            subplot.get_xaxis().set_major_locator(ticker.MaxNLocator(nbins=6))
            subplot.get_xaxis().get_major_formatter().set_useOffset(False)
            
        if (axis == 'x' and rotation <= 45) or (axis == 'y' and rotation > 45):
            if new_subplot:
                subplot.set_xlabel("Longitude")
            plot = subplot.imshow(data, interpolation='nearest', cmap='jet', extent=[start_lon, end_lon, data.shape[0] * hh, 0], aspect='auto')
        else:
            if new_subplot:
                subplot.set_xlabel("Latitude")
            plot = subplot.imshow(data, interpolation='nearest', cmap='jet', extent=[start_lat, end_lat, data.shape[0] * hh, 0], aspect='auto')
    
    if max_color != -1:
        plot.set_clim(0, max_color)
    else:
        plot.set_clim(0, int(math.ceil(np.amax(data)/1000.0)*1000.0)) #round up to nearest 1000
            
    if new_subplot:
    
        subplot.set_title(title, pad=15)  
        legend = plt.colorbar(plot, ax=subplot)
        legend.ax.get_yaxis().labelpad = 15
        legend.ax.set_ylabel("Wave velocity (m/sec)", rotation=270)

    return [plot]

def show_frame(t, datas, subplots, titles, max_color, hh, start_lat, start_lon, end_lat, end_lon, axis, rotation):
    """
    Shows a frame involving multiple images (ie cross section and perturbation
    t: frame number to draw
    datas: array of arrays of numpy 2d arrays. outermost array is a list of different images to plot in parallel,
            next layer is each frame in order of time, next layer is the numpy 2d array to display
    subplots: array of subplot instances to plot on
    titles: array of titles for each subplot
    max_color: maximum velocity (m/s) the chart should go up to (-1 for auto)
    hh: cell size (resolution) of VM (metres) (or 0 for no size)
    start_lat: latitude of top left point of cross section (or 361 for no latitude)
    start_lon: longitude of top left point of cross section (or 361 for no longitude)
    end_lat: latitude of bottom right point of cross section (or 361 for no latitude)
    end_lon: longitude of bottom right point of cross section (or 361 for no longitude)
    axis: 'x' or 'y' for which axis the cross section is on
    rotation: degrees of rotation the model is, where 0 means the x axis follows the same latitude
    """

    blit_array = []
    for i in range(len(datas)):
        blit_array += display_data_matplot(datas[i][t], max_color, hh, subplots[i], titles[i], start_lat, start_lon, end_lat, end_lon, axis, rotation)

    return blit_array

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
    parser.add_argument("axis", help="which axis to plot along (x (west to east) or y (north to south))", choices=["x", "y"])
    parser.add_argument("delta", help="how many samples along the specified axis the plot should go for", type=int)
    parser.add_argument("-max", "--maximum", type=int, default=2500, \
            help="The upper bound for the color scale (in metres/second). Anything faster than this would appear as maximum colour. Use -1 for automatic calculation of maximum (default 2500)")
    parser.add_argument("-o", "--output", type=str, default="", \
            help="If specified, the output file the plot would be saved to, otherwise opens the plot in a new window")
    parser.add_argument("-p", "--perturbation", type=str, help="If specified, the perturbation file for the same area")
    parser.add_argument("-lat", "--latitude", type=float, help="If specified, the latitude of the centre of the VM")
    parser.add_argument("-lon", "--longitude", type=float, help="If specified, the longitude of the centre of the VM")
    parser.add_argument("-rot", "--rotation", type=float, help="The rotation of the VM in degrees (default 0)", default=0)
    parser.add_argument("-ani", "--animationlength", type=int, help="How many samples to animate (default 1)", default=1)

    
    args = parser.parse_args()

    if not os.path.exists(args.vm):
        print("Error: VM file does not exist")
        quit()

    if args.perturbation and not os.path.exists(args.perturbation):
        print("Error: Perturbation file does not exist")
        quit()

    nx = args.nx
    ny = args.ny
    nz = args.nz
    hh = args.hh

    data = []
    
    start_point = (args.origin_x, args.origin_y)

    if start_point[0] >= nx or start_point[0] < 0 or start_point[1] >= ny or start_point[1] < 0:
        print("Error: Origin extends beyond boundaries of given data file")
        quit()

    max_delta = ny - start_point[1]
    max_animation = nx - start_point[0]

    axis = args.axis
    
    if axis == 'x':
        max_delta = nx - start_point[0]
        max_animation = ny - start_point[1]
    
    delta = args.delta
    ani_len = args.animationlength

    if delta <= 0 or delta > max_delta:
        print("Error: delta extends beyond boundaries of given data file")
        quit()

    if ani_len <= 0 or ani_len > max_animation:
        print("Error: animation length extends beyond boundaries of given data file")
        quit()

    length, array = load_data(args.vm, nx, ny, nz)

    if length != nx*ny*nz:
        print("Error: Inconsistent VM size. Check model and nx, ny, nz parameters and try again. Expected",nx*ny*nz,"values, got",len(array))
        quit()

    if args.perturbation:
        pert_length, pert_array = load_data(args.perturbation, nx, ny, nz)
        if pert_length != nx*ny*nz:
            print("Error: Perturbation file size does not match VM")
            quit()

    if args.latitude and args.longitude:
        mat = geo.gen_mat(args.rotation, args.longitude, args.latitude)[0]
        mid_x = math.floor(nx / 2)
        mid_y = math.floor(ny / 2)

        start_point_offset = [hh * (start_point[0]-mid_x) / KM_TO_M, hh * (start_point[1]-mid_y) / KM_TO_M]
        start_coords = tuple(geo.xy2ll(np.array([start_point_offset]), mat)[0])

        if axis == 'x':
            end_point = (start_point[0] + delta, start_point[1])
        else:
            end_point = (start_point[0], start_point[1] + delta)

        end_point_offset = [hh * (end_point[0]-mid_x) / KM_TO_M, hh * (end_point[1]-mid_y) / KM_TO_M]
        end_coords = tuple(geo.xy2ll(np.array([end_point_offset]), mat)[0])
    else:
        start_coords = (None, None)
        end_coords = (None, None)

    frames = []
    
    if axis == 'x':
        slice_x = start_point[1]
        slice_start = start_point[0]
        slice_end = start_point[0] + delta + 1
        for frame in range(slice_x, slice_x + ani_len):
            frames.append(array[frame,:,slice_start:slice_end] * KM_TO_M)
    else:
        slice_y = start_point[0]
        slice_start = start_point[1]
        slice_end = start_point[1] + delta + 1
        for frame in range(slice_y, slice_y + ani_len):
            data = array[slice_start:slice_end,:,frame] * KM_TO_M
            frames.append(np.swapaxes(data,0,1))
    
    if args.perturbation:
        fig, axs = plt.subplots(2, 1, constrained_layout=True, squeeze=False)
        pert_frames = []
        pert_mixed_array = array * pert_array
        if axis == 'x':
            for frame in range(slice_x, slice_x + ani_len):
                pert_frames.append(pert_mixed_array[frame,:,slice_start:slice_end] * KM_TO_M)
        else:
            for frame in range(slice_y, slice_y + ani_len):
                pert_data = pert_mixed_array[slice_start:slice_end,:,frame] * KM_TO_M
                pert_frames.append(np.swapaxes(pert_data,0,1))

        if ani_len == 1:
            display_data_matplot(pert_frames[0], args.maximum, hh, axs[1][0], "Cross section (with perturbation)", start_coords[1], start_coords[0], end_coords[1], end_coords[0], axis, args.rotation)
    else:
        fig, axs = plt.subplots(1, 1, constrained_layout=True, squeeze=False)

    if ani_len == 1:
        display_data_matplot(frames[0], args.maximum, hh, axs[0][0], "Cross section", start_coords[1], start_coords[0], end_coords[1], end_coords[0], axis, args.rotation)
    else:
        if args.perturbation:
            ani = animation.FuncAnimation(fig, lambda data: \
            show_frame(data, [pert_frames, frames], [axs[1][0], axs[0][0]], ["Cross section (with perturbation)", "Cross section"], \
            args.maximum, hh, start_coords[1], start_coords[0], end_coords[1], end_coords[0], axis, args.rotation), \
            blit=True, interval=50, repeat=False, frames=ani_len)
        else:
            ani = animation.FuncAnimation(fig, lambda data: \
            display_data_matplot(frames[data], args.maximum, hh, axs[0][0], "Cross section", start_coords[1], start_coords[0], end_coords[1], end_coords[0], axis, args.rotation), \
            blit=True, interval=50, repeat=False, frames=ani_len)        

    if len(args.output) > 0:
        if ani_len == 1:
            plt.savefig(args.output)
        else:
            Writer = animation.writers['ffmpeg']
            writer = Writer(fps=30, bitrate=5000)
            ani.save(args.output, writer=writer)
    else:
        plt.show()

if __name__ == "__main__":
    main()
