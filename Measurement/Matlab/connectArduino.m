instrreset(); % close any pre-existing connections
clear;

serialPort = 'COM10'; % change if needed

global s;
s = serial(serialPort);
set(s,'BaudRate',9600);
fopen(s);

% read welcome message
out = fscanf(s);
disp(strcat('Welcome: ',out));