 //
//  functions.h
//  CVMversions
//
//  Created by Ethan M. Thomson on 20/06/14.
//  Copyright (c) 2014 Dept. Civil Engineering. All rights reserved.
//

#ifndef functions_h
#define functions_h

// extraction and file I/O
//extern void generateProfile(modOrigin modelOrigin, modVersion modelVersion, modExtent modelExtent, char *outputDirectory);
//extern void writeAllBasinSurfaceDepths(globalBasinData *basinData, gridStruct *location, char *outputDirectory);
//extern void writeBasinSurfaceDepths(globalBasinData *basinData,  int basinNum, gridStruct *location, char *outputDirectory);
//extern void writeIndividualProfile(globalDataValues *globalValues, gridStruct *location, char *outputDirectory);
 extern void writeMultipleProfiles(qualities_vector *QUALITIES_VECTOR, gen_multi_profiles_call GEN_MULTI_PROFILES_CALL, mesh_vector *MESH_VECTOR, char *OUTPUT_DIR, calculation_log *CALCULATION_LOG, int profileNumber);
 extern void writeIndividualProfile(qualities_vector *QUALITIES_VECTOR, gen_profile_call GEN_PROFILE_CALL, mesh_vector *MESH_VECTOR, char *OUTPUT_DIR, calculation_log *CALCULATION_LOG);
extern void writeProfileSurfaceDepths(global_model_parameters *GLOBAL_MODEL_PARAMETERS, basin_data *BASIN_DATA, partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, in_basin *IN_BASIN, mesh_vector *MESH_VECTOR, char *OUTPUT_DIR, calculation_log *CALCULATION_LOG);
extern void writeMultipleProfileSurfaceDepths(global_model_parameters *GLOBAL_MODEL_PARAMETERS, basin_data *BASIN_DATA, partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, in_basin *IN_BASIN,mesh_vector *MESH_VECTOR, char *OUTPUT_DIR, calculation_log *CALCULATION_LOG, int profileNum);
 extern multi_gridpoint_parameters *readGridpointsTextFile(char *gridpointsTextFile);
 extern void runGenerateMultipleVSonGrid(char *MODEL_VERSION, char *OUTPUT_DIR, gen_extract_multi_gridpoint_vs_call GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL, calculation_log *CALCULATION_LOG);
 extern void writeGridpointVelocities(qualities_vector *QUALITIES_VECTOR, gen_extract_multi_gridpoint_vs_call GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL, mesh_vector *MESH_VECTOR, char *OUTPUT_DIR, calculation_log *CALCULATION_LOG, int groupingNum);



         extern void writeAllBasinSurfaceDepths(global_model_parameters *GLOBAL_MODEL_PARAMETERS, partial_global_mesh *PARTIAL_GLOBAL_MESH, int basinNum, char *OUTPUT_DIR,slice_surface_depths *SLICE_SURFACE_DEPTHS);
extern void writeSliceSurfaceDepths(global_model_parameters *GLOBAL_MODEL_PARAMETERS,partial_global_mesh *PARTIAL_GLOBAL_MESH, char *OUTPUT_DIR, slice_surface_depths *SLICE_SURFACE_DEPTHS);
extern void writeAllGlobalSurfaceDepths(slice_surface_depths *SLICE_SURFACE_DEPTHS, partial_global_mesh *PARTIAL_GLOBAL_MESH, global_model_parameters *GLOBAL_MODEL_PARAMETERS,char *OUTPUT_DIR);


// slice functions
//extern void generateSlice(modOrigin modelOrigin, sliceExtent sliceBounds, modVersion modelVersion,  char *outputDirectory);
//extern void extractSlice(gridStruct *location, modOrigin modelOrigin, sliceExtent sliceBounds, globalDataValues *globDataVals, char *outputDirectory, int sliceNumber);
//extern void generateSliceXYpoints(sliceExtractData *sliceData, modOrigin modelOrigin, sliceExtent sliceBounds);
//extern globalDataValues* loadCvmDataAll(gridStruct *location, char *outputDirectory);



// primary functions
extern global_model_parameters *getGlobalModelParameters(char *modelVersion);
//extern surfaceDepthsGlobal *getSurfaceValues(surfNames *surfSubModNames);
extern void loadAllGlobalData(global_model_parameters *GLOBAL_MODEL_PARAMETERS,calculation_log *CALCULATION_LOG, velo_mod_1d_data *VELO_MOD_1D_DATA, nz_tomography_data *NZ_TOMOGRAPHY_DATA, global_surfaces *GLOBAL_SURFACES, basin_data *BASIN_DATA);
extern void generateFullModelGridPointRad(model_extent *MODEL_EXTENT, global_mesh *GLOBAL_MESH);
extern void generateFullModelGridGreatCircle(model_extent *MODEL_EXTENT, global_mesh *GLOBAL_MESH);

