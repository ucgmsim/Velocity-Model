//
//  findClosestPoint.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 2/06/15.
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


void findCornerInds(surfRead *surface, double latPt, double lonPt, adjacentPointsStruct *points)
{
    if(latPt == surface->lati[0])
    {
        points->cornerLatInd = 0;
    }
    else if (latPt == surface->lati[surface->nLat-1])
    {
        points->cornerLatInd = surface->nLat-1;
    }
    else
    {
        printf("Error.\n");
    }
    
    if(lonPt == surface->loni[0])
    {
        points->cornerLonInd = 0;
    }
    else if (lonPt == surface->loni[surface->nLon-1])
    {
        points->cornerLonInd = surface->nLon-1;
    }
    else
    {
        printf("Error.\n");
    }
    
    
    points->inCornerZone = 1;
    
}

void findEdgeInds(surfRead *surface, adjacentPointsStruct *points, int edgeType)
{
    if (edgeType == 1)
    {
        if(surface->maxLat == surface->lati[0])
        {
            points->latEdgeInd = 0;
        }
        else if (surface->maxLat == surface->lati[surface->nLat-1])
        {
            points->latEdgeInd = surface->nLat-1;
        }
        else
        {
            printf("Error.\n");
        }
    }
    else if (edgeType == 3)
    {
        if(surface->minLat == surface->lati[0])
        {
            points->latEdgeInd = 0;
        }
        else if (surface->minLat == surface->lati[surface->nLat-1])
        {
            points->latEdgeInd = surface->nLat-1;
        }
        else
        {
            printf("Error.\n");
        }
    }
    else if (edgeType == 2)
    {
        if(surface->maxLon == surface->loni[0])
        {
            points->lonEdgeInd = 0;
        }
        else if (surface->maxLon == surface->loni[surface->nLon-1])
        {
            points->lonEdgeInd = surface->nLon-1;
        }
        else
        {
            printf("Error.\n");
        }
    }
    else if (edgeType == 4)
    {
        if(surface->minLon == surface->loni[0])
        {
            points->lonEdgeInd = 0;
        }
        else if (surface->minLon == surface->loni[surface->nLon-1])
        {
            points->lonEdgeInd = surface->nLon-1;
        }
        else
        {
            printf("Error.\n");
        }
    }
    else
    {
        printf("Error.\n");
    }

}









