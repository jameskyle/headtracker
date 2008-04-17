/*
 *  FastrakController.h:
 *  Dispatcher object for the Fastrak Log
 *
 *  Fastrak
 *
 *  Created by James A. Kyle on 7/28/07.
 *  Copyright 2007 James A. Kyle. All rights reserved.
 *  
 *  Public API: 
 *
 *  Controller():   Default constructor for the Dispatcher object
 *
 *    PreCondition:   none
 *    PostCondition:  Controller object is created with default params:
 *                    this->_LOG = false
 *                    this->log_name = /tmp/Controller.log
 *
 *  Controller(bool log): Controller constructor with explicit log setting
 *
 *    PreCondition:       none
 *    PostCondition:      Controller object created with params:
 *                        this->_LOG = log 
 *                        this->log_name = "/tmp/Controller.log"
 *
 *  Controller(bool log, string name):  Controller constructor with explicit 
 *    log and log_name 
 *
 *    PreCondition:                     none
 *    PostCondition:                    Controller object created with params:
 *                                      this->_LOG = log
 *                                      this->log_name = name
 *  ~Controller():                      Default Controller destructor
 *
 *  void update(Dispatcher*): Syncs Controller with the current state of the 
 *    Dispatcher
 *
 *    PreCondition:           Observable Dispatcher event has occurred, e.g. 
 *                            coordinates have changed
 *    PostCondition:          Controller object's state is sync'd with that of 
 *                            the Observer object
 *
 *  void enableLog(): Activates logging
 *
 *    PreCondition:   log_name is set
 *    PostCondition:  this->log is opened and this->_LOG is set to true
 *  
 *  void disableLog:  Discontinues logging\
 *
 *    PreCondition:   this->log has been set
 *    PostCondition:  this->log is closed if previously opened and this_LOG is 
 *                    set to false
 *
 *  void printData(const Coordinates*, ostream& stream = cout) const: prints 
 *    data contained in the Coordinates struct to the given stream or stdout if
 *    no stream is provided
 *    
 *    PreCondition: a valid Coordinates object pointer is passed
 *    PostCondition: [stream] contains the new data set
 *
 *  PRIVATE:
 *  void writeToLog(const Coordinates*): prints data to log file
 *    
 *    PreCondition:   Coordinates object is initialized
 *    PostCondition:  log file pointer is rewound to beginning of file
 *                    data is written to log
 *  
 *  bool _LOG:  determines enabled state of the log file
 *
 *  string log_name: path to the log file
 *
 *  ofstream log: log file output stream
 *
 */
#ifndef _CONTROLLER_
#define _CONTROLLER_

#include "Observer.h"
#include "Fastrak.h"
#include "CommonVariables.h"
#include <fstream>

using namespace std;

class Controller : public Observer {
  public:
		Controller();
		Controller(bool);
    Controller(bool,string);
		~Controller();
    void update(Dispatcher*);
		void enableLog();
    void enableLog(string);
		void disableLog();
    void printData(const Coordinates*, ostream& stream = cout) const;


  private:
		void writeToLog(const Coordinates*);
		bool _LOG;
		string log_name;
    ofstream log;
};
#endif
