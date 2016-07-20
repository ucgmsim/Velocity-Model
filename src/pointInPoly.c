//
//  pointInPoly.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 5/08/14.
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

int pointInPoly(basin_data *BASIN_DATA, int basinNum, int boundaryNum, double xLoc, double yLoc)
/*
 Purpose: to determine if a lat - lon point is within a arbitrarly shaped basin boundary
 
 Input variables:
    *BASIN_DATA - struct containing basin data (surfaces submodels etc)
    basinNum - the basin number pertaining to the basin of interest
    boundaryNum - the boundary number pertaining to the boundary of interest
    xLoc - longitude value of point
    yLoc - latitude value of point
 
 Output variables:
    c - interger indicating if point lies within polygon perscribed by the basin boundary
 */
{
    int i, j, c = 0;
    for (i = 0, j = BASIN_DATA->boundaryNumPoints[basinNum][boundaryNum]-1; i < BASIN_DATA->boundaryNumPoints[basinNum][boundaryNum]; j = i++)
    {
        if ( ((BASIN_DATA->boundaryLat[basinNum][boundaryNum][i]>yLoc) != (BASIN_DATA->boundaryLat[basinNum][boundaryNum][j]>yLoc)) &&
            (xLoc < (BASIN_DATA->boundaryLon[basinNum][boundaryNum][j]-BASIN_DATA->boundaryLon[basinNum][boundaryNum][i]) * (yLoc-BASIN_DATA->boundaryLat[basinNum][boundaryNum][i]) / (BASIN_DATA->boundaryLat[basinNum][boundaryNum][j]-BASIN_DATA->boundaryLat[basinNum][boundaryNum][i]) + BASIN_DATA->boundaryLon[basinNum][boundaryNum][i]) )
            c = !c;
    }
    return c;
}

int pnpoly(int nvert, double *vertx, double *verty, double testx, double testy)
{
    int i, j, c = 0;
    for (i = 0, j = nvert-1; i < nvert; j = i++) {
        if ( ((verty[i]>testy) != (verty[j]>testy)) &&
            (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
            c = !c;
    }
    return c;
}

// probably best to rewrite these functions!

/*
//adapted from
//http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
//License to Use
//
//Copyright (c) 1970-2003, Wm. Randolph Franklin
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimers.
//Redistributions in binary form must reproduce the above copyright notice in the documentation and/or other materials provided with the distribution.
//The name of W. Randolph Franklin may not be used to endorse or promote products derived from this Software without specific prior written permission.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//*/