/*
 *  Dispatcher.cpp
 *  Fastrak
 *
 *  Created by James Kyle on 7/25/07.
 *  Copyright 2007 James A. Kyle. All rights reserved.
 *
 */

#include "Dispatcher.h"
#include "Observer.h"
#include <iostream>
#include <errno.h>
using namespace std;
Dispatcher::~Dispatcher() {}
Dispatcher::Dispatcher() {
}

void Dispatcher::attach(Observer* observer) { _observers.push_back(observer); }
void Dispatcher::detach(Observer* observer) { _observers.remove(observer); }
void Dispatcher::notify() {
  list<Observer*>::iterator i;
  list<Observer*>::iterator end(_observers.end());
  
  for(i = _observers.begin(); i != end; i++){ (**i).update(this); } 
}

