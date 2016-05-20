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

int pointInPoly(globalBasinData *basinData, int basinNum, int boundaryNum, double xLoc, double yLoc)
{
    int i, j, c = 0;
    for (i = 0, j = basinData->boundaryNumPoints[basinNum][boundaryNum]-1; i < basinData->boundaryNumPoints[basinNum][boundaryNum]; j = i++)
    {
        if ( ((basinData->boundaryLat[basinNum][boundaryNum][i]>yLoc) != (basinData->boundaryLat[basinNum][boundaryNum][j]>yLoc)) &&
            (xLoc < (basinData->boundaryLon[basinNum][boundaryNum][j]-basinData->boundaryLon[basinNum][boundaryNum][i]) * (yLoc-basinData->boundaryLat[basinNum][boundaryNum][i]) / (basinData->boundaryLat[basinNum][boundaryNum][j]-basinData->boundaryLat[basinNum][boundaryNum][i]) + basinData->boundaryLon[basinNum][boundaryNum][i]) )
            c = !c;
    }
    return c;
}

// probably best to rewrite this function!

/*
adapted from
http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
License to Use

Copyright (c) 1970-2003, Wm. Randolph Franklin

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimers.
Redistributions in binary form must reproduce the above copyright notice in the documentation and/or other materials provided with the distribution.
The name of W. Randolph Franklin may not be used to endorse or promote products derived from this Software without specific prior written permission.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/