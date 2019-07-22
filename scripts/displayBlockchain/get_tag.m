function [tag, index] = get_tag(line)
    index = get_next_character(']', line);
    tag = line(2 : index-1);
end