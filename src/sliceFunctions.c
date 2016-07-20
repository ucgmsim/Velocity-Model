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


void extractSlicesFromBinaryFiles(char *OUTPUT_DIR, gen_extract_velo_mod_call GEN_EXTRACT_VELO_MOD_CALL, calculation_log *CALCULATION_LOG, global_mesh *GLOBAL_MESH, model_extent *MODEL_EXTENT)
{
    slice_parameters *SLICE_PARAMETERS;
    SLICE_PARAMETERS = readExtractedSliceParametersFile(GEN_EXTRACT_VELO_MOD_CALL.EXTRACTED_SLICE_PARAMETERS_DIRECTORY);
    
    global_data_for_interpolation *GLOBAL_DATA_FOR_INTERPOLATION;
    GLOBAL_DATA_FOR_INTERPOLATION = malloc(sizeof(global_data_for_interpolation));
    GLOBAL_DATA_FOR_INTERPOLATION->nPts = 0;
    
    // obtain lat-lon points
    for( int i = 0; i < SLICE_PARAMETERS->nSlices; i++)
    {
        GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_PARAMETERS[i] = malloc(sizeof(individual_slice_parameters));
        GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[i] = malloc(sizeof(individual_slice_data));
        
        GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_PARAMETERS[i]->resXY = SLICE_PARAMETERS->LatLonRes[i];
        GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_PARAMETERS[i]->latPtsSlice[0] = SLICE_PARAMETERS->latA[i];
        GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_PARAMETERS[i]->latPtsSlice[1] = SLICE_PARAMETERS->latB[i];
        GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_PARAMETERS[i]->lonPtsSlice[0] = SLICE_PARAMETERS->lonA[i];
        GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_PARAMETERS[i]->lonPtsSlice[1] = SLICE_PARAMETERS->lonB[i];
        
        generateSlicePoints(GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[i], GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_PARAMETERS[i], GLOBAL_MESH);
        
        generateGlobalIndsForRead(GLOBAL_MESH, GLOBAL_DATA_FOR_INTERPOLATION, MODEL_EXTENT, i);
        
    }
    
    globalIndReduction(GLOBAL_DATA_FOR_INTERPOLATION);
    
    // read necessary points
    readGlobalDataPointsForInterpolation(OUTPUT_DIR, GLOBAL_DATA_FOR_INTERPOLATION, GLOBAL_MESH, CALCULATION_LOG);
    printf("Completed read of binary files for interpolation.\n");

    // interpolate
    for( int i = 0; i < SLICE_PARAMETERS->nSlices; i++)
    {
        interpolateIndividualSlice(GLOBAL_MESH, GLOBAL_DATA_FOR_INTERPOLATION, i);
        
        writeInterpolatedSlice(OUTPUT_DIR, GLOBAL_DATA_FOR_INTERPOLATION, GLOBAL_MESH, CALCULATION_LOG, i);
        printf("Slice %i of %i complete.\n",i+1,SLICE_PARAMETERS->nSlices);
    }
    writeSliceParametersLogFile(OUTPUT_DIR, SLICE_PARAMETERS, MODEL_EXTENT, GLOBAL_MESH, CALCULATION_LOG, "EXTRACTED");
    printf("Completed write of slice parameters log file.\n");

    // free data
    for (int j = 0; j < SLICE_PARAMETERS->nSlices; j++)
    {
        free(GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_PARAMETERS[j]);
        free(GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[j]);
    }
    free(GLOBAL_DATA_FOR_INTERPOLATION);
    free(SLICE_PARAMETERS);
    
    
}

slice_parameters *readExtractedSliceParametersFile(char *sliceParametersDirectory)
{
    slice_parameters *SLICE_PARAMETERS;
    SLICE_PARAMETERS = malloc(sizeof(slice_parameters));
    
    char sliceFileName[MAX_FILENAME_STRING_LEN];
    sprintf(sliceFileName,"%s/SliceParametersExtracted.txt", sliceParametersDirectory);
    FILE *file;
    
    file = fopen(sliceFileName, "r");
    
    if (file == NULL)
    {
        printf("Extracted slice parameters file %s not found.\n",sliceFileName);
        exit(EXIT_FAILURE);
    }
    
    fscanf(file, "%d", &SLICE_PARAMETERS->nSlices);
    
    if(SLICE_PARAMETERS->nSlices>=MAX_NUM_SLICES)
    {
        printf("Number of slices in the slice parameters file exceeds the maximum allowable value of %i.\n",MAX_NUM_SLICES);
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < SLICE_PARAMETERS->nSlices; i++)
    {
        fscanf(file, "%lf %lf %lf %lf %lf", &SLICE_PARAMETERS->latA[i], &SLICE_PARAMETERS->latB[i], &SLICE_PARAMETERS->lonA[i], &SLICE_PARAMETERS->lonB[i], &SLICE_PARAMETERS->LatLonRes[i]);
    }
    
    fclose(file);
    return SLICE_PARAMETERS;
}