extern void assignQualities(global_model_parameters *GLOBAL_MODEL_PARAMETERS, velo_mod_1d_data *VELO_MOD_1D_DATA, nz_tomography_data *NZ_TOMOGRAPHY_DATA, global_surfaces *GLOBAL_SURFACES, basin_data *BASIN_DATA, mesh_vector *MESH_VECTOR,partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS,in_basin *IN_BASIN, qualities_vector *QUALITIES_VECTOR, calculation_log *CALCULATION_LOG, char *TOPO_TYPE);
extern mesh_vector *extractMeshVector(partial_global_mesh *PARTIAL_GLOBAL_MESH, int lonInd);
extern mesh_vector *extendMeshVector(partial_global_mesh *PARTIAL_GLOBAL_MESH, int nPts, double dZPt, int lonInd);

extern void writeGlobalQualities(char *OUTPUT_DIR, partial_global_mesh *PARTIAL_GLOBAL_MESH, partial_global_qualities *PARTIAL_GLOBAL_QUALITIES, gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL, calculation_log *CALCULATION_LOG, int latInd);
//extern gridStruct *generateLatLonForPlotting(modOrigin modelOrigin, modExtent modelExtent, double latPts[], double lonPts[], int nPts);
extern void loadGlobalSurfaceData(global_surfaces *GLOBAL_SURFACES, global_model_parameters *GLOBAL_MODEL_PARAMETERS);
extern global_surf_read *loadGlobalSurface(char *fileName);
extern basin_surf_read *loadBasinSurface(char *fileName);
void loadBasinBoundaries(int basinNum, basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS);
extern void interpolateGlobalSurfaceDepths(global_surfaces *GLOBAL_SURFACES, mesh_vector *MESH_VECTOR ,partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS, calculation_log *CALCULATION_LOG);
extern void determineIfWithinBasinLatLon(basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS, in_basin *IN_BASIN, double Lat, double Lon);
extern void interpolateBasinSurfaceDepths(basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS, in_basin *IN_BASIN, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, mesh_vector *MESH_VECTOR);
extern void runGenerateVelocityModel(char *MODEL_VERSION, char *OUTPUT_DIR, gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL, calculation_log *CALCULATION_LOG);
extern void runExtractFromVelocityModel(char *MODEL_VERSION, char *OUTPUT_DIR, gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL, calculation_log *CALCULATION_LOG);
extern void runGenerateVelocitySlices(char *MODEL_VERSION, char *OUTPUT_DIR, gen_velo_slices_call GEN_VELO_SLICES_CALL, calculation_log *CALCULATION_LOG);
extern void runThresholdVelocityModel(char *MODEL_VERSION, char *OUTPUT_DIR, gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL, calculation_log *CALCULATION_LOG);

extern void runGenerateProfile(char *MODEL_VERSION, char *OUTPUT_DIR, gen_profile_call GEN_PROFILE_CALL, calculation_log *CALCULATION_LOG);
extern void runGenerateMultipleProfiles(char *MODEL_VERSION, char *OUTPUT_DIR, gen_multi_profiles_call GEN_MULTI_PROFILES_CALL, calculation_log *CALCULATION_LOG);
extern void gcprojRev(double *xf,double *yf,double rlon,double rlat,double ref_rad,double g0,double b0,double amat[8],double ainv[8]);
extern void ll2xy(double originLat, double originLon, double originRot, double *xp, double *yp, double latA, double lonA);

extern void writeSampleInputTextFiles(void);
extern gen_extract_velo_mod_call readGenVMInputTextFile(char *fileName);
extern gen_extract_velo_mod_call readExtractVMInputTextFile(char *fileName);
extern gen_velo_slices_call readGenerateSliceInputTextFile(char *fileName);
extern gen_profile_call readGenerateProfileInputTextFile(char *fileName);
extern gen_extract_velo_mod_call readThresholdInputTextFile(char *fileName);
extern gen_multi_profiles_call readGenMultiProfileInputTextFile(char *fileName);
extern gen_extract_multi_gridpoint_vs_call readExtractMultiInputTextFile(char *fileName);
extern multi_profile_parameters *readProfilesTextFile(char *fileName);
extern char *readParameter(char *fileName, char *quality);







//extern void writeLatLonData(gridStruct *location, globalDataValues *globDataVals, char *outputDirectory);
//extern void writeGridPoints(gridStruct *location, char *outputDirectory);




