//
//  plotExtracts.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 21/08/14.
//  Copyright (c) 2014 Dept. Civil Engineering. All rights reserved.
//

//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <string.h>
//#include <assert.h>
//#include "constants.h"
//#include "structs.h"
//#include "functions.h"
//
//void plotProfiles(modOrigin modelOrigin, modExtent modelExtent,modVersion modelVersion, surfNames surfSubModNames);
//void extractSurfaceBounds(gridStruct *location, int basinNum, globalBasinData *basinData);
//
//
//void plotExtracts()
//{
//    modVersion modelVersion;
//    modelVersion.version = 1.2;
//    printf("Generating model version %f.\n", modelVersion.version);
//    
//    /* original values
//    modOrigin modelOrigin;
//    modelOrigin.mlat = -43.6;
//    modelOrigin.mlon = 172.25;
//    modelOrigin.mrot = 0;
//    
//    modExtent modelExtent;
//    modelExtent.Ymax = 40;
//    modelExtent.Xmax = 60;
//    modelExtent.Zmax = 20; // +ve downwards in km
//    modelExtent.Zmin = 0; // -ve upwards in km
//    modelExtent.hDep = 0.1;
//    modelExtent.hLatLon = 0.5;
//    */
//    
//    modOrigin modelOrigin;
//    modelOrigin.mlat = -43.6;
//    modelOrigin.mlon = 172.6;
//    modelOrigin.mrot = 0;
//    
//    modExtent modelExtent;
//    modelExtent.Ymax = 0.4;
//    modelExtent.Xmax = 0.0225;
//    modelExtent.Zmax = .2; // +ve downwards in km
//    modelExtent.Zmin = -.4; // -ve upwards in km
//    modelExtent.hDep = 0.001;
//    modelExtent.hLatLon = 0.025;
//    
////    modOrigin modelOrigin;
////    modelOrigin.mlat = -43.6;
////    modelOrigin.mlon = 172.25;
////    modelOrigin.mrot = 0;
////    
////    modExtent modelExtent;
////    modelExtent.Ymax = 0.0225;
////    modelExtent.Xmax = 60;
////    modelExtent.Zmax = .2; // +ve downwards in km
////    modelExtent.Zmin = -.4; // -ve upwards in km
////    modelExtent.hDep = 0.001;
////    modelExtent.hLatLon = 0.025;
//    
//    surfNames surfSubModNames;
//    surfSubModNames = getSurfSubModNames(modelVersion);
//    
//    plotProfiles(modelOrigin, modelExtent, modelVersion, surfSubModNames);
//}
//
//void plotProfiles(modOrigin modelOrigin, modExtent modelExtent,modVersion modelVersion, surfNames surfSubModNames)
//{
//    // GENERATE AND SAVE PROFILES AT INDIVIDUAL POINTS
//    gridStruct *locToPlot = NULL;
//    
//    // use below for profiles at individual points
//    //locToPlot = generateLatLonForPlotting(modelOrigin, modelExtent, latPts, lonPts, nPts);
//    
//    // use this for slices
//    //locToPlot = generateModelGrid(modelOrigin, modelExtent);
//    
//    surfaceDepthsGlobal *surfDepsLatLon = NULL;
//    surfDepsLatLon = getSurfaceValues(locToPlot, surfSubModNames);
//    globalDataValues *dataValLatLon = NULL;
//    globalBasinData *basinData = NULL;
//    basinData = malloc(sizeof(globalBasinData));
//    dataValLatLon = assignValues(modelVersion, locToPlot, surfSubModNames, surfDepsLatLon);
//    extractSurfaceBounds(locToPlot, 0, basinData);
//    // Write to file
//    // writeLatLonData(locToPlot, dataValLatLon);
//    writeCVMData(locToPlot, dataValLatLon);
//    
//    free(dataValLatLon);
//    free(surfDepsLatLon);
//    free(locToPlot);
//    free(basinData);
//}
//
//void extractSurfaceBounds(gridStruct *location, int basinNum, globalBasinData *basinData)
//{
//    // generate file for writing
//    FILE *fp2;
//    fp2 = fopen("/Users/EthanT/Desktop/veloModelLatSliceSurfaceExtracts.txt","w");
//    fclose(fp2);
//    fp2 = fopen("/Users/EthanT/Desktop/veloModelLatSliceSurfaceExtracts.txt","a+");
//    
//    int j = location->nY/2;
//    for(int k = 0; k < basinData->nSurf[basinNum]-1; k++) //-1 to exclude last surface
//    {
//        for(int i = 0; i < location->nX; i++)
//        {
//            if(basinData->inBasinLatLon[basinNum][i][j] == 1)
//            {
//                fprintf(fp2,"%lf, %lf\n",location->Lon[i][j], basinData->surfVals[basinNum][i][j][k]);
//            }
//        }
//        fprintf(fp2,"%lf, %lf\n", NAN, NAN);
//    }
//    
//    fclose(fp2);
//    
//    FILE *fp3;
//    fp3 = fopen("/Users/EthanT/Desktop/veloModelLonSliceSurfaceExtracts.txt","w");
//    fclose(fp3);
//    fp3 = fopen("/Users/EthanT/Desktop/veloModelLonSliceSurfaceExtracts.txt","a+");
//    
//    int i = location->nX/2;
//    for(int k = 0; k < basinData->nSurf[basinNum]-1; k++) //-1 to exclude last surface
//    {
//        for(int j = 0; j < location->nY; j++)
//        {
//            if(basinData->inBasinLatLon[basinNum][i][j] == 1)
//            {
//                fprintf(fp3,"%lf, %lf\n",location->Lat[i][j], basinData->surfVals[basinNum][i][j][k]);
//            }
//        }
//        fprintf(fp3,"%lf, %lf\n", NAN, NAN);
//    }
//    
//    fclose(fp3);
//
//    
//    
//    
//    
//    
//    
//    
//    
//    
//    
//    
//    
//    
//}
//
//

















