%
% Matlab or Octave script to analyze logs produced by simulating mining process
% in blockchains.
% Aurélia Autem.
% University of Geneva - 07/2019.
%

clc
clear
close all

%% We set up some variables

% The times are originally in milliseconds so we can divide them to get
% seconds (by 1000), minutes (by 60,000), ...
div_time = 1000; 
square_size = 50;
measure = 'seconds';

%%

% First, let's check filenames and get their prefix 
[prefix, fileList] = check_logs_filenames();

% Some variables we want to fill with the general infos file
nb_transactions = 0;
nb_proc = 0;
final_time = 0;

file = fopen(strcat('../../logs/', strcat(prefix, '.txt')));
while ~feof(file)
   line = fgetl(file);
   [tag, index] = get_tag(line);
   
   if (strcmp(tag, 'nbTransactions'))
       line = line(index+2 : end);
       nb_transactions = str2double(line);
   else 
   if (strcmp(tag, 'nbProc'))
       line = line(index+2 : end);
       nb_proc = str2double(line);
   else
   if (strcmp(tag, 'finalTime'))
       line = line(index+2 : end);
       final_time = str2double(line);
   end
   end
   end
end
fclose(file);

procs = cell(nb_proc, 1);
pos = 1;

for i = 1 : length(fileList)
    filename = fileList{i};
    
    nb_blocks_mined = 0;
    blocks_mined = {};
    freq = 0;
    
    if (~strcmp(filename, strcat(prefix, '.txt')))
        % We extract the results produced by each proc during the execution 
        full_filename = strcat('../../logs/', filename);
        file = fopen(full_filename);
        
        while ~feof(file)
           line = fgetl(file);
           [tag, index] = get_tag(line);
           
           if (strcmp(tag, 'freq')) 
               freq = str2double(line(index+2 : end));
           else
           if (strcmp(tag, 'nbChains'))
               nb_chains = str2double(line(index+2 : end));
               
               chains = cell(1, nb_chains);
               line = fgetl(file);
               
               while ~feof(file)
                   
                   [j, index] = get_tag(line);
                   j = str2double(j)+1;
                   
                   chain = {line(index+2 : end)};
                        
                   while (~feof(file) || strcmp(line(1), '['))
                      line = fgetl(file);
                      chain{end+1} = line;
                   end
                   chains{j} = chain;
               end
           
               
           else
               nb_blocks_mined = nb_blocks_mined+1;
               [hours, mins, secs] = get_time(tag);
               line = line(index+2 : end);
               
               [block.time, block.id] = get_time_mining(line);
               block.hours = hours;
               block.mins = mins;
               block.secs = secs;
               blocks_mined{end+1} = block;
           end
           
           end
        end
        
        proc.freq = freq;
        proc.nb_blocks_mined = nb_blocks_mined;
        proc.blocks_mined = blocks_mined;
        proc.nb_chains = nb_chains;
        proc.chains = chains;
        
        procs{pos} = proc;
        pos = pos + 1;
        fclose(file);
        
    end
end

% We create a cell with proc names
names = cell(nb_proc, 1);
for i = 1 : nb_proc 
   names{i} = ['Proc ' int2str(i)];
end

clearvars -except final_time nb_proc nb_transactions procs names div_time square_size measure

%% We've extracted all data from the logs

var_names = {'Name', 'Freq', 'Rewards', 'Forks'};
f = struct2cell(cell2mat(procs));
T = table(names, cell2mat(f(1, :))', cell2mat(f(2, :))', cell2mat(f(4, :))', 'VariableNames', var_names);

longest_chain = get_longest_chain(procs);
[miners, times] = find_miners(longest_chain, procs);

longest_chain = fliplr(longest_chain);
miners = fliplr(miners);
times = fliplr(times);

var_times = {'Unit', 'Minimum', 'Maximum', 'Range', 'Mean', 'Median'};
T_times = table({measure}, min(times)/div_time, max(times)/div_time, ...
            (max(times)-min(times))/div_time, mean(times)/div_time, ...
            median(times)/div_time, 'VariableNames', var_times);

clearvars -except final_time nb_proc nb_transactions procs names div_time square_size measure T T_times longest_chain miners times

%% We've created all needed variables

% We display the table summing up the rewards won
TString = evalc('disp(T)');
% Use TeX Markup for bold formatting and underscores.
TString = strrep(TString,'<strong>','\bf');
TString = strrep(TString,'</strong>','\rm');
TString = strrep(TString,'_','\_');
% Get a fixed-width font.
FixedWidth = get(0,'FixedWidthFontName');
% Output the table using the annotation command.
annotation(gcf,'Textbox','String',TString,'Interpreter','Tex','FontName',FixedWidth, ...
            'Units','Normalized', 'Position',[0 0 1 1], 'FontSize', 10, ...
            'HorizontalAlignment', 'center', 'VerticalAlignment', 'middle');

saveas(gcf,'rewards.png')

% We display statistics 
figure
TString = evalc('disp(T_times)');
% Use TeX Markup for bold formatting and underscores.
TString = strrep(TString,'<strong>','\bf');
TString = strrep(TString,'</strong>','\rm');
TString = strrep(TString,'_','\_');
% Get a fixed-width font.
FixedWidth = get(0,'FixedWidthFontName');
% Output the table using the annotation command.
annotation(gcf,'Textbox','String',TString,'Interpreter','Tex','FontName',FixedWidth, ...
            'Units','Normalized', 'Position',[0 0 1 1], 'FontSize', 10, ...
            'HorizontalAlignment', 'center', 'VerticalAlignment', 'middle');

saveas(gcf,'stats.png')

% We display the evolution of the longest blockchain
square_y = 0;
margin_up = 0.2;
margin_low = 220;
colors = {[0 0.4470 0.7410], [0.8500 0.3250 0.0980], [0.9290 0.6940 0.1250]};

nb_blocks = 10;

times = times ./ div_time;

for j = 1 : size(longest_chain, 2) / nb_blocks
    figure
    hold on
    for i = 1+(j-1)*nb_blocks : nb_blocks+(j-1)*nb_blocks
        rectangle('Position', [times(i) square_y square_size square_size], ...
            'FaceColor', colors{mod(i, size(colors, 2))+1}, ...
            'EdgeColor', 'k', 'LineWidth', 2);
        legend_str = [int2str(i) '. ' longest_chain{i} ' by ' names{miners(i)}];
        line(NaN,NaN,'LineWidth',0.5,'LineStyle','none','Color',[0 0 0], 'DisplayName', legend_str);
    end
    legend('show')
    axis equal
    xlim([times(1+(j-1)*nb_blocks)-square_size times(i)+2*square_size])
    ylim([square_y-square_size square_y+20*square_size])
    xlabel(strcat('Time in ', measure))
    title(sprintf('Mining of %d transactions by %d miners', nb_transactions, nb_proc));
    set(gca,'YTick',[])
    hold off

    saveas(gcf,strcat('timeline_', int2str(j), '.png'))
end

clearvars -except final_time nb_proc nb_transactions procs names T longest_chain miners times





