/* testing
 *  Fastrak.h
 *  Fastrak
 *
 *  Created by James Kyle on 7/25/07.
 *  Copyright 2007 James A. Kyle. All rights reserved.
 *   
 *  ==Begin Function Reference==
 *  ~Fastrak(): default destructor for the fastrak class
 *    
 *    PreCondition:  fastrak object exists
 *    PostCondition: fastrak object has been released
 *  
 *  Fastrak():     default constructor for fastrak object
 *
 *    PreCondition: none
 *    PostCondition: coordinates struct initialized to zero for all variables
 *                   Fastrak output log path set to /tmp/Fastrak.log
 *                   Fastrak watch_state set to FASTRAKLOG
 *
 *  Fastrak(const string): fastrak object constructor with explicit fastrak log
 *                         log file path as argument
 *    
 *    PreCondition: none
 *    PostCondition: coordinates struct initialized to zero for all variables
 *                   Fastrak output log path set to string argument
 *                   Fastrak watch_state set to FASTRAKLOG

 *  void setFastrakLog(const string);
 *      Precondition:  string pointer containing the POSIX path to 
 *                     the Fastrak Log.
 *      Postcondition: file_path contains the given POSIX path 
 *
 *  string* getFastrakLog();
 *      Precondition: file_path contains a valid string parameter
 *
 *      Postcondition: pointer to file_path has been returned
 *
 *  void updateCoordinates(const int*): receives a file descriptor to the fastrak
 *    log used to update the objects Coordinates struct
 *
 *      Precondition: int* is a pointer to a valid file descriptor 
 *      Postcondition: Coordinates struct contains the current set of coordinates
 *
 *  const Coordinates* getCoordinates(): accessor for the Coordinates struct
 *
 *      Precondition: Coordinates contains valid data
 *      Postcondition: The most recent data entry from the Fastrak Log is returned
 *        via the Coordinates structure
 *
 *  void start(): public accessor for initiating watched state
 *    PreCondition: Watch state is defined and valid
 *    PostCondition: monitor set to TRUE
 *									 respective watch state's monitoring is initiated
 *    Valid Watch States:
 *        FASTRAKLOG: the fastrak log file is observed for change
 *        FASTRAKUSB: <--to be implemented-->
 *
 *	void stop(): public accessor for stopping watched state
 *		PreCondition: Watch state is defined and valid
 *		PostCondition: monitor set to FALSE
 *									 respective watch state's monitoring stopped
 *			Valid Watch States:
 *				FASTRAKLOG: the fastrak log file is observed for change
 *				FASTRAKUSB: <-- to be implemented -->
 *
 *  string getLastLine(int*) const: takes a file descriptor and fetches the last
 *    line of the file
 *    
 *    PreCondition: file descriptor is valid
 *    PostCondition: last line of log file is returned and file pointer is 
 *      iterated to the eof
 *	void* fileMonitor(void *): <-- to be completed -->
 *	static void* c_fileMonitor(void *): wrapper function that staticly casts the
 *		fileMonitor function for threading
 *
 *  ==Begin Attribute Reference==
 *
 *  string fileName: POSIX path to the Log file
 *
 *	bool monitor: exit/initiate condition of monitoring
 *  
 *  Coordinates coords: inherited from Dispatcher.h, contains the 6 DoF from 
 *    the fastrak object
 *
 *  Coordinate Data is parsed from a flat text file which receives a 
 *  continuous stream of data from the Fastrak device. The beginning
 *  of a Fastrak session is denoted by time stamp line of the format:
 *   
 *   ***  11:55   7-20-2007  ***
 *                                                                                 
 *  Likewise, the end of session is marked by a similar entry, the only 
 *  difference being the time stamp. The format of the data is (for each line):
 *                                                                                 
 *   01    8.35 -14.52  -6.61 -31.46  24.58   4.93
 *   
 *  From the Fastrak Manual included in project documentation:
 *   Column           Function 
 *   1           01 Header (not shown above) 
 *   2           X position in inches 
 *   3           Y position in inches 
 *   4           Z position in inches 
 *   5           Azimuth attitude in degrees 
 *   6           Elevation attitude in degrees 
 *   7           Roll attitude in degrees 
 *  
 *  The data structure below, coordinateData, has a key descriptor of data type
 *  and a value representing those found for the corresponding log entry.
 *  
 *  string file_path: holds the path to the fastrak log
 *  
 *  string watch_state: sets watch state of the Fastrak object. Currently only
 *    FASTRAKLOG is implemented, however FASTRAKUSB is a possible future feature
 */
#ifndef _FASTRAK_
#define _FASTRAK_

#include <sys/event.h>
#include <sys/time.h> 
#include <fcntl.h>
#include <string>
#include <map>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iomanip>
#include <pthread.h>
#include <sched.h>

#include "safe-read.h"
#include "Dispatcher.h"
#include "CommonVariables.h"
#include <vector>
#include <algorithm>

using std::ostream;
using std::string;
using std::map;
using std::setprecision;

class Fastrak : public Dispatcher {
public:
  ~Fastrak();
	Fastrak();

	void                      setFastrakLog(string);
	const string*             getFastrakLog() const;
	
	const Coordinates*        getCoordinates() const;
  void                      start();
  void                      stop();
	static void*							monitorCallback(void*);
	
	int												setWatchState(string);
	const string							getWatchState() const;
	
	int												setInputType(string);
	const string							getInputType() const;
	
private:
	void                      updateCoordinates(const int*);
	void											fileMonitor(void*);
  string                    getLastLine(const int*) const;   
	bool											isValidWatchState(string);
	bool											isValidInputType(string);
	// sets the valid values for watch and input_types
	void											setupDefaults();
	void											resetCoords();


	std::vector<string>				valid_watch_states;
	std::vector<string>				valid_input_types;
  string                    file_path;
  string                    watch_state;
	string										input_type;
	bool											monitor;
	pthread_t									monitor_thread;
	float											excess_relative_translation;
	
};
#endif
