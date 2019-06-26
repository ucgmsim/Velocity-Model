//
//  linearInterpolation.c
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

double linearInterpolation(double p1, double p2, double v1, double v2, double p3)
/*
 Purpose:   linearly interpolate bewteen two points
 
 Input variables:
 p1           - point 1
 p2           - point 2
 v1           - value of point 1
 v2           - value of point 2
 p3           - location of point for the value to be obtained
 
 Output variables:
 v3           - the interpolated value at the input point p3
 
 */
{
    double v3 = v1 +(v2-v1)*(p3-p1)/(p2-p1);
    return v3;
}

double biLinearInterpolation(double X1, double X2, double Y1, double Y2, double Q11, double Q12, double Q21, double Q22, double X, double Y)
/*
 Purpose:   bilinearly interpolate bewteen four points
 
 Input variables:
 X1
 X2
 Y1
 Y2
 Q11
 Q12
 Q21
 Q22
 X
 Y
 
 Output variables:
 Q           - the interpolated value at the input point
 
 */
{
    
//    printf("%lf %lf %lf\n", X1, X, X2);
//    printf("%lf %lf %lf\n\n", Y1, Y, Y2);

    double A, B, C, D, E, Q;
    A = Q11*(X2-X)*(Y2-Y);
    B = Q21*(X-X1)*(Y2-Y);
    C = Q12*(X2-X)*(Y-Y1);
    D = Q22*(X-X1)*(Y-Y1);
    E = 1/((X2-X1)*(Y2-Y1));
    
    Q = (A+B+C+D)*E;
    return Q;
}


double interpolateGlobalSurface(global_surf_read *GLOBAL_SURF_READ, double latPt, double lonPt, adjacent_points *ADJACENT_POINTS)
/*
 Purpose: to interpolate / extrapolate a gobal surface
 
 Input variables:
 *GLOBAL_SURF_READ - structure containing a global surface
 latPt - latitude of point to for eventual interpolation
 lonPt - longitude of point to for eventual interpolation
 *ADJACENT_POINTS - structure containing indicies of points adjacent to the lat - lon for interpolation / extrapolation
 
 Output variables:
 value interpolated at the lat-lon point
 */
{
    double p1, p2, p3, v1, v2;
    double X1, X2, Y1, Y2, Q11, Q12, Q21, Q22, X, Y;
    
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
        X = lonPt;
        Y = latPt;
        return biLinearInterpolation(X1, X2, Y1, Y2, Q11, Q12, Q21, Q22, X, Y);
        
    }
    
    // if point lies within the extension zone, take on the value of the closest point
    else if (ADJACENT_POINTS->inLatExtensionZone == 1)
    {
        p1 = GLOBAL_SURF_READ->loni[ADJACENT_POINTS->lonInd[0]];
        p2 = GLOBAL_SURF_READ->loni[ADJACENT_POINTS->lonInd[1]];
        v1 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonInd[0]][ADJACENT_POINTS->latEdgeInd];
        v2 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonInd[1]][ADJACENT_POINTS->latEdgeInd];
        p3 = lonPt;
        return linearInterpolation(p1, p2, v1, v2, p3);
    }
    else if (ADJACENT_POINTS->inLonExtensionZone == 1)
    {
        p1 = GLOBAL_SURF_READ->lati[ADJACENT_POINTS->latInd[0]];
        p2 = GLOBAL_SURF_READ->lati[ADJACENT_POINTS->latInd[1]];
        v1 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonEdgeInd][ADJACENT_POINTS->latInd[0]];
        v2 = GLOBAL_SURF_READ->raster[ADJACENT_POINTS->lonEdgeInd][ADJACENT_POINTS->latInd[1]];
        p3 = latPt;

        return linearInterpolation(p1, p2, v1, v2, p3);
    }
    else if (ADJACENT_POINTS->inCornerZone == 1)
    {
        return GLOBAL_SURF_READ->raster[ADJACENT_POINTS->cornerLonInd][ADJACENT_POINTS->cornerLatInd];
    }
    printf("Calcualtion of Global surface value failed.\n");
    exit(EXIT_FAILURE);
}
