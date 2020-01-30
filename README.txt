-SLOP- Spacecraft Launch Optimization Program

--Status--
Proof of Concepte in MATLAB finished May 2019
ReOpened for porting to C++ January 2020

--Project Summary--
SLOP is a program which uses a combination of Monte Carlo methods, Verlet Integration, and nonlinear optimization to find initial launch vectors for a spacecraft traveling from Earth to a user specified destination. This project was originally completed in MATLAB as a proof of concept for my 2017 Computational Physics class final. I have recently reopened the project to port it to C++ and to take it from a proof of concept state to a finished product state.

Planned improvements from the original proof of concepte are shown below. This list is subject to change at any time.
1. Dynamic timesteps for Verlet Integration - Completed
2. NASA JPL Horizons Interfacing - In Progress
3. Multithreading the Monte Carlo and Optimization - In Progress
4. GUI - Not Yet Started

--My Contribution--
I wrote all the code showcased in this project. The MATLAB version was completed in approximatley 1 week as my final project for a computational physics class. The C++ version is currently in development.

--Software--
This folder contains all the code I have written for this project. The C++ folder contains the current version of this program which is being activley developed. The MATLAB folder contains the original proof of concept version of this program. This version of the program is technically functionally but is extremely limited in what it can calculate. 