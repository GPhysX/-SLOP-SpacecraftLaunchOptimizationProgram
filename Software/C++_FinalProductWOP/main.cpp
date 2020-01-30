#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>


using namespace std;

class Planet {
    public:
        string name;
        double mass;
        double posx;
        double posy;
        double velx;
        double vely;

        Planet(string aname, double amass, double aposx, double aposy, double avelx, double avely){
            name = aname;
            mass = amass;
            posx = aposx;
            posy = aposy;
            velx = avelx;
            vely = avely;
        }

        Planet(){
            name = "UnNamed";
            mass = 0;
            posx = 0;
            posy = 0;
            velx = 0;
            vely = 0;

            cout << "Default planet created with all numerical data set to 0" << endl;
        }


};

class Star {
    public:
        string name;
        double mass;
        double posx;
        double posy;
        double velx;
        double vely;

        Star(string aname, double amass, double aposx, double aposy, double avelx, double avely){
            name = aname;
            mass = amass;
            posx = aposx;
            posy = aposy;
            velx = avelx;
            vely = avely;
        }

        Star(){
            name = "UnNamed";
            mass = 0;
            posx = 0;
            posy = 0;
            velx = 0;
            vely = 0;

            cout << "Default Star created with all numerical data set to 0" << endl;
        }


};

class Body {
    public:
        string name;
        double mass;
        double posx;
        double posy;
        double velx;
        double vely;

        Body(string aname, double amass, double aposx, double aposy, double avelx, double avely){
            name = aname;
            mass = amass;
            posx = aposx;
            posy = aposy;
            velx = avelx;
            vely = avely;
        }

        Body(){
            name = "UnNamed";
            mass = 0;
            posx = 0;
            posy = 0;
            velx = 0;
            vely = 0;

            cout << "Default celestial body created with all numerical data set to 0" << endl;
        }


};

class SpaceShip {
    public:
        double velx;
        double vely;
        double posx;
        double posy;

        SpaceShip(double avelx, double avely, double aposx, double aposy) {
            velx = avelx;
            vely = avely;
            posx = aposx;
            posy = aposy;
        }

        SpaceShip() {
            velx = 0;
            vely = 0;
            posx = 0;
            posy = 0;
        }
};

class OutCoords {
    public:
        string objectname;
        vector<double> x;
        vector<double> y;
        vector<double> t;

        OutCoords() {
        }

        OutCoords(string newname) {
            objectname = newname;
        }

        void update(double newx, double newy, double newt) {
            x.push_back(newx);
            y.push_back(newy);
            t.push_back(newt);
        }
};

