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
    //printf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", X1, X, X2, Y1, Y, Y2, Q11, Q12, Q22, Q21);
    double A, B, C, D, E, Q;
    A = Q11*(X2-X)*(Y2-Y);
    B = Q21*(X-X1)*(Y2-Y);
    C = Q12*(X2-X)*(Y-Y1);
    D = Q22*(X-X1)*(Y-Y1);
    E = 1/((X2-X1)*(Y2-Y1));
    
    Q = (A+B+C+D)*E;
    return Q;
}

//double IDW(surfVec *depVec, double x, double y, double power, double radius)
/*
 Purpose:   interpolate a surface to obtain the value at a given x y
 
 Input variables:
 depVec      -  structure containing n by 3 surface vector
 x           -  longitude value of point
 y           -  latitude value of point
 
 Output variables:
 value       - the value of the surface at the input x y
 */
 /*

{
    if(radius<0)
    {
        printf("Error: Radius must be positive.\n");
    }
    
    double D[depVec->lengthCount];
    double V[depVec->lengthCount];
    double wV[depVec->lengthCount];
    double vcc[depVec->lengthCount];
    double sumV = 0;
    double sumwV = 0;
    double minD = 1;
    
    // determine each points radius away from the input point
    for(int i = 0; i < depVec->lengthCount; i++)
    {
        D[i] = sqrt(pow((x-depVec->Vec[i][1]),2)+pow((y-depVec->Vec[i][0]),2));
    }
    
    // determine if any of the points are exactly at the given input location (ie if D=0)
    int minInd = 0;
    for(int i = 0; i < depVec->lengthCount; i++)
    {
        if(i==0)
        {
            minD = D[i];
        }
        if(i!=0)
        {
            if(D[i]<minD)
            {
                minD = D[i];
                minInd = i;
            }
        }
    }
    
    if(minD==0)
    {
        printf("Error: Station has the value of an interpolated point.\n");
        return depVec->Vec[minInd][2]; // return the value of the exact point
    }
    
    // perform IDW
    int countA = 0;
    for(int i = 0; i < depVec->lengthCount; i++)
    {
        if(D[i]<radius)
        {
            vcc[countA] = depVec->Vec[i][2];
            wV[countA] = pow(D[i],power);
            V[countA] = vcc[countA]*wV[countA];
            sumV = sumV + V[countA];
            sumwV = sumwV + wV[countA];
            countA = countA + 1;
        }
    }
    
    double vint = sumV/sumwV;
    return vint;
}
*/