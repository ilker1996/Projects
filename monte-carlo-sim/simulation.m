lambda = 4; % Poisson variable
hours = 3;  % time that is given
epsilon = 0.005; % error 
p = 0.95; % probability
size = (0.25) * ((norminv(p + ((1-p)/2))) / epsilon)^2 ; % Sizeof the Monte Carlo  simulation found by normal  approximation
fishweight = 0; %initial value of total weight
for i = 1:size  %Number of trials = size
    fishweight = 0; %reset total weight for every trial
    for j=1:poissrnd(lambda* hours) %poisson random variable of caught fishes
        fishweight += weight(rand *3 ); % add weight of fishes that is caught 
    end;
    fishing(i) = fishweight;   % make array of weights
    overweight(i) = (fishweight > 25); %make list of successes or failures of catching weight of 25 kg fishes
end;

[mean(overweight),mean(fishing),std(fishing)] % a)Estimated probability ,
                                              b) estimated total weight ,
                                              c) estimated Std


% About accuracy of estimator , since we use normal approximation(intelligent guess) to find number of trials , we can say it is pretty accurate with a small error but since we can't use all samples it is not certain and have small errors.To sum up, this estimator has error but no more than 0.005 but it is not totally accurate as well. 
