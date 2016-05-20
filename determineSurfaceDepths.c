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


surfDepValues *determineSurfaceDepths(gridStruct *location, char *fileName)
/*
 Purpose:   obtain the depths for all lat lon points for a given surface file
 
 Input variables:
 location  - structure containing lat lon grid
 fileName  - filename of the surface file for reading
 
 Output variables:
 surfDep   - (malloc'd) pointer to structure containing surface depths for all lat lon points
 */
{
    // read in the filename
    surfRead *currentSurface;
    currentSurface = loadSurface(fileName);
    
    surfDepValues *surfDep;
    surfDep = malloc(sizeof(surfDepValues));
    adjacentPointsStruct *points;
    
    double p1, p2, p3, v1, v2;

    
    // loop over values and find the depth of the surface at all points (2D interpolation)
    for(int i = 0; i < location->nX; i++)
    {
        for(int j = 0; j < location->nY; j++)
        {
            // find adjacent points
            points = findAdjacentPoints(currentSurface, location->Lat[i][j], location->Lon[i][j]);
            
            if (points->inSurfaceBounds == 1)
            {
            // interpolate
            surfDep->dep[i][j] = biLinearInterpolation( currentSurface->loni[points->lonInd[0]], currentSurface->loni[points->lonInd[1]], currentSurface->lati[points->latInd[0]], currentSurface->lati[points->latInd[1]], currentSurface->raster[points->lonInd[0]][points->latInd[0]], currentSurface->raster[points->lonInd[0]][points->latInd[1]], currentSurface->raster[points->lonInd[1]][points->latInd[0]], currentSurface->raster[points->lonInd[1]][points->latInd[1]], location->Lon[i][j], location->Lat[i][j]);
            }
            else if (points->inLatExtensionZone == 1)
            {
                p1 = currentSurface->loni[points->lonInd[0]];
                p2 = currentSurface->loni[points->lonInd[1]];
                v1 = currentSurface->raster[points->lonInd[0]][points->latEdgeInd];
                v2 = currentSurface->raster[points->lonInd[1]][points->latEdgeInd];
                p3 = location->Lon[i][j];
                surfDep->dep[i][j] = linearInterpolation(p1, p2, v1, v2, p3);
            }
            else if (points->inLonExtensionZone == 1)
            {
                p1 = currentSurface->lati[points->latInd[0]];
                p2 = currentSurface->lati[points->latInd[1]];
                v1 = currentSurface->raster[points->lonEdgeInd][points->latInd[0]];
                v2 = currentSurface->raster[points->lonEdgeInd][points->latInd[1]];
                p3 = location->Lat[i][j];
                surfDep->dep[i][j] = linearInterpolation(p1, p2, v1, v2, p3);
            }
            else if (points->inCornerZone == 1)
            {
                surfDep->dep[i][j] = currentSurface->raster[points->cornerLonInd][points->cornerLatInd];
            }
            free(points);

        }
    }
    free(currentSurface);
        
    return surfDep;
}

surfDepValues *determineSurfaceDepthsBasin(globalBasinData *basinData ,gridStruct *location, char *fileName, int basinNum, int surfNum)
/*
 Purpose:   obtain the depths for all lat lon points within the  for a given surface file
 
 Input variables:
 location  - structure containing lat lon grid
 fileName  - filename of the surface file for reading
 
 Output variables:
 surfDep   - (malloc'd) pointer to structure containing surface depths for all lat lon points
 */
{
    // read in the filename
    surfRead *currentSurface;
    currentSurface = loadSurface(fileName);
    
    surfDepValues *surfDep;
    surfDep = malloc(sizeof(surfDepValues));
    adjacentPointsStruct *points;
    
    // loop over values and find the depth of the surface at all points (2D interpolation)
    for(int i = 0; i < location->nX; i++)
    {
        for(int j = 0; j < location->nY; j++)
        {
            if(basinData->inBasinLatLon[basinNum][basinData->boundaryType[basinNum][surfNum]][i][j] == 1)
            {
                // find adjacent points
                points = findAdjacentPoints(currentSurface, location->Lat[i][j], location->Lon[i][j]);
                //                printf("%lf %lf\n",location->Lat[i][j], location->Lon[i][j]);
                //                printf("%i\n",basinData->inBasinLatLon[basinNum][basinData->boundaryType[basinNum][surfNum]][i][j]);
                
                if (points->inSurfaceBounds == 1)
                {
                    // interpolate
                    surfDep->dep[i][j] = biLinearInterpolation( currentSurface->loni[points->lonInd[0]], currentSurface->loni[points->lonInd[1]], currentSurface->lati[points->latInd[0]], currentSurface->lati[points->latInd[1]], currentSurface->raster[points->lonInd[0]][points->latInd[0]], currentSurface->raster[points->lonInd[0]][points->latInd[1]], currentSurface->raster[points->lonInd[1]][points->latInd[0]], currentSurface->raster[points->lonInd[1]][points->latInd[1]], location->Lon[i][j], location->Lat[i][j]);
                    free(points);
                }
                else
                {
                    printf("%lf %lf\n",location->Lat[i][j], location->Lon[i][j]);

                    printf("Error, point lies outside basin surface domain.\n");
                }
            }
            else
            {
                surfDep->dep[i][j] = NAN; // define as NAN if surface is outside of the boundary
            }
        }
    }
    free(currentSurface);
    
    return surfDep;
}
