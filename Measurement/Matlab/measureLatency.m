s = serial('COM10');
set(s,'BaudRate',9600);
fopen(s);

% read welcome message
out = fscanf(s);
disp(strcat('Welcome: ',out));

% send start command
fprintf(s,'m');
out = fscanf(s);
status = startsWith(out,'Starting measurement...');

if(status==1)
    disp('Measurement started...');
else
    disp('An error occurred');
    disp(out);
    return;
end

out = fscanf(s);

if(startsWith(out,'Detected:')) 
    number=extractAfter(st,'Detected: ');
    result=str2double(number);
    result=result / 1000;
    result=round(result,1); % reduce digits
elseif(startsWith(out,'Timed out'))
    disp('Time out occurred');
else
    disp('Unknown response');
    disp(out);
    return;
end

fclose(s)
delete(s)
clear s