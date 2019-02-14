global s;
if(~exist('s','var'))
    disp('Running connection setup');
    connectArduino; % WARNING: this clears the workspace
end

val = input('New reference value: ');
if(val > 255 || val <= 0) 
    disp('Invalid value must be in [0,255]');
    return;
end

% send command
fprintf(s,num2str(val));
out = fscanf(s);
status = startsWith(out,'New reference set to:');

if(status~=1)
    warning('Error settings reference voltage');
    return;
end

disp('New reference set, waiting for stabilization');
pause(1);
disp('Testing measurement');

status = measureLatency();
if(status < 0)
    warning('Measurement unsuccesfull');
else
    disp('New calibration OK');
end