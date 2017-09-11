//
//  generateModelGrid.c
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

void generateFullModelGridGreatCircle(model_extent *MODEL_EXTENT, global_mesh *GLOBAL_MESH)
/*
 Purpose:   generate the grid of lat long and dep points using the point radial distance method
 
 Input variables:
 MODEL_EXTENT - struct containing the extent, spacing and version of the model
 *GLOBAL_MESH - structure containing the full model grid (lat, lon and depth points)
 
 Output variables:
 n.a.
 */
{
    GLOBAL_MESH->maxLat = -180;
    GLOBAL_MESH->minLat = 0;
    GLOBAL_MESH->maxLon = 0;
    GLOBAL_MESH->minLon = 180;
    
    GLOBAL_MESH->nX = MODEL_EXTENT->Xmax/MODEL_EXTENT->hLatLon;
    GLOBAL_MESH->nY = MODEL_EXTENT->Ymax/MODEL_EXTENT->hLatLon;
    GLOBAL_MESH->nZ = (MODEL_EXTENT->Zmax-MODEL_EXTENT->Zmin) / MODEL_EXTENT->hDep;

    
    if (ceil(GLOBAL_MESH->nX) != GLOBAL_MESH->nX)
    {
        printf("EXTENT_X / EXTENT_LATLON_SPACING does not yield a whole number.\n");
        exit(EXIT_FAILURE);
    }
    if (ceil(GLOBAL_MESH->nY) != GLOBAL_MESH->nY)
    {
        printf("EXTENT_X / EXTENT_LATLON_SPACING does not yield a whole number.\n");
        exit(EXIT_FAILURE);
    }
    if (ceil(GLOBAL_MESH->nZ) != GLOBAL_MESH->nZ)
    {
        printf("(EXTENT_ZMAX - EXTENT_Z_MAX) / EXTENT_Z_SPACING does not yield a whole number.\n");
        exit(EXIT_FAILURE);
    }

    if (GLOBAL_MESH->nZ != 1)
    {
        printf("Number of model points. nx: %i, ny: %i, nz: %i.\n", GLOBAL_MESH->nX, GLOBAL_MESH->nY, GLOBAL_MESH->nZ);
    }
    // enforce the grid is of allowable size
    if(GLOBAL_MESH->nX>=LON_GRID_DIM_MAX)
    {
        printf("nX exceeds the maximum allowable value of %i.\n",LON_GRID_DIM_MAX);
        exit(EXIT_FAILURE);
    }
    else if(GLOBAL_MESH->nY>=LAT_GRID_DIM_MAX)
    {
        printf("nY exceeds the maximum allowable value of %i.\n",LAT_GRID_DIM_MAX);
        exit(EXIT_FAILURE);
    }
    else if(GLOBAL_MESH->nZ>=DEP_GRID_DIM_MAX)
    {
        printf("nZ exceeds the maximum allowable value of %i.\n",DEP_GRID_DIM_MAX);
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < GLOBAL_MESH->nX; i++)
    {
        GLOBAL_MESH->X[i] = 0.5*MODEL_EXTENT->hLatLon+MODEL_EXTENT->hLatLon*(i)-0.5*MODEL_EXTENT->Xmax;
//        GLOBAL_MESH->X[i] = MODEL_EXTENT->hLatLon*(i);
    }
    
    for(int i = 0; i < GLOBAL_MESH->nY; i++)
    {
        GLOBAL_MESH->Y[i] = 0.5*MODEL_EXTENT->hLatLon+MODEL_EXTENT->hLatLon*(i)-0.5*MODEL_EXTENT->Ymax;
//        GLOBAL_MESH->Y[i] = MODEL_EXTENT->hLatLon*(i);
    }
    
    for(int i = 0; i < GLOBAL_MESH->nZ; i++)
    {
        GLOBAL_MESH->Z[i] = -1000*(MODEL_EXTENT->Zmin + MODEL_EXTENT->hDep*(i+0.5));
    }
    
//    double xmax, ymax;// zmax, xlen, ylen, zlen;
    
//    xmax = GLOBAL_MESH->X[GLOBAL_MESH->nX-1];
//    ymax = GLOBAL_MESH->Y[GLOBAL_MESH->nY-1];
//    zmax = GLOBAL_MESH->Z[GLOBAL_MESH->nZ-1];
    
//    xlen = xmax + 0.5*(GLOBAL_MESH->X[GLOBAL_MESH->nX-1] + GLOBAL_MESH->X[0]);
//    ylen = ymax + 0.5*(GLOBAL_MESH->Y[GLOBAL_MESH->nY-1] + GLOBAL_MESH->Y[0]);
//    zlen = zlen + 0.5*(GLOBAL_MESH->Z[GLOBAL_MESH->nZ-1] + GLOBAL_MESH->Z[0]);
    
//    double cosR, sinR;
//    cosR = cos(MODEL_EXTENT->originRot*RPERD);
//    sinR = sin(MODEL_EXTENT->originRot*RPERD);
    
    double arg;
    double cosA, sinA;
    double cosT, sinT;
    double cosP, sinP;
    double det;
    
    arg = MODEL_EXTENT->originRot*RPERD;
    cosA = cos(arg);
    sinA = sin(arg);
    
    arg = (90.0-MODEL_EXTENT->originLat)*RPERD;
    cosT = cos(arg);
    sinT = sin(arg);
    
    arg = MODEL_EXTENT->originLon*RPERD;
    cosP = cos(arg);
    sinP = sin(arg);
    
    double amat[9], ainv[9];
    
    amat[0] = cosA*cosT*cosP + sinA*sinP;
    amat[1] = sinA*cosT*cosP - cosA*sinP;
    amat[2] = sinT*cosP;
    amat[3] = cosA*cosT*sinP - sinA*cosP;
    amat[4] = sinA*cosT*sinP + cosA*cosP;
    amat[5] = sinT*sinP;
    amat[6] = -cosA*sinT;
    amat[7] = -sinA*sinT;
    amat[8] = cosT;
    
    det = amat[0]*(amat[4]*amat[8] - amat[7]*amat[5]) - amat[1]*(amat[3]*amat[8] - amat[6]*amat[5]) + amat[2]*(amat[3]*amat[7] - amat[6]*amat[4]);
    
    det = 1.0/det;
    ainv[0] = det*amat[0];
    ainv[1] = det*amat[3];
    ainv[2] = det*amat[6];
    ainv[3] = det*amat[1];
    ainv[4] = det*amat[4];
    ainv[5] = det*amat[7];
    ainv[6] = det*amat[2];
    ainv[7] = det*amat[5];
    ainv[8] = det*amat[8];


    double g0 = 0.0;//0.5*(MODEL_EXTENT->Ymax - MODEL_EXTENT->hLatLon)/ERAD;
    double b0 = 0.0;//0.5*(MODEL_EXTENT->Xmax - MODEL_EXTENT->hLatLon)/ERAD;
    
    double x, y;
    for(int iy = 0; iy < GLOBAL_MESH->nY; iy++)
    {
        for(int ix = 0; ix < GLOBAL_MESH->nX; ix++)
        {
            
            x = GLOBAL_MESH->X[ix];
            y = GLOBAL_MESH->Y[iy];
            gcproj(x,y,&GLOBAL_MESH->Lon[ix][iy],&GLOBAL_MESH->Lat[ix][iy],ERAD,g0,b0,amat,ainv);

            if( GLOBAL_MESH->maxLat < GLOBAL_MESH->Lat[ix][iy])
            {
                GLOBAL_MESH->maxLat = GLOBAL_MESH->Lat[ix][iy];
            }
            if( GLOBAL_MESH->maxLon < GLOBAL_MESH->Lon[ix][iy])
            {
                GLOBAL_MESH->maxLon = GLOBAL_MESH->Lon[ix][iy];
            }
            if( GLOBAL_MESH->minLat > GLOBAL_MESH->Lat[ix][iy])
            {
                GLOBAL_MESH->minLat = GLOBAL_MESH->Lat[ix][iy];
            }
            if( GLOBAL_MESH->minLon > GLOBAL_MESH->Lon[ix][iy])
            {
                GLOBAL_MESH->minLon = GLOBAL_MESH->Lon[ix][iy];
            }
        }
    }
    
    printf("Completed Generation of Model Grid.\n");
}


