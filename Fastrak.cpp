/*
 *  Fastrak.cpp
 *  Fastrak
 *
 *  Created by James Kyle on 7/25/07.
 *  Copyright 2007 James A. Kyle. All rights reserved.
 *
 */
#include "Fastrak.h"
using namespace std;

Fastrak::~Fastrak() {}
Fastrak::Fastrak() {
	this->resetCoords();
	this->setupDefaults();
}

void Fastrak::start(){
	this->monitor = true;
  if(this->getWatchState() == "FASTRAKLOG") { 
		pthread_create(&monitor_thread, 
                   NULL, 
                   Fastrak::monitorCallback, 
                   this);
	} /* Checks for additional watch states, e.g. usb or serial, would go here */
  else {
	cerr << "No watch state set!" << endl;
	}
}

void Fastrak::setupDefaults() {
	/****************************************************************************
	 *	ABSOLUTE: coordinates are provided as absolute distance from origin     *
	 *  RELATIVE: coordinates are provided as a delta from previous coordinates *
	 ****************************************************************************/
	this->valid_input_types.push_back("ABSOLUTE");
	this->valid_input_types.push_back("RELATIVE");
	
	/****************************************************************************
	 * FASTRAKLOG: coordinates are parsed from a Fastrak log file               *
	 * [below is to be implemented]                                             *
	 * FASTRAKUSB: coordinates are received directly from a USB connection      *
	 * SCANNER:    coordinates are received directly from the MRI Scanner       *   
	 ****************************************************************************/
	this->valid_watch_states.push_back("FASTRAKLOG");
	
	/****************************************************************************
	 * excess_relative_translation is used to cap the amount of translation     *
	 * done during any single iteration while INPUTTYPE is set to RELATIVE (see *
	 * above)
	 ****************************************************************************/
	this->excess_relative_translation = 0.2;
	
	this->setFastrakLog("/tmp/Fastrak.log");
	this->setWatchState("FASTRAKLOG");
	
	this->setInputType("ABSOLUTE");
	this->monitor                     = false;	
}

int Fastrak::setWatchState(string watch_state) {
	// A return code of 0 = success, 1 = failure. 
	int ret = 1;
	if(this->isValidWatchState(watch_state)) {
		this->watch_state = watch_state;
		ret = 0;
	}
	else
		ret = 1;
	
	return ret;
}

const string Fastrak::getWatchState() const{
	return this->watch_state;
}

bool Fastrak::isValidWatchState(string watch_state) {
	bool ret = binary_search(this->valid_watch_states.begin(), 
                           this->valid_watch_states.end(),
                           watch_state);
	return ret;
}

int Fastrak::setInputType(string input_type) {
	// return code of 1 is failure, 0 is success
	int ret = 1;
	if(isValidInputType(input_type)) {
		this->input_type = input_type;
		ret = 0;
	}
	else {
		ret = 1;
	}
	return ret;
}

const string Fastrak::getInputType() const {
	return this->input_type;
}

bool Fastrak::isValidInputType(string input_type) {
	bool ret = binary_search(this->valid_input_types.begin(), 
                           this->valid_input_types.end(), 
                           input_type);
	return ret;
}

void Fastrak::stop() {
	this->monitor = false;
	this->resetCoords();
}

void Fastrak::setFastrakLog(string file) {
	this->file_path = file;
}

const string* Fastrak::getFastrakLog() const { return &file_path; }

string Fastrak::getLastLine(const int *fd) const {
/******************************************************************************
 * Here we will iterate backwards through each character starting from the last char
 * on the last line of our log file until we find the beginning file position 
 * descriptor of the line. I referred to tail.c source for a good reference on 
 * an efficient way to go about this.
 ******************************************************************************/
  off_t beg_pos     = 0;
  off_t end_pos     = 0;
  off_t pos         = 0;
  size_t bytes_read = 0;

  char c_buffer[BUFSIZ];
  string buffer;
  string last_line;  
 
  beg_pos = lseek(*fd, 0, SEEK_CUR);
  end_pos = lseek(*fd, 0, SEEK_END); 
  pos = end_pos;
  
  if (errno) {
    cerr << "lseek failed: " << errno << endl;
    exit(errno);
  }

  /******************************************************************************
   * Here we set the bytes_read to the size of the last buffer, probably partial
   *****************************************************************************/

  bytes_read = (end_pos - beg_pos) % BUFSIZ;
  if(bytes_read == 0) bytes_read = BUFSIZ; 
  
  // Make pos a multiple of BUFSIZ for efficiency sake
  pos -= bytes_read;
  
  // Reset file position to the last char of the last line
  // e.g. trim the buffer fat at the end
  lseek(*fd, pos, SEEK_SET);
  
  bytes_read = safe_read(*fd, c_buffer,bytes_read);
  
  buffer.assign(c_buffer);
  
  if(bytes_read == SAFE_READ_ERROR) {
    cerr << "error reading log file" << endl;
    exit(errno);
  }
  
  // find the beginning and end of the last line
  size_t last = buffer.find_last_of("\r\n");
  size_t first = buffer.find_last_of("\r\n", last-2);
  size_t length = last - first; 
  last_line = buffer.substr(first+1, length);

  return last_line;
}

