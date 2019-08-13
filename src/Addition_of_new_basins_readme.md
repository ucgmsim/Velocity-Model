# How to add new basins to the NZVM

Required resources to add a new basin to the NZVM 
    - (1) Basin surfaces in standard surface '.in' format; 
    - (2) A boundary file with lat-lon points that define the basin polygon (the polygon must be closed i.e the first and last points in the polygon must be the same). 
    - (3) A velocity parameterisation (can use a preexisting parameterisation such as the 1D velocity structure applied in the Canterbury basin)
    - (4) An optional offshore smoothing points text file which basin-edge smoothing will be appied at can also be specified. (The points in this file must be a subset of the points from the boundary in (2))


Steps to add basin model to NZVM source code, requires definining a new model version:

- (1) Define a new model version within 'getSurfSubModNames.c' (see v2.01 for an example),  
  - (a) increment the 'nBasins' variable by 1, 
  - (b) define the name of the basin that is to be added, following the naming convention 'basinName_vXXpX', where 'basinName' is the name of the basin and vXXpX' is the version year/month of the model being added.

- (2) Define a new basin model function in 'basinSubModelList.c', use the function naming convention 'load_basinName_vXXpX'.  
- Within the new function define: 
   - (a) the number of surfaces in the basin; 
   - (b) the number of boundaries (typically 1); 
   - (c) Specify the path to basin resources, both surfaces and the basin boundary.
   - (d) Specify the velocity parameterisation to be applied (basinSubModelNames), can use a preexisting or velocity parameterisation e.g. "Cant1D_v2", this is a string that will be used to apply the correct parameterisation which is defined elsewhere. See predefined basin models for example format e.g. "load_WaikatoHauraki_v19p7" for a simple basin defined by a top and a bottom surface, or  "load_Canterbury_Pre_Quaternary_v18p4" for a more complex basin comprising multiple layers. 
   - (e) copy the function declaration  'load_basinName_vXXpX(...' into the header 'functions.h'.

- (3) Add to the 'if' statement within the "loadBasinResources' function, the name of the basin should follow the format 'basinName_vXXpX'.

- (4) (Only if a new velocity parameterisation is required) The 'callBasinSubVelocityModels' function in 'basinSubModels.c' is used to call basin velocity parameterisations which are defined at the below this function. If desired, a new velocity parameterisation can be created (be sure to add the function declaration to the 'functions.h' header). See 'paleogeneSubModelv1' for a simple constant velocity parameterisation, or 'BPVSubModelv4' for a more complex parameterisation. If using a 1D velocity prameterisation using the standard '.fd_modfile' format, the 'v1DsubMod' function is used as a substitute for a standalone velocity parameterisation functions, 1D submodels need to be loaded into memory in the preprocessing step which is done in the 'loadBasinSubModelData' function within loadBasins.c'. 

- (5) (Only if offshore smoothing is required) place the smoothing textfile which is named 'basinName_vXXpX.txt' in 'Data/Boundaries/Smoothing', this will be automatically utilised whenever the 'basinName_vXXpX' is utilised. This smoothing file must be a subset of the points specified in the boundary used to characterise the basin. 
