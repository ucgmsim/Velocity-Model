//
//  createOutputDirectories.c
//  UCVM-Integration
//
//  Created by Ethan Thomson on 10/07/16.
//  Copyright © 2016 Dept. Civil Engineering. All rights reserved.
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

void createAllOutputDirectories(char *OUTPUT_DIR, char *genCallType)
{
    mkdir(OUTPUT_DIR, 0700);
    char tempDir[MAX_FILENAME_STRING_LEN];
    sprintf(tempDir,"%s/Log",OUTPUT_DIR);
    mkdir(tempDir, 0700);

    if (strcmp(genCallType,"GENERATE_VELOCITY_MOD")== 0)
    {
        char tempDir[MAX_FILENAME_STRING_LEN];
        sprintf(tempDir,"%s/Velocity_Model",OUTPUT_DIR);
        mkdir(tempDir, 0700);
    }
    else if (strcmp(genCallType,"EXTRACT_VELOCITY_SLICES")== 0)
    {
        char tempDir[MAX_FILENAME_STRING_LEN];
        sprintf(tempDir,"%s/Extracted_Slices",OUTPUT_DIR);
        mkdir(tempDir, 0700);
    }
    else if (strcmp(genCallType,"GENERATE_VELOCITY_SLICES")== 0)
    {
        char tempDir[MAX_FILENAME_STRING_LEN];
        sprintf(tempDir,"%s/Generated_Slices",OUTPUT_DIR);
        mkdir(tempDir, 0700);
    }
    else if (strcmp(genCallType,"GENERATE_PROFILE")== 0)
    {
        char tempDir[MAX_FILENAME_STRING_LEN];
        sprintf(tempDir,"%s/Profile",OUTPUT_DIR);
        mkdir(tempDir, 0700);
    }
    else if (strcmp(genCallType,"GENERATE_THRESHOLD")== 0)
    {
        char tempDir[MAX_FILENAME_STRING_LEN];
        sprintf(tempDir,"%s/Vs",OUTPUT_DIR);
        mkdir(tempDir, 0700);
        sprintf(tempDir,"%s/Z",OUTPUT_DIR);
        mkdir(tempDir, 0700);
    }
    else if (strcmp(genCallType,"GENERATE_VELOCITIES_ON_GRID")== 0)
    {
        char tempDir[MAX_FILENAME_STRING_LEN];
        sprintf(tempDir,"%s/Velocities_At_Gridpoints",OUTPUT_DIR);
        mkdir(tempDir, 0700);
    }
    else if (strcmp(genCallType,"GENERATE_MULTIPLE_PROFILES")== 0)
    {
        char tempDir[MAX_FILENAME_STRING_LEN];
        sprintf(tempDir,"%s/Profiles",OUTPUT_DIR);
        mkdir(tempDir, 0700);
    }

}
