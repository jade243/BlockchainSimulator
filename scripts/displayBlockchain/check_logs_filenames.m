function prefix = check_logs_filenames() 
  list = dir('../logs/*.txt');
  fileList = {};

  for i = 1 : size(list, 1)
   
    fileList{i} = list(i).name;
   
  end

  prefix = '';
  res = false;
  if (length(fileList) ~= 0)
    
    % Get the prefix of the files
    prefix = '';
    fileName = fileList{1};
    
    for j = 1 : 3
      i = get_next_character('_', fileName);
      prefix = strcat(prefix, fileName(1:i));
      fileName = fileName(i+1:end);
    end
    
    i = get_next_character('_', fileName);
    if (i == -1)
      i = get_next_character('.', fileName);
    end
    prefix = strcat(prefix, fileName(1:i-1));
    
    % Then, we check that all files have the same prefix and that one file exists with the general infos.
    gen = false;
    same_prefix = true;
    for i = 1 : length(fileList)
      fileName = fileList{i};
      if (strcmp(fileName, strcat(prefix, '.txt')))
        gen = true;
      end
      
      if (!strcmp(prefix, fileName(1:length(prefix))))
        same_prefix = false;
      end
    end
    res = gen && same_prefix;
  end

  if (res == false)
    disp('Something wrong with the files');
    exit(-1);
  end

end