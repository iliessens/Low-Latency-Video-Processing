if~(exist('result','var'))
    disp('Please start a measurement first');
    return;
end

name = input('Filename: ','s');
path = strcat('../',name,'.csv');
csvwrite(path,results');