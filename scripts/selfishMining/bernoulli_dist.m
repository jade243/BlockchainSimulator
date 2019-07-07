function alea = bernoulli_dist(alpha) 
    % Function which returns :
    % 1 if the positive experiment happens (with a probability of alpha)
    % 0 if the negative experiment happens (with a probability of 1 - alpha)
    alea = binornd(1, alpha);
end