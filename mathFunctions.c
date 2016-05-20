//
//  mathFunctions.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 9/09/14.
//  Copyright (c) 2014 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <stdint.h>
#include "constants.h"
#include "structs.h"
#include "functions.h"

//=====================================================================
//              LatLonToDistance
//=====================================================================

double LatLonToDistance(double locationLatLon[], modOrigin modelOrigin)
{
    
    double refLon, refLat;
    double lat, lon;
    double dx, dy, dz;
    double dist;
    
    lat = deg2rad(locationLatLon[0]);
    lon = deg2rad(locationLatLon[1]);
    
    refLon = deg2rad(modelOrigin.mlon);
    refLat = deg2rad(modelOrigin.mlat);
    
    double dLon = lon-refLon;
    
    dz = sin(lat) - sin(refLat);
    dx = (cos(dLon) * cos(lat) - cos(refLat));
    dy = sin(dLon) * cos(lat);
    
    return dist = asin(sqrt(dx*dx + dy*dy + dz*dz)/2) * 2 * EARTH_RADIUS_MEAN;
    
    
}

//=====================================================================
//              rotateGrid
//=====================================================================
singleGridPoint rotateGrid(double rotAngle, double X, double Y)
{
    singleGridPoint rotGridPoint;
    double rotAngleRad;
    
    rotAngleRad = deg2rad(-rotAngle);
    rotGridPoint.X = (X*cos(rotAngleRad) + -Y*sin(rotAngleRad));
    rotGridPoint.Y = (X*sin(rotAngleRad) + Y*cos(rotAngleRad));
    
    return rotGridPoint;
}



//=====================================================================
//              XYtoLatLon
//=====================================================================

singleGridPoint XYtoLatLon(double locationXY[], modOrigin modelOrigin)
/*
 Purpose:   calculate the latitude and longitude values of the input x y and reference coords
 
 Input variables:
 locationXY     - array housing x y point
 refCoords      - a structure with the following elements:
 .mlon , .mlat - values of lon and lat at the origin of the XY domain
 .mrot - the rotation of the model domain (in deg) measured clockwise from south (i.e. mrot=0 means that Y points south, X points east, while mrot=90 means that Y points west and X points south).
 
 Output variables:
 indexStruct    - location of the point in lat lon format
 
 */
{
    // compute the distance
    double h = sqrt(pow(locationXY[1],2) + pow(locationXY[0],2));
    double netAngle = 90 - rad2deg(atan2(-locationXY[1],locationXY[0]));
    // remove the effect of reference coordinate axis rotation
    double bearing = netAngle + modelOrigin.mrot;
    
    double refLatLon[2];
    refLatLon[0] = modelOrigin.mlon;
    refLatLon[1] = modelOrigin.mlat;
    
    singleGridPoint points;
    points = pointRadialDistance(refLatLon, -bearing, h);
    return points;
}



//=====================================================================
//              pointRadialDistance
//=====================================================================

singleGridPoint pointRadialDistance(double refLatLon[],double bearing, double distance)
/*
 Purpose:   return final coordinates in degrees given a reference coord, bearing and distance
 
 Input variables:
 refLatLon   - reference latiude and longitude vector
 bearing     - the bearing from the reference coord
 distance    - the distance from the reference coord
 
 Output variables:
 return      - the latitude and longitude
 */
{
    
    double epsilon = 0.00001;
    
    double lat1 = refLatLon[1];
    double lon1 = refLatLon[0];
    
    double rlat1 = deg2rad(lat1);
    double rlon1 = deg2rad(lon1);
    double rbearing = deg2rad(bearing);
    double rdistance = distance / EARTH_RADIUS_MEAN;
    
    double rlat, rlon;
    rlat = asin( sin(rlat1) * cos(rdistance) + cos(rlat1) * sin(rdistance) * cos(rbearing));
    
    if(fabs(cos(rlat)) < epsilon)
    {
        rlon = rlon1;
    }
    else
    {
        rlon = fmod((rlon1 - asin( sin(rbearing)* sin(rdistance) / cos(rlat) ) + M_PI ) , (2*M_PI) ) - M_PI;
    }
    
    singleGridPoint points;
    points.lat = rad2deg(rlat);
    points.lon = rad2deg(rlon);
    
    return points;
}



//=====================================================================
//              rad2deg
//=====================================================================

double rad2deg(double angRad)
/*
 Purpose:   convert radians to degrees
 
 Input variables:
 angleRad  - angle in radians
 
 Output variables:
 return    - the angle in degrees
 */
{
    return angRad*180/M_PI;
}


//=====================================================================
//              deg2rad
//=====================================================================

double deg2rad(double angDeg)
/*
 Purpose:   convert degrees to radians
 
 Input variables:
 angDeg  - angle in degrees
 
 Output variables:
 return  - the angle in radians
 */
{
    return angDeg*M_PI/180;
}


//=====================================================================
//              float_swap
//=====================================================================

float float_swap(const float inFloat)
/*
 Purpose:   convert endianess
 
 Input variables:
 value - float
 
 Output variables:
 return  - byte swapped to opposite endian
 */
{
    float retVal;
    char *floatToConvert = ( char* ) & inFloat;
    char *returnFloat = ( char* ) & retVal;
    // swap the bytes into a temporary buffer
    returnFloat[0] = floatToConvert[3];
    returnFloat[1] = floatToConvert[2];
    returnFloat[2] = floatToConvert[1];
    returnFloat[3] = floatToConvert[0];
    
    return retVal;
}



//=====================================================================
//              endian
//=====================================================================
/*
 Purpose:   check endianness of system
 
 Input variables:
 n.a.
 
 Output variables:
 return  - int designating big or little endianess 
 #define LITTLE_ENDIAN_CONFIRMED 0
 #define BIG_ENDIAN_CONFIRMED    1
 */

int endian(void)
{
    int i = 1;
    char *p = (char *)&i;
    
    if (p[0] == 1)
        return 0;
    else
        return 1;
}