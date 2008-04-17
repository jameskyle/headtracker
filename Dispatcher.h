/*
 *  Dispatcher.h:
 *  Virtual Class template for dispatcher objects
 *
 *  Fastrak
 *
 *  Created by James Kyle on 7/25/07.
 *  Copyright 2007 James Kyle. All rights reserved.
 *  
 *  PUBLIC
 *
 *  ~Dispatcher(): Destructor for the Dispatcher object
 *
 *    PreCondition: none
 *    PostCondition: Dispatcher object is freed
 *  
 *  void attach(Observer*): adds Observer object to the notify queue
 *
 *    PreCondition: Observer queue exists
 *    PostCondition: Objserver object is appended to queue
 *    
 *  void detach(Observer*): removes Observer object from notify queue
 *
 *    PreCondition: Observer object exists in queue
 *    PostCondition: Observer object no longer exists in queue
 *
 *  void  notify(): Notifies all attached observer objects that watchable event
 *    has occurred
 *
 *    PreCondition: A watchable event has occurred in the dispatcher object
 *    PostCondition: All attached observers have been notified of a change
 *
 *  const Coordinates* getCoordinates(): returns the current set of coordinates
 *    
 *    PreCondition: coordinates struct contains valid data
 *    PostCondition: a valid Coordinates object is returned to the caller
 *
 *  void start(): Initiate observer object
 *    
 *    PreConditions: Observer object and values successfully instantiated
 *    Postcondition: Observer process has begun
 * 
 * PROTECTED
 *  Dispatcher(): Constructor for dispatcher object. Only derivative classes can
 *    call this constructor. 
 *
 *    PreCondition: none
 *    PostCondition: Dispatcher object is created
 *
 *  PRIVATE
 *  list<Observer*> _observers: list object containing a queue of observers
 *
 *  Coordinates* coords:
 *  
 */
#ifndef _DISPATCHER_
#define _DISPATCHER_ 

#include <list>
#include "CommonVariables.h"
#include <iostream>
using std::ostream;
using std::list;

class Observer;

class Dispatcher {
	
public:
	virtual ~Dispatcher();
	virtual void attach(Observer*);
	virtual void detach(Observer*);
	virtual void notify();
	virtual const Coordinates* getCoordinates() const = 0;
  virtual void start() = 0;
	
protected:
		Dispatcher();
    Coordinates coords;

private:
		list<Observer*> _observers;
};
#endif
