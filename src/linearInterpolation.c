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
