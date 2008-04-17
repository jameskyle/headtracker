//
//  GuiController.mm
//  HeadTracker
//
//  Created by James Kyle on 11/19/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "GuiController.h"

@implementation GuiController
-(void)awakeFromNib {
	myFastrak		 = new Fastrak;
	
	[fastrakLogField setStringValue:[[NSString alloc] initWithCString: myFastrak->getFastrakLog()->c_str()]];
	myController = new Controller;
	myController->enableLog();
	myFastrak->setInputType("RELATIVE");
	
	[self attachObserver: myController];
	[fastrakLogField setEditable: FALSE];
	[fastrakLogField setSelectable: FALSE];
	
	isMonitoring = false;
	
}
-(void)dealloc {
	delete myFastrak;
	delete myController;
	[super dealloc];
}
-(void)attachObserver:(Observer *) observer {
	myFastrak->attach(observer);
}
-(void)setLogFile:(NSString *)log {
	myFastrak->setFastrakLog([log cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	[fastrakLogField setStringValue:[[NSString alloc] initWithCString: myFastrak->getFastrakLog()->c_str()]];
}

-(IBAction) toggleMonitorState:(id) sender {

	if(!isMonitoring) {
		myFastrak->start();
		[playStateButton setTitle:@"Stop"];
	}
	
	else {
		myFastrak->stop();
		[playStateButton setTitle:@"Start"];
	}
	isMonitoring = !isMonitoring;

}
-(IBAction) openLogFile:(id)sender {
	// Create a standard open file panel
	NSOpenPanel* oPanel = [NSOpenPanel openPanel];
	
	// Set allowable actions for open panel dialog
	[oPanel setCanChooseFiles: YES];
	[oPanel setCanCreateDirectories: FALSE];
	[oPanel setCanChooseDirectories: FALSE];
	[oPanel setAllowsMultipleSelection: FALSE];
	[oPanel setAlphaValue: 0.95];
	[oPanel setTitle: @"Select the Fastrak Output Log"];
	
	// Process selected files on "OK"
	if([oPanel runModalForTypes: nil] == NSOKButton) {
		NSArray* files = [oPanel filenames];
	
		NSString* file = [files objectAtIndex:0];
		[self setLogFile: file];
		NSLog(file);

	}
}
@end