// secondary functions
extern void createAllOutputDirectories(char *OUTPUT_DIR, char *genCallType);
extern void writeVeloModInputsLogFile(int argc, char *argv[], char *OUTPUT_DIR);
extern void writeVeloModLogFile(calculation_log *CALCULATION_LOG);
extern void writeVeloModCornersTextFile(global_mesh *GLOBAL_MESH, char *OUTPUT_DIR);
extern void checkVeloModInputsLogFile(int argc, char *argv[], char *OUTPUT_DIR);



//extern void writeSliceParametersLogFile(sliceParams *sliceParameters, modVersion modelVersion, gridStruct *location, char *outputDirectory, char *type);
//extern indexStruct calculateSurfaceLatLonIndex(surfRead currentSurface, double x, double y);
//extern double IDW(surfVec *depVec, double x, double y, double power, double radius);
extern double linearInterpolation(double p1, double p2, double v1, double v2, double p3);
extern double biLinearInterpolation(double X1, double X2, double Y1, double Y2, double Q11, double Q12, double Q21, double Q22, double X, double Y);
extern int findGlobalSubVeloModelInd(double depth, partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS);
extern adjacent_points *findBasinAdjacentPoints(basin_surf_read *BASIN_SURF_READ, double lat, double lon);
extern void determineBasinSurfaceDepths(basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS, in_basin *IN_BASIN, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, double Lat, double Lon);


//extern depInterpVals generateSurfaceValuesAllDepths(surfVecGlobal *vector, gridStruct location, int elev[], int nElev, double power, double radius);
extern int pointInPoly(basin_data *BASIN_DATA, int basinNum, int boundaryNum, double xLoc, double yLoc);
extern int pnpoly(int nvert, double *vertx, double *verty, double testx, double testy);

extern void plotExtracts(void);
extern adjacent_points *findGlobalAdjacentPoints(global_surf_read *GLOBAL_SURF_READ, double lat, double lon);
extern void findCornerInds(global_surf_read *GLOBAL_SURF_READ, double latPt, double lonPt, adjacent_points *ADJACENT_POINTS);
extern void findEdgeInds(global_surf_read *GLOBAL_SURF_READ, adjacent_points *ADJACENT_POINTS, int edgeType);
//extern void writeAllSurfaceDepths(gridStruct *location, surfDepValues *surfDep, char *outputDirectory);
//extern void writeGlobalSurfaceDepths(surfaceDepthsGlobal *surfDepGlob, gridStruct *location, surfNames *surfSubModNames, char *outputDirectory);
//extern void writeAllGlobalSurfaceDepths(surfaceDepthsGlobal *surfDepGlob, gridStruct *location, surfNames *surfSubModNames, char *outputDirectory);
extern partial_global_mesh *extractPartialMesh(global_mesh *GLOBAL_MESH, int latInd);
extern void enforceSurfaceDepths(global_model_parameters *GLOBAL_MODEL_PARAMETERS, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, int basinNum);
extern void enforceBasinSurfaceDepths(basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS, in_basin *IN_BASIN, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, mesh_vector *MESH_VECTOR);
extern int determineBasinSurfaceBelow(global_model_parameters *GLOBAL_MODEL_PARAMETERS, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, double depth, int basinNum);
extern int determineBasinSurfaceAbove(global_model_parameters *GLOBAL_MODEL_PARAMETERS, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, double depth, int basinNum);
extern void assignBasinQualities(global_model_parameters *GLOBAL_MODEL_PARAMETERS, basin_data *BASIN_DATA, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, qualities_vector *QUALITIES_VECTOR, double depth, int basinNum, int zInd);
extern void loadBasinSubModelData(int basinNum, basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS);
void callBasinSubVelocityModels(global_model_parameters *GLOBAL_MODEL_PARAMETERS, basin_data *BASIN_DATA, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, qualities_vector *QUALITIES_VECTOR, double depth, int basinSubModelInd, int basinNum, int zInd);
 extern variable_depth_points *readDepthPointsTextFile(char *depthsTextFile);

// surface functions
//extern void enforceSurfaceDepths(globalBasinData *basinData, int xInd, int yInd, int basinNum);
//extern surfVec *generateSurfaceVectors(surfRead *surfStruct);
//extern surfDepValues *determineSurfaceDepths(gridStruct *location, char *fileName);
//extern surfDepValues *determineSurfaceDepthsBasin(globalBasinData *basinData ,gridStruct *location, char *fileName, int basinNum, int surfNum);
//extern surfDepValues *negInfSurf(gridStruct *location);
//extern surfDepValues *posInfSurf(gridStruct *location);
//extern surfDepValues *basementRockSurf(gridStruct *location);
//extern surfDepValues *demSurf(gridStruct *location);



