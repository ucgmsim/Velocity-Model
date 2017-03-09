# Velocity Model

This is the repository for the source code for South Island Velocity Model (SIVM) generation and interrogation. The model generation is a serial process. There are five general call types which are outlined in this readme.

To clone the code from the github repository:
```
git clone https://github.com/ucgmsim/Velocity-Model
```
In order to execute the code:

1) Compile - Run make (this will compile the executable)
```
make
```
or alternatively
```
make parallel
```
2) Execute the code using one of the four call types:
- GENERATE_VELOCITY_MOD - Generates a velocity model from input parameters
- EXTRACT_VELOCITY_SLICES - Extracts velocity slices from a saved model (Model need to have been previously generated to use this call)
- GENERATE_VELOCITY_SLICES - Generated velocity slices for plotting (slices generated from scratch)
- GENERATE_PROFILE - Generates a velocity profile at a single lat-lon location
- GENERATE_MULTIPLE_PROFILES - Generates profiles for a list of lat-lon locations
- GENERATE_THRESHOLD - Generates a map of Vs30, Z1.0 or Z2.5 

First run
```
./NZVM 
```
which creates example input text files (if desired otherwise skip this step)

Then modify the desired parameters and use the text file as an input to NZVM
```
./NZVM "MyParametersFile.txt"
```

All five call types and required inputs are explained here.

Call type 1) GENERATE_VELOCITY_MOD

- 1 CALL_TYPE=GENERATE_VELOCITY_MOD
- 2	MODEL_VERSION (model version - select from list of model versions at bottom of readme)
- 3	OUTPUT_DIR (directory to save outputs to)
- 4	ORIGIN_LAT (origin latitude - in decimal format)
- 5	ORIGIN_LON (origin longitude - in decimal format)
- 6	ORIGIN_ROT (model rotation - clockwise is positive)
- 7	EXTENT_X (model extent in the X direction in km)
- 8	EXTENT_Y (model extent in the Y direction in km)
- 9	EXTENT_ZMAX (maximum model extent in the Z direction - positive downwards in km)
- 10 EXTENT_ZMIN (minimum model extent in the Z direction - positive downwards i.e -1 represents +1km above mean sea level)
- 11 EXTENT_Z_SPACING (gridspacing in the Z direction in km)
- 12 EXTENT_LATLON_SPACING (gridspacing in the Y and X direction in km)
- 13 MIN_VS (minimium shear wave velocity to enforce, in km/s - typically 0.5)
- 14 TOPO_TYPE (SQUASHED, BULLDOZED etc)

The output directory must not exist

Call type 2) EXTRACT_VELOCITY_SLICES - All parameters must be set and the output directory must have been generated previously using GENERATE_VELOCITY_MOD call

- 1 CALL_TYPE=EXTRACT_VELOCITY_SLICES
- 2 MODEL_VERSION (model version - select from list of model versions at bottom of readme)
- 3 OUTPUT_DIR (directory to save outputs to)
- 4 ORIGIN_LAT (origin latitude - in decimal format)
- 5 ORIGIN_LON (origin longitude - in decimal format)
- 6 ORIGIN_ROT (model rotation - clockwise is positive)
- 7 EXTENT_X (model extent in the X direction in km)
- 8	EXTENT_Y (model extent in the Y direction in km)
- 9	EXTENT_ZMAX (maximum model extent in the Z direction - positive downwards in km)
- 10 EXTENT_ZMIN (minimum model extent in the Z direction - positive downwards i.e -1 represents +1km above mean sea level)
- 11 EXTENT_Z_SPACING (gridspacing in the Z direction in km)
- 12 EXTENT_LATLON_SPACING (gridspacing in the Y and X direction in km)
- 13 MIN_VS (minimium shear wave velocity to enforce, in km/s - typically 0.5)
- 14 TOPO_TYPE (SQUASHED, BULLDOZED etc)
- 15 EXTRACTED_SLICE_PARAMETERS_TEXTFILE (textfile location withslice parameters)

- See readme in ExtractedSliceParameters directory for additional information


Call type 3) GENERATE_VELOCITY_SLICES

