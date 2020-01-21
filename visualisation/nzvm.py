#dependencies:
#   MatPlotLib                      pip install matplotlib
#   NumPy                           pip install numpy
#   PyYAML                          pip install pyyaml
#   argparser                       pip install argparser

import math, numpy
import argparse
from matplotlib import pyplot
import matplotlib.ticker as ticker

from yaml import load, dump
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper

def load_data(file):
    global array
    array = numpy.fromfile(file, dtype=numpy.float32, count=-1, sep='', offset=0)
    print("Loaded",len(array),"data points")
    
def get_value(x, y, z):
    """
    Gets the value from the loaded data of the wave velocity at a given point in km/s
    Does not check for array overflow to improve performance
    """
    return array[y*nz*nx + z*nx + x]

#data in order [x1y1, x1y2, x1y3, x2y1, x2y2, x2y3]
def display_data_matplot(data, num_x, num_y, max_color):
    """
    Prepares a figure in matplotlib related to the NZVM to be displayed
    """
    img = numpy.zeros((num_y, num_x)) #swap the axes which is needed for some reason
    for x in range(num_x):
        for y in range(num_y):
            img[y, x]= data[num_y*x + y]

    fig = pyplot.figure(1)
    
    plot = pyplot.matshow(img, 1, interpolation='none')

    plot.set_cmap("jet")

    pyplot.ylabel("Depth (metres below surface)")
    pyplot.xlabel("Horizontal distance (no. samples)")

    ticks_y = ticker.FuncFormatter(lambda y, pos: '{:.0f}'.format(y/(num_y/overall_dz)))
    plot.axes.yaxis.set_major_formatter(ticks_y)

    if max_color != -1:
        pyplot.clim(0, max_color)
    else:
        pyplot.clim(0, max(data))
    
    legend = pyplot.colorbar()
    legend.ax.get_yaxis().labelpad = 15
    legend.ax.set_ylabel("Wave velocity (m/sec)", rotation=270)

if __name__ == "__main__":    
    parser = argparse.ArgumentParser(description='Generate a cross section of the NZVM.')

    parser.add_argument("vm", help="file path to the velocity model")
    parser.add_argument("yml", help="file path to the YAML configuration file for the model")
    parser.add_argument("origin_x", help="x axis offset from model where plot should begin (min 0)", type=int)
    parser.add_argument("origin_y", help="y axis offset from model where plot should begin (min 0)", type=int)
    parser.add_argument("axis", help="which axis to plot along (x (west to east) or y (north to south)) default: x")
    parser.add_argument("delta", help="how many samples along the specified axis the plot should go for", type=int)
    parser.add_argument("-max", "--maximum", type=int, default=-1, \
            help="The upper bound for the color scale (in metres/second). This and anything faster than this would appear red. Default: maximum velocity of specified data range")
    parser.add_argument("-o", "--output", type=str, default="", \
            help="If specified, the output file the plot would be saved to, otherwise opens the plot in a new window")
    
    args = parser.parse_args()

    try:
        yaml_file = open(args.yml, mode='r')
    except FileNotFoundError:
        print("Error: YAML file does not exist")
        quit()
    yaml_data = load(yaml_file, Loader=Loader)
    yaml_file.close()

    nx = yaml_data["nx"]
    ny = yaml_data["ny"]
    nz = yaml_data["nz"]

    overall_dx = yaml_data["extent_x"] * 1000 # converted to metres
    overall_dy = yaml_data["extent_y"] * 1000 # converted to metres
    overall_dz = (yaml_data["extent_zmax"] - yaml_data["extent_zmin"]) * 1000 # converted to metres

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

    try:
        load_data(args.vm)
    except FileNotFoundError:
        print("Error: VM file does not exist")
        quit()

    if len(array) != nx*ny*nz:
        print("Error: Inconsistent VM with YAML file. Check model and yml and try again. Expected",nx*ny*nz,"values, got",len(array))
        quit()
            
    if axis == 'x':
        for point in range(start_point[0], start_point[0] + delta):
            for z in range(nz):
                data.append(get_value(point, start_point[1], z) * 1000)
        overall_da = overall_dx
    else:
        for point in range(start_point[1], start_point[1] + delta):
            for z in range(nz):
                data.append(get_value(start_point[0], point, z) * 1000)
        overall_da = overall_dy

    display_data_matplot(data, int(len(data)/nz), nz, args.maximum)

    if len(args.output) > 0:
        pyplot.savefig(args.output)
    else:
        pyplot.show()
