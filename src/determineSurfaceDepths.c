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
    

    // loop over the number of global surfaces
    for(int i = 0; i < GLOBAL_SURFACES->nSurf; i++)
    {
        GLOBAL_SURF_READ = GLOBAL_SURFACES->surf[i];
        ADJACENT_POINTS = malloc(sizeof(adjacent_points));
        findGlobalAdjacentPoints(GLOBAL_SURF_READ, *MESH_VECTOR->Lat, *MESH_VECTOR->Lon,ADJACENT_POINTS);
        PARTIAL_GLOBAL_SURFACE_DEPTHS->dep[i] = interpolateGlobalSurface(GLOBAL_SURF_READ,*MESH_VECTOR->Lat, *MESH_VECTOR->Lon, ADJACENT_POINTS);
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

