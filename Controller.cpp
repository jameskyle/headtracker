/*
 *  FastrakController.cpp
 *  Fastrak
 *
 *  Created by James A. Kyle on 7/28/07.
 *  Copyright 2007 James A. Kyle. All rights reserved.
 *
 */
#include "Controller.h"

Controller::Controller() {
	this->_LOG = false;
  this->log_name = "/tmp/Controller.log";
};

Controller::Controller(bool log, string name) {
  this->_LOG = log;
  this->log_name = name;
};

Controller::Controller(bool log) {
  this->_LOG = log;
  this->log_name = "/tmp/Controller.log";
}

Controller::~Controller(){
  if(this->log.is_open()) {this->log.close();}
};

void Controller::update(Dispatcher* theChangeDispatcher) {
  const Coordinates* coords;
  coords = theChangeDispatcher->getCoordinates();
  if(_LOG) {this->writeToLog(coords);}
};

void Controller::writeToLog(const Coordinates* coords){	
  this->log.seekp(ios_base::beg);
  this->printData(coords, this->log);
};

void Controller::printData(const Coordinates* coords, ostream& stream) const {

   stream << setprecision(2) << fixed << coords->header << "\t\t" << coords->x 
     << "\t\t" << coords->y << "\t\t" << coords->z << "\t\t" << coords->azimuth 
     << "\t\t" << coords->elevation << "\t\t" << coords->roll;

};

void Controller::enableLog() {
  this->log.open(this->log_name.c_str());
  this->_LOG = true;
};

void Controller::enableLog(string name) {
  this->_LOG = true;
  this->log_name = name;
};

void Controller::disableLog() {
  if(this->log.is_open()) { this->log.close();}
  
  this->_LOG = false;
};
