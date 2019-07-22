function [time, id] = get_time_mining(line)

    i = get_next_character(' ', line);
    id = line(1 : i-1);
    time = str2double(line(i+1 : end));

    
end