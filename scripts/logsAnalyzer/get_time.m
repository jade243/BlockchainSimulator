function [hours, mins, secs] = get_time(string_date)
  % Function which returns the time from a string in the following format :
  % Www Mmm dd hh:mm:ss yyyy.
  % Inputs :  - string_date : string containing the date
  % Outputs : - hours, mins, secs : time extracted in uint8
  
  hours = str2num(['uint8(',string_date(12:13),')']);
  mins = str2num(['uint8(',string_date(15:16),')']);
  secs = str2num(['uint8(',string_date(18:19),')']);
end
