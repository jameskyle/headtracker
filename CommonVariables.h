/*
 *  CommonVariables.h
 *  Fastrak
 *
 *  Created by James Kyle on 9/11/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *  
 *  struct Coordinates:   holds the 6 dof for object location obtained from
 *                        a Dispatcher object (fastrak, scanner, etc)
 */
#ifndef _COMMON_VARIABLES_
#define _COMMON_VARIABLES_ 

struct Coordinates {
  int header;
  float x;
  float y;
  float z;
  float azimuth;
  float elevation;
  float roll;
};
 
#endif
