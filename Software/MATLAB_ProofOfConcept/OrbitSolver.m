function [X,Y, Mass] = OrbitSolver(dt, steps, Bodies)
% Clayton Kirberger
% Final Project
% PY 525
%
% This function uses Verlet integration to solve for the orbits of the
% specified celestial bodies and returns their positions and velocities at
% time t in an array with columns [mass, x(t), y(t), vx(t), vy(t)]. 

StateVarsI = loadObjects(Bodies); % Load initial masses, and positions: [mass, x(t), y(t), x(t+1), y(t+1)]
Mass = StateVarsI(:,1);
numP = size(Bodies); % How many bodies are there
X = zeros(steps,numP(2)); % Store X of each body at time t here
Y = zeros(steps,numP(2)); % Store Y of each body at time t here
X(1,:) = StateVarsI(:,2)';
X(2,:) = StateVarsI(:,4)'; % Establish initial conditions
Y(1,:) = StateVarsI(:,3)';
Y(2,:) = StateVarsI(:,5)';
G = 6.67408*10^(-11); % gravitational constant

for t = 2:steps-1
    for b = 1:numP(2)
        atot = zeros(1,2);
        for otb = 1:numP(2)
            if b~=otb % bodies dont have a force on themselve
                r = [X(t,otb),Y(t,otb)] - [X(t,b),Y(t,b)];
                rmag = sqrt(r(1).^2 + r(2).^2);
                runit = r./rmag;
                a = ((G*Mass(otb))/rmag.^2).*runit;
                atot = atot + a;
            end
        end
    X(t+1,b) = X(t,b) + (X(t,b)-X(t-1,b)) + atot(1)*(dt^2); % new x
    Y(t+1,b) = Y(t,b) + (Y(t,b)-Y(t-1,b)) + atot(2)*(dt^2); % new y

    
end







end

