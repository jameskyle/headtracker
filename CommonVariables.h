/**
 *  \file CommonVariables.h
 *  \brief Common structures and variables used throughout the headtracker project
 *  
 *  Copyright 2007 Center for Cognitive Neuroscience. All rights reserved.
 */

#ifndef _COMMON_VARIABLES_
#define _COMMON_VARIABLES_ 
/**
 * \struct Coordinates
 * 
 * holds the 6 DoF for object location obtained from a Dispatcher object
 * (fastrak, scanner, etc)
 */
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
