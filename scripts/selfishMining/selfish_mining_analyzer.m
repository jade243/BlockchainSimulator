%
% Matlab or Octave script to analyze probabilities of succeeding a selfish
% mining attack.
% Aurélia Autem.
% University of Geneva - 07/2019.
%

clc
clear
close all

%% Rewards according to alpha and gamma

% We evaluate the rewards selfish miners can win according to their 
% computational power

% Number of hours for the attack
hours = 24;  

% Computational powers of the selfish miners
alpha = [1/8, 1/4];
gamma = [0, 1/2, 1];

m = 10 : 10 : hours*6*10;
h = m ./ 60;

% We display the results
figure
hold on

k = 1;
for i = 1 : length(gamma)
   for j = 1 : length(alpha)
       [~, history] = selfish_mining(alpha(j), gamma(i), hours);
       plot(h, history, 'DisplayName', sprintf('\\alpha = %1.2f, \\gamma = %1.2f', alpha(j), gamma(i)), 'LineWidth', 2)
       k = k +1;
   end
end

hold off

title('Selfish mining with different values of \alpha and \gamma')
xlabel('Time in hours')
ylabel('Number of rewards')
legend('Location', 'NorthWest')

%% Simulation with the largest mining pools for Bitcoin with gamma = 1/2

hours = 24;
m = 10 : 10 : hours*6*10;
h = m ./ 60;

gamma = 1/2;
alpha = [0.197, 0.129, 0.120, 0.108];
names = {'BTC.com', 'AntPool', 'Unknown', 'Poolin'};

% We display the results
figure
hold on

for i = 1 : length(alpha)
   [~, history] = selfish_mining(alpha(i), gamma, hours);
   plot(h, history, 'DisplayName', strcat(names{i}, sprintf(', \\alpha = %1.3f', alpha(i))), 'LineWidth', 2)
end

hold off

title(sprintf('Selfish mining for the %d largest pools in Bitcoin and \\gamma = %1.2f', length(alpha), gamma))
xlabel('Time in hours')
ylabel('Number of rewards')
legend('Location', 'NorthWest')


%% Simulation with the largest mining pools for Bitcoin with gamma = 1

hours = 24;
m = 10 : 10 : hours*6*10;
h = m ./ 60;

gamma = 1;
alpha = [0.197, 0.129, 0.120, 0.108];
names = {'BTC.com', 'AntPool', 'Unknown', 'Poolin'};

% We display the results
figure
hold on

for i = 1 : length(alpha)
   [~, history] = selfish_mining(alpha(i), gamma, hours);
   plot(h, history, 'DisplayName', strcat(names{i}, sprintf(', \\alpha = %1.3f', alpha(i))), 'LineWidth', 2)
end

hold off

title(sprintf('Selfish mining for the %d largest pools in Bitcoin and \\gamma = %1.2f', length(alpha), gamma))
xlabel('Time in hours')
ylabel('Number of rewards')
legend('Location', 'NorthWest')

%% Simulation with the largest mining pools for Bitcoin with gamma = 0

hours = 24;
m = 10 : 10 : hours*6*10;
h = m ./ 60;

gamma = 0;
alpha = [0.197, 0.129, 0.120, 0.108];
names = {'BTC.com', 'AntPool', 'Unknown', 'Poolin'};

% We display the results
figure
hold on

for i = 1 : length(alpha)
   [~, history] = selfish_mining(alpha(i), gamma, hours);
   plot(h, history, 'DisplayName', strcat(names{i}, sprintf(', \\alpha = %1.3f', alpha(i))), 'LineWidth', 2)
end

hold off

title(sprintf('Selfish mining for the %d largest pools in Bitcoin and \\gamma = %1.2f', length(alpha), gamma))
xlabel('Time in hours')
ylabel('Number of rewards')
legend('Location', 'NorthWest')