void Fastrak::updateCoordinates(const int *fd) {	
	string last_line = this->getLastLine(fd); 
	if(errno) {exit(errno);}
	int ret,header;
	float x,y,z,azimuth,elevation,roll;

	// We require that 7 values be assigned to our coords struct. Thus, 
	// A successfull read will result in a return code of 7. 
	ret = sscanf(last_line.c_str(), "%d%f%f%f%f%f%f\r\n", &header,&x,&y,&z,
							 &azimuth,&elevation,&roll);
	if(ret != 7) {
		if(EOF) {
      // Sometimes the tracker writes an unscannable line. such as a continuous
      // string without spaces between the integers. So if we've reached EOF,
      // but have not acquired 7 ints...try again.
			this->updateCoordinates(fd);
		}
		else {
			cerr << "There has been an error within: updateCoordinates(int *fd)" <<
			"Return Code: " << ret << " " << "errno code: " << errno << endl;
			exit(ret);			
		}
	}
	
	
	if(this->getInputType() == "ABSOLUTE") {
	  	
		this->coords.header     = header;
		this->coords.x          = x;
		this->coords.y          = y;
		this->coords.z          = z;
		this->coords.azimuth    = azimuth;
		this->coords.elevation  = elevation;
		this->coords.roll			  = roll;
		
	}
	else if(this->getInputType() == "RELATIVE"){
		this->coords.header    = header;
    // Absolute translation of relative coordinates
    // is gained via summation with previous values

		if(this->excess_relative_translation >= x)
			this->coords.x			 = this->coords.x + x;

		if(this->excess_relative_translation >= y)
			this->coords.y			 = this->coords.y + y;

		if(this->excess_relative_translation >= z)
			this->coords.z			 = this->coords.z + z;
		
		this->coords.azimuth   = azimuth;
		this->coords.elevation = elevation;
		this->coords.roll			 = roll;
		
    cout << " x input: "          << x  
         << " x result: "         << this->coords.x 
         << " y input: "          << y
         << " y result: "         << this->coords.y
         << " z input:  "         << z 
         << " z result: "         << this->coords.z 
         << " azimuth input: "    << azimuth 
         << " azimuth result: "   << this->coords.azimuth 
         << " elevation input: "  << elevation
         << " elevation result: " << this->coords.elevation 
         << " roll input: "       << roll 
         << " roll result: "      << this->coords.roll << endl;
	}
				
}
void Fastrak::resetCoords() {
	this->coords.header     = 0;
	this->coords.x          = 0;
	this->coords.y          = 0;
	this->coords.z          = 0;
	this->coords.azimuth    = 0;
	this->coords.elevation  = 0;
	this->coords.roll       = 0;
}

const Coordinates* Fastrak::getCoordinates() const { return &coords; }
void* Fastrak::monitorCallback(void* object) {
  // This function is called from the main Fastrak thread. Currently, all it
  // does is call the fileMonitor method, other methods could go here as well as
  // checks of this->watch_state to determine which monitor method to spawn.
	((Fastrak *)object)->fileMonitor(object);
	return NULL;
}
/*******************************************************************************
 * The fileMonitor method uses kqueue to watch a file for changes/updates. When
 * they occur, it updates the Fastrak's current positional attributes
 * accordingly
 ******************************************************************************/
void Fastrak::fileMonitor(void* object) {
  // cast the object pointer to a fastrak pointer. This is necessary due to the
  // loss of the "this" pointer with pthread and callbacks.
  Fastrak* pthis = static_cast<Fastrak *>(object);
  // Instantiate the needed file descripters and kevent structs
  int fd, kq, nev;
  struct kevent event;
  struct kevent change;
  
  // Create kqueue() object
  kq = kqueue();
  if(kq == -1)  perror("kqueue"); 
  
  // Open the fastrak log file
  fd = open(pthis->file_path.c_str(), O_RDONLY);
  if(fd == -1) perror("open");
  
  // Jump to the end of the file, we're only concerned with the most current
  // coordinates
  lseek(fd, 0, SEEK_END);
   
  // Set the watch states. See kqueue(2) for details
  // EVFILT_VNODE: monitor file
  // EV_ADD:       add event to kqueue
  // EV_ENABLE:    return even if triggered
  // EV_ONESHOT:   only return first occurance
  // NOTE_WRITE:   notify on write event
  // NOTE_EXTEND:  notify on append event
  // 0, 0:         no filter specific data, no user-defined pass throughs
  EV_SET(&change, fd, 
         EVFILT_VNODE, 
         EV_ADD | EV_ENABLE | EV_ONESHOT, 
         NOTE_WRITE | NOTE_EXTEND,
         0, 0);
  
  // Continue to monitor until the Fastrak object toggles the monitor state to
  // false
  while(pthis->monitor) {
    // get any new events
    nev = kevent(kq,&change, 1, &event, 1, NULL);
    if(nev == -1) {
			perror("kevent");
		}
    else if(nev > 0) {
      // If a write or extend event has occurred, we update our coordinates
      // struct with the most recent positional values from the log
      if(event.fflags & NOTE_WRITE || event.fflags & NOTE_EXTEND) {
        
        pthis->updateCoordinates(&fd);   
        pthis->notify();
      }
    }
  }
  // Clean up
  close(kq);
  close(fd);
}
