if(~exist('s'))
    disp('Aready closed');
    return;
end
fclose(s)
delete(s)
clear s
disp('Arduino connection closed')