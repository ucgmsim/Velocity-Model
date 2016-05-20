//
//  calculateSurfaceLatLonIndex.c
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

indexStruct calculateSurfaceLatLonIndex(surfRead currentSurface, double x, double y)
/*
 Purpose:   calculate the lat and lon indicies of the points below the input x and y
 
 Input variables:
 currentSurface - structure containing surface data
 x              - longitude value of the point
 y              - latitude value of the point
 
 Output variables:
 indexStruct    - struct containing the lat and lon indicies of the first point below the input x y
 
 */
{
    indexStruct index;
    
    for(int i = 0; i < currentSurface.nLon; i++)
    {
        if(currentSurface.loni[i]>x)
        {
            index.X = i-1;
            break;
        }
    }
    
    for(int i = 0; i < currentSurface.nLat; i++)
    {
        if(currentSurface.lati[i]>y)
        {
            index.Y = i-1;
            break;
        }
    }
    
    return index;
}
