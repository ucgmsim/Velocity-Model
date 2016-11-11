//
//  extractPartialMesh.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 16/11/15.
//  Copyright (c) 2015 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "constants.h"
#include "structs.h"
#include "functions.h"



partial_global_mesh *generateSlicePartialMesh(individual_slice_parameters INDIVIDUAL_SLICE_PARAMETERS)
{
    partial_global_mesh *PARTIAL_GLOBAL_MESH;
    PARTIAL_GLOBAL_MESH = malloc(sizeof(partial_global_qualities));

    if (PARTIAL_GLOBAL_MESH == NULL)
    {
        printf("Memory allocation of PARTIAL_GLOBAL_MESH failed.\n");
        exit(EXIT_FAILURE);
    }

    
    
    
    double deltaLat;
    double deltaLon;
    int nGrdPts;
    int count = 0;
    
    nGrdPts = INDIVIDUAL_SLICE_PARAMETERS.resXY;
    
    deltaLat = (INDIVIDUAL_SLICE_PARAMETERS.latPtsSlice[0]-INDIVIDUAL_SLICE_PARAMETERS.latPtsSlice[1])/nGrdPts;
    deltaLon = (INDIVIDUAL_SLICE_PARAMETERS.lonPtsSlice[0]-INDIVIDUAL_SLICE_PARAMETERS.lonPtsSlice[1])/nGrdPts;
    
    for(int j = 0; j < nGrdPts+1; j++)
    {
        PARTIAL_GLOBAL_MESH->Lat[count] = INDIVIDUAL_SLICE_PARAMETERS.latPtsSlice[0]-j*deltaLat;
        PARTIAL_GLOBAL_MESH->Lon[count] = INDIVIDUAL_SLICE_PARAMETERS.lonPtsSlice[0]-j*deltaLon;
        count += 1;
    }
    PARTIAL_GLOBAL_MESH->nX= count;
    if( PARTIAL_GLOBAL_MESH->nX>=LON_GRID_DIM_MAX)
    {
        printf("Total number of slice points exceeds the maximum allowable value of %i.\n",LON_GRID_DIM_MAX);
        exit(EXIT_FAILURE);
    }
    
    
    int i;
    
    PARTIAL_GLOBAL_MESH->nY = 1;
    PARTIAL_GLOBAL_MESH->nZ = INDIVIDUAL_SLICE_PARAMETERS.resZ +1.0;
    if( PARTIAL_GLOBAL_MESH->nZ>=DEP_GRID_DIM_MAX)
    {
        printf("Total number of slice depth points exceeds the maximum allowable value of %i.\n",DEP_GRID_DIM_MAX);
        exit(EXIT_FAILURE);
    }
    
    double dz = (INDIVIDUAL_SLICE_PARAMETERS.zMax - INDIVIDUAL_SLICE_PARAMETERS.zMin) / PARTIAL_GLOBAL_MESH->nZ;
    
    for(i = 0; i < PARTIAL_GLOBAL_MESH->nZ; i ++)
    {
        PARTIAL_GLOBAL_MESH->Z[i] = 1000*(-1.0*INDIVIDUAL_SLICE_PARAMETERS.zMin - i*dz);
    }

    
    return PARTIAL_GLOBAL_MESH;
    
    
}
partial_global_mesh *extractPartialMesh(global_mesh *GLOBAL_MESH, int latInd)
/*
 Purpose: to extract one slice of values from the global mesh, i.e nX x nY x nZ becomes nX x 1 x nZ
 
 Input variables:
    *GLOBAL_MESH - structure containing the full model grid (lat, lon and depth points)
    latInd - y indicie of the slice of the global grid to be extracted
 
 Output variables:
    *PARTIAL_GLOBAL_MESH - struct containing a slice of the global mesh
 */
{
    partial_global_mesh *PARTIAL_GLOBAL_MESH;
    PARTIAL_GLOBAL_MESH = malloc(sizeof(partial_global_mesh));
    if(PARTIAL_GLOBAL_MESH == NULL)
    {
        printf("Memory allocation failed for PARTIAL_GLOBAL_MESH array.\n");
        exit(EXIT_FAILURE);
    }
    
    int i;
    
    PARTIAL_GLOBAL_MESH->nX = GLOBAL_MESH->nX;
    PARTIAL_GLOBAL_MESH->nY = 1;
    PARTIAL_GLOBAL_MESH->nZ = GLOBAL_MESH->nZ;
    PARTIAL_GLOBAL_MESH->Y = GLOBAL_MESH->Y[latInd];

    for(i = 0; i < GLOBAL_MESH->nZ; i ++)
    {
        PARTIAL_GLOBAL_MESH->Z[i] = GLOBAL_MESH->Z[i];
    }
    for(i = 0; i < GLOBAL_MESH->nX; i ++)
    {
        PARTIAL_GLOBAL_MESH->Lon[i] = GLOBAL_MESH->Lon[i][latInd];
        PARTIAL_GLOBAL_MESH->Lat[i] = GLOBAL_MESH->Lat[i][latInd];
        PARTIAL_GLOBAL_MESH->X[i] = GLOBAL_MESH->X[i];
    }
    return PARTIAL_GLOBAL_MESH;
}