// basin velocity submodel functions CHCH
extern void paleogeneSubModelv1(int zInd, qualities_vector *QUALITIES_VECTOR);
extern void mioceneSubModelv1(int zInd, qualities_vector *QUALITIES_VECTOR);
extern void plioceneSubModelv1(int zInd, qualities_vector *QUALITIES_VECTOR);

extern void BPVSubModelv1(int zInd, qualities_vector *QUALITIES_VECTOR);
extern void BPVSubModelv2(int zInd, qualities_vector *QUALITIES_VECTOR);
extern void BPVSubModelv3(int zInd, qualities_vector *QUALITIES_VECTOR, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, int basinNum, double depth);
extern void gravelSubModel(int zInd, qualities_vector *QUALITIES_VECTOR, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, double depth, int basinNum);
extern void marineSubModel(int zInd, qualities_vector *QUALITIES_VECTOR, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, double depth, int basinNum);



//slice functions
extern void extractSlicesFromBinaryFiles(char *OUTPUT_DIR, gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL, calculation_log *CALCULATION_LOG, global_mesh *GLOBAL_MESH, model_extent *MODEL_EXTENT);
extern slice_parameters *readGeneratedSliceParametersFile(char *sliceParametersTextFile);
extern slice_parameters *readExtractedSliceParametersFile(char *sliceParametersTextFile);
extern void generateSlicePoints(individual_slice_data *INDIVIDUAL_SLICE_DATA, individual_slice_parameters *INDIVIDUAL_SLICE_PARAMETERS,  global_mesh *GLOBAL_MESH);
extern void generateGlobalIndsForRead(global_mesh *GLOBAL_MESH, global_data_for_interpolation *GLOBAL_DATA_FOR_INTERPOLATION, model_extent *MODEL_EXTENT, int sliceNum);
extern void globalIndReduction(global_data_for_interpolation *GLOBAL_DATA_FOR_INTERPOLATION);
extern void readGlobalDataPointsForInterpolation(char *OUTPUT_DIR, global_data_for_interpolation *GLOBAL_DATA_FOR_INTERPOLATION, global_mesh *GLOBAL_MESH, calculation_log *CALCULATION_LOG);
extern void interpolateGlobalSliceData(slice_parameters *SLICE_PARAMETERS, global_mesh *GLOBAL_MESH, model_extent *MODEL_EXTENT, global_data_for_interpolation *GLOBAL_DATA_FOR_INTERPOLATION,calculation_log *CALCULATION_LOG);
extern void interpolateIndividualSlice(global_mesh *GLOBAL_MESH, global_data_for_interpolation *GLOBAL_DATA_FOR_INTERPOLATION, int sliceNum);
extern void writeInterpolatedSlice(char *OUTPUT_DIR, global_data_for_interpolation *GLOBAL_DATA_FOR_INTERPOLATION, global_mesh *GLOBAL_MESH, calculation_log *CALCULATION_LOG, int sliceNum);
extern void writeGeneratedSlice(char *OUTPUT_DIR, partial_global_mesh *PARTIAL_GLOBAL_MESH, partial_global_qualities *PARTIAL_GLOBAL_QUALITIES, individual_slice_parameters *INDIVIDUAL_SLICE_PARAMETERS,calculation_log *CALCULATION_LOG, int sliceNum);

extern void writeSliceParametersLogFile(char *OUTPUT_DIR, slice_parameters *SLICE_PARAMETRES,  model_extent *MODEL_EXTENT, global_mesh *GLOBAL_MESH, calculation_log *CALCULATION_LOG, char *type);
extern int findGlobalMeshAdjacentPoints(global_mesh *GLOBAL_MESH, model_extent *MODEL_EXTENT, double latA, double lonA, double modelRot, double *adjYInds, double *adjXInds, double *xp, double *yp);
extern void findCrossingIndsX(global_mesh *GLOBAL_MESH, int searchIndX, int *xInd1, int *xInd2, int *yInd1, int *yInd2, int *yFound, int *xFound, double latA, double lonA);
extern void findCrossingIndsY(global_mesh *GLOBAL_MESH, int searchIndY, int *xInd1, int *xInd2, int *yInd1, int *yInd2, int *yFound, int *xFound, double latA, double lonA);
extern partial_global_mesh *generateSlicePartialMesh(individual_slice_parameters INDIVIDUAL_SLICE_PARAMETERS);









