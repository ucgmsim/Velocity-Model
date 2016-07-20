# UCVM-Velocity-Model-Integration

f# Velocity-Model

This is the repository for the source code for South Island Velocity Model (SIVM) generation and interrogation. The model generation is a serial process. There are four general call types which are outlined in this readme.

To clone the code from the github repository:
```
git clone https://github.com/ucgmsim/UCVM-Velocity-Model-Integration
```
In order to execute the code:

1) Compile:
```
gcc -Wall -g -std=c99 -o SIVM *.c -lm -g
```
2) Execute the code using one of the four call types:
- GENERATE_VELOCITY_MOD - Generates a velocity model from input parameters
- EXTRACT_VELOCITY_SLICES - Extracts velocity slices from a saved model (will generate the model first if it does not exist)
- GENERATE_VELOCITY_SLICES - Generated velocity slices for plotting (slices generated from scratch)
- GENERATE_INDIVIDUAL_PROFILE - Generates a velocity profile at a single lat/lon location

All four call types and required inputs are explained here.

Call type 1) GENERATE_VELOCITY_MOD

- 1	GENERATE_VELOCITY_MOD
- 2	MODEL_VERSION (model version - select from list of model versions at bottom of readme)
- 3	OUTPUT_DIR (directory to save outputs to)
- 4	ORIGIN_LAT (origin latitude - in decimal format)
- 5	ORIGIN_LON (origin longitude - in decimal format)
- 6	ORIGIN_ROT (model rotation - clockwise is positive)
- 7	EXTENT_X (model extent in the X direction away from the origin - i.e. for a 100km wide model input 50)
- 8	EXTENT_Y (model extent in the Y direction away from the origin - i.e. for a 100km wide model input 50)
- 9	EXTENT_ZMAX (maximum model extent in the Z direction - positive downwards in km)
- 10	EXTENT_ZMIN (minimum model extent in the Z direction - positive downwards i.e -1 represents +1km above mean sea level)
- 11	EXTENT_Z_SPACING (gridspacing in the Z direction in km)
- 12	EXTENT_LATLON_SPACING (gridspacing in the Y and X direction in km)
- 13	MIN_VS (minimium shear wave velocity to enforce, in km/s - typically 0.5)

Example of GENERATE_VELOCITY_MOD call
```
./SIVM GENERATE_VELOCITY_MOD 1.01 v1.01Model -43.6 173.2 -10 70 60 46 0 1 1 0.5
```
Call type 2) EXTRACT_VELOCITY_SLICES

- 1	EXTRACT_VELOCITY_SLICES
- 2	MODEL_VERSION (model version - select from list of model versions at bottom of readme)
- 3	OUTPUT_DIR (directory to save outputs to)
- 4	ORIGIN_LAT (origin latitude - in decimal format)
- 5	ORIGIN_LON (origin longitude - in decimal format)
- 6	ORIGIN_ROT (model rotation - clockwise is positive)
- 7	EXTENT_X (model extent in the X direction away from the origin - i.e. for a 100km wide model input 50)
- 8	EXTENT_Y (model extent in the Y direction away from the origin - i.e. for a 100km wide model input 50)
- 9	EXTENT_ZMAX (maximum model extent in the Z direction - positive downwards in km)
- 10	EXTENT_ZMIN (minimum model extent in the Z direction - positive downwards i.e -1 represents +1km above mean sea level)
- 11	EXTENT_Z_SPACING (gridspacing in the Z direction in km)
- 12	EXTENT_LATLON_SPACING (gridspacing in the Y and X direction in km)
- 13	MIN_VS (minimium shear wave velocity to enforce, in km/s - typically 0.5)
- 14	EXTRACTED_SLICE_PARAMETERS_DIRECTORY (directory housing slice parameters text file)

Example of EXTRACT_VELOCITY_SLICES call (will generate a model if it does not already exist)
- See readme in ExtractedSliceParameters directory for additional information
```
./SIVM EXTRACT_VELOCITY_SLICES 1.01 v1.01Model -43.6 173.2 -10 70 60 46 0 1 1 0.5 ExtractedSliceParameters
```

Call type 3) GENERATE_VELOCITY_SLICES

- 1	GENERATE_VELOCITY_SLICES
- 2	MODEL_VERSION (model version - select from list of model versions at bottom of readme)
- 3	OUTPUT_DIR (directory to save outputs to)
- 4	GENERATED_SLICE_PARAMETERS_DIRECTORY (directory housing slice parameters text file)

Example of GENERATE_VELOCITY_SLICES call
```
./SIVM GENERATE_VELOCITY_SLICES 1.01 v1.01Model GeneratedSliceParameters
```

Call type 4) GENERATE_INDIVIDUAL_PROFILE

