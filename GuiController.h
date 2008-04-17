//
//  GuiController.h
//  HeadTracker
//
//  Created by James Kyle on 11/19/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <cstdlib>
#include "Fastrak.h"
#include "Controller.h"

@interface GuiController : NSObject {
	@private
	Fastrak* myFastrak;
	Controller* myController;
	
	BOOL isMonitoring;
	
	
@public
	IBOutlet NSButton *playStateButton;
	IBOutlet NSTextField *fastrakLogField;
	
}
-(IBAction)openLogFile:(id)sender;
-(IBAction) toggleMonitorState:(id)sender;
-(void) setLogFile:(NSString *)log;
-(void) attachObserver:(Observer *)observer;
@end
