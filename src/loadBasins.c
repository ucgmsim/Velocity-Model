//
//  loadBasins.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 4/08/14.
//  Copyright (c) 2014 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "constants.h"
#include "structs.h"
#include "functions.h"

void loadBasinData(basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS)
/*
 Purpose: load all basin data into the BASIN_DATA structure
 
 Input variables:
 *BASIN_DATA - struct containing basin data (surfaces submodels etc)
 *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
 
 Output variables:
 n.a.
 */
{
    // loop over nBasins and load in surfaces, boundaries and sub-models
    for( int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasins; i++)
    {
        loadAllBasinSurfaces(i, BASIN_DATA, GLOBAL_MODEL_PARAMETERS);
        loadBasinBoundaries(i, BASIN_DATA, GLOBAL_MODEL_PARAMETERS);
        loadBasinSubModelData(i, BASIN_DATA, GLOBAL_MODEL_PARAMETERS);

    }
    printf("All basin surfaces loaded.\n");
    printf("All basin boundaries loaded.\n");
    printf("All basin sub model data loaded.\n");


}

void loadBasinSubModelData(int basinNum, basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS)
/*
 Purpose: load the velocity submodel data required for the basin
 
 Input variables:
 basinNum - the basin number pertaining to the basin of interest
 *BASIN_DATA - struct containing basin data (surfaces submodels etc)
 *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
 
 Output variables:
 n.a.
 */
{
    for( int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum]-1; i++)
    {
        // insert hard coded basin subvelocity model data loads here
        if(strcmp (GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][i], "Cant1D_v1") == 0)
        {
            char fName[MAX_FILENAME_STRING_LEN] = "Cant1D_v1.fd_modfile";
            load1dVeloSubModel(fName, &BASIN_DATA->BASIN_SUBMODEL_DATA[basinNum].VELO_MOD_1D_DATA);
            
        }
        if(strcmp (GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][i], "Cant1D_v2") == 0)
        {
            char fName[MAX_FILENAME_STRING_LEN] = "Cant1D_v2.fd_modfile";
            load1dVeloSubModel(fName, &BASIN_DATA->BASIN_SUBMODEL_DATA[basinNum].VELO_MOD_1D_DATA);
            
        }
        if(strcmp (GLOBAL_MODEL_PARAMETERS->basinSubModelNames[basinNum][i], "Cant1D_v2_Pliocene_Enforced") == 0)
        {
            char fName[MAX_FILENAME_STRING_LEN] = "Cant1D_v2_Pliocene_Enforced.fd_modfile";
            load1dVeloSubModel(fName, &BASIN_DATA->BASIN_SUBMODEL_DATA[basinNum].VELO_MOD_1D_DATA);
            
        }
    }
}

void interpolateBasinSurfaceDepths(basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS, in_basin *IN_BASIN, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, mesh_vector *MESH_VECTOR)
/*
 Purpose: determine if a lat lon point is in a basin, if so interpolate the basin surface depths, enforce their hierarchy, then determine which depth points lie within the basin limits
 
 Input variables:
 *BASIN_DATA - struct containing basin data (surfaces submodels etc)
 *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
 *IN_BASIN - struct containing flags to indicate if lat lon point - depths lie within the basin
 *PARTIAL_BASIN_SURFACE_DEPTHS - struct containing depths for all applicable basin surfaces at one lat - lon location
 *MESH_VECTOR - struct containing a single lat lon point with one or more depths
 
 Output variables:
 n.a.
 */
{
    determineIfWithinBasinLatLon(BASIN_DATA, GLOBAL_MODEL_PARAMETERS, IN_BASIN, *MESH_VECTOR->Lat, *MESH_VECTOR->Lon);
    determineBasinSurfaceDepths(BASIN_DATA, GLOBAL_MODEL_PARAMETERS, IN_BASIN, PARTIAL_BASIN_SURFACE_DEPTHS, *MESH_VECTOR->Lat, *MESH_VECTOR->Lon);
    enforceBasinSurfaceDepths(BASIN_DATA, GLOBAL_MODEL_PARAMETERS, IN_BASIN, PARTIAL_BASIN_SURFACE_DEPTHS, MESH_VECTOR);
}

