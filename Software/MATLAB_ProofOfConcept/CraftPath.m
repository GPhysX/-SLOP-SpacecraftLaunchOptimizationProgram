function [Xcraft,Ycraft] = CraftPath(Vo, to, Pdata, Tmax, dt)
% This function outputs a value for the error in our guess. The lower this
% value is the better our current launch parameters.

% Input Arguments
% 
% Vo = vector describing the initial velocity of our spacecraft
% to = what time are we launching
% Pdata = A structure with data about the relevant solar bodies positions and masses
% Tmax = maximum number of timesteps we allow for a viable solution
% dt = timestep to use in verlet solver
% 
% 
% Output Arguments
%
% Xcraft = position of the craft in x
% Ycraft = position of the craft in y

% First we need to simulate the motion of our spacecraft
X = Pdata.X(to:Tmax,:); % Planets positions in x
Y = Pdata.Y(to:Tmax,:); % Planets positions in y
Mass = Pdata.Mass; % Planets Mass
G = 6.67408*10^(-11); % gravitational constant
Xcraft = zeros(Tmax-to+1,1); % Store our spacecrafts positions in x here
Ycraft = zeros(Tmax-to+1,1); % Store our spacecrafts positions in y here
Erad = 6.371*10^6; % Earth Radius

Vunit = Vo./sqrt(Vo(1)^2+Vo(2)^2);
Xcraft(1,1) = Pdata.X(to,4) + 20*Erad*Vunit(1); % position of Earth at time to + a displacement in the Vo direction of 3 earth radii
Ycraft(1,1) = Pdata.Y(to,4) + 20*Erad*Vunit(2); % position of Earth at time to + a displacement in the Vo direction of 3 earth radii
atot = zeros(1,2);
for b = 1:9 % for all planets
    r = [X(to,b),Y(to,b)] - [Xcraft(1,1),Ycraft(1,1)];
    rmag = sqrt(r(1).^2 + r(2).^2);
    runit = r./rmag;
    a = ((G*Mass(b))/rmag.^2).*runit;
    atot = atot + a;
end

Xcraft(2) = Xcraft(1) + Vo(1)*dt + (X(2,4)-X(1,4)) + atot(1)*(dt^2); % new x
Ycraft(2) = Ycraft(1) + Vo(2)*dt + (Y(2,4)-Y(1,4)) + atot(2)*(dt^2); % new y




for t = 2:Tmax-to
    atot = zeros(1,2);
    for b = 1:9
        r = [X(t,b),Y(t,b)] - [Xcraft(t,1),Ycraft(t,1)];
        rmag = sqrt(r(1).^2 + r(2).^2);
        runit = r./rmag;
        a = ((G*Mass(b))/rmag.^2).*runit;
        atot = atot + a;
    end

    
    Xcraft(t+1) = Xcraft(t) + (Xcraft(t) - Xcraft(t-1)) + atot(1)*(dt^2); % new x
    Ycraft(t+1) = Ycraft(t) + (Ycraft(t) - Ycraft(t-1)) + atot(2)*(dt^2); % new y
end




end

