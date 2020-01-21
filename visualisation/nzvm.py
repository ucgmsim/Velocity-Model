#dependencies:
#   MatPlotLib                      pip install matplotlib
#   NumPy                           pip install numpy
#   PyYAML                          pip install pyyaml

import math, numpy
import sys
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
    return array[y*nz*nx + z*nx + x]

def distance_sq(x1, x2, y1, y2):
    return (x1-x2)**2 + (y1-y2)**2

#data in order [x1y1, x1y2, x1y3, x2y1, x2y2, x2y3]
def display_data_matplot(data, num_x, num_y):
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

    #ticks_x = ticker.FuncFormatter(lambda x, pos: '{:.0f}'.format(x/(num_x/overall_da)))
    #plot.axes.xaxis.set_major_formatter(ticks_x)
    
    pyplot.clim(0, max(data))
    
    legend = pyplot.colorbar()
    legend.ax.get_yaxis().labelpad = 15
    legend.ax.set_ylabel("Wave velocity (m/sec)", rotation=270)
    
    pyplot.show()

if __name__ == "__main__":
    s_file = "vs3dfile.s" #sys.argv[1]
    load_data(s_file)

    yaml_file = open("vm_params.yaml", mode='r')
    yaml_data = load(yaml_file, Loader=Loader)
    yaml_file.close()

    nx = yaml_data["nx"]
    ny = yaml_data["ny"]
    nz = yaml_data["nz"]

    overall_dx = yaml_data["extent_x"] * 1000 #metres
    overall_dy = yaml_data["extent_y"] * 1000 #metres
    overall_dz = (yaml_data["extent_zmax"] - yaml_data["extent_zmin"]) * 1000 #metres

    data = []
    
    start_point = (0, 0)

    axis = 'k'

    while axis != 'x' and axis != 'y':
        axis = input("Which axis do you want to plot on (x or y)?:  ").lower()

    while True:
        line = input("What coordinates (nx, ny) do you want to plot from? Enter as 2 numbers separated by a comma. max "+str(nx-1)+","+str(ny-1)+":  ")
        try:
            start_x = int(line.split(",")[0])
            start_y = int(line.split(",")[1])
        except:
            continue
        if start_x >= (nx-1) or start_y >= (ny-1) or start_x < 0 or start_y < 0:
            continue
        start_point = (start_x, start_y)
        break

    max_delta = ny - start_point[1] - 1
    if axis == 'x':
        max_delta = nx - start_point[0]
        
    delta = 0

    while delta <= 0 or delta > max_delta:
        try:
            delta = int(input("How many samples along the "+axis+" axis do you want to plot on? max "+str(max_delta)+":  "))
        except:
            pass

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

    display_data_matplot(data, int(len(data)/nz), nz)
