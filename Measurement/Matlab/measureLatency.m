function [result] = measureLatency()

    global s;
    if(~exist('s','var'))
        disp('Running connection setup');
        connectArduino; % WARNING: this clears the workspace
    end
    
    result = -1; % error code

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

    out = fscanf(s); % read result

    if(startsWith(out,'Detected:')) 
        number=extractAfter(out,'Detected: ');
        result=str2double(number);
        fprintf('Result received: %d µs\n',result);
        if(result < 0)
            disp('Invalid result');
            return;
        end
        result=result / 1000;
        result=round(result,1); % reduce digits
    elseif(startsWith(out,'Timed out'))
        disp('Time out occurred');
        return;
    else
        disp('Unknown response');
        disp(out);
        return;
    end
end