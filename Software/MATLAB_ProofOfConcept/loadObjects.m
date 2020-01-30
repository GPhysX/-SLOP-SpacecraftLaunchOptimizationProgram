function Pinfo = loadObjects(Bodies)
% This function returns initial conditions for the various bodies in the
% solar system
% Currently these values are hardcoded for a starting date of 4/29/2018
% TODO: change this to load data for any given date by accessing NASA JPL
% Horrizons

    Planets = zeros(9,5);
    Au = 1.496*10^(11);


    Planets(1,1) = 1.988544*10^30;
    Planets(1,2) = 1.064580386382038*10^(-3)*Au;
    Planets(1,3) = 6.673591014213232*10^(-3)*Au;
    Planets(1,4) = 1.057911630192468*10^(-3)*Au;
    Planets(1,5) = 6.677784352337204*10^(-3)*Au;
    
    Planets(2,1) = 3.302*10^23;
    Planets(2,2) = 2.257417565243433*10^(-2)*Au;
    Planets(2,3) = -4.535262068001816*10^(-1)*Au;
    Planets(2,4) = 4.498474119697951*10^(-2)*Au;
    Planets(2,5) = -4.500739549572465*10^(-1)*Au;
    
    Planets(3,1) = 4.8685*10^24;
    Planets(3,2) = -1.708530943867783*10^(-1)*Au;
    Planets(3,3) = 7.045088517076489*10^(-1)*Au;
    Planets(3,4) = -1.904969151696780*10^(-1)*Au;
    Planets(3,5) = 6.992853448309631*10^(-1)*Au;
    
    Planets(4,1) = 5.972*10^24;
    Planets(4,2) = -7.881777600388478*10^(-1)*Au;
    Planets(4,3) = -6.186383291720821*10^(-1)*Au;
    Planets(4,4) = -7.776661544312061*10^(-1)*Au;
    Planets(4,5) = -6.320923380578504*10^(-1)*Au;
    
    Planets(5,1) = 6.39*10^23;
    Planets(5,2) = -4.619641897160680*10^(-1)*Au;
    Planets(5,3) = -1.415971604202565*Au;
    Planets(5,4) = -4.481164825506957*10^(-1)*Au;
    Planets(5,5) = -1.419034933277763*Au;
    
    Planets(6,1) = 1.898*10^27;
    Planets(6,2) = -3.665828993669469*Au;
    Planets(6,3) = -3.971716516020388*Au;
    Planets(6,4) = -3.660369709873901*Au;
    Planets(6,5) = -3.976471696421420*Au;
   
    Planets(7,1) = 5.683*10^26;
    Planets(7,2) = 6.695021469360583*10^(-1)*Au;
    Planets(7,3) = -1.003565133768334*10^(1)*Au;
    Planets(7,4) = 6.747618366403029*10^(-1)*Au;
    Planets(7,5) = -1.003529647030772*10^(1)*Au;
    
    Planets(8,1) = 8.681*10^25;
    Planets(8,2) = 1.750396686209020*10^(1)*Au;
    Planets(8,3) = 9.452178152785551*Au;
    Planets(8,4) = 1.750206888419359*10^(1)*Au;
    Planets(8,5) = 9.455455390702040*Au;
    
    Planets(9,1) = 1.024*10^26;
    Planets(9,2) = 2.878325030106766*10^(1)*Au;
    Planets(9,3) = -8.233966511744240*Au;
    Planets(9,4) = 2.878409264186948*10^(1)*Au;
    Planets(9,5) = -8.230929736592461*Au;

dims = size(Bodies);
Pinfo = zeros(dims(2),5);

for i = 1:dims(2)
    Pinfo(i,:) = Planets(Bodies(i),:);
end
    
end

