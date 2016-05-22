# groundMotionStationAnalysis

This is the repository for the Python code for post-processing of the ground motion time series for each station, including Intensity Measure (IM) calculation, and plotting of the velocity seismograms and IMs. This replaces the original matlab code that was not parallel and resided on local machines. This Python code can be run either in parallel using the load leveller script or linearly in Python. All the parameters for the IM calculation and plotting are stored in parameters.py. The name of the parameter file is passed as an argument to the script runPostProcessStation.py , so new parameters files with different names can be created. Wherever possible, the parameter and function names from the original matlab code have been retained.

To clone the code from the github repository:
```
git clone https://github.com/ucgmsim/groundMotionStationAnalysis
```
In order to run the code:

1) In parallel:
```
llsubmit runPostProcessStation.ll
```
The parameter filename needs to be changed in the load leveller script. The number of processes in the .ll script is currently set to 8. More than this had no significant improvement on the speed.

2) From python:
```
python runPostProcessStation.py parameterfilename
```
The individual plotting functions can also be run without re-running the IM calculation in python by:
```
python runPlotObsSynIntensityMeasures parameterfilename
```
etc.

3) From ipython (interactive python):
```
ipython --pylab
```
```
run'runPostProcessStation.py' parameterfilename
```
The individual plotting functions can also be run without re-running the IM calculation in ipython by:
```
run'runPlotObsSynIntensityMeasures' parameterfilename
```
and
```
run'runPlotVelocitySeismogramsWithDistance'	parameterfilename
```
```
run'runPlotObservedSimulatedTimeSeries'	parameterfilename
```
In the parameter file, there are five flags in a class object called flags that control the postprocessing workflow in the top level script runPostProcess.py . 

Flag Name | Purpose
------------------------------------------------- | ----------------------------------------------------------------------------------------
calcIM	| Whether to calculate the Intensity Measures and save out to .h5 file 
plotSeismogram | Whether to plot the Velocity Seismograms with distance and save to .eps and .png
plotIM | Whether to plot the Intensity Measures with distance and save to .eps and .png
plotTimeSeries | Whether to plot the Time Series and save to .eps and .png
plotGMPE | Whether to overlay the Ground Motion Prediction Equation on the Intensity Measure figures

In the first 4 cases, for these flags:

Flag Value | Workflow control
------------------- | ------------------------------
0 | Done for neither obseravations nor simulations
1 | Done for observations only
2 | Done for simulations only
3 | Done for both observations and simulations

For the flag plotGMPE, a value of 1 does overlay the GMPE on the IMs, a value of 0 does not.

The other variables in the parameter file define the input and output filenames and directories, and the specifics of the plotting (which IM to plot etc). 
The definitions for these are commented inside the parameter file. The variables that are in the parameter file are grouped together as class objects, based on which plot function they are for etc.

## Unit tests

There are currently three unit tests for this code. They are:

1) Intensity Measurement calculation compared against the Darfield September 4th earthquake,

2) Source-to-Site distance calculation for specific fault and station files (again Darfield Sept 4th earthquake),

3) GMPE calculation (mean and sigma) for both PGA and pSA (at 1s) for the Darfield earthquake.

In all 3 cases, a .h5 file of reference values is saved in the github. By using the parameter file parameters_unit_test.py , the IMs, rupture distances or GMPEs are calculated for the current code version and the outputs are compared against the reference versions. The absolute and relative differences for both the IMs and Rrups are printed to the .out file if they do not exactly agree with the reference values.

The unit tests can be run by setting, respectively:

tests.IM_unit_test=True         
                    
tests.Rrup_unit_test=True

tests.GMPE_unit_test=True

These .h5 reference files have been verified to ~15 significant figures against the original matlab code.

When the calculations within the Python post-processing code are changed, these unit tests should be run!

The plotting functions should be verified against PostProcessPlots.pdf also in this repository.


