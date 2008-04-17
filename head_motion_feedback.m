 % Head Motion Code
% D. Kalar, 9/11/07

function head_motion_feedback

try
% Assert opengl is available or quit
AssertOpenGL;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% set up the screen and keyboard
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

pixelSize = 32; % You might change to 8 if 32 not working, depending on Mac or Windows.
grayLevel=120;


% stimulusRestTime sets the duration that the headtracker feedback is displayed
stimulusRestTime = 6; 

if nargin < 1
  multiSample = 0
end
InitializeMatlabOpenGL;
[w,screenRect] 	= Screen('OpenWindow',0,[],[],[],[],[],multiSample);

% get aspect ratio of screen
aspect_ratio=screenRect(4)/screenRect(3);

[wWidth, wHeight]=Screen('WindowSize', w);
Screen('FillRect', w, grayLevel);  % NB: only need to do this once!
Screen('Flip', w);



% set up screen positions for stimuli
xcenter=wWidth/2;
ycenter=wHeight/2;

% setup basic colors
black=BlackIndex(w); % Should equal 0.
white=WhiteIndex(w); % Should equal 255.

% use Arial font
theFont='Courier';
Screen('TextFont',w,theFont);

 % use 100 point font in white
Screen('TextSize',w, 80);
Screen('TextColor',w,white);

prompt_text = 'Dummy Stimulus';
% offset the drawing of the target letters and fixation to be truly center
[prompt_size, prompt_offset_size] = Screen('TextBounds',w,prompt_text);
prompt_shift = prompt_offset_size(3)/2;



%oldTextSize=Screen('TextSize', w, 40);
%Screen('DrawText',w,'press any key to begin',xcenter-375,ycenter-320);
%Screen('TextSize', w, oldTextSize);

%Screen('Flip',w);

%KbWait;  % wait for keypress

% clear screen
%Screen('Flip', w);


%Screen('DrawText', w, prompt_text, xcenter - prompt_shift, ycenter);
%Screen('Flip', w);
%pause(3);
fid = fopen('/tmp/Controller.log', 'r');
RenderHeadPosition(w,aspect_ratio,xcenter,ycenter/2,30,stimulusRestTime,fid);

% look for a keypress
[keyIsDown,secs,keyCode]=KbCheck;
if keyIsDown,
  
  if strcmpi(KbName(keyCode),'ESCAPE')
    error('User escaped experiment...')
  end
  WaitSecs(0.001);  % prevents overload and decrement of priority
end;
 


	% Clean up after the experiment has ended
  Priority(0);
  ShowCursor;
  Screen('CloseAll');

catch
	sprintf('Execption Caught!: %s', lasterr)
%		save(sprintf('%s_EXCEPTION',subject_code))

	% also clean up if there is an error above
  Priority(0);
  ShowCursor;
  Screen('CloseAll');
  rethrow(lasterror)

end
  % close log file
  fclose(fid);

return
