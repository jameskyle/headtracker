
function RenderHeadPosition(ptr,aspect_ratio,x_center,y_center,scale_factor, duration, fid)
  % Initialize opengl libs
  InitializeMatlabOpenGL;
  
  % Declare the coords struct and assign defaults
  coords = struct('x', 0, 'y', 0, 'z', 0, 'yaw', 0, 'pitch', 0, 'roll', 0,'newcoords', false)
  
  % BEGIN TEXTURE ATTEMPT
  % Setup the OpenGL rendering context of the onscreen window for use by
  % OpenGL wrapper. After this command, all following OpenGL commands will
  % draw into the onscreen window 'win':
  Screen('BeginOpenGL', ptr);


  % If you want to do any shape from shading studies, it is very important to
  % understand the difference between a local lighting model and a global
  % illumination model!!!
  glEnable(GL.LIGHTING);

  % Enable the first local light source GL.LIGHT_0. Each OpenGL
  % implementation is guaranteed to support at least 8 light sources,
  % GL.LIGHT0, ..., GL.LIGHT7
  glEnable(GL.LIGHT0);

  % Enable proper occlusion handling via depth tests:
  glEnable(GL.DEPTH_TEST);

  % Set projection matrix: This defines a perspective projection,
  % corresponding to the model of a pin-hole camera - which is a good
  % approximation of the human eye and of standard real world cameras --
  % well, the best aproximation one can do with 3 lines of code ;-)
  glMatrixMode(GL.PROJECTION);
  glLoadIdentity;

  % Field of view is 25 degrees from line of sight. Objects closer than
  % 0.1 distance units or farther away than 100 distance units get clipped
  % away, aspect ratio is adapted to the monitors aspect ratio:
  gluPerspective(30,1/aspect_ratio,0.1,100);


  % Finish OpenGL rendering into PTB window. This will switch back to the
  % standard 2D drawing functions of Screen and will check for OpenGL errors.
  Screen('EndOpenGL', ptr);

  % Show rendered image at next vertical retrace:
  % Screen('Flip', ptr);
      
  % Now we draw a solid, spinning textured sphere of radius 1.0.

  % Prepare texture to by applied to the sphere: Load & create it from an image file:
  myimg = imread([PsychtoolboxRoot 'PsychDemos/OpenGL4MatlabDemos/earth_512by256.jpg']);

  % Make a special power-of-two texture from the image by setting the enforcepot - flag to 1
  % when calling 'MakeTexture'. GL_TEXTURE_2D textures (==power of two textures) are
  % especially easy to handle in OpenGL. If you use the enforcepot flag, it is important
  % that the texture image 'myimg' has a width and a height that is exactly a power of two,
  % otherwise this command will fail: Allowed values for image width and height are, e.g.,
  % 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048 and on some high-end gfx cards
  % 4096 pixels. Our example image has a size of 512 by 256 pixels...
  % Psychtoolbox also supports rectangular textures of arbitrary size, so called
  % GL_TEXTURE_RECTANGLE_2D textures. These are normally used for Screen's drawing
  % commands, but they are more difficult to handle in standard OpenGL code...
  mytex = Screen('MakeTexture', ptr, myimg, [], 1);

  % Retrieve OpenGL handles to the PTB texture. These are needed to use the texture
  % from "normal" OpenGL code:
  [gltex, gltextarget] = Screen('GetOpenGLTexture', ptr, mytex);

  % Begin OpenGL rendering into onscreen window again:
  Screen('BeginOpenGL', ptr);

  % Enable texture mapping for this type of textures...
  glEnable(gltextarget);

  % Bind our texture, so it gets applied to all following objects:
  glBindTexture(gltextarget, gltex);

  % Textures color texel values shall modulate the color computed by lighting model:
  glTexEnvfv(GL.TEXTURE_ENV,GL.TEXTURE_ENV_MODE,GL.MODULATE);

  % Clamping behaviour shall be a cyclic repeat:
  glTexParameteri(gltextarget, GL.TEXTURE_WRAP_S, GL.REPEAT);
  glTexParameteri(gltextarget, GL.TEXTURE_WRAP_T, GL.REPEAT);

  % Set up minification and magnification filters. This is crucial for the thing to work!
  glTexParameteri(gltextarget, GL.TEXTURE_MIN_FILTER, GL.LINEAR);
  glTexParameteri(gltextarget, GL.TEXTURE_MAG_FILTER, GL.LINEAR);

  % Set basic "color" of object to white to get a nice interaction between the texture
  % and the objects lighting:
  glMaterialfv(GL.FRONT_AND_BACK,GL.AMBIENT, [ 1 1 1 1 ]);
  glMaterialfv(GL.FRONT_AND_BACK,GL.DIFFUSE, [ 1 1 1 1 ]);

  % Reset our virtual camera and all geometric transformations:
  glMatrixMode(GL.MODELVIEW);
  glLoadIdentity;

  % Reposition camera (see above):
  gluLookAt(0,0,10,0,0,0,0,1,0);

  % Create the sphere as a quadric object. This is needed because the simple glutSolidSphere
  % command does not automatically assign texture coordinates for texture mapping onto a sphere:
  % mysphere is a handle that you need to pass to all quadric functions:
  mysphere = gluNewQuadric;

  % Enable automatic generation of texture coordinates for our quadric object:
  gluQuadricTexture(mysphere, GL.TRUE); 



  % Now for our little animation loop. This loop will run until a key is pressed.
  % It rotates the object by a few degrees (actually: Applies a rotation transformation
  % to all objects to be drawn) and then redraws it at its new orientation:

  % Clear out backbuffer and depth buffer:
  start = GetSecs()
  % rotate pitch 90 degrees to approximate subject head position while supine
  % glRotatef(90, 1,0,0);
  glClear;
  % Increment rotation angle around new z-Axis (0,0,1)  by 0.1 degrees:
  glRotatef(0, 0,1,0);
  glRotatef(0, 1,0,0);
  glRotatef(0, 0,0,1);
 
  % Draw the textured sphere-quadric of radius 0.7. As OpenGL has to approximate
  % all curved surfaces (i.e. spheres) with flat triangles, we tell it to resolve
  % the sphere into 100 slices in elevation and 100 sectors in azimuth: Higher values
  % provide a better approximation, but they take longer to draw. Live is full of
  % trade-offs...
  gluSphere(mysphere, 0.7, 100, 100);
  % Finish OpenGL rendering into PTB window. This will switch back to the
  % standard 2D drawing functions of Screen and will check for OpenGL errors.
  Screen('EndOpenGL', ptr);
  % Show new image at next retrace:
  Screen('Flip', ptr);

  % Start OpenGL rendering again after flip for drawing of next frame...
  Screen('BeginOpenGL', ptr);
  % Ready for next draw loop iteration...

  while ~KbCheck 
    [coords] = GetScannerHeadPosition(fid,coords);
    
    glPushMatrix;
    glClear;
    
    % animates positional change along the x,y,z axis
    glTranslatef(-coords.x,-coords.y,0);
     % Increment rotation angle around new z-Axis (0,0,1)  by 0.1 degrees:
    glRotatef(-coords.pitch, 1,0,0);
    glRotatef(-coords.yaw,   0,1,0);
    glRotatef(-coords.roll,  0,0,1);
   
    % Draw the textured sphere-quadric of radius 0.7. As OpenGL has to approximate
    % all curved surfaces (i.e. spheres) with flat triangles, we tell it to resolve
    % the sphere into 100 slices in elevation and 100 sectors in azimuth: Higher values
    % provide a better approximation, but they take longer to draw. Live is full of
    % trade-offs...
    gluSphere(mysphere, 0.7, 100, 100);
    glPopMatrix;
    % Finish OpenGL rendering into PTB window. This will switch back to the
    % standard 2D drawing functions of Screen and will check for OpenGL errors.
    Screen('EndOpenGL', ptr);
    % Show new image at next retrace:
    Screen('Flip', ptr);

    % Start OpenGL rendering again after flip for drawing of next frame...
    Screen('BeginOpenGL', ptr);
    % Ready for next draw loop iteration...
  end;
%  while GetSecs() < start + duration 
%    
%    
%  end;
%  % END ATTEMPT TO IMPORT TEXTURE
return
