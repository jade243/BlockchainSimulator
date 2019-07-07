function [state, rewards] = caseMinus1(rewards, gamma)
    % Function to handle the state 0' (called -1 here). 
    % We will alwats jump to state 0
    % There is one chance over two that the selfish miners' blocks will
    % be rewarded.
    % Params :  - rewards : number of rewards won
    %           - state : new state
    state = 0;
    rewards = rewards + bernoulli_dist(gamma);
end