void enforceBasinSurfaceDepths(basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS, in_basin *IN_BASIN, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, mesh_vector *MESH_VECTOR)
/*
 Purpose: enforce the hierarchy of interpolated basin surface depths
 
 Input variables:
 *BASIN_DATA - struct containing basin data (surfaces submodels etc)
 *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
 *IN_BASIN - struct containing flags to indicate if lat lon point - depths lie within the basin
 *PARTIAL_BASIN_SURFACE_DEPTHS - struct containing depths for all applicable basin surfaces at one lat - lon location
 *MESH_VECTOR - struct containing a single lat lon point with one or more depths
 
 Output variables:
 n.a.
 */
{
    // enforce basin surface depths for all points in the within the lat lon poly (ignore others)
    double topLim, botLim;
    for( int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasins; i++)
    {
        // assume boundary zero (0) is the largest, as boundary one (1) need be fully enclosed within this boundary
        if(IN_BASIN->inBasinLatLon[i][0] == 1)
        {
            // enforce
            enforceSurfaceDepths(GLOBAL_MODEL_PARAMETERS, PARTIAL_BASIN_SURFACE_DEPTHS, i);
            
            // check if depth is within the top and bottom basin layers
            topLim = PARTIAL_BASIN_SURFACE_DEPTHS->dep[i][0];
            botLim = PARTIAL_BASIN_SURFACE_DEPTHS->dep[i][GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[i]-1];
            
            for(int k  = 0; k < MESH_VECTOR->nZ; k++)
            {
                if(MESH_VECTOR->Z[k] > topLim)
                {
                    IN_BASIN->inBasinDep[i][k] = 0;
                }
                else if (MESH_VECTOR->Z[k] < botLim)
                {
                    IN_BASIN->inBasinDep[i][k] = 0;
                }
                else
                {
                    IN_BASIN->inBasinDep[i][k] = 1; // in basin Z limits
                }
            }
        }
        else
        {
            for(int k  = 0; k < MESH_VECTOR->nZ; k++)
            {
                IN_BASIN->inBasinDep[i][k] = 0;
            }
        }
    }
}

void enforceSurfaceDepths(global_model_parameters *GLOBAL_MODEL_PARAMETERS, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, int basinNum)
/*
 Purpose:   enforce the depths of the surfaces are consistant with stratigraphy
 
 Input variables:
 *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
 *PARTIAL_BASIN_SURFACE_DEPTHS - struct containing depths for all applicable basin surfaces at one lat - lon location
 basinNum - the basin number pertaining to the basin of interest
 
 Output variables:
 n.a.
 */
{
    double topVal, botVal;
    int nanObtained = 0; // use this flag to ensure all surfaces are enforced
    int nanInd = 0;
    for(int i = GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum]-1; i > 0; i--)
    {
        topVal = PARTIAL_BASIN_SURFACE_DEPTHS->dep[basinNum][i-1];
        botVal = PARTIAL_BASIN_SURFACE_DEPTHS->dep[basinNum][i];
        
        if(isnan(topVal) !=0)
        {
            nanObtained = 1;
            nanInd = i;
            break; // if the upper surface is NaN, ie does not exist, then break
        }
        else if(topVal < botVal)
        {
            PARTIAL_BASIN_SURFACE_DEPTHS->dep[basinNum][i-1] = PARTIAL_BASIN_SURFACE_DEPTHS->dep[basinNum][i];
            // reassign surface value to that of the one below it
        }
    }
    if (nanObtained == 1)
    {
        for( int j = 0; j < nanInd-1; j++)
        {
            topVal = PARTIAL_BASIN_SURFACE_DEPTHS->dep[basinNum][j];
            botVal = PARTIAL_BASIN_SURFACE_DEPTHS->dep[basinNum][j+1];
            if(topVal < botVal)
            {
                PARTIAL_BASIN_SURFACE_DEPTHS->dep[basinNum][j] = PARTIAL_BASIN_SURFACE_DEPTHS->dep[basinNum][j+1];
                // reassign surface value to that of the one below it
            }
        }
    }
}



