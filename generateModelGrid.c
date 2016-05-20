//
//  generateModelGrid.c
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

gridStruct *generateModelGrid(modOrigin modelOrigin, modExtent modelExtent)
/*
 Purpose:   generate the grid of lat long and dep points
 
 Input variables:
 modelOrigin        - struct containing the origin and rotation of the model
 modelExtent        - struct containing the extent, spacing and version of the model.
 
 Output variables:
 gridPtsStruct      - (malloc'd) pointer to structure containing lat lon and dep points
 
 */
{
    gridStruct *gridPtsStruct;
    gridPtsStruct = malloc(sizeof(gridStruct));
    // determine number of points in each orthogonal direction
	int nX = 2*modelExtent.Xmax/modelExtent.hLatLon;
	int nY = 2*modelExtent.Ymax/modelExtent.hLatLon;
	int nZ = (modelExtent.Zmax-modelExtent.Zmin)/modelExtent.hDep;
    gridPtsStruct->maxLat = -180;
    gridPtsStruct->minLat = 0;
    gridPtsStruct->maxLon = 0;
    gridPtsStruct->minLon = 180;
    
    // esnure the number of points does not exceed that of the struct preallocation
	printf("nx: %i, ny: %i, nz: %i.\n", nX, nY, nZ);
    assert(nX<=LON_GRID_DIM_MAX);
    assert(nY<=LAT_GRID_DIM_MAX);
    assert(nZ<=DEP_GRID_DIM_MAX);
    
    //write values to the struct
    gridPtsStruct->nX = nX;
    gridPtsStruct->nY = nY;
    gridPtsStruct->nZ = nZ; 
    
	double X[LON_GRID_DIM_MAX], Y[LAT_GRID_DIM_MAX], Z[DEP_GRID_DIM_MAX];
    
    // loop over x y z arrays to insert values
	for(int i = 0; i < nX; i++)
	{
		X[i] = -1*modelExtent.Xmax + modelExtent.hLatLon*(i+0.5);
        gridPtsStruct->X[i] = X[i];
	}
    
    for(int i = 0; i < nY; i++)
	{
		Y[i] = -1*modelExtent.Ymax + modelExtent.hLatLon*(i+0.5);
        gridPtsStruct->Y[i] = Y[i];
	}
    
    for(int i = 0; i < nZ; i++)
	{
		Z[i] = modelExtent.Zmin + modelExtent.hDep*(i+0.5);
	}
    
    double locationXY[2];
    // calculate and insert values into struct
    singleGridPoint points;
    for(int ix = 0; ix < nX; ix++) //nX
    {
        for(int iy = 0; iy < nY; iy++) //xY
        {
            locationXY[0] = X[ix];
            locationXY[1] = Y[iy];
            
            points = XYtoLatLon(locationXY, modelOrigin);
            // determin the maximum lat long points of the model grid
            if( gridPtsStruct->maxLat < points.lat)
            {
                gridPtsStruct->maxLat = points.lat;
            }
            if( gridPtsStruct->maxLon < points.lon)
            {
                gridPtsStruct->maxLon = points.lon;
            }
            if( gridPtsStruct->minLat > points.lat)
            {
                gridPtsStruct->minLat = points.lat;
            }
            if( gridPtsStruct->minLon > points.lon)
            {
                gridPtsStruct->minLon = points.lon;
            }
            gridPtsStruct->Lat[ix][iy] = points.lat;
            gridPtsStruct->Lon[ix][iy] = points.lon;
        }
    }
    
    for(int iz = 0; iz < nZ; iz++)
    {
        gridPtsStruct->Z[iz] = -1000*Z[iz]; // convert to m (below ground is negative)
    }
    printf("Completed Generation of Model Grid.\n");
    return gridPtsStruct;
}


