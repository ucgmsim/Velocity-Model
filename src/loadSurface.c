//
//  loadSurface.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 20/06/14.
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

global_surf_read *loadGlobalSurface(char *fileName)
/*
 Purpose:   load a surface into memory
 
 Input variables:
    *fileName  - pointer to the filename to open and read
 
 Output variables:
    *GLOBAL_SURF_READ - structure containing a global surface
 */
{
    double firstLat;
    double firstLon;
    double lastLat;
    double lastLon;
    FILE *file;
    file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error surface file %s not found.\n",fileName);
        exit(EXIT_FAILURE);
    }
    global_surf_read *GLOBAL_SURF_READ;
    GLOBAL_SURF_READ = malloc(sizeof(global_surf_read));
    
    int nLat, nLon;
    // read and assign the number of latitude and longitude values
    fscanf(file, "%d %d", &nLat, &nLon);
    GLOBAL_SURF_READ->nLat = nLat;
    GLOBAL_SURF_READ->nLon = nLon;
    
    // assure the preallocation of the surface struct is not exceeded
//    printf("%i.\n",GLOBAL_SURF_IN_DIM_MAX);
    assert(nLat<=GLOBAL_SURF_IN_DIM_MAX);
    assert(nLon<=GLOBAL_SURF_IN_DIM_MAX);
    
    // read lat values
    for(int i = 0; i < nLat; i++)
    {
        fscanf(file, "%lf",&GLOBAL_SURF_READ->lati[i]);
    }
    
    // read lon values
    for(int i = 0; i < nLon; i++)
    {
        fscanf(file, "%lf",&GLOBAL_SURF_READ->loni[i]);
    }
    
    // read in the surface raster data
    for(int i = 0; i < nLat; i++)
    {
        for(int j = 0; j < nLon; j++)
        {
            fscanf(file, "%lf",&GLOBAL_SURF_READ->raster[j][i]);
        }
    }
    
    fclose(file);
    
    firstLat = GLOBAL_SURF_READ->lati[0];
    lastLat = GLOBAL_SURF_READ->lati[GLOBAL_SURF_READ->nLat-1];
    
    if (firstLat >= lastLat)
    {
        GLOBAL_SURF_READ->maxLat = firstLat;
        GLOBAL_SURF_READ->minLat = lastLat;
    }
    else if (lastLat >= firstLat)
    {
        GLOBAL_SURF_READ->maxLat = lastLat;
        GLOBAL_SURF_READ->minLat = firstLat;
    }
    else
    {
        printf("Error.\n");
    }
    
    firstLon = GLOBAL_SURF_READ->loni[0];
    lastLon = GLOBAL_SURF_READ->loni[GLOBAL_SURF_READ->nLon-1];

    if (firstLon >= lastLon)
    {
        GLOBAL_SURF_READ->maxLon = firstLon;
        GLOBAL_SURF_READ->minLon = lastLon;
    }
    else if (lastLon >= firstLon)
    {
        GLOBAL_SURF_READ->maxLon = lastLon;
        GLOBAL_SURF_READ->minLon = firstLon;
    }
    else
    {
        printf("Error.\n");
    }

    return GLOBAL_SURF_READ;
    
}