void determineBasinSurfaceDepths(basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS, in_basin *IN_BASIN, partial_basin_surface_depths *PARTIAL_BASIN_SURFACE_DEPTHS, double Lat, double Lon)
/*
 Purpose: interpolate the depths of all basin surfaces for the given lat lon
 
 Input variables:
 *BASIN_DATA - struct containing basin data (surfaces submodels etc)
 *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
 *IN_BASIN - struct containing flags to indicate if lat lon point - depths lie within the basin
 *PARTIAL_BASIN_SURFACE_DEPTHS - struct containing depths for all applicable basin surfaces at one lat - lon location
 Lat - latitude value of point of concern
 Lon - longitude value of point of concern
 
 Output variables:
 n.a.
 */
{
    adjacent_points *ADJACENT_POINTS;
    
    for(int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasins; i++)
    {
        for(int j = 0; j < GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[i]; j++)
        {
//            printf("%i, %i\n",i,j);
//            printf("bount num %i\n",GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[i][j]);
//            printf("%s\n", GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[i][j]);
//            printf("%i, %i\n",IN_BASIN->inBasinLatLon[i][GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[i][j]],j);
            if(IN_BASIN->inBasinLatLon[i][GLOBAL_MODEL_PARAMETERS->basinBoundaryNumber[i][j]] == 1)
            {
                // find adjacent points
                ADJACENT_POINTS = findBasinAdjacentPoints(BASIN_DATA->surf[i][j], Lat, Lon);
                if (ADJACENT_POINTS->inSurfaceBounds == 1)
                {
                    // interpolate
                    PARTIAL_BASIN_SURFACE_DEPTHS->dep[i][j] = biLinearInterpolation( BASIN_DATA->surf[i][j]->loni[ADJACENT_POINTS->lonInd[0]], BASIN_DATA->surf[i][j]->loni[ADJACENT_POINTS->lonInd[1]], BASIN_DATA->surf[i][j]->lati[ADJACENT_POINTS->latInd[0]], BASIN_DATA->surf[i][j]->lati[ADJACENT_POINTS->latInd[1]], BASIN_DATA->surf[i][j]->raster[ADJACENT_POINTS->lonInd[0]][ADJACENT_POINTS->latInd[0]], BASIN_DATA->surf[i][j]->raster[ADJACENT_POINTS->lonInd[0]][ADJACENT_POINTS->latInd[1]], BASIN_DATA->surf[i][j]->raster[ADJACENT_POINTS->lonInd[1]][ADJACENT_POINTS->latInd[0]], BASIN_DATA->surf[i][j]->raster[ADJACENT_POINTS->lonInd[1]][ADJACENT_POINTS->latInd[1]], Lon, Lat);
                    free(ADJACENT_POINTS);
                }
                else
                {
                    printf("Error, point lies outside basin surface domain.\n");
                }
            }
            else
            {
                PARTIAL_BASIN_SURFACE_DEPTHS->dep[i][j] = NAN; // define as NAN if surface is outside of the boundary
            }
        }
    }
}

