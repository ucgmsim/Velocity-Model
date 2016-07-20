//
//  writeGridPoints.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 11/05/15.
//  Copyright (c) 2015 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "constants.h"
#include "structs.h"


//void writeGridPoints(gridStruct *location, char *outputDirectory)
//{
//    
//    FILE *fp;
//    char fpfile[128];
//    sprintf(fpfile,"%s/gridPoints.txt",outputDirectory);
//    
//    
//    fp = fopen(fpfile,"w");
//    
//    for(int iy = 0; iy < location->nY; iy++)
//    {
//        for(int iz = 0; iz < location->nZ; iz++)
//        {
//            for (int ix = 0; ix < location->nX; ix++)
//            {
//                fprintf(fp,"%lf,%lf\n",location->Lat[ix][iy],location->Lon[ix][iy]);
//            }
//        }
//    }
//    fclose(fp);
//
//    printf("Grid file write complete.\n");
//}
//
//void writeAllSurfaceDepths(gridStruct *location, surfDepValues *surfDep, char *outputDirectory)
//{
//    
//    FILE *fp;
//    char fpfile[128];
//    sprintf(fpfile,"%s/basementDepth.txt",outputDirectory);
//    
//    
//    fp = fopen(fpfile,"w");
//    
//    for(int iy = 0; iy < location->nY; iy++)
//    {
//        for (int ix = 0; ix < location->nX; ix++)
//        {
//            fprintf(fp,"%lf,%lf,%lf\n",location->Lat[ix][iy],location->Lon[ix][iy], surfDep->dep[ix][iy]);
//        }
//    }
//    fclose(fp);
//    
//    printf("Surface file write complete.\n");
//    
//}


