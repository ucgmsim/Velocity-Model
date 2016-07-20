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


double rhoFromVpBrocher(double vp)
/*
 Purpose: to calcualte Rho given Vp from the Brocher correlation
 
 Input variables:
    vp - primary wave velocity
 
 Output variables:
    density - Rho from the Brocher correlation 

 */
{
    double density;
    
    vp  = vp/1000;
    
    density = vp*(1.6612+vp*(-.4721+vp*(.0671+vp*(-.0043+.000106*vp))));
    
    return density*1000;
}