//
//  determineIfWithinBasinLatLon.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 28/11/14.
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


void determineIfWithinBasinLatLon(gridStruct *location, int basinNum, globalBasinData *basinData)
{
    double currLat, currLon;
    int basinFlag = 0; // 0=outside basin 1=possibly inside basin (prelim screen)
    int inPoly = 0;
//    printf("%i %i\n",location->nY,location->nX);
    for(int i = 0; i < location->nX; i++)
    {
        for(int j = 0; j < location->nY; j++)
        {
            currLat = location->Lat[i][j];
            currLon = location->Lon[i][j];
            for(int m = 0; m < basinData->nBoundaries[basinNum]; m++)
            {

                if(currLon >= basinData->maxLon[basinNum][m])
                {
                    basinFlag = 0;
                }
                else if(currLon <= basinData->minLon[basinNum][m])
                {
                    basinFlag = 0;
                }
                else if(currLat >= basinData->maxLat[basinNum][m])
                {
                    basinFlag = 0;
                }
                else if(currLat <= basinData->minLat[basinNum][m])
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
                    basinData->inBasinLatLon[basinNum][m][i][j] = 0; // not in rectangle
                }
                else if(basinFlag == 1)
                {
                    inPoly = pointInPoly(basinData, basinNum, m, currLon, currLat); // check if in poly
                    if(inPoly == 1) // inside poly, check depth points at a later stage
                    {
                        basinData->inBasinLatLon[basinNum][m][i][j] = 1; // in lat lon poly
                    }
                    else if(inPoly == 0) // outside poly
                    {
                        basinData->inBasinLatLon[basinNum][m][i][j] = 0;
                    }
                }
                basinFlag = 0;
            }
        }
    }
}