function index = jump_next_words(n, string)
  % Function which gives the index to jump <n> words in the string.
  % Inputs :  - n : number of words to jump
  %           - string 
  % Outputs : - index : index in the string to jump <n> words or -1 if not possible
  
  index = 0;
  for i = 1 : n
    index_tmp = get_next_character(' ', string);
    if (index_tmp ~= -1)
      index = index + index_tmp;
      string = string(index_tmp+1:end);
    else
      index = -1;
      break;
    end
  end
  
end
