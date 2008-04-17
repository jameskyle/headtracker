/*
 *  Observer.h
 *  Fastrak
 *
 *  Created by James Kyle on 7/25/07.
 *  Copyright 2007 James A. Kyle. All rights reserved.
 *
 *  virtual void update(const void*) = 0
 *    Precondition:  An event has triggered a monitored change in the 
 *                   Dispatcher
 *    Postcondition: The Observer updated itself to the new state of the
 *                   Dispatcher object.
 *  virtual void printData(const Coordinates*, ostream&): pure virtual function
 *    meant to be overidden by derived dispatcher objects. Prints coordinate data
 *    to an arbitrary stream
 */

#ifndef _OBSERVER_
#define _OBSERVER_
#include "CommonVariables.h"
#include <ostream>
using std::ostream;

class Dispatcher;

class Observer {
public:
	virtual ~Observer();
	virtual void update(Dispatcher*) = 0;
  virtual void printData(const Coordinates*, ostream&) const = 0;
protected:
		Observer();
};
#endif