void interpolateIndividualSlice(global_mesh *GLOBAL_MESH, global_data_for_interpolation *GLOBAL_DATA_FOR_INTERPOLATION, int sliceNum)
{
    
    // loop over points
    int matchingInd1, matchingInd2, matchingInd3, matchingInd4;
    double Q1Vp, Q2Vp, Q3Vp, Q4Vp;
    double Q1Vs, Q2Vs, Q3Vs, Q4Vs;
    double Q1Rho, Q2Rho, Q3Rho, Q4Rho;
    int loni1, loni2, loni3, loni4;
    int lati1, lati2, lati3, lati4;
    double lat1, lat2, lat3, lat4;
    double lon1, lon2, lon3, lon4;
    double interpVals[4];
    
    for(int i = 0; i < GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->nPts; i++)
    {
        if(isnan(GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latInds[4*i])== 1)
        {
            for(int k = 0; k < GLOBAL_MESH->nZ; k++)
            {
                GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->Vp[i][k] = NAN;
                GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->Vs[i][k] = NAN;
                GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->Rho[i][k] = NAN;
            }
        }
        else
        {
            loni1 = GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonInds[4*i];
            loni2 = GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonInds[4*i+1];
            loni3 = GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonInds[4*i+2];
            loni4 = GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonInds[4*i+3];
            
            lati1 = GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latInds[4*i];
            lati2 = GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latInds[4*i+1];
            lati3 = GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latInds[4*i+2];
            lati4 = GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latInds[4*i+3];
            
            //            printf("%i %i %i %i\n",loni1, loni2, loni3, loni4);
            //            printf("%i %i %i %i\n",lati1, lati2, lati3, lati4);
            
            
            // get indice of nearby points
            
            matchingInd1 = findTwinInds(loni1, lati1, GLOBAL_DATA_FOR_INTERPOLATION->sortedLonInds, GLOBAL_DATA_FOR_INTERPOLATION->sortedLatInds, GLOBAL_DATA_FOR_INTERPOLATION->sortedNPts);
            matchingInd2 = findTwinInds(loni2, lati2, GLOBAL_DATA_FOR_INTERPOLATION->sortedLonInds, GLOBAL_DATA_FOR_INTERPOLATION->sortedLatInds, GLOBAL_DATA_FOR_INTERPOLATION->sortedNPts);
            matchingInd3 = findTwinInds(loni3, lati3, GLOBAL_DATA_FOR_INTERPOLATION->sortedLonInds, GLOBAL_DATA_FOR_INTERPOLATION->sortedLatInds, GLOBAL_DATA_FOR_INTERPOLATION->sortedNPts);
            matchingInd4 = findTwinInds(loni4, lati4, GLOBAL_DATA_FOR_INTERPOLATION->sortedLonInds, GLOBAL_DATA_FOR_INTERPOLATION->sortedLatInds, GLOBAL_DATA_FOR_INTERPOLATION->sortedNPts);
            
            // interpolate points
            lat1 = GLOBAL_MESH->Lat[loni1][lati1];
            lat2 = GLOBAL_MESH->Lat[loni2][lati2];
            lat3 = GLOBAL_MESH->Lat[loni3][lati3];
            lat4 = GLOBAL_MESH->Lat[loni4][lati4];
            
            lon1 = GLOBAL_MESH->Lon[loni1][lati1];
            lon2 = GLOBAL_MESH->Lon[loni2][lati2];
            lon3 = GLOBAL_MESH->Lon[loni3][lati3];
            lon4 = GLOBAL_MESH->Lon[loni4][lati4];
            
            double yCoords[4];
            yCoords[0] = lat2;
            yCoords[1] = lat3;
            yCoords[2] = lat4;
            yCoords[3] = lat1;
            
            double xCoords[4];
            xCoords[0] = lon2;
            xCoords[1] = lon3;
            xCoords[2] = lon4;
            xCoords[3] = lon1;
            
            for(int k = 0; k < GLOBAL_MESH->nZ; k++)
            {
                // Vs
                Q4Vs = GLOBAL_DATA_FOR_INTERPOLATION->Vs[matchingInd1][k];
                Q2Vs = GLOBAL_DATA_FOR_INTERPOLATION->Vs[matchingInd2][k];
                Q3Vs = GLOBAL_DATA_FOR_INTERPOLATION->Vs[matchingInd3][k];
                Q1Vs = GLOBAL_DATA_FOR_INTERPOLATION->Vs[matchingInd4][k];
                
                interpVals[0] = Q2Vs;
                interpVals[1] = Q3Vs;
                interpVals[2] = Q4Vs;
                interpVals[3] = Q1Vs;
                
                GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->Vs[i][k] = interpolateQuad(xCoords, yCoords, interpVals, GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonPts[i], GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latPts[i]);
                
                // Vp
                Q1Vp = GLOBAL_DATA_FOR_INTERPOLATION->Vp[matchingInd1][k];
                Q2Vp = GLOBAL_DATA_FOR_INTERPOLATION->Vp[matchingInd2][k];
                Q3Vp = GLOBAL_DATA_FOR_INTERPOLATION->Vp[matchingInd3][k];
                Q4Vp = GLOBAL_DATA_FOR_INTERPOLATION->Vp[matchingInd4][k];
                
                interpVals[0] = Q2Vp;
                interpVals[1] = Q3Vp;
                interpVals[2] = Q4Vp;
                interpVals[3] = Q1Vp;
                
                GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->Vp[i][k] = interpolateQuad(xCoords, yCoords, interpVals, GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonPts[i], GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latPts[i]);
                
                // Rho
                Q1Rho = GLOBAL_DATA_FOR_INTERPOLATION->Rho[matchingInd1][k];
                Q2Rho = GLOBAL_DATA_FOR_INTERPOLATION->Rho[matchingInd2][k];
                Q3Rho = GLOBAL_DATA_FOR_INTERPOLATION->Rho[matchingInd3][k];
                Q4Rho = GLOBAL_DATA_FOR_INTERPOLATION->Rho[matchingInd4][k];
                
                interpVals[0] = Q2Rho;
                interpVals[1] = Q3Rho;
                interpVals[2] = Q4Rho;
                interpVals[3] = Q1Rho;
                
                GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->Rho[i][k] = interpolateQuad(xCoords, yCoords, interpVals, GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonPts[i], GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latPts[i]);            }
        }
        
    }
    
}

