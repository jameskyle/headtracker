% Project Name: HeadTracker
% Matlab Function: newRenderHeadPosition(ptr,aspect_ratio,x_center,y_center,scale_factor, duration, fid)
%   PreCondition: Center coordinates, ptr, aspect_ratio, and scale_factor have been 
%     set properly
%   PostCondition: An object is rendered to the new coordinates on the screen 
%     representing the subjects position as given by GetNewCoordinates


function newRenderHeadPosition(ptr,aspect_ratio,x_center,y_center,scale_factor, duration, fid)
  
  InitializeMatlabOpenGL;

  % Declare the coords struct and assign default values
  coords = struct('x', 0, 'y', 0, 'z', 0, 'yaw', 0, 'pitch', 0, 'roll', 0,'newcoords', false)
  % Setup the OpenGL rendering cntext of the onscreen window for use by OpenGL 
  Screen('BeginOpenGL', ptr);
  % -Set the OpenGL lighting model to be used
  % -Select the OpenGL light source
  % -Set the Depth function and enable smoothing
  % -Set perspective
  % -Set color model
  glEnable(GL.LIGHTING);
  glEnable(GL.LIGHT0);
  glEnable(GL.DEPTH_TEST);
  glMatrixMode(GL.PROJECTION);
  glLoadIdentity;
  gluPerspective(30, 1/aspect_ratio, 0.1, 100); 
  Screen('EndOpenGL', ptr);
  
  Screen('BeginOpenGL', ptr);
  % -Set reposition camera
  gluLookAt(0,0,10,0,0,0,0,1,0);

  %glEnable(GL.COLOR_MATERIAL);
  
  % -Set material ambience
  % -Set specular model
  % -Set material shininess
  % -Set light position
  % -Set light mode to ambient
  mat_ambient = [0.2, 0.2, 0.2, 0.0];
  mat_specular = [0.3, 0.3, 0.3, 0.0];
  light_position = [1,1,5,1];
  lm_ambient = [0.5,0.5,0.5,0.0];

  glMaterialfv(GL.FRONT, GL.AMBIENT, mat_ambient);
  glMaterialfv(GL.FRONT, GL.SPECULAR, mat_specular);
  glMaterialfv(GL.FRONT, GL.SHININESS, 50);
  glLightfv(GL.LIGHT0, GL.POSITION, light_position);
  glLightModelfv(GL.LIGHT_MODEL_AMBIENT, lm_ambient);
  % -Set default clear color
  glClearColor(0.0,0.0,0.0,0.0);

  % -Set our timing variables
  start = GetSecs();
  
  % -Set intial rotation to none
  glRotatef(20, 1, 0, 0);
  glRotatef(20, 0, 1, 0);
  glRotatef(20, 0, 0, 1);

  Screen('EndOpenGL',ptr); 
  Screen('Flip', ptr);
  Screen('BeginOpenGL', ptr);
  while ~KbCheck
    [coords] = GetScannerHeadPosition(fid, coords);
    glPushMatrix;
    glClear;

    glTranslatef(-coords.x, -coords.y, 0);
    glRotatef(-coords.pitch, 1, 0, 0);
    glRotatef(-coords.yaw,   0, 1, 0);
    glRotatef(-coords.roll,  0, 0, 1);
    sphere_diffuse = [0.7, 0.0, 0.7, 1.0];
    glEnable(GL.POLYGON_OFFSET_FILL);
    glMaterialfv(GL.FRONT, GL.DIFFUSE, sphere_diffuse);
    glutSolidSphere(0.6,2048, 2048);
    glDisable(GL.POLYGON_OFFSET_FILL);

    glutSolidSphere(0.6,2048, 2048);    

    glEnable(GL.LINE_SMOOTH)
    glEnable(GL.BLEND);
    glBlendFunc(GL.SRC_ALPHA, GL.ONE_MINUS_SRC_ALPHA);
    glHint(GL.LINE_SMOOTH_HINT, GL.DONT_CARE);
    glLineWidth(7.0);
    
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL.LINES);
    glVertex3f(0.0,  1.75, 0.0);
    glVertex3f(0.0, -1.75, 0.0);
    glEnd();
    glBegin(GL.LINES);
    glVertex3f(1.75, 0.0, 0.0);
    glVertex3f(-1.75, 0.0, 0.0);
    glEnd();
    glBegin(GL.LINES);
    glVertex3f(0.0, 0.0, 1.75);
    glVertex3f(0.0, 0.0, -1.75);
    glEnd();
    glPopMatrix;
    Screen('EndOpenGL', ptr);
    Screen('Flip', ptr);
    Screen('BeginOpenGL', ptr);
  end
  
return

function displaySphere


%  drawLine(0.0, 1.75, 0.0, 0.0, -1.75, 0.0);
%  drawLine(1.75, 0.0, 0.0, -1.75, 0.0, 0.0);
%  drawLine(0.0, 0.0, 1.75, 0.0, 0.0 , -1.75);
return

function drawLine(x1, y1, z1, x2, y2, z2)
  glEnable(GL.LINE_SMOOTH)
  glEnable(GL.BLEND);
  glBlendFunc(GL.SRC_ALPHA, GL.ONE_MINUS_SRC_ALPHA);
  glHint(GL.LINE_SMOOTH_HINT, GL.DONT_CARE);
  glLineWidth(7.0);

  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL.LINES);
  glVertex3f(x1,x1,z1);
  glVertex3f(x2, y2, z2);
  glEnd();
return

