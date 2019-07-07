function times = read_infos_from_file(fileName)
  % Function which reads the infos from a file.
  % Inputs :  - fileName : name of the file
  % Outputs : - times : vector with the times when the blocks were mined
  
  file = fopen(fileName);
  times = [];
  while ~feof(file)
      line = fgetl(file);
      i = get_next_character(']', line);
      [hours, mins, secs] = get_time(line(2:i-1));
      
      line = line(i+2:end);
      i = jump_next_words(4, line);
      line = line(i+1 : end);
      
      i = get_next_character(' ', line);
      time = str2num(['uint32(',line(1 : i-1),')']);
      times = [times time];
      
      %fprintf('The execution finished at %d:%d:%d and took %d milliseconds\n', hours, mins, secs, time);
  end
  
  for i = 2 : length(times)
    times(i) = times(i) + times(i-1);
  end
  


  fclose(file);
end
