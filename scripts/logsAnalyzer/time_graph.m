%
% Matlab or Octave script to analyze logs produced by simulating mining process
% in blockchains.
% Aurélia Autem.
% University of Geneva - 06/2019.
%
%%

% First, let's analyse the logs folder and check the files' names.
[prefix, fileList] = check_logs_filenames();

%% Now, we can extract the data from the files

fileList
prefix
times = [];
ids = [];
for i = 1 : length(fileList)
  fileName = fileList{i};
  
  if (strcmp(fileName, strcat(prefix, '.txt')))
    % We extract the general infos about the execution
    
  else
    % We extract the results produced by each proc during the execution 
    
    % We get the id of the proc
    str_tmp = fileName(length(prefix)+2 : end);
    i = get_next_character('.', str_tmp);
    id = str2num(['uint32(',str_tmp(1:i-1),')']) + 1;
    
    % We get the times where each block where mined
    times = [times; read_infos_from_file(strcat('../logs/', fileName))];
    ids = [ids; id];
    

  end
  
end

%% We display our results in a graph 

figure
legend_str = {};
hold on
for i = 1 : length(ids)
  plot(times(i, :), '+-')
  legend_str{i} = int2str(ids(i));
end

%legend(legend_str);
xlabel('Blocks');
ylabel('Time in milliseconds');
title(sprintf('Total time to mine %d blocks by %d miners', size(times, 2), size(times, 1)));
hold off


