//
//  enforceSurfaceDepths.c
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

void enforceSurfaceDepths(globalBasinData *basinData, int xInd, int yInd, int basinNum)
/*
 Purpose:   enforce the depths of the surfaces are consistant with stratigraphy
 
 Input variables:

 NA
 */
{
    double topVal, botVal;
    for(int i = basinData->nSurf[basinNum]-1; i > 0; i--)
    {
        topVal = basinData->surfVals[basinNum][xInd][yInd][i-1];
        botVal = basinData->surfVals[basinNum][xInd][yInd][i];
        
        if(isnan(topVal) !=0)
        {
            break; // if the upper surface is NaN, ie does not exist, then break
        }
        else if(topVal < botVal)
        {
            basinData->surfVals[basinNum][xInd][yInd][i-1] = basinData->surfVals[basinNum][xInd][yInd][i]; // reassign surface value to that of the one below it
        }
    }
}