void generateFullModelGridPointRad(model_extent *MODEL_EXTENT, global_mesh *GLOBAL_MESH)
/*
 Purpose:   generate the grid of lat long and dep points using the point radial distance method
 
 Input variables:
    MODEL_EXTENT - struct containing the extent, spacing and version of the model
    *GLOBAL_MESH - structure containing the full model grid (lat, lon and depth points)

 Output variables:
    n.a.
 */
{

    // determine number of points in each orthogonal direction
	int nX = 2*MODEL_EXTENT->Xmax/MODEL_EXTENT->hLatLon;
	int nY = 2*MODEL_EXTENT->Ymax/MODEL_EXTENT->hLatLon;
	int nZ = (MODEL_EXTENT->Zmax-MODEL_EXTENT->Zmin)/MODEL_EXTENT->hDep;
    GLOBAL_MESH->maxLat = -180;
    GLOBAL_MESH->minLat = 0;
    GLOBAL_MESH->maxLon = 0;
    GLOBAL_MESH->minLon = 180;
    
    // esnure the number of points does not exceed that of the struct preallocation
	printf("nx: %i, ny: %i, nz: %i.\n", nX, nY, nZ);
    assert(nX<=LON_GRID_DIM_MAX);
    assert(nY<=LAT_GRID_DIM_MAX);
    assert(nZ<=DEP_GRID_DIM_MAX);
    
    //write values to the struct
    GLOBAL_MESH->nX = nX;
    GLOBAL_MESH->nY = nY;
    GLOBAL_MESH->nZ = nZ; 
    
	double X[LON_GRID_DIM_MAX], Y[LAT_GRID_DIM_MAX], Z[DEP_GRID_DIM_MAX];
    
    // loop over x y z arrays to insert values
	for(int i = 0; i < nX; i++)
	{
		X[i] = -1*MODEL_EXTENT->Xmax + MODEL_EXTENT->hLatLon*(i+0.5);
        GLOBAL_MESH->X[i] = X[i];
	}
    
    for(int i = 0; i < nY; i++)
	{
		Y[i] = -1*MODEL_EXTENT->Ymax + MODEL_EXTENT->hLatLon*(i+0.5);
        GLOBAL_MESH->Y[i] = Y[i];
	}
    
    for(int i = 0; i < nZ; i++)
	{
		Z[i] = MODEL_EXTENT->Zmin + MODEL_EXTENT->hDep*(i+0.5);
	}
    
    double locationXY[2];
    // calculate and insert values into struct
    singleGridPoint points;
    for(int ix = 0; ix < nX; ix++) //nX
    {
        for(int iy = 0; iy < nY; iy++) //xY
        {
            locationXY[0] = X[ix];
            locationXY[1] = Y[iy];
            
            points = XYtoLatLon(locationXY, MODEL_EXTENT->originLat, MODEL_EXTENT->originLon, MODEL_EXTENT->originRot);
            // determin the maximum lat long points of the model grid
            if( GLOBAL_MESH->maxLat < points.lat)
            {
                GLOBAL_MESH->maxLat = points.lat;
            }
            if( GLOBAL_MESH->maxLon < points.lon)
            {
                GLOBAL_MESH->maxLon = points.lon;
            }
            if( GLOBAL_MESH->minLat > points.lat)
            {
                GLOBAL_MESH->minLat = points.lat;
            }
            if( GLOBAL_MESH->minLon > points.lon)
            {
                GLOBAL_MESH->minLon = points.lon;
            }
            GLOBAL_MESH->Lat[ix][iy] = points.lat;
            GLOBAL_MESH->Lon[ix][iy] = points.lon;
        }
    }
    
    for(int iz = 0; iz < nZ; iz++)
    {
        GLOBAL_MESH->Z[iz] = -1000*Z[iz]; // convert to m (below ground is negative)
    }
    printf("Completed Generation of Model Grid.\n");
}


