//
//  IDW.c
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

adjacent_points *findGlobalAdjacentPoints(global_surf_read *GLOBAL_SURF_READ, double lat, double lon)
/*
 Purpose: to find the adjacent points for interpolation for a given lat lon value within the global surfaces
 
 Input variables:
    *GLOBAL_SURF_READ - structure containing a global surface
    lat - latitude of point to for eventual interpolation
    lon - longitude of point to for eventual interpolation
 
 Output variables:
    *ADJACENT_POINTS - structure containing indicies of points adjacent to the lat - lon for interpolation
 
 */
{
    adjacent_points *ADJACENT_POINTS;
    ADJACENT_POINTS = malloc(sizeof(adjacent_points));
    
    int latAssignedFlag = 0;
    int lonAssignedFlag = 0;
    ADJACENT_POINTS->inSurfaceBounds = 0;
        
    for( int i = 0; i < GLOBAL_SURF_READ->nLat; i++)
    {
        if(GLOBAL_SURF_READ->lati[i] >= lat)
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
        for(int i = GLOBAL_SURF_READ->nLat-1; i >= 0; i--)
        {
            if(GLOBAL_SURF_READ->lati[i] >= lat)
            {
                if (i==GLOBAL_SURF_READ->nLat-1)
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
    for( int j = 0; j < GLOBAL_SURF_READ->nLon; j++)
    {
        if(GLOBAL_SURF_READ->loni[j] >= lon)
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
        for( int j = GLOBAL_SURF_READ->nLon-1; j >= 0; j--)
        {
            if(GLOBAL_SURF_READ->loni[j] >= lon)
            {
                if (j==GLOBAL_SURF_READ->nLon-1)
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
        ADJACENT_POINTS->inLatExtensionZone = 0;
        ADJACENT_POINTS->inLonExtensionZone = 0;
        ADJACENT_POINTS->inCornerZone = 0;
        if((lonAssignedFlag == 1) && (latAssignedFlag == 0)) // longitude assigned
        {
            // check if point is within extended latitude limits
            if((lat - GLOBAL_SURF_READ->maxLat) <= MAX_LAT_SURFACE_EXTENSION && (lat >= GLOBAL_SURF_READ->maxLat))
            {
                ADJACENT_POINTS->inLatExtensionZone = 1;
                findEdgeInds(GLOBAL_SURF_READ, ADJACENT_POINTS,1);
            }
            else if((GLOBAL_SURF_READ->minLat - lat) <= MAX_LAT_SURFACE_EXTENSION && (lat <= GLOBAL_SURF_READ->minLat))
            {
                ADJACENT_POINTS->inLatExtensionZone = 1;
                findEdgeInds(GLOBAL_SURF_READ, ADJACENT_POINTS,3);
            }
        }
        
        if((latAssignedFlag == 1) && (lonAssignedFlag == 0)) // latitude assigned
        {
            // check if the point is within extended longitude limits
            if((GLOBAL_SURF_READ->minLon - lon) <= MAX_LON_SURFACE_EXTENSION && (lon <= GLOBAL_SURF_READ->minLon))
            {
                ADJACENT_POINTS->inLonExtensionZone = 1;
                findEdgeInds(GLOBAL_SURF_READ, ADJACENT_POINTS,4);

            }
            else if((lon - GLOBAL_SURF_READ->maxLon) <= MAX_LON_SURFACE_EXTENSION && (lon >=GLOBAL_SURF_READ->maxLon))
            {
                ADJACENT_POINTS->inLonExtensionZone = 1;
                findEdgeInds(GLOBAL_SURF_READ, ADJACENT_POINTS,2);

            }
        }
        
        // four cases for corner zones
        if(((lat - GLOBAL_SURF_READ->maxLat) <= MAX_LAT_SURFACE_EXTENSION) && ((GLOBAL_SURF_READ->minLon - lon) <= MAX_LON_SURFACE_EXTENSION) && (lon <= GLOBAL_SURF_READ->minLon) && (lat >= GLOBAL_SURF_READ->maxLat))
        {
            // top left
            findCornerInds(GLOBAL_SURF_READ, GLOBAL_SURF_READ->maxLat, GLOBAL_SURF_READ->minLon, ADJACENT_POINTS);
        }
        else if (((lat - GLOBAL_SURF_READ->maxLat) <= MAX_LAT_SURFACE_EXTENSION) && ((lon - GLOBAL_SURF_READ->maxLon) <= MAX_LON_SURFACE_EXTENSION) && (lon >= GLOBAL_SURF_READ->maxLon) && (lat >= GLOBAL_SURF_READ->maxLat) )
        {
            // top right
            findCornerInds(GLOBAL_SURF_READ, GLOBAL_SURF_READ->maxLat, GLOBAL_SURF_READ->maxLon, ADJACENT_POINTS);
        }
        else if (((GLOBAL_SURF_READ->minLat - lat) <= MAX_LAT_SURFACE_EXTENSION) && ((GLOBAL_SURF_READ->minLon - lon) <= MAX_LON_SURFACE_EXTENSION) && (lon <= GLOBAL_SURF_READ->minLon) && (lat <= GLOBAL_SURF_READ->minLat))
        {
            // bottom left
            findCornerInds(GLOBAL_SURF_READ, GLOBAL_SURF_READ->minLat, GLOBAL_SURF_READ->minLon, ADJACENT_POINTS);
        }
        else if (((GLOBAL_SURF_READ->minLat - lat) <= MAX_LAT_SURFACE_EXTENSION) && ((lon - GLOBAL_SURF_READ->maxLon) <= MAX_LON_SURFACE_EXTENSION) && (lon >= GLOBAL_SURF_READ->maxLon) && (lat <= GLOBAL_SURF_READ->minLat))
        {
            // bottom right
            findCornerInds(GLOBAL_SURF_READ, GLOBAL_SURF_READ->minLat, GLOBAL_SURF_READ->maxLon, ADJACENT_POINTS);
        }
    }
    else
    {
        ADJACENT_POINTS->inSurfaceBounds = 1;
    }
    
    return ADJACENT_POINTS;
}




void findEdgeInds(global_surf_read *GLOBAL_SURF_READ, adjacent_points *ADJACENT_POINTS, int edgeType)
/*
 Purpose: to find the indicies of the edge of the global surface closest to the lat - lon point
 
 Input variables:
    *GLOBAL_SURF_READ - structure containing a global surface
    *ADJACENT_POINTS - structure containing indicies of points adjacent to the lat - lon for interpolation
    edgetype - int indicating whether the point lies to the north, east, south, or west of the global surface

 Output variables:
    n.a.
 */
{
    if (edgeType == 1)
    {
        if(GLOBAL_SURF_READ->maxLat == GLOBAL_SURF_READ->lati[0])
        {
            ADJACENT_POINTS->latEdgeInd = 0;
        }
        else if (GLOBAL_SURF_READ->maxLat == GLOBAL_SURF_READ->lati[GLOBAL_SURF_READ->nLat-1])
        {
            ADJACENT_POINTS->latEdgeInd = GLOBAL_SURF_READ->nLat-1;
        }
        else
        {
            printf("Point lies outside of surface bounds.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (edgeType == 3)
    {
        if(GLOBAL_SURF_READ->minLat == GLOBAL_SURF_READ->lati[0])
        {
            ADJACENT_POINTS->latEdgeInd = 0;
        }
        else if (GLOBAL_SURF_READ->minLat == GLOBAL_SURF_READ->lati[GLOBAL_SURF_READ->nLat-1])
        {
            ADJACENT_POINTS->latEdgeInd = GLOBAL_SURF_READ->nLat-1;
        }
        else
        {
            printf("Point lies outside of surface bounds.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (edgeType == 2)
    {
        if(GLOBAL_SURF_READ->maxLon == GLOBAL_SURF_READ->loni[0])
        {
            ADJACENT_POINTS->lonEdgeInd = 0;
        }
        else if (GLOBAL_SURF_READ->maxLon == GLOBAL_SURF_READ->loni[GLOBAL_SURF_READ->nLon-1])
        {
            ADJACENT_POINTS->lonEdgeInd = GLOBAL_SURF_READ->nLon-1;
        }
        else
        {
            printf("Point lies outside of surface bounds.\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (edgeType == 4)
    {
        if(GLOBAL_SURF_READ->minLon == GLOBAL_SURF_READ->loni[0])
        {
            ADJACENT_POINTS->lonEdgeInd = 0;
        }
        else if (GLOBAL_SURF_READ->minLon == GLOBAL_SURF_READ->loni[GLOBAL_SURF_READ->nLon-1])
        {
            ADJACENT_POINTS->lonEdgeInd = GLOBAL_SURF_READ->nLon-1;
        }
        else
        {
            printf("Point lies outside of surface bounds.\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("Point lies outside of surface bounds.\n");
        exit(EXIT_FAILURE);
    }
    
}

void findCornerInds(global_surf_read *GLOBAL_SURF_READ, double latPt, double lonPt, adjacent_points *ADJACENT_POINTS)
/*
 Purpose: to find the indicies of the corner of the global surface closest to the lat - lon point
 
 Input variables:
    *GLOBAL_SURF_READ - structure containing a global surface
    latPt - latitude of point to for eventual interpolation
    lonPt - longitude of point to for eventual interpolation
    *ADJACENT_POINTS - structure containing indicies of points adjacent to the lat - lon for interpolation
 
 Output variables:
    n.a.
 */
{
    if(latPt == GLOBAL_SURF_READ->lati[0])
    {
        ADJACENT_POINTS->cornerLatInd = 0;
    }
    else if (latPt == GLOBAL_SURF_READ->lati[GLOBAL_SURF_READ->nLat-1])
    {
        ADJACENT_POINTS->cornerLatInd = GLOBAL_SURF_READ->nLat-1;
    }
    else
    {
        printf("Point lies outside of surface bounds.\n");
        exit(EXIT_FAILURE);
    }
    
    if(lonPt == GLOBAL_SURF_READ->loni[0])
    {
        ADJACENT_POINTS->cornerLonInd = 0;
    }
    else if (lonPt == GLOBAL_SURF_READ->loni[GLOBAL_SURF_READ->nLon-1])
    {
        ADJACENT_POINTS->cornerLonInd = GLOBAL_SURF_READ->nLon-1;
    }
    else
    {
        printf("Point lies outside of surface bounds.\n");
        exit(EXIT_FAILURE);
    }
    
    ADJACENT_POINTS->inCornerZone = 1;
    
}


