function [miners, times] = find_miners(longest_chain, procs)
    miners_tmp = [];
    times_tmp = [];
    blocks_tmp = {};
    
    f = struct2cell(cell2mat(procs));
    for i = 1 : size(f(2, :), 2)
        cell = f(2, i);
        cell = cell{1};
        for structure = cell
           miners_tmp = [miners_tmp i];
           times_tmp = [times_tmp structure{1}.time];
           blocks_tmp{end+1} = structure{1}.id;
        end
    end
    
    miners = zeros(size(longest_chain));
    times = zeros(size(longest_chain));
    for i = 1 : size(longest_chain, 2)
       for j = 1 : size(blocks_tmp, 2)
          if (strcmp(longest_chain{i}, blocks_tmp{j}))
              miners(i) = miners_tmp(j);
              times(i) = times_tmp(j);
          end
       end
    end
    
    
end