//void obtainGlobalIndsSlice(global_mesh *GLOBAL_MESH, individual_slice_data *INDIVIDUAL_SLICE_DATA, ind_struct *IND_STRUCT, int point)
//{
//
//    // if outside cartesian domain
//    if(INDIVIDUAL_SLICE_DATA->xPts[point]<GLOBAL_MESH->X[0] || INDIVIDUAL_SLICE_DATA->xPts[point]>GLOBAL_MESH->X[GLOBAL_MESH->nX-1] || INDIVIDUAL_SLICE_DATA->yPts[point]>GLOBAL_MESH->Y[GLOBAL_MESH->nY-1] || INDIVIDUAL_SLICE_DATA->yPts[point]<GLOBAL_MESH->Y[0] )
//    {
//    }
//    else  // get indice of nearby points
//    {
//        for(int k = 0; k < GLOBAL_MESH->nX; k++)
//        {
//            if(GLOBAL_MESH->X[k]>=INDIVIDUAL_SLICE_DATA->xPts[point])
//            {
//                IND_STRUCT->xInd1 = k-1;
//                IND_STRUCT->xInd2 = k;
//
//                break;
//            }
//        }
//        for(int j = 0; j < GLOBAL_MESH->nY; j++)
//        {
//            if(GLOBAL_MESH->Y[j]>=INDIVIDUAL_SLICE_DATA->yPts[point])
//            {
//                IND_STRUCT->yInd1 = j-1;
//                IND_STRUCT->yInd2 = j;
//                break;
//            }
//        }
//    }
//
//}


void writeInterpolatedSlice(char *OUTPUT_DIR, global_data_for_interpolation *GLOBAL_DATA_FOR_INTERPOLATION, global_mesh *GLOBAL_MESH, calculation_log *CALCULATION_LOG, int sliceNum)
{
    
    // generate file for writing
    FILE *fp2;
    double currRho, currVp, currVs;
    char fName[MAX_FILENAME_STRING_LEN];
    sprintf(fName,"%s/Extracted_Slices/ExtractedSlice%i.txt",OUTPUT_DIR,sliceNum+1);
    fp2 = fopen(fName,"w");
    if (fp2 == NULL)
    {
        printf("Unable to generate slice text file for writing data.\n");
        exit(EXIT_FAILURE);
    }
    
    fprintf(fp2,"Extracted slice #%i.\n",sliceNum+1);
    fprintf(fp2,"Slice_Horizontal_Resolution\t%i\n",GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_PARAMETERS[sliceNum]->resXY );
    fprintf(fp2,"LatA:\t%lf\n",GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latPts[0]);
    fprintf(fp2,"LatB:\t%lf\n",GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latPts[GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->nPts-1]);
    fprintf(fp2,"LonA:\t%lf\n",GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonPts[0]);
    fprintf(fp2,"LonB:\t%lf\n",GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonPts[GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->nPts-1]);
    fprintf(fp2,"Lat \t Lon \t Depth \t Vp \t Vs \t Rho\n");
    for(int i = 0; i < GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->nPts; i++)
    {
        for(int m = 0; m < GLOBAL_MESH->nZ; m++)
        {
            currVp = GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->Vp[i][m];
            currRho = GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->Rho[i][m];
            currVs = GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->Vs[i][m];
            fprintf(fp2, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latPts[i],GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonPts[i], GLOBAL_MESH->Z[m], currVp, currVs, currRho);
            
        }
        
    }
    
    fclose(fp2);
    
}

