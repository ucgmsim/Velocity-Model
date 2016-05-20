//
//  rhoFromVpBrocher.c
//  CVMversions
//
//  Created by Ethan M. Thomson on 23/04/15.
//  Copyright (c) 2015 Dept. Civil Engineering. All rights reserved.
//

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "constants.h"
#include "structs.h"
#include "functions.h"


double rhoFromVpBrocher(double vp){
    double rho;
    
    rho = 1.6612*vp -0.4721*pow(vp,2) +0.0671*pow(vp,3) -0.0043*pow(vp,4) +0.000106*pow(vp,5);
    
    return rho;
}

double vsFromVpBrocher(double vp){
    double vs;
        
    vs = 0.7858 - 1.2344*vp + 0.7949*pow(vp,2) -0.1238*pow(vp,3) + 0.0064*pow(vp,4);
    
    return vs;
}