// sub velocity functions
extern void v1DsubMod(int zInd, double dep, qualities_vector *QUALITIES_VECTOR, velo_mod_1d_data *VELO_MOD_1D_DATA);
extern void load1dVeloSubModel(char *fileName, velo_mod_1d_data *VELO_MOD_1D_DATA);

extern void EPtomo2010subMod(int zInd, double dep, mesh_vector *MESH_VECTOR, qualities_vector *QUALITIES_VECTOR, nz_tomography_data *NZ_TOMOGRAPHY_DATA);
extern void freeEPtomoSurfaceData(nz_tomography_data *NZ_TOMOGRAPHY_DATA);
extern void loadEPtomoSurfaceData(char *tomoType, nz_tomography_data *NZ_TOMOGRAPHY_DATA);
extern void NaNsubMod(int zInd, qualities_vector *QUALITIES_VECTOR);
extern void GenericSubModA(int zInd, qualities_vector *QUALITIES_VECTOR);
extern void GenericSubModB(int zInd, qualities_vector *QUALITIES_VECTOR);
extern void GenericSubModC(int zInd, qualities_vector *QUALITIES_VECTOR);

extern void freeGlobalSurfaceData(global_surfaces *GLOBAL_SURFACES, global_model_parameters *GLOBAL_MODEL_PARAMETERS);



// Basin data
//extern void loadCanterburyBasinData(int basinNum, globalBasinData *basinData);
//extern void loadPreQCanterburyBasinData(int basinNum, globalBasinData *basinData);
//extern void loadPreQCanterburyBasinData1D(int basinNum, globalBasinData *basinData);
//extern void loadQCanterburyBasinData(int basinNum, globalBasinData *basinData);
//extern void loadPreQCanterburyBasinDataPaleogeneMiocene(int basinNum, globalBasinData *basinData);
//extern void loadPreQCanterburyBasinDataPaleogene(int basinNum, globalBasinData *basinData);
//extern void loadBPVBasinData(int basinNum, globalBasinData *basinData);
//extern void loadBPVWheatheredBasinData(int basinNum, globalBasinData *basinData);

extern void loadBasinData(basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS);
extern void freeAllBasinSurfaces(basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS);


//extern void determineBasinProperties(globalBasinData *basinData, int basinNum, int xInd int yInd, int zInd, gridStruct *location, velo1D *subModel1D);
extern void loadAllBasinSurfaces(int basinNum, basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS);
//extern void assignBasinProperties(gridStruct *location, int basinNum, globalBasinData *basinData);
//extern basin_data *loadBasinData(global_model_parameters *GLOBAL_MODEL_PARAMETERS);

// math functions
extern int isValueInArray(int val, int *arr, int size);
extern int isValueInTwoArrays(int valA, int valB, int *arrA, int *arrB, int size);
extern int findTwinInds(int valA, int valB, int *arrA, int *arrB, int size);
extern double LatLonToDistance(double locationLatLon[], double originLat, double originLon);
extern singleGridPoint XYtoLatLon(double locationXY[], double originLat, double originLon, double originRot);
extern singleGridPoint pointRadialDistance(double refLatLon[],double bearing, double distance);
extern double rad2deg(double angRad);
extern double deg2rad(double angDeg);
extern singleGridPoint rotateGrid(double rotAngle, double X, double Y);
extern float float_swap(const float inFloat);
extern int endian(void);
extern double findMinValueArray(int nPts, double pts[5]);
extern double findMaxValueArray(int nPts, double pts[5]);
extern double interpolateQuad(double lons[4], double lats[4], double values[4], double latA, double lonA);
extern void gcproj(double xf,double yf,double *rlon,double *rlat,double ref_rad,double g0,double b0,double amat[8],double ainv[8]);
extern void calcAndSaveVs(char *OUTPUT_DIR, partial_global_mesh *PARTIAL_GLOBAL_MESH, partial_global_qualities *PARTIAL_GLOBAL_QUALITIES, calculation_log *CALCULATION_LOG, char *VS_DEPTH, int latInd);
extern void writeVsFile(char *OUTPUT_DIR, double Lat, double Lon, double VsTotal, double latInd, char *VS_DEPTH);
extern void writeZThresholdFile(char *OUTPUT_DIR, double Lat, double Lon, double Z_WRITE, double latInd, char *Z_THRESHOLD);
void calcAndSaveZThreshold(char *OUTPUT_DIR, partial_global_mesh *PARTIAL_GLOBAL_MESH, partial_global_qualities *PARTIAL_GLOBAL_QUALITIES, calculation_log *CALCULATION_LOG, char *Z_THRESHOLD, int latInd);










extern calculation_log *initializeCalculationLog(int argc, char *argv[]);





#endif
