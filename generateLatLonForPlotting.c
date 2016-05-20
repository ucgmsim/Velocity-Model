
//
//  generateLatLonForPlotting.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 15/07/14.
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

gridStruct *generateLatLonForPlotting(modOrigin modelOrigin, modExtent modelExtent, double latPts[], double lonPts[], int nPts)
/*
 Purpose:   generate the grid of lat long dep points to obtain the velocity profiles from
 
 Input variables:
 modelOrigin        - struct containing the origin and rotation of the model
 modelExtent        - struct containing the extent, spacing and version of the model.
 latPts             - vector of latitude points for plotting
 lonPts             - vector of longitude points for plotting
 nPts               - number of points for plotting
 
 Output variables:
 gridPtsStruct      - structure containing lat lon and dep points at the desired points for plotting
 */
{
    gridStruct *gridPtsStruct;
    gridPtsStruct = malloc(sizeof(gridStruct));
    // determine number of points to place in the struct
	int nX = nPts;
	int nY = nPts;
	int nZ = (modelExtent.Zmax-modelExtent.Zmin)/modelExtent.hDep;
    
    assert(nZ<=DEP_GRID_DIM_MAX);
    
    //write values to the struct
    gridPtsStruct->nX = nX;
    gridPtsStruct->nY = nY;
    gridPtsStruct->nZ = nZ;
    
    for(int i = 0; i < nPts; i++)
    {
        for(int j = 0; j < nPts; j++)
            
        {
            gridPtsStruct->Lat[i][j] = latPts[i];
            gridPtsStruct->Lon[i][j] = lonPts[i];
        }
    }
    for(int i = 0; i < nZ; i++)
	{
		gridPtsStruct->Z[i] = -1000*(modelExtent.Zmin+modelExtent.hDep*(i+0.5));
	}
    return gridPtsStruct;
}