adjacent_points *findBasinAdjacentPoints(basin_surf_read *BASIN_SURF_READ, double lat, double lon)
/*
 Purpose: find the adjacent points for interpolation within the basin surface for a given lat lon
 
 Input variables:
 *BASIN_SURF_READ - struct containing a basin surface
 Lat - latitude value of point of concern
 Lon - longitude value of point of concern
 
 Output variables:
 *ADJACENT_POINTS - structure containing indicies of points adjacent to the lat - lon for interpolation
 */
{
    adjacent_points *ADJACENT_POINTS;
    ADJACENT_POINTS = malloc(sizeof(adjacent_points));
    
    int latAssignedFlag = 0;
    int lonAssignedFlag = 0;
    ADJACENT_POINTS->inSurfaceBounds = 0;
    
    for( int i = 0; i < BASIN_SURF_READ->nLat; i++)
    {
        if(BASIN_SURF_READ->lati[i] >= lat)
        {
            if (i==0)
            {
                break;
            }
            ADJACENT_POINTS->latInd[0]= i-1;
            ADJACENT_POINTS->latInd[1] = i;
            latAssignedFlag = 1;
            break;
            
        }
    }
    if(latAssignedFlag == 0) // to account for some surface file vectors of lat long being ascending not descending
    {
        for(int i = BASIN_SURF_READ->nLat-1; i >= 0; i--)
        {
            if(BASIN_SURF_READ->lati[i] >= lat)
            {
                if (i==BASIN_SURF_READ->nLat-1)
                {
                    break;
                }
                ADJACENT_POINTS->latInd[0]= i;
                ADJACENT_POINTS->latInd[1] = i+1;
                latAssignedFlag = 1;
                break;
                
            }
        }
    }
    for( int j = 0; j < BASIN_SURF_READ->nLon; j++)
    {
        if(BASIN_SURF_READ->loni[j] >= lon)
        {
            if (j==0)
            {
                break;
            }
            ADJACENT_POINTS->lonInd[0] = j-1;
            ADJACENT_POINTS->lonInd[1] = j;
            lonAssignedFlag = 1;
            break;
        }
    }
    if (lonAssignedFlag == 0)
    {
        for( int j = BASIN_SURF_READ->nLon-1; j >= 0; j--)
        {
            if(BASIN_SURF_READ->loni[j] >= lon)
            {
                if (j==BASIN_SURF_READ->nLon-1)
                {
                    break;
                }
                ADJACENT_POINTS->lonInd[0] = j;
                ADJACENT_POINTS->lonInd[1] = j+1;
                lonAssignedFlag = 1;
                break;
            }
        }
    }
    
    if((latAssignedFlag != 1)||(lonAssignedFlag !=1)) // if any indicies are unassigned
    {
        printf("Error, basin point lies outside of the extent of the basin surface.\n");
    }
    else
    {
        ADJACENT_POINTS->inSurfaceBounds = 1;
    }
    
    return ADJACENT_POINTS;
}


void determineIfWithinBasinLatLon(basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS, in_basin *IN_BASIN, double Lat, double Lon)
/*
 Purpose: determine if a point lies within the different basin boundaries
 
 Input variables:
 *BASIN_DATA - struct containing basin data (surfaces submodels etc)
 *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
 *IN_BASIN - struct containing flags to indicate if lat lon point - depths lie within the basin
 Lat - latitude value of point of concern
 Lon - longitude value of point of concern
 
 Output variables:
 n.a.
 */
{
    int basinFlag = 0;
    int inPoly;
    for( int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasins; i++)
    {
        for( int j = 0; j < GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[i]; j++)
        {
            if(Lon >= BASIN_DATA->maxLonBoundary[i][j])
            {
                basinFlag = 0;
            }
            else if(Lon <= BASIN_DATA->minLonBoundary[i][j])
            {
                basinFlag = 0;
            }
            else if(Lat >= BASIN_DATA->maxLatBoundary[i][j])
            {
                basinFlag = 0;
            }
            else if(Lat <= BASIN_DATA->minLatBoundary[i][j])
            {
                basinFlag = 0;
            }
            else
            {
                basinFlag = 1; // possibly in basin
            }
            
            // assign flag to indicate if point is inside(1) or outside(0) basin
            if(basinFlag == 0)
            {
                IN_BASIN->inBasinLatLon[i][j] = 0; // not in rectangle
            }
            else if(basinFlag == 1)
            {
                inPoly = pointInPoly(BASIN_DATA, i, j, Lon, Lat); // check if in poly
                if(inPoly == 1) // inside poly, check depth points at a later stage
                {
                    IN_BASIN->inBasinLatLon[i][j] = 1; // in lat lon poly
                }
                else if(inPoly == 0) // outside poly
                {
                    IN_BASIN->inBasinLatLon[i][j] = 0;
                }
                basinFlag = 0;
            }
            
        }
    }
}