basin_surf_read *loadBasinSurface(char *fileName)
/*
 Purpose:   load a basin surface into memory
 
 Input variables:
    *fileName  - pointer to the filename to open and read
 
 Output variables:
    *BASIN_SURF_READ - structure containing a basin surface
 */
{
    double firstLat;
    double firstLon;
    double lastLat;
    double lastLon;
    FILE *file;
    file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Error basin surface %s not found.\n",fileName);
        exit(EXIT_FAILURE);
    }

    basin_surf_read *BASIN_SURF_READ;
    BASIN_SURF_READ = malloc(sizeof(basin_surf_read));
    
    int nLat, nLon;
    // read and assign the number of latitude and longitude values
    fscanf(file, "%d %d", &nLat, &nLon);
    BASIN_SURF_READ->nLat = nLat;
    BASIN_SURF_READ->nLon = nLon;
    
    // assure the preallocation of the surface struct is not exceeded
    assert(nLat<=BASIN_SURF_IN_DIM_MAX);
    assert(nLon<=BASIN_SURF_IN_DIM_MAX);
    
    // read lat values
    for(int i = 0; i < nLat; i++)
    {
        fscanf(file, "%lf",&BASIN_SURF_READ->lati[i]);
    }
    
    // read lon values
    for(int i = 0; i < nLon; i++)
    {
        fscanf(file, "%lf",&BASIN_SURF_READ->loni[i]);
    }
    
    // read in the surface raster data
    for(int i = 0; i < nLat; i++)
    {
        for(int j = 0; j < nLon; j++)
        {
            fscanf(file, "%lf",&BASIN_SURF_READ->raster[j][i]);
        }
    }
    
    fclose(file);
    
    firstLat = BASIN_SURF_READ->lati[0];
    lastLat = BASIN_SURF_READ->lati[BASIN_SURF_READ->nLat-1];
    
    if (firstLat >= lastLat)
    {
        BASIN_SURF_READ->maxLat = firstLat;
        BASIN_SURF_READ->minLat = lastLat;
    }
    else if (lastLat >= firstLat)
    {
        BASIN_SURF_READ->maxLat = lastLat;
        BASIN_SURF_READ->minLat = firstLat;
    }
    else
    {
        printf("Error.\n");
    }
    
    firstLon = BASIN_SURF_READ->loni[0];
    lastLon = BASIN_SURF_READ->loni[BASIN_SURF_READ->nLon-1];
    
    if (firstLon >= lastLon)
    {
        BASIN_SURF_READ->maxLon = firstLon;
        BASIN_SURF_READ->minLon = lastLon;
    }
    else if (lastLon >= firstLon)
    {
        BASIN_SURF_READ->maxLon = lastLon;
        BASIN_SURF_READ->minLon = firstLon;
    }
    else
    {
        printf("Error.\n");
    }
    
    return BASIN_SURF_READ;
    
}


void loadGlobalSurfaceData(global_surfaces *GLOBAL_SURFACES, global_model_parameters *GLOBAL_MODEL_PARAMETERS)
/*
 Purpose:   load global surfaces and place pointers to them within the GLOBAL_SURFACES struct
 
 Input variables:
    *GLOBAL_SURFACES - struct containing pointers to global surfaces (whole domain surfaces which sub velocity models apply between)
    *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
 
 Output variables:
    n.a.
 */
{
    // place global surfaces into struct
    GLOBAL_SURFACES->nSurf = GLOBAL_MODEL_PARAMETERS->nSurf;
    
    for(int i = 0; i < GLOBAL_MODEL_PARAMETERS->nSurf; i++)
    {
        // load surface and transfer data into global struct
        GLOBAL_SURFACES->surf[i] = loadGlobalSurface(GLOBAL_MODEL_PARAMETERS->globalSurfFilenames[i]);
    }
}

void freeGlobalSurfaceData(global_surfaces *GLOBAL_SURFACES, global_model_parameters *GLOBAL_MODEL_PARAMETERS)
/*
 Purpose:   free memory allocated to global surfaces within the GLOBAL_SURFACES struct
 
 Input variables:
 *GLOBAL_SURFACES - struct containing pointers to global surfaces (whole domain surfaces which sub velocity models apply between)
 *GLOBAL_MODEL_PARAMETERS - struct containing all model parameters (surface names, submodel names, basin names etc)
 
 Output variables:
 n.a.
 */
{
    // place global surfaces into struct
    GLOBAL_SURFACES->nSurf = GLOBAL_MODEL_PARAMETERS->nSurf;
    
    for(int i = 0; i < GLOBAL_MODEL_PARAMETERS->nSurf; i++)
    {
        // free the global surfaces
        free (GLOBAL_SURFACES->surf[i]);
    }
}



