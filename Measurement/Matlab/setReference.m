global s;
if(isa(s,'double') || ~isvalid(s))
    disp('Running connection setup');
    connectArduino; % WARNING: this clears the workspace
end

val = input('New reference offset: ');
if(val > 255 || val <= 0) 
    disp('Invalid value must be in [0,255]');
    return;
end

% send command
fprintf(s,num2str(val));
out = fscanf(s);
status = startsWith(out,'New reference offset set to:');

if(status~=1)
    warning('Error settings reference voltage offset');
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