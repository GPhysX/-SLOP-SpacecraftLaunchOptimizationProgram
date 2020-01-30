% Clayton Kirberger
% Final Project
% PY 525

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This program is designed to return optimal launch parameters for a
% spacecraft leaving from Earth
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% !!!!!! TO DO !!!!!!!!!!
% Add multistop trip functionality
% IE specify a vector with planets to "fly by"

close all %housekeeping
clear all %housekeeping

% Fit Constants
% These variables are the known values that will be optimized around
destPlanet = 5; % destination planet based on number from sun. String functionality to be added
dt = 86400; % Timestep used to calculate planetary motion in seconds
Rideal = 10^7;

verbose = 1; % Do we want to see some plots and animations? Turning this on will cause the program to take a long time to run


% Variables for optimizing the soonest arrival
% We don't have a specified endpoint so we need to set one to prevent
% infinite loops
maxTime = 500; % If a path doesn't connect after this number of steps then it is a nonsolution

% Other Variables
findOrbits = 0; % 1 to solve for orbits, 0 to load them from saved data
PloadName = 'Test.mat'; % Name of the file to load
PsaveName = 'Test.mat'; % The solved verlet data will be saved for future use
doall = 1;
Au = 1.496*10^(11); % astronomical unit

% Fit options to use in the optimizer
fitoptions = optimoptions('lsqnonlin','Algorithm','levenberg-marquardt','MaxIter',400,'MaxFunEvals',1600,'TolFun',10^-12,'Display','final-detailed','StepTolerance',1e-08);
Rtol = 10^6; % This number is Voodoo magic
Vtol = 10^3; % This number is Voodoo magic



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% First lets find the position of our solar objects at any given time t
% These will be used repetedly in the optimization process so better to
% calculated them once on the outside and then just pass them in
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

if destPlanet < 3 % if planet is closer to the Sun than Earth
    Bodies = [1, destPlanet:4];
else % planet is further from the sun than Earth
    Bodies = [1, 3:destPlanet];
end

if doall
    Bodies = [1:1:9];
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Get Planets position and velocity at time t here
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if findOrbits % Calculate the positions of the solar bodies
    [X,Y, Mass] = OrbitSolver(dt,maxTime,Bodies);
    % Save the data so it doesn't need to be calculated again
    Pdata.X = X;
    Pdata.Y = Y;
    Pdata.Mass = Mass;
    save(PsaveName,'Pdata');
else
    % load a previously saved set
    Pdata = load(PloadName);
    Psize = size(Pdata.Pdata);
    X = Pdata.Pdata.X;
    Y = Pdata.Pdata.Y;
    Mass = Pdata.Pdata.Mass;
    Pdata = Pdata.Pdata;
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Do some plotting to visualize planets orbits
% This was mostly used for debugging and should be commented out
% unless it is specifically desired
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

figure(1)
plot(X(:,1:9)./Au,Y(:,1:9)./Au)
xlim([-35, 35]);
ylim([-35, 35]);
title('Calculated Orbits');
xlabel('Distance(Au)');
ylabel('Distance(Au)');

% A fun animation of the inner planets
% Animation can take a very long time for a large maxTime
% if verbose
% figure(2)
% xlim([-2, 2]);
% ylim([-2, 2]);
% xlabel('Distance (Au)');
% ylabel('Distance (Au)');
% zlabel('Time (days)');
% hold on
% grid on;
% h1 = animatedline('color','y');
% h2 = animatedline();
% h3 = animatedline('color','b');
% h4 = animatedline('color','g');
% h5 = animatedline('color','r');
% legend('Sun','Mercury','Venus','Earth','Mars')
% view(3);
% for t = 1:maxTime
%     addpoints(h1,X(t,1)./Au,Y(t,1)./Au,t);
%     addpoints(h2,X(t,2)./Au,Y(t,2)./Au,t);
%     addpoints(h3,X(t,3)./Au,Y(t,3)./Au,t);
%     addpoints(h4,X(t,4)./Au,Y(t,4)./Au,t);
%     addpoints(h5,X(t,5)./Au,Y(t,5)./Au,t);
%     drawnow
% end
% end

