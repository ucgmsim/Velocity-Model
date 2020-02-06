## What is the NZVM Visualiser?
A tool created for researchers working on the New Zealand Velocity Model, the NZVM Visualiser creates cross sections of the model to portray the wave velocity in different parts of NZ

## Installation
The visualiser is created to run in Python 3, which can be installed from https://www.python.org/downloads/

You can install the standard python libraries with:
```
pip install matplotlib numpy
```

To install the qcore library:
```
git clone https://github.com/ucgmsim/qcore.git
cd qcore
python setup.py
```

To export animations you will need FFmpeg installed and added to your path

## Getting Started
1. Basic example:
```
python nzvm.py vs3dfile.s 486 763 120 x 450
```
Plots the file vs3dfile.s, being `486` wide, `763` tall and `120` deep. Given cross section will start at `(0, 0)` and travel along the `x` axis for `450` samples.

2. Comprehensive example using latitudes and longitudes:
Using a model that is `486` wide, `763` tall and `120` samples deep. The centre of the model is at `(-43.045, 172.512)` and is rotated `41` degrees clockwise. Each sample is `400` metres wide.
Plot `480` samples along the `x` axis starting from the sample at `(0, 0)` (the north western most point). Let the maximum velocity on the scale be `4000` m/s, such that anything faster will be clipped and be the same colour as at 4000. Create an animation that cuts through the `y` axis (defined as the axis not plotted along, where `x` was specified) and goes from the default `(0, 0)` sample to `y=763`
```
python nzvm.py vs3dfile.s 486 763 120 x 480 -hh 400 -max 4000 -lat -43.045 -lon 172.512 -rot 41 -ani 763
```

3. If there was a perturbation file you can also draw that by adding `-p pert.out` where `pert.out` is the name of the perturbation file. This will then create 2 plots vertically, the top one being without perturbation, and the bottom one being with perturbation.

4. When `-lat` and `-lon` aren't specified, the application doesn't know where the data is located so will default to showing the data in metres. If `-hh` is not specified then the application will use no. of samples as a unit.

5. When `-ani` is not specified, `-o` (output file) will be in `PNG` format. Otherwise output is `MP4` format.

## Auto-Generated Help File
```
usage: nzvm.py [-h] [-hh HH] [-x ORIGIN_X] [-y ORIGIN_Y] [-max MAXIMUM] [-o OUTPUT] [-p PERTURBATION] [-lat LATITUDE]
               [-lon LONGITUDE] [-rot ROTATION] [-ani ANIMATIONLENGTH]
               vm nx ny nz {x,y} delta

Generate a cross section of the NZVM.

positional arguments:
  vm                    file path to the velocity model
  nx                    data number of samples in x direction
  ny                    data number of samples in x direction
  nz                    data number of samples in x direction
  {x,y}                 which axis to plot along (x (west to east) or y (north to south))
  delta                 how many samples along the specified axis the plot should go for

optional arguments:
  -h, --help            show this help message and exit
  -hh HH, --hh HH       The resolution of the data (in metres). e.g. 400 means each change in nx, ny, nz is 400m
  -x ORIGIN_X, --origin_x ORIGIN_X
                        x axis offset from model where plot should begin (default 0)
  -y ORIGIN_Y, --origin_y ORIGIN_Y
                        y axis offset from model where plot should begin (default 0)
  -max MAXIMUM, --maximum MAXIMUM
                        The upper bound for the color scale (in metres/second). Anything faster than this would appear
                        as maximum colour. Use -1 for automatic calculation of maximum (default 2500)
  -o OUTPUT, --output OUTPUT
                        If specified, the output file the plot would be saved to, otherwise opens the plot in a new
                        window
  -p PERTURBATION, --perturbation PERTURBATION
                        If specified, the perturbation file for the same area
  -lat LATITUDE, --latitude LATITUDE
                        If specified, the latitude of the centre of the VM
  -lon LONGITUDE, --longitude LONGITUDE
                        If specified, the longitude of the centre of the VM
  -rot ROTATION, --rotation ROTATION
                        The rotation of the VM in degrees (default 0)
  -ani ANIMATIONLENGTH, --animationlength ANIMATIONLENGTH
                        How many samples to animate (default 1)
```
						

## Known Issues
1. When there is an animation using lat and lon, the degrees along the x axis do not update if the model has a rotation other than 0 degrees.