double updateSystem (SpaceShip &oldShip, vector<Planet> &oldPlanets, vector<Star> &oldStars, vector<Body> &oldBodies, double timestep) {
    // This function will update the state variables of all the large bodies in the stellar system.
    // First make a copy of all the vector inputs
    double G = 6.674*pow(10,-11);
    vector<Planet> PlanetsC = oldPlanets;
    vector<Star> StarsC = oldStars;
    vector<Body> BodiesC = oldBodies;

    int numPlanets = oldPlanets.size();
    int numStars = oldStars.size();
    int numBodies = oldBodies.size();
    double accelX = 0;
    double accelY = 0;
    // Variables for use in calculating the acceleration vector of each body.
    double dirx, diry, magdir, accelMag, dist, distmin;

    // First I'll update the ships position and calculate the dynamic time step.
    // Find the minimum distance between the ship and the massive bodies
    for (int p =  0; p < numPlanets; p++) {
        dist = sqrt(pow(oldShip.posx - PlanetsC[p].posx,2) + pow(oldShip.posy - PlanetsC[p].posy,2));
        if (dist < distmin || p == 1) {
            distmin = dist;
        }
    }
    for (int s = 0; s < numStars; s++) {
        dist = sqrt(pow(oldShip.posx - StarsC[s].posx,2) + pow(oldShip.posy - StarsC[s].posx,2));
        if (dist < distmin) {
            distmin = dist;
        }
    }
    for (int b = 0; b < numBodies; b++) {
        dist = sqrt(pow(oldShip.posx - BodiesC[b].posx,2) + pow(oldShip.posy - BodiesC[b].posy,2));
        if (dist < distmin) {
            distmin = dist;
        }
    }

    // Now dynamically adjust the time step so that we can get accurate slingshots
    double dthresh = 1.496*pow(10,10); // minimum distance for time step adjustments to occur
    if (distmin < dthresh) {
        timestep = timestep*(pow(distmin,2)/pow(dthresh,2));
    }

    // Next update the position of the ship
    double oldpx = oldShip.posx; // Save these for updating velocity
    double oldpy = oldShip.posy;



    // Account for all planets, stars, and other bodies in the net acceleration of the spacecraft.
    for (int p = 0; p < numPlanets; p++) {
        // Generate unit vector from craft to planet
        dirx = oldPlanets[p].posx - oldpx;
        diry = oldPlanets[p].posy - oldpy;
        magdir = sqrt(pow(dirx,2)+pow(diry,2));
        dirx = dirx/magdir;
        diry = diry/magdir;
        // Calculate magnitude of acceleration and multiply by unit vector
        accelMag = G*oldPlanets[p].mass/pow(magdir,2);
        accelX += accelMag*dirx;
        accelY += accelMag*diry;
    }
    for (int s = 0; s < numStars; s++) {
        // Generate unit vector from craft to planet
        dirx = oldStars[s].posx - oldpx;
        diry = oldStars[s].posy - oldpy;
        magdir = sqrt(pow(dirx,2)+pow(diry,2));
        dirx = dirx/magdir;
        diry = diry/magdir;
        // Calculate magnitude of acceleration and multiply by unit vector
        accelMag = G*oldStars[s].mass/pow(magdir,2);
        accelX += accelMag*dirx;
        accelY += accelMag*diry;
    }
    for (int b = 0; b < numBodies; b++) {
        // Generate unit vector from craft to planet
        dirx = oldBodies[b].posx - oldpx;
        diry = oldBodies[b].posy - oldpy;
        magdir = sqrt(pow(dirx,2)+pow(diry,2));
        dirx = dirx/magdir;
        diry = diry/magdir;
        // Calculate magnitude of acceleration and multiply by unit vector
        accelMag = G*oldBodies[b].mass/pow(magdir,2);
        accelX += accelMag*dirx;
        accelY += accelMag*diry;
    }

    // Now that we have an acceleration and initial velocity vector we can use Verlet methods to get the next position and velocity
    oldShip.posx += oldShip.velx*timestep + accelX*pow(timestep,2);
    oldShip.posy += oldShip.vely*timestep + accelY*pow(timestep,2);
    oldShip.velx += accelX*timestep;
    oldShip.vely += accelY*timestep;

    // First update all planets
    for (int p = 0; p < numPlanets; p++) {
        accelX = 0; // Reset variables for each planets state variable update.
        accelY = 0;
        dirx = 0;
        diry = 0;
        magdir = 0;
        accelMag = 0;
        for (int altp = 0; altp < numPlanets; altp++) {
            if (altp != p) {
                // Get unit vector pointing from initial planet to gravitational source planet
                dirx = PlanetsC[altp].posx - PlanetsC[p].posx;
                diry = PlanetsC[altp].posy - PlanetsC[p].posy;
                magdir = sqrt(pow(dirx,2) + pow(diry,2));
                dirx = dirx/magdir;
                diry = diry/magdir;
                // Calculate the resulting acceleration.
                accelMag = G*PlanetsC[altp].mass/pow(magdir,2);
                accelX += accelMag*dirx;
                accelY += accelMag*diry;
            }
        }
        for (int alts = 0; alts < numStars; alts++) {
            dirx = StarsC[alts].posx - PlanetsC[p].posx;
            diry = StarsC[alts].posy - PlanetsC[p].posy;
            magdir = sqrt(pow(dirx,2) + pow(diry,2));
            dirx = dirx/magdir;
            diry = diry/magdir;
            // Calculate the resulting acceleration.
            accelMag = G*StarsC[alts].mass/pow(magdir,2);
            accelX += accelMag*dirx;
            accelY += accelMag*diry;
        }
        for (int altb = 0; altb < numBodies; altb++) {
            dirx = BodiesC[altb].posx - PlanetsC[p].posx;
            diry = BodiesC[altb].posy - PlanetsC[p].posy;
            magdir = sqrt(pow(dirx,2) + pow(diry,2));
            dirx = dirx/magdir;
            diry = diry/magdir;
            // Calculate the resulting acceleration.
            accelMag = G*BodiesC[altb].mass/pow(magdir,2);
            accelX += accelMag*dirx;
            accelY += accelMag*diry;
        }

        // Now use the total acceleration experienced by planet p to update its state variables
        oldPlanets[p].posx += oldPlanets[p].velx*timestep + accelX*pow(timestep,2);
        oldPlanets[p].posy += oldPlanets[p].vely*timestep + accelY*pow(timestep,2);
        oldPlanets[p].velx += accelX*timestep;
        oldPlanets[p].vely += accelY*timestep;
    }
    // Now update all Stars
    for (int s = 0; s < numStars; s++) {
        accelX = 0; // Reset variables for each planets state variable update.
        accelY = 0;
        dirx = 0;
        diry = 0;
        magdir = 0;
        accelMag = 0;
        for (int alts = 0; alts < numStars; alts++) {
            if (alts != s) {
                // Get unit vector pointing from initial planet to gravitational source planet
                dirx = StarsC[alts].posx - StarsC[s].posx;
                diry = StarsC[alts].posy - StarsC[s].posy;
                magdir = sqrt(pow(dirx,2) + pow(diry,2));
                dirx = dirx/magdir;
                diry = diry/magdir;
                // Calculate the resulting acceleration.
                accelMag = G*StarsC[alts].mass/pow(magdir,2);
                accelX += accelMag*dirx;
                accelY += accelMag*diry;
            }
        }
        for (int altp = 0; altp < numPlanets; altp++) {
            dirx = PlanetsC[altp].posx - StarsC[s].posx;
            diry = PlanetsC[altp].posy - StarsC[s].posy;
            magdir = sqrt(pow(dirx,2) + pow(diry,2));
            dirx = dirx/magdir;
            diry = diry/magdir;
            // Calculate the resulting acceleration.
            accelMag = G*PlanetsC[altp].mass/pow(magdir,2);
            accelX += accelMag*dirx;
            accelY += accelMag*diry;
        }
        for (int altb = 0; altb < numBodies; altb++) {
            dirx = BodiesC[altb].posx - StarsC[s].posx;
            diry = BodiesC[altb].posy - StarsC[s].posy;
            magdir = sqrt(pow(dirx,2) + pow(diry,2));
            dirx = dirx/magdir;
            diry = diry/magdir;
            // Calculate the resulting acceleration.
            accelMag = G*BodiesC[altb].mass/pow(magdir,2);
            accelX += accelMag*dirx;
            accelY += accelMag*diry;
        }

        // Now use the total acceleration experienced by planet p to update its state variables
        oldStars[s].posx += oldStars[s].velx*timestep + accelX*pow(timestep,2);
        oldStars[s].posy += oldStars[s].vely*timestep + accelY*pow(timestep,2);
        //cout << "Updating Star position to" << oldStars[s].posx << "  " << oldStars[s].posy <<endl;
        oldStars[s].velx += accelX*timestep;
        oldStars[s].vely += accelY*timestep;

    }
    // Last update the other bodies in the system
    for (int b = 0; b < numBodies; b++) {
        accelX = 0; // Reset variables for each planets state variable update.
        accelY = 0;
        dirx = 0;
        diry = 0;
        magdir = 0;
        accelMag = 0;
        for (int altb = 0; altb < numBodies; altb++) {
            if (altb != b) {
                // Get unit vector pointing from initial planet to gravitational source planet
                dirx = BodiesC[altb].posx - BodiesC[b].posx;
                diry = BodiesC[altb].posy - BodiesC[b].posy;
                magdir = sqrt(pow(dirx,2) + pow(diry,2));
                dirx = dirx/magdir;
                diry = diry/magdir;
                // Calculate the resulting acceleration.
                accelMag = G*BodiesC[altb].mass/pow(magdir,2);
                accelX += accelMag*dirx;
                accelY += accelMag*diry;
            }
        }
        for (int altp = 0; altp < numPlanets; altp++) {
            dirx = PlanetsC[altp].posx - BodiesC[b].posx;
            diry = PlanetsC[altp].posy - BodiesC[b].posy;
            magdir = sqrt(pow(dirx,2) + pow(diry,2));
            dirx = dirx/magdir;
            diry = diry/magdir;
            // Calculate the resulting acceleration.
            accelMag = G*PlanetsC[altp].mass/pow(magdir,2);
            accelX += accelMag*dirx;
            accelY += accelMag*diry;
        }
        for (int alts = 0; alts < numStars; alts++) {
            dirx = StarsC[alts].posx - BodiesC[b].posx;
            diry = StarsC[alts].posy - BodiesC[b].posy;
            magdir = sqrt(pow(dirx,2) + pow(diry,2));
            dirx = dirx/magdir;
            diry = diry/magdir;
            // Calculate the resulting acceleration.
            accelMag = G*StarsC[alts].mass/pow(magdir,2);
            accelX += accelMag*dirx;
            accelY += accelMag*diry;
        }

        // Now use the total acceleration experienced by planet p to update its state variables
        oldBodies[b].posx += oldBodies[b].velx*timestep + accelX*pow(timestep,2);
        oldBodies[b].posy += oldBodies[b].vely*timestep + accelY*pow(timestep,2);
        oldBodies[b].velx += accelX*timestep;
        oldBodies[b].vely += accelY*timestep;
    }

    return timestep;
}