void readGlobalDataPointsForInterpolation(char *OUTPUT_DIR, global_data_for_interpolation *GLOBAL_DATA_FOR_INTERPOLATION, global_mesh *GLOBAL_MESH, calculation_log *CALCULATION_LOG)
{
    
    // perform endian check
    int endianInt;
    endianInt = endian();
    
    FILE *fvp, *fvs, *frho;
    char vp3dfile[MAX_FILENAME_STRING_LEN];
    sprintf(vp3dfile,"%s/Velocity_Model/vp3dfile.p",OUTPUT_DIR);
    
    char vs3dfile[MAX_FILENAME_STRING_LEN];
    sprintf(vs3dfile,"%s/Velocity_Model/vs3dfile.s",OUTPUT_DIR);
    
    char rho3dfile[MAX_FILENAME_STRING_LEN];
    sprintf(rho3dfile,"%s/Velocity_Model/rho3dfile.d",OUTPUT_DIR);
    
    float *vp, *vs, *rho;
    int bsize, ip;
    
    fvp = fopen(vp3dfile,"r");
    fvs = fopen(vs3dfile,"r");
    frho = fopen(rho3dfile,"r");
    if (fvp == NULL)
    {
        printf("Unable to open binary files for reading.\n");
        exit(EXIT_FAILURE);
    }
    
    bsize = GLOBAL_MESH->nX*GLOBAL_MESH->nZ*sizeof(float);
    vp = (float*) malloc(bsize);
    vs = (float*) malloc(bsize);
    rho = (float*) malloc(bsize);
    printf("step2\n");

    float vsRead, vpRead, rhoRead;
    int inYArray, inBothArrays;
    
    read_flags *READ_FLAGS;
    READ_FLAGS = malloc(sizeof(read_flags));
    

    printf("step3\n");

    for(int i = 0; i < GLOBAL_MESH->nY; i++)
    {
        for(int j = 0; j < GLOBAL_MESH->nX; j++)
        {
            READ_FLAGS->xyReqFlag[j][i] = 0;
            READ_FLAGS->xyReqFlag[j][i] = 0; // set as zero for not assigned then 1 for required and 2 for not required
        }
    }
    
    printf("Reading binary files.\n");
    for(int iy = 0; iy < GLOBAL_MESH->nY; iy++)
    {
        //increment a counter
        printf("\rReading velocity model from file %d%% complete.", iy*100/GLOBAL_MESH->nY);
        fflush(stdout);
        
        //now read the obtained file in binary
        fread(vp,sizeof(vp[0]),GLOBAL_MESH->nX*GLOBAL_MESH->nZ,fvp);
        fread(vs,sizeof(vs[0]),GLOBAL_MESH->nX*GLOBAL_MESH->nZ,fvs);
        fread(rho,sizeof(rho[0]),GLOBAL_MESH->nX*GLOBAL_MESH->nZ,frho);
        inYArray = isValueInArray(iy, GLOBAL_DATA_FOR_INTERPOLATION->sortedLatInds, GLOBAL_DATA_FOR_INTERPOLATION->sortedNPts);
        if(inYArray == 1)
        {
            for(int iz = 0; iz < GLOBAL_MESH->nZ; iz++)
            {
                for (int ix = 0; ix < GLOBAL_MESH->nX; ix++)
                {
                    if( READ_FLAGS->xyReqFlag[ix][iy] == 0 )
                    {
                        inBothArrays = isValueInTwoArrays(iy, ix, GLOBAL_DATA_FOR_INTERPOLATION->sortedLatInds, GLOBAL_DATA_FOR_INTERPOLATION->sortedLonInds, GLOBAL_DATA_FOR_INTERPOLATION->sortedNPts);
                        if( inBothArrays == 1)
                        {
                            READ_FLAGS->xyReqFlag[ix][iy] = 1;
                            READ_FLAGS->xyIndFlag[ix][iy] = findTwinInds(iy, ix, GLOBAL_DATA_FOR_INTERPOLATION->sortedLatInds, GLOBAL_DATA_FOR_INTERPOLATION->sortedLonInds, GLOBAL_DATA_FOR_INTERPOLATION->sortedNPts);
                            ip = ix + iz * GLOBAL_MESH->nX;  //index counter
                            vpRead = vp[ip];
                            vsRead = vs[ip];
                            rhoRead = rho[ip];
                            
                            if (endianInt == 1) // big endian, implement byteswap
                            {
                                
                                GLOBAL_DATA_FOR_INTERPOLATION->Vp[READ_FLAGS->xyIndFlag[ix][iy]][iz] = float_swap(vpRead);
                                GLOBAL_DATA_FOR_INTERPOLATION->Vs[READ_FLAGS->xyIndFlag[ix][iy]][iz] = float_swap(vsRead);
                                GLOBAL_DATA_FOR_INTERPOLATION->Rho[READ_FLAGS->xyIndFlag[ix][iy]][iz] = float_swap(rhoRead);
                            }
                            else if (endianInt == 0) // system is little endain, read as is.
                            {
                                GLOBAL_DATA_FOR_INTERPOLATION->Vp[READ_FLAGS->xyIndFlag[ix][iy]][iz] = vpRead;
                                GLOBAL_DATA_FOR_INTERPOLATION->Vs[READ_FLAGS->xyIndFlag[ix][iy]][iz] = vsRead;
                                GLOBAL_DATA_FOR_INTERPOLATION->Rho[READ_FLAGS->xyIndFlag[ix][iy]][iz] = rhoRead;
                            }
                            
                        }
                        else
                        {
                            READ_FLAGS->xyReqFlag[ix][iy] = 2;
                        }
                        
                    }
                    else if (READ_FLAGS->xyReqFlag[ix][iy] == 1 )
                    {
                        ip = ix + iz * GLOBAL_MESH->nX;  //index counter
                        vpRead = vp[ip];
                        vsRead = vs[ip];
                        rhoRead = rho[ip];
                        
                        if (endianInt == 1) // big endian, implement byteswap
                        {
                            
                            GLOBAL_DATA_FOR_INTERPOLATION->Vp[READ_FLAGS->xyIndFlag[ix][iy]][iz] = float_swap(vpRead);
                            GLOBAL_DATA_FOR_INTERPOLATION->Vs[READ_FLAGS->xyIndFlag[ix][iy]][iz] = float_swap(vsRead);
                            GLOBAL_DATA_FOR_INTERPOLATION->Rho[READ_FLAGS->xyIndFlag[ix][iy]][iz] = float_swap(rhoRead);
                        }
                        else if (endianInt == 0) // system is little endain, read as is.
                        {
                            GLOBAL_DATA_FOR_INTERPOLATION->Vp[READ_FLAGS->xyIndFlag[ix][iy]][iz] = vpRead;
                            GLOBAL_DATA_FOR_INTERPOLATION->Vs[READ_FLAGS->xyIndFlag[ix][iy]][iz] = vsRead;
                            GLOBAL_DATA_FOR_INTERPOLATION->Rho[READ_FLAGS->xyIndFlag[ix][iy]][iz] = rhoRead;
                        }
                        
                    }
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
    free(READ_FLAGS);
    fclose(fvp);
    fclose(fvs);
    fclose(frho);
    
    printf("Binary file read complete.\n");
}



void globalIndReduction(global_data_for_interpolation *GLOBAL_DATA_FOR_INTERPOLATION)
{
    // first sort by latitudes
    int temp;
    int changePassFlag = 0;
    for(int i = 0; i < GLOBAL_DATA_FOR_INTERPOLATION->nPts; i++)
    {
        for(int j = 0; j < GLOBAL_DATA_FOR_INTERPOLATION->nPts -1; j++)
        {
            if(GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[j]>GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[j+1])
            {
                temp = GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[j+1];
                GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[j+1] = GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[j];
                GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[j] = temp;
                
                temp = GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[j+1];
                GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[j+1] = GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[j];
                GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[j] = temp;
                changePassFlag = 1;
            }
        }
        if (changePassFlag == 0) // if no changes made in one full pass then break
        {
            break;
        }
        changePassFlag = 0;
    }
    
    // remove duplicate indicies
    int firstInd = 0, finalInd = 0, count = 0;
    int nLon;
    GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[GLOBAL_DATA_FOR_INTERPOLATION->nPts] = -1; // ensures that this indice is not used
    GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[GLOBAL_DATA_FOR_INTERPOLATION->nPts] = -1;
    for(int i = 1; i <= GLOBAL_DATA_FOR_INTERPOLATION->nPts; i++)
    {
        if(GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[firstInd] == GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[i])
        {
            finalInd = i;
        }
        else
        {
            nLon = finalInd - firstInd;
            if(nLon == 0)
            {
                GLOBAL_DATA_FOR_INTERPOLATION->sortedLatInds[count] = GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[firstInd];
                GLOBAL_DATA_FOR_INTERPOLATION->sortedLonInds[count] = GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[firstInd];
                count += 1;
                
            }
            else
            {
                changePassFlag = 0;
                for (int j = firstInd; j <= finalInd; j++)
                {
                    for (int k = firstInd; k < finalInd; k++)
                    {
                        if(GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[k]>GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[k+1])
                        {
                            temp = GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[k+1];
                            GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[k+1] = GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[k];
                            GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[k] = temp;
                            changePassFlag = 1;
                        }
                        
                    }
                    if (changePassFlag == 0) // if no changes made in one full pass then break
                    {
                        break;
                    }
                    changePassFlag = 0;
                    
                }
                for (int j = firstInd; j <= finalInd; j++)
                {
                    if((GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[j] != GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[j+1]) || j == finalInd)
                    {
                        GLOBAL_DATA_FOR_INTERPOLATION->sortedLatInds[count] = GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[j];
                        GLOBAL_DATA_FOR_INTERPOLATION->sortedLonInds[count] = GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[j];
                        count += 1;
                        
                    }
                }
            }
            
            firstInd = i;
        }
    }
    GLOBAL_DATA_FOR_INTERPOLATION->sortedNPts = count;
}


int findGlobalMeshAdjacentPoints(global_mesh *GLOBAL_MESH, double latA, double lonA, double modelRot, double *adjLatInds, double *adjLonInds)
{
    int nVert = 5;
    double lon[nVert], lat[nVert];
    int inPoly;
    
    double maxLat, maxLon;
    double minLat, minLon;

//    int xFound = 0;
//    int yFound = 0;
//    
//    int xFoundTemp = 0;
//    int yFoundTemp = 0;
//    int xInds[2][2];
//    int yInds[2][2];

    
    if(latA> GLOBAL_MESH->maxLat || latA < GLOBAL_MESH->minLat || lonA > GLOBAL_MESH->maxLon || lonA < GLOBAL_MESH->minLon)
    {
        return 0; // outside the maximum lat lon of the domain
    }
    else
    {

//        printf("%i %i\n",xFound,yFound);
//        
//        findCrossingIndsX(GLOBAL_MESH, 0, &xInds[xFound][0], &xInds[xFound][1], &yInds[yFound][0], &yInds[yFound][1], &yFoundTemp, &xFoundTemp, latA, lonA);
//        if( xFoundTemp == 1)
//        {
//            xFound += 1;
//            xFoundTemp = 0;
//        }
//        if( yFoundTemp == 1)
//        {
//            yFound += 1;
//            yFoundTemp = 0;
//        }
//        findCrossingIndsX(GLOBAL_MESH, GLOBAL_MESH->nX-1, &xInds[xFound][0], &xInds[xFound][1], &yInds[yFound][0], &yInds[yFound][1], &yFoundTemp, &xFoundTemp, latA, lonA);
//        if( xFoundTemp == 1)
//        {
//            xFound += 1;
//            xFoundTemp = 0;
//        }
//        if( yFoundTemp == 1)
//        {
//            yFound += 1;
//            yFoundTemp = 0;
//        }
//        printf("%i %i %i %i\n", xInds[0][0],xInds[0][1],xInds[1][0],xInds[0][1]);
//        printf("%i %i %i %i\n", yInds[0][0],yInds[0][1],yInds[1][0],yInds[0][1]);
//        printf("%i %i\n",xFound,yFound);
//        
//        findCrossingIndsY(GLOBAL_MESH, 0, &xInds[xFound][0], &xInds[xFound][1], &yInds[yFound][0], &yInds[yFound][1], &yFoundTemp, &xFoundTemp, latA, lonA);
//        if( xFoundTemp == 1)
//        {
//            xFound += 1;
//            xFoundTemp = 0;
//        }
//        if( yFoundTemp == 1)
//        {
//            yFound += 1;
//            yFoundTemp = 0;
//        }
//        findCrossingIndsY(GLOBAL_MESH, GLOBAL_MESH->nY-1, &xInds[xFound][0], &xInds[xFound][1], &yInds[yFound][0], &yInds[yFound][1], &yFoundTemp, &xFoundTemp, latA, lonA);
//        if( xFoundTemp == 1)
//        {
//            xFound += 1;
//            xFoundTemp = 0;
//        }
//        if( yFoundTemp == 1)
//        {
//            yFound += 1;
//            yFoundTemp = 0;
//        }
//        printf("%i %i %i %i\n", xInds[0][0],xInds[0][1],xInds[1][0],xInds[0][1]);
//        printf("%i %i %i %i\n", yInds[0][0],yInds[0][1],yInds[1][0],yInds[0][1]);
//        printf("%i %i\n",xFound,yFound);
//        
//        printf("%lf %lf %lf\n",latA,GLOBAL_MESH->Lat[xInds[0][0]][xInds[0][1]], GLOBAL_MESH->Lat[xInds[1][0]][xInds[1][1]]);
//        
//        printf("%lf %lf %lf\n",lonA,GLOBAL_MESH->Lon[yInds[0][0]][yInds[0][1]], GLOBAL_MESH->Lat[yInds[1][0]][xInds[1][1]]);

        
        for( int iy = 0; iy < GLOBAL_MESH->nY; iy ++)
        {
        }
        for( int ix = 0; ix < GLOBAL_MESH->nX-1; ix ++)
        {
            for( int iy = 0; iy < GLOBAL_MESH->nY-1; iy ++)
            {
                lon[0] = GLOBAL_MESH->Lon[ix][iy];
                lon[1] = GLOBAL_MESH->Lon[ix][iy+1];
                lon[2] = GLOBAL_MESH->Lon[ix+1][iy+1];
                lon[3] = GLOBAL_MESH->Lon[ix+1][iy];
                lon[4] = GLOBAL_MESH->Lon[ix][iy];

                lat[0] = GLOBAL_MESH->Lat[ix][iy];
                lat[1] = GLOBAL_MESH->Lat[ix][iy+1];
                lat[2] = GLOBAL_MESH->Lat[ix+1][iy+1];
                lat[3] = GLOBAL_MESH->Lat[ix+1][iy];
                lat[4] = GLOBAL_MESH->Lat[ix][iy];

                maxLat = findMaxValueArray(nVert-1, lat);
                minLat = findMinValueArray(nVert-1, lat);

                maxLon = findMaxValueArray(nVert-1, lon);
                minLon = findMinValueArray(nVert-1, lon);
                
                if(latA>maxLat || latA < minLat || lonA > maxLon || lonA < minLon)
                {
                }
                else
                {
                    inPoly = pnpoly(nVert, lon, lat, lonA, latA);
                    if(inPoly == 1)
                    {
                        adjLonInds[0] = ix;
                        adjLonInds[1] = ix;
                        adjLonInds[2] = ix + 1;
                        adjLonInds[3] = ix + 1;
                        
                        adjLatInds[0] = iy;
                        adjLatInds[1] = iy + 1;
                        adjLatInds[2] = iy + 1;
                        adjLatInds[3] = iy;
                        
                        return 1;
                    }
                }
                
            }
        }
        
        return 0;
    }
}

void findCrossingIndsX(global_mesh *GLOBAL_MESH, int searchIndX, int *xInd1, int *xInd2, int *yInd1, int *yInd2, int *yFound, int *xFound, double latA, double lonA)
{
    for( int iy = 0; iy < GLOBAL_MESH->nY-1; iy ++)
    {
        if((GLOBAL_MESH->Lon[searchIndX][iy] > lonA) &&  (GLOBAL_MESH->Lon[searchIndX][iy+1] < lonA))
        {
            *yInd1 = searchIndX;
            *yInd2 = iy;
            *yFound = 1;
        }
        if((GLOBAL_MESH->Lon[searchIndX][iy] < lonA) &&  (GLOBAL_MESH->Lon[searchIndX][iy+1] > lonA))
        {
            *yInd1 = searchIndX;
            *yInd2 = iy;
            *yFound = 1;
        }
        if((GLOBAL_MESH->Lat[searchIndX][iy] > latA) &&  (GLOBAL_MESH->Lat[searchIndX][iy+1] < latA))
        {
            *xInd1 = searchIndX;
            *xInd2 = iy;
            *xFound = 1;
        }
        if((GLOBAL_MESH->Lat[searchIndX][iy] < latA) &&  (GLOBAL_MESH->Lat[searchIndX][iy+1] > latA))
        {
            *xInd1 = searchIndX;
            *xInd2 = iy;
            *xFound = 1;
        }
    }
    printf("%i %i\n",*xFound,*yFound);

}

void findCrossingIndsY(global_mesh *GLOBAL_MESH, int searchIndY, int *xInd1, int *xInd2, int *yInd1, int *yInd2, int *yFound, int *xFound, double latA, double lonA)
{
    for( int ix = 0; ix < GLOBAL_MESH->nX-1; ix ++)
    {
        if((GLOBAL_MESH->Lon[ix][searchIndY] > lonA) &&  (GLOBAL_MESH->Lon[ix+1][searchIndY] < lonA))
        {
            *yInd1 = ix;
            *yInd2 = searchIndY;
            *yFound = 1;
        }
        if((GLOBAL_MESH->Lon[ix][searchIndY] < lonA) &&  (GLOBAL_MESH->Lon[ix+1][searchIndY] > lonA))
        {
            *yInd1 = ix;
            *yInd2 = searchIndY;
            *yFound = 1;
        }
        if((GLOBAL_MESH->Lat[ix][searchIndY] > latA) &&  (GLOBAL_MESH->Lat[ix+1][searchIndY] < latA))
        {
            *xInd1 = ix;
            *xInd2 = searchIndY;
            *xFound = 1;
        }
        if((GLOBAL_MESH->Lat[ix][searchIndY] < latA) &&  (GLOBAL_MESH->Lat[ix+1][searchIndY] > latA))
        {
            *xInd1 = ix;
            *xInd2 = searchIndY;
            *xFound = 1;
        }
    }
    printf("%i %i\n",*xFound,*yFound);
    
}



void generateGlobalIndsForRead(global_mesh *GLOBAL_MESH, global_data_for_interpolation *GLOBAL_DATA_FOR_INTERPOLATION, model_extent *MODEL_EXTENT, int sliceNum)
{
    double adjLatInds[4], adjLonInds[4];
    int pointsFound;
    for(int i = 0; i < GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->nPts; i++)
    {
        printf("Searching slice #%i, point #%i of %i.\n", sliceNum+1,i+1,GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->nPts );
        pointsFound = findGlobalMeshAdjacentPoints(GLOBAL_MESH, GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latPts[i], GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonPts[i], MODEL_EXTENT->originRot, adjLatInds, adjLonInds);
        if( pointsFound == 1)
        {
            
            GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[GLOBAL_DATA_FOR_INTERPOLATION->nPts] = adjLatInds[0];
            GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[GLOBAL_DATA_FOR_INTERPOLATION->nPts+1] = adjLatInds[1];
            GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[GLOBAL_DATA_FOR_INTERPOLATION->nPts+2] = adjLatInds[2];
            GLOBAL_DATA_FOR_INTERPOLATION->requiredLatInds[GLOBAL_DATA_FOR_INTERPOLATION->nPts+3] = adjLatInds[3];
            
            GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[GLOBAL_DATA_FOR_INTERPOLATION->nPts] = adjLonInds[0];
            GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[GLOBAL_DATA_FOR_INTERPOLATION->nPts+1] = adjLonInds[1];
            GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[GLOBAL_DATA_FOR_INTERPOLATION->nPts+2] = adjLonInds[2];
            GLOBAL_DATA_FOR_INTERPOLATION->requiredLonInds[GLOBAL_DATA_FOR_INTERPOLATION->nPts+3] = adjLonInds[3];
            
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latInds[4*i] = adjLatInds[0];
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latInds[4*i+1] = adjLatInds[1];
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latInds[4*i+2] = adjLatInds[2];
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latInds[4*i+3] = adjLatInds[3];
            
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonInds[4*i] = adjLonInds[0];
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonInds[4*i+1] = adjLonInds[1];
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonInds[4*i+2] = adjLonInds[2];
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonInds[4*i+3] = adjLonInds[3];
            
            GLOBAL_DATA_FOR_INTERPOLATION->nPts += 4;
        }
        else
        {
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latInds[4*i] = NAN;
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latInds[4*i+1] = NAN;
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latInds[4*i+2] = NAN;
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->latInds[4*i+3] = NAN;
            
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonInds[4*i] = NAN;
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonInds[4*i+1] = NAN;
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonInds[4*i+2] = NAN;
            GLOBAL_DATA_FOR_INTERPOLATION->INDIVIDUAL_SLICE_DATA[sliceNum]->lonInds[4*i+3] = NAN;
        }
    }
}


void generateSlicePoints(individual_slice_data *INDIVIDUAL_SLICE_DATA, individual_slice_parameters *INDIVIDUAL_SLICE_PARAMETERS,  global_mesh *GLOBAL_MESH)
{
    double deltaLat;
    double deltaLon;
    int nGrdPts;
    int count = 0;
    
    nGrdPts = INDIVIDUAL_SLICE_PARAMETERS->resXY;

    deltaLat = (INDIVIDUAL_SLICE_PARAMETERS->latPtsSlice[0]-INDIVIDUAL_SLICE_PARAMETERS->latPtsSlice[1])/nGrdPts;
    deltaLon = (INDIVIDUAL_SLICE_PARAMETERS->lonPtsSlice[0]-INDIVIDUAL_SLICE_PARAMETERS->lonPtsSlice[1])/nGrdPts;
    
    for(int j = 0; j < nGrdPts+1; j++)
    {
        INDIVIDUAL_SLICE_DATA->lonPts[count] = INDIVIDUAL_SLICE_PARAMETERS->lonPtsSlice[0]-j*deltaLon;
        INDIVIDUAL_SLICE_DATA->latPts[count] = INDIVIDUAL_SLICE_PARAMETERS->latPtsSlice[0]-j*deltaLat;
        count += 1;
    }
    INDIVIDUAL_SLICE_DATA->nPts = count;
    if( INDIVIDUAL_SLICE_DATA->nPts>=MAX_NUM_SLICE_GRIDPTS)
    {
        printf("Total number of slice sample points exceeds the maximum allowable value of %i.\n",MAX_NUM_SLICE_GRIDPTS);
        exit(EXIT_FAILURE);
    }
}







//void extractSlice(gridStruct *location, modOrigin modelOrigin, sliceExtent sliceBounds, globalDataValues *globDataVals, char *outputDirectory, int sliceNumber)
//{
//
//    sliceExtractData *sliceData;
//    sliceData = malloc(sizeof(sliceExtractData));
//
//
//    // loop over points
//    int xInd = 0;
//    int yInd = 0;
//    double X1, X2, Y1, Y2;
//    double Q11Vp, Q12Vp, Q21Vp, Q22Vp;
//    double Q11Vs, Q12Vs, Q21Vs, Q22Vs;
//    double Q11Rho, Q12Rho, Q21Rho, Q22Rho;
//
//    for(int i = 0; i < sliceData->nPts; i++)
//    {
//        // if x or y value is outside of the bound of the dataset return NaNs.
////        printf("%lf %lf %lf\n", sliceData->xPts[i], location->X[0],location->X[location->nX-1]);
////        printf("%lf %lf %lf\n", sliceData->yPts[i], location->Y[0],location->Y[location->nY-1]);
//
//        if(sliceData->xPts[i]<location->X[0] || sliceData->xPts[i]>location->X[location->nX-1] || sliceData->yPts[i]>location->Y[location->nY-1] || sliceData->yPts[i]<location->Y[0] )
//        {
//            for(int k = 0; k < location->nZ; k++)
//            {
//                sliceData->Vp[i][k] = NAN;
//                sliceData->Vs[i][k] = NAN;
//                sliceData->Rho[i][k] = NAN;
//            }
//        }
//        else  // get indice of nearby points
//        {
//            for(int k = 0; k < location->nX; k++)
//            {
//                if(location->X[k]>=sliceData->xPts[i])
//                {
//                    xInd = k-1;
//                    break;
//                }
//            }
//            for(int j = 0; j < location->nY; j++)
//            {
//                if(location->Y[j]>=sliceData->yPts[i])
//                {
//                    yInd = j-1;
//                    break;
//                }
//            }
//            // interpolate points
//            X1 = location->X[xInd];
//            X2 = location->X[xInd+1];
//            Y1 = location->Y[yInd];
//            Y2 = location->Y[yInd+1];
//
////            double lat1, lat2, lon1, lon2;
////            lat1 = location->Lat[xInd][yInd];
////            lat2 = location->Lat[xInd+1][yInd+1];
////            lon1 = location->Lon[xInd][yInd];
////            lon2 = location->Lon[xInd+1][yInd+1];
////
////            printf("%lf %lf %lf %lf\n", X1, X2, Y1,Y2);
////            printf("%lf %lf\n",sliceData->xPts[i], sliceData->yPts[i]);
////            printf("%lf %lf %lf %lf\n", lat1, lat2, lon1, lon2);
////            printf("%lf %lf\n",sliceData->latPts[i], sliceData->lonPts[i]);
//
//            for(int k = 0; k < location->nZ; k++)
//            {
//                // Vs
//                Q11Vs = globDataVals->Vs[xInd][yInd][k];
//                Q12Vs = globDataVals->Vs[xInd][yInd+1][k];
//                Q21Vs = globDataVals->Vs[xInd+1][yInd][k];
//                Q22Vs = globDataVals->Vs[xInd+1][yInd+1][k];
//
////                printf("%lf %lf %lf %lf\n", Q11Vs, Q21Vs, Q22Vs, Q12Vs);
//
//                sliceData->Vs[i][k] = biLinearInterpolation(X1, X2, Y1, Y2, Q11Vs, Q12Vs, Q21Vs, Q22Vs, sliceData->xPts[i], sliceData->yPts[i]);
//
//                // Vp
//                Q11Vp = globDataVals->Vp[xInd][yInd][k];
//                Q12Vp = globDataVals->Vp[xInd][yInd+1][k];
//                Q21Vp = globDataVals->Vp[xInd+1][yInd][k];
//                Q22Vp = globDataVals->Vp[xInd+1][yInd+1][k];
////                printf("%lf %lf %lf %lf\n", Q11Vp, Q21Vp, Q22Vp, Q12Vp);
//
//
//                sliceData->Vp[i][k] = biLinearInterpolation(X1, X2, Y1, Y2, Q11Vp, Q12Vp, Q21Vp, Q22Vp, sliceData->xPts[i], sliceData->yPts[i]);
//
//                // Rho
//                Q11Rho = globDataVals->Rho[xInd][yInd][k];
//                Q12Rho = globDataVals->Rho[xInd][yInd+1][k];
//                Q21Rho = globDataVals->Rho[xInd+1][yInd][k];
//                Q22Rho = globDataVals->Rho[xInd+1][yInd+1][k];
//
//                sliceData->Rho[i][k] = biLinearInterpolation(X1, X2, Y1, Y2, Q11Rho, Q12Rho, Q21Rho, Q22Rho, sliceData->xPts[i], sliceData->yPts[i]);
//            }
//        }
//
//    }
//    char sliceDir[128];
//    sprintf(sliceDir,"%s/Slices",outputDirectory);
//
//    struct stat st = {0};
//    if (stat(sliceDir, &st) == -1)
//    {
//        // create output directory and the velocity model
//        mkdir(sliceDir, 0700);
//    }
//
//    // generate file for writing
//    FILE *fp2;
//    double currRho, currVp, currVs;
//    char fName[128];
//    sprintf(fName,"%s/ExtractedSlice%i.txt",sliceDir,sliceNumber);
//    fp2 = fopen(fName,"w");
//    fprintf(fp2,"Extracted slice #%i.\n",sliceNumber);
//    fprintf(fp2,"Slice_Horizontal_Resolution\t%i\n",sliceBounds.resXY);
//    fprintf(fp2,"LatA:\t%lf\n",sliceData->latPts[0]);
//    fprintf(fp2,"LatB:\t%lf\n",sliceData->latPts[sliceData->nPts-1]);
//    fprintf(fp2,"LonA:\t%lf\n",sliceData->lonPts[0]);
//    fprintf(fp2,"LonB:\t%lf\n",sliceData->lonPts[sliceData->nPts-1]);
//    fprintf(fp2,"Lat \t Lon \t Depth \t Vp \t Vs \t Rho\n");
//    for(int i = 0; i < sliceData->nPts; i++)
//    {
//        for(int m = 0; m < location->nZ; m++)
//        {
//            currVp = sliceData->Vp[i][m];
//            currRho = sliceData->Rho[i][m];
//            currVs = sliceData->Vs[i][m];
//            fprintf(fp2, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",sliceData->latPts[i],sliceData->lonPts[i], location->Z[m], currVp, currVs, currRho);
//
//        }
//
//    }
//
//    fclose(fp2);
//    free(sliceData);
//
//}







