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
//              isValueInArray
//=====================================================================

// Purpose - detetmine if an interger value is within a vector

int isValueInArray(int val, int *arr, int size)
{
    int i;
    for (i=0; i < size; i++)
    {
        if (arr[i] == val)
            return 1;
    }
    return 0;
}

//=====================================================================
//              isValueInTwoArrays
//=====================================================================

// Purpose - determine if two intergers are at the same indicie within two vectors

int isValueInTwoArrays(int valA, int valB, int *arrA, int *arrB, int size)
{
    int i;
    for (i=0; i < size; i++)
    {
        if ((arrA[i] == valA)&&(arrB[i] == valB))
        {
            return 1;
        }
    }
    return 0;
}

//=====================================================================
//              findTwinInds
//=====================================================================

// Purpose - determine the indice within two vectors that match two desired values

int findTwinInds(int valA, int valB, int *arrA, int *arrB, int size)
{
    int i;
    for (i=0; i < size; i++)
    {
        if ((arrA[i] == valA)&&(arrB[i] == valB))
        {
            return i;
        }
    }
    printf("Error in findTwinInds.\n");
    return -1;
    
}



//=====================================================================
//              LatLonToDistance
//=====================================================================

double LatLonToDistance(double locationLatLon[], double originLat, double originLon)
{
    
    double refLon, refLat;
    double lat, lon;
    double dx, dy, dz;
    double dist;
    
    lat = deg2rad(locationLatLon[0]);
    lon = deg2rad(locationLatLon[1]);
    
    refLon = deg2rad(originLon);
    refLat = deg2rad(originLat);
    
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

singleGridPoint XYtoLatLon(double locationXY[], double originLat, double originLon, double originRot)
/*
 Purpose:   calculate the latitude and longitude values of the input x y and reference coords
 
 Input variables:
 locationXY     - array housing x y point
 refCoords      - a structure with the following elements:
 .mlon , .mlat - values of lon and lat at the origin of the XY domain
 .mrot - the rotation of the model domain (in deg) measured clockwise from south (i.e. mrot=0 means that Y points south, X points east, while mrot=90 means that Y points west and X points south).
 
 Output variables:
 points    - location of the point in lat lon format
 
 */
{
    // compute the distance
    double h = sqrt(pow(locationXY[1],2) + pow(locationXY[0],2));
    double netAngle = 90 - rad2deg(atan2(-locationXY[1],locationXY[0]));
    // remove the effect of reference coordinate axis rotation
    double bearing = netAngle + originRot;
    
    double refLatLon[2];
    refLatLon[0] = originLon;
    refLatLon[1] = originLat;
    
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
 points      - the latitude and longitude
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
    return angDeg * pi_180;
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

double findMaxValueArray(int nPts, double pts[5])
{
    double max = -1e6;
    for(int i = 0; i < nPts; i++)
    {
        if (max < pts[i])
        {
            max = pts[i];
        }
    }
    return max;
}

double findMinValueArray(int nPts, double pts[5])
{
    double min = 1e6;
    for(int i = 0; i < nPts; i++)
    {
        if (min > pts[i])
        {
            min = pts[i];
        }
    }
    return min;
}


double interpolateQuad(double lons[4], double lats[4], double values[4], double lonA, double latA)
{
    
    printf("%lf %lf\n", lonA,latA);
    for (int k = 0; k < 4; k++)
    {
        printf("%lf %lf %lf\n", lons[k], lats[k],values[k]);
    }
    double N1, N2, N3, N4;
    double AA, BB, CC;
    
    double a0, a1, a2, a3, b0, b1, b2, b3;
    double tempA, e, n, interpVal;
    
    double AI0[4] = {0.25,0.25,0.25,0.25};
    double AI1[4] = {-0.25,0.25,0.25,-0.25};
    double AI2[4] =  {-0.25,-0.25,0.25,0.25};
    double AI3[4] = {0.25,-0.25,0.25,-0.25};
    
    
    double a[4] = {0};
    double b[4] = {0};
    
    for (int i = 0 ; i < 4; i++)
    {
        
        a[0] += AI0[i]*lons[i];
        a[1] += AI1[i]*lons[i];
        a[2] += AI2[i]*lons[i];
        a[3] += AI3[i]*lons[i];
        
        b[0] += AI0[i]*lats[i];
        b[1] += AI1[i]*lats[i];
        b[2] += AI2[i]*lats[i];
        b[3] += AI3[i]*lats[i];
    }
    
    a0 = a[0];
    a1 = a[1];
    a2 = a[2];
    a3 = a[3];
    
    b0 = b[0];
    b1 = b[1];
    b2 = b[2];
    b3 = b[3];
    
    AA = a1*b3 - a3*b1;
    BB = a1*b2 - b3*lonA + a0*b3 - b1*a2 + latA*a3 -b0*a3;
    CC = a2*latA - a2*b0 -b2*lonA + a0*b2;
    
    tempA = sqrt(BB*BB -4*AA*CC);
    
    e = (-BB + tempA)/(2*AA);
    n = (lonA - a0 -a1*e)/(a2+a3*e);
    
    N1 = (0.25)*(1-e)*(1-n)*values[0];
    N2 = (0.25)*(1+e)*(1-n)*values[1];
    N3 = (0.25)*(1+e)*(1+n)*values[2];
    N4 = (0.25)*(1-e)*(1+n)*values[3];
    
    interpVal = N1 + N2 + N3 + N4;
    printf("%lf\n",interpVal);
    
    return interpVal;
    
    
    
    
}



void gcproj(double xf,double yf,double *rlon,double *rlat,double ref_rad,double g0,double b0,double amat[8],double ainv[8])
{
    double xp, yp, zp;
    double xg, yg, zg;
    double arg;
    double cosG, sinG;
    double cosB, sinB;
    
    double rperd = RPERD;
    
    arg = (xf)/(ref_rad) - (b0);
    cosB = cos(arg);
    sinB = sin(arg);
    
    arg = (yf)/(ref_rad) - (g0);
    cosG = cos(arg);
    sinG = sin(arg);
    
    arg = sqrt(1.0 + sinB*sinB*sinG*sinG);
    xp = sinG*cosB*arg;
    yp = sinB*cosG*arg;
    zp = sqrt(1.0 - xp*xp - yp*yp);
    
    xg = xp*amat[0] + yp*amat[1] + zp*amat[2];
    yg = xp*amat[3] + yp*amat[4] + zp*amat[5];
    zg = xp*amat[6] + yp*amat[7] + zp*amat[8];
    
    /*
     RWG 04/10/2012
     Following is incorrect for southern hemisphere.  Replace with the following
     additional conditionals on zg
     
     arg = sqrt(xg*xg + yg*yg)/zg;
     (*rlat) = 90.0 - atan(arg)/rperd;
     */
    
    if(zg != (double)(0.0))
    {
        arg = sqrt(xg*xg + yg*yg)/zg;
        (*rlat) = 90.0 - atan(arg)/rperd;
        if(zg < (double)(0.0))
        {
            (*rlat) = (*rlat) - 180.0;
        }
    }
    else
        (*rlat) = 0.0;
    
    if(xg != (double)(0.0))
    {
        arg = yg/xg;
        (*rlon) = atan(arg)/rperd;
    }
    else
        (*rlon) = 0.0;
    
    /*
     RWG 05/08/08
     This is incorrect.  Replaced with following conditional on 'xg'.
     if(yg < (double)(0.0))
     */
    if(xg < (double)(0.0))
    {
        (*rlon) = (*rlon) - 180.0;
    }
    
    
    while((*rlon) < (double)(-180.0))
    {
        (*rlon) = (*rlon) + 360.0;
    }
}

void calcAndSaveZThreshold(char *OUTPUT_DIR, partial_global_mesh *PARTIAL_GLOBAL_MESH, partial_global_qualities *PARTIAL_GLOBAL_QUALITIES, calculation_log *CALCULATION_LOG, char *Z_THRESHOLD, int latInd)
{
    double Z_WRITE = 0;
    double Z_THRESH_DOUBLE = atof(Z_THRESHOLD);
    int i, j;
    for(i = 0; i < PARTIAL_GLOBAL_MESH->nX; i++)
    {
        Z_WRITE = 0;
        for (j = 0; j < PARTIAL_GLOBAL_MESH->nZ-1; j++)
        {
            if( PARTIAL_GLOBAL_QUALITIES->Vs[i][j] >= Z_THRESH_DOUBLE)
            {
                Z_WRITE = PARTIAL_GLOBAL_MESH->Z[j];
                break;
            }
        }
        if (Z_WRITE == 0)
        {
            printf("%i %i %lf %lf.\n", i, j, PARTIAL_GLOBAL_QUALITIES->Vs[i][j], Z_THRESH_DOUBLE);
            printf("Z_Threshold outside of limits.\n");
            exit(EXIT_FAILURE);
        }
        writeZThresholdFile(OUTPUT_DIR, PARTIAL_GLOBAL_MESH->Lat[i], PARTIAL_GLOBAL_MESH->Lon[i], Z_WRITE, latInd, Z_THRESHOLD);
    }
    
}

void writeZThresholdFile(char *OUTPUT_DIR, double Lat, double Lon, double Z_WRITE, double latInd, char *Z_THRESHOLD)
{
    char zFile[MAX_FILENAME_STRING_LEN];
    FILE *zFileTxt = NULL;
    sprintf(zFile,"%s/Z/Z_%s.txt",OUTPUT_DIR,Z_THRESHOLD);
    
    if( latInd == 0) // if first time generate file
    {
        zFileTxt = fopen(zFile, "w");
        fprintf(zFileTxt,"Lat\tLon\tZ_%s(m)\n",Z_THRESHOLD);
        fprintf(zFileTxt,"%lf\t%lf\t%lf\n",Lat,Lon,Z_WRITE);
    }
    else // append to existing file
    {
        zFileTxt = fopen(zFile, "a");
        fprintf(zFileTxt,"%lf\t%lf\t%lf\n",Lat,Lon,Z_WRITE);
    }
    fclose(zFileTxt);
    
    
}




void calcAndSaveVs(char *OUTPUT_DIR, partial_global_mesh *PARTIAL_GLOBAL_MESH, partial_global_qualities *PARTIAL_GLOBAL_QUALITIES, calculation_log *CALCULATION_LOG, char *VS_DEPTH, int latInd)
{
    double Vs;
    double VsTotal;
    double dZ = PARTIAL_GLOBAL_MESH->Z[0] - PARTIAL_GLOBAL_MESH->Z[1];
    
    
    for( int i = 0; i < PARTIAL_GLOBAL_MESH->nX; i++)
    {
        Vs = 0;
        //        printf("%i.\n",PARTIAL_GLOBAL_MESH->nZ-1);
        for (int j = 0; j < PARTIAL_GLOBAL_MESH->nZ; j++)
        {
            Vs += dZ/PARTIAL_GLOBAL_QUALITIES->Vs[i][j];
        }
        VsTotal = -PARTIAL_GLOBAL_MESH->Z[PARTIAL_GLOBAL_MESH->nZ-1]/Vs; // in km/s
        //        printf("%lf %lf %lf.\n",VsDepth,PARTIAL_GLOBAL_MESH->Z[PARTIAL_GLOBAL_MESH->nZ-1],Vs );
        writeVsFile(OUTPUT_DIR, PARTIAL_GLOBAL_MESH->Lat[i], PARTIAL_GLOBAL_MESH->Lon[i], VsTotal, latInd, VS_DEPTH);
    }
    
    
    
    
    
}

void writeVsFile(char *OUTPUT_DIR, double Lat, double Lon, double VsTotal, double latInd, char *VS_DEPTH)
{
    char vsFile[MAX_FILENAME_STRING_LEN];
    FILE *vsFileTxt = NULL;
    sprintf(vsFile,"%s/Vs/Vs_%s.txt",OUTPUT_DIR,VS_DEPTH);
    
    if( latInd == 0) // if first time generate file
    {
        vsFileTxt = fopen(vsFile, "w");
        if (vsFileTxt == NULL)
        {
            printf("Unable to open file to write data to (%s).\n",vsFile);
            exit(EXIT_FAILURE);
        }
        fprintf(vsFileTxt,"Lat\tLon\tVs_%s(km/s)\n",VS_DEPTH);
        fprintf(vsFileTxt,"%lf\t%lf\t%lf\n",Lat,Lon,VsTotal);
    }
    else // append to existing file
    {
        vsFileTxt = fopen(vsFile, "a");
        if (vsFileTxt == NULL)
        {
            printf("Unable to append data to file (%s).\n",vsFile);
            exit(EXIT_FAILURE);
        }
        fprintf(vsFileTxt,"%lf\t%lf\t%lf\n",Lat,Lon,VsTotal);
    }
    fclose(vsFileTxt);
    
    
}







