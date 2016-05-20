//
//  writeCVMData.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 24/06/14.
//  Copyright (c) 2014 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include "constants.h"
#include "structs.h"
#include "functions.h"

void writeCVMData(gridStruct *location, globalDataValues *globDataVals, char *outputDirectory)
/*
 Purpose:   write the full velocity model to file
 
 Input variables:
 location        - pointer to structure containing lat lon grid
 globalValues    - pointer to structure containing vp vs and rho for all gridpoints
 
 Output variables:
 N/A.
 */
{
    // perform endian check
    int endianInt;
    endianInt = endian();
    
    char veloModDir[128];
    sprintf(veloModDir,"%s/Velocity_Model",outputDirectory);
    
    struct stat st = {0};
    if (stat(veloModDir, &st) == -1)
    {
        // create output directory and the velocity model
        mkdir(veloModDir, 0700);
    }
    
    FILE *fvp, *fvs, *frho; //*fp
    char vp3dfile[64];
    sprintf(vp3dfile,"%s/vp3dfile.p",veloModDir);

    char vs3dfile[64];
    sprintf(vs3dfile,"%s/vs3dfile.s",veloModDir);

	char rho3dfile[64];
    sprintf(rho3dfile,"%s/rho3dfile.d",veloModDir);
    
    char lats[64];
    char latsTxt[64];

    sprintf(lats,"%s/lats.bin",veloModDir);
    FILE *flats = fopen(lats, "w");
    sprintf(latsTxt,"%s/lats.txt",veloModDir);
    FILE *flatsTxt = fopen("lats.txt", "w");

    
    char lons[64];
    char lonsTxt[64];

    sprintf(lons,"%s/lons.bin",veloModDir);
    FILE *flons = fopen(lons, "w");
    sprintf(lonsTxt,"%s/lons.txt",veloModDir);
    FILE *flonsTxt = fopen("lons.txt", "w");

    
    char deps[64];
    char depsTxt[64];

    sprintf(deps,"%s/deps.bin",veloModDir);
    FILE *fdeps = fopen(deps, "w");
    sprintf(depsTxt,"%s/deps.txt",veloModDir);
    FILE *fdepsTxt = fopen("deps.txt", "w");


    float *vp, *vs, *rho;
    float vpTemp, vsTemp, rhoTemp;
    float vpWrite, vsWrite, rhoWrite;

    int bsize, ip;
    
	fvp = fopen(vp3dfile,"w");
	fvs = fopen(vs3dfile,"w");
	frho = fopen(rho3dfile,"w");
    
	//determine the number of x,y,z layers
    printf("Starting binary file write.\n");
    printf("Number of grid points, ");
	printf("nx: %d ",location->nX);
	printf("ny: %d ",location->nY);
	printf("nz: %d ",location->nZ);
	printf("\n");
    
	bsize = location->nX*location->nZ*sizeof(float);
	vp = (float*) malloc(bsize);
	vs = (float*) malloc(bsize);
	rho = (float*) malloc(bsize);
    
    globDataVals->nPtsBelowVsThreshold = 0;
    
	for(int iy = 0; iy < location->nY; iy++)
    {
		for(int iz = 0; iz < location->nZ; iz++)
        {
			for (int ix = 0; ix < location->nX; ix++)
            {
				ip = ix + iz * location->nX;  //index counter
                if (globDataVals->Vs[ix][iy][iz] < globDataVals->minVs) // enforce min Vs
                {
                    vsTemp = globDataVals->minVs;
                    globDataVals->nPtsBelowVsThreshold += 1;
                }
                else
                {
                    vsTemp = globDataVals->Vs[ix][iy][iz]; // else assign from global structure
                }
                vpTemp = globDataVals->Vp[ix][iy][iz];
                rhoTemp = globDataVals->Rho[ix][iy][iz];
                
                
                if (endianInt == 1) // big endian
                {
                    vsWrite = float_swap(vsTemp);
                    vpWrite = float_swap(vpTemp);
                    rhoWrite = float_swap(rhoTemp);
                }
                else if (endianInt == 0) // little endian
                {
                    vsWrite = vsTemp;
                    vpWrite = vpTemp;
                    rhoWrite = rhoTemp;
                }

                
                fwrite(&location->Lat[ix][iy],sizeof(float),1,flats);
                fwrite(&location->Lon[ix][iy],sizeof(float),1,flons);
                fwrite(&location->Z[iz],sizeof(float),1,fdeps);

                fprintf(flatsTxt,"%f\n",location->Lat[ix][iy]);
                fprintf(flonsTxt,"%f\n",location->Lon[ix][iy]);
                fprintf(fdepsTxt,"%f\n",location->Z[iz]);
                
                fwrite(&vpWrite,sizeof(vpWrite),1,fvp);
                fwrite(&vsWrite,sizeof(vsWrite),1,fvs);
                fwrite(&rhoWrite,sizeof(rhoWrite),1,frho);
                
			}
		}
        
		//increment a counter
        printf("\rWriting velocity model to file %d%% complete.", iy*100/location->nY);
        fflush(stdout);
        
		//now write the obtained file in binary
//		fwrite(vp,sizeof(vp[0]),location->nX*location->nZ,fvp);
//		fwrite(vs,sizeof(vs[0]),location->nX*location->nZ,fvs);
//		fwrite(rho,sizeof(rho[0]),location->nX*location->nZ,frho);
	}
    printf("\rWriting velocity model to file 100%% complete.");
    fflush(stdout);
    printf("\n");
    
    if (endianInt == 1) // big endian
    {
        printf("Big endian\n");
    }
    else if (endianInt == 0) // little endian
    {
        printf("Little endian\n");
    }
    
    free(vp);
    free(vs);
    free(rho);
	fclose(fvp);
	fclose(fvs);
	fclose(frho);

    fclose(flatsTxt);
    fclose(flonsTxt);
    fclose(fdepsTxt);
    
    fclose(flats);
    fclose(flons);
    fclose(fdeps);
printf("Binary file write complete.\n");

    
}


