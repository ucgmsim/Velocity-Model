//
//  callRoutines.c
//  UCVM-Integration
//
//  Created by Ethan Thomson on 10/07/16.
//  Copyright © 2016 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "omp.h"

#include "constants.h"
#include "structs.h"
#include "functions.h"

void runGenerateVelocitySlices(char *MODEL_VERSION, char *OUTPUT_DIR, gen_velo_slices_call GEN_VELO_SLICES_CALL, calculation_log *CALCULATION_LOG)
{
    slice_parameters *SLICE_PARAMETERS;
    SLICE_PARAMETERS = readGeneratedSliceParametersFile(GEN_VELO_SLICES_CALL.GENERATED_SLICE_PARAMETERS_TEXTFILE);
    
    model_extent *MODEL_EXTENT = malloc(sizeof(model_extent));
    if (MODEL_EXTENT == NULL)
    {
        printf("Memory allocation of MODEL_EXTENT failed.\n");
        exit(EXIT_FAILURE);
    }
    MODEL_EXTENT->version = MODEL_VERSION;
    global_mesh *GLOBAL_MESH;
    
    
    
    // obtain surface filenames based off version number
    global_model_parameters *GLOBAL_MODEL_PARAMETERS;
    GLOBAL_MODEL_PARAMETERS = getGlobalModelParameters(MODEL_EXTENT->version);
    
    mesh_vector *MESH_VECTOR;
    qualities_vector *QUALITIES_VECTOR;
    partial_global_qualities *PARTIAL_GLOBAL_QUALITIES;
    
    // read in velocity model data (surfaces, 1D models, tomography etc)
    velo_mod_1d_data *VELO_MOD_1D_DATA;
    VELO_MOD_1D_DATA = malloc(sizeof(velo_mod_1d_data));
    if (VELO_MOD_1D_DATA == NULL)
    {
        printf("Memory allocation of VELO_MOD_1D_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    
    nz_tomography_data *NZ_TOMOGRAPHY_DATA;
    NZ_TOMOGRAPHY_DATA = malloc(sizeof(nz_tomography_data));
    if (NZ_TOMOGRAPHY_DATA == NULL)
    {
        printf("Memory allocation of NZ_TOMOGRAPHY_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    
    global_surfaces *GLOBAL_SURFACES;
    GLOBAL_SURFACES = malloc(sizeof(global_surfaces));
    if (GLOBAL_SURFACES == NULL)
    {
        printf("Memory allocation of GLOBAL_SURFACES failed.\n");
        exit(EXIT_FAILURE);
    }
    
    basin_data *BASIN_DATA;
    BASIN_DATA = malloc(sizeof(basin_data));
    if (BASIN_DATA == NULL)
    {
        printf("Memory allocation of BASIN_DATA failed.\n");
        exit(EXIT_FAILURE);
    }

    partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS;
    partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS;
    in_basin *IN_BASIN;
    slice_surface_depths *SLICE_SURFACE_DEPTHS;
    // Load Data
    loadAllGlobalData(GLOBAL_MODEL_PARAMETERS, CALCULATION_LOG, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA);

    // Loop over grid points and assign values
    for(int j = 0; j < SLICE_PARAMETERS->nSlices; j++)
    {
        individual_slice_parameters INDIVIDUAL_SLICE_PARAMETERS;
        
        INDIVIDUAL_SLICE_PARAMETERS.latPtsSlice[0] = SLICE_PARAMETERS->latA[j];
        INDIVIDUAL_SLICE_PARAMETERS.latPtsSlice[1] = SLICE_PARAMETERS->latB[j];
        INDIVIDUAL_SLICE_PARAMETERS.lonPtsSlice[0] = SLICE_PARAMETERS->lonA[j];
        INDIVIDUAL_SLICE_PARAMETERS.lonPtsSlice[1] = SLICE_PARAMETERS->lonB[j];
        
        INDIVIDUAL_SLICE_PARAMETERS.resZ = SLICE_PARAMETERS->DepRes[j];
        INDIVIDUAL_SLICE_PARAMETERS.resXY = SLICE_PARAMETERS->LatLonRes[j];
        INDIVIDUAL_SLICE_PARAMETERS.zMin = SLICE_PARAMETERS->depMin[j];
        INDIVIDUAL_SLICE_PARAMETERS.zMax = SLICE_PARAMETERS->depMax[j];

        partial_global_mesh *PARTIAL_GLOBAL_MESH;
        PARTIAL_GLOBAL_MESH = generateSlicePartialMesh(INDIVIDUAL_SLICE_PARAMETERS);
        PARTIAL_GLOBAL_QUALITIES = malloc(sizeof(partial_global_qualities));
        if (PARTIAL_GLOBAL_QUALITIES == NULL)
        {
            printf("Memory allocation of PARTIAL_GLOBAL_QUALITIES failed.\n");
            exit(EXIT_FAILURE);
        }
        SLICE_SURFACE_DEPTHS = malloc(sizeof(slice_surface_depths));
        if (SLICE_SURFACE_DEPTHS == NULL)
        {
            printf("Memory allocation of SLICE_SURFACE_DEPTHS failed.\n");
            exit(EXIT_FAILURE);
        }

        for(int k = 0; k < PARTIAL_GLOBAL_MESH->nX; k++)
        {
            IN_BASIN = malloc(sizeof(in_basin));
            if (IN_BASIN == NULL)
            {
                printf("Memory allocation of IN_BASIN failed.\n");
                exit(EXIT_FAILURE);
            }
            PARTIAL_GLOBAL_SURFACE_DEPTHS = malloc(sizeof(partial_global_surface_depths));
            if (PARTIAL_GLOBAL_SURFACE_DEPTHS == NULL)
            {
                printf("Memory allocation of PARTIAL_GLOBAL_SURFACE_DEPTHS failed.\n");
                exit(EXIT_FAILURE);
            }
            PARTIAL_BASIN_SURFACE_DEPTHS = malloc(sizeof(partial_basin_surface_depths));
            if (PARTIAL_BASIN_SURFACE_DEPTHS == NULL)
            {
                printf("Memory allocation of PARTIAL_BASIN_SURFACE_DEPTHS failed.\n");
                exit(EXIT_FAILURE);
            }
            QUALITIES_VECTOR = malloc(sizeof(qualities_vector));
            if (QUALITIES_VECTOR == NULL)
            {
                printf("Memory allocation of QUALITIES_VECTOR failed.\n");
                exit(EXIT_FAILURE);
            }
            
            
            MESH_VECTOR = extractMeshVector(PARTIAL_GLOBAL_MESH, k);
            
            assignQualities(GLOBAL_MODEL_PARAMETERS, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA, MESH_VECTOR, PARTIAL_GLOBAL_SURFACE_DEPTHS, PARTIAL_BASIN_SURFACE_DEPTHS, IN_BASIN, QUALITIES_VECTOR, CALCULATION_LOG, GEN_VELO_SLICES_CALL.TOPO_TYPE);
            for(int i = 0; i < PARTIAL_GLOBAL_MESH->nZ; i++)
            {
                PARTIAL_GLOBAL_QUALITIES->Rho[k][i] = QUALITIES_VECTOR->Rho[i];
                PARTIAL_GLOBAL_QUALITIES->Vp[k][i] = QUALITIES_VECTOR->Vp[i];
                PARTIAL_GLOBAL_QUALITIES->Vs[k][i] = QUALITIES_VECTOR->Vs[i];
                //                printf("%lf %lf %lf.\n",PARTIAL_GLOBAL_QUALITIES->Vp[i], PARTIAL_GLOBAL_QUALITIES->Vs[i],PARTIAL_GLOBAL_QUALITIES->Rho[i]);
                //                printf("%lf %lf %lf.\n\n",QUALITIES_VECTOR->Vp[i],QUALITIES_VECTOR->Vs[i],QUALITIES_VECTOR->Rho[i]);
                
                
            }
            for(int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasins; i++)
            {
                for(int j = 0; j < GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[i]; j++)
                {
                    SLICE_SURFACE_DEPTHS->basinSurfdep[i][j][k] = PARTIAL_BASIN_SURFACE_DEPTHS->dep[i][j];
                }
                
            }
            for(int i = 0; i < GLOBAL_MODEL_PARAMETERS->nSurf; i++)
            {
                SLICE_SURFACE_DEPTHS->globSurfdep[i][k] = PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[i];
            }


            free(MESH_VECTOR);
            free(QUALITIES_VECTOR);
            free(PARTIAL_BASIN_SURFACE_DEPTHS);
            free(PARTIAL_GLOBAL_SURFACE_DEPTHS);
            free(IN_BASIN);
        }
        writeGeneratedSlice(OUTPUT_DIR, PARTIAL_GLOBAL_MESH, PARTIAL_GLOBAL_QUALITIES, &INDIVIDUAL_SLICE_PARAMETERS,CALCULATION_LOG, j);
        writeSliceSurfaceDepths(GLOBAL_MODEL_PARAMETERS,PARTIAL_GLOBAL_MESH, OUTPUT_DIR, SLICE_SURFACE_DEPTHS);
        printf("Slice %i of %i complete.\n",j+1,SLICE_PARAMETERS->nSlices);
        free(SLICE_SURFACE_DEPTHS);
        free(PARTIAL_GLOBAL_MESH);
        free(PARTIAL_GLOBAL_QUALITIES);
    }
    
    writeSliceParametersLogFile(OUTPUT_DIR, SLICE_PARAMETERS, MODEL_EXTENT, GLOBAL_MESH, CALCULATION_LOG, "GENERATED");
    free(VELO_MOD_1D_DATA);
    freeEPtomoSurfaceData(NZ_TOMOGRAPHY_DATA);
    free(NZ_TOMOGRAPHY_DATA);
    freeGlobalSurfaceData(GLOBAL_SURFACES, GLOBAL_MODEL_PARAMETERS);
    free(GLOBAL_SURFACES);
    freeAllBasinSurfaces(BASIN_DATA, GLOBAL_MODEL_PARAMETERS);
    free(BASIN_DATA);
    
}

void runGenerateMultipleProfiles(char *MODEL_VERSION, char *OUTPUT_DIR, gen_multi_profiles_call GEN_MULTI_PROFILES_CALL, calculation_log *CALCULATION_LOG)
{
    // Read in text file with profile parameters
    multi_profile_parameters *MULTI_PROFILE_PARAMETERS;
    MULTI_PROFILE_PARAMETERS = readProfilesTextFile(GEN_MULTI_PROFILES_CALL.COORDINATES_TEXTFILE);
    variable_depth_points *VARIABLE_DEPTH_POINTS;

    if (strcmp(GEN_MULTI_PROFILES_CALL.SPACING_TYPE,"VARIABLE") == 0)
    {
        VARIABLE_DEPTH_POINTS = readDepthPointsTextFile(GEN_MULTI_PROFILES_CALL.PROFILE_DEPTHS_TEXTFILE);
    }

    double half = 0.5;
    // obtain surface filenames based off version number
    global_model_parameters *GLOBAL_MODEL_PARAMETERS;
    GLOBAL_MODEL_PARAMETERS = getGlobalModelParameters(MODEL_VERSION);

    // read in velocity model data (surfaces, 1D models, tomography etc)
    velo_mod_1d_data *VELO_MOD_1D_DATA;
    VELO_MOD_1D_DATA = malloc(sizeof(velo_mod_1d_data));
    if (VELO_MOD_1D_DATA == NULL)
    {
        printf("Memory allocation of VELO_MOD_1D_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    nz_tomography_data *NZ_TOMOGRAPHY_DATA;
    NZ_TOMOGRAPHY_DATA = malloc(sizeof(nz_tomography_data));
    if (NZ_TOMOGRAPHY_DATA == NULL)
    {
        printf("Memory allocation of NZ_TOMOGRAPHY_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    global_surfaces *GLOBAL_SURFACES;
    GLOBAL_SURFACES = malloc(sizeof(global_surfaces));
    if (GLOBAL_SURFACES == NULL)
    {
        printf("Memory allocation of GLOBAL_SURFACES failed.\n");
        exit(EXIT_FAILURE);
    }
    basin_data *BASIN_DATA;
    BASIN_DATA = malloc(sizeof(basin_data));
    if (BASIN_DATA == NULL)
    {
        printf("Memory allocation of BASIN_DATA failed.\n");
        exit(EXIT_FAILURE);
    }

    loadAllGlobalData(GLOBAL_MODEL_PARAMETERS, CALCULATION_LOG, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA);

    // loop over nProfiles
    for( int i = 0; i < MULTI_PROFILE_PARAMETERS->nProfiles; i++ )
    {
        printf("Generating profile %i of %i.\n",i+1, MULTI_PROFILE_PARAMETERS->nProfiles);
        model_extent *MODEL_EXTENT = malloc(sizeof(model_extent));
        MODEL_EXTENT->version = MODEL_VERSION;
        MODEL_EXTENT->originRot = 0;
        MODEL_EXTENT->Xmax = 1;
        MODEL_EXTENT->Ymax = 1;
        MODEL_EXTENT->hLatLon = 1;
        MODEL_EXTENT->originLat = MULTI_PROFILE_PARAMETERS->lats[i];
        MODEL_EXTENT->originLon = MULTI_PROFILE_PARAMETERS->lons[i];


        if (strcmp(GEN_MULTI_PROFILES_CALL.SPACING_TYPE,"CONSTANT") == 0)
        {
            MODEL_EXTENT->Zmax = GEN_MULTI_PROFILES_CALL.PROFILE_ZMAX + half*GEN_MULTI_PROFILES_CALL.SPACING_PROFILE; // max depth (positive downwards)
            MODEL_EXTENT->Zmin = GEN_MULTI_PROFILES_CALL.PROFILE_ZMIN - half*GEN_MULTI_PROFILES_CALL.SPACING_PROFILE;
            MODEL_EXTENT->hDep = GEN_MULTI_PROFILES_CALL.SPACING_PROFILE;
        }
        else if (strcmp(GEN_MULTI_PROFILES_CALL.SPACING_TYPE,"VARIABLE") == 0)
        {
            // insert place holder values to be wirtten over once variable depth points have been read from file
            MODEL_EXTENT->Zmax = 1;
            MODEL_EXTENT->Zmin = 0;
            MODEL_EXTENT->hDep = 1;
        }
        // generate the model grid
        global_mesh *GLOBAL_MESH;
        GLOBAL_MESH = malloc(sizeof(global_mesh));
        if (GLOBAL_MESH == NULL)
        {
            printf("Memory allocation of GLOBAL_MESH failed.\n");
            exit(EXIT_FAILURE);
        }
        generateFullModelGridGreatCircle(MODEL_EXTENT, GLOBAL_MESH);

        if (strcmp(GEN_MULTI_PROFILES_CALL.SPACING_TYPE,"VARIABLE") == 0)
        {
            GLOBAL_MESH->nZ = VARIABLE_DEPTH_POINTS->nDep;
            printf("Number of model points. nx: %i, ny: %i, nz: %i.\n", GLOBAL_MESH->nX, GLOBAL_MESH->nY, GLOBAL_MESH->nZ);
            for ( int j = 0; j < VARIABLE_DEPTH_POINTS->nDep; j++)
            {
                GLOBAL_MESH->Z[j] = -1000*VARIABLE_DEPTH_POINTS->dep[j];
            }
        }

        partial_global_mesh *PARTIAL_GLOBAL_MESH;
        mesh_vector *MESH_VECTOR;


        in_basin *IN_BASIN;
        IN_BASIN = malloc(sizeof(in_basin));
        if (IN_BASIN == NULL)
        {
            printf("Memory allocation of IN_BASIN failed.\n");
            exit(EXIT_FAILURE);
        }



        PARTIAL_GLOBAL_MESH = extractPartialMesh(GLOBAL_MESH, 0);
        MESH_VECTOR = extractMeshVector(PARTIAL_GLOBAL_MESH, 0);

        partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS;
        PARTIAL_GLOBAL_SURFACE_DEPTHS = malloc(sizeof(partial_global_surface_depths));
        if (PARTIAL_GLOBAL_SURFACE_DEPTHS == NULL)
        {
            printf("Memory allocation of PARTIAL_GLOBAL_SURFACE_DEPTHS failed.\n");
            exit(EXIT_FAILURE);
        }

        partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS;
        PARTIAL_BASIN_SURFACE_DEPTHS = malloc(sizeof(partial_basin_surface_depths));
        if (PARTIAL_BASIN_SURFACE_DEPTHS == NULL)
        {
            printf("Memory allocation of PARTIAL_BASIN_SURFACE_DEPTHS failed.\n");
            exit(EXIT_FAILURE);
        }

        qualities_vector *QUALITIES_VECTOR;
        QUALITIES_VECTOR = malloc(sizeof(qualities_vector));
        if (QUALITIES_VECTOR == NULL)
        {
            printf("Memory allocation of QUALITIES_VECTOR failed.\n");
            exit(EXIT_FAILURE);
        }
//        loadAllGlobalData(GLOBAL_MODEL_PARAMETERS, CALCULATION_LOG, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA);
        assignQualities(GLOBAL_MODEL_PARAMETERS, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA, MESH_VECTOR, PARTIAL_GLOBAL_SURFACE_DEPTHS, PARTIAL_BASIN_SURFACE_DEPTHS, IN_BASIN, QUALITIES_VECTOR, CALCULATION_LOG, GEN_MULTI_PROFILES_CALL.TOPO_TYPE);
        writeMultipleProfiles(QUALITIES_VECTOR, GEN_MULTI_PROFILES_CALL, MESH_VECTOR, OUTPUT_DIR, CALCULATION_LOG,i);
        writeMultipleProfileSurfaceDepths(GLOBAL_MODEL_PARAMETERS, BASIN_DATA, PARTIAL_GLOBAL_SURFACE_DEPTHS, PARTIAL_BASIN_SURFACE_DEPTHS, IN_BASIN, MESH_VECTOR, OUTPUT_DIR, CALCULATION_LOG,i);
        printf("Profile %i of %i complete.\n",i+1, MULTI_PROFILE_PARAMETERS->nProfiles);
        free(GLOBAL_MESH);
        free(IN_BASIN);
        free(PARTIAL_BASIN_SURFACE_DEPTHS);
        free(PARTIAL_GLOBAL_SURFACE_DEPTHS);
        free(MESH_VECTOR);
        free(QUALITIES_VECTOR);
    }
    free(VELO_MOD_1D_DATA);
    freeEPtomoSurfaceData(NZ_TOMOGRAPHY_DATA);
    free(NZ_TOMOGRAPHY_DATA);
    free(GLOBAL_SURFACES);
    freeGlobalSurfaceData(GLOBAL_SURFACES, GLOBAL_MODEL_PARAMETERS);
    freeAllBasinSurfaces(BASIN_DATA, GLOBAL_MODEL_PARAMETERS);
    free(BASIN_DATA);
    if (strcmp(GEN_MULTI_PROFILES_CALL.SPACING_TYPE,"VARIABLE") == 0)
    {
        free(VARIABLE_DEPTH_POINTS);
    }
    free(MULTI_PROFILE_PARAMETERS);
}


void runGenerateMultipleVSonGrid(char *MODEL_VERSION, char *OUTPUT_DIR, gen_extract_multi_gridpoint_vs_call GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL, calculation_log *CALCULATION_LOG)
{
    // Read in text file with parameters
    multi_gridpoint_parameters *MULTI_GRIDPOINT_PARAMETERS;
    MULTI_GRIDPOINT_PARAMETERS = readGridpointsTextFile(GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL.COORDINATES_TEXT_FILE);
    variable_depth_points *VARIABLE_DEPTH_POINTS;

    // obtain surface filenames based off version number
    global_model_parameters *GLOBAL_MODEL_PARAMETERS;
    GLOBAL_MODEL_PARAMETERS = getGlobalModelParameters(MODEL_VERSION);

    // read in velocity model data (surfaces, 1D models, tomography etc)
    velo_mod_1d_data *VELO_MOD_1D_DATA;
    VELO_MOD_1D_DATA = malloc(sizeof(velo_mod_1d_data));
    if (VELO_MOD_1D_DATA == NULL)
    {
        printf("Memory allocation of VELO_MOD_1D_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    nz_tomography_data *NZ_TOMOGRAPHY_DATA;
    NZ_TOMOGRAPHY_DATA = malloc(sizeof(nz_tomography_data));
    if (NZ_TOMOGRAPHY_DATA == NULL)
    {
        printf("Memory allocation of NZ_TOMOGRAPHY_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    global_surfaces *GLOBAL_SURFACES;
    GLOBAL_SURFACES = malloc(sizeof(global_surfaces));
    if (GLOBAL_SURFACES == NULL)
    {
        printf("Memory allocation of GLOBAL_SURFACES failed.\n");
        exit(EXIT_FAILURE);
    }
    basin_data *BASIN_DATA;
    BASIN_DATA = malloc(sizeof(basin_data));
    if (BASIN_DATA == NULL)
    {
        printf("Memory allocation of BASIN_DATA failed.\n");
        exit(EXIT_FAILURE);
    }

    loadAllGlobalData(GLOBAL_MODEL_PARAMETERS, CALCULATION_LOG, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA);
    mesh_vector *MESH_VECTOR;
    MESH_VECTOR = malloc(sizeof(mesh_vector));
    if (MESH_VECTOR == NULL)
    {
        printf("Memory allocation of MESH_VECTOR failed.\n");
        exit(EXIT_FAILURE);
    }


    int grdPtsCount = 0;
    // loop over gridpoints
    for( int i = 0; i < MULTI_GRIDPOINT_PARAMETERS->nGroupings; i++ )
    {
        MESH_VECTOR->nZ = 0;
        while (MULTI_GRIDPOINT_PARAMETERS->grouping[grdPtsCount] == i)
        {
            MESH_VECTOR->Z[MESH_VECTOR->nZ] = MULTI_GRIDPOINT_PARAMETERS->dep[grdPtsCount];
            MESH_VECTOR->nZ += 1;
            grdPtsCount += 1;
        }
        MESH_VECTOR->Lat = &MULTI_GRIDPOINT_PARAMETERS->lat[i];
        MESH_VECTOR->Lon = &MULTI_GRIDPOINT_PARAMETERS->lon[i];


        in_basin *IN_BASIN;
        IN_BASIN = malloc(sizeof(in_basin));
        if (IN_BASIN == NULL) {
            printf("Memory allocation of IN_BASIN failed.\n");
            exit(EXIT_FAILURE);
        }

        partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS;
        PARTIAL_GLOBAL_SURFACE_DEPTHS = malloc(sizeof(partial_global_surface_depths));
        if (PARTIAL_GLOBAL_SURFACE_DEPTHS == NULL) {
            printf("Memory allocation of PARTIAL_GLOBAL_SURFACE_DEPTHS failed.\n");
            exit(EXIT_FAILURE);
        }

        partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS;
        PARTIAL_BASIN_SURFACE_DEPTHS = malloc(sizeof(partial_basin_surface_depths));
        if (PARTIAL_BASIN_SURFACE_DEPTHS == NULL) {
            printf("Memory allocation of PARTIAL_BASIN_SURFACE_DEPTHS failed.\n");
            exit(EXIT_FAILURE);
        }

        qualities_vector *QUALITIES_VECTOR;
        QUALITIES_VECTOR = malloc(sizeof(qualities_vector));
        if (QUALITIES_VECTOR == NULL) {
            printf("Memory allocation of QUALITIES_VECTOR failed.\n");
            exit(EXIT_FAILURE);
        }
        assignQualities(GLOBAL_MODEL_PARAMETERS, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA,
                        MESH_VECTOR, PARTIAL_GLOBAL_SURFACE_DEPTHS, PARTIAL_BASIN_SURFACE_DEPTHS, IN_BASIN,
                        QUALITIES_VECTOR, CALCULATION_LOG, GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL.TOPO_TYPE);
        writeGridpointVelocities(QUALITIES_VECTOR, GEN_EXTRACT_MULTI_GRIDPOINT_VS_CALL, MESH_VECTOR, OUTPUT_DIR, CALCULATION_LOG, i);

        printf("Gridpoint lat-lon %i of %i complete.\n", i + 1, MULTI_GRIDPOINT_PARAMETERS->nGroupings);


        free(IN_BASIN);
        free(PARTIAL_BASIN_SURFACE_DEPTHS);
        free(PARTIAL_GLOBAL_SURFACE_DEPTHS);
        free(MESH_VECTOR);
        free(QUALITIES_VECTOR);

        MESH_VECTOR = malloc(sizeof(mesh_vector));
        if (MESH_VECTOR == NULL)
        {
            printf("Memory allocation of MESH_VECTOR failed.\n");
            exit(EXIT_FAILURE);
        }
    }
//    free(VELO_MOD_1D_DATA);
//    freeEPtomoSurfaceData(NZ_TOMOGRAPHY_DATA);
//    free(NZ_TOMOGRAPHY_DATA);
//    free(GLOBAL_SURFACES);
//    freeGlobalSurfaceData(GLOBAL_SURFACES, GLOBAL_MODEL_PARAMETERS);
//    freeAllBasinSurfaces(BASIN_DATA, GLOBAL_MODEL_PARAMETERS);
//    free(BASIN_DATA);
//
//    free(MULTI_GRIDPOINT_PARAMETERS);
}



void runGenerateProfile(char *MODEL_VERSION, char *OUTPUT_DIR, gen_profile_call GEN_PROFILE_CALL, calculation_log *CALCULATION_LOG)
{
    
    model_extent *MODEL_EXTENT = malloc(sizeof(model_extent));
    MODEL_EXTENT->version = MODEL_VERSION;
    
    // Model origin paremeters
    MODEL_EXTENT->originLat = GEN_PROFILE_CALL.PROFILE_LAT;
    MODEL_EXTENT->originLon = GEN_PROFILE_CALL.PROFILE_LON;
    MODEL_EXTENT->originRot = 0;
    
    // Model extent parameters
    double half = 0.5;
    MODEL_EXTENT->Xmax = 1;
    MODEL_EXTENT->Ymax = 1;
    MODEL_EXTENT->Zmax = GEN_PROFILE_CALL.PROFILE_ZMAX + half*GEN_PROFILE_CALL.EXTENT_Z_SPACING_PROFILE; // max depth (positive downwards)
    MODEL_EXTENT->Zmin = GEN_PROFILE_CALL.PROFILE_ZMIN - half*GEN_PROFILE_CALL.EXTENT_Z_SPACING_PROFILE;
    MODEL_EXTENT->hDep = GEN_PROFILE_CALL.EXTENT_Z_SPACING_PROFILE;
    MODEL_EXTENT->hLatLon = 1;
    
    // generate the model grid
    global_mesh *GLOBAL_MESH;
    GLOBAL_MESH = malloc(sizeof(global_mesh));
    if (GLOBAL_MESH == NULL)
    {
        printf("Memory allocation of GLOBAL_MESH failed.\n");
        exit(EXIT_FAILURE);
    }
    generateFullModelGridGreatCircle(MODEL_EXTENT, GLOBAL_MESH);

    // obtain surface filenames based off version number
    global_model_parameters *GLOBAL_MODEL_PARAMETERS;
    GLOBAL_MODEL_PARAMETERS = getGlobalModelParameters(MODEL_EXTENT->version);
    
    partial_global_mesh *PARTIAL_GLOBAL_MESH;
    mesh_vector *MESH_VECTOR;
    
    // read in velocity model data (surfaces, 1D models, tomography etc)
    velo_mod_1d_data *VELO_MOD_1D_DATA;
    VELO_MOD_1D_DATA = malloc(sizeof(velo_mod_1d_data));
    if (VELO_MOD_1D_DATA == NULL)
    {
        printf("Memory allocation of VELO_MOD_1D_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    nz_tomography_data *NZ_TOMOGRAPHY_DATA;
    NZ_TOMOGRAPHY_DATA = malloc(sizeof(nz_tomography_data));
    if (NZ_TOMOGRAPHY_DATA == NULL)
    {
        printf("Memory allocation of NZ_TOMOGRAPHY_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    
    global_surfaces *GLOBAL_SURFACES;
    GLOBAL_SURFACES = malloc(sizeof(global_surfaces));
    if (GLOBAL_SURFACES == NULL)
    {
        printf("Memory allocation of GLOBAL_SURFACES failed.\n");
        exit(EXIT_FAILURE);
    }
    basin_data *BASIN_DATA;
    BASIN_DATA = malloc(sizeof(basin_data));
    if (BASIN_DATA == NULL)
    {
        printf("Memory allocation of BASIN_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    in_basin *IN_BASIN;
    IN_BASIN = malloc(sizeof(in_basin));
    if (IN_BASIN == NULL)
    {
        printf("Memory allocation of IN_BASIN failed.\n");
        exit(EXIT_FAILURE);
    }
    
    
    
    PARTIAL_GLOBAL_MESH = extractPartialMesh(GLOBAL_MESH, 0);
    MESH_VECTOR = extractMeshVector(PARTIAL_GLOBAL_MESH, 0);
    
    partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS;
    PARTIAL_GLOBAL_SURFACE_DEPTHS = malloc(sizeof(partial_global_surface_depths));
    if (PARTIAL_GLOBAL_SURFACE_DEPTHS == NULL)
    {
        printf("Memory allocation of PARTIAL_GLOBAL_SURFACE_DEPTHS failed.\n");
        exit(EXIT_FAILURE);
    }
    
    partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS;
    PARTIAL_BASIN_SURFACE_DEPTHS = malloc(sizeof(partial_basin_surface_depths));
    if (PARTIAL_BASIN_SURFACE_DEPTHS == NULL)
    {
        printf("Memory allocation of PARTIAL_BASIN_SURFACE_DEPTHS failed.\n");
        exit(EXIT_FAILURE);
    }
    
    qualities_vector *QUALITIES_VECTOR;
    QUALITIES_VECTOR = malloc(sizeof(qualities_vector));
    if (QUALITIES_VECTOR == NULL)
    {
        printf("Memory allocation of QUALITIES_VECTOR failed.\n");
        exit(EXIT_FAILURE);
    }
    loadAllGlobalData(GLOBAL_MODEL_PARAMETERS, CALCULATION_LOG, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA);
    assignQualities(GLOBAL_MODEL_PARAMETERS, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA, MESH_VECTOR, PARTIAL_GLOBAL_SURFACE_DEPTHS, PARTIAL_BASIN_SURFACE_DEPTHS, IN_BASIN, QUALITIES_VECTOR, CALCULATION_LOG, GEN_PROFILE_CALL.TOPO_TYPE);
    writeIndividualProfile(QUALITIES_VECTOR, GEN_PROFILE_CALL, MESH_VECTOR, OUTPUT_DIR, CALCULATION_LOG);
    writeProfileSurfaceDepths(GLOBAL_MODEL_PARAMETERS, BASIN_DATA, PARTIAL_GLOBAL_SURFACE_DEPTHS, PARTIAL_BASIN_SURFACE_DEPTHS, IN_BASIN, MESH_VECTOR, OUTPUT_DIR, CALCULATION_LOG);
    
    free(VELO_MOD_1D_DATA);
    freeEPtomoSurfaceData(NZ_TOMOGRAPHY_DATA);
    free(NZ_TOMOGRAPHY_DATA);
    free(GLOBAL_MESH);
    freeGlobalSurfaceData(GLOBAL_SURFACES, GLOBAL_MODEL_PARAMETERS);
    free(GLOBAL_SURFACES);
    freeAllBasinSurfaces(BASIN_DATA, GLOBAL_MODEL_PARAMETERS);
    free(BASIN_DATA);
    free(IN_BASIN);
    free(PARTIAL_BASIN_SURFACE_DEPTHS);
    free(PARTIAL_GLOBAL_SURFACE_DEPTHS);
    free(MESH_VECTOR);
    free(QUALITIES_VECTOR);
    
}

void runExtractFromVelocityModel(char *MODEL_VERSION, char *OUTPUT_DIR, gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL, calculation_log *CALCULATION_LOG)
{
    model_extent *MODEL_EXTENT = malloc(sizeof(model_extent));
    if (MODEL_EXTENT == NULL)
    {
        printf("Memory allocation of MODEL_EXTENT failed.\n");
        exit(EXIT_FAILURE);
    }
    MODEL_EXTENT->version = MODEL_VERSION;
    
    // Model origin paremeters
    MODEL_EXTENT->originLat = GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LAT;
    MODEL_EXTENT->originLon = GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LON;
    MODEL_EXTENT->originRot = GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_ROT;
    
    // Model extent parameters
    MODEL_EXTENT->Xmax = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_X;
    MODEL_EXTENT->Ymax = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Y;
    MODEL_EXTENT->Zmax = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_ZMAX; // max depth (positive downwards)
    MODEL_EXTENT->Zmin = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_ZMIN;
    MODEL_EXTENT->hDep = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Z_SPACING;
    MODEL_EXTENT->hLatLon = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_LATLON_SPACING;
    
    // generate the model grid
    global_mesh *GLOBAL_MESH;
    GLOBAL_MESH = malloc(sizeof(global_mesh));
    if (GLOBAL_MESH == NULL)
    {
        printf("Memory allocation of GLOBAL_MESH failed.\n");
        exit(EXIT_FAILURE);
    }
    generateFullModelGridGreatCircle(MODEL_EXTENT, GLOBAL_MESH);
    
    printf("Extracting slices.\n");
    extractSlicesFromBinaryFiles(OUTPUT_DIR, GEN_EXTRACT_VELO_MOD_CALL, CALCULATION_LOG, GLOBAL_MESH, MODEL_EXTENT);
    
    free(GLOBAL_MESH);
    free(MODEL_EXTENT);
}


void runThresholdVelocityModel(char *MODEL_VERSION, char *OUTPUT_DIR, gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL, calculation_log *CALCULATION_LOG)
{
    model_extent *MODEL_EXTENT = malloc(sizeof(model_extent));
    if (MODEL_EXTENT == NULL)
    {
        printf("Memory allocation of MODEL_EXTENT failed.\n");
        exit(EXIT_FAILURE);
    }
    MODEL_EXTENT->version = MODEL_VERSION;
    
    // Model origin paremeters
    MODEL_EXTENT->originLat = GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LAT;
    MODEL_EXTENT->originLon = GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LON;
    MODEL_EXTENT->originRot = GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_ROT;
    
    // Model extent parameters
    MODEL_EXTENT->Xmax = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_X;
    MODEL_EXTENT->Ymax = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Y;
    MODEL_EXTENT->hLatLon = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_LATLON_SPACING;
    char *Z_THRESH = "";
    
    
    if (strcmp(GEN_EXTRACT_VELO_MOD_CALL.VS_TYPE, "VS500") == 0)
    {
        MODEL_EXTENT->Zmax = 0.505;
        MODEL_EXTENT->Zmin = -0.005;
        MODEL_EXTENT->hDep = 0.01;
    }
    else if (strcmp(GEN_EXTRACT_VELO_MOD_CALL.VS_TYPE, "VS30") == 0)
    {
        MODEL_EXTENT->Zmax = 0.0305;
        MODEL_EXTENT->Zmin = -0.0005;
        MODEL_EXTENT->hDep = 0.001;
    }
    else if (strcmp(GEN_EXTRACT_VELO_MOD_CALL.VS_TYPE, "Z1.0") == 0)
    {
        MODEL_EXTENT->Zmax = 2; // 2km downwards
        MODEL_EXTENT->Zmin = 0;
        MODEL_EXTENT->hDep = 0.01;
        Z_THRESH = "1.0";
    }
    else if (strcmp(GEN_EXTRACT_VELO_MOD_CALL.VS_TYPE, "Z2.5") == 0)
    {
        MODEL_EXTENT->Zmax = 12; // 10km downwards
        MODEL_EXTENT->Zmin = 0;
        MODEL_EXTENT->hDep = 0.05;
        Z_THRESH = "2.5";
    }
    else
    {
        printf("Vs type '%s' not recognised, see readme.\n",GEN_EXTRACT_VELO_MOD_CALL.VS_TYPE);
        exit(EXIT_FAILURE);
    }

    
    // generate the model grid
    global_mesh *GLOBAL_MESH;
    GLOBAL_MESH = malloc(sizeof(global_mesh));
    if (GLOBAL_MESH == NULL)
    {
        printf("Memory allocation of GLOBAL_MESH failed.\n");
        exit(EXIT_FAILURE);
    }
    generateFullModelGridGreatCircle(MODEL_EXTENT, GLOBAL_MESH);
    
    MODEL_EXTENT->version = MODEL_VERSION;
    
    // obtain surface filenames based off version number
    global_model_parameters *GLOBAL_MODEL_PARAMETERS;
    GLOBAL_MODEL_PARAMETERS = getGlobalModelParameters(MODEL_EXTENT->version);
    
    partial_global_mesh *PARTIAL_GLOBAL_MESH;
    mesh_vector *MESH_VECTOR;
    qualities_vector *QUALITIES_VECTOR;
    partial_global_qualities *PARTIAL_GLOBAL_QUALITIES;
    
    // read in velocity model data (surfaces, 1D models, tomography etc)
    velo_mod_1d_data *VELO_MOD_1D_DATA;
    VELO_MOD_1D_DATA = malloc(sizeof(velo_mod_1d_data));
    if (VELO_MOD_1D_DATA == NULL)
    {
        printf("Memory allocation of VELO_MOD_1D_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    
    nz_tomography_data *NZ_TOMOGRAPHY_DATA;
    NZ_TOMOGRAPHY_DATA = malloc(sizeof(nz_tomography_data));
    if (NZ_TOMOGRAPHY_DATA == NULL)
    {
        printf("Memory allocation of NZ_TOMOGRAPHY_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    
    global_surfaces *GLOBAL_SURFACES;
    GLOBAL_SURFACES = malloc(sizeof(global_surfaces));
    if (GLOBAL_SURFACES == NULL)
    {
        printf("Memory allocation of GLOBAL_SURFACES failed.\n");
        exit(EXIT_FAILURE);
    }
    
    basin_data *BASIN_DATA;
    BASIN_DATA = malloc(sizeof(basin_data));
    if (BASIN_DATA == NULL)
    {
        printf("Memory allocation of BASIN_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    
    CALCULATION_LOG->latC1 = GLOBAL_MESH->Lat[0][0];
    CALCULATION_LOG->latC2 = GLOBAL_MESH->Lat[0][GLOBAL_MESH->nY-1];
    CALCULATION_LOG->latC3 = GLOBAL_MESH->Lat[GLOBAL_MESH->nX-1][0];
    CALCULATION_LOG->latC4 = GLOBAL_MESH->Lat[GLOBAL_MESH->nX-1][GLOBAL_MESH->nY-1];
    
    CALCULATION_LOG->lonC1 = GLOBAL_MESH->Lon[0][0];
    CALCULATION_LOG->lonC2 = GLOBAL_MESH->Lon[0][GLOBAL_MESH->nY-1];
    CALCULATION_LOG->lonC3 = GLOBAL_MESH->Lon[GLOBAL_MESH->nX-1][0];
    CALCULATION_LOG->lonC4 = GLOBAL_MESH->Lon[GLOBAL_MESH->nX-1][GLOBAL_MESH->nY-1];
    
    partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS;
    partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS;
    in_basin *IN_BASIN;
    // Load Data
    loadAllGlobalData(GLOBAL_MODEL_PARAMETERS, CALCULATION_LOG, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA);
    
    // Loop over grid points and assign values
    for(int j = 0; j < GLOBAL_MESH->nY; j++)
    {
        PARTIAL_GLOBAL_MESH = extractPartialMesh(GLOBAL_MESH, j);
        PARTIAL_GLOBAL_QUALITIES = malloc(sizeof(partial_global_qualities));
        if (PARTIAL_GLOBAL_QUALITIES == NULL)
        {
            printf("Memory allocation of PARTIAL_GLOBAL_QUALITIES failed.\n");
            exit(EXIT_FAILURE);
        }
        printf("Slice %i of %i\r",j+1,GLOBAL_MESH->nY);
        fflush(stdout);
        for(int k = 0; k < PARTIAL_GLOBAL_MESH->nX; k++)
        {
            IN_BASIN = malloc(sizeof(in_basin));
            if (IN_BASIN == NULL)
            {
                printf("Memory allocation of IN_BASIN failed.\n");
                exit(EXIT_FAILURE);
            }
            PARTIAL_GLOBAL_SURFACE_DEPTHS = malloc(sizeof(partial_global_surface_depths));
            if (PARTIAL_GLOBAL_SURFACE_DEPTHS == NULL)
            {
                printf("Memory allocation of PARTIAL_GLOBAL_SURFACE_DEPTHS failed.\n");
                exit(EXIT_FAILURE);
            }
            PARTIAL_BASIN_SURFACE_DEPTHS = malloc(sizeof(partial_basin_surface_depths));
            if (PARTIAL_BASIN_SURFACE_DEPTHS == NULL)
            {
                printf("Memory allocation of PARTIAL_BASIN_SURFACE_DEPTHS failed.\n");
                exit(EXIT_FAILURE);
            }
            QUALITIES_VECTOR = malloc(sizeof(qualities_vector));
            if (QUALITIES_VECTOR == NULL)
            {
                printf("Memory allocation of QUALITIES_VECTOR failed.\n");
                exit(EXIT_FAILURE);
            }
            
            MESH_VECTOR = extractMeshVector(PARTIAL_GLOBAL_MESH, k);
            
            assignQualities(GLOBAL_MODEL_PARAMETERS, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA, MESH_VECTOR, PARTIAL_GLOBAL_SURFACE_DEPTHS, PARTIAL_BASIN_SURFACE_DEPTHS, IN_BASIN, QUALITIES_VECTOR, CALCULATION_LOG, "SQUASHED"); // hardcode squashed topography here
            for(int i = 0; i < PARTIAL_GLOBAL_MESH->nZ; i++)
            {
                PARTIAL_GLOBAL_QUALITIES->Rho[k][i] = QUALITIES_VECTOR->Rho[i];
                PARTIAL_GLOBAL_QUALITIES->Vp[k][i] = QUALITIES_VECTOR->Vp[i];
                PARTIAL_GLOBAL_QUALITIES->Vs[k][i] = QUALITIES_VECTOR->Vs[i];
                
            }
            free(MESH_VECTOR);
            free(QUALITIES_VECTOR);
            free(PARTIAL_BASIN_SURFACE_DEPTHS);
            free(PARTIAL_GLOBAL_SURFACE_DEPTHS);
            free(IN_BASIN);
        }
        if (strcmp(GEN_EXTRACT_VELO_MOD_CALL.VS_TYPE, "VS500") == 0)
        {
            calcAndSaveVs(OUTPUT_DIR, PARTIAL_GLOBAL_MESH, PARTIAL_GLOBAL_QUALITIES, CALCULATION_LOG,"500",j);
        }
        else if (strcmp(GEN_EXTRACT_VELO_MOD_CALL.VS_TYPE, "VS30") == 0)
        {
            calcAndSaveVs(OUTPUT_DIR, PARTIAL_GLOBAL_MESH, PARTIAL_GLOBAL_QUALITIES, CALCULATION_LOG,"30",j);
        }
        else if (strcmp(GEN_EXTRACT_VELO_MOD_CALL.VS_TYPE, "Z1.0") == 0)
        {
            calcAndSaveZThreshold(OUTPUT_DIR, PARTIAL_GLOBAL_MESH, PARTIAL_GLOBAL_QUALITIES, CALCULATION_LOG, Z_THRESH, j);
        }
        else if (strcmp(GEN_EXTRACT_VELO_MOD_CALL.VS_TYPE, "Z2.5") == 0)
        {
            calcAndSaveZThreshold(OUTPUT_DIR, PARTIAL_GLOBAL_MESH, PARTIAL_GLOBAL_QUALITIES, CALCULATION_LOG, Z_THRESH, j);
        }
        free(PARTIAL_GLOBAL_MESH);
        free(PARTIAL_GLOBAL_QUALITIES);
    }
    
    free(VELO_MOD_1D_DATA);
    freeEPtomoSurfaceData(NZ_TOMOGRAPHY_DATA);
    free(NZ_TOMOGRAPHY_DATA);
    free(GLOBAL_MESH);
    freeGlobalSurfaceData(GLOBAL_SURFACES, GLOBAL_MODEL_PARAMETERS);
    free(GLOBAL_SURFACES);
    freeAllBasinSurfaces(BASIN_DATA, GLOBAL_MODEL_PARAMETERS);
    free(BASIN_DATA);
    free(MODEL_EXTENT);
}


void runGenerateVelocityModel(char *MODEL_VERSION, char *OUTPUT_DIR, gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL, calculation_log *CALCULATION_LOG)
{
    int smoothingRequired = 0; // set as zero if no smoothing is required, set as 1 for smoothing
    int nPtsSmooth = 1; // number of points (eitherside of gridpoint) to incorporate for smoothing
    
    model_extent *MODEL_EXTENT = malloc(sizeof(model_extent));
    if (MODEL_EXTENT == NULL)
    {
        printf("Memory allocation of MODEL_EXTENT failed.\n");
        exit(EXIT_FAILURE);
    }
    MODEL_EXTENT->version = MODEL_VERSION;
    
    // Model origin paremeters
    MODEL_EXTENT->originLat = GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LAT;
    MODEL_EXTENT->originLon = GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_LON;
    MODEL_EXTENT->originRot = GEN_EXTRACT_VELO_MOD_CALL.ORIGIN_ROT;
    
    // Model extent parameters
    MODEL_EXTENT->Xmax = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_X;
    MODEL_EXTENT->Ymax = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Y;
    MODEL_EXTENT->Zmax = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_ZMAX; // max depth (positive downwards)
    MODEL_EXTENT->Zmin = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_ZMIN;
    MODEL_EXTENT->hDep = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_Z_SPACING;
    MODEL_EXTENT->hLatLon = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_LATLON_SPACING;
    
    // generate the model grid
    global_mesh *GLOBAL_MESH;
    GLOBAL_MESH = malloc(sizeof(global_mesh));
    if (GLOBAL_MESH == NULL)
    {
        printf("Memory allocation of GLOBAL_MESH failed.\n");
        exit(EXIT_FAILURE);
    }
    generateFullModelGridGreatCircle(MODEL_EXTENT, GLOBAL_MESH);
    
    writeVeloModCornersTextFile(GLOBAL_MESH, OUTPUT_DIR);
    
    // obtain surface filenames based off version number
    global_model_parameters *GLOBAL_MODEL_PARAMETERS;
    GLOBAL_MODEL_PARAMETERS = getGlobalModelParameters(MODEL_EXTENT->version);

    // read in velocity model data (surfaces, 1D models, tomography etc)
    velo_mod_1d_data *VELO_MOD_1D_DATA;
    VELO_MOD_1D_DATA = malloc(sizeof(velo_mod_1d_data));
    if (VELO_MOD_1D_DATA == NULL)
    {
        printf("Memory allocation of VELO_MOD_1D_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    
    nz_tomography_data *NZ_TOMOGRAPHY_DATA;
    NZ_TOMOGRAPHY_DATA = malloc(sizeof(nz_tomography_data));
    if (NZ_TOMOGRAPHY_DATA == NULL)
    {
        printf("Memory allocation of NZ_TOMOGRAPHY_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    
    global_surfaces *GLOBAL_SURFACES;
    GLOBAL_SURFACES = malloc(sizeof(global_surfaces));
    if (GLOBAL_SURFACES == NULL)
    {
        printf("Memory allocation of GLOBAL_SURFACES failed.\n");
        exit(EXIT_FAILURE);
    }
    
    basin_data *BASIN_DATA;
    BASIN_DATA = malloc(sizeof(basin_data));
    if (BASIN_DATA == NULL)
    {
        printf("Memory allocation of BASIN_DATA failed.\n");
        exit(EXIT_FAILURE);
    }
    
    CALCULATION_LOG->latC1 = GLOBAL_MESH->Lat[0][0];
    CALCULATION_LOG->latC2 = GLOBAL_MESH->Lat[0][GLOBAL_MESH->nY-1];
    CALCULATION_LOG->latC3 = GLOBAL_MESH->Lat[GLOBAL_MESH->nX-1][0];
    CALCULATION_LOG->latC4 = GLOBAL_MESH->Lat[GLOBAL_MESH->nX-1][GLOBAL_MESH->nY-1];
    
    CALCULATION_LOG->lonC1 = GLOBAL_MESH->Lon[0][0];
    CALCULATION_LOG->lonC2 = GLOBAL_MESH->Lon[0][GLOBAL_MESH->nY-1];
    CALCULATION_LOG->lonC3 = GLOBAL_MESH->Lon[GLOBAL_MESH->nX-1][0];
    CALCULATION_LOG->lonC4 = GLOBAL_MESH->Lon[GLOBAL_MESH->nX-1][GLOBAL_MESH->nY-1];
    
    double oneThird = 1.0/3.0;
    double half = 1.0/2.0;
    double fourThirds = 4.0/3.0;
    double A, B, C;
    // Load Data
    loadAllGlobalData(GLOBAL_MODEL_PARAMETERS, CALCULATION_LOG, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA);
    // Loop over grid points and assign values
    for(int j = 0; j < GLOBAL_MESH->nY; j++)
    {
        partial_global_mesh *PARTIAL_GLOBAL_MESH;
        partial_global_qualities *PARTIAL_GLOBAL_QUALITIES;

        printf("\rGenerating velocity model %d%% complete.", j*100/GLOBAL_MESH->nY);
        fflush(stdout);
        PARTIAL_GLOBAL_MESH = extractPartialMesh(GLOBAL_MESH, j);
        PARTIAL_GLOBAL_QUALITIES = malloc(sizeof(partial_global_qualities));
        if (PARTIAL_GLOBAL_QUALITIES == NULL)
        {
            printf("Memory allocation of PARTIAL_GLOBAL_QUALITIES failed.\n");
            exit(EXIT_FAILURE);
        }
        #pragma omp parallel for
        for(int k = 0; k < PARTIAL_GLOBAL_MESH->nX; k++)
        {
            in_basin *IN_BASIN;
            partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS;
            partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS;
            mesh_vector *MESH_VECTOR;
            mesh_vector *EXTENDED_MESH_VECTOR;
            qualities_vector *QUALITIES_VECTOR;
            qualities_vector *EXTENDED_QUALITIES_VECTOR;

            IN_BASIN = malloc(sizeof(in_basin));
            if (IN_BASIN == NULL)
            {
                printf("Memory allocation of IN_BASIN failed.\n");
                exit(EXIT_FAILURE);
            }
            PARTIAL_GLOBAL_SURFACE_DEPTHS = malloc(sizeof(partial_global_surface_depths));
            if (PARTIAL_GLOBAL_SURFACE_DEPTHS == NULL)
            {
                printf("Memory allocation of PARTIAL_GLOBAL_SURFACE_DEPTHS failed.\n");
                exit(EXIT_FAILURE);
            }
            PARTIAL_BASIN_SURFACE_DEPTHS = malloc(sizeof(partial_basin_surface_depths));
            if (PARTIAL_BASIN_SURFACE_DEPTHS == NULL)
            {
                printf("Memory allocation of PARTIAL_BASIN_SURFACE_DEPTHS failed.\n");
                exit(EXIT_FAILURE);
            }
            QUALITIES_VECTOR = malloc(sizeof(qualities_vector));
            if (QUALITIES_VECTOR == NULL)
            {
                printf("Memory allocation of QUALITIES_VECTOR failed.\n");
                exit(EXIT_FAILURE);
            }
            EXTENDED_QUALITIES_VECTOR = malloc(sizeof(qualities_vector));
            if (QUALITIES_VECTOR == NULL)
            {
                printf("Memory allocation of QUALITIES_VECTOR failed.\n");
                exit(EXIT_FAILURE);
            }
            
            if (smoothingRequired == 1)
            {
                EXTENDED_MESH_VECTOR = extendMeshVector(PARTIAL_GLOBAL_MESH, nPtsSmooth, MODEL_EXTENT->hDep*1000, k);
                //EXTENDED_MESH_VECTOR->referenceDepth = GEN_EXTRACT_VELO_MOD_CALL.EXTENT_ZMIN;
                assignQualities(GLOBAL_MODEL_PARAMETERS, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA, EXTENDED_MESH_VECTOR, PARTIAL_GLOBAL_SURFACE_DEPTHS, PARTIAL_BASIN_SURFACE_DEPTHS, IN_BASIN, EXTENDED_QUALITIES_VECTOR, CALCULATION_LOG, GEN_EXTRACT_VELO_MOD_CALL.TOPO_TYPE);
                int midPtCount, midPtCountPlus, midPtCountMinus;
                
                for(int i = 0; i < PARTIAL_GLOBAL_MESH->nZ; i++)
                {
                    midPtCount = i * (1+2*nPtsSmooth) + 1 ;
                    midPtCountPlus = midPtCount + 1;
                    midPtCountMinus = midPtCount - 1;

                    //(1/2)*{(1/3)Vs(i-1)+(4/3)Vs(i)+(1/3)Vs(i+1)}.
                    A = oneThird*EXTENDED_QUALITIES_VECTOR->Rho[midPtCountMinus];
                    B = fourThirds*EXTENDED_QUALITIES_VECTOR->Rho[midPtCount];
                    C = oneThird*EXTENDED_QUALITIES_VECTOR->Rho[midPtCountPlus];
//                    printf("%i %i %i %lf %lf %lf.\n",midPtCount,midPtCountPlus, midPtCountMinus, A,B,C);
                    PARTIAL_GLOBAL_QUALITIES->Rho[k][i] = half *( A + B + C);
                    
                    A = oneThird*EXTENDED_QUALITIES_VECTOR->Vp[midPtCountMinus];
                    B = fourThirds*EXTENDED_QUALITIES_VECTOR->Vp[midPtCount];
                    C = oneThird*EXTENDED_QUALITIES_VECTOR->Vp[midPtCountPlus];
//                                        printf("%lf %lf %lf.\n",A,B,C);
                    PARTIAL_GLOBAL_QUALITIES->Vp[k][i] = half *( A + B + C);
                    
                    A = oneThird*EXTENDED_QUALITIES_VECTOR->Vs[midPtCountMinus];
                    B = fourThirds*EXTENDED_QUALITIES_VECTOR->Vs[midPtCount];
                    C = oneThird*EXTENDED_QUALITIES_VECTOR->Vs[midPtCountPlus];
//                                        printf("%lf %lf %lf.\n",A,B,C);
                    PARTIAL_GLOBAL_QUALITIES->Vs[k][i] = half *( A + B + C);

                }
                free(EXTENDED_MESH_VECTOR);
            }
            else
            {
                MESH_VECTOR = extractMeshVector(PARTIAL_GLOBAL_MESH, k);
                assignQualities(GLOBAL_MODEL_PARAMETERS, VELO_MOD_1D_DATA, NZ_TOMOGRAPHY_DATA, GLOBAL_SURFACES, BASIN_DATA, MESH_VECTOR, PARTIAL_GLOBAL_SURFACE_DEPTHS, PARTIAL_BASIN_SURFACE_DEPTHS, IN_BASIN, QUALITIES_VECTOR, CALCULATION_LOG, GEN_EXTRACT_VELO_MOD_CALL.TOPO_TYPE);
                for(int i = 0; i < PARTIAL_GLOBAL_MESH->nZ; i++)
                {
                    PARTIAL_GLOBAL_QUALITIES->Rho[k][i] = QUALITIES_VECTOR->Rho[i];
                    PARTIAL_GLOBAL_QUALITIES->Vp[k][i] = QUALITIES_VECTOR->Vp[i];
                    PARTIAL_GLOBAL_QUALITIES->Vs[k][i] = QUALITIES_VECTOR->Vs[i];
                    
                }
                free(MESH_VECTOR);

            }
            free(EXTENDED_QUALITIES_VECTOR);
            free(QUALITIES_VECTOR);
            free(PARTIAL_BASIN_SURFACE_DEPTHS);
            free(PARTIAL_GLOBAL_SURFACE_DEPTHS);
            free(IN_BASIN);
        }
        writeGlobalQualities(OUTPUT_DIR, PARTIAL_GLOBAL_MESH, PARTIAL_GLOBAL_QUALITIES, GEN_EXTRACT_VELO_MOD_CALL,CALCULATION_LOG, j);
        free(PARTIAL_GLOBAL_MESH);
        free(PARTIAL_GLOBAL_QUALITIES);
    }
    printf("\rGeneration of velocity model 100%% complete.");
    fflush(stdout);
    printf("\n");
    printf("Model generation complete.\n");
    
    
    free(VELO_MOD_1D_DATA);
    freeEPtomoSurfaceData(NZ_TOMOGRAPHY_DATA);
    free(NZ_TOMOGRAPHY_DATA);
    free(GLOBAL_MESH);
    freeGlobalSurfaceData(GLOBAL_SURFACES, GLOBAL_MODEL_PARAMETERS);
    free(GLOBAL_SURFACES);
    freeAllBasinSurfaces(BASIN_DATA, GLOBAL_MODEL_PARAMETERS);
    free(BASIN_DATA);
}
