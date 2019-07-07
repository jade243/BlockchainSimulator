function [state, length] = caseOtherwise(state, alpha, length)
    % Function to handle the state n. 
    % We jump to state n+1 if the selfish miners find a block
    % We jump to state n-1 if the honest miners find a block
    % Params :  - state : new state
    %           - alpha : computational power of the selfish miners
    %           - length : we save the length of the secret chain
    new_block = bernoulli_dist(alpha); % 1 if the selfish miners find a new block
    state = state - 1 + 2*new_block;
    length = length + new_block;
end