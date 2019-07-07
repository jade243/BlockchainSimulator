function [rewards, history] = selfish_mining(alpha, gamma, hours)
    state = 0;
    rewards = 0;
    length = 0;
    history = zeros(1, hours*6);
    for i = 1 : hours*6
        %fprintf('I = %d, state = %d', i, state);
        switch state
            case 0
                state = case0(alpha);
            case -1
                [state, rewards] = caseMinus1(rewards, gamma);
            case 1
                [state, length] = case1(alpha);
            case 2
                [state, length, rewards] = case2(alpha, length, rewards);
            otherwise
                [state, length] = caseOtherwise(state, alpha, length);
        end
        history(i) = rewards;
        %fprintf(', length = %d, rewards = %d\n', length, rewards);
    end
    if (length >= 2)
       rewards = rewards + length; 
    end
    history(i) = rewards;
end