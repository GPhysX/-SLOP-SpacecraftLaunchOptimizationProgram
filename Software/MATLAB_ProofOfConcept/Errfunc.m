function Error = Errfunc(Vo, t0, Rtol, Vtol, Pdata, maxTime, dt, Rideal, Destination, verbose)
% This function outputs a value for the error in our guess. The lower this
% value is the better our current launch parameters.


Au = 1.496*10^(11); % astronomical unit

[Xcraft,Ycraft] = CraftPath(Vo,t0,Pdata,maxTime,dt);

dims = size(Xcraft);
Times = max(dims);
X = Pdata.X(t0:end,Destination);
Y = Pdata.Y(t0:end,Destination);

directory = '\Users\4lius\Documents\MATLAB\Fall2018\PY525Final\OptimizationVid\';
files = dir([directory, '*']); %which files are we processing ?
nFrames = length(files); %how many files are we processing ?
fname = num2str(nFrames-1);

if verbose
    figure(1)
    p1 = plot(Xcraft(1:365)./Au,Ycraft(1:365)./Au);
    hold on
    p2 = plot(X(1:365)./Au,Y(1:365)./Au);
    xlim([-2 2]);
    ylim([-2 2]);
    title('Optimization');
    xlabel('Distance(Au)');
    ylabel('Distance(Au)');
    saveas(gcf,[directory,fname,'Frame']);
    delete(p1);
    delete(p2);
end


Errors = zeros(Times,1);
Rmin = 10^20;
Vmin = 10^20;
for t = 2:Times
    r = sqrt((Xcraft(t) - X(t))^2 + (Ycraft(t) - Y(t))^2);
    Vp = [X(t)-X(t-1), Y(t)-Y(t-1)]./dt;
    Vc = [Xcraft(t)-Xcraft(t-1), Ycraft(t)-Ycraft(t-1)]./dt;
    Vdif = Vp - Vc;
    Vdif = sqrt(Vdif(1)^2 + Vdif(2)^2);
    Rp = abs(Rideal-r)/Rtol;
    Vp = abs(Vdif)/Vtol;
    
    if Rp < 1 % prevent damping of the Vp term
        Rp = 1;
    end
    if Vp < 1 % prevent damping of the Rp term
        Vp = 1;
    end
    
    Errors(t) = (Rp^2) * (Vp^4);
    Vmin = min([Vmin,Vdif]);
    Rmin = min([Rmin,abs(Rideal-r)]);
end

    Error = min(Errors(2:end));
    if verbose
        display("Vmin = "+num2str(Vmin)+" and Rmin = "+num2str(Rmin));
    end

end

