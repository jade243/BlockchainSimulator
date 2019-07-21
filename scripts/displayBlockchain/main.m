%
% Matlab or Octave script to analyze logs produced by simulating mining process
% in blockchains.
% Aurélia Autem.
% University of Geneva - 07/2019.
%
%%

% First, let's check filenames and get their prefix 
[prefix, fileList] = check_logs_filenames();

% Then, we define keywords
keywords = {"nbChains"};

% Some variables we want to fill
nb_transactions = 0;
nb_proc = 0;

file = fopen(strcat('../logs/', strcat(prefix

for filename = fileList
  
   if (strcmp(fileName, strcat(prefix, '.txt')))
    % We extract the general infos about the execution
    
  else
    % We extract the results produced by each proc during the execution 
    full_filename = strcat('../logs/', fileName);
    file = fopen(fileName);
    
    while ~feof(file)
      line = fgetl(file);
      
      i = get_next_character(']', line); 
      tag = line(2, i-1);
     
      if (strcmp(tag, "nbChains"))
        
      else
      
      end
    
    end
  end
  
end
