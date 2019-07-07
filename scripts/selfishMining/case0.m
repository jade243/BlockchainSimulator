function state = case0(alpha)
    % Function to handle the state 0. 
    % We jump to state 1 if the selfish miners find a block
    % We stay at state 0 if the honest miners find a block
    % Params :  - alpha : computational power of the selfish miners
    %           - state : new state
    state = bernoulli_dist(alpha); % 1 if the selfish miners find a new block
                                   % 0 otherwise
end