- 1 CALL_TYPE=GENERATE_VELOCITY_SLICES
- 2 MODEL_VERSION (model version - select from list of model versions at bottom of readme)
- 3 OUTPUT_DIR (directory to save outputs to)
- 4 GENERATED_SLICE_PARAMETERS_TEXTFILE (textfile location with slice parameters)
- 5 TOPO_TYPE (SQUASHED, BULLDOZED etc)


Call type 4) GENERATE_PROFILE - All parameters must be set and the output directory must not exist

- 1 CALL_TYPE=GENERATE_PROFILE
- 2 MODEL_VERSION (model version - select from list of model versions at bottom of readme)
- 3 OUTPUT_DIR (directory to save outputs to)
- 4 PROFILE_LAT (latitude point of profile - in decimal format)
- 5 PROFILE_LON (longitude point of profile - in decimal format)
- 6 PROFILE_ZMIN (minimum depth of profile +ve downwards in km ie. -0.1 corresponds to the
- 7 PROFILE_ZMAX (maximum depth of profile +ve downwards in km) 
- 8	 EXTENT_Z_SPACING_PROFILE (z spacing of the profile in km)
 top of the profile at +0.1km above mean sea level)
- 9 PROFILE_MIN_VS (minimium shear wave velocity to enforce, in km/s - typically 0.5, set as 0.0 for no restriction)
- 10 TOPO_TYPE (SQUASHED, BULLDOZED etc)

Call type 5) GENERATE_MULTIPLE_PROFILES - Two different types: constant spacing, and irable spacing profile generation (the output directory must not exist)

- 1 CALL_TYPE=GENERATE_MULTIPLE_PROFILES
- 2 MODEL_VERSION (model version - select from list of model versions at bottom of readme)
- 3 OUTPUT_DIR (directory to save outputs to)
- 4 PROFILE_MIN_VS (minimium shear wave velocity to enforce, in km/s - typically 0.5, set as 0.0 for no restriction)
- 5 TOPO_TYPE (SQUASHED, BULLDOZED etc)
- 6 COORDINATES_TEXTFILE (textfile location with coordinates to generate profiles at)
- 7 SPACING_TYPE (Set as CONSTANT, or VARIABLE)
Optional inputs
if SPACING_TYPE=CONSTANT, then set:
- 8 PROFILE_ZMIN (minimum depth of profile +ve downwards in km ie. -0.1 corresponds to the
- 9 PROFILE_ZMAX (maximum depth of profile +ve downwards in km) 
- 10 SPACING_PROFILE (z spacing of the profile in km)
if SPACING_TYPE=VARIABLE, then set:
- 11 PROFILE_DEPTHS_TEXTFILE (textfile location with vector of depths to prescribe velocities to)

Call type 6) GENERATE_THRESHOLD

Extracts Vs30 Vs500 Z1.0 Z2.5

- 1	CALL_TYPE=GENERATE_THRESHOLD
- 2 MODEL_VERSION (model version - select from list of model versions at bottom of readme)
- 3 OUTPUT_DIR (directory to save outputs to)
- 4 ORIGIN_LAT (origin latitude - in decimal format)
- 5 ORIGIN_LON (origin longitude - in decimal format)
- 6 ORIGIN_ROT (model rotation - clockwise is positive)
- 7 EXTENT_X (model extent in the X direction in km)
- 8 EXTENT_Y (model extent in the Y direction in km)
- 9 EXTENT_LATLON_SPACING (gridspacing in the Y and X direction in km)
- 10 VS_TYPE (set as VS30 VS500 Z1.0 Z2.5)


Execute one of these six call types and open the output directory to view saved outputs.

List of TOPO_TYPES

Unshifted representations: 
- TRUE
- BULLDOZED - Cuts off all gridpoints above Mean Sea Level (0km)

Shifted representations: Note that no gridpoint with a value above Mean Sea Level (MSL) (0km) will have velocities prescribed to it.
- SQUASHED - Pushes the ground surface (DEM) to zero meteres, all values are then relative to this. 
- SQUASHED_TAPERED - Compacts the region between the DEM and MSL into the region between MSL and -1.0xZ(DEM)

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
- 1D model version (Cant1D_v2.fd_modfile)

- v1.65 Same as v1.64 with updated 1D velocity model to ensure Vs_1dmax < Vs_pliocene
- 1D model version (Cant1D_v2_Pliocene_Enforced.fd_modfile)