% Here ends the animation Stuff and Things

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% !!!!!!!!!HERE STARTS THE OPTIMIZATION!!!!!!
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CRITICAL ASSUMPTIONS
% 1. If the spacecraft gets within an inner planets perihelion, or beyond
%    an outer planets aphelion consider the mission failed
% 2. If after maxTime timesteps the spacecraft has not arrived at the
%    planet consider the mission failed
% 3. If the spacecraft does not stay within a certain distance of the
%    destination for a certain number of timesteps consider this a "flyby"
%    option, but not a solution
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Variables to Optimize
% A good initial guess here will help immensly
% TODO add some form of automated guessing

t0 = 1; % when are we launching from Earth
%Vmag = [3500,4000,4500,5000,5500,6000,6500,7000]; % magnitude of initial velocity guess: OPTIMIZATION HIGHLY SENSITIVE TO THIS NUMBER
Vmag = [4500]
numMag = length(Vmag);

% TODO: autogenerate a user defined number of directions
%Vang = [1 0;-1 0;0 1;0 -1;1/sqrt(2) 1/sqrt(2);-1/sqrt(2) 1/sqrt(2);1/sqrt(2) -1/sqrt(2);-1/sqrt(2) -1/sqrt(2)];
Vang = [1 0]
numAngles = length(Vang);

Vopt = zeros(numMag,numAngles,2); % store our optimized guess here
Errors = zeros(numMag,numAngles);

err = @(V) Errfunc(V, t0, Rtol, Vtol, Pdata, maxTime, dt, Rideal, destPlanet, 1);
for s = 1:numMag
    optG = zeros(numAngles,2);
    for g = 1:numAngles
        optG(g,:) = lsqnonlin(err,Vmag(s).*Vang(g,:),[],[],fitoptions);
        Errors(s,g) = err(optG(g,:));
    end
    Vopt(s,:,:) = optG;
end

[minErr, idminy] = min(Errors);
[absMin, idminx] = min(minErr);
Errfunc(Vopt(idminy(idminx),idminx,:), t0, Rtol, Vtol, Pdata, maxTime, dt, Rideal, destPlanet, 1);
[Xcraft Ycraft] = CraftPath(Vopt(idminy(idminx),idminx,:),t0,Pdata,maxTime,dt);


if verbose
gcf = figure(3)
set(gcf, 'Units', 'Normalized', 'OuterPosition', [0, 0.04, 1, 0.96]);
xlim([-2, 2]);
ylim([-2, 2]);
xlabel('Distance (Au)');
ylabel('Distance (Au)');
% zlabel('Time (days)');
hold on
grid on;
h1 = animatedline('color','y','marker','o','markersize',10,'markerfacecolor','y');
h2 = animatedline('color','k','marker','o','markersize',10,'markerfacecolor','k');
h3 = animatedline('color','b','marker','o','markersize',10,'markerfacecolor','b');
h4 = animatedline('color','g','marker','o','markersize',10,'markerfacecolor','g');
h5 = animatedline('color','r','marker','o','markersize',10,'markerfacecolor','r');
% h6 = animatedline('color','k');
hc = animatedline('color','c','marker','o','markersize',10,'markerfacecolor','c');
legend('Sun','Mercury','Venus','Earth','Mars','Craft')
% view(3);
vidObj = VideoWriter('VenusTrip.avi');
open(vidObj);
for t = 1:maxTime
    addpoints(h1,X(t0 + t -1,1)./Au,Y(t0 + t -1,1)./Au,t);
    addpoints(h2,X(t0 + t -1,2)./Au,Y(t0 + t -1,2)./Au,t);
    addpoints(h3,X(t0 + t -1,3)./Au,Y(t0 + t -1,3)./Au,t);
    addpoints(h4,X(t0 + t -1,4)./Au,Y(t0 + t -1,4)./Au,t);
    addpoints(h5,X(t0 + t -1,5)./Au,Y(t0 + t -1,5)./Au,t);
    addpoints(hc,Xcraft(t)./Au,Ycraft(t)./Au);
    drawnow
    currFrame = getframe;
    writeVideo(vidObj,currFrame);
    clearpoints(h1);
    clearpoints(h2);
    clearpoints(h3);
    clearpoints(h4);
    clearpoints(h5);
    clearpoints(hc);
end
close(vidObj);
end