int main()
{

// Define some useful constants in SI units
double Au = 1.496*pow(10,11);
double LaunchDist = 40000000;
double G = 6.674*pow(10,-11);
double sperd = 86400;

// First get the desired launch date from the user. This will be used to load planetary bodies data from NASA JPL Horizons.

string launchDate; // this may need to be converted to a different datatype depending on how Horizons is called.
cout << "Enter a desired launch date mm-dd-yyyy:";
cin >> launchDate;
// TODO: Add functionality to check for valid input

string destination;
cout << "Enter desired destination planet name (Mercury, Venus, Mars, etc). Note that all journeys start from Earth by default." << endl;
cout << "Destination Planet:";
cin >> destination;
//TODO: Add functionality to check for valid input



/*  Load the 8 planets and suns data into the appropriate objects
    Eventually I would like to use the desired launch date to connect to JPL Horizons and automatically fill in the info for the 8 planets and the Sun.
    For now I have hardcoded starting values in so that I can get the rest of the program functioning.
*/

vector<Planet> Planets;
vector<Star> Stars;
vector<Body> Bodies;
//default values based on 1/8/2020 positions and velocities. In the future automate this to pull for NASA JPL Horizons based on user inputed launch date.
//Automating this may be easier to do by calling a Python script
Stars.push_back(Star("Sun", 1.988544*pow(10,30), -3.857022300275326*Au*pow(10,-3), 7.425738921434983*Au*pow(10,-3), -8.334249208811721*Au*pow(10,-6)/sperd, -2.062229084310973*Au*pow(10,-6)/sperd));
Planets.push_back(Planet("Mercury", 3.302*pow(10,23), 8.917337664647064*Au*pow(10,-2), -4.367636406985999*Au*pow(10,-1), 2.188614729211309*Au*pow(10,-2)/sperd, 7.204663284214451*Au*pow(10,-3)/sperd));
Planets.push_back(Planet("Venus", 4.8685*pow(10,24), 6.949223033944875*Au*pow(10,-1), 1.986644384095169*Au*pow(10,-1), -5.418586678814747*Au*pow(10,-3)/sperd, 1.941677223635651*Au*pow(10,-2)/sperd));
Planets.push_back(Planet("Earth", 5.972*pow(10,24), -2.892698924363897*Au*pow(10,-1), 9.483553307419959*Au*pow(10,-1), -1.674611909353826*Au*pow(10,-2)/sperd, -5.062528512469250*Au*pow(10,-3)/sperd));
Planets.push_back(Planet("Mars", 6.4171*pow(10,23), -1.263360948767393*Au*pow(10,0), -9.496414668352595*Au*pow(10,-1), 8.982566795948855*Au*pow(10,-3)/sperd, -9.946132702161077*Au*pow(10,-3)/sperd));
Planets.push_back(Planet("Jupiter", 1.898*pow(10,27), 5.742338178040277*Au*pow(10,-1), -5.185515997450260*Au*pow(10,0), 7.407727406017702*Au*pow(10,-3)/sperd, 1.190721677448268*Au*pow(10,-3)/sperd));
Planets.push_back(Planet("Saturn", 5.6834*pow(10,26), 3.827400496219513*Au*pow(10,0), -9.265926635229302*Au*pow(10,0), 4.846789169648662*Au*pow(10,-3)/sperd, 2.113030873302228*Au*pow(10,-3)/sperd));
Planets.push_back(Planet("Uranus", 8.6813*pow(10,25), 1.620566358636939*Au*pow(10,1), 1.140771629700922*Au*pow(10,1), -2.292816256332509*Au*pow(10,-3)/sperd, 3.032865898649047*Au*pow(10,-3)/sperd));
Planets.push_back(Planet("Neptune", 1.02413*pow(10,26), 2.924353020234171*Au*pow(10,1), -6.338146601755990*Au*pow(10,0), 6.436374603185884*Au*pow(10,-4)/sperd, 3.086392186822279*Au*pow(10,-3)/sperd));


// Allow the user to add additional celestial bodies to the default 8 planets and sun.
bool addBodies = true;
while(addBodies == true){

    string addmore;
    cout << "Add additional celestial bodies to the simulation (Sun and 8 planets included by default)? type y/n: ";
    cin >> addmore;

    if (addmore == "n" || addmore == "N"){
        cout << "No more bodies to add" << endl;
        break;
    } else {
        string newType;
        cout << "What type of body would you like to add? Type Planet, Star, or Other:"; // Add ability to search New Horrizons database for an object
        cin >> newType;

        // Initialize some new variables to store inputed values for the new body
        string newname;
        double newmass;
        double newlocx;
        double newlocy;
        double newvelx;
        double newvely;

        if (newType == "Planet") {
                cout << "Enter new planet's name:";
                cin >> newname;
                cout << "Enter new planet's mass:";
                cin >> newmass;
                cout << "Enter new planet's initial x coordinate:";
                cin >> newlocx;
                cout << "Enter new planet's initial y coordinate:";
                cin >> newlocy;
                cout << "Enter new planet's initial x velocity:";
                cin >> newvelx;
                cout << "Enter new planet's initial y velocity:";
                cin >> newvely;
                // Need a function to procedurally generate a variable names for planets specifically P1 needs to iterate each time
                Planets.push_back(Planet(newname,newmass,newlocx,newlocy,newvelx,newvely));

        } else if (newType == "Star") {
                cout << "Enter new Star's name:";
                cin >> newname;
                cout << "Enter new Star's mass:";
                cin >> newmass;
                cout << "Enter new Star's initial x coordinate:";
                cin >> newlocx;
                cout << "Enter new Star's initial y coordinate:";
                cin >> newlocy;
                cout << "Enter new Star's initial x velocity:";
                cin >> newvelx;
                cout << "Enter new Star's initial y velocity:";
                cin >> newvely;
                // Need a function to procedurally generate a variable names for planets specifically P1 needs to iterate each time
                Stars.push_back(Star(newname,newmass,newlocx,newlocy,newvelx,newvely));

        } else if (newType == "Other") {
                cout << "Enter new body's name:";
                cin >> newname;
                cout << "Enter new body's mass:";
                cin >> newmass;
                cout << "Enter new body's initial x coordinate:";
                cin >> newlocx;
                cout << "Enter new body's initial y coordinate:";
                cin >> newlocy;
                cout << "Enter new body's initial x velocity:";
                cin >> newvelx;
                cout << "Enter new body's initial y velocity:";
                cin >> newvely;
                // Need a function to procedurally generate a variable names for planets specifically P1 needs to iterate each time
                Bodies.push_back(Body(newname,newmass,newlocx,newlocy,newvelx,newvely));

        } else {
            cout << "Invalid type" << endl;
        }

        }
    }

int numPlanets = Planets.size();
int numStars = Stars.size();
int numBodies = Bodies.size();

cout << "Number of Planets = " << numPlanets << endl;
cout << "Number of Stars = " << numStars << endl;
cout << "Number of other celestial bodies = " << numBodies << endl;

// Now I'll make a copy of all the planets, stars, and celestial bodies initial positions.



/*  We now have all the celestial bodies we need to account for when finding spacecraft launch parameters.
    Now we need to get some information from the user about the spacecraft.
    A fully fleshed out program would have far more options.
*/

double maxlaunchVel;
cout << "What is the maximum velocity your spacecraft can attain after leaving Earths gravitational influence (m/s) : ";
cin >> maxlaunchVel;


// Lets identify which of the planets we are trying to get to
int destID;
for (int i=0; i<numPlanets; i++) {
    if (Planets[i].name == destination) {
        destID = i;
        break;
    }
}
cout<< "Beginning nonlinear optimization to find launch parameters for a trip to " << Planets[destID].name << " with a launch date of " <<launchDate << endl;

// NEED TO IMPLIMENT MULTITHREADING STARTING HERE
// Initialize our spacecraft
// First generate a starting guess for velocity based on user inputs.

// Choose direction based on whether our destination is closer, or further away from the sun than Earth.
vector<double> vel0dir;
double EarthVelMag = sqrt(pow(Planets[2].velx,2) + pow(Planets[2].vely,2));
if (destID < 2) {
   vel0dir.push_back(-Planets[2].velx/EarthVelMag);
   vel0dir.push_back(-Planets[2].vely/EarthVelMag);
} else {
   vel0dir.push_back(Planets[2].velx/EarthVelMag);
   vel0dir.push_back(Planets[2].vely/EarthVelMag);
}


SpaceShip Craft(5000,5000,1,1);

// Normalize initial direction and multiply by typical Earth orbit altitude
double NormFactor = sqrt(pow(Craft.posx,2)*pow(Craft.posy,2));
Craft.posx = (Craft.posx/NormFactor)*LaunchDist + Planets[2].posx;
Craft.posy = (Craft.posy/NormFactor)*LaunchDist + Planets[2].posy;
Craft.velx = Planets[2].velx + vel0dir[0]*maxlaunchVel;
Craft.vely = Planets[2].vely + vel0dir[1]*maxlaunchVel;


//Initialize some variables, some of these should be user inputs in future versions
bool destinationReached = false;
double maxDurationTrip = pow(10,8);
double tripDuration = 0;
double defaultTimestep = 86400; // Use 1 day timestep as default




// Prepare output file for writing output too
std::ofstream outfile ("Output.txt");

for (int i=0; i < numPlanets; i++) {
    //outfile << Planets[i].name << "_X    ";
    //outfile << Planets[i].name << "_Y    ";
    outfile << Planets[i].posx << "    ";
    outfile << Planets[i].posy << "    ";
}
for (int i=0; i < numStars; i++) {
    //outfile << Stars[i].name << "_X    ";
    //outfile << Stars[i].name << "_Y    ";
    outfile << Stars[i].posx << "    ";
    outfile << Stars[i].posy << "    ";
}
for (int i=0; i < numBodies; i++) {
    //outfile << Bodies[i].name << "_X    ";
    //outfile << Bodies[i].name << "_Y    ";
    outfile << Bodies[i].posx << "    ";
    outfile << Bodies[i].posy << "    ";
}

//outfile << "Time" << std::endl;
outfile << Craft.posx << "    ";
outfile << Craft.posy << "    ";
outfile << tripDuration << std::endl;

double dynamicTime;


while (destinationReached == false && tripDuration < maxDurationTrip) {
    // Update the system until we either complete our journey successfully, or we exceed the maximum allowed trip duration.
    dynamicTime = updateSystem(Craft, Planets, Stars, Bodies, defaultTimestep);
    // We need to check to see if we've reached our destination.


    // Write trip coordinates to output file for animation
    for (int i=0; i < numPlanets; i++) {
        outfile << Planets[i].posx << "    ";
        outfile << Planets[i].posy << "    ";
    }
    for (int i=0; i < numStars; i++) {
        outfile << Stars[i].posx << "    ";
        outfile << Stars[i].posy << "    ";
    }
    for (int i=0; i < numBodies; i++) {
        outfile << Bodies[i].posx << "    ";
        outfile << Bodies[i].posy << "    ";
    }
    outfile << Craft.posx << "    ";
    outfile << Craft.posy << "    ";

    tripDuration += dynamicTime;
    outfile << tripDuration << std::endl;
}


}




