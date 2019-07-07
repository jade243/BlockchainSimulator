function [state, length, rewards] = case2(alpha, length, rewards)
    % Function to handle the state 2. 
    % We jump to state 3 if the selfish miners find a block
    % We jump to state 0 if the honest miners find a block because we
    % publish the secret chain and earn the rewards.
    % Params :  - state : new state
    %           - alpha : computational power of the selfish miners
    %           - length : we save the length of the secret chain
    %           - rewards : number of rewards won

    new_block = bernoulli_dist(alpha); % 1 if the selfish miners find a new block
    state = 3*new_block;
    rewards = rewards + (1 - new_block)*length;
    length = new_block*(length + new_block);
    
            
end