void loadAllBasinSurfaces(int basinNum, basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS)
/*
 Purpose: load all basin surfaces as perscribed in GLOBAL_MODEL_PARAMETERS
 
 Input variables:
 basinNum - the basin number pertaining to the basin of interest
 *BASIN_DATA - struct containing basin data (surfaces submodels etc)
 *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
 
 Output variables:
 n.a.
 */
{
    for(int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[basinNum]; i++)
    {
        // load surface and transfer data into global struct
        BASIN_DATA->surf[basinNum][i] = loadBasinSurface(GLOBAL_MODEL_PARAMETERS->basinSurfaceFilenames[basinNum][i]);
    }
}


void freeAllBasinSurfaces(basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS)
/*
 Purpose: free all previously load all basin surfaces as perscribed in GLOBAL_MODEL_PARAMETERS
 
 Input variables:
 *BASIN_DATA - struct containing basin data (surfaces submodels etc)
 *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
 
 Output variables:
 n.a.
 */
{
    for( int j = 0; j < GLOBAL_MODEL_PARAMETERS->nBasins; j++)
    {
        for(int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasinSurfaces[j]; i++)
        {
            // load surface and transfer data into global struct
            free(BASIN_DATA->surf[j][i]);
        }
    }
}


void loadBasinBoundaries(int basinNum, basin_data *BASIN_DATA, global_model_parameters *GLOBAL_MODEL_PARAMETERS)
/*
 Purpose: load all basin boundaries
 
 Input variables:
 basinNum - the basin number pertaining to the basin of interest
 *BASIN_DATA - struct containing basin data (surfaces submodels etc)
 *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
 
 Output variables:
 n.a
 */
{
    for (int i = 0; i < GLOBAL_MODEL_PARAMETERS->nBasinBoundaries[basinNum]; i++)
    {
        FILE *file;
        file = fopen(GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[basinNum][i], "r");
        if (file == NULL)
        {
            printf("Error basin boundary file %s not found.\n",GLOBAL_MODEL_PARAMETERS->basinBoundaryFilenames[basinNum][i]);
            exit(EXIT_FAILURE);
        }
        int count = 0;
        BASIN_DATA->minLonBoundary[basinNum][i] = 180; // starting points to refine the encompassing rectangle
        BASIN_DATA->maxLonBoundary[basinNum][i] = -180;
        BASIN_DATA->minLatBoundary[basinNum][i] = 180;
        BASIN_DATA->maxLatBoundary[basinNum][i] = -180;
        
        while(!feof(file))
        {
            fscanf(file, "%lf %lf", &BASIN_DATA->boundaryLon[basinNum][i][count], &BASIN_DATA->boundaryLat[basinNum][i][count]);
            
            BASIN_DATA->minLonBoundary[basinNum][i] = fmin(BASIN_DATA->minLonBoundary[basinNum][i],BASIN_DATA->boundaryLon[basinNum][i][count]);
            BASIN_DATA->minLatBoundary[basinNum][i] = fmin(BASIN_DATA->minLatBoundary[basinNum][i],BASIN_DATA->boundaryLat[basinNum][i][count]);
            
            BASIN_DATA->maxLonBoundary[basinNum][i] = fmax(BASIN_DATA->maxLonBoundary[basinNum][i],BASIN_DATA->boundaryLon[basinNum][i][count]);
            BASIN_DATA->maxLatBoundary[basinNum][i] = fmax(BASIN_DATA->maxLatBoundary[basinNum][i],BASIN_DATA->boundaryLat[basinNum][i][count]);
            
            count += 1;
        }
        fclose(file);
        
        BASIN_DATA->boundaryNumPoints[basinNum][i] = count;
        assert(count<=MAX_DIM_BOUNDARY_FILE);
        assert(BASIN_DATA->boundaryLon[basinNum][i][count-1] == BASIN_DATA->boundaryLon[basinNum][i][0]);
        assert(BASIN_DATA->boundaryLat[basinNum][i][count-1] == BASIN_DATA->boundaryLat[basinNum][i][0]);
    }
}

