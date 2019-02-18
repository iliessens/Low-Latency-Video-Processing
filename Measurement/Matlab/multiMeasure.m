n=30;

results = zeros(1,n);
for i=1:n
    result = measureLatency;
    if(result < 0) 
        disp('Invalid measurement dropped');
    else
        results(i) = result;
    end
    pause(1);
end

results = results(results~=0); % remove dropped values

disp([ newline '** DONE **' newline]);
disp('Results:');
disp(results);

m=mean(results);
fprintf('Mean: %0.2f\n',m);

m=std(results);
fprintf('Standard Deviation: %0.2f\n',m);

%create scatter plot
x=1:length(results);
scatter(x,results);
xlabel 'Measurement []'
ylabel 'Latency [ms]'
title 'Latency Measurements'
