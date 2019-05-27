function index = get_next_character(character, string)
    index = -1;
    for i = 1 : length(string)
      
      if (strcmp(string(i), character))
        index = i;
        break;
        end
      
      end
  end