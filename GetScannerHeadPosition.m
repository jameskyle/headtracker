% Project Name: HeadTracker
% Matlab Function: GetScannerHeadPosition(fid)
%   PreCondition: A valid file descriptor is passed to method. The file contains
%                 a single line of tab deliminated coordinates of 
%                 x, y, z, yaw, pitch, roll. x,y, & z are given as absolute
%                 positionals. Yaw, pitch, and roll are given as angles. 
%
%   PostCondition: x_shift, y_shift, z_shift, yaw,pitch, and roll are assigned 
%                  the values contained within the passed file. this implies a 
%                  valid read is Performed on the file. 
%                  If the the read fails, no values are assigned to the 
%                  positional variables.
%                  
%%%%%%%%%%%%%%%%%%%%%%%%%%%% Variable Description %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [coords] = GetScannerHeadPosition(fid,coords)
  if fid ~= -1
    [fail] = fseek(fid, 0, 'bof');
    if fail == -1
      coords.new_coords = false;
    else
      s = fgetl(fid);
      
      if ischar(s)
        [result] = sscanf(s,'%d\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f');

        coords.x      = result(2);
        coords.y      = result(3);
        coords.z      = result(4);
        coords.yaw 	  = result(5);
        coords.pitch  = result(6);
        coords.roll 	= result(7);
      end
    end;
  else
    coords.new_coords = false;
  end;
return