mesh_vector *extractMeshVector(partial_global_mesh *PARTIAL_GLOBAL_MESH, int lonInd)
/*
 Purpose: to extract one vector of values from the global mesh, i.e  nX x 1 x nZ becomes 1 x 1 x nZ
 
 Input variables:
    *GLOBAL_MESH - structure containing the full model grid (lat, lon and depth points)
    lonInd - x indicie of the slice of the grid to be extracted
 
 Output variables:
    *MESH_VECTOR - struct containing one lat - lon point and the depths of all grid points at this location
 */
{
    mesh_vector *MESH_VECTOR;
    MESH_VECTOR = malloc(sizeof(mesh_vector));
    if(MESH_VECTOR == NULL)
    {
        printf("Memory allocation failed for MESH_VECTOR array.\n");
        exit(EXIT_FAILURE);
    }
    
    MESH_VECTOR->Lat = &PARTIAL_GLOBAL_MESH->Lat[lonInd];
    MESH_VECTOR->Lon = &PARTIAL_GLOBAL_MESH->Lon[lonInd];
    
    for(int i = 0; i < PARTIAL_GLOBAL_MESH->nZ; i++)
    {
        MESH_VECTOR->Z[i] = PARTIAL_GLOBAL_MESH->Z[i];
    }
    MESH_VECTOR->nZ = PARTIAL_GLOBAL_MESH->nZ;
    
    return MESH_VECTOR;
}

mesh_vector *extendMeshVector(partial_global_mesh *PARTIAL_GLOBAL_MESH, int nPts, double dZPt, int lonInd)
{
    mesh_vector *MESH_VECTOR;
    MESH_VECTOR = malloc(sizeof(mesh_vector));
    if(MESH_VECTOR == NULL)
    {
        printf("Memory allocation failed for MESH_VECTOR array.\n");
        exit(EXIT_FAILURE);
    }
    int nPtsSmooth = 1 + 2 * nPts;

    int nZ = nPtsSmooth * PARTIAL_GLOBAL_MESH->nZ;
    if(nZ>=DEP_GRID_DIM_MAX)
    {
        printf("nZ required for smoothing exceeds the allowable value of %i.\n",DEP_GRID_DIM_MAX);
        exit(EXIT_FAILURE);
    }
    
    
    MESH_VECTOR->Lat = &PARTIAL_GLOBAL_MESH->Lat[lonInd];
    MESH_VECTOR->Lon = &PARTIAL_GLOBAL_MESH->Lon[lonInd];
    
    int count = 0;
    double half = 0.9999*1.0/2.0;
    for(int i = 0; i < PARTIAL_GLOBAL_MESH->nZ; i++)
    {
        for (int j = 0; j < nPtsSmooth; j ++)
        {
            count = i*nPtsSmooth + j;
            if(count == 0)
            {
                MESH_VECTOR->Z[count] = PARTIAL_GLOBAL_MESH->Z[i] - (j - nPts)*dZPt*half;
            }
            else
            {
                MESH_VECTOR->Z[count] = PARTIAL_GLOBAL_MESH->Z[i] - (j - nPts)*dZPt;
            }
            
        }
    }
    MESH_VECTOR->nZ = nZ;
    
    return MESH_VECTOR;
}