- 1	GENERATE_INDIVIDUAL_PROFILE
- 2	MODEL_VERSION (model version - select from list of model versions at bottom of readme)
- 3	OUTPUT_DIR (directory to save outputs to)
- 4	PROFILE_LAT (latitude point of profile - in decimal format)
- 5	PROFILE_LON (longitude point of profile - in decimal format)
- 6	PROFILE_ZMAX (maximum depth of profile +ve downwards in km) 
- 7	PROFILE_ZMIN (minimum depth of profile +ve downwards in km ie. -0.1 corresponds to the top of the profile at +0.1km above mean sea level)
- 8	EXTENT_Z_SPACING (z spacing of the profile in km)

Example of GENERATE_INDIVIDUAL_PROFILE 
- See readme in GeneratedSliceParameters directory for additional information

```
./SIVM GENERATE_INDIVIDUAL_PROFILE 1.01 v1.01Model -43.5 172.5 0.1 -.05 0.001
```


Execute one of these four call types and open the output directory to view saved outputs.



Summary of velocity model version numbers

- v1.01 1D velocity model 
- 1D model version (Cant. 1D velocity model)
- v1.02 1D velocity model 
- 1D model version (Cant. 1D velocity model v2)

- v1.11 Eberhart-Phillips Tomography 
- (Unpublished updated Canterbury version)

- v1.21 Tomography below basement, 1D above
- 1D model version (Cant. 1D velocity model)
- Basement version (BasementPoints_35_v8p9_1000m.txt)
- v1.22 Tomography below basement, 1D above
- 1D model version (Cant. 1D velocity model v2)
- Basement version (BasementPoints_35_v8p9_1000m.txt)

- v1.31 Tomography below basement, constant velocity Paleogene layer, 1D Above
- 1D model version (Cant. 1D velocity model)
- Basement version (BasementPoints_35_v8p9_1000m.txt)
- Paleogene version (PaleogenePoints_35_v8p9_1000m.txt)
- Paleogene - Vp=2.7 , Vs=1.1511 , Rho=2.151

- v1.41 Tomography below basement, constant velocity Paleogene & Miocene layers, 1D Above
- 1D model version (Cant. 1D velocity model)
- Basement version (BasementPoints_35_v8p9_1000m.txt)
- Paleogene version (PaleogenePoints_35_v8p9_1000m.txt)
- Miocene version (MiocenePoints_35_v8p9_1000m.txt)
- Paleogene - Vp=2.7 , Vs=1.1511 , Rho=2.151
- Miocene - Vp=2.5, Vs=0.98355, Rho=2.9031 

- v1.51 Tomography below basement, constant velocity Paleogene & Miocene layers, 1D Above, BPV Basin
- 1D model version (Cant. 1D velocity model)
- Basement version (BasementPoints_35_v8p9_1000m.txt)
- Paleogene version (PaleogenePoints_35_v8p9_1000m.txt)
- Miocene version (MiocenePoints_35_v8p9_1000m.txt)
- BPV top version (BPPoints_41_v1p13_1000m.txt)
- Paleogene - Vp=2.7 , Vs=1.1511 , Rho=2.151
- Miocene - Vp=2.5, Vs=0.98355, Rho=2.9031 
- BPV - Vp=4.0, Vs=2.2818, Rho= 2.393 

- v1.61 Tomography below basement, constant velocity Paleogene, Miocene & Pliocene layers, 1D Above, BPV Basin
- 1D model version (Cant. 1D velocity model)
- Basement version (BasementPoints_35_v8p9_1000m.txt)
- Paleogene version (PaleogenePoints_35_v8p9_1000m.txt)
- Miocene version (MiocenePoints_35_v8p9_1000m.txt)
- Pliocene version (PliocenePoints_35_v8p9_1000m.txt)
- BPV top version (BPPoints_41_v1p13_1000m.txt)
- Paleogene - Vp=2.7 , Vs=1.1511 , Rho=2.151
- Miocene - Vp=2.5, Vs=0.98355, Rho=2.9031 
- Pliocene - Vp=2.0, Vs=0.6086, Rho = 1.905
- BPV - Vp=4.0, Vs=2.2818, Rho= 2.393 

- v1.62 Same as v1.61 with different parameters for the BPV
- BPV - Vp=3.6, Vs=1.9428, Rho= 2.334 

- v1.63 Same as v1.61 with weathering function for the top 100m of the BPV
- BPV at 0m - Vp=3.2, Vs=1.59, Rho= 2.265 
- BPV below 100m - Vp=4.0, Vs=2.2818, Rho= 2.393 
- Linear relationship between 0 and 100m depth

- v1.64 Same as v1.63 with updated 1D velocity model
- 1D model version (Cant. 1D velocity model v2)





