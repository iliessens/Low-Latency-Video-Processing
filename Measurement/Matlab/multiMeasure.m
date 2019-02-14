n=10;

results = zeros(1,n);
for i=1:n
    result = measureLatency;
    if(result < 0) 
        disp('Stopping measurements');
        return;
    else
        results(i) = result;
    end
end

disp([ newline '** DONE **' newline]);
disp('Results:');
disp(results);

m=mean(results);
fprintf('Mean: %0.2f\n',m);

m=std(results);
fprintf('Standard Deviation: %0.2f\n',m);

%create scatter plot
x=1:n;
scatter(x,results);
xlabel 'Measurement []'
ylabel 'Latency [ms]'
title 'Latency Measurements'
