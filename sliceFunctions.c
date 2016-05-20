//
//  sliceFunctions.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 1/03/15.
//  Copyright (c) 2015 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "constants.h"
#include "structs.h"
#include "functions.h"


void generateSlice(modOrigin modelOrigin, sliceExtent sliceBounds, modVersion modelVersion, char *outputDirectory)
{
    globalDataValues *globDataVals = NULL;
    surfaceDepthsGlobal *surfDepsGlob = NULL;
    sliceExtractData *sliceData;
    sliceData = malloc(sizeof(sliceExtractData));
    generateSliceXYpoints(sliceData, modelOrigin, sliceBounds);
    
    
    // generate the model grid
    gridStruct *location;
    location = malloc(sizeof(gridStruct));
    location->saveSurfaceDepths = 1;
    int nZ = (sliceBounds.zMax-sliceBounds.zMin)/sliceBounds.resZ;
    
    location->nX = sliceData->nPts;
    location->nY = 1;
    location->nZ = nZ;
    
    // ensure the number of points does not exceed that of the struct preallocation
    printf("nx: %i, ny: %i, nz: %i.\n", location->nX, location->nY, location->nZ);
    assert(location->nX<=LON_GRID_DIM_MAX);
    assert(location->nY<=LAT_GRID_DIM_MAX);
    assert(location->nZ<=DEP_GRID_DIM_MAX);
    
    for(int i = 0; i < sliceData->nPts; i++)
    {
        location->Lat[i][0] = sliceData->latPts[i];
        location->Lon[i][0] = sliceData->lonPts[i];
    }
    for(int j = 0; j < nZ; j++)
    {
        location->Z[j] = -1000*(sliceBounds.zMin + sliceBounds.resZ*(j+0.5));
    }

    
    // obtain surface filenames based off version number
    surfNames surfSubModNames;
    surfSubModNames = getSurfSubModNames(modelVersion);
    
    // determine the depths of each surface for each lat lon point
    surfDepsGlob = getSurfaceValues(location, surfSubModNames, outputDirectory);
    
    // assign values
    globDataVals = assignValues(modelVersion, location, surfSubModNames, surfDepsGlob, outputDirectory);
    
    
    
    
    // loop over points and extract values
    for(int i = 0; i < sliceData->nPts; i++)
    {
        for(int k = 0; k < location->nZ; k++)
        {
            sliceData->Vp[i][k] = globDataVals->Vp[i][0][k];
            sliceData->Vs[i][k] = globDataVals->Vs[i][0][k];
            sliceData->Rho[i][k] = globDataVals->Rho[i][0][k];

        }
    }
    
    if (sliceBounds.saveSlices == 1)
    {
        // generate file for writing
        FILE *fp2;
        double currRho, currVp, currVs;
        static int sliceNumber = 0;
        sliceNumber += 1;
        char fName[128];
        
        char sliceDir[128];
        sprintf(sliceDir,"%s/Slices",outputDirectory);
        
        struct stat st = {0};
        if (stat(sliceDir, &st) == -1)
        {
            // create output directory and the velocity model
            mkdir(sliceDir, 0700);
        }
        sprintf(fName,"%s/GeneratedSlice%i.txt",sliceDir,sliceNumber);
        fp2 = fopen(fName,"w");
        fprintf(fp2,"Generated slice #%i.\n",sliceNumber);
        fprintf(fp2,"Slice_Horizontal_Resolution\t%i\n",sliceBounds.resXY);
        fprintf(fp2,"LatA:\t%lf\n",sliceData->latPts[0]);
        fprintf(fp2,"LatB:\t%lf\n",sliceData->latPts[sliceData->nPts-1]);
        fprintf(fp2,"LonA:\t%lf\n",sliceData->lonPts[0]);
        fprintf(fp2,"LonB:\t%lf\n",sliceData->lonPts[sliceData->nPts-1]);
        fprintf(fp2,"Lat \t Lon \t Depth \t Vp \t Vs \t Rho\n");
        for(int i = 0; i < sliceData->nPts; i++)
        {
            for(int m = 0; m < location->nZ; m++)
            {
                currVp = sliceData->Vp[i][m];
                currRho = sliceData->Rho[i][m];
                currVs = sliceData->Vs[i][m];
                fprintf(fp2, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",sliceData->latPts[i],sliceData->lonPts[i], location->Z[m], currVp, currVs, currRho);
                
            }
            
        }
        fclose(fp2);
        printf("High resolution velocity model slice file save complete.\n");
    }



	free(location);
	free(sliceData);
    free(globDataVals);
    free(surfDepsGlob);
}

void extractSlice(gridStruct *location, modOrigin modelOrigin, sliceExtent sliceBounds, globalDataValues *globDataVals, char *outputDirectory, int sliceNumber)
{
    
    sliceExtractData *sliceData;
    sliceData = malloc(sizeof(sliceExtractData));
    generateSliceXYpoints(sliceData, modelOrigin, sliceBounds);

    
    // loop over points
    int xInd = 0;
    int yInd = 0;
    double X1, X2, Y1, Y2;
    double Q11Vp, Q12Vp, Q21Vp, Q22Vp;
    double Q11Vs, Q12Vs, Q21Vs, Q22Vs;
    double Q11Rho, Q12Rho, Q21Rho, Q22Rho;
    
    for(int i = 0; i < sliceData->nPts; i++)
    {
        // if x or y value is outside of the bound of the dataset return NaNs.
//        printf("%lf %lf %lf\n", sliceData->xPts[i], location->X[0],location->X[location->nX-1]);
//        printf("%lf %lf %lf\n", sliceData->yPts[i], location->Y[0],location->Y[location->nY-1]);

        if(sliceData->xPts[i]<location->X[0] || sliceData->xPts[i]>location->X[location->nX-1] || sliceData->yPts[i]>location->Y[location->nY-1] || sliceData->yPts[i]<location->Y[0] )
        {
            for(int k = 0; k < location->nZ; k++)
            {
                sliceData->Vp[i][k] = NAN;
                sliceData->Vs[i][k] = NAN;
                sliceData->Rho[i][k] = NAN;
            }
        }
        else  // get indice of nearby points
        {
            for(int k = 0; k < location->nX; k++)
            {
                if(location->X[k]>=sliceData->xPts[i])
                {
                    xInd = k-1;
                    break;
                }
            }
            for(int j = 0; j < location->nY; j++)
            {
                if(location->Y[j]>=sliceData->yPts[i])
                {
                    yInd = j-1;
                    break;
                }
            }
            // interpolate points
            X1 = location->X[xInd];
            X2 = location->X[xInd+1];
            Y1 = location->Y[yInd];
            Y2 = location->Y[yInd+1];
            
//            double lat1, lat2, lon1, lon2;
//            lat1 = location->Lat[xInd][yInd];
//            lat2 = location->Lat[xInd+1][yInd+1];
//            lon1 = location->Lon[xInd][yInd];
//            lon2 = location->Lon[xInd+1][yInd+1];
//            
//            printf("%lf %lf %lf %lf\n", X1, X2, Y1,Y2);
//            printf("%lf %lf\n",sliceData->xPts[i], sliceData->yPts[i]);
//            printf("%lf %lf %lf %lf\n", lat1, lat2, lon1, lon2);
//            printf("%lf %lf\n",sliceData->latPts[i], sliceData->lonPts[i]);
            
            for(int k = 0; k < location->nZ; k++)
            {
                // Vs
                Q11Vs = globDataVals->Vs[xInd][yInd][k];
                Q12Vs = globDataVals->Vs[xInd][yInd+1][k];
                Q21Vs = globDataVals->Vs[xInd+1][yInd][k];
                Q22Vs = globDataVals->Vs[xInd+1][yInd+1][k];
                
//                printf("%lf %lf %lf %lf\n", Q11Vs, Q21Vs, Q22Vs, Q12Vs);
                
                sliceData->Vs[i][k] = biLinearInterpolation(X1, X2, Y1, Y2, Q11Vs, Q12Vs, Q21Vs, Q22Vs, sliceData->xPts[i], sliceData->yPts[i]);
                
                // Vp
                Q11Vp = globDataVals->Vp[xInd][yInd][k];
                Q12Vp = globDataVals->Vp[xInd][yInd+1][k];
                Q21Vp = globDataVals->Vp[xInd+1][yInd][k];
                Q22Vp = globDataVals->Vp[xInd+1][yInd+1][k];
//                printf("%lf %lf %lf %lf\n", Q11Vp, Q21Vp, Q22Vp, Q12Vp);

                
                sliceData->Vp[i][k] = biLinearInterpolation(X1, X2, Y1, Y2, Q11Vp, Q12Vp, Q21Vp, Q22Vp, sliceData->xPts[i], sliceData->yPts[i]);
                
                // Rho
                Q11Rho = globDataVals->Rho[xInd][yInd][k];
                Q12Rho = globDataVals->Rho[xInd][yInd+1][k];
                Q21Rho = globDataVals->Rho[xInd+1][yInd][k];
                Q22Rho = globDataVals->Rho[xInd+1][yInd+1][k];
                
                sliceData->Rho[i][k] = biLinearInterpolation(X1, X2, Y1, Y2, Q11Rho, Q12Rho, Q21Rho, Q22Rho, sliceData->xPts[i], sliceData->yPts[i]);
            }
        }
        
    }
    char sliceDir[128];
    sprintf(sliceDir,"%s/Slices",outputDirectory);

    struct stat st = {0};
    if (stat(sliceDir, &st) == -1)
    {
        // create output directory and the velocity model
        mkdir(sliceDir, 0700);
    }
    
    // generate file for writing
    FILE *fp2;
    double currRho, currVp, currVs;
    char fName[128];
    sprintf(fName,"%s/ExtractedSlice%i.txt",sliceDir,sliceNumber);
    fp2 = fopen(fName,"w");
    fprintf(fp2,"Extracted slice #%i.\n",sliceNumber);
    fprintf(fp2,"Slice_Horizontal_Resolution\t%i\n",sliceBounds.resXY);
    fprintf(fp2,"LatA:\t%lf\n",sliceData->latPts[0]);
    fprintf(fp2,"LatB:\t%lf\n",sliceData->latPts[sliceData->nPts-1]);
    fprintf(fp2,"LonA:\t%lf\n",sliceData->lonPts[0]);
    fprintf(fp2,"LonB:\t%lf\n",sliceData->lonPts[sliceData->nPts-1]);
    fprintf(fp2,"Lat \t Lon \t Depth \t Vp \t Vs \t Rho\n");
    for(int i = 0; i < sliceData->nPts; i++)
    {
        for(int m = 0; m < location->nZ; m++)
        {
            currVp = sliceData->Vp[i][m];
            currRho = sliceData->Rho[i][m];
            currVs = sliceData->Vs[i][m];
            fprintf(fp2, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",sliceData->latPts[i],sliceData->lonPts[i], location->Z[m], currVp, currVs, currRho);
            
        }
        
    }
    
    fclose(fp2);
    free(sliceData);
    
}




globalDataValues* loadCvmDataAll(gridStruct *location, char *outputDirectory)
{
    
    // perform endian check
    int endianInt;
    endianInt = endian();
    
    globalDataValues *globDataVals;
    globDataVals = malloc(sizeof(globalDataValues));

    char veloModDir[128];
    sprintf(veloModDir,"%s/Velocity_Model",outputDirectory);
    
    FILE *fvp, *fvs, *frho;
    char vp3dfile[64];
    sprintf(vp3dfile,"%s/vp3dfile.p",veloModDir);
    
    char vs3dfile[64];
    sprintf(vs3dfile,"%s/vs3dfile.s",veloModDir);
    
    char rho3dfile[64];
    sprintf(rho3dfile,"%s/rho3dfile.d",veloModDir);
    
    float *vp, *vs, *rho;
    int bsize, ip;
    
    fvp = fopen(vp3dfile,"r");
    fvs = fopen(vs3dfile,"r");
    frho = fopen(rho3dfile,"r");
    
    bsize = location->nX*location->nZ*sizeof(float);
    vp = (float*) malloc(bsize);
    vs = (float*) malloc(bsize);
    rho = (float*) malloc(bsize);
    
    float vsRead, vpRead, rhoRead;
    
    printf("Reading binary files.\n");
    for(int iy = 0; iy < location->nY; iy++)
    {
        //increment a counter
//        printf("%d / %d complete \n",iy+1,location->nY);
        printf("\rReading velocity model from file %d%% complete.", iy*100/location->nY);
        fflush(stdout);
        
        //now read the obtained file in binary
        fread(vp,sizeof(vp[0]),location->nX*location->nZ,fvp);
        fread(vs,sizeof(vs[0]),location->nX*location->nZ,fvs);
        fread(rho,sizeof(rho[0]),location->nX*location->nZ,frho);
        for(int iz = 0; iz < location->nZ; iz++)
        {
            for (int ix = 0; ix < location->nX; ix++)
            {
                ip = ix + iz * location->nX;  //index counter
                vpRead = vp[ip];
                vsRead = vs[ip];
                rhoRead = rho[ip];

                if (endianInt == 1) // big endian, implement byteswap
                {
                    globDataVals->Vp[ix][iy][iz] = float_swap(vpRead);
                    globDataVals->Vs[ix][iy][iz] = float_swap(vsRead);
                    globDataVals->Rho[ix][iy][iz] = float_swap(rhoRead);
//                    printf("swappingRead\n");
                }
                else if (endianInt == 0) // system is little endain, read as is.
                {
                    globDataVals->Vp[ix][iy][iz] = vpRead;
                    globDataVals->Vs[ix][iy][iz] = vsRead;
                    globDataVals->Rho[ix][iy][iz] = rhoRead;
                }
            }
        }
    }
    printf("\rReading velocity model from file 100%% complete.");
    fflush(stdout);
    printf("\n");

    
    free(vp);
    free(vs);
    free(rho);
    fclose(fvp);
    fclose(fvs);
    fclose(frho);
    printf("Binary file read complete.\n");
    return globDataVals;
}





void generateSliceXYpoints(sliceExtractData *sliceData, modOrigin modelOrigin, sliceExtent sliceBounds)
{
    int nBranches = sliceBounds.nSections;
    int nGrdPts;
    double deltaLat;
    double deltaLon;
    int count = 0;
    double distX, distY;
    double locationXY[2];
    double dLatRot, dLonRot;
    singleGridPoint rotatedGridPoint;
    
    
    for(int i = 0; i < nBranches; i++)
    {
        nGrdPts = sliceBounds.resXY;
        deltaLat = (sliceBounds.latPtsSlice[i]-sliceBounds.latPtsSlice[i+1])/nGrdPts;
        deltaLon = (sliceBounds.lonPtsSlice[i]-sliceBounds.lonPtsSlice[i+1])/nGrdPts;
        for(int j = 0; j < nGrdPts+1; j++)
        {
            sliceData->lonPts[count] = sliceBounds.lonPtsSlice[i]-j*deltaLon;
            sliceData->latPts[count] = sliceBounds.latPtsSlice[i]-j*deltaLat;
            
            locationXY[0] = modelOrigin.mlat;
            locationXY[1] = sliceData->lonPts[count];
            
            distX = LatLonToDistance(locationXY, modelOrigin);
            
            locationXY[0] =sliceData->latPts[count];
            locationXY[1] = modelOrigin.mlon;
            
            distY = LatLonToDistance(locationXY, modelOrigin);
            
            dLatRot = modelOrigin.mlat - sliceData->latPts[count];
            dLonRot = modelOrigin.mlon - sliceData->lonPts[count];
            
            
            if(dLatRot < 0)
            {
                distY = -distY;
            }
            
            if (dLonRot > 0)
            {
                distX = - distX;
            }
            
            rotatedGridPoint = rotateGrid(modelOrigin.mrot, distX, distY);
            
            sliceData->xPts[count] = rotatedGridPoint.X;
            sliceData->yPts[count] = rotatedGridPoint.Y;
//            printf("%lf %lf\n",sliceData->xPts[count],sliceData->yPts[count]);
            count += 1;
        }
    }
    sliceData->nPts = count;
    assert(sliceData->nPts<=MAX_NUM_SLICE_GRIDPTS);
}






