//
//  determineSurfaceDepths.c
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


void interpolateGlobalSurfaceDepths(global_surfaces *GLOBAL_SURFACES, mesh_vector *MESH_VECTOR ,partial_global_surface_depths *PARTIAL_GLOBAL_SURFACE_DEPTHS, calculation_log *CALCULATION_LOG)
/*
 Purpose: interpolate the surface depths at the lat lon location given in MESH_VECTOR
 
 Input variables:
    *GLOBAL_SURFACES - struct containing pointers to global surfaces (whole domain surfaces which sub velocity models apply between)
    *MESH_VECTOR - struct containing a single lat lon point with one or more depths
    *CALCULATION_LOG - struct containing calculation data and output directory (tracks various parameters for error reporting etc)
 
 Output variables:
    *PARTIAL_GLOBAL_SURFACE_DEPTHS - struct containing the global surface depths at the lat lon of the grid point

 */
{
    adjacent_points *ADJACENT_POINTS;
    global_surf_read *GLOBAL_SURF_READ;
    
    double p1, p2, p3, v1, v2;
    double X1, X2, Y1, Y2, Q11, Q12, Q21, Q22, X, Y;
    // loop over the number of global surfaces
    for(int i = 0; i < GLOBAL_SURFACES->nSurf; i++)
    {
        GLOBAL_SURF_READ = GLOBAL_SURFACES->surf[i];
        ADJACENT_POINTS = findGlobalAdjacentPoints(GLOBAL_SURF_READ, *MESH_VECTOR->Lat, *MESH_VECTOR->Lon);
        
        // if point lies within the surface bounds, interpolate as normal
        if (ADJACENT_POINTS->inSurfaceBounds == 1)
        {
            // interpolate
            X1 = GLOBAL_SURF_READ->loni[ADJACENT_POINTS->lonInd[0]];
            X2 = GLOBAL_SURF_READ->loni[ADJACENT_POINTS->lonInd[1]];
            Y1 = GLOBAL_SURF_READ->lati[ADJACENT_POINTS->latInd[0]];
            Y2 = GLOBAL_SURF_READ->lati[ADJACENT_POINTS->latInd[1]];
            Q11 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonInd[0]][ADJACENT_POINTS->latInd[0]];
            Q12 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonInd[0]][ADJACENT_POINTS->latInd[1]];
            Q21 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonInd[1]][ADJACENT_POINTS->latInd[0]];
            Q22 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonInd[1]][ADJACENT_POINTS->latInd[1]];
            X = *MESH_VECTOR->Lon;
            Y = *MESH_VECTOR->Lat;
            PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[i] =  biLinearInterpolation(X1, X2, Y1, Y2, Q11, Q12, Q21, Q22, X, Y);
            
        }
        
        // if point lies within the extension zone, take on the value of the closest point
        else if (ADJACENT_POINTS->inLatExtensionZone == 1)
        {
            p1 = GLOBAL_SURF_READ->loni[ADJACENT_POINTS->lonInd[0]];
            p2 = GLOBAL_SURF_READ->loni[ADJACENT_POINTS->lonInd[1]];
            v1 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonInd[0]][ADJACENT_POINTS->latEdgeInd];
            v2 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonInd[1]][ADJACENT_POINTS->latEdgeInd];
            p3 = *MESH_VECTOR->Lon;
            PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[i] = linearInterpolation(p1, p2, v1, v2, p3);
            CALCULATION_LOG->nPointsInGlobalLatSurfaceExtensionZone += 1;
        }
        else if (ADJACENT_POINTS->inLonExtensionZone == 1)
        {
            p1 = GLOBAL_SURF_READ->lati[ADJACENT_POINTS->latInd[0]];
            p2 = GLOBAL_SURF_READ->lati[ADJACENT_POINTS->latInd[1]];
            v1 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonEdgeInd][ADJACENT_POINTS->latInd[0]];
            v2 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonEdgeInd][ADJACENT_POINTS->latInd[1]];
            p3 = *MESH_VECTOR->Lat;
            PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[i] = linearInterpolation(p1, p2, v1, v2, p3);
            CALCULATION_LOG->nPointsInGlobalLonSurfaceExtensionZone += 1;
        }
        else if (ADJACENT_POINTS->inCornerZone == 1)
        {
            PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[i] = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->cornerLonInd][ADJACENT_POINTS->cornerLatInd];
            CALCULATION_LOG->nPointsInGlobalCornerSurfaceExtensionZone += 1;
        }

        free(ADJACENT_POINTS);
    }
    double topVal, botVal;
    
    PARTIAL_GLOBAL_SURFACE_DEPTHS->nSurfDep = GLOBAL_SURFACES->nSurf;
    for(int i = GLOBAL_SURFACES->nSurf -1 ; i > 0; i--)
    {
        // enforce surface depths from bottom down
        
        topVal = PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[i-1];
        botVal = PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[i];
        
        if(topVal < botVal)
        {
            PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[i] = PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[i-1];
            CALCULATION_LOG->nPointsGlobalSurfacesEnforced += 1;

        }
        
    }
        
}

