function [state, length] = case1(alpha)
    % Function to handle the state 1. 
    % We jump to state 2 if the selfish miners find a block
    % We jump to state 0' (called -1) if the honest miners find a block
    % Params :  - state : new state
    %           - length : we save the length of the secret chain
    %           - alpha : computational power of the selfish miners
    new_block = bernoulli_dist(alpha); % 1 if the selfish miners find a new block
    state = 3*new_block - 1;
    length = 2*new_block;
end