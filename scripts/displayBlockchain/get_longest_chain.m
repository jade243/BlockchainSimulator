function longest_chain = get_longest_chain(procs)
    longest_chain = [];
    
    for i = 1 : size(procs, 1)
        for j = 1 : size(procs{i}.chains, 1)
            if (size(procs{i}.chains{j}, 2) > size(longest_chain, 2))
               longest_chain = procs{i}.chains{j};
            end
        end
    end
    
end